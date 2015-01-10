#include <Adafruit_NeoPixel.h>

#define PIN 6

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

int rState = 0; 
const int rPin = 4;


void setup() {
  strip.begin();
  strip.show();
  pinMode(rPin, INPUT);
  Serial.begin(9600);
}

void loop(){
  check_switches();
}

void check_switches(){
  rState = digitalRead(rPin);
  
  if (rState == HIGH) {
    Serial.println("Right Turn");
    strip.show();
    strip.setPixelColor(4,255,0,0);
    strip.show();
  }
  else if (rState == LOW){
    Serial.println("Right Off");
    strip.setPixelColor (4,0,0,0);
    strip.show();
  }
}
