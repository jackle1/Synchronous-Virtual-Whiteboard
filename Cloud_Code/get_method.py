import boto3
import os
import json
import csv
from io import StringIO

s3 = boto3.client('s3')

def lambda_handler(event, context):

    # if checking(event) == -1:
    #     return wrong_arguments("The Parameters given to the rquest are wrong. Please follow the type or the numebr of parameters required!")

    param = event['params']
    event = param['querystring']


    member = event.get('member', "De1-Soc")
    room_password: int = int(event['RoomID'])

    # Prepare the DynamoDB client
    dynamodb = boto3.resource("dynamodb")
    table_name = os.environ["TABLE_NAME"]
    table = dynamodb.Table(table_name)

    # Finding the room with the password as given in the request header
    tables = table.scan()

    # Finding if the password matches
    for data in tables["Items"]:
        if data["room_password"] == room_password:
            # Found the table data we wanted 
            members = data["members"]
            response = {}
            response["statusCode"] = 200
            response["RGB"] = get_values()
            response["members"] = list(members)
            return response
        
    # At this point, we know the data isnt in the server with the matching room_password
    return {
        "statusCode": 404,
        "Error": "No such room exists"
    }

def checking(event):
    password = event.get('RoomID', None)
    member = event.get('member', "De1-Soc")
    
    if password == None or member == None:
        return -1
    if type(password) != int or type(member) != str:
        return -1
    
    return 1

def wrong_arguments(message):
    return{
            "statusCode": 404,
            "error" : message
         }

def get_values():
    bucket_name = 'cpen391'
    file_key = '/tmp/roomID_8862.csv'

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

    # Return the integer 2D array
    return rows

if __name__ == "__main__":
    os.environ["TABLE_NAME"] = "Cpen391"

    test_event = {
	"RoomID": 8862,
	"member": "Ranbir",

}
    result = lambda_handler(test_event, None)
    print(result)
    