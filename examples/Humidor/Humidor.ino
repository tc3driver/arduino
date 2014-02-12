// include the library code:
#include <LiquidCrystal.h>
#include <Wire.h>
#include <DHT.h>

// Define RGB Pins 
#define REDLITE 11
#define GREENLITE 3
#define BLUELITE 5

//Define DHT Pin


// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(2, 6, 10, 9, 8, 7);
#define DHTTYPE DHT22

#define DHT22PIN 13
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
  Serial.begin(9600);
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
  Serial.print("red: ");
  Serial.print(red);
  Serial.print(" Green: " );
  Serial.print(green);
  Serial.print(" Blue: " );
  Serial.print(blue);
  Serial.print(" thappy: ");
  Serial.print(thappy);
  Serial.print(" hhappy: ");
  Serial.print(hhappy);
  Serial.print(" T: ");
  Serial.print(t);
  Serial.print(" H: ");
  Serial.print(h);
  Serial.print(" TH: ");
  Serial.print(th);
  Serial.print(" HH: ");
  Serial.print(hh);
  Serial.println();
  Serial.print("thappy should be: ");
  Serial.print(t - thappyupper);
  Serial.print(" hhappy should be: ");
  Serial.print(h - hhappyupper);
  Serial.println(" ");
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

//Celsius to Kelvin conversion
double Kelvin(double celsius)
{
        return celsius + 273.15;
}

// dewPoint function NOAA
// reference: http://wahiduddin.net/calc/density_algorithms.htm 
double dewPoint(double celsius, double humidity)
{
        double A0= 373.15/(273.15 + celsius);
        double SUM = -7.90298 * (A0-1);
        SUM += 5.02808 * log10(A0);
        SUM += -1.3816e-7 * (pow(10, (11.344*(1-1/A0)))-1) ;
        SUM += 8.1328e-3 * (pow(10,(-3.49149*(A0-1)))-1) ;
        SUM += log10(1013.246);
        double VP = pow(10, SUM-3) * humidity;
        double T = log(VP/0.61078);   // temp var
        return (241.88 * T) / (17.558-T);
}

// delta max = 0.6544 wrt dewPoint()
// 5x faster than dewPoint()
// reference: http://en.wikipedia.org/wiki/Dew_point
double dewPointFast(double celsius, double humidity)
{
        double a = 17.271;
        double b = 237.7;
        double temp = (a * celsius) / (b + celsius) + log(humidity/100);
        double Td = (b * temp) / (a - temp);
        return Td;
}

