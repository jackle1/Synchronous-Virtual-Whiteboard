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

# define the handler function that the Lambda service will use an entry point
def lambda_handler(event, context):
    
    # Getting the data out of the request parameter
    r_values = event['R']
    g_values = event['G']
    b_values = event['B']
    person = event['member']
    room_id = event['room_id']
    room_password = event['password']

    # Checking if its a correct input
    result = checking(r_values, g_values, b_values)

    if(result == -1):
        return {
            'statusCode' : 400,
            'ErrorMessage' : "R, G and B are not list of floats"
        }
    
    

# write result and time to the DynamoDB table using the object we instantiated and save response in a variable
    response = table.put_item(
        Item={
            'ID': str(mathResult),
            'LatestGreetingTime':now
            })

# return a properly formatted JSON object
    return {
    'statusCode': 200,
    'body': json.dumps('Your result is ' + str(mathResult))
    }
    
def checking(a, b, c):
    # Lets check if this is a list 
    if(type(a) != list or type(b) != list or type(c) != list):
        return -1
    
    tmp = a
    while i in range(3):
        if( i == 0):
            tmp = a
        elif()