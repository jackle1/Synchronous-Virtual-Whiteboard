const net = require('net');

// create a new TCP server
const server = net.createServer();

// listen for incoming connections
server.on('connection', (socket) => {
  console.log('Client connected:', socket.remoteAddress + ':' + socket.remotePort);

  // handle incoming data from the client
  socket.on('data', (data) => {
    console.log('Received data:', data.toString());

    // send data back to the client
    ///socket.write('You said: ' + data);
    // socket.write('Second message');
    // if (data.toString() == 'HI')
    for (let x = 0; x < 5; x++)
        for (let y = 0; y < 5; y++)
        {
            let toSend = `${String(x).padStart(3, '0')}${String(y).padStart(3, '0')}${String(1234).padStart(5, 0)}`;
            // console.log(toSend);
            socket.write(toSend);
        }
                
                
  });

  // handle client disconnection
  socket.on('end', () => {
    console.log('Client disconnected');
  });
});

// start listening on the specified port
const port = 8080;
server.listen(port, '192.168.137.1', () => {
  console.log(`Server listening on port ${port}`);
});

// handle server errors
server.on('error', (error) => {
  console.error('Error:', error);
});