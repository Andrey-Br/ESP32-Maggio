#ifndef _MIXER_ESP32_
#define _MIXER_ESP32_

#include "Settings.h"
#include "Functions.h"

#define MIXER_DELTA_SPEED 4                   //  Измениние скорости на это значение
#define MIXER_DELTA_TIME 100                  //  Время скорости опроса
#define MIXER_PERIOD_PAUSE_AUTOREVERSE  700   //  Время остановки между сменой направления вращения
#define MIXER_START_SPEED 80                  //  Начальная скважность ШИМ сигнала

class C_Mixer{
  private:    
    bool _need_dir = false;                  // Требуемое направление
    bool _current_dir;                       // текущее направление
    bool _mixer_work = false;                // Включен ли Миксер

    /* 
    / Режимы  
    / 0: набор скорости
    / 1: удержание 
    / 2: сбрасывание скорости перед изменением направления 
    / 3: пауза
    */
    uint8_t _mode = 0;                     
    
    int8_t _current_speed = 0;                // Текущая скорость в % (100)
    int8_t _need_speed = 100;                 // Нужная скорость в %  (100)
    uint64_t _time_tick = 0;                  // Переменная для задания периода изменения скорости
    uint64_t _time_autoreverse;               // Время когда меняем направление
    uint64_t _period_autoreverse = 0;         // Время изменения направлния при автореверсе, "0" если не пользуемся автореверсом  

    // запустить миксер скоросью speed в сторону rev (прямое воздействие, без плавного изменения) 
    void work ( int8_t speed, bool rev = false );


  public:

    //проверяет надо ли останавливаться миксеру
    bool isStop();  

    // перерасчет скорости и напраления, обновляет время автореверса
    void reculc();
  
    //Остановить миксер
    void stop();

    //устновить направление (true, false)
    void setDir(bool direction);

    void setSpeed(int8_t procent);

    void setTimeAutoReverse(uint64_t time);

    void changeDir();

    void start();

    // Запуск миксера  (скорость(0..100), автореверс в мс (0 если выключен))
    void start(int8_t sp, uint64_t time = 0);

    void tick();
};

extern C_Mixer Mixer;

#endif