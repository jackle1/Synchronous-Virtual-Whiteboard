import json
import urllib3
import boto3
import random

client = boto3.client('apigatewaymanagementapi', endpoint_url="https://7nbl97eho0.execute-api.us-east-1.amazonaws.com/production")

m = 10
n = 10

values = []
for i in range(m):
    tmp = []
    for j in range(n):
        tmp.append(random.randint(1, 100))
    values.append(tmp)



def lambda_handler(event, context):

    global values

    # First get the values from the context
    connectionId = event['requestContext']['connectionId']
    routeKey = event['requestContext']['routeKey']


    if(routeKey == 'get'):
        client.post_to_connection(ConnectionId=connectionId, Data=json.dumps(values).encode('utf-8'))
        return { "statusCode": 200  }
    
    elif(routeKey == 'post'):
        values1 = event["body"]
        values1 = json.loads(values1)
        values = values1['values']
        client.post_to_connection(ConnectionId=connectionId, Data=json.dumps(values).encode('utf-8'))
        return {"statusCode": 200}
    
    else:
        return {"statusCode": 200}
    

    # client.post_to_connection(ConnectionId=connectionId, Data=json.dumps(event).encode('utf-8'))


    