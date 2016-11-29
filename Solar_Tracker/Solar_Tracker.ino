#define SHUTDOWN_SERVO 5
#define ACTIVATE_SERVO 4
#define COMPARE_LIGHT_CHANGE 3
#define CALC_TARG_ANGLE 2
#define PULL_SENSOR_DATA 1
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
float tempF = 0;
float rawTemp = 0;
int lastHeading = 0;          // Last known direction of solar panel
int targetHeading = 0;        // Where we want the solar panel to point.
int currentLightDirection = 0;
int lastLightDirection = 0;   // The last direction the solar panel was towards
unsigned long int currentTime = 0;
unsigned long int lastReportTime;
<<<<<<< HEAD
int reportFreq = 5000;
int noLightThresh = 0;        // Set value to minimum light in classroom. Do with function.
=======
int reportFreq = 500;
>>>>>>> origin/master



void setup() 
{
  radial.attach(9);                   // Attach the Servos to the perspective pins.
  tilt.attach(10);                    
  pinMode(photoSwitchTop, INPUT);     // Make sure that digital pins for the two photoswitches know that they're inputs.
  pinMode(photoSwitchBottom, INPUT);
  Serial.begin(9600);

}

void loop() 
{
  
  currentTime = millis();
  if((currentTime - lastReportTime) >= reportFreq)
  {
    dataReport();
    lastReportTime = currentTime; 
  }
  
  //collectLightData();

}

void collectLightData()
{
  for(int i = 0; i <= 4; i++)
  {   
    lightSensorData[i] = analogRead("photoSensor"+'i');
  }

  tiltUpSensor = digitalRead(photoSwitchTop);
  tiltDownSensor = digitalRead(photoSwitchBottom);
}

void adjustTilt()
{
  
}

void activeSearch()
{
  
}

void dataReport()
{
  Serial.write(12);
  rawTemp = analogRead(tempSensor);
  float voltage = (rawTemp * 5.0) / 1024.0;
  temp = (voltage - 0.5) * 100;
  tempF = (temp * 9.0/5.0) + 32.0;
  Serial.println(tempF);
  // Print out air temprature and direction sun based on pannel position.
}

