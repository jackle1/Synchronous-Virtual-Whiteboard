import React from 'react';
import '../App.css';
import { Button } from './Button';
import './RoomSection.css';
import { JoinButton } from './JoinButton';

function RoomSection() {
  return (
    <div className='room-container'>
      <h1>BEGIN DRAWING</h1>
      <p>Create or join a room?</p>
      <div className='room-btns'>
        <Button
          className='btns'
          buttonStyle='btn--outline'
          buttonSize='btn--large'
        >
          CREATE ROOM
        </Button>
        <JoinButton
          className='btns'
          buttonStyle='btn--primary'
          buttonSize='btn--large'
        >
          JOIN ROOM
        </JoinButton>
      </div>
    </div>
  );
}

export default RoomSection;
