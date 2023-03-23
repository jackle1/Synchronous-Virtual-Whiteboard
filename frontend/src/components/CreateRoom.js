import React, { useState, useEffect } from 'react';
import './CreateRoom.css';
import DrawingCanvas from './DrawingCanvas';
import axios from 'axios';

function CreateRoom() {
  const [backgroundImage, setBackgroundImage] = useState("");

  function handleImageUpload(event) {
    const file = event.target.files[0];
    const reader = new FileReader();
    reader.readAsDataURL(file);
    reader.onloadend = () => {
      setBackgroundImage(reader.result);
    };
  }

  const [data, setData] = useState([]);

  var properties = {
    "member": "Ranbir",
    "roomID": 2,
    "RGB" : 1,
    "request-for": 0,
    "x": 1,
    "y": 1
  }

  useEffect(() => {
    axios.post('https://hbzwo0rl65.execute-api.us-east-1.amazonaws.com/dev/cpen391', properties)
      .then(response => {
        setData(response.data);
        console.log(response.data);
        console.log(response.data["RoomID"]);
      })
      .catch(error => {
        console.error('Error fetching data:', error);
      });
  }, []);

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
        <p>{data["RoomID"]}</p>
      </h1>

      <DrawingCanvas data={data["RoomID"]}/>
    </div>
  )
}

export default CreateRoom