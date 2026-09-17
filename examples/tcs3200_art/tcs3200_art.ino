#include <Arduino.h>
#include "ART.h"

// TCS3200 pins wiring to Arduino
#define S0 4
#define S1 5
#define S2 6
#define S3 7
#define sensorOut 8

// stores frequency from the photodiodes
int redFrequency = 0;
int greenFrequency = 0;
int blueFrequency = 0;

// Define the input size and prototype size
const int inputSize = 3;
const int prototypeSize = 6;

// Define the vigilance parameter (between 0 and 1 though keep it lower for stability)
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

  // Frequency scaling set to 2% (~10-12 kHz)
  digitalWrite(S0, LOW);
  digitalWrite(S1, HIGH);

  // Start the serial communication 9600 baud
  Serial.begin(9600);

  // Initialize the ART network with sample data
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
  // Setting RED filter
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);

  // Reading the output 
  redFrequency = pulseIn(sensorOut, LOW);

  // printing RED value
  Serial.print("R = ");
  Serial.print(redFrequency);
  delay(100);

  // Setting GREEN filter
  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);

  // Reading the output
  greenFrequency = pulseIn(sensorOut, LOW);

  // printing GREEN value
  Serial.print(" G = ");
  Serial.print(greenFrequency);
  delay(100);

  // Setting BLUE filter
  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);

  // Reading the output 
  blueFrequency = pulseIn(sensorOut, LOW);

  // Printing BLUE value
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
