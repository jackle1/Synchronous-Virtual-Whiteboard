import websocket
import json
RGB = []
x = []
y = []
for i in range(250, 300):
    RGB.append(1111)
    x.append(i)
    y.append(i)

def on_message(ws, message):
    print(f"Received message: {message}")
    # print("Hellpo")

    # res = input("Enter again?")
    name = input("WHATYTTTTT")
    obj = {"action": "post", "roomID": 8862, "user": "De1Soccccc", "RGB": RGB, "x": x, "y": y}
    ws.send(json.dumps(obj))
    



def on_error(ws, error):
    print(error)

def on_close(ws):
    print("### closed ###")

def on_open(ws):
    # Send a message over the WebSocket
    obj = {"action": "connect_to_roomID", "roomID": 8862, "user": "De1Soccccc"}
    ws.send(json.dumps(obj))




if __name__ == "__main__":
    #websocket.enableTrace(True)
    websocket_url = 'wss://7nbl97eho0.execute-api.us-east-1.amazonaws.com/production'
    ws = websocket.WebSocketApp(websocket_url,
                              on_message = on_message,
                              on_error = on_error,
                              on_close = on_close)
    ws.on_open = on_open
    ws.run_forever()
    # Receive a message from the WebSocket

    ws.close()


