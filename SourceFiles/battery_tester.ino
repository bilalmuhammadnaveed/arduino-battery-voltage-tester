#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define LCD_ADDRESS 0x27     // Try 0x27 first. If not working, try 0x3F.
#define BATTERY_PIN A0

// Measure Arduino 5V pin with multimeter and update this if needed.
// Example: if Arduino 5V is actually 4.82V, write 4.82 here.
#define ADC_REF_VOLTAGE 5.00
#define ADC_MAX_VALUE 1023.0

// For 1.5V cell under load
#define NO_CELL_VOLTAGE 0.10
#define EMPTY_VOLTAGE   0.90
#define FULL_VOLTAGE    1.50

LiquidCrystal_I2C lcd(LCD_ADDRESS, 16, 2);

float readBatteryVoltage();
int getHealthBars(float voltage);
const char* getBatteryStatus(float voltage);
void printHealthBar(int bars);

void setup() {
  lcd.init();
  lcd.backlight();

  pinMode(BATTERY_PIN, INPUT);

  lcd.setCursor(0, 0);
  lcd.print("Battery Tester");
  lcd.setCursor(0, 1);
  lcd.print("Loaded Voltage");

  delay(2000);
  lcd.clear();
}

void loop() {
  float voltage = readBatteryVoltage();

  lcd.setCursor(0, 0);

  if (voltage < NO_CELL_VOLTAGE) {
    lcd.print("No Cell         ");
    lcd.setCursor(0, 1);
    lcd.print("                ");
  } else {
    int bars = getHealthBars(voltage);
    const char* status = getBatteryStatus(voltage);

    lcd.print("V:");
    lcd.print(voltage, 3);
    lcd.print("V ");
    lcd.print(status);
    lcd.print("   ");

    printHealthBar(bars);
  }

  delay(500);
}

float readBatteryVoltage() {
  long total = 0;

  for (int i = 0; i < 60; i++) {
    total += analogRead(BATTERY_PIN);
    delay(2);
  }

  float averageAdc = total / 60.0;
  float voltage = averageAdc * (ADC_REF_VOLTAGE / ADC_MAX_VALUE);

  return voltage;
}

int getHealthBars(float voltage) {
  if (voltage <= EMPTY_VOLTAGE) {
    return 0;
  }

  if (voltage >= FULL_VOLTAGE) {
    return 16;
  }

  float health = (voltage - EMPTY_VOLTAGE) / (FULL_VOLTAGE - EMPTY_VOLTAGE);
  int bars = health * 16.0;

  return bars;
}

const char* getBatteryStatus(float voltage) {
  if (voltage < 0.90) {
    return "DEAD";
  } else if (voltage < 1.10) {
    return "LOW";
  } else if (voltage < 1.30) {
    return "OK";
  } else {
    return "GOOD";
  }
}

void printHealthBar(int bars) {
  lcd.setCursor(0, 1);

  for (int i = 0; i < 16; i++) {
    if (i < bars) {
      lcd.write(byte(255));   // full block
    } else {
      lcd.print(" ");
    }
  }
}