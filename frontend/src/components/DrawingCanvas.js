import { useRef, useState, useEffect } from 'react';
import axios from 'axios';
import './DrawingCanvas.css';

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

    ws.addEventListener('open', () => {
      console.log(props.data)
      const data = {
        action: 'connect_to_roomID',
        roomID: props.data,
        user: props.username,
      };
      ws.send(JSON.stringify(data));
      console.log('WebSocket connection established');
    });

    ws.addEventListener('message', (event) => {
      console.log('WebSocket message received', event.data);
      props.onChildVariable(JSON.parse(event.data)["members"]);
    });

    ws.addEventListener('close', () => {
      console.log('WebSocket connection closed');
    });

    setWs(ws);

    return () => {
      const disconnection = {
        action: 'disconnect_roomID',
        roomID: props.data,
        user: props.username,
      };
      ws.send(JSON.stringify(disconnection));
      console.log('WebSocket connection disconnected')
      setTimeout(() => {
        ws.close();
      }, 2000);
    };
  }, []);

  function sendWebSocketData(RGBData, XData, YData) {
    if (ws && ws.readyState === WebSocket.OPEN) {
      const data = {
        action: 'post',
        roomID: props.data,
        user: props.username,
        RGB: RGBData,
        x: XData,
        y: YData,
      };
      ws.send(JSON.stringify(data));
      ws.addEventListener('message', (event) => {
        if(event.data == "Server and the clients are updated") {
          setRGBData([]);
          setXData([]);
          setYData([]);
        }
      });
    }
  }

  function startDrawing(event) {
    setIsDrawing(true);
    setLastX(event.nativeEvent.offsetX);
    setLastY(event.nativeEvent.offsetY);
  }

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
    sendWebSocketData(RGBData, XData, YData);
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

  async function handleGetRGBData() {
    const canvas = canvasRef.current;
    const context = canvas.getContext('2d');
    const imageData = context.getImageData(0, 0, canvas.width, canvas.height);
    const pixelData = imageData.data;
    console.log(pixelData)
  
    const response = await axios.get(`https://hbzwo0rl65.execute-api.us-east-1.amazonaws.com/dev/cpen391?RoomID=5232`);
    const receivedRGBData = response.data.RGB;
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
  }

  return (
    <div>
      <div className="canvas-container">
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
