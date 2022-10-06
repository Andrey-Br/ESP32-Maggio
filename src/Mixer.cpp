#include "Mixer.h"

C_Mixer Mixer;

bool C_Mixer::isStop(){
  return !_mixer_work || b_redBtnPress;
}

void C_Mixer::work ( int8_t speed, bool rev){

  if (speed  > -2 && speed  < 2) {
  ledcWrite(LEDC_MIXER1,0);
  ledcWrite(LEDC_MIXER2,0);
  }

  else if (! rev) {
    ledcWrite(LEDC_MIXER2,0); 
    ledcWrite(LEDC_MIXER1,map(speed,0,100,MIXER_START_SPEED,255));
  } else {
    ledcWrite(LEDC_MIXER1,0);
    ledcWrite(LEDC_MIXER2,map(speed,0,100,MIXER_START_SPEED,255)); 
  }
};

// функция проверяет в каком режиме должен быть миксер
void C_Mixer::reculc(){
  if ( !Mixer.isStop() ) {
    if (_current_speed == 0) {
      _mode = 0;
      _current_dir = _need_dir;
    } else _mode = (_need_dir == _current_dir) ? 0 : 2;
  }
  else _mode = 2;
};

//Остановить миксер
void C_Mixer::stop(){
  // Serial.println("MIXER: Stop");
  _mixer_work = false;
  reculc();
};
    
//устновить направление (true, false)
void C_Mixer::setDir(bool direction){
  _need_dir = direction;
  // Serial.print("MIXER: Set direction : ");
  // Serial.println(direction ? "True" : "False");

  reculc();
};

void C_Mixer::setSpeed(int8_t procent){
  if (procent == 0) stop();
  else {
    _need_speed = procent;
    start();
  }
};

  

void C_Mixer::setTimeAutoReverse(uint64_t time){
  _period_autoreverse = time;
  // Serial.print("MIXER: Time autoreverse = ");
  // Serial.print(time);
  // Serial.println(" ms");
  reculc();
};

void C_Mixer::changeDir(){
  _need_dir = ! _need_dir;
  // Serial.println("MIXER: User change direction");
  reculc();
};

void C_Mixer::start(){
  _mixer_work = true;
  reculc();
  // Serial.println("Mixer Start");
};

// Запуск миксера  (скорость(0..100), автореверс в мс (0 если выключен))
void C_Mixer::start(int8_t sp, uint64_t time){ 
  _period_autoreverse = time;
  setSpeed(sp);
};

void C_Mixer::tick(){
  if ( _time_tick > millis()) return;
  _time_tick = millis() + MIXER_DELTA_TIME;
  
  // Если миксер выключен, а скорость есть : гасим
  if ((_mixer_work == false) && (_current_speed > 0)) _mode = 2;
  
  // Если миксер выключен и стоит -> выходим из функции
  if ((_mixer_work == false) && (_current_speed == 0)) return;

  switch (_mode) {
    case 0:
      if (_current_speed == _need_speed) {
        _mode = 1;
        _time_autoreverse = millis() + _period_autoreverse; 
      } 
      else 
      {
        if (_current_speed < _need_speed ) {
          _current_speed += MIXER_DELTA_SPEED;
          if (_current_speed > _need_speed)  _current_speed = _need_speed;
        } else {
          _current_speed -= MIXER_DELTA_SPEED;
          if (_current_speed < _need_speed)  _current_speed = _need_speed;
        }
        work(_current_speed, _current_dir);
      };
    break;

    case 1:
      // если включен автореверс и пришло время менять направление
      if ((_period_autoreverse != 0) && (_time_autoreverse <= millis()) ) 
      {
        _mode = 2;
        _need_dir = ! _need_dir;
      }
    break;

    // плавноая остановка перед сменой направления
    case 2:
      
      // Если следующий тик последний
      if (_current_speed <= MIXER_DELTA_SPEED) {
        _current_speed = 0;
        _time_autoreverse = millis() + MIXER_PERIOD_PAUSE_AUTOREVERSE;
        _mode = 3;
      } 
      else _current_speed -= MIXER_DELTA_SPEED;
      work(_current_speed,_current_dir);
    break;

    // Пауза между сменой направления,если mixer_work false тут и будет стопориться 
    case 3:
      if ( _time_autoreverse <= millis() && !Mixer.isStop() ) {
        // смотрим итоговый реверс
        _current_dir = _need_dir;
        
        // в режим набирания скорости
        _mode = 0;
      };
    break;
  }
};
