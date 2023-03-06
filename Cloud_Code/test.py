import requests
import json

url = "https://hbzwo0rl65.execute-api.us-east-1.amazonaws.com/dev/cpen391"

# url =  "https://7jasr5uat3.execute-api.us-east-1.amazonaws.com/dev"
# headers = {
#     "password": 1234,
#     "member": "Ranbir",
#     "roomID": 0

# }

data = {
                "member": "Ranbir",
                  "roomID": 2,
                  "password": 5277,
                  "R-values" : [1],
                  "G-values" : [1],
                  "B-values" : [1],
                  "request-for": 1,
                  "x": 0,
                  "y": 0
                }

# Convert the data to a JSON string
json_data = json.dumps(data)

# Define the headers with Content-Type: application/json
headers = {'Content-Type': 'application/json'}

# Send the HTTP POST request with the JSON data and headers
response = requests.post(url, headers=headers, data=json_data)



# check if the request was successful (HTTP status code 200)
if response.status_code == 200:
    # print the response content (HTML code)
    print(response.text)
else:
    # handle error cases
    print("Error:", response.status_code)
