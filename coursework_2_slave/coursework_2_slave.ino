//include necessary libraries
#include <LiquidCrystal.h>
#include "Ultrasonic.h"
#include <Wire.h>

//initialise the ultrasonic ranger, set it on pin 7
Ultrasonic ultrasonic(7);

//set pins for components
int temperaturePin = A0;
int ldrPin = A1;
int micPin = A3;

//create variables for component values
int ldrValue = 0;
int micValue = 0;
int temperature = 0;
int temperatureCelsius = 0;

//create variables for the data to be transmitted to the slave arduino
int micData = 0;
int temperatureData = 0;
int ldrData = 0;
int ultrasoundData = 0;


//set pinMode for components and begin transmission to slave arduino
void setup() {
  pinMode(temperaturePin, INPUT);
  pinMode(ldrPin, INPUT);
  pinMode(micPin, INPUT);
  //join i2c bus
  Wire.begin();
}

//read the components and transmit them to the slave arduino
void loop() {
  micData = readMic();
  temperatureData = readTemperature();
  ldrData = readLDR();
  ultrasoundData = readUltrasound();

  //transmit to device #9
  Wire.beginTransmission(9);
  
  Wire.write(micData);
  Wire.write(temperatureData);
  Wire.write(ldrData);
  Wire.write(ultrasoundData);
  
  //stop transmitting
  Wire.endTransmission();
  
  delay(500);
}

//function to read the value of the microphone
int readMic() {
  micValue = analogRead(micPin);
  //subtract 27 to make it easier to calculate changes
  micValue -= 27;
  return micValue;
}

//function to read the value of the temperature
int readTemperature() {
  temperature = analogRead(temperaturePin);
  //convert temperature to Farenheit
  temperature = temperature * 0.48828125;
  //convert temperature to Celsius
  temperatureCelsius = (temperature - 32) / 1.8;
  return temperatureCelsius;
}

//function to read the value of the LDR
int readLDR() {
  ldrValue = analogRead(ldrPin);
  return ldrValue;
}

//function to read the value of the ultrasound
int readUltrasound() {
  long RangeInCentimeters;
  RangeInCentimeters = ultrasonic.MeasureInCentimeters();
  //max range is 400, so if range is greater, set range to 400
  if (RangeInCentimeters > 400) {
    RangeInCentimeters = 400;
  }
  return RangeInCentimeters;
}
