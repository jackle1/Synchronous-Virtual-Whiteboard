import json
import urllib3
import boto3
import random
import os
from io import StringIO
import csv

client = boto3.client('apigatewaymanagementapi', endpoint_url="https://7nbl97eho0.execute-api.us-east-1.amazonaws.com/production")



# Preparing the dynamoDB
dynamodb = boto3.resource("dynamodb", region_name='us-east-1')
table_name = 'Cpen391'
table = dynamodb.Table(table_name)


# This is the dimension of the array m x n pixels
height = 480
width = 640



def lambda_handler(event, context):
    global rows

    # First get the values from the context
    connectionId = event['requestContext']['connectionId']
    routeKey = event['requestContext']['routeKey']

    try:
        
        if(routeKey == 'post'):
            values1 = event["body"]
            values1 = json.loads(values1)
            roomID = values1['roomID']
            member = values1['user']
            RGB = values1['RGB']
            x = values1['x']
            y = values1['y']
            data = find_table(roomID)
            # Find the data in DynamoDB table
            if data == None:
                message = "No such RoomID exists! Try again" 
                client.post_to_connection(ConnectionId=connectionId, Data=json.dumps(message).encode('utf-8'))
                return {"statusCode": 200}
            
            members = data['members']
            # Finding if the user is a part of the member list, if not its best to sign in!
            if members == None or member not in members:
                message = "You are not a member in this RoomID, first connect: do action: connect_to_roomID" 
                client.post_to_connection(ConnectionId=connectionId, Data=json.dumps(message).encode('utf-8'))
                return {"statusCode": 200}

            try:
                update_pixels(RGB,x,y)
                putting_it_back()
            except Exception as error:
                    message = f"Something is wrong: {error}"
                    client.post_to_connection(ConnectionId=connectionId, Data=json.dumps(message).encode('utf-8'))
                    return {"statusCode": 200}
            

            response = {
                "RGB": RGB,
                "x": x,
                "y": y,
                "members": list(members.keys())
            }
            send_to_de1(members, data_structuring(RGB, x, y), connectionId)
            send_to_all(members, response, connectionId)

            message = "Server and the clients are updated"
            client.post_to_connection(ConnectionId=connectionId, Data=json.dumps(message).encode('utf-8'))
            return {"statusCode": 200}

        elif routeKey == 'connect_to_roomID':
            values1 = event["body"]
            values1 = json.loads(values1)
            roomID = values1['roomID']
            member = values1['user']
            data = find_table(roomID)
            if data == None:
                message = "No such RoomID exists! Try again" 
                client.post_to_connection(ConnectionId=connectionId, Data=json.dumps(message).encode('utf-8'))
                return {"statusCode": 200}
            
            # Right now we have the right data
            members = {}
            if data['members'] != None:
                members = data['members']

            members[member] = connectionId
            message = f"You are now connected to this RoomID: {roomID} "
            client.post_to_connection(ConnectionId=connectionId, Data=json.dumps(message).encode('utf-8'))
            put_back(members, data)
            response = {
                "members": list(members.keys())
            }
            send_to_all(members, response, member)
            return {"statusCode": 200}
        
        elif (routeKey == 'disconnect_roomID'):
            values1 = event["body"]
            values1 = json.loads(values1)
            roomID = values1['roomID']
            member = values1['user']
            # Finding the roomID from the table
            data = find_table(roomID)
            if data == None:
                message = "No such RoomID exists! Try again" 
                client.post_to_connection(ConnectionId=connectionId, Data=json.dumps(message).encode('utf-8'))
                return {"statusCode": 200}
            members = data['members']
            members.pop(member) 
            if len(members) == 0:
                members = None
            message = f"Disconnected from RoomID: {roomID}!"
            client.post_to_connection(ConnectionId=connectionId, Data=json.dumps(message).encode('utf-8'))
            if members == None:
                response = {
                    "members": members
                }
            else:
                response = {
                "members": list(members.keys())
            }
            send_to_all(members, response, connectionId)
            put_back(members, data)
            return {"statusCode": 200}
        
        elif (routeKey == 'postDone'):
            values1 = event["body"]
            values1 = json.loads(values1)
            roomID = values1['roomID']
            member = values1['user']
            # Finding the data from the roomID
            data = find_table(roomID)
            if data == None:
                message = "No such RoomID exists! Try again" 
                client.post_to_connection(ConnectionId=connectionId, Data=json.dumps(message).encode('utf-8'))
                return {"statusCode": 200}
            
            members = data['members']

            # Sending messages to every client to do a whole GET request
            response = "Please do a whole GET Request"
            send_to_all(members, response, connectionId)
            send_to_de1(members, response, connectionId)
            message = "Message send to all connections with this RoomID"
            client.post_to_connection(ConnectionId=connectionId, Data=json.dumps(message).encode('utf-8'))

            return {"statusCode": 200}
        
        else:
            return {"statusCode": 200}
        
    except Exception as error:
        message = f"Error happened at the server side : {error}"
        client.post_to_connection(ConnectionId=connectionId, Data=json.dumps(message).encode('utf-8'))
        raise error
        return { "statusCode": 200  }

        
    # client.post_to_connection(ConnectionId=connectionId, Data=json.dumps(event).encode('utf-8'))


def find_table(roomID):
      # Finding the room with the password as given in the request header
    tables = table.scan()
    

    # Finding if the password matches
    for data in tables["Items"]:
        if data["room_password"] == roomID:
            # Found the room with the roomID
            return data
    
    return None

def put_back(members, data):
    table.put_item(
            Item={
                'room_id': data['room_id'],
                'members': members,
                'room_password': data["room_password"]
            }
    )


def putting_it_back():

    global rows

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


def update_pixels(RGB, x, y):
    # If the password also match that means, that we can update the pixels
    # Before that, lets check if the member is actually inside this room
    # if member not in data["members"]:
    #     return wrong_arguments("You are not a member in this room, first please sign in!")
                
    # First getting the pixel [0, 1, 2, 3, ...]
    # RGB_table = data["RGB"] [0, 0, 0, 0, ...]

    global rows

    if type(RGB) == list:
        for i in range(len(RGB)):
            if not(0 <= y[i] and y[i] < height):
                raise ValueError("Height dimensions are wrong")
            if not(0 <= x[i] and x[i] < width):
                raise ValueError("Width dimensions are wrong")
            rows[y[i]][x[i]] = RGB[i]

                       
    else:
        # Updating the local copy of the pixels
        if not(0 <= y and y < height):
                raise ValueError("Height dimensions are wrong")
        if not(0 <= x and x < width):
                raise ValueError("Width dimensions are wrong")
        rows[x][y] = RGB

    
def send_to_all(members, message, connectionID):
    name = "De1"
    if members == None:
        return 
    if len(members) == 0:
        return
    for key, value in members.items():
        if value == connectionID:
            continue
        if key[:3] == name:
            # We know this is De1Soc 
            continue
        try:
            client.post_to_connection(ConnectionId=value, Data=json.dumps(message).encode('utf-8'))
        except Exception as error:
            print(f"Happened in Client send sent {error}")
# {"action": "connect_to_roomID", "roomID": 8862, "user": "Demo"}
# {"action": "disconnect_roomID", "roomID": 8862, "user": "Demo"}
# {"action": "post", "roomID": 8862, "user": "Demo", "RGB": {0}, "x": {0}, "y": {0}}
def send_to_de1(members, data, connectionID):
    name = "De1"

    if members == None:
        return 
    if len(members) == 0:
        return
    for key, value in members.items():
        if value == connectionID:
            continue
        if key[:3] != name:
            continue
                    
        for tmp in data:
            try:
                client.post_to_connection(ConnectionId=value, Data=json.dumps(tmp).encode('utf-8'))
            except:
                print(f"Error happend at De1SoC")
                    
    
def padding(value: str, pad: int):
    needed_pad = pad - len(value)
    padd_done = ""
    for i in range(needed_pad):
        padd_done = padd_done + '0'
    return padd_done + value

def data_structuring(RGB, x, y):
    data = []      
    for i in range(len(RGB)):
        value_RGB = padding(str(RGB[i]), 10)
        value_x = padding(str(x[i]), 3)
        value_y = padding(str(y[i]), 3)
                        
        result = value_RGB + ',' + value_x + ',' + value_y
        data.append(result)
    
    return data

def finding_data(roomID):
    # Defining the storage
    bucket_name = 'cpen391'
    file_key = f'/tmp/roomID_{roomID}.csv'
    s3 = boto3.client('s3')

    # Retrieve the CSV file from S3
    try:
        s3_object = s3.get_object(Bucket=bucket_name, Key=file_key)
    except:
        return -1

    csv_content = s3_object['Body'].read().decode('utf-8')

    # Process the CSV data
    csv_reader = csv.reader(StringIO(csv_content))
    rows = []
    for row in csv_reader:
        # Convert the row to integers and append it to the list
        int_row = [int(x) for x in row]
        rows.append(int_row)
    pass



