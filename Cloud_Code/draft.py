import csv
import boto3
import io

def csv_to_array(bucket_name, file_key):
    # Download the CSV file from S3 and read its contents
    s3 = boto3.resource('s3')
    bucket = s3.Bucket(bucket_name)
    file_object = bucket.Object(file_key)
    file_content = file_object.get()['Body'].read().decode('utf-8')

    # Parse the CSV file into a list of lists
    reader = csv.reader(io.StringIO(file_content))
    rows = []
    for row in reader:
        int_row = [int(val) for val in row]
        rows.append(int_row)

    return rows


print(csv_to_array('cpen391', '/tmp/new-file.csv'))