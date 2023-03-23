import React from 'react';
import './Button.css';
import { Link } from 'react-router-dom';
import axios from 'axios';

const STYLES = ['btn--primary', 'btn--outline'];

const SIZES = ['btn--medium', 'btn--large'];

// async function postData() {
//     var data = {
//       "member": "Ranbir",
//       "roomID": 2,
//       "password": 2361,
//       "R-values" : 1,
//       "G-values" : 1,
//       "B-values" : 1,
//       "request-for": 0,
//       "x": 1,
//       "y": 1
//     }

//     try {
//       const response = await axios.post('https://hbzwo0rl65.execute-api.us-east-1.amazonaws.com/dev/cpen391', data);
//       console.log(response.data);
//       console.log(response.data["RoomID"])
//     } catch (error) {
//       console.error(error);
//     }
//   }



export const Button  = ({
    children,
    type,
    onClick,
    buttonStyle,
    buttonSize
}) => {
    const checkButtonStyle = STYLES.includes(buttonStyle) ? buttonStyle : STYLES[0]; //for default style
    const checkButtonSize = SIZES.includes(buttonSize) ? buttonSize : SIZES[0]; //for default size

    // function handleButtonClick() {
    //     postData();
    // }
    
    return (
        <Link to='/create-room-draw' className='btn-mobile'>
            <button
            className={`btn ${checkButtonStyle} ${checkButtonSize}`}
            onClick={onClick}
            type={type}
            >
                {children}
            </button>
        </Link>
    )
};