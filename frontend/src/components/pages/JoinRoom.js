import React, { useState } from "react";
import { useNavigate } from "react-router-dom";
import "../pages/JoinRoom.css";

export default function JoinRoom() {
  const [username, setUsername] = useState("");
  const [roomID, setroomID] = useState("");
  const navigate = useNavigate();

  return (
    <div className="joinroom-container">
      <h1>Please Enter Room Code</h1>
      <form
        onSubmit={(e) => {
          e.preventDefault();
          navigate("/join-room-draw", { state: { username, roomID } });
        }}
      >
        <label>
          <p>Username</p>
          <input
            className="joinroom-textbox"
            type="text"
            value={username}
            onChange={(e) => setUsername(e.target.value)}
          />
        </label>
        <label>
          <p>Room ID</p>
          <input
            className="joinroom-textbox"
            type="text"
            value={roomID}
            onChange={(e) => setroomID(e.target.value)}
          />
        </label>
        <div>
          <button className="joinroom-button" type="submit">
            Submit
          </button>
        </div>
      </form>
    </div>
  );
}