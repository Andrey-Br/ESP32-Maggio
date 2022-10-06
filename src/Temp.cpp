#include "Temp.h"

OneWire ds_milk(PIN_TEMP_MILK);
OneWire ds_water(PIN_TEMP_WATER);

float temp_water, temp_milk , temp_milk_non_calibrate, temp_water_non_calibrate ;

//Данные о калибровке молока
str_Calibrate CalibrateMilk [2] = { {10,10}, {100,100} };

//Данные о калибровке воды
str_Calibrate CalibrateWater [2] = { {10,10}, {100,100} };


void saveCalibrateInEEPROM(){
  EEPROM.put(100,CalibrateMilk[0].in);
  EEPROM.put(104,CalibrateMilk[0].out);
  EEPROM.put(108,CalibrateMilk[1].in);
  EEPROM.put(112,CalibrateMilk[1].out);

  EEPROM.put(116,CalibrateWater[0].in);
  EEPROM.put(120,CalibrateWater[0].out);
  EEPROM.put(124,CalibrateWater[1].in);
  EEPROM.put(128,CalibrateWater[1].out);

  EEPROM.commit();
};

//Сбросить калибровку датчиков
void resetCalibrate(){
  CalibrateMilk[0].in = 10;
  CalibrateMilk[0].out = 10;
  CalibrateMilk[1].in = 100;
  CalibrateMilk[1].out = 100;

  CalibrateWater[0].in = 10;
  CalibrateWater[0].out = 10;
  CalibrateWater[1].in = 100;
  CalibrateWater[1].out = 100;

  saveCalibrateInEEPROM();
};

void setCalibrate(str_Calibrate &Calibr, float input_temp, float need_temp){
  Calibr.in = input_temp;
  Calibr.out = need_temp;

  saveCalibrateInEEPROM();
};

// оригинальная функция map, переделанная под float. Нужна для калибровки датчика
float map_f(float x, float in_min, float in_max, float out_min, float out_max) {
    const float run = in_max - in_min;
    if(run == 0){
        return -1; // AVR returns -1, SAM returns 0
    }
    const float rise = out_max - out_min;
    const float delta = x - in_min;
    return (delta * rise) / run + out_min;
}

float getDallas(OneWire &ds){
  ds.reset();  // сброс шины
  ds.write(0xCC, 0); // пропуск ROM 
  ds.write(0xBE, 0); // команда чтения памяти датчика 
  byte bufData[9];
  ds.read_bytes(bufData, 9);  // чтение памяти датчика, 9 байтов

  ds.reset();  // сброс шины
  ds.write(0xCC, 0); // пропуск ROM
  ds.write(0x44, 0); // инициализация измерения
  
  return (float)((int)bufData[0] | (((int)bufData[1]) << 8)) * 0.0625 + 0.03125;
}

String stringTemp(){
  String message;

  message+="TM";
  message+= (int32_t)(temp_milk*10);
  message+= "_TW";
  message+= (int32_t)(temp_water*10);

  return message;
}


void temptick(){
  static int64_t time_temp = millis();

  if (time_temp < millis()) {
      time_temp = millis() + TIME_RESET_TEMP_SENSOR;
      temp_milk_non_calibrate = getDallas(ds_milk);
      temp_water_non_calibrate = getDallas(ds_water);
      temp_milk = map_f (temp_milk_non_calibrate , CalibrateMilk[0].in, CalibrateMilk[1].in, CalibrateMilk[0].out, CalibrateMilk[1].out );
      temp_water = map_f (temp_water_non_calibrate , CalibrateWater[0].in, CalibrateWater[1].in, CalibrateWater[0].out, CalibrateWater[1].out );;

      serialSend(stringTemp());
  }; 

}