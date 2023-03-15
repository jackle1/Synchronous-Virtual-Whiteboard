const url = "https://2hanc5qruj.execute-api.us-east-1.amazonaws.com/dev"
const data = 
{
	"password": 2361,
	"member": "Ranbir",
	"roomID": 2
};


var e = fetch(url, {
  method: 'POST',
  body: JSON.stringify(data)
})
  .then(response => response.json())
  .then(data => console.log(data))
  .catch(error => console.error(error));

console.log(e)
