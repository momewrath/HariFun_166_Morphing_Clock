/*
 * mirel.lazar@gmail.com
 * 
 * provided 'AS IS', use it at your own risk
 */
#ifndef TINYICONS_H
#define TINYICONS_H

#include <Arduino.h>
#include <PxMatrix.h> // https://github.com/2dom/PxMatrix

/*
 * icons set
 */
//icons 10x5: 10 cols, 5 rows
/*int moony_ico [50] = {
  //3 nuances: 0x18c3 < 0x3186 < 0x4a49
  0x0000, 0x4a49, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x18c3,
  0x0000, 0x0000, 0x0000, 0x4a49, 0x3186, 0x3186, 0x18c3, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x4a49, 0x4a49, 0x3186, 0x3186, 0x3186, 0x18c3, 0x0000, 0x0000,
  0x0000, 0x0000, 0x4a49, 0x3186, 0x3186, 0x3186, 0x3186, 0x18c3, 0x0000, 0x0000,
};

int moony1_ico [50] = {
  //3 nuances: 0x18c3 < 0x3186 < 0x4a49
  0x0000, 0x18c3, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x4a49,
  0x0000, 0x0000, 0x0000, 0x4a49, 0x3186, 0x3186, 0x18c3, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x4a49, 0x4a49, 0x3186, 0x3186, 0x3186, 0x18c3, 0x0000, 0x0000,
  0x0000, 0x0000, 0x4a49, 0x3186, 0x3186, 0x3186, 0x3186, 0x18c3, 0x0000, 0x0000,
};

int moony2_ico [50] = {
  //3 nuances: 0x18c3 < 0x3186 < 0x4a49
  0x0000, 0x3186, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x3186,
  0x0000, 0x0000, 0x0000, 0x4a49, 0x3186, 0x3186, 0x18c3, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x4a49, 0x4a49, 0x3186, 0x3186, 0x3186, 0x18c3, 0x0000, 0x0000,
  0x0000, 0x0000, 0x4a49, 0x3186, 0x3186, 0x3186, 0x3186, 0x18c3, 0x0000, 0x0000,
};

int sunny_ico [50] = {
  0x0000, 0x0000, 0x0000, 0xffe0, 0x0000, 0x0000, 0xffe0, 0x0000, 0x0000, 0x0000,
  0x0000, 0xffe0, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xffe0, 0x0000,
  0x0000, 0x0000, 0x0000, 0xffe0, 0xffe0, 0xffe0, 0xffe0, 0x0000, 0x0000, 0x0000,
  0xffe0, 0x0000, 0xffe0, 0xffe0, 0xffe0, 0xffe0, 0xffe0, 0xffe0, 0x0000, 0xffe0,
  0x0000, 0x0000, 0xffe0, 0xffe0, 0xffe0, 0xffe0, 0xffe0, 0xffe0, 0x0000, 0x0000,
};

int sunny1_ico [50] = {
  0x0000, 0x0000, 0x0000, 0xffe0, 0x0000, 0x0000, 0xffff, 0x0000, 0x0000, 0x0000,
  0x0000, 0xffff, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xffe0, 0x0000,
  0x0000, 0x0000, 0x0000, 0xffe0, 0xffe0, 0xffe0, 0xffe0, 0x0000, 0x0000, 0x0000,
  0xffe0, 0x0000, 0xffe0, 0xffe0, 0xffe0, 0xffe0, 0xffe0, 0xffe0, 0x0000, 0xffff,
  0x0000, 0x0000, 0xffe0, 0xffe0, 0xffe0, 0xffe0, 0xffe0, 0xffe0, 0x0000, 0x0000,
};

int sunny2_ico [50] = {
  0x0000, 0x0000, 0x0000, 0xffff, 0x0000, 0x0000, 0xffe0, 0x0000, 0x0000, 0x0000,
  0x0000, 0xffe0, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xffff, 0x0000,
  0x0000, 0x0000, 0x0000, 0xffe0, 0xffe0, 0xffe0, 0xffe0, 0x0000, 0x0000, 0x0000,
  0xffff, 0x0000, 0xffe0, 0xffe0, 0xffe0, 0xffe0, 0xffe0, 0xffe0, 0x0000, 0xffe0,
  0x0000, 0x0000, 0xffe0, 0xffe0, 0xffe0, 0xffe0, 0xffe0, 0xffe0, 0x0000, 0x0000,
};

int cloudy_ico [50] = {
  0x0000, 0x0000, 0x0000, 0xffe0, 0x0000, 0x0000, 0xffe0, 0x0000, 0x0000, 0x0000,
  0x0000, 0xffe0, 0x0000, 0x0000, 0x0000, 0x0000, 0xc618, 0xc618, 0xffe0, 0x0000,
  0x0000, 0x0000, 0x0000, 0xffe0, 0xffe0, 0xc618, 0xc618, 0xc618, 0xc618, 0x0000,
  0xffe0, 0x0000, 0xffe0, 0xffe0, 0xffe0, 0xc618, 0xc618, 0xc618, 0xc618, 0xc618,
  0x0000, 0x0000, 0xffe0, 0xffe0, 0xffe0, 0xffe0, 0xc618, 0xc618, 0xc618, 0xc618,
};

int cloudy1_ico [50] = {
  0x0000, 0x0000, 0x0000, 0xffff, 0x0000, 0x0000, 0xffe0, 0x0000, 0x0000, 0x0000,
  0x0000, 0xffe0, 0x0000, 0x0000, 0x0000, 0x0000, 0xc618, 0xc618, 0xffff, 0x0000,
  0x0000, 0x0000, 0x0000, 0xffe0, 0xffe0, 0xc618, 0xc618, 0xc618, 0xc618, 0x0000,
  0xffff, 0x0000, 0xffe0, 0xffe0, 0xffe0, 0xc618, 0xc618, 0xc618, 0xc618, 0xc618,
  0x0000, 0x0000, 0xffe0, 0xffe0, 0xffe0, 0xffe0, 0xc618, 0xc618, 0xc618, 0xc618,
};

int cloudy2_ico [50] = {
  0x0000, 0x0000, 0x0000, 0xffe0, 0x0000, 0x0000, 0xffff, 0x0000, 0x0000, 0x0000,
  0x0000, 0xffff, 0x0000, 0x0000, 0x0000, 0x0000, 0xc618, 0xc618, 0xffe0, 0x0000,
  0x0000, 0x0000, 0x0000, 0xffe0, 0xffe0, 0xc618, 0xc618, 0xc618, 0xc618, 0x0000,
  0xffe0, 0x0000, 0xffe0, 0xffe0, 0xffe0, 0xc618, 0xc618, 0xc618, 0xc618, 0xc618,
  0x0000, 0x0000, 0xffe0, 0xffe0, 0xffe0, 0xffe0, 0xc618, 0xc618, 0xc618, 0xc618,
};

int ovrcst_ico [50] = {
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xc618, 0xc618, 0x0000, 0x0000,
  0x0000, 0x0000, 0xc618, 0xc618, 0x0000, 0xc618, 0xc618, 0xc618, 0xc618, 0x0000,
  0x0000, 0xc618, 0xffff, 0xffff, 0xc618, 0xffff, 0xffff, 0xffff, 0xc618, 0x0000,
  0x0000, 0xc618, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xc618, 0x0000,
  0x0000, 0x0000, 0xc618, 0xc618, 0xc618, 0xc618, 0xc618, 0xc618, 0x0000, 0x0000,
};

int ovrcst1_ico [50] = {
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xc618, 0xc618, 0x0000, 0x0000,
  0x0000, 0x0000, 0xc618, 0xc618, 0x0000, 0xc618, 0xc618, 0xc618, 0xc618, 0x0000,
  0x0000, 0xc618, 0xc618, 0xc618, 0xc618, 0xffff, 0xffff, 0xffff, 0xc618, 0x0000,
  0x0000, 0xc618, 0xc618, 0xc618, 0xffff, 0xffff, 0xffff, 0xffff, 0xc618, 0x0000,
  0x0000, 0x0000, 0xc618, 0xc618, 0xc618, 0xc618, 0xc618, 0xc618, 0x0000, 0x0000,
};

int ovrcst2_ico [50] = {
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xc618, 0xc618, 0x0000, 0x0000,
  0x0000, 0x0000, 0xc618, 0xc618, 0x0000, 0xc618, 0xc618, 0xc618, 0xc618, 0x0000,
  0x0000, 0xc618, 0xffff, 0xffff, 0xc618, 0xc618, 0xc618, 0xc618, 0xc618, 0x0000,
  0x0000, 0xc618, 0xffff, 0xffff, 0xffff, 0xc618, 0xc618, 0xc618, 0xc618, 0x0000,
  0x0000, 0x0000, 0xc618, 0xc618, 0xc618, 0xc618, 0xc618, 0xc618, 0x0000, 0x0000,
};

int thndr_ico [50] = {
  0x041f, 0xc618, 0x041f, 0xc618, 0xc618, 0xc618, 0x041f, 0xc618, 0xc618, 0x041f,
  0xc618, 0xc618, 0xc618, 0xc618, 0x041f, 0xc618, 0xc618, 0x041f, 0xc618, 0xc618,
  0xc618, 0x041f, 0xc618, 0xc618, 0xc618, 0x041f, 0xc618, 0xc618, 0xc618, 0xc618,
  0xc618, 0xc618, 0xc618, 0x041f, 0xc618, 0xc618, 0xc618, 0xc618, 0xc618, 0x041f,
  0xc618, 0x041f, 0xc618, 0xc618, 0xc618, 0xc618, 0x041f, 0xc618, 0x041f, 0xc618,
};

int rain_ico [50] = {
  0x041f, 0x0000, 0x041f, 0x0000, 0x0000, 0x0000, 0x041f, 0x0000, 0x0000, 0x041f,
  0x0000, 0x0000, 0x0000, 0x0000, 0x041f, 0x0000, 0x0000, 0x041f, 0x0000, 0x0000,
  0x0000, 0x041f, 0x0000, 0x0000, 0x0000, 0x041f, 0x0000, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000, 0x041f, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x041f,
  0x0000, 0x041f, 0x0000, 0x0000, 0x0000, 0x0000, 0x041f, 0x0000, 0x041f, 0x0000,
};

int rain1_ico [50] = {
  0x0000, 0x041f, 0x0000, 0x0000, 0x0000, 0x0000, 0x041f, 0x0000, 0x041f, 0x0000,
  0x041f, 0x0000, 0x041f, 0x0000, 0x0000, 0x0000, 0x041f, 0x0000, 0x0000, 0x041f,
  0x0000, 0x0000, 0x0000, 0x0000, 0x041f, 0x0000, 0x0000, 0x041f, 0x0000, 0x0000,
  0x0000, 0x041f, 0x0000, 0x0000, 0x0000, 0x041f, 0x0000, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000, 0x041f, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x041f,
};

int rain2_ico [50] = {
  0x0000, 0x0000, 0x0000, 0x041f, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x041f,
  0x0000, 0x041f, 0x0000, 0x0000, 0x0000, 0x0000, 0x041f, 0x0000, 0x041f, 0x0000,
  0x041f, 0x0000, 0x041f, 0x0000, 0x0000, 0x0000, 0x041f, 0x0000, 0x0000, 0x041f,
  0x0000, 0x0000, 0x0000, 0x0000, 0x041f, 0x0000, 0x0000, 0x041f, 0x0000, 0x0000,
  0x0000, 0x041f, 0x0000, 0x0000, 0x0000, 0x041f, 0x0000, 0x0000, 0x0000, 0x0000,
};

int rain3_ico [50] = {
  0x0000, 0x041f, 0x0000, 0x0000, 0x0000, 0x041f, 0x0000, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000, 0x041f, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x041f,
  0x0000, 0x041f, 0x0000, 0x0000, 0x0000, 0x0000, 0x041f, 0x0000, 0x041f, 0x0000,
  0x041f, 0x0000, 0x041f, 0x0000, 0x0000, 0x0000, 0x041f, 0x0000, 0x0000, 0x041f,
  0x0000, 0x0000, 0x0000, 0x0000, 0x041f, 0x0000, 0x0000, 0x041f, 0x0000, 0x0000,
};

int rain4_ico [50] = {
  0x0000, 0x0000, 0x0000, 0x0000, 0x041f, 0x0000, 0x0000, 0x041f, 0x0000, 0x0000,
  0x0000, 0x041f, 0x0000, 0x0000, 0x0000, 0x041f, 0x0000, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000, 0x041f, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x041f,
  0x0000, 0x041f, 0x0000, 0x0000, 0x0000, 0x0000, 0x041f, 0x0000, 0x041f, 0x0000,
  0x041f, 0x0000, 0x041f, 0x0000, 0x0000, 0x0000, 0x041f, 0x0000, 0x0000, 0x041f,
};

int snow_ico [50] = {
  0xc618, 0x0000, 0xc618, 0x0000, 0x0000, 0x0000, 0xc618, 0x0000, 0x0000, 0xc618,
  0x0000, 0x0000, 0x0000, 0x0000, 0xc618, 0x0000, 0x0000, 0xc618, 0x0000, 0x0000,
  0x0000, 0xc618, 0x0000, 0x0000, 0x0000, 0xc618, 0x0000, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000, 0xc618, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xc618,
  0x0000, 0xc618, 0x0000, 0x0000, 0x0000, 0x0000, 0xc618, 0x0000, 0xc618, 0x0000,
};

int snow1_ico [50] = {
  0x0000, 0xc618, 0x0000, 0x0000, 0x0000, 0x0000, 0xc618, 0x0000, 0xc618, 0x0000,
  0xc618, 0x0000, 0xc618, 0x0000, 0x0000, 0x0000, 0xc618, 0x0000, 0x0000, 0xc618,
  0x0000, 0x0000, 0x0000, 0x0000, 0xc618, 0x0000, 0x0000, 0xc618, 0x0000, 0x0000,
  0x0000, 0xc618, 0x0000, 0x0000, 0x0000, 0xc618, 0x0000, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000, 0xc618, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xc618,
};

int snow2_ico [50] = {
  0x0000, 0x0000, 0x0000, 0xc618, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xc618,
  0x0000, 0xc618, 0x0000, 0x0000, 0x0000, 0x0000, 0xc618, 0x0000, 0xc618, 0x0000,
  0xc618, 0x0000, 0xc618, 0x0000, 0x0000, 0x0000, 0xc618, 0x0000, 0x0000, 0xc618,
  0x0000, 0x0000, 0x0000, 0x0000, 0xc618, 0x0000, 0x0000, 0xc618, 0x0000, 0x0000,
  0x0000, 0xc618, 0x0000, 0x0000, 0x0000, 0xc618, 0x0000, 0x0000, 0x0000, 0x0000,
};

int snow3_ico [50] = {
  0x0000, 0xc618, 0x0000, 0x0000, 0x0000, 0xc618, 0x0000, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000, 0xc618, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xc618,
  0x0000, 0xc618, 0x0000, 0x0000, 0x0000, 0x0000, 0xc618, 0x0000, 0xc618, 0x0000,
  0xc618, 0x0000, 0xc618, 0x0000, 0x0000, 0x0000, 0xc618, 0x0000, 0x0000, 0xc618,
  0x0000, 0x0000, 0x0000, 0x0000, 0xc618, 0x0000, 0x0000, 0xc618, 0x0000, 0x0000,
};

int snow4_ico [50] = {
  0x0000, 0x0000, 0x0000, 0x0000, 0xc618, 0x0000, 0x0000, 0xc618, 0x0000, 0x0000,
  0x0000, 0xc618, 0x0000, 0x0000, 0x0000, 0xc618, 0x0000, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000, 0xc618, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xc618,
  0x0000, 0xc618, 0x0000, 0x0000, 0x0000, 0x0000, 0xc618, 0x0000, 0xc618, 0x0000,
  0xc618, 0x0000, 0xc618, 0x0000, 0x0000, 0x0000, 0xc618, 0x0000, 0x0000, 0xc618,
};

int *suny_ani[] = {sunny_ico, sunny1_ico, sunny2_ico, sunny1_ico, sunny2_ico};
int *clod_ani[] = {cloudy_ico, cloudy1_ico, cloudy2_ico, cloudy1_ico, cloudy2_ico};
int *ovct_ani[] = {ovrcst_ico, ovrcst1_ico, ovrcst2_ico, ovrcst1_ico, ovrcst2_ico};
int *rain_ani[] = {rain_ico, rain1_ico, rain2_ico, rain3_ico, rain4_ico};
int *thun_ani[] = {thndr_ico, rain1_ico, rain2_ico, rain3_ico, rain4_ico};
int *snow_ani[] = {snow_ico, snow1_ico, snow2_ico, snow3_ico, snow4_ico};
int *mony_ani[] = {moony_ico, moony1_ico, moony2_ico, moony1_ico, moony2_ico};*/



void draw_icon(PxMATRIX* d, int *ico, char xo, char yo, int cols, int rows);

#endif

