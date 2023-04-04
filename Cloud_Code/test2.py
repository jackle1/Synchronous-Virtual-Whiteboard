import websocket
import json


height = 480
width = 640
height = 10
width = 10
RGB = []
x = []
y = []
for i in range(height):
    for j in range(width):
        x.append(i)
        y.append(j)
        RGB.append(100)

RGB = RGB[:5]
x = x[:5]
y = y[:5]
# Define the WebSocket URL
ws_url = "wss://7nbl97eho0.execute-api.us-east-1.amazonaws.com/production"

# Define the message to send
message = {"action": "connect_to_roomID", "roomID": 8862, "user": "Ranbir1"}
message2 = {"action": "post", "roomID": 8862, "user": "User", "RGB": RGB, "x": x, "y": y}
message3 = {"action": "disconnect_roomID", "roomID": 8862, "user": "User"}

# Connect to the WebSocket server
ws = websocket.create_connection(ws_url)

# Send the message as JSON
ws.send(json.dumps(message))
ws.send(json.dumps(message))

# Receive and print the response
response = ws.recv()


print("Received response:", response)

ws.send(json.dumps(message2))

# Keep receiving and printing data until the connection is closed
while True:
    try:
        data = ws.recv()
        print("Received data:", data)
    except websocket.WebSocketConnectionClosedException:
        print("Connection closed.")
        break

# Close the WebSocket connection
ws.close()
