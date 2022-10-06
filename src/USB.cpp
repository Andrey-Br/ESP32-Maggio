#include "USB.h"


//макрос для преобразовывания символов в число (нужен для switch)
#define S(x) ((uint16_t)( ((uint8_t)x[0]<<8) + ((uint8_t)x[1]) ) )  


void serialSend(String s1, String s2){
  Serial.println(s1+s2);
}

//Вывести информацию о датчиках и кнопках
void printInfo (){
  String message;
  message += stringTemp();
  message += SPLIT_CHAR;
  message += b_waterError ? "WE" : "WF";
  message += SPLIT_CHAR;
  message += b_redBtnPress ? "RP" : "RR";
  message += SPLIT_CHAR;  

  for (uint8_t i = 0; i < 3; i++ ){
    message +='T';
    message += i;
    message += btn_active_ten[i] ? '1' : '0';
    message += SPLIT_CHAR;
  }


  serialSend(message);


}

//обработка строчки,как одной команды
void command (String &s){
  char command[3];
  s.toCharArray(command,3);
  String param = s.substring(2);
  uint16_t c = S(command);

  switch (c){
    // Mixer Freeze
    case S("MF"):
      Serial.println("Mixer Freeze");
      Mixer.stop();
    break;

    // Mixer Speed **
    case S("MS"):
      {
      int8_t p = (int8_t)RANGE( param.toInt() , 0 , 100);
      Mixer.setSpeed(p);
      
      serialSend("Mixer Speed set ", String(p));
      }
    break;
    
    //Mixer Run (start)
    case S("MR"):
      serialSend("Mixer Start");
      Mixer.start();
    break;

    //Mixer Change direction
    case S("MC"):
      serialSend("Mixer Change direction");
      Mixer.changeDir();
    break;
    
    //Mixer Direction *
    case S("MD"):
    {
      bool p = (bool)param.toInt();
      serialSend("Mixer Direction Set: ", String (p));

      Mixer.setDir(p);
    }
    break;
    
    //Mixer time AutoReverse ****
    case S("MA"):
    {
      uint32_t t = (uint32_t) param.toInt();
      Mixer.setTimeAutoReverse(t);
      serialSend("Mixer Autoreverse time = ", String( (uint32_t) t ));
    }
    break;

    //Ten On
    case S("T+"):
      serialSend("Ten ON");
      
      Cooking.setAutoCooking(false);
      Cooking.setTen(true);
    break;

    //Ten Off
    case S("T-"):
      serialSend("Ten OFF");
      Cooking.setAutoCooking(false);
      Cooking.setTen(false);
    break;

    //Open Klapan
    case S("KO"):
      serialSend("Klapan open");
      Cooking.setAutoCooking(false);
      Cooking.setKlapan(true);
    break;

    //Close Klapan
    case S("KC"):
      serialSend("Klapan close");
      Cooking.setAutoCooking(false);
      Cooking.setKlapan(false);
    break;
    
    //Heat Set  (Auto)
    case S("HS"):
    {
      float t = (float) ( (float)param.toInt() / 10 );
      Cooking.setHeatTemp(t);

      serialSend("Auto heat Set :", String(t));
    }
    break;

        //Timer Set  (Auto)
    case S("TS"):
    {
      uint32_t t = (uint32_t) ( param.toInt() );
      Cooking.setDelayTime(t);

      serialSend("Auto delay heat To :", String(t));
    }
    break;

    //KT****	Klapan Set to Temp
    case S("KS"):
    {
      float t = (float) ( (float) param.toInt() / 10 );
      Cooking.setCoolingTemp(t);

      serialSend("Cooling Set :", String(t));
    }
    break;

    //Recipe Cook
    case S("RC"):
      serialSend("Recipe Cook start");
      Cooking.setAutoCooking(true);
    break;

    //Recipe Stop
    case S("RS"):
      serialSend("Recipe Cook stop");
      Cooking.setAutoCooking(false);
    break;

    //Calibrate MILK HIGH
    case S("S1"):{
      float t = (float) ( (float)param.toInt() / 10 );
      setCalibrate(CalibrateMilk[1],temp_milk_non_calibrate, t);
      serialSend("CalibrateMilk HIGH in :", String( t ) );
    }
    break;

    //Calibrate MILK LOW
    case S("S2"):{
      float t = (float) ( (float)param.toInt() / 10 );
      setCalibrate(CalibrateMilk[0], temp_milk_non_calibrate, t);
      serialSend("Calibrate Milk LOW in :", String( t ) );
    }
    break;

    //Calibrate WATER HIGH
    case S("S3"):{
      float t = (float) ( (float)param.toInt() / 10 );
      setCalibrate(CalibrateWater[1], temp_water_non_calibrate, t);
      serialSend("Calibrate Water HIGH in :", String( t ) );
    }
    break;

    //Calibrate WATER LOW
    case S("S4"):{
      float t = (float) ( (float)param.toInt() / 10 );
      setCalibrate(CalibrateWater[0], temp_water_non_calibrate, t);
      serialSend("Calibrate Water LOW in :", String( t ) );
    }
    break;

    // Sensors Reset
    case S("SR"):
      resetCalibrate();
      serialSend("Calibrate Sensors Reset");
    break;


    case S("??"):
      printInfo();
    break;




  };
};

void checkSerial(){
  if (Serial.available() == 0 ) return;
  String data;
  while (Serial.available() > 0)
  {
    char c = Serial.read();
    if (c != '\n') data += c;
    // чтобы не пропустить символы, без него были ложные деления на подстроки
    delayMicroseconds(100);
  }
    serialSend(">>", data);

    int8_t search;
    String sup_s;
    // Serial_DEBUG(data);
  do {
    search = data.indexOf(SPLIT_CHAR);
    if (search == -1 ) sup_s = data;
    else sup_s = data.substring(0,search);
    command (sup_s);
    data = data.substring(search+1);
    
  } while (search > -1);
}
