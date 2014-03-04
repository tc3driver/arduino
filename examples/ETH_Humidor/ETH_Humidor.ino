// include the library code:
#include <LiquidCrystal.h>
#include <Wire.h>
#include <DHT.h>

// Define RGB Pins 
#define REDLITE 5
#define GREENLITE 6
#define BLUELITE 9

//Define DHT Pin


// initialize the library with the numbers of the interface pins
//LuquidCrystal lcd(RS, EN, DB4, DB5, DB6, DB7);
LiquidCrystal lcd(4, 7, 16, 15, 14, 8);
#define DHTTYPE DHT22

#define DHT22PIN 17
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
}
 
 
void loop() {
/*  for (int i = 0; i < 255; i++) {
    setBacklight(i, 0, 255-i);
    delay(5);
  }
  for (int i = 0; i < 255; i++) {
    setBacklight(255-i, i, 0);
    delay(5);
  }
  for (int i = 0; i < 255; i++) {
    setBacklight(0, 255-i, i);
    delay(5);
  } */
  int th = 0;
  int hh = 0;
  red = 0;
  green = 255;
  blue = 0;
  float h=dht.readHumidity();
  float ct=dht.readTemperature();
  float t= f(ct);
  if ( t > thappyupper ) 
  {
    thappy = t - thappyupper;
    th = 1;
  }
  if ( t < thappylower )
  {
    thappy = thappylower - t;
    th = 2;
  }
  if ( h > hhappyupper ) 
  {
    hhappy = h - hhappyupper;
    hh = 1;
  }
  if ( h < hhappylower )
  {
    hhappy = hhappylower - h;
    hh = 2;
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
  setBacklight(red, green, blue);
  lcd.setCursor(4,2);
  lcd.print(t, 0);
  lcd.write(byte(0));
  lcd.print("F ");
  lcd.print(h, 0);
  lcd.print("% RH");
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

