#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
const char *ssid = "kdkdkk";
const char *password = "12345678";
ESP8266WebServer server(80);


void handleSentVar() {
  if (server.hasArg("sensor_reading")) { // this is the variable sent from the client
  Serial.println( "data="+String(server.hasArg("sensor_reading")));
    String readingInt = server.arg("sensor_reading");
   Serial.println(readingInt);
    server.send(200, "text/html", "Data received");
  }
}

void setup() {
  Serial.begin(115200);
   WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  // put your setup code here, to run once:
   server.on("/data/", HTTP_GET, handleSentVar); // when the server receives a request with /data/ in the string then run the handleSentVar function
  server.begin();

}

void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();

}
