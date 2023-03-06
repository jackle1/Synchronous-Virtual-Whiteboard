
# Welome to the codebase for the Cloud Component 

### POST Requests to retrieve or update the database on the cloud 

For **retrieving data** on the server, make a POST request at 
https://2hanc5qruj.execute-api.us-east-1.amazonaws.com/dev

For **updating data** on the server, make a POST request at 
https://hbzwo0rl65.execute-api.us-east-1.amazonaws.com/dev/cpen391


### POST body requirements

For **retrieving data** from the server, make a POST request at 
https://2hanc5qruj.execute-api.us-east-1.amazonaws.com/dev


*Requirements for body are*:
```
{
	"password": 1234,
	"member": "Ranbir",
	"roomID": 0
}
```

This is just a example, you need to enter the information as per your needs besides the appropriate headers

For **retrieving data** from the server, make a POST request at 
https://hbzwo0rl65.execute-api.us-east-1.amazonaws.com/dev/cpen391

*Requirements for body are*:
```
{
    "member": "Ranbir",
      "roomID": 2,
      "password": 2361,
      "R-values" : [1],
      "G-values" : [1],
      "B-values" : [1],
      "request-for": 2,
      "x": 1,
      "y": 1
}
```

This is just a example, you need to enter the information as per your needs besides the appropriate headers

In this, the control of action is controlled by the value in request_for header.
These are the values for request_for that are supported
0: Create new room
1: Update pixels at the server
2: Sign out 

Even though, you may not have the values to the header at the moment, for example in creating a new room, just put appropriate garbage value of the right type of the variable.
For example, 
```for R-values: [13131231311121]```
or ```x : 101```, or ```“roomID”: 101```.

 Dont put None or NULL, otherwise you will receive a error

