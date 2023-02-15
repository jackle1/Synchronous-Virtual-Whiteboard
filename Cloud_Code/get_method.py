import boto3
import os
import json


def lambda_handler(event: any, context: any):

    if checking(event) == -1:
        return wrong_arguments("The Parameters given to the rquest are wrong. Please follow the type or the numebr of parameters required!")

    user: str = event["member"]
    room_id = event["roomID"]
    room_password: int = event['password']

    # Prepare the DynamoDB client
    dynamodb = boto3.resource("dynamodb")
    table_name = os.environ["TABLE_NAME"]
    table = dynamodb.Table(table_name)

    # Get the table with the primary key as room_id
    response_table = table.get_item(Key={"room_id": room_id})
    response = {}


    # If a room exists then we check if the password match
    if "Item" in response_table:
        password = response_table["Item"]["room_password"]
        if password == room_password:
            # If the password also match that means, we can give back the response with the r, g, b values
            response["statusCode"] = 200
            response["R-values"]  = response_table["Item"]["R_values"]
            response["G-values"]  = response_table["Item"]["G_values"]
            response["B-values"]  = response_table["Item"]["B_values"]
        else:
            return wrong_arguments("Password for the room isnt correct, please try again!")
    else:
        return wrong_arguments("Room_id given doesnt exist!")


    # If we are at here, we know that everything went well
    # Lets get the members list in this room 
    members = response_table["Item"]["members"]
    if user not in members:
        members.add(user)
        # Lets update the table about this new user
        table.update_item(Key={"room_id": room_id},
                          UpdateExpression = "set members = :newmembers",
                          ExpressionAttributeValues = {":newmembers": members},
                        ReturnValues="UPDATED_NEW"
        )
    
    return response

def checking(event):
    password = event.get('password', None)
    roomID = event.get('roomID', None)
    member = event.get('member', None)
    
    if password == None or roomID == None or member == None:
        return -1
    if type(password) != int or type(roomID) != int or type(member) != str:
        return -1
    
    return 1

def wrong_arguments(message):
    return{
            "statusCode": 404,
            "error" : json.dumps(message)
         }
    