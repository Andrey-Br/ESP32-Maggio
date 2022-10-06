#ifndef _ANDROID_CHEESE_USB_
#define _ANDROID_CHEESE_USB_

#include "Functions.h"
#include "Settings.h"
#include "Mixer.h"
#include "Temp.h"


#define SPLIT_CHAR '_'

//макрос для преобразовывания символов в число (нужен для switch)
#define S(x) ((uint16_t)( ((uint8_t)x[0]<<8) + ((uint8_t)x[1]) ) )  


void serialSend(String s1, String s2 = "");

void printInfo();

void command (String &s);

void checkSerial();


#endif
