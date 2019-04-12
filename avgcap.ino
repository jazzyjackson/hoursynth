#include "Volume3.h"
#define speakerPin 9

#include <CapacitiveSensor.h>
CapacitiveSensor   cs_4_2 = CapacitiveSensor(4,2);        // 10M resistor between pins 4 & 2, pin 2 is sensor pin, add a wire and or foil if desired


const int n = 20;
const int timeBetweenSamples = 1;
int readIndex = 0;
int runningPresent[n];
int runningPast[n];
float presentSum = 0;
float pastSum = 0;
float threshold = 1.25;
int runningHigh = 0;
int runningLow = 0;

void setup() {
  Serial.begin(9600);
  pinMode(A0, OUTPUT);
  pinMode(A4, OUTPUT);
  digitalWrite(A0, HIGH);
  digitalWrite(A4, LOW);
  
  for(int i = 0; i < n; i++){
    int sample = cs_4_2.capacitiveSensor(30) + 1;
    pastSum += sample;
    runningPast[i] = sample;
    delay(timeBetweenSamples);
  }
  
  for(int i = 0; i < n; i++){
    int sample = cs_4_2.capacitiveSensor(30) + 1;
    presentSum += sample;
    runningPresent[i] = sample;
    delay(timeBetweenSamples);
  }
}

void loop() {
  // subtract last index from totals, we're about to update it.
  int newSample = cs_4_2.capacitiveSensor(30) + 1;
  int oldSample = runningPresent[readIndex];
  
  pastSum -= runningPast[readIndex];
  runningPast[readIndex] = oldSample;
  pastSum += oldSample;
  
  presentSum -= oldSample;
  presentSum += newSample;
  runningPresent[readIndex] = newSample;

  float pastAvg = pastSum / n;
  float presentAvg = presentSum / n;

  if(pastSum > presentSum and pastSum / presentSum > threshold){
    runningLow = presentSum / n;
  }
  
  if(presentSum > pastSum and presentSum / pastSum > threshold){
    runningHigh = presentSum / n;
  }

  int output = map(newSample, runningLow, runningHigh, 0, 1000);
  output = constrain(output, 0, 1000);
  
  Serial.println(output);
  
  vol.tone(speakerPin, analogRead(A2), output);
  
  delay(timeBetweenSamples);
  
  readIndex++;
  if(readIndex >= n){
    readIndex = 0;
  }
}
