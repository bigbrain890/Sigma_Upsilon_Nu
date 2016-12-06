#define SHUTDOWN_SERVO 1
#define ACTIVATE_SERVO 2
#define COMPARE_LIGHT_CHANGE 3 
#define CALC_TARG_ANGLE 4
#define PULL_SENSOR_DATA 5
#define FIND_INITIAL_POSITION 6
#define GO_TO_INITIAL_POSITION 7
#define ADJUST_INITIAL_TILT 8
#define ACTIVE_TRACKING 9
#define TRACK_SUN 3


#define VERTICLE 0
#define FLAT 90
#include <Servo.h>

Servo radial, tilt;           // Make two servo objects.
int photoSensorLeft = A0;        // Pin allocation for first 5 photosensors. Going to use analog inputs to get variable readings.
int photoSensorRight = A1;  
int photoSensorTop = A2;
int photoSensorBottom = A3;
int tempSensor = A5;          // Tempreature sensor for weather data we need to send.

float lightSensorData[4];     // Array used for storing analog photosensor data
float temp = 0;               // Variable used for storing tempreature reading.
float tempF = 0;
float rawTemp = 0;
int lastHeading = 0;          // Last known direction of solar panel
int targetHeading = 0;        // Where we want the solar panel to point.
int currentLightDirection = 0;
int currentTilt = 0;
int lastLightDirection = 0;   // The last direction the solar panel was towards
unsigned long int currentTime = 0;
unsigned long int lastReportTime;
int reportFreq = 5000;
int startingLightDirection = 0;
int maxTiltErrorThresh = 30;
int startingLightTilt = 0;
int noLightThresh = 0;        // Set value to minimum light in classroom. Do with function.
int state = FIND_INITIAL_POSITION;
int activeTrackingState = 1;
int solarErrorThresh = 50;


void setup() 
{
  radial.attach(9);                   // Attach the Servos to the perspective pins.
  tilt.attach(10);                    
  Serial.begin(9600);
  tilt.write(45);
  radial.write(0);
  delay(500);

}

void loop() 
{
  
  currentTime = millis();
  if ((currentTime - lastReportTime) >= reportFreq)
  {
    dataReport();
    lastReportTime = currentTime; 
  }

  if (state == FIND_INITIAL_POSITION)
  {
  	int brightestLight = 0;
  	int currentLight = 0;

    for(int i = 0; i <= 180; i++)
    {
    	currentLight = (analogRead(photoSensorLeft) + analogRead(photoSensorRight));
    	if(currentLight > brightestLight)
    	{
    		brightestLight = currentLight;
    		startingLightDirection = i; 
    		startingLightTilt = 45;
    	}
    	radial.write(i+1);
    	delay(50);
    }

    tilt.write(135);
    delay(1000);

    for(int i = 180; i >= 0; i--)
    {
 		currentLight = (analogRead(photoSensorLeft) + analogRead(photoSensorRight));
 		if(currentLight > brightestLight)
 		{
 			brightestLight = currentLight;
 			startingLightDirection = i;
 			startingLightTilt = 135;
 		}
 		radial.write(i-1);
 		delay(50);
    }

    state = GO_TO_INITIAL_POSITION;
  }

  else if (state == GO_TO_INITIAL_POSITION)
  {
  	radial.write(startingLightDirection);
  	tilt.write(startingLightTilt);
  	currentTilt = startingLightTilt;
  	currentLightDirection = startingLightDirection;
  	currentLightDirection = startingLightDirection;
  	delay(1000);
  	state == ADJUST_INITIAL_TILT;

  }

  else if (state == ADJUST_INITIAL_TILT)
  {
  	int topValue = analogRead(photoSensorTop);
  	int bottomValue = analogRead(photoSensorBottom);
  	int difference = abs(bottomValue - topValue);

  	if(difference > maxTiltErrorThresh)
  	{
  		if(topValue > bottomValue)
  		{
  			currentTilt--; 			// Change based on servo direction and stuff
  		}
  		else if (bottomValue > topValue )
  		{
  			currentTilt++;			// May have to flip this one with the other value
  		}
  		tilt.write(currentTilt);
  	}

  	else 
  	{
  		state = ACTIVE_TRACKING;
  	}
  	
  }
  else if (state == ACTIVE_TRACKING)
  {
  	if(activeTrackingState == SHUTDOWN_SERVO)
  	{
  		tilt.detach();
  		radial.detach();
  		int leftLight = analogRead(photoSensorLeft);
  		int rightLight = analogRead(photoSensorRight);
  		int error = (abs(leftLight - rightLight));
  		if (error > solarErrorThresh)
  		{
  			activeTrackingState = ACTIVATE_SERVO;
  		}
  	}
  	else if (activeTrackingState == ACTIVATE_SERVO)
  	{
  		tilt.attach(10);
  		radial.attach(9);
  		activeTrackingState = TRACK_SUN;
  	}
  	else if (activeTrackingState == TRACK_SUN)
  	{
  		int leftLight = analogRead(photoSensorLeft);
  		int rightLight = analogRead(photoSensorRight);
  		int error = (abs(leftLight - rightLight)); 
  		if(error < )
  	}

  }
}

void collectLightData()
{
  lightSensorData[0] = analogRead(photoSensorLeft);
  lightSensorData[1] = analogRead(photoSensorRight);
  lightSensorData[2] = analogRead(photoSensorTop);
  lightSensorData[3] = analogRead(photoSensorBottom);
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

