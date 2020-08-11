#include <Arduino.h>
#include "DHT.h"
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>


#include "calibration.h"

#define RED 12
#define GREEN 11
#define BLUE 10 

#define DHTPIN 4
#define DHTTYPE DHT22
#define MQ2PIN 7
#define INTPIN 2

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
float R0 = 0;
volatile byte state = LOW;
volatile int pwm = 100;
int value = 0;

void blink();

void setup() {
  Serial.begin(9600);
  dht.begin();
  lcd.begin(16, 2);
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Calibration...");
  R0 = MQCalibration(MQ2PIN);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Done..."); 
  lcd.print("Ro=");
  lcd.print(R0);
  
  // put your setup code here, to run once:
  
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(INTPIN, INPUT_PULLUP);
}

void loop() {

  // read several values from MQ2 
  float mq2 = MQRead(MQ2PIN);
  int CoConcentration = MQGetGasPercentage(mq2 / R0, (char *) "CO");
  
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  value = digitalRead(INTPIN);
  pwm = (value == HIGH) ? 255 : 100;
  
  if(t > 27 && h >= 50) {
    digitalWrite(RED, HIGH);
    digitalWrite(GREEN, LOW);
    analogWrite(BLUE, 0);
  } else {
    digitalWrite(GREEN, value);
    digitalWrite(RED, value);
    analogWrite(BLUE, pwm);
  }

  Serial.print("CO concentration: ");
  Serial.print(CoConcentration);
  Serial.print("\tTemp: ");
  Serial.print(t);
  Serial.print("\t Humidity: ");
  Serial.println(h);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("T:");
  lcd.print(t, 1);

  lcd.setCursor(8, 0);
  lcd.print("RH:");
  lcd.print(h, 1);
  lcd.setCursor(0, 1);
  lcd.print("CO:");
  lcd.print(CoConcentration, 1);
  lcd.print("ppm");
  
  delay(500);
}
