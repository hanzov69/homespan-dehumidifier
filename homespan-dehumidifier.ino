#include "HomeSpan.h"
#include "DEV_DHT.h"
#include "DEV_IDENTIFY.h"

void setup() {               

  Serial.begin(115200);  
  
  homeSpan.setControlPin(5);
  homeSpan.setStatusPin(21);
  const uint8_t dhtpin = 19; // DHT data pin
  const uint8_t presistpin = 32; // ADC pin used for photo resistor analog values - look at pinout to determine isolation from other in use pins

  homeSpan.begin(Category::Dehumidifiers,"HomeSpan Dehumidifier");

  new SpanAccessory();  
  new DEV_Identify("Dehumidifier","HomeSpan","123-ABC","HS Dehumidifier","0.9",3);
  new Service::HAPProtocolInformation();
    new Characteristic::Version("1.1.0");
    
  new SpanAccessory();                                                          
    new DEV_Identify("Dehumidifier","HomeSpan","123-ABC","Humidity Sensor","0.9",0);
    new DEV_HumiditySensor(dhtpin, presistpin); 
  
  new SpanAccessory();                                                          
    new DEV_Identify("Dehumidifier","HomeSpan","123-ABC","Full Sensor","0.9",0);
    new DEV_LeakSensor(presistpin);

  new SpanAccessory();                                                          
    new DEV_Identify("Dehumidifier","HomeSpan","123-ABC","Temperature Sensor","0.9",0);
    new DEV_TempSensor(dhtpin); 
} 
void loop(){

  homeSpan.poll();  
  
} 