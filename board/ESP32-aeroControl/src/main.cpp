#include <Arduino.h>
#include <WiFiUdp.h>
#include <WiFi.h>
#include <aero_servo.h>
#include <aero_gyro.h>

WiFiUDP udp;
int port = 12000;
int i = 0;
bool connected = false;

int motor_pin = GPIO_NUM_4;
int motor_chn = 2;
int throttle = 0;

void setup() {
  setup_servos();
  setup_gyro();

  pinMode(motor_pin, OUTPUT_OPEN_DRAIN);
  ledcAttachPin(motor_pin, motor_chn);
  ledcSetup(motor_chn, 30000, 8);

  Serial.begin(9600); //Inicializando a serial
  int wStatus = WL_DISCONNECTED;
  int limit = 60; //tempo de espera para conexão
  WiFi.begin("NET_2GDC570B", "4BDC570B");//propriedades da rede a conectar
  //WiFi.begin("joaoemarcella", "baixinhalinda");//propriedades da rede a conectar
  while (wStatus != WL_CONNECTED &&
         wStatus != WL_CONNECT_FAILED) //Aguardando conexão ou falha
  {
    Serial.println("Tentando conexão...");
    delay(1000);
    wStatus = WiFi.status();
    if (limit-- <= 0)
    {
      wStatus = WL_CONNECT_FAILED; //Se o tempo de espera for excedido
      break;                       //Damos a conexão como falha
    }
  }

  if (wStatus == WL_CONNECTED) //Se a conexão for bem-sucedida
  {
    String ip = WiFi.localIP().toString(); //Mostramos o IP
    String ipStr = "Connected to the WiFi network: " + ip;
    Serial.println(ipStr);
    int ret = udp.begin(port);
    Serial.println(ret);
    //IPAddress addr = IPAddress(INADDR_ANY);
    //int ret = udp.beginMulticast(addr, port);
    //Serial.println(ret);
  }
}

// https://stackoverflow.com/questions/9072320/split-string-into-string-array
String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}

boolean sendUDP(String string) {
  int ret = udp.beginPacket(INADDR_BROADCAST, port);
  udp.println(string);
  udp.endPacket();
  udp.flush();
  return ret != 0;
}

void parse_packet(String msg){
  char buf[msg.length()];
  msg.toCharArray(buf, sizeof(buf));
  
  String msgX = getValue(msg, '|', 0);
  String msgY = getValue(msg, '|', 1);

  int inputX = (msgX.toFloat()*100000.0);
  int angleX = (90 + (90*inputX/100000));
  
  int inputY = (msgY.toFloat()*100000.0);
  int angleY = 90*inputY/100000;

  set_angle(0, angleX + angleY);
  set_angle(1, angleX - angleY);

  String msgT = getValue(msg, '|', 3);
  int inputT = msgT.toFloat()*100;
  throttle -= inputT*20/100;
  if(throttle >= 255){//Se valor passa do limite de 255, voltamos para zero  
    throttle = 255;  
  }else if(throttle <= 0){
    throttle = 0;  
  }
  ledcWrite(motor_chn, throttle);//Escrevemos o valor no canal configurado
}

void loop() {
  if(!connected){
    connected = sendUDP("teste");
    Serial.println("connected: " + connected);
  }else{
    int packetSize = udp.parsePacket();
    if (packetSize) {
      String msg = udp.readStringUntil('\n');
      parse_packet(msg);
      int *result = readRawMPU();
      char response[64];
      sprintf(response, "%d|%d|%d|%d|%d|%d", 
        result[0], result[1], result[2], result[3], throttle, WiFi.RSSI());
      sendUDP(String(response));
    }
  }
}