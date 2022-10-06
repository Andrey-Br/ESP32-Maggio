#include "Encoders.h"

EncButton2 <EB_ENCBTN> encA (INPUT_PULLUP, PIN_ENC1_A, PIN_ENC1_B, PIN_ENC1_BTN);
EncButton2 <EB_ENCBTN> encB (INPUT_PULLUP, PIN_ENC2_A, PIN_ENC2_B, PIN_ENC2_BTN);
EncButton2 <EB_ENCBTN> encC (INPUT_PULLUP, PIN_ENC3_A, PIN_ENC3_B, PIN_ENC3_BTN);

void oneEncoderCheck (EncButton2 <EB_ENCBTN> &enc, String c){
  enc.tick();

  if (enc.releaseStep(enc.clicks)) serialSend(c+ "U");

  if (enc.right()) serialSend(c + "+");
  if (enc.left()) serialSend(c+ "-");

  if (enc.rightH()) serialSend(c+ "R");
  if (enc.leftH()) serialSend(c+ "L");

  if (enc.click()) serialSend(c+ "C");
  if (enc.held()) serialSend(c+ "H", String(enc.clicks+1));
  if (enc.hasClicks()) serialSend(c+ "N",String(enc.clicks));

}

void encodersInit(){
  encA.setStepTimeout(32);
  encB.setStepTimeout(32);
  encC.setStepTimeout(32);
};

void encodersTick(){
  oneEncoderCheck(encA,"A");
  oneEncoderCheck(encB,"B");
  oneEncoderCheck(encC,"C");
};
