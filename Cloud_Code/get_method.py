import boto3
import os
import json

'''
Hello, this multi-line comment is for my fellow team mates on what to send for the database on the server
This is the file for Http GET request
Make the GET request on https://hbzwo0rl65.execute-api.us-east-1.amazonaws.com/dev/cpen391
Follow the request body as follows
    test_event = {
                    "password": 1234,
                    "member": "Ranbir",
                    "roomID": 0
                }
This will return a http response on the r,b,g values.
The response looks something like this 
    response = {
        'statusCode' = ____ (Depends if it was a succefull execution or not)
        'R-values' = ___
        'G-values' = ___
        'B-values' = ___
    }

'''

def lambda_handler(event, context):

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
    if members == None:
        members = set()
        print("Here")
    if user not in members:
        members.add(user)
        # Lets update the table about this new user
        table.update_item(Key={"room_id": room_id},
                          UpdateExpression = "set members = :newmembers",
                          ExpressionAttributeValues = {":newmembers": members},
                        ReturnValues="UPDATED_NEW"
        )
    # Sending the active members on this room 
    response["members"] = members

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
            "error" : (message)
         }


if __name__ == "__main__":
    os.environ["TABLE_NAME"] = "Cpen391"

    test_event = {
	"password": 2361,
	"member": "Ranbir",
	"roomID": 2
}
    result = lambda_handler(test_event, None)
    print(result)
    