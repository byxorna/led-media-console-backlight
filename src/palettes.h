#ifndef _PALETTES_H_
#define _PALETTES_H_

#ifndef _PARTICLE_H_
#include "Particle.h"
#endif
#ifndef _FASTLED_H_
#include "FastLED.h"
#endif

/* custom color palettes */
// orange 255,102,0 FF6600
// pink 255,0,255 #ff00ff
// pornj 255,51,51 #ff3333
extern const NSFastLED::TProgmemRGBGradientPalette_byte Disorient_gp[] = {
      0,   0,   0,   0,    // black
     75, 255,  26, 153,    // pink
    147, 255,  51,  51,    // pornj
    208, 255, 111,  15,    // orange
    255, 255, 255, 255, }; // white

// Gradient palette "bhw1_01_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw1/tn/bhw1_01.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 12 bytes of program space.

extern const NSFastLED::TProgmemRGBGradientPalette_byte  bhw1_01_gp [] {
    0, 227,101,  3,
  117, 194, 18, 19,
  255,  92,  8,192};

// Gradient palette "bhw1_04_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw1/tn/bhw1_04.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 20 bytes of program space.

extern const NSFastLED::TProgmemRGBGradientPalette_byte  bhw1_04_gp [] {
    0, 229,227,  1,
   15, 227,101,  3,
  142,  40,  1, 80,
  198,  17,  1, 79,
  255,   0,  0, 45};

// Gradient palette "bhw3_50_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw3/tn/bhw3_50.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 36 bytes of program space.

extern const NSFastLED::TProgmemRGBGradientPalette_byte  bhw3_50_gp [] {
    0, 171,  1, 34,
   25, 224, 29, 84,
   58, 222, 70,228,
   89, 227, 90, 59,
  127, 255,136,125,
  165, 227, 90, 59,
  188, 222, 70,228,
  216, 224, 29, 84,
  255, 171,  1, 34};

// Gradient palette "bhw3_52_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw3/tn/bhw3_52.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 28 bytes of program space.

extern const NSFastLED::TProgmemRGBGradientPalette_byte bhw3_52_gp [] {
    0,  31,  1, 27,
   45,  34,  1, 16,
   99, 137,  5,  9,
  132, 213,128, 10,
  175, 199, 22,  1,
  201, 199,  9,  6,
  255,   1,  0,  1};

// Gradient palette "bhw3_02_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw3/tn/bhw3_02.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 24 bytes of program space.

extern const NSFastLED::TProgmemRGBGradientPalette_byte bhw3_02_gp [] {
    0, 121,  1,  1,
   63, 255, 57,  1,
  112, 255, 79, 25,
  145, 255, 79, 25,
  188, 244,146,  3,
  255, 115, 14,  1};


// https://gist.github.com/kriegsman/8281905786e8b2632aeb

// Gradient palette "es_pinksplash_08_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/es/pink_splash/tn/es_pinksplash_08.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 20 bytes of program space.
extern const NSFastLED::TProgmemRGBGradientPalette_byte es_pinksplash_08_gp[] = {
    0, 126, 11,255,
  127, 197,  1, 22,
  175, 210,157,172,
  221, 157,  3,112,
  255, 157,  3,112};

// Gradient palette "es_pinksplash_07_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/es/pink_splash/tn/es_pinksplash_07.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 28 bytes of program space.

extern const NSFastLED::TProgmemRGBGradientPalette_byte es_pinksplash_07_gp[] = {
    0, 229,  1,  1,
   61, 242,  4, 63,
  101, 255, 12,255,
  127, 249, 81,252,
  153, 255, 11,235,
  193, 244,  5, 68,
  255, 232,  1,  5};

// Gradient palette "Sunset_Real_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/nd/atmospheric/tn/Sunset_Real.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 28 bytes of program space.

extern const NSFastLED::TProgmemRGBGradientPalette_byte Sunset_Real_gp[] {
    0, 120,  0,  0,
   22, 179, 22,  0,
   51, 255,104,  0,
   85, 167, 22, 18,
  135, 100,  0,103,
  198,  16,  0,130,
  255,   0,  0,160};

// Gradient palette "rgi_15_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/ds/rgi/tn/rgi_15.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 36 bytes of program space.

extern const NSFastLED::TProgmemRGBGradientPalette_byte rgi_15_gp[] {
    0,   4,  1, 31,
   31,  55,  1, 16,
   63, 197,  3,  7,
   95,  59,  2, 17,
  127,   6,  2, 34,
  159,  39,  6, 33,
  191, 112, 13, 32,
  223,  56,  9, 35,
  255,  22,  6, 38};

extern const NSFastLED::TProgmemRGBGradientPalette_byte BlacK_Red_Magenta_Yellow_gp[] {
    0,   0,  0,  0,
   42,  42,  0,  0,
   84, 255,  0,  0,
  127, 255,  0, 45,
  170, 255,  0,255,
  212, 255, 55, 45,
  255, 255,255,  0};


// Gradient palette "Bad_Kitten_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/colo/Bionic_Blender/tn/Bad_Kitten.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 40 bytes of program space.

extern const NSFastLED::TProgmemRGBGradientPalette_byte Bad_Kitten_gp[] {
    0,   1,  1,  1,
   86,   1,  1,  1,
   86,  28,  2, 23,
  102,  28,  2, 23,
  102,  97,  3, 13,
  117,  97,  3, 13,
  117, 177, 12,  2,
  132, 177, 12,  2,
  132, 206, 79,  1,
  255, 206, 79,  1};

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


// Gradient palette "PuRd_08_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/cb/seq/tn/PuRd_08.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 64 bytes of program space.

extern const NSFastLED::TProgmemRGBGradientPalette_byte PuRd_08_gp [] {
    0, 234,231,240,
   31, 234,231,240,
   31, 197,193,216,
   63, 197,193,216,
   63, 157,125,172,
   95, 157,125,172,
   95, 137, 77,137,
  127, 137, 77,137,
  127, 179, 33,100,
  159, 179, 33,100,
  159, 197,  4, 54,
  191, 197,  4, 54,
  191, 146,  1, 16,
  223, 146,  1, 16,
  223,  58,  0,  7,
  255,  58,  0,  7};

// Gradient palette "Oranges_06_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/cb/seq/tn/Oranges_06.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 48 bytes of program space.

extern const NSFastLED::TProgmemRGBGradientPalette_byte Oranges_06_gp [] {
    0, 252,217,180,
   42, 252,217,180,
   42, 249,162, 82,
   84, 249,162, 82,
   84, 249,109, 29,
  127, 249,109, 29,
  127, 249, 69,  6,
  170, 249, 69,  6,
  170, 194, 22,  1,
  212, 194, 22,  1,
  212,  83,  8,  1,
  255,  83,  8,  1};

// Gradient palette "bhw4_098_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw4/tn/bhw4_098.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 32 bytes of program space.

extern const NSFastLED::TProgmemRGBGradientPalette_byte bhw4_098_gp [] {
    0, 128, 33, 52,
   35, 255, 17, 47,
   58, 222,  2, 51,
   99, 144, 56, 78,
  124, 188,115,137,
  178, 255, 16, 52,
  219, 199,  1,  4,
  255, 106,  1,  2};

  // Gradient palette "bhw4_058_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw4/tn/bhw4_058.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 28 bytes of program space.

extern const NSFastLED::TProgmemRGBGradientPalette_byte bhw4_058_gp [] {
    0,  14,  2,  8,
   51,  16,  5, 26,
   96, 110, 17, 91,
  127, 255, 35,133,
  168, 132, 16, 95,
  204,  83, 18, 80,
  255,  14,  3, 19};

// Gradient palette "bhw4_040_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw4/tn/bhw4_040.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 32 bytes of program space.

extern const NSFastLED::TProgmemRGBGradientPalette_byte bhw4_040_gp [] {
    0,  26,  7, 69,
   73, 110, 36,240,
  109, 249, 69,245,
  142, 255,182,255,
  173, 120,124,245,
  204,  98, 59,207,
  249,  30,  9,103,
  255,  30,  9,103};

// Gradient palette "bhw4_026_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw4/tn/bhw4_026.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 44 bytes of program space.

extern const NSFastLED::TProgmemRGBGradientPalette_byte bhw4_026_gp [] {
    0, 194,144,145,
   17, 234, 32, 37,
   43, 165,118,168,
   73, 190,  7,  4,
   96, 217, 33, 13,
  119, 244, 54,  3,
  137, 229,207,  2,
  175, 197, 41, 61,
  193, 165,118,168,
  211, 234, 32, 37,
  255, 227,125,147};

// Gradient palette "bhw4_035_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw4/tn/bhw4_035.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 20 bytes of program space.

extern const NSFastLED::TProgmemRGBGradientPalette_byte bhw4_035_gp [] {
    0, 239, 22, 19,
   43, 255, 92, 73,
  112, 255,175, 62,
  201, 118, 22, 47,
  255,  21,  1, 21};

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

// for effects that are palette based
NSFastLED::CRGBPalette16 palettes[] = {
  Disorient_gp,
  Lucy_in_the_Sky_gp,
  es_pinksplash_08_gp,
  es_pinksplash_07_gp,
  rgi_15_gp,
  bhw1_01_gp,
  bhw1_04_gp,
  bhw3_50_gp,
  bhw3_52_gp,
  bhw3_02_gp,
  bhw4_018_gp,
  bhw4_035_gp,
  bhw4_026_gp,
  bhw4_040_gp,
  bhw4_058_gp,
  bhw4_098_gp,
  PuRd_08_gp,
  Oranges_06_gp,
};
#define PALETTES_COUNT (sizeof(palettes)/sizeof(*palettes))

#endif
