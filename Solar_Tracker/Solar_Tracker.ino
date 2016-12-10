#include <Servo.h>
#define SHUTDOWN_SERVO 1
#define ACTIVATE_SERVO 2
#define FIND_INITIAL_POSITION 6
#define GO_TO_INITIAL_POSITION 7
#define ADJUST_INITIAL_TILT 8
#define ACTIVE_TRACKING 9
#define TRACK_SUN 3
#define LESS_THAN_90 1
#define GREATER_THAN_90 2


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
int currentLightDirection = 0;
int currentTilt = 0;
unsigned long int currentTime = 0;
unsigned long int lastReportTime = 0;
int reportFreq = 5000;
int startingLightDirection = 0;
int maxTiltErrorThresh = 10;
int startingLightTilt = 0;
int noLightThresh = 0;        // Set value to minimum light in classroom. Do with function.
int state = FIND_INITIAL_POSITION;
int activeTrackingState = SHUTDOWN_SERVO;
int moveDirectionState = LESS_THAN_90;
int solarErrorThresh = 10;


void setup() 
{
  radial.attach(9);                   // Attach the Servos to the perspective pins.
  tilt.attach(10);                    
  Serial.begin(9600);
  tilt.write(20);
  radial.write(0);
  delay(500);

}

void loop() 
{
	/*
  Serial.print(state);
  Serial.print("   ");
  Serial.print("Active Tracking State: ");
  Serial.println(activeTrackingState);
  */
  currentTime = millis();
  if ((currentTime - lastReportTime) >= reportFreq)
  {
    dataReport();
    lastReportTime = currentTime; 
  }

  if (state == FIND_INITIAL_POSITION)
  {
  	int lowestError = 180;
  	int lightError = 0;

    for(int i = 0; i <= 180; i++)
    {
    	lightError = abs((analogRead(photoSensorLeft) - analogRead(photoSensorRight)));
    	if(lightError < lowestError)
    	{
    		lowestError = lightError;
    		startingLightDirection = i; 
    		startingLightTilt = 20;
    	}
    	radial.write(i+1);
    	delay(35);
    }

    tilt.write(170);
    delay(700);

    for(int i = 180; i >= 0; i--)
    {
 		lightError = (analogRead(photoSensorLeft) + analogRead(photoSensorRight));
 		if(lightError < lowestError)
 		{
 			lowestError = lightError;
 			startingLightDirection = i;
 			startingLightTilt = 170;
 		}
 		radial.write(i-1);
 		delay(35);
    }

    state = GO_TO_INITIAL_POSITION;
  }

  else if (state == GO_TO_INITIAL_POSITION)
  {
  	radial.write(startingLightDirection);
  	tilt.write(startingLightTilt);
  	currentTilt = startingLightTilt;
  	currentLightDirection = startingLightDirection;
  	delay(1000);
  	state = ADJUST_INITIAL_TILT;

  }

  else if (state == ADJUST_INITIAL_TILT)
  {
  	int topValue = analogRead(photoSensorTop);
  	int bottomValue = analogRead(photoSensorBottom);
  	int difference = abs(bottomValue - topValue);

  	if(difference >= maxTiltErrorThresh)
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
  		/*
  		if (currentTilt > 90)
  		{
  			moveDirectionState = GREATER_THAN_90;
  		}
  		else if (currentTilt < 90)
  		{
  			moveDirectionState = LESS_THAN_90;
  		}
  		*/
  		int leftValue = analogRead(photoSensorLeft);
		int rightValue = analogRead(photoSensorRight)*1.2;
		int topValue = analogRead(photoSensorTop);
		int bottomValue = analogRead(photoSensorBottom);
		int rotationError = leftValue - rightValue;
		int tiltError = topValue - bottomValue;
  		if(abs(rotationError) > 5)
		{
			if(rotationError > 0)
			{
				if(currentTilt > 90)
				{
					currentLightDirection++;
				}
				else{
					currentLightDirection--;
				}
			}
			else if (rotationError <0)
			{
				if (currentTilt > 90)
				{
					currentLightDirection--;
				}
				else
				{
					currentLightDirection++;
				}
				
			}
			if(currentLightDirection < 0)
			{
				currentLightDirection = 0;
			}
			if(currentLightDirection > 180)
			{
				currentLightDirection = 180;
			}
		}

		if(abs(tiltError) > 5)
		{
			if(tiltError > 0)
			{
				currentTilt++;
			}
			if (tiltError < 0)
			{
				currentTilt--;
			}
			
			if(currentTilt < 10)
			{
				currentTilt = 10;
			}
			if (currentTilt > 170)
			{
				currentTilt = 170;
			}
		}
		radial.write(currentLightDirection);
		tilt.write(currentTilt);
		delay(15);
  	}

  }
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

