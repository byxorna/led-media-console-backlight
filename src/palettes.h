#ifndef _PALETTES_H_
#define _PALETTES_H_

#ifndef _PARTICLE_H_
#include "Particle.h"
#endif
#ifndef _FASTLED_H_
#include "FastLED.h"
#endif

/* custom color palettes */

// Gradient palette "Lucy_in_the_Sky_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/colo/vredeling/tn/Lucy_in_the_Sky.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 40 bytes of program space.

extern const NSFastLED::TProgmemRGBGradientPalette_byte Lucy_in_the_Sky_gp [] {
    0, 175,  1, 44,
   51, 175,  1, 44,
   51, 194, 14, 35,
  102, 194, 14, 35,
  102, 213, 40, 27,
  153, 213, 40, 27,
  153, 234, 80, 21,
  204, 234, 80, 21,
  204, 255,136, 15,
  255, 255,136, 15};

// Gradient palette "bhw4_018_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw4/tn/bhw4_018.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 28 bytes of program space.

extern const NSFastLED::TProgmemRGBGradientPalette_byte bhw4_018_gp [] {
    0,  32,  1, 26,
   45,  49,  1, 11,
   76, 121,  2,  8,
  104, 194,  7,  3,
  127, 222, 36,  1,
  160, 227,107,  1,
  255,  92, 16,  2};

// Gradient palette "temperature_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/arendal/tn/temperature.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 144 bytes of program space.

extern const NSFastLED::TProgmemRGBGradientPalette_byte temperature_gp [] {
    0,   1, 27,105,
   14,   1, 27,105,
   14,   1, 40,127,
   28,   1, 40,127,
   28,   1, 70,168,
   42,   1, 70,168,
   42,   1, 92,197,
   56,   1, 92,197,
   56,   1,119,221,
   70,   1,119,221,
   70,   3,130,151,
   84,   3,130,151,
   84,  23,156,149,
   99,  23,156,149,
   99,  67,182,112,
  113,  67,182,112,
  113, 121,201, 52,
  127, 121,201, 52,
  127, 142,203, 11,
  141, 142,203, 11,
  141, 224,223,  1,
  155, 224,223,  1,
  155, 252,187,  2,
  170, 252,187,  2,
  170, 247,147,  1,
  184, 247,147,  1,
  184, 237, 87,  1,
  198, 237, 87,  1,
  198, 229, 43,  1,
  212, 229, 43,  1,
  212, 220, 15,  1,
  226, 220, 15,  1,
  226, 171,  2,  2,
  240, 171,  2,  2,
  240,  80,  3,  3,
  255,  80,  3,  3};

// Gradient palette "bhw1_purplered_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw1/tn/bhw1_purplered.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 8 bytes of program space.

extern const NSFastLED::TProgmemRGBGradientPalette_byte bhw1_purplered_gp [] {
    0, 255,  0,  0,
  255, 107,  1,205};

// Gradient palette "alarm_p1_0_3_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/dca/tn/alarm.p1.0.3.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 84 bytes of program space.

extern const NSFastLED::TProgmemRGBGradientPalette_byte alarm_p1_0_3_gp [] {
    0, 182,255,168,
   12, 255,149, 17,
   25, 255, 68,  6,
   38, 255, 25,  2,
   51, 255,  5,  1,
   63, 255,  1,  1,
   76, 255,  1, 34,
   89, 255,  1,114,
  102, 255,  1,240,
  114, 135,  1,255,
  127,  59,  1,255,
  140,  20,  1,255,
  153,   3,  1,255,
  165,   1,  1,255,
  178,   1, 12,255,
  191,   1, 36,255,
  204,   1, 72,255,
  216,   1,118,255,
  229,   1,178,255,
  242,   1,244,255,
  255,   1,255,186};

// Gradient palette "rainbow_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/ds9/tn/rainbow.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 24 bytes of program space.

extern const NSFastLED::TProgmemRGBGradientPalette_byte rainbow_gp [] {
    0, 255,  0,255,
   51,   0,  0,255,
  102,   0,255,255,
  153,   0,255,  0,
  204, 255,255,  0,
  255, 255,  0,  0};


// Gradient palette "red_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/ds9/tn/red.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 8 bytes of program space.

extern const NSFastLED::TProgmemRGBGradientPalette_byte red_gp [] {
    0,   0,  0,  0,
  255, 255,  0,  0};

// Gradient palette "blue_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/ds9/tn/blue.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 8 bytes of program space.

extern const NSFastLED::TProgmemRGBGradientPalette_byte blue_gp [] {
    0,   0,  0,  0,
  255,   0,  0,255};


// for effects that are palette based
NSFastLED::CRGBPalette16 palettes[] = {
  temperature_gp,
  bhw4_018_gp,
  Lucy_in_the_Sky_gp,
  bhw1_purplered_gp,
  alarm_p1_0_3_gp,
  rainbow_gp,
  red_gp,
  blue_gp,
  NSFastLED::HeatColors_p,
};
#define PALETTES_COUNT (sizeof(palettes)/sizeof(*palettes))

#endif
