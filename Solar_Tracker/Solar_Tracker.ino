#include <Servo.h>

Servo radial, tilt;           // Make two servo objects.
int photoSensor1 = A0;        // Pin allocation for first 5 photosensors. Going to use analog inputs to get variable readings.
int photoSensor2 = A1;  
int photoSensor3 = A2;
int photoSensor4 = A3;
int photoSensor5 = A4;
int photoSwitchTop = 3;       // Put a couple of photo resistors on digital pins. Going to try and use them like photo switches.
int photoSwitchBottom = 4;
int tempSensor = A5;          // Tempreature sensor for weather data we need to send.
float lightSensorData[4];     // Array used for storing analog photosensor data
float tiltUpSensor = 0;       // Variables for storing digital photosensor data
float tiltDownSensor = 0;
float temp = 0;               // Variable used for storing tempreature reading.
int lastHeading = 0;          // Last known direction of solar panel
int targetHeading = 0;        // Where we want the solar panel to point.
int currentLightDirection = 0;
int lastLightDirection = 0;   // The last direction the solar panel was towards


void setup() 
{
  radial.attach(9);                   // Attach the Servos to the perspective pins.
  tilt.attach(10);                    
  pinMode(photoSwitchTop, INPUT);     // Make sure that digital pins for the two photoswitches know that they're inputs.
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
