#ifndef _ANDROID_CHEESE_MAIN_
#define _ANDROID_CHEESE_MAIN_

#include "Settings.h"
#include "Mixer.h"
#include "USB.h"
#include "Functions.h"
#include "Mixer.h"
#include "Encoders.h"
#include "Temp.h"
#include <EEPROM.h>

void setup() {

  Serial.begin(115200);
  EEPROM.begin(255);
  
  // Serial.println("Start");

  pinMode(PIN_MIXER1,OUTPUT);
  pinMode(PIN_MIXER2,OUTPUT);
  digitalWrite(PIN_MIXER1,LOW);
  digitalWrite(PIN_MIXER2,LOW);

  pinMode(PIN_TEN, OUTPUT);
  digitalWrite(PIN_TEN, LOW);
  
  for (uint8_t i = 0; i < 3; i++ )
    pinMode(pin_ten[i],INPUT_PULLUP);

  pinMode(PIN_KLAPAN, OUTPUT);

  pinMode(PIN_WATER_DETECT, INPUT);
  pinMode(PIN_STOP_BTN, INPUT_PULLUP);

  pinMode(PIN_TEMP_WATER, INPUT);
  pinMode(PIN_TEMP_MILK, INPUT);

  ledcSetup(LEDC_MIXER1,40000,8);
  ledcSetup(LEDC_MIXER2,40000,8);
  ledcWrite(LEDC_MIXER1,0);
  ledcWrite(LEDC_MIXER2,0);
  ledcAttachPin(PIN_MIXER1,LEDC_MIXER1);
  ledcAttachPin(PIN_MIXER2,LEDC_MIXER2);

  if (EEPROM.read(99) == 99 ) {
    EEPROM.get(100,CalibrateMilk[0].in);
    EEPROM.get(104,CalibrateMilk[0].out);
    EEPROM.get(108,CalibrateMilk[1].in);
    EEPROM.get(112,CalibrateMilk[1].out);
    
    EEPROM.get(116,CalibrateWater[0].in);
    EEPROM.get(120,CalibrateWater[0].out);
    EEPROM.get(124,CalibrateWater[1].in);
    EEPROM.get(128,CalibrateWater[1].out);
  } else {
    Serial.println("non loaded");
    EEPROM.write(99,99);
    saveCalibrateInEEPROM();
  };

  /*
    TEST ***********
  */

  parametrsCheck();

  Cooking.setHeatActive(false);
  Cooking.setDelayActive(false);

  if (b_redBtnPress) {
  Cooking.setHeatTemp(70);
  Cooking.setDelayTime(1200); // 20 минут
  };

  Cooking.setCoolingTemp(25);
  // Cooking.setHeatActive(true);
  // Cooking.setDelayActive(true);

  Cooking.setAutoCooking(true);
  /*
    TEST ***********
  */


}

uint64_t time_temp = millis();

void loop() {
  temptick();
  encodersTick();
  parametrsCheck();
  
  checkSerial();
  
  Mixer.tick();
  Cooking.tick();

  //устраняет дребезг кнопок, возможно не нужен, при аппаратной защите
  delay(1);
}


#endif