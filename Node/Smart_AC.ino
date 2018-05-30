#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <time.h>

#define IR_LED 4

IRsend irsend(IR_LED);

int khz = 38; //NB Change this default value as neccessary to the correct modulation frequency
uint16_t ON[] = {8500,4250,500,1650,500,550,550,550,500,600,500,1650,500,550,550,550,500,600,500,550,550,550,500,600,500,550,550,550,500,600,500,550,500,600,500,600,500,550,500,1650,550,1600,550,550,500,1650,550,550,500,600,500,550,500,1650,550,1600,550,1650,500};
uint16_t OFF[] = {8400,4200,450,1650,450,700,400,700,350,700,400,1700,550,600,500,600,500,550,550,1600,550,1650,500,550,500,600,500,600,500,550,500,600,500,550,550,550,500,600,500,550,550,550,500,600,500,1650,500,550,550,1600,550,600,450,600,500,550,550,1650,500}; 

const char* ssid = "CMPE_IOT";
const char* password = "cmpeiot123";

int ledPin = 13;
int timezone = 0; // 3 * 3600 GMT +3
int dst = 0;
int temp;

void setup () {
  irsend.begin();
  Serial.begin(115200);
  
  Serial.setDebugOutput(true);
  WiFi.begin(ssid, password);
  Serial.print("Connecting..");

  while (WiFi.status() != WL_CONNECTED) {
 
    Serial.print(".");
       delay(1000);
  }
  configTime(0, 0, "pool.ntp.org", "time.nist.gov");
  Serial.println("\nWaiting for Internet time");

  while(!time(nullptr)){
     Serial.print("*");
     delay(1000);
  }
  Serial.println("\nTime response....OK");
}

void checkTemp () {
  if (temp > 24) {
    Serial.println("Air conditioning is turning on");
    irsend.sendRaw(ON, 59, 38);
    Serial.println("ON");
    delay(1000);
  } else {
    Serial.println("Air conditioning is turning off");
    irsend.sendRaw(OFF, 59, 38);
    Serial.println("OFF");
    delay(1000);
  }
}
 
void loop() {

  time_t now = time(nullptr);
  //time_t now = time_t now ();
  Serial.println("\n Now will be printed: \n");
  Serial.println(now);
  //Serial.println(ctime(&now));
  Serial.print("\n");
  
  if (WiFi.status() == WL_CONNECTED) {              //Check WiFi connection status

    HTTPClient http;                                //Declare an object of class HTTPClient
  
    http.begin("http://iot.emirsavran.com/today");  //Specify request destination
    int httpCode = http.GET();                      //Send the request
  
    if (httpCode > 0) {                             //Check the returning code
  
      String payload = http.getString();            //Get the request response payload
      Serial.println(payload);                      //Print the response payload

      const size_t bufferSize = JSON_ARRAY_SIZE(3) + 4*JSON_OBJECT_SIZE(2) + 130; // parsing
      DynamicJsonBuffer jsonBuffer(bufferSize);
      JsonObject& root = jsonBuffer.parseObject(payload);

      temp = root["temp"]; //

      JsonArray& sessions = root["sessions"];

      long sessions0_start = sessions[0]["start"]; 
      long sessions0_end = sessions[0]["end"]; 

      long sessions1_start = sessions[1]["start"]; 
      long sessions1_end = sessions[1]["end"]; 

      long sessions2_start = sessions[2]["start"]; 
      long sessions2_end = sessions[2]["end"]; 

      Serial.println(temp);

      if (now > sessions0_start && now < sessions0_end) {
      //if (now > 1 && now < sessions0_end) {
        checkTemp();
      } else if (now > sessions1_start && now < sessions1_end) {
        checkTemp();
      } else if (now > sessions2_start && now < sessions2_end) {
        checkTemp();
      } else {
        // Send off signal for other times
        Serial.println("Air conditioning is turning off");
        irsend.sendRaw(OFF, 59, 38);
        Serial.println("OFF");
        delay(1000);
      }

    }
    http.end();   //Close connection
  }
 
  delay(300000);   //Send a request every 5 min.
 
}
