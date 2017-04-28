#include <WiFiManager.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

MDNSResponder mdns;

ESP8266WebServer server(80);

String webPage = "";

void setup(void){
  WiFiManager wifiManager;
  wifiManager.autoConnect("feedcatfood", "meow");
  
  webPage += "<script src=\"//rawgit.com/ryanknutson/feedcatfood/master/html/page.js\"></script>";
  
  delay(1000);
  Serial.begin(115200);

  if (mdns.begin("esp8266", WiFi.localIP())) {
    Serial.println("MDNS responder started");
  }
  
  server.on("/", [](){
    server.send(200, "text/html", webPage);
  });
  
  server.on("/feed", [](){
    server.send(200, "text/html", webPage);

    //feeding commands go here

    delay(1000);
  });

  server.begin();
  Serial.println("HTTP server started on");
  Serial.println(WiFi.localIP());
}
 
void loop(void){
  server.handleClient();
} 
