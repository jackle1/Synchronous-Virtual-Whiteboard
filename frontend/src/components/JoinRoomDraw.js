import React, { useState, useEffect, useRef } from 'react';
import './JoinRoomDraw.css';
import { useLocation } from "react-router-dom";
import axios from 'axios';
import DrawingCanvas from './DrawingCanvas';

function JoinRoomDraw() {
  const location = useLocation();
  const { username, roomID } = location.state;
  const [backgroundImage, setBackgroundImage] = useState("");

  const [childVariable, setChildVariable] = useState(null);
  const [membersList, setMembersList] = useState(null);
  const prevChildVariableRef = useRef();

  function handleChildVariable(variable) {
    setChildVariable(variable);
  }

  function handleMembersList(childVariable) {
    setMembersList(childVariable);
  }

  useEffect(() => {
    if (childVariable !== prevChildVariableRef.current && childVariable !== undefined) {
      handleMembersList(childVariable);
      console.log("Members list updated:", membersList);
    }
    prevChildVariableRef.current = childVariable;
  }, [childVariable]);

  function handleImageUpload(event) {
    const file = event.target.files[0];
    const reader = new FileReader();
    reader.readAsDataURL(file);
    reader.onloadend = () => {
      setBackgroundImage(reader.result);
    };
  }

  return (
    <div className='createroom-container'>
      {/* style={{ backgroundImage: `url(${backgroundImage})` }} */}
      {/* <h1>
       Upload a background to draw on: 
      </h1>
      <div>
        <input type="file" onChange={handleImageUpload} />
      </div> */}

      <h1>
        RoomID: 
      </h1>
      <h2>{roomID}</h2>

      <h1>
        Members:
      </h1>
      <h2>
          {membersList && membersList.map((member, index) => (
            <React.Fragment key={index}>
              {member}
              <br />
            </React.Fragment>
          ))}
        </h2>

      <DrawingCanvas roomNumber={parseInt(roomID)} onChildVariable={handleChildVariable} username={username}/>
    </div>
  )
}

export default JoinRoomDraw