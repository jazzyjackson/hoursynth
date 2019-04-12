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
  
  setupKnob();
  
  for(int i = 0; i < n; i++){
    buildAverage(runningPast, i, cs_4_2.capacitiveSensor(30) + 1);
  }
  for(int i = 0; i < n; i++){
    buildAverage(runningPresent, i, cs_4_2.capacitiveSensor(30) + 1);
  }
}

void loop() {
  // subtract last index from totals, we're about to update it.
  int sample = cs_4_2.capacitiveSensor(30) + 1
  
  updateAverage(sample); // sets the runningHighs and runningLows to determine the bounds for normalize...via global variables
  
  int amplitude = normalize(sample, 0, 1000)
  int frequency = map(analogRead(A2), 0, 1023, 10, 5000)) // <-- the last two integers are your output frequency range!
  
  vol.tone(speakerPin, frequency, amplitude);

  // iterate
  if(readIndex < n){
    readIndex++;
  } else {
    readIndex = 0;
  }
  
  delay(timeBetweenSamples);
}

void setupKnob(){
  pinMode(A0, OUTPUT);
  pinMode(A4, OUTPUT);
  digitalWrite(A0, HIGH);
  digitalWrite(A4, LOW);
}

void buildAverage(int index, int sample){
  for(int i = 0; i < n; i++){
    pastSum += sample;
    runningPast[index] = sample;
    delay(timeBetweenSamples);
  }
  
  for(int i = 0; i < n; i++){
    presentSum += sample;
    runningPresent[i] = sample;
    delay(timeBetweenSamples);
  }
}

void updateAverage(int newSample){
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
}

int normalize(int sample, int minimum, int maximum){
  int k = map(sample, runningLow, runningHigh, minumum, maximum)
  int j = constrain(k, minimum, maximum)
  return j;
}
