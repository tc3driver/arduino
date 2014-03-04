// include the library code:
#include <LiquidCrystal.h>
#include <Wire.h>
#include <DHT.h>
#include <Adafruit_CC3000.h>
#include <ccspi.h>
#include <SPI.h>
#include <string.h>
#include "utility/debug.h"
#include<stdlib.h>

// Define RGB Pins 
#define REDLITE 6
#define GREENLITE 7
#define BLUELITE 8

// Define CC3000 chip pins
#define ADAFRUIT_CC3000_IRQ   3
#define ADAFRUIT_CC3000_VBAT  5
#define ADAFRUIT_CC3000_CS    10

// initialize the library with the numbers of the interface pins
//LuquidCrystal lcd(RS, EN, DB4, DB5, DB6, DB7);
LiquidCrystal lcd(2, A1, A0, 9, A2, A3);

// Create CC3000 instances
Adafruit_CC3000 cc3000 = Adafruit_CC3000(ADAFRUIT_CC3000_CS, ADAFRUIT_CC3000_IRQ, ADAFRUIT_CC3000_VBAT,
                                         SPI_CLOCK_DIV2); // you can change this clock speed
// Define DHTTYPE
#define DHTTYPE DHT22

// Define DHTXX Pin
#define DHT22PIN A4

// WLAN parameters
#define WLAN_SSID       "Humidor Net"
#define WLAN_PASS       "th1s1smyhum1d04"
// Security can be WLAN_SEC_UNSEC, WLAN_SEC_WEP, WLAN_SEC_WPA or WLAN_SEC_WPA2
#define WLAN_SECURITY   WLAN_SEC_WPA2

// Xively parameters
#define WEBSITE  "api.xively.com"
#define API_key  "sEfXAuqwksPe2T3z0mwLbwrT7PxFYsscsKPWNK8MkleU8N6J"
#define feedID  "1274577701"

// you can change the overall brightness by range 0 -> 255
int brightness = 255;
int red = 0;
int green = 255;
int blue = 0;
float thappyupper = 72;
float thappylower = 68;
float hhappyupper = 72;
float hhappylower = 65;
float hhappy = 0;
float thappy = 0;

DHT dht(DHT22PIN, DHTTYPE);

uint32_t ip;

byte deg[8] = {
  B1100,
  B10010,
  B10010,
  B1100,
  B0,
  B0,
  B0,
  B0,
};

void setup() {
    // Initialize
  Serial.begin(115200);
  
  Serial.println(F("\nInitializing..."));
  if (!cc3000.begin())
  {
    Serial.println(F("Couldn't begin()! Check your wiring?"));
    while(1);
  }
  // set up the LCD's number of rows and columns:
  lcd.createChar(0, deg); 
  lcd.begin(20, 4);
  // Print a message to the LCD.
  lcd.print(" Daniel Marshal 125");
  lcd.setCursor(0,1);
  lcd.print("      Humidor");
  
  pinMode(REDLITE, OUTPUT);
  pinMode(GREENLITE, OUTPUT);
  pinMode(BLUELITE, OUTPUT);
 
  brightness = 100;
  dht.begin();
   // Connect to WiFi network
  cc3000.connectToAP(WLAN_SSID, WLAN_PASS, WLAN_SECURITY);
  Serial.println(F("Connected!"));
  
  /* Wait for DHCP to complete */
  Serial.println(F("Request DHCP"));
  while (!cc3000.checkDHCP())
  {
    delay(100);
  }  

}
 
 
void loop() {
  // Get the website IP & print it
  ip = 0;
  Serial.print(WEBSITE); Serial.print(F(" -> "));
  while (ip == 0) {
    if (! cc3000.getHostByName(WEBSITE, &ip)) {
      Serial.println(F("Couldn't resolve!"));
    }
    delay(500);
  }
  cc3000.printIPdotsRev(ip);
  
  int th = 0;
  int hh = 0;
  red = 0;
  green = 255;
  blue = 0;
  float h=dht.readHumidity();
  float ct=dht.readTemperature();
  float t= f(ct);
  int te = int(t);
  int hu = int(h);
  if ( t > thappyupper ) 
  {
    thappy = t - thappyupper;
    th = 1;
  }
  else if ( t < thappylower )
  {
    thappy = thappylower - t;
    th = 2;
  }
  else {
	  th = 0;
	  thappy = 0;
  }
  if ( h > hhappyupper ) 
  {
    hhappy = h - hhappyupper;
    hh = 1;
  }
  else if ( h < hhappylower )
  {
    hhappy = hhappylower - h;
    hh = 2;
  }
  else
  {
	  hhappy = 0;
	  hh= 0;
  }
  if ( thappy > 10 ) 
  {
    lcd.setCursor(0,3);
    lcd.print("Temp Emer ");
  }
  else if (thappy <= 10 && thappy > 5)
  {
    lcd.setCursor(0,3);
    lcd.print("Temp Dang ");
  }
  else if (thappy <= 5 && thappy > 0)
  {
   lcd.setCursor(0,3);
    lcd.print("Temp Warn ");
  }
  else
  {
    lcd.setCursor(0,3);
    lcd.print("Temp Good ");
  }
  if ( hhappy > 10 ) 
  {
    lcd.setCursor(10,3);
    lcd.print(" Humi Emer");
  }
  else if (hhappy <= 10 && thappy > 5)
  {
    lcd.setCursor(10,3);
    lcd.print(" Humi Dang");
  }
  else if (hhappy <= 5 && hhappy > 0)
  {
    lcd.setCursor(10,3);
    lcd.print(" Humi Warn");
  }
  else
  {
    lcd.setCursor(10,3);
    lcd.print(" Humi Good");
  }
  red = 25 * (int) thappy;
  blue = 25 * (int) hhappy;
  if (red > 255)
  {
    red = 255;
    green = 0;
  }
  else 
  {
    green = green - red;
  }
  if (blue > 255)
  {
    blue = 255;
    green = 0;
  }
  else if ( blue <= 255 && blue > green)
  {
    green = 0;
  }
  else 
  {
    green = green - blue;
  }
  if (green < 0)
  {
	green = 0;
  }
  setBacklight(red, green, blue);
  lcd.setCursor(4,2);
  lcd.print(t, 0);
  lcd.write(byte(0));
  lcd.print("F ");
  lcd.print(h, 0);
  lcd.print("% RH   ");
  // Prepare JSON for Xively & get length
  int length = 0;

  String data = "";
  data = data + "\n" + "{\"version\":\"1.0.0\",\"datastreams\" : [ {\"id\" : \"DM_Humidor_Tempature\",\"current_value\" : \"" + String(te) + "\"}," 
  + "{\"id\" : \"DM_Humidor_Humidity\",\"current_value\" : \"" + String(hu) + "\"}]}";
  
  length = data.length();
  Serial.print("Data length");
  Serial.println(length);
  Serial.println();
  
  // Print request for debug purposes
  Serial.print("PUT /v2/feeds/");
  Serial.print(feedID);
  Serial.println(".json HTTP/1.0");
  Serial.println("Host: api.xively.com");
  Serial.print("X-ApiKey: ");
  Serial.println(API_key);
  Serial.print("Content-Length: ");
  Serial.println(length, DEC);
  Serial.print("Connection: close");
  Serial.println();
  Serial.print(data);
  Serial.println();
  
  // Send request
  Adafruit_CC3000_Client client = cc3000.connectTCP(ip, 80);
  if (client.connected()) {
    Serial.println("Connected!");
    client.println("PUT /v2/feeds/" + String(feedID) + ".json HTTP/1.0");
    client.println("Host: api.xively.com");
    client.println("X-ApiKey: " + String(API_key));
    client.println("Content-Length: " + String(length));
    client.print("Connection: close");
    client.println();
    client.print(data);
    client.println();
  } else {
    Serial.println(F("Connection failed"));    
    return;
  }
  
  Serial.println(F("-------------------------------------"));
  while (client.connected()) {
    while (client.available()) {
      char c = client.read();
      Serial.print(c);
    }
  }
  client.close();
  Serial.println(F("-------------------------------------"));
  
  // Wait 10 seconds until next update
  delay(10000);
}
 
 
 
void setBacklight(uint8_t r, uint8_t g, uint8_t b) {
  // normalize the red LED - its brighter than the rest!
  r = map(r, 0, 255, 0, 100);
  g = map(g, 0, 255, 0, 150);
 
  r = map(r, 0, 255, 0, brightness);
  g = map(g, 0, 255, 0, brightness);
  b = map(b, 0, 255, 0, brightness);
 
  // common anode so invert!
  r = map(r, 0, 255, 255, 0);
  g = map(g, 0, 255, 255, 0);
  b = map(b, 0, 255, 255, 0);
  analogWrite(REDLITE, r);
  analogWrite(GREENLITE, g);
  analogWrite(BLUELITE, b);
}
/*-----( Declare User-written Functions )-----*/
//
//Celsius to Fahrenheit conversion
double f(double celsius)
{
        return 1.8 * celsius + 32;
}

