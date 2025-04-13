#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <semphr.h>


// LCD Setup (I2C 0x27)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Pins
const int ledWhite = 10;
const int ledBlack = 11;
const int buttonWhite = 2;
const int buttonBlack = 3;
const int buzzerPin = 12;
const int frameSize = 20; // ms

// Game state
int timeWhite = 30; // 30 seconds for each player
int timeBlack = 30;
bool isWhiteTurn = true;
bool whitePressed = false;
bool blackPressed = false;

unsigned long prevMillis = 0;
unsigned long lastSecond = 0;

void setup() {
  pinMode(ledWhite, OUTPUT);
  pinMode(ledBlack, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(buttonWhite, INPUT_PULLUP);
  pinMode(buttonBlack, INPUT_PULLUP);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Speed Chess Init");
  delay(2000);
  lcd.clear();
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - prevMillis >= frameSize) {
    prevMillis = currentMillis;

    // === Task 1: Display Times ===
    lcd.setCursor(0, 0);
    lcd.print("P1: ");
    lcd.print(timeWhite);
    lcd.print("s     ");

    lcd.setCursor(0, 1);
    lcd.print("P2: ");
    lcd.print(timeBlack);
    lcd.print("s     ");

    // === Task 2: Read Button Input ===
    if (digitalRead(buttonWhite) == LOW && !whitePressed && isWhiteTurn) {
      isWhiteTurn = false;
      whitePressed = true;
    } else if (digitalRead(buttonWhite) == HIGH) {
      whitePressed = false;
    }

    if (digitalRead(buttonBlack) == LOW && !blackPressed && !isWhiteTurn) {
      isWhiteTurn = true;
      blackPressed = true;
    } else if (digitalRead(buttonBlack) == HIGH) {
      blackPressed = false;
    }

    // === Task 3: Update LEDs ===
    digitalWrite(ledWhite, isWhiteTurn ? HIGH : LOW);
    digitalWrite(ledBlack, isWhiteTurn ? LOW : HIGH);

    // === Countdown and Buzzer Logic ===
    if (currentMillis - lastSecond >= 1000) {
      lastSecond = currentMillis;

      if (isWhiteTurn && timeWhite > 0) timeWhite--;
      else if (!isWhiteTurn && timeBlack > 0) timeBlack--;

      if ((isWhiteTurn && timeWhite <= 10 && timeWhite > 0) ||
          (!isWhiteTurn && timeBlack <= 10 && timeBlack > 0)) {
        tone(buzzerPin, 1000, 200); // 1kHz beep for 200ms
      }

      if (timeWhite <= 0 || timeBlack <= 0) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Time's up!");
        digitalWrite(ledWhite, LOW);
        digitalWrite(ledBlack, LOW);
        noTone(buzzerPin);
        while (1); // halt
      }
    }
  }
}
