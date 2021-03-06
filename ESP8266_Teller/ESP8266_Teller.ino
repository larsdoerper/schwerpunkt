#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

//SSID
//PWD
const char* ssid = "**********************";
const char* password = "**********************";

// GPIO where the DS18B20 is connected to
const int oneWireBus = 5;   //D1

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);


// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

 
void setup () {
  pinMode(10, HIGH);  
 
  Serial.begin(19200);
  sensors.begin();
 
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
 
    delay(1000);
    Serial.print("Connecting..");
 
  }
 
}
 
void loop() {
  temperatur();
  delay(10);
 
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
 
    HTTPClient http;  //Declare an object of class HTTPClient
 
    http.begin("http://192.168.178.116:1337/login?name=" + String(temperatur(), 2));  //Specify request destination
    int httpCode = http.GET();                                  //Send the request
    delay(10);
 
    if (httpCode > 0) { //Check the returning code
 
      String payload = http.getString();   //Get the request response payload
      Serial.println(payload);             //Print the response payload
 
    }
    delay(10);
 
    http.end();   //Close connection
 
  }
  delay(10);
  if (temperatur() < 70){
    digitalWrite(10, HIGH);
    Serial.print("Hallo");
  }
  else{
    digitalWrite(10, LOW);
  }
 
  delay(300);    //Send a request every 30 seconds
}

float temperatur(){
  sensors.requestTemperatures(); 
  float temperatureC = sensors.getTempCByIndex(0);
  //float temperatureF = sensors.getTempFByIndex(0);
  
  
  Serial.print(temperatureC);
  Serial.println("ºC");
  //Serial.print(temperatureF);
  //Serial.println("ºF");


  return temperatureC;
}
