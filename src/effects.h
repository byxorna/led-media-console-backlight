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

typedef void (*EffectFunction)(Output*,uint8_t,uint8_t,uint8_t);

void effect_wave_decay(Output* s, uint8_t dw, uint8_t p1, uint8_t p2) {
  int x = NSFastLED::beatsin8(12, 0, NUM_LEDS);
  //Serial.printlnf("i=%d f=%d", x, f);
  for(int i = 0; i < NUM_LEDS; ++i) {
    int f = NSFastLED::beatsin8(24, 0, 128+(dw/2), 0, i);
    //Serial.printlnf("i=%d f=%d", i, f);
    if (x < i+3 && x > i+3) {
      s->leds[i].fadeToBlackBy(f);
    }
  }
}

void effect_random_decay(Output* s, uint8_t dw, uint8_t p1, uint8_t p2) {
  for(int i = 0; i < NUM_LEDS; ++i) {
    if (random(NUM_LEDS) < NUM_LEDS/4) {
      s->leds[i].fadeToBlackBy(random(5) * dw / 256 );
    }
  }
}

void effect_sparkles(Output* s, uint8_t dw, uint8_t p1, uint8_t p2) {
  uint8_t b = 255-dw;
  // cap brightness at 50% of max
  if (b > 127) {
    b = 127;
  }
  for(int i = 0; i < NUM_LEDS; ++i) {
    if (random(NUM_LEDS) == 0) {
      s->leds[i] = NSFastLED::CRGB::White;
      s->leds[i].fadeToBlackBy(255-dw);
    }
  }
}

#endif
