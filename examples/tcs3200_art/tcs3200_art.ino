#include <Arduino.h>
#include "ART.h"

// TCS230 or TCS3200 pins wiring to Arduino
#define S0 4
#define S1 5
#define S2 6
#define S3 7
#define sensorOut 8

// Stores frequency read by the photodiodes
int redFrequency = 0;
int greenFrequency = 0;
int blueFrequency = 0;

// Define the input size and prototype size
const int inputSize = 3;
const int prototypeSize = 6;

// Define the vigilance parameter (between 0 and 1)
const float vigilance = 0.27;

// Create an instance of the ART network
ART art(inputSize, prototypeSize, vigilance);

void setup() {
  // Setting the outputs
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);

  // Setting the sensorOut as an input
  pinMode(sensorOut, INPUT);

  // Setting frequency scaling to 2%
  digitalWrite(S0, LOW);
  digitalWrite(S1, HIGH);

  // Begins serial communication
  Serial.begin(9600);

  // Initialize the ART network with some sample data
  float sampleDefault[inputSize] = {71.0, 82.0, 69.0};
  float sampleBlack[inputSize] = {60.0, 62.0, 52.0};
  float sampleWhite[inputSize] = {8.0, 8.0, 7.0};
  float sampleRed[inputSize] = {18.0, 45.0, 35.0};
  float sampleGreen[inputSize] = {38.0, 21.0, 27.0};
  float sampleBlue[inputSize] = {34.0, 28.0, 13.0};

  art.initialize(sampleDefault);
  art.initialize(sampleBlack);
  art.initialize(sampleWhite);
  art.initialize(sampleRed);
  art.initialize(sampleGreen);
  art.initialize(sampleBlue);

}
void loop() {
  // Setting RED (R) filtered photodiodes to be read
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);

  // Reading the output frequency
  redFrequency = pulseIn(sensorOut, LOW);

  // Printing the RED (R) value
  Serial.print("R = ");
  Serial.print(redFrequency);
  delay(100);

  // Setting GREEN (G) filtered photodiodes to be read
  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);

  // Reading the output frequency
  greenFrequency = pulseIn(sensorOut, LOW);

  // Printing the GREEN (G) value
  Serial.print(" G = ");
  Serial.print(greenFrequency);
  delay(100);

  // Setting BLUE (B) filtered photodiodes to be read
  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);

  // Reading the output frequency
  blueFrequency = pulseIn(sensorOut, LOW);

  // Printing the BLUE (B) value
  Serial.print(" B = ");
  Serial.println(blueFrequency);
  delay(100);

  // Input data to classify
  float input[inputSize] = {(float)redFrequency, (float)greenFrequency, (float)blueFrequency};
  // Classify the input data
  int category = art.classify(input);

  //Serial.print("Input belongs to category: ");
  //Serial.println(category);

  switch (category) {
    case 0:
      Serial.println("black");
      break;
    case 1:
      Serial.println("white");
      break;
    case 2:
      Serial.println("red");
      break;
    case 3:
      Serial.println("green");
      break;
    case 4:
      Serial.println("blue");
      break;
    default:
      Serial.println("unknown");
  }
  delay(1000);
}
