
#include "CTBot.h"

#define SENSOR_PIN D1
#define LED_BUILTIN D2

CTBot myBot;

String pa = "*EL PORTON ESTA ABIERTO*";
boolean activo = true;
String pc = "<- El porton esta cerrado ->";
String ssid = "";
String pass = "";
String token = "";
long id = ; //chat_id sin comillas

int BanderaSensor = 0;
volatile bool BanderaPortonAbierto = false;
int milisanteriores = 0;

void setup(){

  Serial.begin(9600);
  pinMode(SENSOR_PIN, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);

  //attachInterrupt(SENSOR_PIN, Movimiento,CHANGE);
  attachInterrupt(digitalPinToInterrupt(SENSOR_PIN), movimiento, CHANGE);

  myBot.wifiConnect(ssid, pass);

  myBot.setTelegramToken(token);

  if (myBot.testConnection()) {
    Serial.println("Conectado a la Red Wifi");
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    Serial.println("Error al Conectar a la Red");
    digitalWrite(LED_BUILTIN, LOW);
  }

} // FIN SETUP

void loop(){

  TBMessage msg;
    
  if (BanderaSensor == 1){

    int actual = millis() / 1000;

    if (actual >= milisanteriores){
      if(activo == true){
        envioMensaje();  
        }
      }
    
  }

  if (myBot.getNewMessage(msg)) {

    Serial.print("Mensaje recibido -> ");
    Serial.println(msg.text);
    
    if (msg.messageType == CTBotMessageText){
      
      if (msg.text.equalsIgnoreCase("estadoPorton")){
          envioEstadoPorton();
      }
      
      if (msg.text.equalsIgnoreCase("cambiarEstado")){

          if(activo == true){
             activo = false;
             myBot.sendMessage(id, " ** Mensajes desactivados ** ");
           }else{
             activo = true;
             myBot.sendMessage(id, " ** Mensajes activados ** ");
          }
           
      }
      
    }


  }

} //FIN LOOP

void movimiento(){
  int sensor = digitalRead(SENSOR_PIN);
  if (sensor == HIGH && BanderaSensor == 0){

    BanderaPortonAbierto = true;
    Serial.println("Porton Abierto");
    Serial.print("millis : ");
    Serial.println(milisanteriores);
    BanderaSensor = 1;
    milisanteriores = millis() / 1000 + 180;
    delay(1500);

  }
  if (sensor == HIGH) {

    BanderaPortonAbierto = true;
    BanderaSensor = 1;

  }
  else {
    BanderaPortonAbierto = false;
    Serial.println("Porton Cerrado");
    BanderaSensor = 0;
    milisanteriores = 0;
  }
  return;
} // FIN MOVIMIENTO

void envioMensaje(){

  Serial.println("Enviando Mensaje.....");
  BanderaPortonAbierto = false;
  myBot.sendMessage(id, "!!! PORTON ABIERTO !!!");
  Serial.println("Mensaje Enviado");
  Serial.println(" ");
  BanderaSensor = 0;
  movimiento();

} // FIN ENVIO MENSAJE

void envioEstadoPorton(){

  int sensor = digitalRead(SENSOR_PIN);
  Serial.print("Respondiendo el porton esta  -> ");
  if(sensor == HIGH){  myBot.sendMessage(id, pa); Serial.print(" Abierto "); } else { myBot.sendMessage(id, pc); Serial.print(" Cerrado ");}
  Serial.println(" ");
  Serial.println("Mensaje Enviado");
  Serial.println(" ");
  return;
} // FIN ENVIO ESTADO PORTON
