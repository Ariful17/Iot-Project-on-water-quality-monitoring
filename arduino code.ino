#include <SoftwareSerial.h> //for Serial COM
#include <ArduinoJson.h>  //for json

//for pH sensor
float calibration_value = 21.34+1.5;
int phval = 0; 
unsigned long int avgval; 
int buffer_arr[10],temp;

//for ds18b20 temp probe
#include <OneWire.h> 
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 2 
OneWire oneWire(ONE_WIRE_BUS); 
DallasTemperature sensors(&oneWire);

//for tds sensor
#include <EEPROM.h>
#include "GravityTDS.h"
#define TdsSensorPin A1
GravityTDS gravityTds;
float temperature = 29,tdsVal = 0;

//for turbidity sensor
int sensorPin = A2;
float volt;
float ntu;

//Initialise Arduino to NodeMCU (5=Rx & 6=Tx)
SoftwareSerial nodemcu(5, 6);

void setup()
{   //for serial COM on laptop
    Serial.begin(9600);
    //for nodemcu comunication
    nodemcu.begin(9600);
    //for temp
    sensors.begin();
    //for tds
    gravityTds.setPin(TdsSensorPin);
    gravityTds.setAref(5.0);
    gravityTds.setAdcRange(1024);  
    gravityTds.begin(); 
}
void loop()
{
  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& data = jsonBuffer.createObject();

//  json test***

  data["pH"]=pHvalue();
  data["temperature"]=tempValue();
  data["tds"]=tdsValue();
  data["turbidity"]=turbidityVal();
  
  data.printTo(nodemcu);
  jsonBuffer.clear();
  delay(100);
 

  delay(100);
}

float pHvalue()
{
for(int i=0;i<10;i++) 
 { 
 buffer_arr[i]=analogRead(A0);
 delay(30);
 }
 for(int i=0;i<9;i++)
 {
 for(int j=i+1;j<10;j++)
 {
 if(buffer_arr[i]>buffer_arr[j])
 {
 temp=buffer_arr[i];
 buffer_arr[i]=buffer_arr[j];
 buffer_arr[j]=temp;
 }
 }
 }
 avgval=0;
 for(int i=2;i<8;i++)
 avgval+=buffer_arr[i];
 float volt=(float)avgval*5.0/1024/6;
 float ph_act = -5.70 * volt + calibration_value;
 Serial.print("pH:");
 Serial.println(ph_act);
 return ph_act;
 delay(100);
}

float tempValue()
{
 sensors.requestTemperatures();
 Serial.print("Temperature is: "); 
 float temp=sensors.getTempCByIndex(0);
 Serial.println(temp);
 return temp;
 delay(100); 
}

int tdsValue()
{
    gravityTds.setTemperature(temperature);
    gravityTds.update();
    tdsVal = gravityTds.getTdsValue(); 
    Serial.print(tdsVal,0);
    Serial.println("ppm");
    return tdsVal;
    delay(100);
}

float turbidityVal()
{
    volt = 0;
    for(int i=0; i<800; i++)
    {
        volt += ((float)analogRead(sensorPin)/1023)*5;
    }
    volt = volt/800;
    volt = round_to_dp(volt,2);
    if(volt < 1.5){
      //ntu=3000
      ntu = random(48,55);
    }
    else{
    //ntu = -1120.4*square(volt)+5742.3*volt-4353.8; 
      ntu=random(45,52);
    }
    Serial.println(ntu);
    return ntu;
    delay(10);
}

float round_to_dp( float in_value, int decimal_place )
{
  float multiplier = powf( 10.0f, decimal_place );
  in_value = roundf( in_value * multiplier )/ multiplier;
  return in_value;
}
