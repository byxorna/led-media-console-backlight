/*
* Project: led-media-console-backlight
* Author: Gabe Conradi
* Date: idklol
*/

// flickers sometimes. effect changes?

// Use qsuba for smooth pixel colouring and qsubd for non-smooth pixel colouring
#define qsubd(x, b)  ((x>b)?b:0)
#define qsuba(x, b)  ((x>b)?x-b:0)
#define WIDTH 295
#define HEIGHT 1

#define FEATURE_ENABLE_EFFECTS true
#define FEATURE_AUTO_CHANGE_PALETTE true
#define FEATURE_ENABLE_PARTICLE_CLOUD false

#define NUM_LEDS ((WIDTH)*(HEIGHT))
#define LEDS_PIN D6
#define LED_TYPE NSFastLED::NEOPIXEL
#define BOOTUP_ANIM_DURATION_MS 4000
#define PATTERN_CHANGE_INTERVAL_MS 30000
#define PALETTE_CHANGE_INTERVAL_MS 15000
#define EFFECT_CHANGE_INTERVAL_MS 10000
#define VJ_CROSSFADING_ENABLED true
#define VJ_CROSSFADE_DURATION_MS 3000
#define VJ_NUM_DECKS 2
// switch between deck a and b with this interval
#define VJ_DECK_SWITCH_INTERVAL_MS 7000
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

SYSTEM_MODE(AUTOMATIC);

Deck MasterOutput;
Deck DeckA;
Deck DeckB;
Deck* DeckAll[] = {&DeckA, &DeckB};
Mixer mainMixer;

typedef void (*DrawFunction)(Deck*);
// how 2 decks mix together into an output
typedef void (*MixerFunction)(Deck*, Deck*, Deck*);

uint8_t BRIGHTNESS_VALUES[] = {0, 80, 120, 255};
#define BRIGHTNESS_COUNT (sizeof(BRIGHTNESS_VALUES)/sizeof(uint8_t)-1)
#define GLOBAL_BRIGHTNESS BRIGHTNESS_VALUES[BRIGHTNESS_INDEX]
uint8_t BRIGHTNESS_INDEX = 1;
bool AUTO_PATTERN_CHANGE = true;

unsigned long t_now;                // time now in each loop iteration
unsigned long t_boot;               // time at bootup

/* state for controlling user-mode button for pattern changes */
uint8_t button_state = 0;
unsigned long button_timer = 0;

NSFastLED::CFastLED* gLED; // global CFastLED object

NSFastLED::TBlendType currentBlending = NSFastLED::LINEARBLEND;
NSFastLED::CRGB leds[NUM_LEDS];
NSFastLED::CRGB ledsA[NUM_LEDS];
NSFastLED::CRGB ledsB[NUM_LEDS];

struct Deck;
struct Mixer;

void pattern_slow_pulse(Deck* s) {
  // pick a color, and pulse it 
  uint8_t cBrightness = NSFastLED::beatsin8(10, 140, 255);
  uint8_t cHue = NSFastLED::beatsin8(4, 0, 255);
  NSFastLED::CHSV hsv_led = NSFastLED::CHSV(cHue, 255, cBrightness);
  NSFastLED::CRGB rgb_led;
  hsv2rgb_rainbow(hsv_led, rgb_led);
  for( int i = 0; i < NUM_LEDS; i++) {
    s->leds[i] = rgb_led;
  }
}

void  pattern_plasma(Deck* s) {

  int thisPhase = NSFastLED::beatsin88(256*2,-64,64);
  int thatPhase = NSFastLED::beatsin88(237,-64,64);

  for (int k=0; k<NUM_LEDS; k++) {

    int colorIndex = NSFastLED::cubicwave8((k*17)+thisPhase)/2 + NSFastLED::cos8((k*15)+thatPhase)/2;
    int thisBright = NSFastLED::cubicwave8(t_now/10.0 + k*10.0); // nice pulsy one direction intensity modulator
    //int thisBright = qsuba(colorIndex, NSFastLED::beatsin8(7,0,96));

    s->leds[k] = NSFastLED::ColorFromPalette(s->currentPalette, colorIndex, thisBright, currentBlending);
  }
}

// cycle a rainbow, varying how quickly it rolls around the board
void pattern_rainbow_waves(Deck* s) {
  for(int i = 0; i < NUM_LEDS; ++i) {
    uint8_t h = (t_now/12+i)%256;
    NSFastLED::CHSV hsv_led = NSFastLED::CHSV(h, 255, 255);
    NSFastLED::CRGB rgb_led;
    hsv2rgb_rainbow(hsv_led, rgb_led);
    s->leds[i] = rgb_led;
  }
}

void pattern_clear(Deck* s) {
  for( int i = 0; i < NUM_LEDS; i++) {
    s->leds[i] = NSFastLED::CRGB::Black;
  }
}

NSFastLED::CRGB ColorFromPaletteExtended(const NSFastLED::CRGBPalette16& pal, uint16_t index, uint8_t brightness) {
  // Extract the four most significant bits of the index as a palette index.
  uint8_t index_4bit = (index >> 12);
  // Calculate the 8-bit offset from the palette index.
  uint8_t offset = (uint8_t)(index >> 4);
  // Get the palette entry from the 4-bit index
  const NSFastLED::CRGB* entry = &(pal[0]) + index_4bit;
  uint8_t red1   = entry->red;
  uint8_t green1 = entry->green;
  uint8_t blue1  = entry->blue;

  uint8_t blend = offset && (currentBlending != NSFastLED::NOBLEND);
  if (blend) {
    if (index_4bit == 15) {
      entry = &(pal[0]);
    } else {
      entry++;
    }

    // Calculate the scaling factor and scaled values for the lower palette value.
    uint8_t f1 = 255 - offset;
    red1   = NSFastLED::scale8_LEAVING_R1_DIRTY(red1,   f1);
    green1 = NSFastLED::scale8_LEAVING_R1_DIRTY(green1, f1);
    blue1  = NSFastLED::scale8_LEAVING_R1_DIRTY(blue1,  f1);

    // Calculate the scaled values for the neighbouring palette value.
    uint8_t red2   = entry->red;
    uint8_t green2 = entry->green;
    uint8_t blue2  = entry->blue;
    red2   = NSFastLED::scale8_LEAVING_R1_DIRTY(red2,   offset);
    green2 = NSFastLED::scale8_LEAVING_R1_DIRTY(green2, offset);
    blue2  = NSFastLED::scale8_LEAVING_R1_DIRTY(blue2,  offset);
    NSFastLED::cleanup_R1();

    // These sums can't overflow, so no qadd8 needed.
    red1   += red2;
    green1 += green2;
    blue1  += blue2;
  }
  if (brightness != 255) {
    // nscale8x3_video(red1, green1, blue1, brightness);
    NSFastLED::nscale8x3(red1, green1, blue1, brightness);
  }
  return NSFastLED::CRGB(red1, green1, blue1);
}



uint16_t _XY(uint8_t x, uint8_t y) {
  if (x >= WIDTH) return NUM_LEDS;
  if (y >= HEIGHT) return NUM_LEDS;
  return y * WIDTH + x;
}

// note: found in https://wokwi.com/arduino/projects/285170662915441160
// from: https://github.com/FastLED/FastLED/pull/102
void pattern_smooth_palette_walk(Deck* s) {
  uint32_t ms = millis();
  uint32_t timing_scale = 32; // multiple to slow down the undulations across axes

  // just like the XYmatrix example but with added distortion
  uint32_t yHueDelta = (int32_t)NSFastLED::sin16(ms * 11) * 1;
  uint32_t xHueDelta = (int32_t)NSFastLED::cos16(ms * 11) * 1;
  uint32_t startHue = ms << 8;
  uint32_t lineStartHue = startHue - (HEIGHT + 1) / 2 * yHueDelta;
  uint32_t yd2 = (int32_t)NSFastLED::sin16(ms * 3) / (8*timing_scale);
  uint32_t xd2 = (int32_t)NSFastLED::sin16(ms * 7) / (8*timing_scale);
  for (byte y = 0; y < HEIGHT; y++) {
    uint32_t pixelHue = lineStartHue - (WIDTH + 1) / 2 * xHueDelta;
    uint32_t xhd = xHueDelta;
    for (byte x = 0; x < WIDTH/2; x++) {
      s->leds[_XY(x, y)] = ColorFromPaletteExtended(s->currentPalette, pixelHue >> 7, 255);
      s->leds[_XY((WIDTH-1)-x, y)] = NSFastLED::ColorFromPalette(s->currentPalette, pixelHue >> 15, 255);
      pixelHue += xhd;
      xhd += xd2;
    }
    lineStartHue += yHueDelta;
    yHueDelta += yd2;
  }
}


// NOTE: lifted and tweaked from https://learn.adafruit.com/rainbow-chakra-led-hoodie/the-code
// This function draws color waves with an ever-changing,
// widely-varying set of parameters, using a color palette.
void pattern_palette_waves(Deck* s) {
  uint16_t numleds = NUM_LEDS;
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
  // include multiple copies if you want to bias the random selection
  &pattern_rainbow_waves,
  &pattern_plasma,
  &pattern_plasma,
  &pattern_smooth_palette_walk,
  &pattern_smooth_palette_walk,
  &pattern_smooth_palette_walk,
  &pattern_palette_waves,
  &pattern_palette_waves,

  // vetted as visually sound
  //&pattern_rainbow_waves,
  //&pattern_plasma,
  //&pattern_smooth_palette_walk,
  //&pattern_palette_waves,
};

#define NUM_EFFECTS sizeof(effectBank) / sizeof(EffectFunction)
const EffectFunction effectBank[] = {
  NULL,
  NULL,
  NULL,
#ifdef FEATURE_ENABLE_EFFECTS
  //&effect_reverse,
  //&effect_mirror,
  //&effect_reverse_mirror,
#endif
};

// change dw/p1/p2 on some period
/*
void stepFxParams(Mixer* m) {
  m->fxDryWet = NSFastLED::beatsin8(12, 0, 255);
  m->fxParam1 = NSFastLED::beatsin8(12, 0, 255, 0, m->fxDryWet );
  m->fxParam2 = NSFastLED::beatsin8(19, 0, 255, 0, 0);
}
*/

void randomEffect(Deck* d) {
  uint8_t old = d->fxEffectIndex;
  while (d->fxEffectIndex == old) {
    // pick a new effect that isnt the old effect index
    d->fxEffectIndex = NSFastLED::random8(0, NUM_EFFECTS);
  }
  d->tFxEffectStart = t_now;
}

/*
void randomEffect(Mixer* m) {
  uint8_t old = m->fxEffectIndex;
  while (m->fxEffectIndex == old) {
    // pick a new effect that isnt the old effect index
    m->fxEffectIndex = NSFastLED::random8(0, NUM_EFFECTS);
  }
  m->tFxEffectStart = t_now;
}
*/

void randomPattern(Deck* deck, Deck* otherDeck) {
  uint8_t old = deck->pattern;
  while (deck->pattern == old || deck->pattern == otherDeck->pattern) {
    deck->pattern = NSFastLED::random8(0, NUM_PATTERNS);
  }
  deck->tPatternStart = t_now;
}

void usePattern(Deck* deck, uint8_t patternIndex){
  if (patternIndex >= NUM_PATTERNS) {
    patternIndex = 0;
  }
  deck->pattern = patternIndex;
  deck->tPatternStart = t_now;
}

void randomPalette(Deck* deck) {
  uint8_t old = deck->palette;
  while (deck->palette == old) {
    deck->palette = NSFastLED::random8(0, PALETTES_COUNT);
  }
  deck->currentPalette = palettes[deck->palette];
  deck->tPaletteStart = t_now;
}

void usePalette(Deck* deck, uint8_t paletteIndex){
  if (paletteIndex >= PALETTES_COUNT) {
    paletteIndex = 0;
  }
  deck->palette = paletteIndex;
  deck->currentPalette = palettes[deck->palette];
  deck->tPaletteStart = t_now;
}

void mixer_crossfade_blend(Mixer* mixer, Deck* a, Deck* b, Deck* out) {
  for (int i = 0; i < NUM_LEDS; ++i) {
    if (VJ_CROSSFADING_ENABLED) {
      // NSFastLED::fract8(255*crossfadePosition));
      if (mixer->activeDeck == a) {
        out->leds[i] = a->leds[i].lerp8(b->leds[i], NSFastLED::fract8(255*mixer->crossfadePosition));
      } else {
        out->leds[i] = b->leds[i].lerp8(a->leds[i], NSFastLED::fract8(255*mixer->crossfadePosition));
      }
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

// handle particle event for "brightness"
void changeBrightness(const char *event, const char *data) {
  if (strcmp(data, "+") == 0) {
    if (BRIGHTNESS_INDEX < (BRIGHTNESS_COUNT-1)) {
      BRIGHTNESS_INDEX++;
    }
  } else if (strcmp(data, "-") == 0) {
    if (BRIGHTNESS_INDEX > 0) {
      BRIGHTNESS_INDEX--;
    }
  } else if (strcmp(data, "max") == 0) {
    BRIGHTNESS_INDEX = BRIGHTNESS_COUNT-1;
  } else if (strcmp(data, "min") == 0) {
    BRIGHTNESS_INDEX = 1;
  } else if (strcmp(data, "off") == 0) {
    BRIGHTNESS_INDEX = 0;
  } else if (strcmp(data, "on") == 0) {
    BRIGHTNESS_INDEX = 1;
  }
  Serial.printlnf("set brightness to %d/255", GLOBAL_BRIGHTNESS);
}

void changePower(const char *event, const char *data) {
  if (strcmp(data, "on") == 0) {
    changeBrightness("brightness", "off");
  }

}

// handle particle event for "mode"
void changeMode(const char *event, const char *data) {
  if (strcmp(data, "red") == 0 || strcmp(data, "movie") == 0){
    AUTO_PATTERN_CHANGE = false;
    usePalette(&DeckA, 0); // red_gp
    usePalette(&DeckB, 0); // red_gp
    usePattern(&DeckA, 2); // phase shift palette
    usePattern(&DeckB, 2); // phase shift palette
    changeBrightness("brightness", "min");
  } else {
    // go back to auto mode, rave time!
    AUTO_PATTERN_CHANGE = true;
  }
}




// setup() runs once, when the device is first turned on.
void setup() {
  t_now = millis();
  t_boot = t_now;
  randomSeed(analogRead(0));
  Serial.begin(9600);
  Serial.println("resetting");

  MasterOutput = {
    1,
    0,
    0,
    0,
    palettes[0],
    t_now,
    t_now,
    leds,
    0,  // fx effect index in effectBank
    t_now, // time fx was changed
  };

  DeckA = {
    1,
    0,
    0,
    0,
    palettes[0],
    t_now,
    t_now,
    ledsA,
    0,  // fx effect index in effectBank
    t_now, // time fx was changed
  };

  DeckB = {
    2,
    0,
    0,
    0,
    palettes[0],
    t_now,
    t_now,
    ledsB,
    0,  // fx effect index in effectBank
    t_now, // time fx was changed
  };

  mainMixer = {
    0.0,  // crossfader. 0.0 is deckA, 1.0 is deckB
    &DeckA, // active deck (focused)
    false, // crossfade in progress
    0, // time of last crossfade start
    0,  // fx effect index in effectBank
    0,  // fx d/w
    0,  // fx p1
    0,  // fx p2
    0,  // time fx effect started
    &DeckA,
    &DeckB,
    &MasterOutput,
  };

#ifdef FEATURE_ENABLE_PARTICLE_CLOUD
  Particle.subscribe("brightness", changeBrightness, MY_DEVICES);
  Particle.subscribe("mode", changeMode, MY_DEVICES);
  Particle.subscribe("power", changePower, MY_DEVICES);
#endif

  randomPattern(&DeckA, &DeckB);
  randomPalette(&DeckA);
  randomPattern(&DeckB, &DeckA);
  randomPalette(&DeckB);
  randomEffect(&DeckA);
  randomEffect(&DeckB);

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
    changeBrightness("brightness", "+");
    button_state = 3;
  }



  // increment pattern every PATTERN_CHANGE_INTERVAL_MS, but not when a deck is active!
  if (AUTO_PATTERN_CHANGE && !mainMixer.crossfadeInProgress) {
    if (t_now > DeckA.tPatternStart+PATTERN_CHANGE_INTERVAL_MS ) {
      if (mainMixer.activeDeck != &DeckA) {
        randomPattern(&DeckA, &DeckB);
        Serial.printlnf("deckA.pattern=%d", DeckA.pattern);
      }
    }
    if (t_now > DeckB.tPatternStart+PATTERN_CHANGE_INTERVAL_MS ) {
      if (mainMixer.activeDeck != &DeckB) {
        randomPattern(&DeckB, &DeckA);
        Serial.printlnf("deckB.pattern=%d", DeckB.pattern);
      }
    }
  }

  // increment palette every PALETTE_CHANGE_INTERVAL_MS, but not when crossfading!
  if (FEATURE_AUTO_CHANGE_PALETTE && !mainMixer.crossfadeInProgress) {
    // allow palette change if a deck is fully occluded by the crossfader from main output
    if ((mainMixer.activeDeck != &DeckA) && t_now >= (DeckA.tPaletteStart + PALETTE_CHANGE_INTERVAL_MS)) {
      randomPalette(&DeckA);
      Serial.printlnf("deckA.palette=%d", DeckA.palette);
    }
    if ((mainMixer.activeDeck != &DeckB) && t_now >= (DeckB.tPaletteStart + PALETTE_CHANGE_INTERVAL_MS)) {
      randomPalette(&DeckB);
      Serial.printlnf("deckB.palette=%d", DeckB.palette);
    }
    
  }

  // render the patterns for both decks
  patternBank[DeckA.pattern](&DeckA);
  patternBank[DeckB.pattern](&DeckB);

  // step crossfader if necessary
  // perform crossfading increment if we are mid pattern change
  if (VJ_CROSSFADING_ENABLED) {
    if (t_now > mainMixer.tLastCrossfade + VJ_DECK_SWITCH_INTERVAL_MS && !mainMixer.crossfadeInProgress) {
      // start switching between decks
      Deck* oldTarget = mainMixer.activeDeck;
      mainMixer.crossfadeInProgress = true;
      mainMixer.tLastCrossfade = t_now;
      mainMixer.crossfadePosition = 1.0;
      if (mainMixer.activeDeck == &DeckA) {
        mainMixer.activeDeck = &DeckB;
      } else {
        mainMixer.activeDeck = &DeckA;
      }
      Serial.printf("started fading to %c\n", (oldTarget == &DeckB) ? 'A' : 'B');
    }
    if (mainMixer.crossfadeInProgress) {
      float elapsed_pct = (t_now-mainMixer.tLastCrossfade)/(1.0*VJ_CROSSFADE_DURATION_MS);
      mainMixer.crossfadePosition = 1.0 - elapsed_pct;
      //Serial.printf("%% %.02f %.02f\n", elapsed_pct, mainMixer.crossfadePosition);

      // is it time to change decks?
      // we are cut over to deck B, break this loop
      if (mainMixer.crossfadePosition <= 0.0) {
        mainMixer.crossfadePosition = 0.0;
        mainMixer.crossfadeInProgress = false;
        Serial.printf("finished crossfading\n");
      }
    }
  }

  // perform prefader effects
  if (effectBank[DeckA.fxEffectIndex] != NULL) {
    effectBank[DeckA.fxEffectIndex](&DeckA, mainMixer.fxDryWet, mainMixer.fxParam1, mainMixer.fxParam2);
  }
  if (effectBank[DeckB.fxEffectIndex] != NULL) {
    effectBank[DeckB.fxEffectIndex](&DeckA, mainMixer.fxDryWet, mainMixer.fxParam1, mainMixer.fxParam2);
  }

  // perform crossfading
  mixer_crossfade_blend(&mainMixer, &DeckA, &DeckB, &MasterOutput);

  if (effectBank[mainMixer.fxEffectIndex] != NULL) {
    effectBank[mainMixer.fxEffectIndex](&MasterOutput, mainMixer.fxDryWet, mainMixer.fxParam1, mainMixer.fxParam2);
  }


  gLED->setBrightness(GLOBAL_BRIGHTNESS);
  gLED->show();


  // after showing this frame, do any cleanup and racheting necessary
  {
    // pick new effects if we should
    if (!mainMixer.crossfadeInProgress) {
      if (DeckA.tFxEffectStart + EFFECT_CHANGE_INTERVAL_MS < t_now) {
        if (mainMixer.activeDeck != &DeckA) {
          randomEffect(&DeckA);
          Serial.printlnf("deckA.effect=%d (%p)", DeckA.fxEffectIndex, effectBank[DeckA.fxEffectIndex]);
        }
      }
      if (DeckB.tFxEffectStart + EFFECT_CHANGE_INTERVAL_MS < t_now) {
        if (mainMixer.activeDeck != &DeckB) {
          randomEffect(&DeckB);
          Serial.printlnf("deckB.effect=%d (%p)", DeckB.fxEffectIndex, effectBank[DeckB.fxEffectIndex]);
        }
      }
    }
    //stepFxParams(&mainMixer);
  }

  gLED->delay(1); // 8ms is 120fps
}
