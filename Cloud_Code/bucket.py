import boto3
import csv
from io import StringIO

s3 = boto3.client('s3')

def lambda_handler(event, context):
    # Set the bucket name and file key for your CSV file
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
    return {
        'statusCode': 200,
        'body': rows
    }


print(lambda_handler(None, None))
print(len(lambda_handler(None, None)['body']))





