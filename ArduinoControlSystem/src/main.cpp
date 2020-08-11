#include <Arduino.h>
#include "DHT.h"
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#include "calibration.h"

#define RED 12
#define GREEN 11
#define BLUE 10 

#define DHTPIN 2
#define DHTTYPE DHT22

#define MQ2PIN 7


DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
float R0 = 0;

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
}

void loop() {

  // read several values from MQ2 
  float mq2 = MQRead(MQ2PIN);
  int CoConcentration = MQGetGasPercentage(mq2 / R0, "CO");
  
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if(t > 27 && h >= 50){
    digitalWrite(RED, HIGH);
    digitalWrite(GREEN, HIGH);
    digitalWrite(BLUE, HIGH);
  } else{
    digitalWrite(GREEN, LOW);
    digitalWrite(BLUE, LOW);
    digitalWrite(RED, LOW);
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
  
  delay(1000);
}
