/*
  drift von farben im Kreis um den Neopixel Ring
*/

/*
  Grundidee: 
  1. Ein subsegement der Palette von 30 Farbtönen verteilt sich auf jeweils 1 Ring-Hälfte und rotiert mit UPS
  2. Farben durchlaufen die Palette mit UPS/4
  

*/

#include <FastLED.h>

#define LED_PIN     6
#define NUM_LEDS    60
#define BRIGHTNESS  32
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

#define UPDATES_PER_SECOND 100

// setup() function -- runs once at startup --------------------------------

CRGBPalette16 currentPalette;
TBlendType    currentBlending;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;

DEFINE_GRADIENT_PALETTE( heatmap_gp ) {
  0,   255,  0,  0,   //red
128,   0,  255,  0,   //green
224,   0,0,  255,   //blue
255,   127,255,127 }; //composite

CRGBPalette16 myPal = heatmap_gp;

void setup() {
    Serial.begin(9600);
    delay( 3000 ); // power-up safety delay
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(  BRIGHTNESS );
    
    currentPalette = RainbowColors_p;
    currentBlending = LINEARBLEND;
}


void loop()
{
//    currentPalette = ForestColors_p;           
    currentBlending = LINEARBLEND;
    static uint16_t colIndex = 0;
    static uint8_t borderIndex = 0;
    static uint8_t oldCol = 0 ;
    static uint8_t newCol = 0 ;
    
    oldCol = colIndex / 8;
    colIndex++ ; /* color speed */
    newCol = colIndex / 8;
    borderIndex++ ;
    borderIndex = borderIndex  % NUM_LEDS;  /* rotation speed */

    Serial.println(colIndex);
    FillLEDsFromPaletteColors( borderIndex, oldCol, newCol ); 
//    fadeall();   

    FastLED.show();
    FastLED.delay(1000 / UPDATES_PER_SECOND);
}

void fadeall() { for(int i = 0; i < NUM_LEDS; i++) { leds[i].nscale8(250); } }

void FillLEDsFromPaletteColors( uint8_t border, uint8_t oldCol, uint8_t colorIndex)
{
    uint8_t brightness = 64;
    
//    for(CRGB & pixel : leds) { pixel = CHSV(hue++,255,255); }

/*    
    Going round the pixel ring   ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending); 
 */
    for( int i = 0; i < NUM_LEDS/2; i++) {
        if (i<border)
          leds[i] = ColorFromPalette( currentPalette, i+oldCol, brightness); // normal palette access
        else
          leds[i] = ColorFromPalette( currentPalette, i+colorIndex, brightness); // normal palette access
    }
    for( int i = NUM_LEDS; i >= NUM_LEDS/2; i--) {
        if (i<border)
          leds[i] = ColorFromPalette( currentPalette, NUM_LEDS-i+oldCol, brightness); // normal palette access
        else
          leds[i] = ColorFromPalette( currentPalette, NUM_LEDS-i+colorIndex, brightness); // normal palette access
    }
}
