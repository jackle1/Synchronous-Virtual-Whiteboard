import boto3
import time

dynamodb = boto3.resource('dynamodb')
table = dynamodb.Table('Cpen391')

# Generate a unique requestId to identify the thread acquiring the lock
request_id = 'BB'

# Define the item key for the record you want to lock
item_key = {'room_id': 6}

# Define the update expression to acquire the lock
update_expression = 'SET #lock = :new_lock_value, #lock_acquired = :request_id'

# Define the condition expression to ensure that the lock is not already acquired
condition_expression = '#lock = :old_value AND #lock_acquired = :old_request'

# Define the attribute values for the update expression and condition expression
expression_attribute_values = {
    ':new_lock_value': 1,
    ':request_id': request_id,
    ':old_value': 0,
    ':old_request': None  # set this to the previous request_id if it exists
}

# Define the attribute names for the update expression and condition expression
expression_attribute_names = {
    '#lock': 'lock',
    '#lock_acquired': 'lock_acquired'
}
while(True):
    try:
        # Acquire the lock by updating the record with the specified key and expression
        response = table.update_item(
            Key=item_key,
            UpdateExpression=update_expression,
            ConditionExpression=condition_expression,
            ExpressionAttributeValues=expression_attribute_values,
            ExpressionAttributeNames=expression_attribute_names,
            ReturnValues='ALL_NEW'
        )
        print("lock acquired")
        break
    except:
        print("No worries for 1!")
        time.sleep(1)
        continue


hello = input("Should I???")


# Define the update expression to acquire the lock
update_expression = 'SET #lock = :new_lock_value, #lock_acquired = :request_id'

# Define the condition expression to ensure that the lock is not already acquired
condition_expression = '#lock = :old_value AND #lock_acquired = :old_request'

# Define the attribute values for the update expression and condition expression
expression_attribute_values = {
    ':new_lock_value': 0,
    ':request_id': None,
    ':old_value': 1,
    ':old_request': request_id  # set this to the previous request_id if it exists
}

# Define the attribute names for the update expression and condition expression
expression_attribute_names = {
    '#lock': 'lock',
    '#lock_acquired': 'lock_acquired'
}
while(True):
    try:
        # Acquire the lock by updating the record with the specified key and expression
        response = table.update_item(
            Key=item_key,
            UpdateExpression=update_expression,
            ConditionExpression=condition_expression,
            ExpressionAttributeValues=expression_attribute_values,
            ExpressionAttributeNames=expression_attribute_names,
            ReturnValues='ALL_NEW'
        )

        print("lock released")
        break

    except:
        print("No worries for 2 !")
        time.sleep(1)
        continue
# # Check if the lock was successfully acquired
# if 'Attributes' in response:
#     locked_by = response['Attributes']['lock_acquired']
#     print(f"Lock acquired by thread {locked_by}")
# else:
#     print("Failed to acquire lock")
