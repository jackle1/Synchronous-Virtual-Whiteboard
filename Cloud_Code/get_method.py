# import the JSON utility package
import json
# import the Python math library
import math

# import the AWS SDK (for Python the package name is boto3)
import boto3
# import two packages to help us with dates and date formatting
from time import gmtime, strftime

# create a DynamoDB object using the AWS SDK
dynamodb = boto3.resource('dynamodb')
# use the DynamoDB object to select our table
table = dynamodb.Table('cpen391')
# store the current time in a human readable format in a variable
now = strftime("%a, %d %b %Y %H:%M:%S +0000", gmtime())

def lambda_handler(event, context):

    if checking(event) == -1:
        return{
                "statusCode": 404,
                "error" : json.dumps("Parameter isnt correct")
               }
        
    password = event['password']
    roomID = event['roomID']
    member = event['member']
    # Now we start checking if the roomID is correct

    table.




    return {
        'roomID' : roomID,
        'statusCode': 200,
        'body': json.dumps(password),
        "member": member
    }



def checking(event):
    password = event.get('password', None)
    roomID = event.get('roomID', None)
    member = event.get('member', None)
    
    if password == None or roomID == None or member == None:
        return -1
    
    return 1

def wrong_arguments(message):
    return{
            "statusCode": 404,
            "error" : json.dumps(message)
         }
    
