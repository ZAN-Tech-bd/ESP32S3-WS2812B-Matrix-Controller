#include <FastLED.h>

// --- Hardware Settings ---
#define LED_PIN     5        // The ESP32-S3 GPIO pin connected to 74HCT125 Pin 2
#define NUM_LEDS    64       // 8x8 matrix = 64 LEDs
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB

// --- Brightness Control ---
// Range is 0 (off) to 255 (max brightness). 
// Keep this low (e.g., 50) if powering via USB for testing!
#define BRIGHTNESS  50       

CRGB leds[NUM_LEDS];

void setup() {
  delay(1000); // Safety startup delay

  // Initialize FastLED
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS)
         .setCorrection(TypicalLEDStrip);
         
  // Master brightness control
  FastLED.setBrightness(BRIGHTNESS);
  
  // Clear the matrix on startup
  FastLED.clear();
  FastLED.show();
}

void loop() {
  // Example: Fill the matrix with solid Blue
  fill_solid(leds, NUM_LEDS, CRGB::Blue);
  FastLED.show();
  delay(1000);

  // Example: Change brightness on the fly
  FastLED.setBrightness(100); 
  fill_solid(leds, NUM_LEDS, CRGB::Red);
  FastLED.show();
  delay(1000);

  // Example: Custom color using RGB values (Red, Green, Blue)
  // 0-255 for each channel
  FastLED.setBrightness(BRIGHTNESS); // Reset to original brightness
  for(int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB(255, 100, 0); // Orange
  }
  FastLED.show();
  delay(1000);
}