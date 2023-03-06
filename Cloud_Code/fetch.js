//const url = "https://7jasr5uat3.execute-api.us-east-1.amazonaws.com/dev";
// const url = "https://hbzwo0rl65.execute-api.us-east-1.amazonaws.com/dev/cpen391";

const url = "https://2hanc5qruj.execute-api.us-east-1.amazonaws.com/dev"
const data = 
{
  "member": "Ranbir1",
    "roomID": 2,
    "password": 2361,
    "R-values" : [3],
    "G-values" : [3],
    "B-values" : [3],
    "request-for": 1,
    "x": 0,
    "y": 0
  };
  // const data = 
  // {
  //   "password": 1234,
  //   "member": "Ranbir",
  //   "roomID": 0
  // };
  


// const headers = new Headers({ 'Content-Type': 'application/json' });

fetch(url, {
  method: 'POST',
//   headers: headers,
  body: JSON.stringify(data)
})
  .then(response => response.json())
  .then(data => console.log(data))
  .catch(error => console.error(error));
