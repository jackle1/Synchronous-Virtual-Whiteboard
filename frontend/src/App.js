
import React from 'react';
import Navbar from './components/Navbar';
import './App.css';
import { BrowserRouter as Router, Routes, Route } from 'react-router-dom';
import Home from './components/pages/Home';
import CreateRoom from './components/CreateRoom';
import Product from './components/pages/Product';
import Contact from './components/pages/Contact';
import JoinRoom from './components/pages/JoinRoom';
import JoinRoomDraw from './components/JoinRoomDraw';

function App() {

  return (
    <>
      <Router>
        <Navbar />
        <Routes>
          <Route path='/' exact element={<Home />} />
          <Route path='/create-room-draw' exact element={<CreateRoom />} />
          <Route path='/product' exact element={<Product />} />
          <Route path='/contact' exact element={<Contact />} />
          <Route path='/join-room' exact element={<JoinRoom />} />
          <Route path='/join-room-draw' exact element={<JoinRoomDraw />} />
        </Routes>
      </Router>
    </>
  );
}

export default App;