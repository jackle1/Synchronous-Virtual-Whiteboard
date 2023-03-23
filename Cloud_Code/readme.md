
# Welome to the codebase for the Cloud Component 

### POST Requests to retrieve or update the database on the cloud 

For **retrieving data** on the server, make a POST request at 
https://hbzwo0rl65.execute-api.us-east-1.amazonaws.com/dev/cpen391

For **updating data** on the server, make a POST request at 
https://hbzwo0rl65.execute-api.us-east-1.amazonaws.com/dev/cpen391

They are at the same endpoint!



### GET Request Requirements

Make a GET Request with the string query parameters:
https://hbzwo0rl65.execute-api.us-east-1.amazonaws.com/dev/cpen391?RoomID=8862

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


## Web Socket 
The url for the Web Socket is ```wss://7nbl97eho0.execute-api.us-east-1.amazonaws.com/production```

To connect to the websocket, use standard libraies.
However, connecting to the websocket doesnt mean you are connected to the RoomID

#### To Connect to the Room 
Send a message into the websocket 
``` {"action": "connect_to_roomID", "roomID": 8862, "user": "User"}```
Change roomID and User accordingly to your needs

#### To send pixels to the webSocket
Send a message into the websocket
``` {"action": "post", "roomID": 8862, "user": "User", "RGB": RGB, "x": x, "y": y}```

#### To disconnect from a Room 
Send a messahe to the WebSocket 
```{"action": "disconnect_roomID", "roomID": 8862, "user": "User"} ```

#### Important points while connected to a Room 
If a member leaved that room, you will be notified 
If a member joins that room, you will be notified 
If a member updates a pixel/pixels, you will be notified 
If a member updates all pixels on a post request, you will be notified  (Feature Not completed!)

At a time, this websocket can only update 2548 pixels, so if you have a bigger data, please divide your pixel data into smaller chunks of 2548
