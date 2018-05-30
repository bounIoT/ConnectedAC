const app = require('express')();
const axios = require('axios');

const weatherUrl = 'http://api.openweathermap.org/data/2.5/weather?id=745044&units=metric&appid=029b4af712ebcfa50858fcf0185c8e0a';

// BOUN Class Hours, e.g. 1 = 9:00
const sampleClassData = [1, 3, 4, 5, 8];


app.use(cors());
app.set('x-powered-by', false);

app.get('/today', (req, res) => {
  // Using GMT 0 to eliminate Timezone Issues
  const date = new Date();
  date.setUTCHours(0);
  date.setUTCMilliseconds(0);
  date.setUTCMinutes(0);
  date.setUTCSeconds(0);
  const epochToday = date / 1000;
  // one hour = 3600
  console.log(epochToday);
  const sessions = [
    {start: epochToday + (8 * 3600) + (45 * 60), end: epochToday + (9 * 3600) + (50 * 60)},
    {start: epochToday + (10 * 3600) + (45 * 60), end: epochToday + (13 * 3600) + (50 * 60)},
    {start: epochToday + (15 * 3600) + (45 * 60), end: epochToday + (16 * 3600) + (50 * 60)},
  ];

  axios.get(weatherUrl)
    .then(response => {
      console.log(response.data);
      const average = Math.round((response.data.main.temp_min + response.data.main.temp_max)/2);
      res.json({temp: average, sessions});
    })
    .catch(e => {
      console.log(e);
      res.json(e);
    });

});

app.listen(9000, () => console.log('App listening on port 9000'));
