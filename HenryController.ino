/* RGB Controller for Henry
 *  Author: Bernhard Stoeffler
 *  
 *  The outputs are 2x 12V RGB stripes, 3x ARGB fans and 1x ARGB infinity mirror.
*/

#include <FastLED.h>
//#include <math.h>


// Teensy 2.0 has the LED on pin 11

// The RGB strips are ordered +BRG

#define NUM_IM_LEDS 68
#define IM_DATA 18
#define NUM_FAN_LEDS 15
#define FAN_DATA 20

#define R1LED 9
#define G1LED 4
#define B1LED 10
#define R2LED 14
#define G2LED 15
#define B2LED 12

//============

// global ARGB arrays, Sine Lookup Tables and other variables
CRGB imLeds[NUM_IM_LEDS];
CRGB fanLeds[NUM_FAN_LEDS];
CRGB table;
CRGB lego;
//double imSLT[2*NUM_IM_LEDS+1] = {0};
//double fanSLT[2*NUM_FAN_LEDS+1] = {0};
//int imLT[2*NUM_IM_LEDS+1];
int r1, g1, b1, r2, g2, b2;

long progcnt = 0;

void setup() 
{
  // init serial
  Serial.begin(115200);
  Serial.println("No Data is awaited. This is just a constant pattern.");
  
  // configure GPIO pins    
  pinMode(R1LED, OUTPUT);
  pinMode(G1LED, OUTPUT);
  pinMode(B1LED, OUTPUT);
  pinMode(R2LED, OUTPUT);
  pinMode(G2LED, OUTPUT);
  pinMode(B2LED, OUTPUT);
  FastLED.addLeds<WS2812B, IM_DATA, GRB>(imLeds, NUM_IM_LEDS);
  FastLED.addLeds<WS2812B, FAN_DATA, GRB>(fanLeds, NUM_FAN_LEDS);

#if 0    
  //initialize Sine Lookup tables
  for(int i=0;i<(2*NUM_IM_LEDS+1); i++){
    imSLT[i] = sin16_avr(3.1415*(i/5))/32767.0; //sin is behaving weirdly. doesn't work
    Serial.printf("SLT[%d] = %lf\n", i, imSLT[i]);

    imLT[i] = (int) 256.0 * ((float)i/(float)NUM_IM_LEDS);
  }
#endif

  //initialize mirror to solid cyan
  for(int i = 0; i < NUM_IM_LEDS; i++){
    imLeds[i].setRGB(0, 125, 255);
  }

  //initialize fans to solid cyan
  for(int i = 0; i < NUM_FAN_LEDS; i++){
    fanLeds[i].setRGB(0, 125, 255);
  }

  //initialize table and lego to solid cyan
  table.setRGB(0, 125, 255);
  lego.setRGB(0, 125, 255);

  //update LEDs
  FastLED.show();
  analogWrite(R1LED, table.r);
  analogWrite(G1LED, table.g);
  analogWrite(B1LED, table.b);
  analogWrite(R2LED, lego.r);
  analogWrite(G2LED, lego.g);
  analogWrite(B2LED, lego.b);

}
void loop() 
{
  //variables that continually increase but at slower speeds;
  int imSpd = progcnt/68;
  int fanSpd = progcnt/15;

  //set the mirror to a rainbow that syncs at the ends
  for(int i=0; i<NUM_IM_LEDS; i++){
    imLeds[i].setHue((int) ((i+imSpd)%255)*255.0/(float)NUM_IM_LEDS);
  }
  Serial.printf("Current Progcnt: %d, RGB: %d, %d, %d\n", progcnt, imLeds[0].r, imLeds[0].g, imLeds[0].b);

  //set the fans to a rainbow that syncs at the ends
  for(int i=NUM_FAN_LEDS; i>0; i++){
    fanLeds[i].setHue((int) ((i+fanSpd)%255)*255.0/(float)NUM_FAN_LEDS);
  }

  //set the table and lego to the color of the top right mirror led
  table.setHue(progcnt%255/10);
  lego.setHue(progcnt%255/10);
  
#if 0  
  //reset mirror to cyan
  for(int i = 0; i < NUM_IM_LEDS; i++){
    imLeds[i].setRGB(0, 125, 255);
  }
  //setting a single LED to white (cycling around)
  imLeds[progcnt%NUM_IM_LEDS] = CRGB::White;
#endif

  //---------------------------------------------------------------------------------------------------
  
  //update LEDs and advance program
  FastLED.show();
  analogWrite(R1LED, table.r);
  analogWrite(G1LED, table.g);
  analogWrite(B1LED, table.b);
  analogWrite(R2LED, lego.r);
  analogWrite(G2LED, lego.g);
  analogWrite(B2LED, lego.b);
  progcnt++;
  delay(100);
}
