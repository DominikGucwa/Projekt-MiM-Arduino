#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// DS18B20 data pin
#define ONE_WIRE_BUS 2

// OLED 
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1 
#define SCREEN_ADDRESS 0x3C // I2C address dla wyświetlacza

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Buzzer pin
const int BZR = 5;

void setup() {
  Serial.begin(9600);

  sensors.begin();
  pinMode(BZR, OUTPUT);

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Temp:");
  display.display();
  delay(1000);
}

void loop() {
  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(0);

  Serial.print("Temperature: ");
  Serial.print(tempC);
  Serial.println(" C");

  // Sprawdzenie, czy czujnik jest podłączony
  if (tempC == DEVICE_DISCONNECTED_C) {
    noTone(BZR); // wyłącz buzzer
  } else if (tempC >= 27.00) {
    tone(BZR, 1000); 
  } else {
    noTone(BZR); // poniżej progu – wycisz buzzer
  }

  // Wyświetlanie na OLED
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Temp:");

  display.setTextSize(3);
  display.setCursor(0, 30);
  if (tempC == DEVICE_DISCONNECTED_C) {
    display.print("Error");
  } else {
    display.print(tempC, 1);
    display.print("C");
    if (tempC >= 27.00) {
      display.setTextSize(1);
      display.setCursor(85, 0);
      display.print("!");
    }
  }

  display.display();
  delay(1000);
}
