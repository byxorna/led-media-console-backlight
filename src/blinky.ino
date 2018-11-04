/*
* Project d18-scarf
* Description: LEDs for DI8ORIENT scarf
* Author: Gabe Conradi
* Date: idklol
*/

// Use qsuba for smooth pixel colouring and qsubd for non-smooth pixel colouring
#define qsubd(x, b)  ((x>b)?b:0)
#define qsuba(x, b)  ((x>b)?x-b:0)
#define NUM_LEDS 200
#define LEDS_PIN D6
#define LED_TYPE NSFastLED::NEOPIXEL
#define UPDATES_PER_SECOND 120
#define MAX_BRIGHTNESS 255
#define MAX_SATURATION 255
#define BOOTUP_ANIM_DURATION_MS 4000
#define PATTERN_CHANGE_INTERVAL_MS 30000
#define PALETTE_CHANGE_INTERVAL_MS 30000
#define EFFECT_CHANGE_INTERVAL_MS 20000
#define VJ_CROSSFADING_ENABLED 1
#define VJ_CROSSFADE_DURATION_MS 6000
#define VJ_NUM_DECKS 2
// switch between deck a and b with this interval
#define VJ_DECK_SWITCH_INTERVAL_MS 15000
#define AUTO_CHANGE_PALETTE 1
#define SETUP_BUTTON_HOLD_DURATION_MS 800

#ifndef _PARTICLE_H_
#include "Particle.h"
#endif
#ifndef _FASTLED_H_
#include "FastLED.h"
#endif
#ifndef _STRUCTS_H_
#include "structs.h"
#endif
#ifndef _PALETTES_H_
#include "palettes.h"
#endif
#ifndef _EFFECTS_H_
#include "effects.h"
#endif

SYSTEM_MODE(SEMI_AUTOMATIC);

Output MasterOutput;
Deck DeckA;
Deck DeckB;
Deck* DeckAll[] = {&DeckA, &DeckB};
Mixer mainMixer;

typedef void (*DrawFunction)(Deck*);
// how 2 decks mix together into an output
typedef void (*MixerFunction)(Deck*, Deck*, Output*);

uint8_t BRIGHTNESS_VALUES[] = {255, 180, 110, 80, 65, 40};
uint8_t BRIGHTNESS_INDEX = 3;
#define GLOBAL_BRIGHTNESS BRIGHTNESS_VALUES[BRIGHTNESS_INDEX]
#define BRIGHTNESS_COUNT sizeof(BRIGHTNESS_VALUES)/sizeof(uint8_t)
bool AUTO_PATTERN_CHANGE = true;

unsigned long t_now;                // time now in each loop iteration
unsigned long t_boot;               // time at bootup

/* state for controlling user-mode button for pattern changes */
uint8_t button_state = 0;
unsigned long button_timer = 0;

#define FILL_NOISE false
#ifdef FILL_NOISE
static uint16_t x = NSFastLED::random16(); // x is index into pixel strip
//static uint16_t y = NSFastLED::random16(); // y is the time variable
static uint16_t z = NSFastLED::random16();
// We're using the x/y dimensions to map to the x/y pixels on the matrix.  We'll
// use the z-axis for "time".  speed determines how fast time moves forward.  Try
// 1 for a very slow moving effect, or 60 for something that ends up looking like
// water.
uint16_t noisespeed = 5; // speed is set dynamically once we've started up
// Scale determines how far apart the pixels in our noise matrix are.  Try
// changing these values around to see how it affects the motion of the display.  The
// higher the value of scale, the more "zoomed out" the noise iwll be.  A value
// of 1 will be so zoomed in, you'll mostly see solid colors.
uint16_t noisescale = 7; // scale is set dynamically once we've started up
// This is the array that we keep our computed noise values in (only 1d)
uint8_t noise[NUM_LEDS];

// Fill the x/y array of 8-bit noise values using the inoise8 function.
void fillnoise8() {
  // If we're runing at a low "speed", some 8-bit artifacts become visible
  // from frame-to-frame.  In order to reduce this, we can do some fast data-smoothing.
  // The amount of data smoothing we're doing depends on "speed".
  uint8_t dataSmoothing = 0;
  if( noisespeed < 50) {
    dataSmoothing = 200 - (noisespeed * 4);
  }

  for(int i = 0; i < NUM_LEDS; i++) {
    int ioffset = noisescale * i;

    uint8_t data = NSFastLED::inoise8(x + ioffset, z);

    // The range of the inoise8 function is roughly 16-238.
    // These two operations expand those values out to roughly 0..255
    // You can comment them out if you want the raw noise data.
    data = NSFastLED::qsub8(data, 16);
    data = NSFastLED::qadd8(data, NSFastLED::scale8(data,39));

    if( dataSmoothing ) {
      uint8_t olddata = noise[i];
      uint8_t newdata = NSFastLED::scale8( olddata, dataSmoothing) + NSFastLED::scale8( data, 256 - dataSmoothing);
      data = newdata;
    }

    noise[i] = data;
  }

  z += noisespeed;

  // apply slow drift to X and Y, just for visual variation.
  x += noisespeed / 8;
  //y -= noisespeed / 16;
}
#endif


NSFastLED::CFastLED* gLED; // global CFastLED object

NSFastLED::TBlendType currentBlending = NSFastLED::LINEARBLEND;
NSFastLED::CRGB leds[NUM_LEDS];
NSFastLED::CRGB ledsA[NUM_LEDS];
NSFastLED::CRGB ledsB[NUM_LEDS];

/*
TODO WTF FUCK WHY DOES HAVING `struct Deck;` in here make this compile, and without
everything is broken with 
src/scarf.cpp:31:39: error: variable or field 'pattern_slow_pulse_with_sparkles' declared void
 void pattern_slow_pulse_with_sparkles(Deck* s);
                                       ^
src/scarf.cpp:31:39: error: 'Deck' was not declared in this scope
src/scarf.cpp:31:45: error: 's' was not declared in this scope
 void pattern_slow_pulse_with_sparkles(Deck* s);
                                             ^
src/scarf.cpp:32:34: error: variable or field 'pattern_phase_shift_palette' declared void
 void pattern_phase_shift_palette(Deck* s);
                                  ^
src/scarf.cpp:32:34: error: 'Deck' was not declared in this scope
src/scarf.cpp:32:40: error: 's' was not declared in this scope
*/
struct Deck;
struct Mixer;

void pattern_white_test(Deck* s) {
  for( int i = 0; i < NUM_LEDS; i++) {
    s->leds[i] = NSFastLED::CRGB::White;
  }
}

void pattern_slow_pulse_with_sparkles(Deck* s) {
  // pick a color, and pulse it 
  uint8_t cBrightness = NSFastLED::beatsin8(20, 140, 255);
  uint8_t cHue = NSFastLED::beatsin8(4, 0, 255);
  NSFastLED::CHSV hsv_led = NSFastLED::CHSV(cHue, 255, cBrightness);
  NSFastLED::CRGB rgb_led;
  hsv2rgb_rainbow(hsv_led, rgb_led);
  for( int i = 0; i < NUM_LEDS; i++) {
    //if (random(NUM_LEDS*3) == 0) {
    //  s->leds[i] = NSFastLED::CRGB::White;
    //} else {
      s->leds[i] = rgb_led;
    //}
  }
}

/*vars for pattern_phase_shift_palette*/
int wave1=0;
void pattern_phase_shift_palette(Deck* s) {
  // phase shift
  wave1 += 8;
  int phase2 = NSFastLED::beatsin88(7*256,-64,64);

  for (int k=0; k<NUM_LEDS; k++) {
    int phase1 = NSFastLED::sin8(3*k + wave1/128);
    int colorIndex = NSFastLED::cubicwave8((k)+phase1)/2 + NSFastLED::cos8((k*3)+phase2)/2;

    //int bri8 = NSFastLED::cubicwave8(t_now/10.0 + k*10.0); // nice pulsy one direction intensity modulator
    // generate undulating intensity phases
    int bri8 = NSFastLED::cubicwave8(t_now/10.0 + NSFastLED::cubicwave8(k*10.0));

    //Serial.printlnf("%d %d", k, bri8);
    s->leds[k] = NSFastLED::ColorFromPalette(s->currentPalette, colorIndex, bri8, currentBlending);
  }
}

void  pattern_plasma(Deck* s) {

  int thisPhase = NSFastLED::beatsin88(256*2,-64,64);
  int thatPhase = NSFastLED::beatsin88(237,-64,64);

  for (int k=0; k<NUM_LEDS; k++) {

    int colorIndex = NSFastLED::cubicwave8((k*17)+thisPhase)/2 + NSFastLED::cos8((k*15)+thatPhase)/2;
    int thisBright = NSFastLED::cubicwave8(t_now/10.0 + k*10.0); // nice pulsy one direction intensity modulator
    //int thisBright = qsuba(colorIndex, NSFastLED::beatsin8(7,0,96));

    s->leds[k] = ColorFromPalette(s->currentPalette, colorIndex, thisBright, currentBlending);
  }
}

// cycle a rainbow, varying how quickly it rolls around the board
void pattern_rainbow_waves_with_sparkles(Deck* s) {
  for(int i = 0; i < NUM_LEDS; ++i) {
    //if (random(NUM_LEDS*3) == 0) {
    //  s->leds[i] = NSFastLED::CRGB::White;
    //} else {
      uint8_t h = (t_now/12+i)%256;
      NSFastLED::CHSV hsv_led = NSFastLED::CHSV(h, 255, 255);
      NSFastLED::CRGB rgb_led;
      hsv2rgb_rainbow(hsv_led, rgb_led);
      s->leds[i] = rgb_led;
    //}
  }
}

void pattern_clear(Deck* s) {
  for( int i = 0; i < NUM_LEDS; i++) {
    s->leds[i] = NSFastLED::CRGB::Black;
  }
}

void pattern_disorient_palette_sparkles(Deck* s) {
  uint8_t b = NSFastLED::beatsin8(4, 0, 255);
  for( int i = 0; i < NUM_LEDS; i++) {
    //if (random(NUM_LEDS*4) == 0) {
    //  s->leds[i] = NSFastLED::CRGB::White;
    //} else {
      s->leds[i] = ColorFromPalette((NSFastLED::CRGBPalette16)Disorient_gp, s->animationIndex + i + b, MAX_BRIGHTNESS, currentBlending);
    //}
  }
  // slow down progression by 1/3
  if (t_now%3 == 0) {
    s->animationIndex = NSFastLED::addmod8(s->animationIndex, 1, 255);
  }
}

void pattern_from_palette(Deck* s) {
  uint8_t b = NSFastLED::beatsin8(4, 0, 255);
  for( int i = 0; i < NUM_LEDS; i++) {
    s->leds[i] = NSFastLED::ColorFromPalette(s->currentPalette, s->animationIndex + i + b, MAX_BRIGHTNESS, currentBlending);
  }
  // slow down progression by 1/3
  if (t_now%3 == 0) {
    s->animationIndex = NSFastLED::addmod8(s->animationIndex, 1, 255);
  }
}

// NOTE: lifted and tweaked from https://learn.adafruit.com/rainbow-chakra-led-hoodie/the-code
// This function draws color waves with an ever-changing,
// widely-varying set of parameters, using a color palette.
void pattern_palette_waves(Deck* s) {
  uint8_t numleds = NUM_LEDS;
  static uint16_t sPseudotime = 0;
  static uint16_t sLastMillis = 0;
  static uint16_t sHue16 = 0;

  //uint8_t sat8 = NSFastLED::beatsin88( 87, 220, 250);
  uint8_t brightdepth = NSFastLED::beatsin88( 341, 96, 224);
  uint16_t brightnessthetainc16 = NSFastLED::beatsin88( 203, (25 * 256), (40 * 256));
  uint8_t msmultiplier = NSFastLED::beatsin88(147, 23, 60);

  uint16_t hue16 = sHue16;//gHue * 256;
  uint16_t hueinc16 = NSFastLED::beatsin88(113, 300, 1500);

  uint16_t ms = millis();
  uint16_t deltams = ms - sLastMillis ;
  sLastMillis  = ms;
  sPseudotime += deltams * msmultiplier;
  sHue16 += deltams * NSFastLED::beatsin88( 400, 5,9);
  uint16_t brightnesstheta16 = sPseudotime;

  for( uint16_t i = 0 ; i < numleds; i++) {
    hue16 += hueinc16;
    uint8_t hue8 = hue16 / 256;
    uint16_t h16_128 = hue16 >> 7;
    if( h16_128 & 0x100) {
      hue8 = 255 - (h16_128 >> 1);
    } else {
      hue8 = h16_128 >> 1;
    }

    brightnesstheta16  += brightnessthetainc16;
    uint16_t b16 = NSFastLED::sin16( brightnesstheta16  ) + 32768;

    uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
    uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
    bri8 += (255 - brightdepth);

    uint8_t index = hue8;
    index = NSFastLED::scale8( index, 240);

    NSFastLED::CRGB newcolor = NSFastLED::ColorFromPalette(s->currentPalette, index, bri8);

    uint16_t pixelnumber = i;
    pixelnumber = (numleds-1) - pixelnumber;

    nblend(s->leds[pixelnumber], newcolor, 128);
  }
}

/** update this with patterns you want to be cycled through **/
#define NUM_PATTERNS sizeof(patternBank) / sizeof(DrawFunction)
const DrawFunction patternBank[] = {
  &pattern_palette_waves,
  &pattern_slow_pulse_with_sparkles,
  &pattern_phase_shift_palette,
  &pattern_plasma,
  &pattern_plasma,
  &pattern_from_palette,
  &pattern_disorient_palette_sparkles,
  &pattern_rainbow_waves_with_sparkles,
};

#define NUM_EFFECTS sizeof(effectBank) / sizeof(EffectFunction)
const EffectFunction effectBank[] = {
  NULL,
  NULL,
  NULL,
  NULL,
  //&effect_sparkles,
  //NULL,
  //&effect_random_decay,
  //NULL,
  //&effect_wave_decay,
  //NULL,
};

// change dw/p1/p2 on some period
void stepFxParams(Mixer* m) {
  m->fxDryWet = NSFastLED::beatsin8(12, 0, 255);
  m->fxParam1 = NSFastLED::beatsin8(12, 0, 255, 0, m->fxDryWet );
  m->fxParam2 = NSFastLED::beatsin8(19, 0, 255, 0, 0);
  //Serial.printlnf("dw=%d p1=%d p2=%d", m->fxDryWet, m->fxParam1, m->fxParam2);
}

void randomEffect(Mixer* m) {
  uint8_t old = m->fxEffectIndex;
  while (m->fxEffectIndex == old) {
    // pick a new effect that isnt the old effect index
    m->fxEffectIndex = NSFastLED::random8(0, NUM_EFFECTS);
  }
  m->tFxEffectStart = t_now;
}

void randomPattern(Deck* deck, Deck* otherDeck) {
  uint8_t old = deck->pattern;
  while (deck->pattern == old || deck->pattern == otherDeck->pattern) {
    deck->pattern = NSFastLED::random8(0, NUM_PATTERNS);
  }
  deck->tPatternStart = t_now;
}

void randomPalette(Deck* deck, Deck* otherDeck) {
  uint8_t old = deck->palette;
  while (deck->palette == old || deck->palette == otherDeck->palette) {
    deck->palette = NSFastLED::random8(0, PALETTES_COUNT);
  }
  deck->currentPalette = palettes[deck->palette];
  deck->tPaletteStart = t_now;
}

void mixer_crossfade_blend(Mixer* mixer, Deck* a, Deck* b, Output* out) {
  for (int i = 0; i < NUM_LEDS; ++i) {
    if (VJ_CROSSFADING_ENABLED) {
      // NSFastLED::fract8(255*crossfadePosition));
      out->leds[i] = a->leds[i].lerp8(b->leds[i], NSFastLED::fract8(255*mixer->crossfadePosition));
    } else {
      out->leds[i] = a->leds[i];
    }
    if (t_now < + BOOTUP_ANIM_DURATION_MS) {
      // ramp intensity up slowly, so we fade in when turning on
      int8_t bri8 = (uint8_t)((t_now*1.0)/BOOTUP_ANIM_DURATION_MS*255.0);
      out->leds[i] = out->leds[i].fadeToBlackBy(255-bri8);
    }
  }
}


// setup() runs once, when the device is first turned on.
void setup() {
  t_now = millis();
  t_boot = t_now;
  Serial.begin(9600);
  Serial.println("resetting");

  // disable the built in LED
  RGB.control(true);
  RGB.brightness(0);

  MasterOutput = {
    leds,
  };

  DeckA = {
    1,
    0.0,
    0,
    0,
    0,
    palettes[0],
    t_now,
    t_now,
    ledsA,
  };

  DeckB = {
    2,
    1.0,
    0,
    0,
    0,
    palettes[0],
    t_now,
    t_now,
    ledsB,
  };

  mainMixer = {
    1.0,  // position; 0.0 is deckA, 1.0 is deckB
    -1, // start fading direction going B -> A
    0, // crossfade in progress
    0, // last crossfade
    0,  // fx effect index in effectBank
    0,  // fx d/w
    0,  // fx p1
    0,  // fx p2
    0,  // time fx effect started
    &DeckA,
    &DeckB,
    &MasterOutput,
  };

  randomPattern(&DeckA, &DeckB);
  randomPalette(&DeckA, &DeckB);
  randomPattern(&DeckB, &DeckA);
  randomPalette(&DeckB, &DeckA);
  randomEffect(&mainMixer);

  // led controller, data pin, clock pin, RGB type (RGB is already defined in particle)
  gLED = new NSFastLED::CFastLED();
  gLED->addLeds<LED_TYPE, LEDS_PIN>(MasterOutput.leds, NUM_LEDS);
  gLED->setBrightness(GLOBAL_BRIGHTNESS);
  pattern_clear(&DeckA);
  pattern_clear(&DeckB);
  gLED->show();

  Serial.println("booted up");
}

void loop() {
  t_now = millis();

#ifdef FILL_NOISE
  if (FILL_NOISE) {
    // fill noise array
    fillnoise8();
  }
#endif

  // handle user interaction with reset button
  if (HAL_Core_Mode_Button_Pressed(SETUP_BUTTON_HOLD_DURATION_MS)) {
    switch (button_state) {
    case 0:
      // we werent pressed before, so start timer!
      button_state = 1;
      button_timer = t_now;
      break;
    case 1:
      if (t_now - button_timer > SETUP_BUTTON_HOLD_DURATION_MS) {
        // we have been held longer than
        button_state = 2;
      }
      break;
    // we are waiting to take action now
    case 2: break;
    // action already taken, do nothing until release!
    case 3: break;
    default:
        button_state = 0;
      break;
    }
  } else {
    button_state = 0;
  }
  // toggle patterns
  /*
  if (button_state == 2) {
    // disable auto pattern changing now!
    DeckA.pattern++;
    DeckB.pattern++;
    button_state = 3;
    if (AUTO_PATTERN_CHANGE) {
      AUTO_PATTERN_CHANGE = false;
      Serial.printlnf("moving to fixed pattern mode (use button to change)");
    }
    Serial.printlnf("deckA.pattern=%d", DeckA.pattern);
    Serial.printlnf("deckB.pattern=%d", DeckB.pattern);
  }
  */
  if (button_state == 2) {
    // disable auto pattern changing now!
    BRIGHTNESS_INDEX++;
    button_state = 3;
    if (BRIGHTNESS_INDEX >= BRIGHTNESS_COUNT){
      BRIGHTNESS_INDEX = 0;
    }
    Serial.printlnf("set brightness to %d/255", GLOBAL_BRIGHTNESS);
  }



  // increment pattern every PATTERN_CHANGE_INTERVAL_MS, but not when a deck is active!
  if (AUTO_PATTERN_CHANGE) {
    if (t_now > DeckA.tPatternStart+PATTERN_CHANGE_INTERVAL_MS && !mainMixer.crossfadeInProgress) {
      if (mainMixer.crossfadePosition == 1.0) {
        randomPattern(&DeckA, &DeckB);
        Serial.printlnf("deckA.pattern=%d", DeckA.pattern);
      }
    }
    if (t_now > DeckB.tPatternStart+PATTERN_CHANGE_INTERVAL_MS && !mainMixer.crossfadeInProgress) {
      if (mainMixer.crossfadePosition == 0.0) {
        randomPattern(&DeckB, &DeckA);
        Serial.printlnf("deckB.pattern=%d", DeckB.pattern);
      }
    }
  }

  // increment palette every PALETTE_CHANGE_INTERVAL_MS, but not when crossfading!
  if (AUTO_CHANGE_PALETTE && !mainMixer.crossfadeInProgress) {
    for (int x = 0; x < VJ_NUM_DECKS ; x++){
      int xOther = (x == 0) ? 1 : 0;
      Deck* deck = DeckAll[x];
      Deck* otherdeck = DeckAll[xOther];
      if ((deck->crossfadePositionActive != mainMixer.crossfadePosition) &&
          (deck->tPaletteStart + PALETTE_CHANGE_INTERVAL_MS < t_now)) {
        randomPalette(deck, otherdeck);
        Serial.printlnf("deck%d.palette=%d", deck->label, deck->palette);
      }
    }
  }

  if (!mainMixer.crossfadeInProgress) {
    if (mainMixer.fxDryWet == 0 && mainMixer.tFxEffectStart + EFFECT_CHANGE_INTERVAL_MS < t_now) {
      randomEffect(&mainMixer);
      Serial.printlnf("mixer.effect=%d (%p)", mainMixer.fxEffectIndex, effectBank[mainMixer.fxEffectIndex]);
    }
  }
  stepFxParams(&mainMixer);


  // fill in patterns on both decks! we will crossfade master output later
  // NOTE: only render to a deck if its "visible" through the crossfader
  if ( !VJ_CROSSFADING_ENABLED || mainMixer.crossfadePosition < 1.0 ) {
    patternBank[DeckA.pattern](&DeckA);
  }
  if ( VJ_CROSSFADING_ENABLED && mainMixer.crossfadePosition > 0 ) {
    patternBank[DeckB.pattern](&DeckB);
  }

  // perform crossfading increment if we are mid pattern change
  if (VJ_CROSSFADING_ENABLED) {
    if (t_now > mainMixer.tLastCrossfade + VJ_DECK_SWITCH_INTERVAL_MS && !mainMixer.crossfadeInProgress) {
      // start switching between decks
      Serial.printf("starting fading to %c\n", (mainMixer.crossfadePosition == 1.0) ? 'A' : 'B');
      mainMixer.crossfadeInProgress = 1;
      mainMixer.tLastCrossfade = t_now;
    }
    if (mainMixer.crossfadeInProgress) {
      float step = (float)1.0/(VJ_CROSSFADE_DURATION_MS/1000*UPDATES_PER_SECOND);
      mainMixer.crossfadePosition = mainMixer.crossfadePosition + mainMixer.crossfadeDirection * step;

      // is it time to change decks?
      // we are cut over to deck B, break this loop
      if (mainMixer.crossfadePosition > 1.0) {
        mainMixer.crossfadePosition = 1.0;
        mainMixer.crossfadeDirection = -1; // 1->0
        mainMixer.crossfadeInProgress = 0;
        Serial.printf("finished fading to B\n");
      }
      // we are cut over to deck B
      if (mainMixer.crossfadePosition < 0.0) {
        mainMixer.crossfadePosition = 0.0;
        mainMixer.crossfadeDirection = 1;  // 0->1
        mainMixer.crossfadeInProgress = 0;
        Serial.printf("finished fading to A\n");
      }
    }
  }

  // perform crossfading
  mixer_crossfade_blend(&mainMixer, &DeckA, &DeckB, &MasterOutput);

  if (effectBank[mainMixer.fxEffectIndex] != NULL) {
    effectBank[mainMixer.fxEffectIndex](&MasterOutput, mainMixer.fxDryWet, mainMixer.fxParam1, mainMixer.fxParam2);
  }

  gLED->setBrightness(GLOBAL_BRIGHTNESS);
  gLED->show();
  delay(1000.0 / UPDATES_PER_SECOND);
}
