import websocket
import json


# Define the WebSocket URL
websocket_url = 'wss://7nbl97eho0.execute-api.us-east-1.amazonaws.com/production'

obj = {'action': 'get','values': [20, 152]}

# Create a WebSocket connection c
ws = websocket.WebSocket()
ws.connect(websocket_url)

# Send a message over the WebSocketx

# Receive a message from the WebSocket
result = ws.recv()

print(result)
print(json.dumps(obj))

# result = json.loads(result)
# print(result)
# print(type(result))


# Close the WebSocket connection
ws.close()



# json_string = '{"name":"Alice","age":30,"city":"New York"}'
# obj = json.loads(json_string)
# print(obj)
# print(type(obj))




