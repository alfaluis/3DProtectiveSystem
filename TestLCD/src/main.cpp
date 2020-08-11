#include <Arduino.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>


//LiquidCrystal_I2C lcd(0x38);  // Set the LCD I2C address

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address


void setup() {
  Serial.begin(115200);
  Serial.println(F("DHTxx test!"));

  lcd.begin(16, 2);
  lcd.setCursor(0,0); 
  lcd.print("Hello word");
}

void loop() {
  // Wait a few seconds between measurements.
  delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = random(100);
  // Read temperature as Celsius (the default)
  float t = random(10);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Temp: ");
  lcd.print(t, 1);
  lcd.setCursor(0, 1);
  lcd.print("Humidity: ");
  lcd.print(h, 1);
}