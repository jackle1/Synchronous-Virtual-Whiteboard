const url = "https://hbzwo0rl65.execute-api.us-east-1.amazonaws.com/dev/cpen391"
const data = 
{
    "member": "Ranbir",
      "roomID": 2,
      "password": 2361,
      "R-values" : [1],
      "G-values" : [1],
      "B-values" : [1],
      "request-for": 2,
      "x": 1,
      "y": 1
};



fetch(url, {
  method: 'POST',
  body: JSON.stringify(data)
})
  .then(response => response.json())
  .then(data => console.log(data))
  .catch(error => console.error(error));
