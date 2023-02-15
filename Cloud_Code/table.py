import boto3
import os


def lambda_handler(event: any, context: any):

    user: str = event["user"]
    room_id = event["room_id"]
    room_password: int = event['room_password']

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
            response["R-values"]  = response_table["Item"]["R_values"]
            response["G-values"]  = response_table["Item"]["G_values"]
            response["B-values"]  = response_table["Item"]["B_values"]
            response["StatusCode"] = 200
        else:
            response["StatusCode"] = 404
            response["errorMessage"] = "Password for the room isnt correct, please try again!"
    else:
        response["StatusCode"] = 404
        response["errorMessage"] = "Room_id given doesnt exist!"



    # Increment the visit count and put the item into DynamoDB table.
    #table.put_item(Item={"user": user, "visit_count": visit_count})
    return response


if __name__ == "__main__":
    os.environ["TABLE_NAME"] = "Cpen391"
    test_event = {"user": "local_pixegami",
                  "room_id": 0,
                  "room_password": 1234
                }
    result = lambda_handler(test_event, None)
    print(result)