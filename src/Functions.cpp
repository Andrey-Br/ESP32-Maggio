#include "Functions.h"

uint8_t pin_ten[3] = {PIN_BTN_TEN1, PIN_BTN_TEN2, PIN_BTN_TEN3};
bool btn_active_ten[3] = {false, false, false};

float result;


//Ошибка воды в рубашке (true если воды нет,  false - вода в рубашке есть)
bool b_waterError = true;
//Нажата ли красная кнопка
bool b_redBtnPress = true;

//Проверка всех кнопок и датчика уровня воды
void parametrsCheck(){
  bool b;

  //инверсия, потому что при отсутсвии сигнала должно быть true
  b = !digitalRead(PIN_WATER_DETECT);
  if (b != b_waterError) {
    b_waterError = b;
    serialSend(b_waterError? "WE" : "WF");
  }

  // инверсия, потому что INPUT_PULLUP, если digitalRead = false - значит кнопка нажата
  b = !digitalRead(PIN_STOP_BTN);
  if ( b != b_redBtnPress) {
    b_redBtnPress = b;
    Mixer.reculc();

    serialSend(b_redBtnPress? "RP" : "RR");
  }

  for (int8_t i = 0; i < 3; i++){
    b = ! digitalRead(pin_ten[i]);
    // Serial_DEBUG (pin_ten[i]);
    
    if (b != btn_active_ten[i]) {
      btn_active_ten[i] = b;

      String message = "T";
      message += i;
      message += btn_active_ten[i] ? "1" : "0";

      serialSend(message);
    };
  };
};


C_Cooking Cooking;

//Установить готовку в атоматический режим
void C_Cooking::setAutoCooking(bool f){
  b_autoCooking = f;
  _mode = 0;
};

//Включить / выключить ТЭН в ручном режиме
void C_Cooking::setTen(bool f){
  b_ten = f;
};

//Включить / выключить Клапан в ручном режиме
void C_Cooking::setKlapan(bool f){
  b_klapan = f;
};

// Включить / выключить нагрев
void C_Cooking::setHeatActive(bool f){
  b_auto_heat = f;
};

// Включить / выключить выдержку
void C_Cooking::setDelayActive(bool f){
  b_auto_delay  = f;
};


// Включить / выключить охлаждение
void C_Cooking::setCoolingActive(bool f){
  b_auto_cooling = f;
};

// Установить температуру нагрева в автоматическом режиме готовки
void C_Cooking::setHeatTemp(float temp){
  // Serial_DEBUG(temp);
  if (temp == 0) setHeatActive(false);
  else{
  need_temp_heat = temp;
  setHeatActive(true);
  }
};

// Установить время выдержки в СЕК в автоматическом режиме готовки
void C_Cooking::setDelayTime(uint64_t time){
  if (time == 0) setDelayActive(false);
  else {
    need_delay_heat = time;
    setDelayActive(true);
  }
};

// Установить температуру охлаждения в автоматическом режиме готовки
void C_Cooking::setCoolingTemp(float temp){
  if (temp == 0) setCoolingActive(false);
  else {
    setCoolingActive(true);
    need_temp_cooling = temp;
  }
};

//Отключить все системы
void C_Cooking::disableAll(){
  setAutoCooking(false);
  setTen(false);
  setKlapan(false);

};


//Проверка всех систем
void C_Cooking::tick(){
  
  if (b_autoCooking) {
    
    switch (_mode)
    {
    case 0: 
      changeMode();
    break;
    
    case 1:
      // Serial.println("heat");
      if (!b_auto_heat) _mode = 0;
      else {
        //  *********   TEST ***********
        if (b_waterError) {
          klapanWork(true);
           tenWork( false );
        } 
        //  *********   TEST ***********
        else 
        {
          klapanWork(false);
          tenWork( PIDRegulator() );
        }

        //гестерезис
        if ( need_temp_heat - temp_milk <= 0.2 ) changeMode();
      }
    break;
    
    case 2:
        /////////// TEST /////////////////
        klapanWork(b_waterError);   //false
        //////////////////////////

        if (b_auto_heat) tenWork( PIDRegulator() );
        else tenWork( false );

        if (time_delay <= millis()) changeMode();
    
    break;
    
    case 3:

      klapanWork(true);
      tenWork(false);

      if (temp_milk <= need_temp_cooling) changeMode();
    
    break;
    
    case 4:
    default:
    Cooking.disableAll();
    break;
    
    };
  } 
  else {
    tenWork( b_ten );
    klapanWork( b_klapan );
  }
};


//работа ТЭНа
void C_Cooking::tenWork(bool f){
  if (b_redBtnPress || b_waterError || !( btn_active_ten[0] || btn_active_ten[1] ||btn_active_ten[2] ))
    digitalWrite(PIN_TEN, false);

  else digitalWrite(PIN_TEN, f);
};

//работа Клапана
void C_Cooking::klapanWork(bool f){
  if (b_redBtnPress) digitalWrite(PIN_KLAPAN, false);
  else digitalWrite(PIN_KLAPAN, f);
};


//регулятор плавного нагрева
bool C_Cooking::PIDRegulator(){
  static uint64_t time = millis() + PID_TIME_ONE_PERIOD;
  static uint8_t count = 1;
  // static float result; 
  
  //Если рубашка холоднее необходимой температуры, то включаем
  if (need_temp_heat - temp_water > 1) return true;

  if (time < millis () ) {
    time = millis() + PID_TIME_ONE_PERIOD;
    count++;
    if (count > PID_COUNT_POWER_PERIOD) count = 1;

            /* Разница между нужной и текущей температурой молока,
            чем больше тем сильнее выключаем мощность*/
            float t1 = need_temp_heat - temp_milk;
            /* Разница между температурой воды и температурой молока,
            чем больше тем сильнее выключаем мощность  */
            float t2 = temp_water - temp_milk;

            //общий Коэффицент
            #define K (1.5)
            // Коэффицент усиления t1
            #define K1 (3)
            // Коэффицент усиления t2
            #define K2 (0.5)

            result = K * ( (t1 * K1) - (t2 * K2) );
            result = RANGE(result, 0 , 10);
            result = map(result,0,10,0,PID_COUNT_POWER_PERIOD);

            
            
  }  

  

  return result >= count;
};


//Функция выбора нужного режима 
void C_Cooking :: changeMode(){
  
  //Оповещение о выполнении
  switch (_mode)
  {
  //Done Heat
  case 1:
    serialSend("DH");
    break;
  
  //Done Delay
  case 2:
    serialSend("DD");
    break;

  //Done Cooling
  case 3:
    serialSend("DC");
    break;
  }

  //переход на новый режим
  switch (_mode)
  {   
    case 0:
      if      (b_auto_heat)     {_mode = 1; return;};
    case 1:
      if      (b_auto_delay)    {_mode = 2; time_delay = millis() + (need_delay_heat * 1000); return;};
    case 2:
      if      (b_auto_cooling)  {_mode = 3; return;};
    case 3:
    case 4:
      //Done All
      serialSend("DA");
      
      disableAll();

    break;

    default : _mode = 4;
  }
};


