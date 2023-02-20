#include <SPI.h>
#include <LoRa.h>


#define INIT_ST 0
#define WAITING_ST  1
#define RECIVING_ST 2
#define SEND_ST 3
#define BEGIN_ST 4


#define INIT_COMMAND 0
#define BEGIN_COMMAND 1
#define SEND_COMMAND 2
#define NO_COMMAND 3
#define MAX_COMMAND NO_COMMAND 

int state; //глобальная переменная, хранящая код состояния 

#define INIT_TIMEOUT 5000
//определяем номера пинов, используемые трансивером
#define ss 5
#define rst 14
#define dio0 2
 
int sec = 0;
int temp=0;
int pres=0;

void waiting(){
  int comd =getComd();
  switch(comd){
    case NO_COMMAND:
    break;
    case BEGIN_COMMAND:
    state=BEGIN_ST;
    break;
    case INIT_COMMAND:
    state=INIT_ST;
    break;
    case SEND_COMMAND:
    state=SEND_ST;
  }
  state=WAITING_ST;
}
void INIT(){
  LoRa.setPins(ss, rst, dio0);
  while (!LoRa.begin(866E6)) {
    Serial.println(".");
    delay(500);
  }
  LoRa.setSyncWord(0xF3);
}
int getComd(){
  int comd=NO_COMMAND;
 
    comd=LoRa.read();
    if (comd>MAX_COMMAND){
      comd=NO_COMMAND;
    }
    else{
      return comd;
    }
}
void BEGIN(){
    while (!LoRa.begin(866E6)) {
    Serial.println(".");
    delay(500);
  }
  LoRa.setSyncWord(0xF3);
}
void setup() {
  //запускаем монитор порта
  Serial.begin(115200);
 
  //настраиваем трансивер
}
void SEND(){
  for i in range(20){
    LoRa.beginPacket();
  LoRa.print(sec,temp,pres);
  LoRa.endPacket();
 
  sec++;
 
  delay(1000);
  }

 
void loop() {
 switch(state){
  case INIT_ST:
  INIT();
  break;
  case WAITING_ST:
  waiting();
  break;
  case BEGIN_ST:
  BEGIN();
  break;
  case SEND_ST:
  SEND();
  break;
  case RECIVING_ST:
  recive();
  break;
 }
 state=WAITING_ST;
}
