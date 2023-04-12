import requests
import json
import time

start_time = time.time()

url = "https://hbzwo0rl65.execute-api.us-east-1.amazonaws.com/dev/cpen391"

# url =  "https://7jasr5uat3.execute-api.us-east-1.amazonaws.com/dev"
# headers = {
#     "password": 1234,
#     "member": "Ranbir",
#     "roomID": 0

# }
RGB = []
x = []
y = []
# for i in range(480):
#     for j in range(640):
#         RGB.append(16777215)
#         x.append(i)
#         y.append(j)


data = {
                  "member": "Ranbir",
                  "roomID": 8862,
                  'RGB': RGB,
                  "request-for": 0,
                  "x": x,
                  "y": y
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
    print(response.text)

end_time = time.time()
elapsed_time = end_time - start_time

print(f"Elapsed time: {elapsed_time:.2f} seconds")