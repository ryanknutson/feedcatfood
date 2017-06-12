#include <WiFiManager.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Servo.h>
Servo servo1;
#define servo1Pin D7

LiquidCrystal_I2C lcd(0x27, 16, 2);
uint8_t heart[8] = {0x0, 0xa, 0x1f, 0x1f, 0xe, 0x4, 0x0};

MDNSResponder mdns;

ESP8266WebServer server(80);

String webPage = "";

void setup(void){
  
  lcd.createChar(3, heart);
  Wire.begin(D2, D1);
  lcd.begin();
  lcd.home();

  WiFiManager wifiManager;
  wifiManager.autoConnect("feedcatfood", "meow");


  lcd.write(3);
  lcd.print("feedcatfood");
  lcd.write(3);

  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP());


  
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

    servo1.attach(servo1Pin);
    servo1.write(0);
    
    //feeding commands go here
    lcd.backlight(); // turn on backlight
    lcd.clear();
    lcd.print("Feeding...");
    Serial.println("Feeding...");
    
    delay(10000);

    servo1.detach();
    lcd.clear();
    lcd.noBacklight(); // turn off backlight
    Serial.println("Done feeding");
  });

  server.begin();
  Serial.println("HTTP server started on");
  Serial.println(WiFi.localIP());
}
 
void loop(void){
  server.handleClient();
} 
