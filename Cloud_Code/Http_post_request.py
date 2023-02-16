import boto3
import os
import json
import random

'''
Hello, this multi-line comment is for my fellow team mates on what to send for the database on the server
This is the file for Http POST request
Make the POST request on https://hbzwo0rl65.execute-api.us-east-1.amazonaws.com/dev/cpen391
Follow the request body as follows
    test_event = {
                "member": "local_pixegami",
                  "roomID": 0,
                  "password": 1234,
                  "R-values" : [2222],
                  "G-values" : [1,2],
                  "B-values" : [1,2],
                  "request-for": 2
                }
Even if you dont have the information yet, for example, in creating a new room. Leave the field as blank. However, please do
it in the required datatype
For example, 'roomID' = -1 or 'password' = "" or 'R-values' = []
If the above rule is not followed, then you will recieve an exception 
Another thing to note is that use the different values of 'request-for' for different behaviour
request-for means 
0 : Create a new room and 1 : Update the pixels for a existing room and 2 : User wants to sign out

'''


def lambda_handler(event: any, context: any):

    if checking(event) == -1:
        return wrong_arguments("The Parameters given to the request are wrong. Please follow the type or the number of parameters required!")

    room_password = event['password']
    roomID = event['roomID']
    member = event['member']
    R_values = event["R-values"]
    G_values = event["G-values"]
    B_values = event["B-values"]

    # Over here request-for means 0 : Create a new room and 1 : Update the pixels for a existing room and 2 : User wants to sign out
    request_for = event["request-for"]

    # Prepare the DynamoDB client
    dynamodb = boto3.resource("dynamodb", region_name='us-east-1')
    table_name = os.environ["TABLE_NAME"]
    table = dynamodb.Table(table_name)

    if(request_for == 0):
        tmp = table.scan()
        room_id = -1
        for data in tmp["Items"]:
            room_id = data['room_id']
        
        # Now, we have the latest room_id, so to make a new one with do ++
        room_id += 1

        # Therefore, a new_room_id is room_id
        # Setting up a new passsword
        pasword = random.randrange(1000, 10000)
        members = {member}

        # Creating and putting this new room
        table.put_item(
            Item={
                'room_id': room_id,
                'room_password': pasword,
                'members': members,
                'R_values': R_values,
                'G_values': G_values,
                'B_values': B_values
            }
        )

        response = {}
        response['StatusCode'] = 200
        response["new_room_id"] = room_id
        response["new_password"] = pasword
        response['members'] = members

        return response
    
    elif request_for == 1:
        # In this case, we are updating the pixels in the current exisiting 
        # If a room exists then we check if the password match
        response_table = table.get_item(Key={"room_id": roomID})
        response = {}

        if "Item" in response_table:
            password = response_table["Item"]["room_password"]
            if password == room_password:
                # If the password also match that means, that we can update the pixels
                # Before that, lets check if the member is actually inside this room

                if member not in response_table["Item"]["members"]:
                    return wrong_arguments("You are not a member in this room, first please sign in!")
                
                table.put_item(
                    Item={
                        'room_id': roomID,
                        'room_password': password,
                        'members': response_table["Item"]["members"],
                        'R_values': R_values,
                        'G_values': G_values,
                        'B_values': B_values
                    }
                )
                return {
                    'stausCode': 200,
                    'message': json.dumps("The server has been updated!")
                }
            else:
                return wrong_arguments("Password for the room isnt correct, please try again!")
        else:
            return wrong_arguments("Room_id given doesnt exist!")

    else:
        # This case is where member want to sign out
        response_table = table.get_item(Key={"room_id": roomID})
        response = {
            'statusCode': 200,
            'Message': json.dumps("Sign out completed!")
        }

        if "Item" in response_table:
            if member in response_table["Item"]["members"]:
                    members = response_table["Item"]["members"]
                    members.remove(member)
                    # Update the member list on server
                    table.update_item(Key={"room_id": roomID},
                            UpdateExpression = "set members = :newmembers",
                            ExpressionAttributeValues = {":newmembers": members},
                            ReturnValues="UPDATED_NEW"
                    )
        return response

def checking(event):
    password = event.get('password', None)
    roomID = event.get('roomID', None)
    member = event.get('member', None)
    R_values = event.get("R-values", None)
    G_values = event.get("G-values", None)
    B_values = event.get("B-values", None)
    request_for = event.get("request-for", None)
    
    if password == None or roomID == None or member == None or R_values == None or G_values == None or B_values == None or request_for == None :
        return -1

    if type(password) != int or type(roomID) != int or type(member) != str or type(R_values) != list or type(G_values) != list or type(B_values) != list or type(request_for) != int :
        return -1

    if request_for != 0 and request_for != 1 and request_for != 2:
        return -1
    
    return 1

def wrong_arguments(message):
    return{
            "statusCode": 404,
            "error" : json.dumps(message)
         }
    