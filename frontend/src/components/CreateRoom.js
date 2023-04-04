import React, { useState, useEffect, useRef } from 'react';
import './CreateRoom.css';
import DrawingCanvas from './DrawingCanvas';
import axios from 'axios';

function CreateRoom() {
  const [backgroundImage, setBackgroundImage] = useState("");

  const [childVariable, setChildVariable] = useState(null);
  const [membersList, setMembersList] = useState(null);
  const prevChildVariableRef = useRef();

  function handleChildVariable(variable) {
    console.log(variable);
    setChildVariable(variable);
  }

  function handleMembersList(childVariable) {
    setMembersList(childVariable);
  }

  useEffect(() => {
    console.log("Child variable changed:", childVariable);
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
      {/* <h1>
       Upload a background to draw on: 
      </h1>
      <div>
        <input type="file" onChange={handleImageUpload} />
      </div> */}

      <h1>
        RoomID: 
        <p>{data["RoomID"]}</p>
      </h1>

      <h1>
        Members:
        <p>
          {membersList && membersList.map((member, index) => (
            <React.Fragment key={index}>
              {member}
              <br />
            </React.Fragment>
          ))}
        </p>
      </h1>

      <DrawingCanvas data={8862} onChildVariable={handleChildVariable} username={"CreateroomUser"}/>

      {/* data={data["RoomID"]} */}
    </div>
  )
}

export default CreateRoom