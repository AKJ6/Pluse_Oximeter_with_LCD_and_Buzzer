#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include "MAX30100_PulseOximeter.h"

#define REPORTING_PERIOD_MS     1000
LiquidCrystal_I2C lcd(0x27, 16, 2);

byte smile[] = {
  B00000,
  B00000,
  B01010,
  B00000,
  B10001,
  B01110,
  B00000,
  B00000
};
byte mod[] = {
  B00000,
  B00000,
  B01010,
  B00000,
  B11111,
  B00000,
  B00000,
  B00000
};
byte sad[] = {
  B00000,
  B00000,
  B01010,
  B00000,
  B01110,
  B10001,
  B00000,
  B00000
};

PulseOximeter pox;
uint32_t tsLastReport = 0;

void onBeatDetected()
{
  Serial.println("Beat!!!");
}

void setup()
{
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  lcd.createChar(1 , smile);
  lcd.createChar(2 , mod);
  lcd.createChar(3 , sad);
  lcd.setCursor(0, 0);
  lcd.print("      Pulse");
  lcd.setCursor(0, 1);
  lcd.print("    Oximeter");
  delay(2000);

  if (!pox.begin()) {
    lcd.println("FAILED");
    for (;;);
  } else {
    lcd.println("SUCCESS");
  }
  pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);

  pox.setOnBeatDetectedCallback(onBeatDetected);

  // Initialize LED pin
  pinMode(13, OUTPUT);
}

void loop()
{
  pox.update();
  if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
    lcd.clear();
    lcd.setCursor(0 , 0);
    lcd.print("BPM : ");
    lcd.print(pox.getHeartRate());
    lcd.setCursor(0 , 1);
    lcd.print("SpO2: ");
    lcd.print(pox.getSpO2());
    lcd.print("%");
    tsLastReport = millis();

    if (pox.getSpO2() >= 96) {
      lcd.setCursor(15 , 1);
      lcd.write(1);
       digitalWrite(13,LOW); 
    }
    else if (pox.getSpO2() >= 90) {
      lcd.setCursor(15 , 1);
      lcd.write(2);
    digitalWrite(13, LOW); 
    }
    else if (pox.getSpO2() < 90) {
      lcd.setCursor(15 , 1);
      lcd.write(3);
      digitalWrite(13, HIGH); // Turn on Buzzer
    }
  }
}