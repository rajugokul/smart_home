#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>


 ESP8266WebServer server(80); //server variable
 String light1 = "0";
 char buf[20];

 enum connection {
  ofline=0, // offline-server_mode_enable
  online=1, // blink enable
};
 
enum connection con;

short int option= 0;
void check();

//-----------------------------------chek option---------------------------//

// data == 1--offline &data == 0--online
void check(){
  //------------------------eeprom--ssid & psd=--------------------------
  for(int a=0;a<=10;a++){
    
    Serial.println(char(EEPROM.read(a)));
  }
  
   Serial.println(EEPROM.read(50));
   if(EEPROM.read(50)==0 ||EEPROM.read(50)==1){
    Serial.println("mode_ok..");  
   }
   else
      EEPROM.write(50,ofline);
      
   Serial.println(EEPROM.read(50));
   (int(EEPROM.read(5))==1)?Serial.println("online_mode"):Serial.println("offline_mode");
  
}
//------------------------------------select option--------------------------/

void select(){
  
  if(EEPROM.read(50)==online){
    Serial.println("ur_in_online_mode");
    //online_mode
    WiFi.softAPdisconnect(1);
     EEPROM.write(50,ofline);
    
  }
  
  else{
    
    String ssid = "loop-"+(WiFi.macAddress()); //Put your Wi-Fi ssdid here
    char* password = "123456789"; //Your Wi-Fi Password
   
      WiFi.softAP(ssid, password);
      IPAddress myIP = WiFi.softAPIP();
      Serial.println(myIP);
      server.on("/data/", HTTP_GET, readOflineData); // when the server receives a request with /data/ in the string then run the handleSentVar function
      server.on("/light1",light1_state);

      server.begin();
      EEPROM.write(50,online);


  }
}





//------------------------------------main_function------------------------//
void setup() {
   pinMode(D0, OUTPUT);
   digitalWrite(D0, LOW);
  Serial.begin(115200);
  EEPROM.begin(512);
  check();
  Serial.print("fst_eeprom data=");
  Serial.println(EEPROM.read(50));
  select();
  EEPROM.commit();
  
}

void loop() {

 server.handleClient();

}

//-------------------------------------------------server fun-------------------
///----------------------------------------- read ssid & password-------------------------
 
void readOflineData(){
  if (server.hasArg("sensor_reading")) { // this is the variable sent from the client
    String readingInt = server.arg("sensor_reading");
   Serial.println(readingInt);
   
   readingInt.toCharArray(buf, 15);
   
   for(int i=0;buf[i]!='\0';i++){
    Serial.println("data="+String(buf[i]));
    EEPROM.write(i,buf[i]);
    
   }
    EEPROM.commit();
    server.send(200, "text/html", "Data received");

}
}

//---------------------------------------------------------------------------------------







void readdata(){
  Serial.println("read_data_ok");
}

void light1_state(){
  if(light1 == "0"){
    light1 = "1";
    digitalWrite(D0, HIGH);
    server.send(200, "text/html", light1);
    Serial.println("okkkkkkkkkkkkkkkkkkk");
  }else{
    light1 = "0";
    digitalWrite(D0, LOW);
    server.send(200, "text/html", light1);
  }
}
