import React, { useState } from "react";
import '../pages/Contact.css';

const FORM_ENDPOINT = ""; // TODO - fill on the later step

const ContactForm = () => {
  const [submitted, setSubmitted] = useState(false);
  const handleSubmit = () => {
    // setTimeout(() => {
      setSubmitted(true);
    // }, 100);
  };

  if (submitted) {
    return (
      <>
        <div className="contact-container">
            <h1>Thank you!</h1>
            <p>We'll be in touch soon.</p>
        </div>
      </>
    );
  }

  return (
    // <form
    //   // action={FORM_ENDPOINT}
    //   onSubmit={handleSubmit}
    //   // method="POST"
    //   // target="_blank"
    // >
    //     <div className="contact-container">
    //     <h1>
    //         Contact Us:
    //     </h1>
    //   <div>
    //     <input className="textbox" type="text" placeholder="Your name" name="name" required />
    //   </div>
    //   <div>
    //     <input className="textbox" type="email" placeholder="Email" name="email" required />
    //   </div>
    //   <div>
    //     <textarea className="messagebox" placeholder="Your message" name="message" required />
    //   </div>
    //   <div>
    //     <button type="submit"> Send a message </button>
    //   </div>
    //   </div>
    // </form>
    <div className="contact-container">
      <h1>
        Founders:
      </h1>
      <h2>
        Jack Le
      </h2>
      <h2>
        Jacky Le
      </h2>
      <h2>
        Jane Wu
      </h2>
      <h2>
        Ranbir Sharma
      </h2>
      <h1>
        Contant Info:
      </h1>
      <h2>
        Please email jackcjle@gmail.com for any questions or concern about our service!
      </h2>
    </div>
    
  );
};

export default ContactForm;