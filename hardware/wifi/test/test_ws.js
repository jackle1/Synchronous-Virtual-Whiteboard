const WebSocket = require('ws');
const https = require('https');

// Create an HTTPS agent with the necessary options for TLS
const httpsAgent = new https.Agent({
  rejectUnauthorized: true, // Verify the server's TLS certificate
  keepAlive: true, // Use keep-alive connections
});

// Connect to the WebSocket server
const ws = new WebSocket('wss://7nbl97eho0.execute-api.us-east-1.amazonaws.com/production', {
  agent: httpsAgent, // Use the HTTPS agent with TLS options
});

ws.on('open', () => {
  console.log('Connected to WebSocket server');

  // Send a message to the server
  ws.send('Hello, world!');
});

ws.on('message', (data) => {
  console.log(`Received message: ${data}`);
});

ws.on('close', () => {
  console.log('Disconnected from WebSocket server');
});
// const WebSocket = require('websocket');

// // Create a new WebSocket client
// const client = new WebSocket.client();

// // Connect to the WebSocket server
// client.connect('wss://7nbl97eho0.execute-api.us-east-1.amazonaws.com/production', 'echo-protocol');

// // When the connection is established
// client.on('connect', function(connection) {
//   console.log('WebSocket client connected');

//   // Send a message to the server
//   connection.send('Hello, server!');

//   // When a message is received from the server
//   connection.on('message', function(message) {
//     console.log('Received message:', message.utf8Data);
//   });
// });

// // When an error occurs
// client.on('connectFailed', function(error) {
//   console.log('Connect Error: ' + error.toString());
// });