/*
  В этом файле описаны функции для работы с датчиками и для приготовления
*/


#ifndef _ANDROID_CHESSE_COOCKINH_H_
#define _ANDROID_CHESSE_COOCKINH_H_

#include "USB.h"
#include "Settings.h"
#include "Mixer.h"
#include "Temp.h"

//Глобальные переменные
// пины ТЭНов в массиве для удобного использования 
extern uint8_t pin_ten[3];
// Массив активных ТЭНов
extern bool btn_active_ten[3];

//Ошибка воды в рубашке (true если воды нет,  false - вода в рубашке есть)
extern bool b_waterError;
//Нажата ли красная кнопка
extern bool b_redBtnPress;

/////////////////
extern float result;

// проверка уровня вводы, экстренной кнопки и кнопок ТЭНов.
void parametrsCheck();


class C_Cooking {
  public:
        /*  Текущий режим работы автоприготовления
        0 - стартовая  
        1 - нагрев
        2 - выдержка
        3 - охлаждение
        4 - приготовлено
    */
    int8_t _mode = 0;

    
    //Включена ли атоматическая готовка готовка
    bool b_autoCooking = false;
    
    //Включен ли ТЭН для ручного режима
    bool b_ten = false;
    //Открыт ли клапан для ручного режима
    bool b_klapan = false;
    //Включен ли нагрев
    bool b_auto_heat = false;
    //Включена ли выдержка
    bool b_auto_delay = false;
    //Включено ли охлаждение
    bool b_auto_cooling = false;
    //Температура до которой нужно нагревать молоко
    float need_temp_heat = 0;
    //Время выдержки в секундах 
    uint64_t need_delay_heat = 0;
    //Теспература до котрой нужно охладить молоко
    float need_temp_cooling = 0;

    // следующий режим автоприготовления
    void changeMode();

    // (true) Начать готовку в атоматическом режиме / (false) или выключить авто режим
    void setAutoCooking(bool f);
    
    //Включить / выключить ТЭН в ручном режиме
    void setTen(bool f);
    //Включить / выключить Клапан в ручном режиме
    void setKlapan(bool f);

    // Включить / выключить нагрев в автоматическом режиме готовки
    void setHeatActive(bool f);
    // Включить / выключить выдержку в [СЕК] в автоматическом режиме готовки
    void setDelayActive(bool f);
    // Включить / выключить охлаждение в автоматическом режиме готовки
    void setCoolingActive(bool f);
    
    // Установить температуру нагрева в автоматическом режиме
    void setHeatTemp(float temp);
    // Установить время выдержки в с в автоматическом режиме
    void setDelayTime(uint64_t time);
    // Установить температуру охлаждения в автоматическом режиме
    void setCoolingTemp(float temp);

    //Отключить все системы
    void disableAll();

    //Проверка всех систем
    void tick();

  private: 

    
    //переменная для отслеживания времени во время выдержки
    uint64_t time_delay;

    //регулятор плавного нагрева
    bool PIDRegulator();

    //работа ТЭНа
    void tenWork(bool f);
    //работа Клапана
    void klapanWork(bool f);

};

extern C_Cooking Cooking;




#endif