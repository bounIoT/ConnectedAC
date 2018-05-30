
# Description of the project

Automatically controlled air conditioner system which turns on and off the air conditioner depending on the instantaneous weather.

# Team Members

Çağrı Kılıçoğlu   
Abdulsamed Bayrakoğlu

### Hardware setup

Components Used:    
- IR sender   
- ESP 8266

Connections: 

- IR sender GND  to ESP 8266   
- IR sender V to ESP 8266 D7

### Flow of data 

Instantaneous weather is coming to the ESP 8266 every five minutes.   
If the temperature is above a certain level ESP 8266 send ON signal to the air conditioner via IR sender
If the temperature is below a certain level ESP 8266 send OFF signal to the air conditioner via IR sender


### Development Environment

Development Operating System: MacOS High Sierra 10.13.4   
Development tool: nodeJs
