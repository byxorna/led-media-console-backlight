#ifndef _PALETTES_H_
#define _PALETTES_H_

#ifndef _PARTICLE_H_
#include "Particle.h"
#endif
#ifndef _FASTLED_H_
#include "FastLED.h"
#endif


// Gradient palette "bhw1_28_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw1/tn/bhw1_28.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 32 bytes of program space.

extern const NSFastLED::TProgmemRGBGradientPalette_byte bhw1_28_gp[] {
    0,  75,  1,221,
   30, 252, 73,255,
   48, 169,  0,242,
  119,   0,149,242,
  170,  43,  0,242,
  206, 252, 73,255,
  232,  78, 12,214,
  255,   0,149,242};


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

// Gradient palette "hg_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/esdb/tn/hg.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 72 bytes of program space.

extern const NSFastLED::TProgmemRGBGradientPalette_byte hg_gp [] {
    0, 171,117, 71,
   28, 171,117, 71,
   28, 255, 55, 71,
   56, 255, 55, 71,
   56, 255,156, 71,
   84, 255,156, 71,
   84, 171,255, 71,
  113, 171,255, 71,
  113, 101,255, 71,
  141, 101,255, 71,
  141, 101,255,145,
  170, 101,255,145,
  170, 101,117,174,
  198, 101,117,174,
  198, 171, 82,255,
  226, 171, 82,255,
  226, 255, 55,255,
  255, 255, 55,255};



// Gradient palette "red_g0_4_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/go2/webtwo/tn/red-g0-4.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 8 bytes of program space.

extern const NSFastLED::TProgmemRGBGradientPalette_byte red_g0_4_gp [] {
    0, 121,  4,  3,
  255, 210,  1,  1};

// for effects that are palette based
NSFastLED::CRGBPalette16 palettes[] = {
  //hg_gp,
  //red_g0_4_gp, /too simple
  bhw1_purplered_gp,
  alarm_p1_0_3_gp,
  rainbow_gp,
  bhw1_28_gp,
};
#define PALETTES_COUNT (sizeof(palettes)/sizeof(*palettes))


#endif
