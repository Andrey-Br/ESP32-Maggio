#ifndef _ANROID_CHEESE_TEMP_H
#define _ANROID_CHEESE_TEMP_H

#include <OneWire.h>

#include "Settings.h"
#include "USB.h"
#include <EEPROM.h>

//С какой периодичностью оправшиваем датчики температуры
#define TIME_RESET_TEMP_SENSOR 1000

//объект датчика молока
extern OneWire ds_milk;
//объект датчика воды
extern OneWire ds_water;

//Структура для калибровки датчика in - "сырых" значение с датчика, out - какое должно быть
struct str_Calibrate{
  float in, out;
};

// Нижняя[0] и Верхняя[1] границы калибровки молока
extern str_Calibrate CalibrateMilk  [2];

// Нижняя[0] и Верхняя[1] границы калибровки молока
extern str_Calibrate CalibrateWater [2];

//текущая температура воды в рубашке
extern float temp_water;
//текущая откалиброванная температура молока
extern float temp_milk;

//"Сырые" значения с датчиков
extern float temp_milk_non_calibrate, temp_water_non_calibrate;

//Сохранить настройки калибровки в память
void saveCalibrateInEEPROM();

//Сбросить калибровку датчиков
void resetCalibrate();

// Измениьт калибровачное значение. При inputtemp будет needtemp
void setCalibrate(str_Calibrate &Calibr, float inputtemp, float needtemp);

// оригинальная функция map, переделанная под float. Нужна для калибровки датчика
float map_f(float x,float in_min, float in_max, float out_min, float out_max);

//сгенерировать строку для выдачи температуры
String stringTemp();

//Получить температуру от датчика
float getDallas(OneWire &ds);

//Получаем темперауры от датчиков
void temptick();

#endif