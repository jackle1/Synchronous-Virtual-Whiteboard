import websocket
import json
import time
RGB = []
x = []
y = []
for i in range(250, 450):
    RGB.append(1111)
    x.append(i)
    y.append(i)

def on_message(ws, message):
    print(f"Received message: {message}")
    # print("Hellpo")

    # res = input("Enter again?")

    

roomID = 3008 # 8197

def on_error(ws, error):
    print(error)

def on_close(ws):
    print("### closed ###")

def on_open(ws):
    # Send a message over the WebSocket
    obj = {"action": "connect_to_roomID", "roomID": roomID, "user": "HIII"}
    ws.send(json.dumps(obj).encode('utf-8'))
    time.sleep(0.5)
    # for i in range(len(RGB)):
    #     time.sleep(0.3)
    #     obj = {"action": "post", "roomID": 5447, "user": "HIII", "RGB": RGB[i], "x": x[i], "y": y[i]}
    #     ws.send(json.dumps(obj).encode('utf-8'))


    obj = {"action": "post", "roomID": roomID, "user": "HIII", "RGB": [], "x": [], "y": []}
    ws.send(json.dumps(obj).encode('utf-8'))


    time.sleep(4)
    obj = obj = {"action": "disconnect_roomID", "roomID": roomID, "user": "HIII"}
    ws.send(json.dumps(obj).encode('utf-8'))




if __name__ == "__main__":
    #websocket.enableTrace(True)
    websocket_url = 'wss://7nbl97eho0.execute-api.us-east-1.amazonaws.com/production'
    ws = websocket.WebSocketApp(websocket_url,
                              on_message = on_message,
                              on_error = on_error,
                              on_close = on_close)
    ws.on_open = on_open
    ws.close()
    ws.run_forever()
    # Receive a message from the WebSocket

    ws.close()


