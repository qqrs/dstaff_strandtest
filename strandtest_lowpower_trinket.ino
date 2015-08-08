#include <Adafruit_NeoPixel.h>

#define NEOPIXEL_PIN 0
#define FULL_ON 191

#define BUT_PIN 2

unsigned long dbtime;       // debounce time

#define adelay(ms)  if (digitalRead(BUT_PIN) == LOW && millis() - dbtime > 300) { dbtime = millis(); return; } else { delay(ms); }

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

void setup() {
  pinMode(BUT_PIN, INPUT_PULLUP);
  pinMode(1, OUTPUT);
  digitalWrite(1, LOW);           // GND for button pin
  dbtime = millis();

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  //colorWipe(strip.Color(FULL_ON, 0, 0), 50); // Red
  //colorWipe(strip.Color(0, FULL_ON, 0), 50); // Green
  //colorWipe(strip.Color(0, 0, FULL_ON), 50); // Blue

  wipeUp(strip.Color(0, FULL_ON, 0), 40); // Green
  wipeUp(strip.Color(0, 0, FULL_ON), 20); // Blue
  wipeUp(strip.Color(FULL_ON, 0, 0), 20); // Red

  theaterChase(strip.Color(127, 127, 127), 50); // White
  theaterChase(strip.Color(127,   0,   0), 50); // Red
  theaterChase(strip.Color(  0,   0, 127), 50); // Blue

  rainbow(20);
  rainbowCycle(20);
  theaterChaseRainbow(50);
}

// Fill the dots one after the other with a color
/*
void colorWipe(uint32_t c, uint8_t wait) {
 while (1) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      adelay(wait);
  }
 }
}
*/

// Fill the dots one after the other with a color
void wipeUp(uint32_t c, uint8_t wait) {

 while (1) {
  strip.clear();
  strip.show();

  uint16_t j = strip.numPixels() - 1;
  for(uint16_t i = 0; i<= j; i++, j--) {
      strip.setPixelColor(i, c);
      strip.setPixelColor(j, c);
      strip.show();
      adelay(wait);
  }
 }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

 while (1) {
  for(j=0; j<128; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 127));
    }
    strip.show();
    adelay(wait);
  }
 }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

 while (1) {
  for(j=0; j<128*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 128 / strip.numPixels()) + j) & 127));
    }
    strip.show();
    adelay(wait);
  }
 }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
 while (1) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();
     
      adelay(wait);
     
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
 }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
 while (1) {
  for (int j=0; j < 128; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
        for (int i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, Wheel( (i+j) % 127));    //turn every third pixel on
        }
        strip.show();
       
        adelay(wait);
       
        for (int i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, 0);        //turn every third pixel off
        }
    }
  }
 }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 127 - WheelPos;
  if(WheelPos < 42) {
   return strip.Color(127 - WheelPos * 3, 0, WheelPos * 3);
  } else if(WheelPos < 85) {
    WheelPos -= 42;
   return strip.Color(0, WheelPos * 3, 127 - WheelPos * 3);
  } else {
   WheelPos -= 85;
   return strip.Color(WheelPos * 3, 127 - WheelPos * 3, 0);
  }
}

