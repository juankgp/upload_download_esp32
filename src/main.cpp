#include <Arduino.h>
#include "UDHttps.h"
#include "mySD.h"
#include <WiFi.h>
#include <WiFiMulti.h>
#include <WiFiClientSecure.h>
#include <WebSocketsClient.h>
#include <Streaming.h>
#include "wsplit.h"
unsigned long startMillis;  //some global variables available anywhere in the program
unsigned long currentMillis;
#define USE_SERIAL Serial
const char* ssid     = "EcuDomo";
const char* password = "ecudomo2016";
WebSocketsClient webSocket;
WiFiClientSecure client;
//char * archivo = "arrows.jpg";//nombre del archivo a bajar
String command = "";
File root;
WiFiMulti WiFiMulti;
//write data callback
int wdataf(uint8_t *buffer, int len){
  //write downloaded data to file
  return root.write(buffer, len);
}

void progressf(int percent){
  Serial.printf("%d\n", percent);
}
void descarga(char * file){
  SD.remove(file);
  {
    UDHttp udh;
    //open file on sdcard to write
    root = SD.open(file, FILE_WRITE);
    if (!root) {
       Serial.println("can not open file!");
       return;
    }
    //download the file from url
    //char newurl[30] = "http://144.202.102.219:3005/";

    char newurl[35] = "https://144.202.102.219:5000/&";
    //char newurl[30] = "http://192.168.100.217:5000/&";
    strcat(newurl,file);
    delay(1000);
    //udh.download("http://192.168.100.217:5000/&test.jpg", wdataf, progressf);
    //udh.download("http://144.202.102.219:3004/", wdataf, progressf);

   Serial.println(newurl);
    udh.download(newurl, wdataf, progressf);
    root.close();
    Serial.printf("done downloading\n");
  }
}
String get_comando(String data){
	string sdata(data.c_str());
	vector<string> datos = wsp::wsplit(sdata,'&');
	//int cut = data.indexOf('&');
	//data = data.substring(cut+1);
	for (size_t i = 0; i < datos.size(); i++)
	{
		USE_SERIAL << "******** "<< datos[i].c_str() << " *********" << endl;
	}
	data = datos[1].c_str();
  char *file = &data[0];
  //strcpy(file, datos[1].c_str());

  String eval = datos[2].c_str();

  if(eval=="borrar"){
    USE_SERIAL << "////////borrar archivo///////"<< endl;
    SD.remove(file);
  }else{
      descarga(file);
  }

	return data;
}

void hexdump(const void *mem, uint32_t len, uint8_t cols = 16) {
	const uint8_t* src = (const uint8_t*) mem;
	USE_SERIAL.printf("\n[HEXDUMP] Address: 0x%08X len: 0x%X (%d)", (ptrdiff_t)src, len, len);
	for(uint32_t i = 0; i < len; i++) {
		if(i % cols == 0) {
			USE_SERIAL.printf("\n[0x%08X] 0x%08X: ", (ptrdiff_t)src, i);
		}
		USE_SERIAL.printf("%02X ", *src);
		src++;
	}
	USE_SERIAL.printf("\n");
}

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
	String data = "";
	switch(type) {

		case WStype_DISCONNECTED:
			USE_SERIAL.printf("[WSc] Disconnected!\n");

			break;
		case WStype_CONNECTED:
			 USE_SERIAL.printf("[WSc] Connected to url: %s\n", payload);

			// send message to server when Connected
			webSocket.sendTXT("ConectadoEQ");
			break;
		case WStype_TEXT:

			USE_SERIAL.printf("[WSc] get text: %s\n", payload);
			for (size_t i = 0; i < length; i++)
				{
					data+=char(payload[i]);
				}
			USE_SERIAL << "Llega " << data << endl;
			//String comando = get_comando(data);
			//USE_SERIAL << "COMANDO " << get_comando(data) << endl;
			command = get_comando(data);

			break;
		case WStype_BIN:
			//String data = "";
			USE_SERIAL.printf("[WSc] get binary length: %u\n", length);
      		USE_SERIAL.printf("[WSc] get text: %s\n", payload);
				for (size_t i = 0; i < length; i++)
				{
					data+=char(payload[i]);
				}
			USE_SERIAL << "Llega " << data << endl;
			data = "";

			hexdump(payload, length);

			// send data to server
			// webSocket.sendBIN(payload, length);
			break;
		case WStype_ERROR:
		case WStype_FRAGMENT_TEXT_START:
		case WStype_FRAGMENT_BIN_START:
		case WStype_FRAGMENT:
		case WStype_FRAGMENT_FIN:
			break;
      case WStype_PING:
             // pong will be send automatically
             USE_SERIAL.printf("[WSc] get ping\n");
             break;
         case WStype_PONG:
             // answer to a ping we send
             USE_SERIAL.printf("[WSc] get pong\n");
             break;
	}

}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  WiFiMulti.addAP("EcuDomo", "ecudomo2016");

  WiFiMulti.addAP("VIDORTEC S.A.", "Vidortec2017");

  //WiFi.disconnect();
  while(WiFiMulti.run() != WL_CONNECTED) {
    delay(100);
  }
  USE_SERIAL << "conectado wifi" << endl;
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.print("Initializing SD card...");
  if (!SD.begin(5)) {
    Serial.println("initialization failed!");
    return;
  }

  Serial.println("initialization done.");
  //Serial.println(archivo);


USE_SERIAL << "WebSocket" << endl;
  webSocket.begin("144.202.102.219", 3001, "/ws?LETREROSTAXI");
  //webSocket.begin("192.168.100.217", 3000, "/ws?MIRAVALLE");


  // event handler
  webSocket.onEvent(webSocketEvent);

  // use HTTP Basic Authorization this is optional remove if not needed


  // try ever 5000 again if connection has failed
  webSocket.setReconnectInterval(5000);
  webSocket.enableHeartbeat(15000, 3000, 2);


}

void loop() {
  webSocket.loop();
  //delay(6000);

  if (1)
	{
		currentMillis = millis();  //get the current "time" (actually the number of milliseconds since the program started)
		if (currentMillis - startMillis >= 100000)  //test whether the period has elapsed
		{
			//digitalWrite(sirena, !digitalRead(sirena));  //if so, change the state of the LED.  Uses a neat trick to change the state
      USE_SERIAL << "seis segundos" << endl;
			startMillis = currentMillis;  //IMPORTANT to save the start time of the current LED state.
		}
	}

}
