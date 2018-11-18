#ifndef _EFFECTS_H_
#define _EFFECTS_H_

#ifndef _PARTICLE_H_
#include "Particle.h"
#endif
#ifndef _FASTLED_H_
#include "FastLED.h"
#endif
#ifndef _STRUCTS_H_
#include "structs.h"
#endif

typedef void (*EffectFunction)(Deck*,uint8_t,uint8_t,uint8_t);

void effect_reverse(Deck* s, uint8_t dw, uint8_t p1, uint8_t p2) {
  NSFastLED::CRGB flex = NSFastLED::CRGB::Black;
  for(int i = 0; i < NUM_LEDS/2; ++i) {
    flex = s->leds[i];
    s->leds[i] = s->leds[NUM_LEDS-i-1];
    s->leds[NUM_LEDS-i-1] = flex;
  }
}

void effect_mirror(Deck* s, uint8_t dw, uint8_t p1, uint8_t p2) {
  for(int i = 0; i < NUM_LEDS/2; ++i) {
    s->leds[i] = s->leds[NUM_LEDS-i-1];
  }
}

void effect_reverse_mirror(Deck* s, uint8_t dw, uint8_t p1, uint8_t p2) {
  NSFastLED::CRGB flex = NSFastLED::CRGB::Black;
  for(int i = 0; i < NUM_LEDS/2; ++i) {
    flex = s->leds[NUM_LEDS/2 - 1 - i];
    s->leds[NUM_LEDS/2 - 1 - i] = s->leds[i];
    s->leds[i] = flex;
    // now mirror this over the mid axis
    s->leds[NUM_LEDS/2-1 + (NUM_LEDS/2-1-i)] = s->leds[i];
  }
}



#endif
