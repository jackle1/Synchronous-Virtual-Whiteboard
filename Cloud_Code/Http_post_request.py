import boto3
import os
from io import StringIO
import json
import random
import csv

bucket_name = 'cpen391'
file_key = '/tmp/roomID_8862.csv'
s3 = boto3.client('s3')

# Retrieve the CSV file from S3
s3_object = s3.get_object(Bucket=bucket_name, Key=file_key)
csv_content = s3_object['Body'].read().decode('utf-8')

# Process the CSV data
csv_reader = csv.reader(StringIO(csv_content))
rows = []
for row in csv_reader:
    # Convert the row to integers and append it to the list
    int_row = [int(x) for x in row]
    rows.append(int_row)


# This is the dimension of the array m x n pixels
m = 10
n = 10

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

def update_pixels(table, data, member, RGB, x, y):
    # If the password also match that means, that we can update the pixels
    # Before that, lets check if the member is actually inside this room
    # if member not in data["members"]:
    #     return wrong_arguments("You are not a member in this room, first please sign in!")
                
    # First getting the pixel
    # RGB_table = data["RGB"]

    if type(RGB) == list:
        for i in range(len(RGB)):
            rows[x[i]][y[i]] = RGB[i]

                       
    else:
        # Updating the local copy of the pixels
        rows[x][y] = RGB
                
    # I am updating the pixels here
    table.put_item(
        Item={
                'room_id': data["room_id"],
                'room_password': data["room_password"],
                'members': data["members"]
            }
    )

    # Putting the change values back to the S3 bucket
    putting_it_back()

    return {
                'stausCode': 200,
                'message': ("The server has been updated!"),
                'members': (list(data["members"]))
    }

def create_new_room(table, member):
    print(member)
    tmp = table.scan()
    room_id = -1
    for data in tmp["Items"]:
        if room_id < data['room_id']:
            room_id = data['room_id']
        
    # Now, we have the latest room_id, so to make a new one with do ++
    room_id += 1

    # Therefore, a new_room_id is room_id
    # Setting up a new passsword
    password = random.randrange(1000, 10000)
    members = {member}

    # # Making a new Array of size m and n
    # RBG = []
    # tmp = []
    # for i in range(n):
    #     tmp.append(101010)
    # for i in range(m):
    #     RBG.append(tmp)
    

    # Creating and putting this new room
    table.put_item(
            Item={
                'room_id': room_id,
                'members': members,
                'room_password': password
            }
    )

    response = {}
    response['StatusCode'] = 200
    response['RoomID'] = password
    response['members'] = list(members)

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
    
def putting_it_back():

    # Create a CSV string from the 2D array
    csv_string = ''
    for row in rows:
        csv_string += ','.join(map(str, row)) + '\n'

    # Overwrite the existing CSV file in the S3 bucket with the updated CSV data
    s3 = boto3.resource('s3')
    bucket_name = 'cpen391'
    key = '/tmp/roomID_8862.csv'

    s3_object = s3.Object(bucket_name, key)
    s3_object.put(Body=csv_string)

    # s3_object = s3.Object(bucket_name, file_key)
    s3_object.put(Body=csv_string)



if __name__ == "__main__":
    os.environ["TABLE_NAME"] = "Cpen391"
    test_event = {
                  "member": "Ranbir",
                  "roomID": 8862,
                  'RGB': 121212,
                  "request-for": 1,
                  "x": 1,
                  "y": 1
                }
    result = lambda_handler(test_event, None)
    print(result)
    