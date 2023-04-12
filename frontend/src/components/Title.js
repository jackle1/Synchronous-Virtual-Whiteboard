import React from 'react';
import '../App.css';
import '../components/Title.css';

function Title() {
  return (
    <div className='title-container'>
        <h1>Our Product</h1>
        <p> A touch screen pad and camera driven by a DE1-SoC and accompanying web application 
            that will allow users to collaborate on a virtual whiteboard. The DE1 will use a Wifi module 
            to connect to a web server to send user input to a communal whiteboard. 
            The DE1 will also use a touch screen for users to both draw on and display the current communal whiteboard. 
            Users collaborating can use the same room password to share the same whiteboard. 
            To join a shared whiteboard, DE1 users will enter the ID number using the switches on the DE1-SoC, 
            while web app users can type in the ID on the application.</p>

        

        <div className='title-description'>
        The web application will give users the option to either create or join a room. 
        If create room is selected, a room password will be generated and users can input the ID onto their De1-SoC boards 
        to join the room and start drawing. If users want to join the room from another computer, they can also input the room 
        password onto their web application. The web server will use a database to synchronize the current view of the communal 
        whiteboard across all devices (DE1 or web app) for users to view and draw on. Users can also take photos through webcam 
        (on web app) or the camera connected to the DE1 (on touch screen). 
        These photos will be displayed and positioned on the whiteboard for other users to see and draw on top of. </div>        
    </div>
  )
}

export default Title