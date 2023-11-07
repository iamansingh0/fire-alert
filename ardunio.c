#include <WiFi.h>
#include <HTTPClient.h>
#include <Adafruit_Sensor.h>
#include "DHT.h"
#include <math.h>
#define DHTTYPE DHT11   
#define DHTPIN 23
int Gas_analog = 32;  
int Fire_analog = 33; 
int Buzzer=12;
int alert=0;

int randomNumber;
  
const char* ssid = "Helooo";
const char* password = "123456789";


DHT dht(DHTPIN, DHTTYPE);
//Your Domain name with URL path or IP address with path
// const char* serverName = "http://192.168.94.81:5000/sensordata/";//devanshu
const char* serverName = "http://192.168.94.109:5000/sensordata/";
unsigned long lastTime = 0;
unsigned long timerDelay = 2000;

float flame_val=0;
float sensitivity=0;

float temperature=0;
float sensitivity_temp =0;

float gas=0;
float sensitivity_gas =0;

float humidity=0;
float sensitivity_humid =0;
float intensity_reading;
float intensity;

float Sens_gas; 
float Sens_flame ;
float Sens_temp ;
float Sens_hum;

void setup() {
  Serial.begin(115200);
  pinMode(Buzzer, OUTPUT); 
  dht.begin();  
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
 
  Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");
}

void loop() {
    int gassensorAnalog = analogRead(Gas_analog);
    int firesensorAnalog = analogRead(Fire_analog);
  if ((millis() - lastTime) > timerDelay) {
    if(WiFi.status()== WL_CONNECTED){
      WiFiClient client;
      HTTPClient http;    
      http.begin(client, serverName);

    //reading sensor data

//      float h = dht.readHumidity();
//      float t = dht.readTemperature();
//      float hic = dht.computeHeatIndex(t, h, false);

for (int i = 1; i <= 50; i++) {
//flame sensor
         float baseline_reading=4010;
    intensity = i * 100;
   // Serial.print("Flame Sensor Reading: ");
    flame_val = analogRead(Fire_analog);
    //Serial.println(flame_val);
     sensitivity = (float)(intensity_reading - baseline_reading) / (float)intensity;
    Sens_flame = abs(sensitivity);

//temperature sensor
    float baseline_temperature;
    baseline_temperature = 30.2;
    randomNumber=random(20,30);
   // Serial.print("Temperature Reading: ");
    temperature = dht.readTemperature();
    //Serial.println(temperature);
    sensitivity_temp = (temperature - baseline_temperature) / (randomNumber - baseline_temperature);
    Sens_temp = abs(sensitivity_temp);

//gas sensor

    float propane_concentration;
    float baseline_gas;
    
    baseline_gas = 1000;
    float val=50*random(5,10);
    propane_concentration = val / 50.0; // Convert to fraction
   // Serial.print("Gas Reading: ");
    gas = analogRead(Gas_analog);
   // Serial.println(gas);
    sensitivity_gas = (gas - baseline_gas) / propane_concentration;
   // Serial.print("Sensitivity to gas: ");
   // Serial.println(abs(sensitivity_gas));
    Sens_gas = abs(sensitivity_gas/50);

   

//humidity sensor

humidity = dht.readHumidity();
   // Serial.print("Humidity Reading: ");
  //  Serial.println(humidity);
 sensitivity_humid = random(1.5,3.5) / humidity * 100.0;
   // Serial.print("Sensitivity of humidity: ");
   // Serial.println(sensitivity_humid);
 Sens_hum = abs(sensitivity_humid);

    String data_s=String(millis()) + "," + String(flame_val) + "," + String(Sens_flame) + "," + String(temperature)+ "," + String(Sens_temp) + "," + String(gas) + "," + String(Sens_gas)+ "," + String(humidity) + "," + String(Sens_hum);
    // Serial.println(data_s);
 
}//for loop end
    //posting sensor data
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
      String httpRequestData = 
                                "fire="+String(flame_val)+
                                "&gas="+String(gas)+
                                "&temp="+String(temperature)+
                                "&humidity="+String(humidity)+
                                "&fire_s="+String(Sens_flame)+
                                "&gas_s="+String(Sens_gas)+
                                "&temp_s="+String(Sens_temp)+
                                "&humidity_s="+String(Sens_hum);
                                
      int httpResponseCode = http.POST(httpRequestData);
      Serial.println(httpRequestData);
      Serial.print("HTTP Response code: "+String(httpResponseCode));
      //Serial.println(httpResponseCode);
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}