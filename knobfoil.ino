#include "Volume3.h"
#include <CapacitiveSensor.h>

#define speakerPin 9
/*
 * CapitiveSense Library Demo Sketch
 * Paul Badger 2008
 * Uses a high value resistor e.g. 10M between send pin and receive pin
 * Resistor effects sensitivity, experiment with values, 50K - 50M. Larger resistor values yield larger sensor values.
 * Receive pin is the sensor pin - try different amounts of foil/metal on this pin
 */
int freq = 100;
int amp = 1023;


CapacitiveSensor   cs_4_2 = CapacitiveSensor(4,2);        // 10M resistor between pins 4 & 2, pin 2 is sensor pin, add a wire and or foil if desired

uint16_t frequency = 440;

void setup() {
   Serial.begin(9600);
   pinMode(A0, OUTPUT);
   pinMode(A4, OUTPUT);
   digitalWrite(A0, LOW);
   digitalWrite(A4, HIGH);
}

void loop() {  
  freq = cs_4_2.capacitiveSensor(30);
  amp = analogRead(A2);
  vol.tone(speakerPin, freq, amp);
  Serial.println(freq);
}
