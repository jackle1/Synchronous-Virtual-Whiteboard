import axios from 'axios';
import '../App.css';

import React,{Component} from 'react';
  
class CreateRoom extends Component {

   
    state = {
  
      // Initially, no file is selected
      selectedFile: null
    };

    // On file select (from the pop up)
    onFileChange = event => {
     
      // Update the state
      this.setState({ selectedFile: event.target.files[0] });

      // // Create an object of formData
      // const formData = new FormData();

      // // Update the formData object
      // formData.append(
      //   "myFile",
      //   this.state.selectedFile,
      //   this.state.selectedFile.name
      // );
      
      // // Details of the uploaded file
      // console.log(this.state.selectedFile);

      // // Request made to the backend api
      // // Send formData object
      // axios.post("api/uploadfile", formData);

      const image = event.target.files[0];

      image.addEventListener('change', function() {
        const bgUrl = URL.createObjectUrl(image);

        const createroom = document.getElementById('create-room');

        createroom.style.backgroundImage = `url(${bgUrl})`;
      });
     
    };


     
    render() {
      return (
        <div className='create-room'>
            <h1>
               Upload a background to draw on: 
            </h1>
            <div>
                <input type="file" onChange={this.onFileChange} />
            </div>
        </div>
      );
    }
  }
  
  export default CreateRoom;