#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "TPG DA33";
const char* pass = "1234567890";

// For the Adafruit shield, these are the default.
#define TFT_DC D1
#define TFT_CS D2

// Use hardware SPI (on Uno, #13, #12, #11) and the above for CS/DC
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

ESP8266WebServer server(80);

void root()
{
server.send(200, "text/plain",
"1->Open /number?value=90 to print number on LCD \r\n\
2->Open /string?input=putYourString to print text on LCD \
");
}

void handle_string() {
  char inputString[100];
  server.arg("input").toCharArray(inputString, 99, 0);

  tft.setCursor(0, 80);
  tft.fillRect(0, 80, 240, 240, ILI9341_BLACK);
  tft.println(inputString);

  server.send(200, "text/plain",inputString);
}

void handle_number()
{
  String sval = server.arg("value");
  String xval = server.arg("muga");

/*
  Serial.println(sval.toInt());
  Serial.println(xval.toFloat());
  int ival = sval.toInt();
  Serial.println(ival);
// print on the LCD
  setCursor(0, 50);
  print(ival);
  print("          ");

  analogWrite(13, ival);
*/
 // server.send(200, "text/plain", String(ival, DEC));
}
void setup() {

  tft.begin();

  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(3);
  tft.println("Connecting to wifi");
WiFi.begin(ssid, pass);
while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  tft.print(".");
  }

  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(0, 0);
  tft.println("Connected to");
  tft.setCursor(0, 24);
  tft.println(ssid);

  IPAddress ipadd;
  char ipAddConverted2String[26];
  ipadd = WiFi.localIP();
  sprintf(ipAddConverted2String, "%d.%d.%d.%d", ipadd[0], ipadd[1], ipadd[2], ipadd[3]);
  tft.setCursor(0, 48);
  tft.println(ipAddConverted2String);

server.on("/", root);
  server.on("/number", handle_number);
  server.on("/string", handle_string);
  server.begin();
}


void loop(void) {

server.handleClient();

}

