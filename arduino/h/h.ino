#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <ESP8266WiFi.h>          //ESP8266 Core WiFi Library (you most likely already have this in your sketch)
#include <DNSServer.h>            //Local DNS Server used for redirecting all requests to the configuration portal
#include <ESP8266WebServer.h>     //Local WebServer used to serve the configuration portal
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager WiFi Configuration Magic
#include <Wire.h>
#include <Servo.h>
Servo servo1;
#define servo1Pin D7
#include <NTPClient.h>
#include <Time.h>
#include <TimeAlarms.h>

WiFiUDP ntpUDP;

// By default 'time.nist.gov' is used with 60 seconds update interval and
// no offset
NTPClient timeClient(ntpUDP);

// You can specify the time server pool and the offset, (in seconds)
// additionaly you can specify the update interval (in milliseconds).
// NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600, 60000);




LiquidCrystal_I2C lcd(0x27, 16, 2);
uint8_t heart[8] = {0x0, 0xa, 0x1f, 0x1f, 0xe, 0x4, 0x0};
String webPage = "<script src=\"//rawgit.com/ryanknutson/feedcatfood/master/html/page.js\"></script>";
ESP8266WebServer server(80);

void feed() {
  timeClient.update();
  
  lcd.clear();
  lcd.backlight();
  lcd.print("Feeding...");
  
  
  server.send(200, "text/html", "<script>document.location.href=\"/success\";</script>");
  
  
  servo1.attach(servo1Pin);
  servo1.write(0);

  Alarm.delay(5000); // feed time

  setTime(timeClient.getHours(),timeClient.getMinutes(),timeClient.getSeconds(),1,1,18);

  servo1.detach();

  Alarm.delay(1000);

  lcd.noBacklight();

  lcd.clear();
  //lcd.print(timeClient.getFormattedTime());

  lcd.setCursor(0,1);
  lcd.print(WiFi.localIP());
  return;
}


void setup() {
  //pinMode(2, OUTPUT); led onboard
  pinMode(D6, INPUT_PULLUP);

  pinMode(D5, INPUT_PULLUP);

  Serial.begin(115200);

  Wire.begin(D2, D1);
  
  lcd.begin();
  
  lcd.home();

  lcd.clear();

  lcd.print("Join wifi");
  lcd.setCursor(0,1);
  lcd.print("FeedCatFood");

  WiFiManager wifiManager;

  wifiManager.autoConnect("FeedCatFood");
  
  //Use predefined PINS consts
  
  
  timeClient.begin();

  timeClient.update();
  Alarm.delay(5000);
  setTime(timeClient.getHours(),timeClient.getMinutes(),timeClient.getSeconds(),1,1,18);
  
  server.begin();

  
  Serial.println("Connected to wifi");
  lcd.clear();
  lcd.print("WiFi Connected!");

  delay(5000);
  lcd.clear();
  lcd.print("Connect to");

  lcd.setCursor(0,1);
  lcd.print(WiFi.localIP());

  delay(10000);
  lcd.noBacklight();

  

  server.on("/", [](){
    server.send(200, "text/html", webPage);
  });

  server.on("/success", [](){
    server.send(200, "text/html", "feed successful");
  });

  server.on("/feed", HTTP_GET,[](){
    feed();
  });
  
}



void loop(void){

  
  
  server.handleClient();

  lcd.clear();
  lcd.print(timeClient.getFormattedTime());

  lcd.setCursor(0,1);
  lcd.print(WiFi.localIP());

  while (digitalRead(D6) == 0) {
    feed();
  }

  while (digitalRead(D5) == 0) {
    lcd.backlight();
    Alarm.delay(5000);
    lcd.noBacklight();
  }

  Serial.println((String)hour() + ":" + (String)minute() + ":" + (String)second());
  
  Alarm.delay(1000);
  
  Alarm.alarmOnce(20,59,00, feed);
}
