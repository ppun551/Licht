/*/*
  Test of analog voltage connected to A0
*/

/*
  How to use this sketch
  
  1) Remove any connection cable, shield or jumper from your Arduino ZERO
  2) Connect pin A1 to the nearest GND pin using the shortest jumper possible
  3) Connect pin A2 to the 3.3V pin using the shortest jumper possible
  4) Connect the Arduino ZERO to your PC using a USB cable plugged in the USB programming port of the board
  5) Upload this sketch and leave the board powered on for at least one minute
  6) Open the Serial Monitor and press the reset button on the Arduino ZERO
  7) At the and of the procedure you can find logged
       - the offset and gain values for the board where the sketch has been just executed
       - the instruction line to copy/paste in the final sketch
*/

#include <Adafruit_NeoPixel.h>

// Which pin on the Arduino is connected to the NeoPixels?
#define LED_PIN    6

// How many NeoPixels are attached to the Arduino?
#define LED_COUNT 60

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)

// select ADC pin for moisture sensor
int ADC_In_Pin = 0;
// max voltage seen out of MAKEVMA303
int maxReading = 680;
// delay for dail pixel
int dialSpeed = 5;
// measurement delay
int measurementDelay = 50;

// setup() function -- runs once at startup --------------------------------

void setup() {
  Serial.begin(9600);
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)
}

void loop()
{
  int reading = analogRead(ADC_In_Pin);
  int showVal = reading*strip.numPixels()/maxReading;
//  Serial.println(showVal);

  // Fill along the length of the strip in colors...

  colorDial(strip.Color(  0, 255,   0), showVal, dialSpeed); // Green
  delay(measurementDelay);
}

// Fill strip pixels one after another with a color. Strip is cleared
// first; anything there will be covered pixel by pixel. Pass in color
// (as a single 'packed' 32-bit value, which you can get by calling
// strip.Color(red, green, blue) as shown in the loop() function above),
// and a delay time (in milliseconds) between pixels.
void colorDial(uint32_t color, int val, int wait) {
    // strip.clear(); // reset strip
    // strip.show();
    for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
      if (i<val) {
        strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
      } else {
        strip.setPixelColor(i, 0);
      }
      // delay(wait);                           //  Pause for a moment
  }
  strip.show();                          //  Update strip to match
}
