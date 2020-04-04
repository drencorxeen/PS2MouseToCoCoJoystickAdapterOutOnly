// Built using Arduino IDE version 1.8.2
// Include the needed PS2Mouse library.
// Can be acquired from:
// https://github.com/rucek/arduino-ps2-mouse
//
// You will also need to include the DAC_MCP49xx
// library to get this program working.
// You can download the library from:
// https://github.com/exscape/electronics/tree/master/Arduino/Libraries/DAC_MCP49xx
//
// You will also need to include the Joystick
// library if you want to use a Arduino Micro (or clone)
// to give you passthrough to PC joystick support.
// You can download the library from:
// https://github.com/MHeironimus/ArduinoJoystickLibrary
#ifdef __AVR___
#include <avr/wdt.h>
#endif
#include <SPI.h>         // Remember this line!
#include "DAC_MCP49xx.h"
#include "PS2Mouse.h"

#define INVERT_JOY_BUT TRUE

#if defined(INVERT_JOY_BUT)
#define BUTTON LOW
#define BUTTOFF HIGH
#else
#define BUTTON HIGH
#define BUTTOFF LOW
#endif

// The Arduino pin used for the slave select / chip select
#define SS_PIN 10

// The Arduino pin used for the LDAC (output synchronization) feature
#define LDAC_PIN 9

// Set up the DAC. 
// First argument: DAC model (MCP4902, MCP4912, MCP4922)
// Second argument: SS pin (10 is preferred)
// (The third argument, the LDAC pin, can be left out if not used)
DAC_MCP49xx dac(DAC_MCP49xx::MCP4922, SS_PIN, LDAC_PIN);

// Define the PS2 Mouse Data & Clock PINs
// Data Line on the PS/2 6 PIN Mini DIN is PIN 1
// Clock Line on the PS/2 6 PIN Mini DIN is PIN 5
// VCC Line on PS/2 6 PIN Mini DIN is PIN 4
// GND Line on PS/2 6 PIN Mini DIN is PIN 3
#define MDATA_PIN 2
#define MCLOCK_PIN 4

// Set the Min,Max, and Center mouse DAC resolution
#define MMin 0
#define MMax 1023
#define MCent 511

// Set DAC Min and Max
#define DACMax 4095
#define DACMin 0

PS2Mouse mouse(MCLOCK_PIN, MDATA_PIN);

int PCBLED=1;
int But0=8;
int But1=7;
byte MButtons;
byte MMButton;
byte OldB0;
byte OldB1;
byte JoyPass1;

uint16_t CCPOSX=MCent;
uint16_t CCPOSY=MCent;
uint16_t COPOSX=0;
uint16_t COPOSY=0;
uint16_t MPPOSX;
uint16_t MPPOSY;
int16_t TCPOSX;
int16_t TCPOSY;
int16_t OLDX;
int16_t OLDY;

void setup(void) {
  // Here we enable the watchdog system and set it it to 1 second.
  // This should help keep the system resetting till the PS/2 Mouse
  // is ready to be setup.
  #ifdef __AVR___
  wdt_enable(WDTO_1S);
  #endif
  //pinMode(PCBLED, OUTPUT);
  JoyPass1=0;
  //digitalWrite(PCBLED, LOW);
  mouse.initialize();
  // Set the SPI frequency to 1 MHz (on 16 MHz Arduinos), to be safe.
  // DIV2 = 8 MHz works for me, though, even on a breadboard.
  // This is not strictly required, as there is a default setting.
  dac.setSPIDivider(SPI_CLOCK_DIV16);
  // Use "port writes", see the manual page. In short, if you use pin 10 for
  // SS (and pin 7 for LDAC, if used), this is much faster.
  // Also not strictly required (no setup() code is needed at all).
  dac.setPortWrite(false);
  // Pull the LDAC pin low automatically, to synchronize output
  // This is true by default, however.
  dac.setAutomaticallyLatchDual(true);
  //Serial.begin(115200);
  pinMode(But1, OUTPUT);
  pinMode(But0, OUTPUT);
  delay(20);
  digitalWrite(But0, BUTTOFF);
  digitalWrite(But1, BUTTOFF);
  //wdt_enable(WDTO_2S);
  dac.output2(map(CCPOSX,MMin,MMax,DACMin,DACMax), map(CCPOSY,MMax,MMin,DACMin,DACMax));
  //digitalWrite(PCBLED, HIGH);
}

void loop (void) {
  #ifdef __AVR___
  wdt_reset();
  #endif
  MouseData data = mouse.readData();
  MButtons = data.status & 3;
  MMButton = data.status & 4;

    switch(MButtons) {
      case 0:
        digitalWrite(But0, BUTTOFF);
        digitalWrite(But1, BUTTOFF);
        break;
      case 1:
        digitalWrite(But0, BUTTON);
        digitalWrite(But1, BUTTOFF);
        break;
      case 2:
        digitalWrite(But0, BUTTOFF);
        digitalWrite(But1, BUTTON);
        break;
      case 3:
        digitalWrite(But0, BUTTON);
        digitalWrite(But1, BUTTON);
        break;
    }
    if ( (data.position.x != 0)||(data.position.y != 0) ) {
      COPOSX = CCPOSX;
      TCPOSX = CCPOSX + data.position.x;
      CCPOSX = CCPOSX + data.position.x;
      if ( TCPOSX > MMax ) {
        CCPOSX = MMax;
      } else {
        if ( TCPOSX < MMin ) {
          CCPOSX = MMin;
        }
      }
      COPOSY = CCPOSY;
      TCPOSY = CCPOSY + data.position.y;
      CCPOSY = CCPOSY + data.position.y;
      if ( TCPOSY > MMax ) {
        CCPOSY = MMax;
      } else {
        if ( TCPOSY < MMin ) {
          CCPOSY = MMin;
        }
      }
      if ( (COPOSX != CCPOSX)||(COPOSY != CCPOSY) ) {
        dac.output2(map(CCPOSX,MMin,MMax,DACMin,DACMax), map(CCPOSY,MMax,MMin,DACMin,DACMax));
      }
    }
  // Here we set the delay before we go back to the
  // start of the loop. Minimum delay of 20 is required
  // but something higher can be used to liking.
  delay(31);
}
