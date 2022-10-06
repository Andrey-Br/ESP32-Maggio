#ifndef _ANDROID_CHEESE_SETTINGS_
#define _ANDROID_CHEESE_SETTINGS_

#include "Arduino.h"



// Макрос вывода значения переменной для отладки
#define Serial_DEBUG(x) do {Serial.print("DEBUG: ");Serial.print(#x);Serial.print(" = ");Serial.println(x);} while(0)



//Макросс. Если число выходит за крайние границы приравниет к краям
#define RANGE(x,min,max) ( ((x) > (min)) ? ( ((x) < (max)) ? (x) : (max) ) : (min) )


/*  ____Регулирование мощности____
  переменная увеличивается на 1 каждые TIME_ONE_PERIOD_PID
  Если переменная больше нужной мощности ТЭН выключается, иначе включается
  
*/

//Время одного шага для регулирования мощности ТЭНа (мс)
#define PID_TIME_ONE_PERIOD    100

//Максимальное количество регулирования
#define PID_COUNT_POWER_PERIOD 20


extern uint8_t pin_ten[3];
extern bool btn_active_ten[3];  

// Пины
#define PIN_ENC1_A      39
#define PIN_ENC1_B      34
#define PIN_ENC1_BTN    35

#define PIN_ENC2_A      32
#define PIN_ENC2_B      33
#define PIN_ENC2_BTN    25

#define PIN_ENC3_A      26
#define PIN_ENC3_B      27
#define PIN_ENC3_BTN    14

#define PIN_MIXER1      17
#define PIN_MIXER2      16

#define PIN_TEN 5
#define PIN_BTN_TEN1 18
#define PIN_BTN_TEN2 19
#define PIN_BTN_TEN3 21

#define PIN_KLAPAN 12

#define PIN_WATER_DETECT 36
#define PIN_STOP_BTN 4

#define PIN_TEMP_WATER 23
#define PIN_TEMP_MILK 22

#define PIN_TEMP_MILK 22
#define PIN_TEMP_WATER 23

//Каналы ШИМ сигналов на миксер
#define LEDC_MIXER1     0
#define LEDC_MIXER2     1

#endif