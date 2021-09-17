#include "DHT.h"

#define DHTTYPE DHT22 // DHT11 also accepted

struct DEV_HumiditySensor :  Service::HumidifierDehumidifier {
 
  SpanCharacteristic *active;
  SpanCharacteristic *humid;
  SpanCharacteristic *currentstate;
  SpanCharacteristic *targetstate;
  SpanCharacteristic *maxhumid;
  SpanCharacteristic *water;

  int dhtpin; 
  int presistpin;
  int lightvalue=50; 
  uint32_t timer=0;


  DEV_HumiditySensor(int dhtpin, int presistpin) : Service::HumidifierDehumidifier(){ 

    this->dhtpin=dhtpin;
    this->presistpin=presistpin;
    
    pinMode(dhtpin, INPUT);
    pinMode(presistpin, INPUT);

    new Characteristic::Active();
    humid=new Characteristic::CurrentRelativeHumidity(0);
    new Characteristic::CurrentHumidifierDehumidifierState();
    new Characteristic::TargetHumidifierDehumidifierState();
    maxhumid=new Characteristic::RelativeHumidityDehumidifierThreshold(0);
    water=new Characteristic::WaterLevel(0);

    //active->setRange(0,1);
    //humid->setRange(0,100);
    //currentstate->setRange(0,1);
    //targetstate->setRange(0,1);
    //maxhumid->setRange(0,100,1);
    //water->setRange(0,100);
    
    Serial.print("Configuring Humidity Sensor");
    Serial.print("\n");

  }

  boolean update(){   

    if(maxhumid->getNewVal()>humid->getVal()){ 
      LOG1("Turn ON Dehumidifier\n");
    } else 
    if(maxhumid->getNewVal()<humid->getVal()){ 
      LOG1("Turn OFF Dehumidifier\n");
    }

    return(true);
  }

  void loop(){
  
    if(millis()-timer>2000){  
      timer=millis();
      
      DHT dht(dhtpin, DHTTYPE);
      dht.begin();
      float humidity=dht.readHumidity();
      if ( humidity >= 0 && humidity <= 100){
        humid->setVal(humidity);
      }
      
      lightvalue=analogRead(presistpin);
      if ( lightvalue >= 500){
        water->setVal(100);
      }
      else if ( lightvalue <= 500){
        water->setVal(0);
      }
      LOG1("Humidity Update: ");
      LOG1(humidity);
      LOG1("\n");
      LOG1("Photo resistor value: ");
      LOG1(lightvalue);
      LOG1("\n");            
    }
    
  }
  
};

struct DEV_TempSensor : Service::TemperatureSensor { 

  SpanCharacteristic *temp;
  int dhtpin;
  uint32_t timer=0; 
  
  DEV_TempSensor(int dhtpin) : Service::TemperatureSensor(){ 

    this->dhtpin=dhtpin;
    
    pinMode(dhtpin, INPUT);

    temp=new Characteristic::CurrentTemperature(0);
    temp->setRange(-40,125);
        
    Serial.print("Configuring Temperature Sensor");
    Serial.print("\n");
    
  }

  void loop(){

    if(millis()-timer>2000){  
      timer=millis();
      DHT dht(dhtpin, DHTTYPE);
      dht.begin();
      float temperature=dht.readTemperature();
      if (temperature >= -40 && temperature <= 125){
        temp->setVal(temperature);
      }
      LOG1("Temperature Update: ");
      LOG1(temperature);
      LOG1("\n");            
    }
    
  }
  
};

struct DEV_LeakSensor : Service::LeakSensor { 
  SpanCharacteristic *full;

  int presistpin;
  int lightvalue=50;
  uint32_t timer=0; 
  
  DEV_LeakSensor(int presistpin) : Service::LeakSensor(){ 

    this->presistpin=presistpin;
    
    pinMode(presistpin, INPUT);

    full=new Characteristic::LeakDetected(0);
        
    Serial.print("Configuring Full Sensor");
    Serial.print("\n");
    
  }

  void loop(){

    if(millis()-timer>2000){  
      timer=millis();
      lightvalue=analogRead(presistpin);
      if ( lightvalue >= 500){
        full->setVal(1);
        LOG1("Bucket Full");
      }
      else if ( lightvalue <= 500){
        full->setVal(0);
      }
              
    }
    
  }
  
};

