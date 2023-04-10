import requests
import urllib.parse

import time

start_time = time.time()

base_url = "https://hbzwo0rl65.execute-api.us-east-1.amazonaws.com/dev/cpen391"

# Define the query string parameters
params = {
    "RoomID": 8197
}

# Encode the parameters and append them to the base URL
url = base_url + "?" + urllib.parse.urlencode(params)
print(url)


# Send the HTTP POST request with the JSON data and headers
response = requests.get(url)
# RGB = response["RGB"]
# x = response["x"]
# y = response["y"]

width = 640
height = 480




# R = RGB[i][j] >> 16
# G = (RGB[i][j] >> 8) & 255
# B = RGB[i][j] & 255


# check if the request was successful (HTTP status code 200)
if response.status_code == 200:
    # print the response content (HTML code)
    # print(response.text)
    pass

else:
    # handle error cases
    print("Error:", response.status_code)


import csv
import json

# Define the 2D array to save to the CSV file
my_data = json.loads(response.text)
my_data= my_data["RGB"]
# Define the name of the CSV file to save to
filename = "my_data.csv"

# Open the CSV file for writing
with open(filename, mode='w', newline='') as csvfile:

    # Create a CSV writer object
    csvwriter = csv.writer(csvfile)

    # Write each row of the 2D array to the CSV file
    for row in my_data:
        csvwriter.writerow(row)

# Print a message indicating that the CSV file was saved
print("CSV file saved successfully.")


# Code you want to time goes here

end_time = time.time()
elapsed_time = end_time - start_time

print(f"Elapsed time: {elapsed_time:.2f} seconds")