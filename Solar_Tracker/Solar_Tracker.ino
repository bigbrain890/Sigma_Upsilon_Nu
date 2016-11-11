#include <Servo.h>

Servo radial, tilt;
int photoSensor1 = A0;
int photoSensor2 = A1;
int photoSensor3 = A2;
int photoSensor4 = A3;
int photoSensor5 = A4;
int photoSwitchTop = 3;
int photoSwitchBottom = 4;
int tempSensor = A5;
float lightSensorData[4];
float tiltUpSensor = 0;
float tiltDownSensor = 0;
float temp = 0;
int lastHeading = 0;
int targetHeading = 0;
int currentLightDirection = 0;
int lastLightDirection = 0;       //the last direction the solar panel was towards


void setup() 
{
  radial.attach(9);
  tilt.attach(10);
  pinMode(photoSwitchTop, INPUT);
  pinMode(photoSwitchBottom, INPUT);

}

void loop() 
{
  collectLightData();

}

void collectLightData()
{
  for(int i = 0; i <= 4; i++)
  {   
    lightSensorData[i] = analogRead("photoSensor"+'i');
  }

  temp = analogRead(tempSensor);
  tiltUpSensor = digitalRead(photoSwitchTop);
  tiltDownSensor = digitalRead(photoSwitchBottom);
}

void adjustTilt()
{
  
}

void activeSearch()
{
  
}
