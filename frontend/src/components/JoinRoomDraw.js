import React, { useState, useEffect } from 'react';
import './JoinRoomDraw.css';
import { useLocation } from "react-router-dom";
import axios from 'axios';
import DrawingCanvas from './DrawingCanvas';

function JoinRoomDraw() {
  const location = useLocation();
  const { username, roomID } = location.state;
  const [backgroundImage, setBackgroundImage] = useState("");

  function handleImageUpload(event) {
    const file = event.target.files[0];
    const reader = new FileReader();
    reader.readAsDataURL(file);
    reader.onloadend = () => {
      setBackgroundImage(reader.result);
    };
  }

//   const [data, setData] = useState([]);

//   useEffect(() => {
//     axios.get('`https://hbzwo0rl65.execute-api.us-east-1.amazonaws.com/dev/cpen391?RoomID=${roomID}')
//       .then(response => {
//         setData(response.data);
//         console.log(response.data);
//         console.log(response.data["RoomID"]);
//       })
//       .catch(error => {
//         console.error('Error fetching data:', error);
//       });
//   }, []);

  return (
    <div className='createroom-container' style={{ backgroundImage: `url(${backgroundImage})` }}>
      <h1>
       Upload a background to draw on: 
      </h1>
      <div>
        <input type="file" onChange={handleImageUpload} />
      </div>

      <h1>
        RoomID: 
        <p>{roomID}</p>
      </h1>

      <DrawingCanvas data={roomID}/>
    </div>
  )
}

export default JoinRoomDraw