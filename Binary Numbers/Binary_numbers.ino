#include <math.h>
const int red = 5;
const int yellow = 6;
const int green = 9;
int button = 3;
int state = 0;
int leds = 3;
int count = 0;

void setup() {
  pinMode(red, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(button, INPUT);
}

void loop() {
  state = digitalRead(button);
  if (state == HIGH) {
    count = (count + 1) % (int)pow(2,leds); // (int)pow(2,leds)
    int b[3];
    b[0] = count & 1;
    b[1] = (count >> 1) & 1;
    b[2] = (count >> 2) & 1;
    digitalWrite(red, b[0]);
    digitalWrite(yellow, b[1]);
    digitalWrite(green, b[2]);
    delay(200);
  }
}
