import websocket
import json

def on_message(ws, message):
    print("Received message: {}".format(message))
    print("Hellpo")

    res = input("Enter again?")

    obj = {"action": "post", "roomID": 8862, "user": "Demo", "RGB": [1,12], "x": [0, 12], "y": [0, 12]}
    ws.send(json.dumps(obj))
    



def on_error(ws, error):
    print(error)

def on_close(ws):
    print("### closed ###")

def on_open(ws):
    # Send a message over the WebSocket
    obj = {"action": "connect_to_roomID", "roomID": 8862, "user": "Demo"}
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


