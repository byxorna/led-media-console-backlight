#ifndef _STRUCTS_H_
#define _STRUCTS_H_

#ifndef _PARTICLE_H_
#include "Particle.h"
#endif
#ifndef _FASTLED_H_
#include "FastLED.h"
#endif

typedef struct Deck {
  uint8_t label;
  //float crossfadePositionActive;
  uint8_t pattern;
  uint8_t palette;
  uint8_t animationIndex;
  NSFastLED::CRGBPalette16 currentPalette; // current color palette
  unsigned long tPatternStart;  // time last pattern changed
  unsigned long tPaletteStart;  // time last palette changed
  NSFastLED::CRGB* leds;
  uint8_t fxEffectIndex;    // which effect in effectBank is active (prefader)
  unsigned long tFxEffectStart; // when the last effect was changed
} Deck;


typedef struct Mixer {
  float crossfadePosition;
  //int crossfadeDirection;
  bool crossfadeInProgress;
  unsigned long tLastCrossfade;
  uint8_t fxEffectIndex;    // which effect in effectBank is active (postfader)
  uint8_t fxDryWet;         // fx1 d/w
  uint8_t fxParam1;         // fx1 param 1
  uint8_t fxParam2;         // fx1 param 2
  unsigned long tFxEffectStart; // when the last effect was changed
  Deck* a;
  Deck* b;
  Deck* out;
} Mixer;

#endif
