
# Welome to the codebase for the Cloud Component 

### POST Requests to retrieve or update the database on the cloud 

For **retrieving data** on the server, make a POST request at 
https://hbzwo0rl65.execute-api.us-east-1.amazonaws.com/dev/cpen391

For **updating data** on the server, make a POST request at 
https://hbzwo0rl65.execute-api.us-east-1.amazonaws.com/dev/cpen391

They are at the same endpoint!



### GET Request Requirements

Make a GET Request with the string query parameters:
https://hbzwo0rl65.execute-api.us-east-1.amazonaws.com/dev/cpen391?RoomID=????

You can see the sample result below

```
https://hbzwo0rl65.execute-api.us-east-1.amazonaws.com/dev/cpen391?RoomID=8862&User=Ranbir
{"statusCode": 200, "R-values": [[100, 100], [100, 1]], "G-values": [[100, 100], [100, 1]], "B-values": [[100, 100], [100, 1]], "members": ["De1-Soc"]}
```
### POST body requirements

For **retrieving data** from the server, make a POST request at 
https://hbzwo0rl65.execute-api.us-east-1.amazonaws.com/dev/cpen391

*Requirements for body are*:
```
{
                  "member": "Ranbir",
                  "roomID": 8862,
                  'RGB': 121212,
                  "request-for": 1,
                  "x": 1,
                  "y": 1
                }
```

This is just a example, you need to enter the information as per your needs besides the appropriate headers

In this, the control of action is controlled by the value in request_for header.
These are the values for request_for that are supported
```
0: Create new room
1: Update pixels at the server
2: Sign out 
```

Even though, you may not have the values to the header at the moment, for example in creating a new room, just put appropriate garbage value of the right type of the variable.
For example, 
```for R-values: 13131231311121```
or ```x : 101```, or ```“roomID”: 101```.

 Dont put None or NULL, otherwise you will receive a error

