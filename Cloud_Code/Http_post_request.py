import boto3
import os
from io import StringIO
import json
import random
import csv

m = 480
n = 640
# Define the data you want to write to the CSV file
data = []
for i in range(m):
    tmp = []
    for j in range(n):
        tmp.append(16777215)
    data.append(tmp)

def lambda_handler(event: any, context: any):

    # if checking(event) == -1:
    #     return wrong_arguments("The Parameters given to the request are wrong. Please follow the type or the number of parameters required!")

    room_password = event['roomID']
    member = event['member']
    RGB = event['RGB']
    x = event['x']
    y = event['y']



    # Over here request-for means 0 : Create a new room and 1 : Update the pixels for a existing room and 2 : User wants to sign out
    request_for = event["request-for"]

    # Prepare the DynamoDB client
    dynamodb = boto3.resource("dynamodb", region_name='us-east-1')
    table_name = os.environ["TABLE_NAME"]
    table = dynamodb.Table(table_name)


   # Finding the room with the password as given in the request header
    tables = table.scan()
    
    if(request_for == 0):
        return create_new_room(table, member)

    # Finding if the password matches
    for data in tables["Items"]:
        if data["room_password"] == room_password:
            # Found the room with the roomID

            # This is for updating the pixels
            if request_for == 1:
                return update_pixels(table, data, member, RGB, x, y)
            
            # This is for signing out
            else:
                return sign_out(table, data, member)
    
    # At this time, we know there doesnt exists any room with the given room_password
    return wrong_arguments("No such Room exits with the given RoomID")


def sign_out(table, data, member):

    # This case is where member want to sign out
    response = {
        'statusCode': 200,
        'Message': ("Sign out completed!")
    }

    if member in data["members"]:
        members = data["members"]
        members.remove(member)
        if len(members) == 0:
            members.add("De1-Soc")
        # Update the member list on server
        table.update_item(Key={"room_id": data["room_id"]},
                UpdateExpression = "set members = :newmembers",
                ExpressionAttributeValues = {":newmembers": members},
                ReturnValues="UPDATED_NEW"
        )
    return response

def update_pixels(table, data1, member, RGB, x, y):
    # If the password also match that means, that we can update the pixels
    # Before that, lets check if the member is actually inside this room
    # if member not in data["members"]:
    #     return wrong_arguments("You are not a member in this room, first please sign in!")
                
    # First getting the pixel
    # RGB_table = data["RGB"]
    global data
    # create a shallow copy of the outer list using the copy() method
    rows = data.copy()

    # create a shallow copy of each inner list using a list comprehension
    rows = [inner_list.copy() for inner_list in rows]


    if type(RGB) == list:
        for i in range(len(RGB)):
            rows[y[i]][x[i]] = RGB[i]

                       
    else:
        # Updating the local copy of the pixels
        rows[y][x] = RGB
                
    # # I am updating the pixels here
    # table.put_item(
    #     Item={
    #             'room_id': data["room_id"],
    #             'room_password': data["room_password"],
    #             'members': data["members"]
    #         }
    # )

    # Putting the change values back to the S3 bucket
    putting_it_back(rows, data1['room_id'])

    return {
                'stausCode': 200,
                'message': ("The server has been updated!"),
    }

def create_new_room(table, member):
    tmp = table.scan()
    room_id = -1
    for data in tmp["Items"]:
        if room_id < data['room_id']:
            room_id = data['room_id']
        
    # Now, we have the latest room_id, so to make a new one with do ++
    room_id += 1

    # Therefore, a new_room_id is room_id
    # Setting up a new passsword
    password = random.randrange(100, 1023)
    members = None

    # Creating and putting this new room
    table.put_item(
            Item={
                'room_id': room_id,
                'members': members,
                'room_password': password,
                'lock': 0,
                'lock_acquired': None
            }
    )

    # Putting the csv file in
    new_file(room_id)

    response = {}
    response['StatusCode'] = 200
    response['RoomID'] = password
    return response

def checking(event):
    password = event.get('roomID', None)
    member = event.get('member', None)
    R_values = event.get("R-values", None)
    G_values = event.get("G-values", None)
    B_values = event.get("B-values", None)
    request_for = event.get("request-for", None)
    x = event.get('x', None)
    y = event.get('y', None)
    
    if password == None or member == None or R_values == None or G_values == None or B_values == None or request_for == None or x == None or y == None :
        return -1

    if type(password) != int or type(member) != str or type(request_for) != int : #type(R_values) != int or type(G_values) != int or type(B_values) != int or 
        return -1

    if request_for != 0 and request_for != 1 and request_for != 2:
        return -1
    
    return 1

def wrong_arguments(message):
    return{
            "statusCode": 404,
            "error" : message
        }
    
def putting_it_back(rows, roomID):

    # Create a CSV string from the 2D array
    csv_string = ''
    for row in rows:
        csv_string += ','.join(map(str, row)) + '\n'

    # Overwrite the existing CSV file in the S3 bucket with the updated CSV data
    s3 = boto3.resource('s3')
    bucket_name = 'cpen391'
    key = f'/tmp/room_index_{roomID}.csv'

    s3_object = s3.Object(bucket_name, key)
    s3_object.put(Body=csv_string)

    # s3_object = s3.Object(bucket_name, file_key)
    s3_object.put(Body=csv_string)

def new_file(roomID):
    global data
    # Define the S3 bucket name and CSV file name
    bucket_name = 'cpen391'
    file_name = f'/tmp/room_index_{roomID}.csv' 

    # Write the data to the CSV file
    with open(file_name, 'w', newline='') as file:
        writer = csv.writer(file)
        writer.writerows(data)

    # Upload the CSV file to S3
    s3 = boto3.resource('s3')
    bucket = s3.Bucket(bucket_name)
    bucket.upload_file(file_name, file_name)

def getting_things(roomID):

    bucket_name = 'cpen391'
    file_key = f'/tmp/room_index_{roomID}.csv'
    s3 = boto3.client('s3')

    # Retrieve the CSV file from S3
    s3_object = s3.get_object(Bucket=bucket_name, Key=file_key)
    csv_content = s3_object['Body'].read().decode('utf-8')

    # Process the CSV data
    csv_reader = csv.reader(StringIO(csv_content))
    rows = []
    global_room_index = roomID
    for row in csv_reader:
        # Convert the row to integers and append it to the list
        int_row = [int(x) for x in row]
        rows.append(int_row)
    return rows
        

# if __name__ == "__main__":
#     os.environ["TABLE_NAME"] = "Cpen391"
#     RGB = []
#     x = []
#     y = []
#     # for i in range(m):
#     #     for j in range(n):
#     #         RGB.append(16777215)
#     #         x.append(i)
#     #         y.append(j)
#     test_event = {
#                   "member": "Ranbir",
#                   "roomID": 1014,
#                   'RGB': RGB,
#                   "request-for": 0,
#                   "x": y,
#                   "y": x
#                 }
#     result = lambda_handler(test_event, None)
#     print(result)
    