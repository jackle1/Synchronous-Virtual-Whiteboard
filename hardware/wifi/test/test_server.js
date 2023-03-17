// server/index.js
// https://esp8266-shop.com/iot/websocket-connection-between-esp8266-and-node-js-server/
const express = require("express");
const WebSocket = require("ws");
const http = require("http");
const path = require('path'); // Node.js built-in module for handling file paths
const fs = require('fs'); // Node.js built-in module for handling file system operations
const multer = require('multer'); // middleware for handling file uploads
// Set up multer middleware for handling file uploads
const storage = multer.diskStorage({
  destination: function (req, file, cb) {
    cb(null, './uploads'); // specify the directory to save uploaded files
  },
  filename: function (req, file, cb) {
    cb(null, file.fieldname + '-' + Date.now() + path.extname(file.originalname)); // specify the filename to save
  }
});
const upload = multer({ storage: storage });

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

app.post('/upload', upload.single('file'), (req, res) => {
  const filePath = req.file.path;
  console.log(req.file);
  fs.readFile(filePath, (err, data) => {
    if (err) {
      console.error(err);
      res.statusCode = 500;
      res.end('Error while reading uploaded file');
    } else {
      const newFilePath = './saved-files/' + req.file.filename; // specify the location to save the file
      fs.writeFile(newFilePath, data, (err) => {
        if (err) {
          console.error(err);
          res.statusCode = 500;
          res.end('Error while saving uploaded file');
        } else {
          res.send(`File ${req.file.filename} uploaded and saved successfully!`);
        }
      });
    }
  });
});

app.post('/test', (req, res) => {
    console.log(req.body);
    let toSend = Math.round(Math.random() * 127);
    let x = req.body.substring(0, 4);
    let y = req.body.substring(4, 8);
    console.log(`Got Post with: (${x}, ${y}) ${req.body}; Will send: ${toSend}`);
    return res.status(200).set('Content-Type', 'text/plain').send(String.fromCharCode(toSend));
});

app.get('/file', (req, res) => {
    const filepath = path.join(__dirname, 'room_number.bin');
    res.sendFile(filepath);
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