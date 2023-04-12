import { useRef, useState, useEffect } from 'react';
import axios from 'axios';
import './DrawingCanvas.css';
import { FaSpinner } from 'react-icons/fa';
import { IconContext } from 'react-icons';
import throttle from 'lodash/throttle';

function DrawingCanvas(props) {
  const canvasRef = useRef(null);
  const [isDrawing, setIsDrawing] = useState(false);
  const [lastX, setLastX] = useState(0);
  const [lastY, setLastY] = useState(0);
  const [color, setColor] = useState('black');
  const [RGBData, setRGBData] = useState([]);
  const [XData, setXData] = useState([]);
  const [YData, setYData] = useState([]);
  const [ws, setWs] = useState(null);

  useEffect(() => {
    const canvas = canvasRef.current;
    const context = canvas.getContext('2d');

    context.strokeStyle = color;
    context.lineJoin = 'round';
    context.lineCap = 'round';
    context.lineWidth = 5;

    handleGetRGBData();

    const ws = new WebSocket('wss://7nbl97eho0.execute-api.us-east-1.amazonaws.com/production');

    console.log(props.roomNumber)
    ws.addEventListener('open', () => {
      const data = {
        action: 'connect_to_roomID',
        roomID: props.roomNumber,
        user: props.username,
      };
      ws.send(JSON.stringify(data));
      console.log('WebSocket connection established');
    });

    ws.addEventListener('message', (event) => {
      console.log('WebSocket message received', event.data);
      props.onChildVariable(JSON.parse(event.data)["members"]);
      if (JSON.parse(event.data)["RGB"] && JSON.parse(event.data)["x"] && JSON.parse(event.data)["y"]) {
        console.log(JSON.parse(event.data)["RGB"])
        console.log('Received message with RGB:', JSON.parse(event.data)["RGB"], 'x:', JSON.parse(event.data)["x"], 'y:', JSON.parse(event.data)["y"]);
        for (let i = 0; i < JSON.parse(event.data)["x"].length; i++) {
          const x = JSON.parse(event.data)["x"][i];
          const y = JSON.parse(event.data)["y"][i];
          const redComponent = (JSON.parse(event.data)["RGB"][i] >> 16) & 0xFF;
          const greenComponent = (JSON.parse(event.data)["RGB"][i] >> 8) & 0xFF;
          const blueComponent = JSON.parse(event.data)["RGB"][i] & 0xFF;
          const color = `rgb(${redComponent}, ${greenComponent}, ${blueComponent})`;
          context.fillStyle = color;
          context.fillRect(x, y, 1, 1);
        }
      }
      if(JSON.parse(event.data) == "Please do a whole GET Request") {
        console.log("New picture uploaded, doing GET")
        handleGetRGBData();
      }
    });

    ws.addEventListener('close', () => {
      console.log('WebSocket connection closed');
    });

    setWs(ws);

    return () => {
      const disconnection = {
        action: 'disconnect_roomID',
        roomID: props.roomNumber,
        user: props.username,
      };
      ws.send(JSON.stringify(disconnection));
      console.log('WebSocket connection disconnected')
      setTimeout(() => {
        ws.close();
      }, 2000);
    };
  }, []);

  // function sendWebSocketData(RGBData, XData, YData) {
  //   if (ws && ws.readyState === WebSocket.OPEN) {
  //     console.log(XData);
  //     const data = {
  //       action: 'post',
  //       roomID: props.roomNumber,
  //       user: props.username,
  //       RGB: RGBData,
  //       x: XData,
  //       y: YData,
  //     };
  //     ws.send(JSON.stringify(data));
  //     ws.addEventListener('message', (event) => {
  //         setRGBData([]);
  //         setXData([]);
  //         setYData([]);
  //     });
  //   }
  // }

  function sendWebSocketData(RGBData, XData, YData, callback) {
    const MAX_ARRAY_LENGTH = 5000;
    if (ws && ws.readyState === WebSocket.OPEN) {
      const dataLength = RGBData.length;
      let startIndex = 0;
      let endIndex = Math.min(MAX_ARRAY_LENGTH, dataLength);
      console.log(XData)
      
      const sendNextChunk = () => {
        const limitedRGBData = RGBData.slice(startIndex, endIndex);
        const limitedXData = XData.slice(startIndex, endIndex);
        const limitedYData = YData.slice(startIndex, endIndex);
        
        const data = {
          action: 'post',
          roomID: props.roomNumber,
          user: props.username,
          RGB: limitedRGBData,
          x: limitedXData,
          y: limitedYData,
        };
        
        console.log(`Sending chunk from index ${startIndex} to ${endIndex}...`);
        ws.send(JSON.stringify(data));
        console.log(`Sent chunk from index ${startIndex} to ${endIndex}.`);
        
        startIndex = endIndex;
        endIndex = Math.min(endIndex + MAX_ARRAY_LENGTH, dataLength);
        
        if (startIndex < dataLength) {
          setTimeout(() => {
            sendNextChunk();
          }, 100);
        } else {
          callback();
        }
      };
      
      sendNextChunk();
    }
  }

  function startDrawing(event) {
    setIsDrawing(true);
    setLastX(event.nativeEvent.offsetX);
    setLastY(event.nativeEvent.offsetY);
  }

  const throttledDraw = throttle(draw, 500);

  function draw(event) {
    if (!isDrawing) return;

    const canvas = canvasRef.current;
    const context = canvas.getContext('2d');
    context.strokeStyle = color;
    context.beginPath();
    context.moveTo(lastX, lastY);
    context.lineTo(event.nativeEvent.offsetX, event.nativeEvent.offsetY);
    context.stroke();
    setLastX(event.nativeEvent.offsetX);
    setLastY(event.nativeEvent.offsetY);

    const imageData = context.getImageData(event.nativeEvent.offsetX, event.nativeEvent.offsetY, 1, 1);
    const pixel = imageData.data;
    let red = pixel[0];
    let green = pixel[1];
    let blue = pixel[2];
    const rgb = (red << 16) | (green << 8) | blue;;
    // const rgb = 0;

    if(event.nativeEvent.offsetX > 2 && event.nativeEvent.offsetX < 648
      && event.nativeEvent.offsetY > 2 && event.nativeEvent.offsetY < 478) {
      setRGBData(prevRGBData => [...prevRGBData, rgb, rgb, rgb, rgb, rgb, rgb, rgb, rgb, rgb, rgb, rgb, rgb, rgb, rgb, rgb, rgb, rgb, rgb, rgb, rgb, rgb, rgb, rgb, rgb, rgb]);
      setXData(prevXData => [...prevXData, event.nativeEvent.offsetX-2, event.nativeEvent.offsetX-2, event.nativeEvent.offsetX-2, event.nativeEvent.offsetX-2, event.nativeEvent.offsetX-2,
        event.nativeEvent.offsetX-1, event.nativeEvent.offsetX-1, event.nativeEvent.offsetX-1, event.nativeEvent.offsetX-1, event.nativeEvent.offsetX-1,
        event.nativeEvent.offsetX, event.nativeEvent.offsetX, event.nativeEvent.offsetX, event.nativeEvent.offsetX, event.nativeEvent.offsetX,
        event.nativeEvent.offsetX+1, event.nativeEvent.offsetX+1, event.nativeEvent.offsetX+1, event.nativeEvent.offsetX+1, event.nativeEvent.offsetX+1,
        event.nativeEvent.offsetX+2, event.nativeEvent.offsetX+2, event.nativeEvent.offsetX+2, event.nativeEvent.offsetX+2, event.nativeEvent.offsetX+2]);
      setYData(prevYData => [...prevYData, event.nativeEvent.offsetY-2, event.nativeEvent.offsetY-1, event.nativeEvent.offsetY-0, event.nativeEvent.offsetY+1, event.nativeEvent.offsetY+2,
        event.nativeEvent.offsetY-2, event.nativeEvent.offsetY-1, event.nativeEvent.offsetY-0, event.nativeEvent.offsetY+1, event.nativeEvent.offsetY+2,
        event.nativeEvent.offsetY-2, event.nativeEvent.offsetY-1, event.nativeEvent.offsetY-0, event.nativeEvent.offsetY+1, event.nativeEvent.offsetY+2,
        event.nativeEvent.offsetY-2, event.nativeEvent.offsetY-1, event.nativeEvent.offsetY-0, event.nativeEvent.offsetY+1, event.nativeEvent.offsetY+2,
        event.nativeEvent.offsetY-2, event.nativeEvent.offsetY-1, event.nativeEvent.offsetY-0, event.nativeEvent.offsetY+1, event.nativeEvent.offsetY+2]);
    }
    else {
      setRGBData(prevRGBData => [...prevRGBData, rgb]);
      setXData(prevXData => [...prevXData, event.nativeEvent.offsetX]);
      setYData(prevYData => [...prevYData, event.nativeEvent.offsetY]);
    }
  }

  function stopDrawing() {
    setIsDrawing(false);
    console.log("done drawing, about to send")
    sendWebSocketData(RGBData, XData, YData, () => {
      console.log('All chunks sent!');
      setRGBData([]);
      setXData([]);
      setYData([]);
    });
    // sendWebSocketData(RGBData, XData, YData);
  }

  function handleColorChange(event) {
    setColor(event.target.value);
  }

  function separateRGBData(rgbData) {
    const redData = [];
    const greenData = [];
    const blueData = [];
    for (let i = 0; i < rgbData.length; i++) {
      const redRow = [];
      const greenRow = [];
      const blueRow = [];
      for (let j = 0; j < rgbData[i].length; j++) {
        const r = (rgbData[i][j] >> 16);
        const g = (rgbData[i][j] >> 8) & 255;
        const b = (rgbData[i][j] & 255);
        redRow.push(r);
        greenRow.push(g);
        blueRow.push(b);
      }
      redData.push(redRow);
      greenData.push(greenRow);
      blueData.push(blueRow);
    }

    console.log("this is the width");
    console.log(redData[0].length);
    console.log("this is the height");
    console.log(redData.length);
    return {
      redData: redData.flat(),
      greenData: greenData.flat(),
      blueData: blueData.flat(),
    };
  }

  // async function handleGetRGBData() {
  //   const canvas = canvasRef.current;
  //   const context = canvas.getContext('2d');
  //   const imageData = context.getImageData(0, 0, canvas.width, canvas.height);
  //   const pixelData = imageData.data;
  //   console.log(pixelData)
  
  //   const response = await axios.get(`https://hbzwo0rl65.execute-api.us-east-1.amazonaws.com/dev/cpen391?RoomID=${props.roomNumber}`);
  //   const receivedRGBData = response.data.RGB;
  //   console.log(response.data);
  //   console.log(receivedRGBData);

  //   const { redData, greenData, blueData } = separateRGBData(receivedRGBData);

  //   console.log(redData.length);

  //   let j = 0;
  
  //   for (let i = 0; i < pixelData.length; i += 4) {
  //     let r = redData[j];
  //     let g = greenData[j];
  //     let b = blueData[j];
  //     j++;
  //     let alpha = 255;
  //     pixelData[i] = r;
  //     pixelData[i + 1] = g;
  //     pixelData[i + 2] = b;
  //     pixelData[i + 3] = alpha;
  //   }
  
  //   console.log(imageData)
  //   context.putImageData(imageData, 0, 0);
  // }

  const [loading, setLoading] = useState(false);

  async function handleGetRGBData() {
    const canvas = canvasRef.current;
    const context = canvas.getContext('2d');
    const imageData = context.getImageData(0, 0, canvas.width, canvas.height);
    const pixelData = imageData.data;
    console.log(pixelData)

    // Display loading icon
    setLoading(true);

    try {
      const response = await axios.get(`https://hbzwo0rl65.execute-api.us-east-1.amazonaws.com/dev/cpen391?RoomID=${props.roomNumber}`);

      const receivedRGBData = response.data.RGB;
      console.log(response.data);
      console.log(receivedRGBData);

      const { redData, greenData, blueData } = separateRGBData(receivedRGBData);

      console.log(redData.length);

      let j = 0;

      for (let i = 0; i < pixelData.length; i += 4) {
        let r = redData[j];
        let g = greenData[j];
        let b = blueData[j];
        j++;
        let alpha = 255;
        pixelData[i] = r;
        pixelData[i + 1] = g;
        pixelData[i + 2] = b;
        pixelData[i + 3] = alpha;
      }

      console.log(imageData)
      context.putImageData(imageData, 0, 0);
    } catch (error) {
      console.error(error);
    } finally {
      // Hide loading icon
      setLoading(false);
    }
  }

  return (
    <div>
      <div className="canvas-container">
      <IconContext.Provider value={{ className: 'spinner', style: { verticalAlign: 'middle', display: 'inline-block' } }}>
        {loading && (
          <div className="loading-container" style={{ display: 'flex', justifyContent: 'center', alignItems: 'center', height: '50px' }}>
            <FaSpinner className="icon-spin" /> Loading...
          </div>
        )}
      </IconContext.Provider>

      <canvas
        ref={canvasRef}
        width={640}
        height={480}
        onMouseDown={startDrawing}
        onMouseMove={draw}
        onMouseUp={stopDrawing}
        onMouseLeave={stopDrawing}
      />
      </div>

      <div>
        <label htmlFor="color-picker">Select color:</label>
        <input type="color" id="color-picker" value={color} onChange={handleColorChange} />
      </div>

      <button onClick={handleGetRGBData}>Refresh Canvas</button>
    </div>
  );
}

export default DrawingCanvas;
