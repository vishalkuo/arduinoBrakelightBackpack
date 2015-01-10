#include <Adafruit_NeoPixel.h>
// include the library code:
#include <LiquidCrystal.h>

//-------------------------------------------GPS---------------------------------------

#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial(3, 2);
#define BRAKETHRESHOLD        0.7
#define BRAKETIMETHRESHOLD    0.3

Adafruit_GPS GPS(&mySerial);

#define GPSECHO  false

boolean usingInterrupt = false;
void useInterrupt(boolean); 
const int rPin = 4;
const int lPin = 5;
int rState = 0;
int lState = 0;
long brakeTime = 0;



//------------------------------------------------------------------------------------------



// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

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
float i = 0;
float v = 256;
int start = 0;

int prevX = 0;
int currentX = 0;
void setup(){
  pinMode(rPin, INPUT); 
  pinMode(lPin, INPUT);
  strip.begin();
  strip.show();
  Serial.begin(115200);
  
   // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  // Print a message to the LCD.
  
  //-------------------------------------------------------GPS--------------------------------------
  Serial.begin(115200);
  Serial.println("Adafruit GPS library basic test!");

  // 9600 NMEA is the default baud rate for Adafruit MTK GPS's- some use 4800
  GPS.begin(9600);
  
  // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  // uncomment this line to turn on only the "minimum recommended" data
  //GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
  // For parsing data, we don't suggest using anything but either RMC only or RMC+GGA since
  // the parser doesn't care about other sentences at this time
  
  // Set the update rate
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);   // 1 Hz update rate
  // For the parsing code to work nicely and have time to sort thru the data, and
  // print it out we don't suggest using anything higher than 1 Hz

  // Request updates on antenna status, comment out to keep quiet
  GPS.sendCommand(PGCMD_ANTENNA);

  // the nice thing about this code is you can have a timer0 interrupt go off
  // every 1 millisecond, and read data from the GPS for you. that makes the
  // loop code a heck of a lot easier!
  useInterrupt(true);

  delay(1000);
  // Ask for firmware version
  mySerial.println(PMTK_Q_RELEASE);
  //--------------------------------------------------------------------------------------------------------------------------
}
//-------------------------------------------------------GPS------------------------------------------------------------------
// Interrupt is called once a millisecond, looks for any new GPS data, and stores it
SIGNAL(TIMER0_COMPA_vect) {
  char c = GPS.read();
  // if you want to debug, this is a good time to do it!
#ifdef UDR0
  if (GPSECHO)
    if (c) UDR0 = c;  
    // writing direct to UDR0 is much much faster than Serial.print 
    // but only one character can be written at a time. 
#endif
}

void useInterrupt(boolean v) {
  if (v) {
    // Timer0 is already used for millis() - we'll just interrupt somewhere
    // in the middle and call the "Compare A" function above
    OCR0A = 0xAF;
    TIMSK0 |= _BV(OCIE0A);
    usingInterrupt = true;
  } else {
    // do not call the interrupt function COMPA anymore
    TIMSK0 &= ~_BV(OCIE0A);
    usingInterrupt = false;
  }
}

uint32_t timer = millis();
//----------------------------------------------------------------------------------------------------------------
void loop(){ 
  rState=digitalRead(rPin);
  lState=digitalRead(lPin);
  if (rState==HIGH){
    strip.setBrightness(255);
    lcd.print("right blink on");
    lcd.setCursor(0,0);
    
    for (i == 0; i <=256; (i=i+1.5)){
      v=v-1.5;
      strip.setPixelColor(0,i,0,0);
      strip.setPixelColor(3,i,0,0);
      strip.setPixelColor(5,v,0,0);
      strip.setPixelColor(6,v,0,0);
      //strip.setPixelColor(4,0,0,i);
      //strip.setPixelColor(5,v,0,0);
      //strip.setPixelColor(6,0,0,i);
      //strip.setPixelColor(7,v,0,0);
      strip.show();
    }
      
    for (i == 256; i >= 0; (i=i-1.5)){
      v=v+1.5;
      strip.setPixelColor(0,i,0,0);
      strip.setPixelColor(3,i,0,0);
      strip.setPixelColor(5,v,0,0);
      strip.setPixelColor(6,v,0,0);
      //strip.setPixelColor(4,0,0,i);
      //strip.setPixelColor(5,v,0,0);
      //strip.setPixelColor(6,0,0,i);
      //strip.setPixelColor(7,v,0,0);
      strip.show();
    }
  }
  if (lState==HIGH){
    lcd.print("left blink on");
    lcd.setCursor(0,0);
    strip.setBrightness(255);

    
    for (i == 0; i <=256; (i=i+1.5)){
      v=v-1.5;
      strip.setPixelColor(1,i,0,0);
      strip.setPixelColor(2,i,0,0);
      strip.setPixelColor(4,v,0,0);
      strip.setPixelColor(7,v,0,0);
      //strip.setPixelColor(4,0,0,i);
      //strip.setPixelColor(5,v,0,0);
      //strip.setPixelColor(6,0,0,i);
      //strip.setPixelColor(7,v,0,0);
      strip.show();
    }
      
    for (i == 256; i >= 0; (i=i-1.5)){
      v=v+1.5;
      strip.setPixelColor(1,i,0,0);
      strip.setPixelColor(2,i,0,0);
      strip.setPixelColor(4,v,0,0);
      strip.setPixelColor(7,v,0,0);
      //strip.setPixelColor(4,0,0,i);
      //strip.setPixelColor(5,v,0,0);
      //strip.setPixelColor(6,0,0,i);
      //strip.setPixelColor(7,v,0,0);
      strip.show();
    }
  }
  if (rState==LOW && lState==LOW){
    strip.setBrightness(0);
    lcd.print("                                     ");
    lcd.setCursor(0,0);
    strip.show();
  }
  if (rState==HIGH && lState==HIGH){
    strip.setBrightness(0);
    strip.show();
  }
      // set the cursor to column 0, line 1
      // (note: line 1 is the second row, since counting begins with 0):
      
  
    //-------------------------------------------------------------------GPS-----------------------------------------------------------
    if (! usingInterrupt) {
    // read data from the GPS in the 'main loop'
    char c = GPS.read();
    // if you want to debug, this is a good time to do it!
    if (GPSECHO)
      if (c) Serial.print(c);
  }
  
  // if a sentence is received, we can check the checksum, parse it...
  if (GPS.newNMEAreceived()) {
    // a tricky thing here is if we print the NMEA sentence, or data
    // we end up not listening and catching other sentences! 
    // so be very wary if using OUTPUT_ALLDATA and trytng to print out data
    //Serial.println(GPS.lastNMEA());   // this also sets the newNMEAreceived() flag to false
  
    if (!GPS.parse(GPS.lastNMEA()))   // this also sets the newNMEAreceived() flag to false
      return;  // we can fail to parse a sentence in which case we should just wait for another
  }

  // if millis() or timer wraps around, we'll just reset it
  if (timer > millis())  timer = millis();

  // approximately every 2 seconds or so, print out the current stats
  if (millis() - timer > 200) { 
    timer = millis(); // reset the timer
    lcd.setCursor(0, 0);
    lcd.print("Speed: "); lcd.println(GPS.speed);
    lcd.setCursor(0, 1);
    //lcd.print("Angle: "); lcd.println(GPS.angle);
  
    Serial.print("\nTime: ");
    Serial.print(GPS.hour-4, DEC); Serial.print(':');
    Serial.print(GPS.minute, DEC); Serial.print(':');
    Serial.print(GPS.seconds, DEC); Serial.print('.');
    Serial.println(GPS.milliseconds);
    Serial.print("Date: ");
    Serial.print(GPS.day, DEC); Serial.print('/');
    Serial.print(GPS.month, DEC); Serial.print("/20");
    Serial.println(GPS.year, DEC);
    Serial.print("Fix: "); Serial.print((int)GPS.fix);
    Serial.print(" quality: "); Serial.println((int)GPS.fixquality); 
    if (GPS.fix) {
      Serial.print("Location: ");
      Serial.print(GPS.latitude, 4); Serial.print(GPS.lat);
      Serial.print(", "); 
      Serial.print(GPS.longitude, 4); Serial.println(GPS.lon);
      
      Serial.print("Speed: "); Serial.println(GPS.speed*1.85);
      Serial.print("Angle: "); Serial.println(GPS.angle);
      Serial.print("Altitude: "); Serial.println(GPS.altitude);
      Serial.print("Satellites: "); Serial.println((int)GPS.satellites);
    }
  }
  //-----------------------------------------------------------------------------------------------------------------
  currentX = abs(GPS.speed*1.85);
   
  if (start == 0){
    prevX = currentX;
    start = 1;
  }
  int i = currentX - prevX;
  if (abs(i) > BRAKETHRESHOLD) {
    brakeTime = millis();
    int strikes = 0;
    while ((abs(i) > BRAKETHRESHOLD) && (strikes < 3)) {
      if (abs(i) < BRAKETHRESHOLD) {
        strikes = strikes + 1;
        Serial.println(strikes);
      }
      currentX = abs(GPS.speed*1.85);
      i = currentX - prevX;
      
      if ((millis() - brakeTime) > BRAKETIMETHRESHOLD) {
        strip.setPixelColor(1,255,0,0);
        strip.setPixelColor(2,255,0,0);
        strip.setPixelColor(3,255,0,0);
        strip.setPixelColor(4,255,0,0);
        strip.setPixelColor(5,255,0,0);
        strip.setPixelColor(6,255,0,0);
        strip.setPixelColor(7,255,0,0);
        strip.setPixelColor(0,255,0,0);
        strip.show();
        if (abs(i) > BRAKETHRESHOLD) {
          currentX = abs(GPS.speed*1.85);
          i = currentX - prevX;
          Serial.println(i);
          delay(100);
        }
        strip.setBrightness(0);
        brakeTime = millis();
        i = 0;
                
          currentX = abs(GPS.speed*1.85);
      }
    }
  }

  prevX = currentX;
  delay(200);
  
}
