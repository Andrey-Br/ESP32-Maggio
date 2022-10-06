#ifndef _ANDROID_CHEESE_ENCODERS_H
#define _ANDROID_CHEESE_ENCODERS_H

// #include "EncButton.h"
#include "EncButton2.h"
#include "Settings.h"
#include "USB.h"

extern EncButton2 <EB_ENCBTN> encA;
extern EncButton2 <EB_ENCBTN> encB;
extern EncButton2 <EB_ENCBTN> encC;

//EncButton<EB_TICK, PIN_ENC1_A, PIN_ENC1_B, PIN_ENC1_BTN> encA;
// EncButton<EB_TICK, PIN_ENC2_A, PIN_ENC2_B, PIN_ENC2_BTN> encB;
// EncButton<EB_TICK, PIN_ENC3_A, PIN_ENC3_B, PIN_ENC3_BTN> encC;

void oneEncoderCheck (EncButton2 <EB_ENCBTN> &enc, String c);

void encodersInit();

void encodersTick();


#endif