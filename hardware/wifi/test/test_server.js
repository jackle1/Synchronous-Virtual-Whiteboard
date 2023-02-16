// server/index.js
// https://esp8266-shop.com/iot/websocket-connection-between-esp8266-and-node-js-server/
const express = require("express");
const WebSocket = require("ws");
const http = require("http");

const PORT = process.env.PORT || 8080;

const app = express();
const server = http.createServer(app);//create a server
const io = new WebSocket.Server({ server });
app.use(express.text());
app.use(express.json());

app.get('/', (req, res) => {
    console.log("DID GET");
    return res.send("Hello World!");
});

app.post('/test', (req, res) => {
    let toSend = Math.round(Math.random() * 127);
    let x = req.body.substring(0, 4);
    let y = req.body.substring(4, 8);
    console.log(`Got Post with: (${x}, ${y}) ${req.body}; Will send: ${toSend}`);
    return res.status(200).set('Content-Type', 'text/plain').send(String.fromCharCode(toSend));
});

io.on('connection',function(ws,req){
  /******* when server receives messsage from client trigger function with argument message *****/
  ws.on('message',function(message){
    console.log("Received: "+message);
    s.clients.forEach(function(client){ //broadcast incoming message to all clients (s.clients)
      if  (client!=ws && client.readyState ){ //except to the same client (ws) that sent this message
        client.send("broadcast: " +message);
      }
    });
    // ws.send("From Server only to sender: "+ message); //send to client where message is from
  });
  ws.on('close', function(){
    console.log("lost one client");
  });
  //ws.send("new client connected");
  console.log("new client connected");
});

server.listen(PORT, "192.168.137.1",() => {
  console.log(`Server listening on ${PORT}`);
});