#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// Pins
const int led1 = 10;       // LED for Player 1 (Black)
const int led2 = 11;       // LED for Player 2 (White)
const int button1 = 2;     // Button for Player 1
const int button2 = 3;     // Button for Player 2
const int buzzer = 12;     // Buzzer pin

// Timer variables
unsigned long player1_time = 30000;  // 30 seconds
unsigned long player2_time = 30000;
unsigned long lastUpdate = 0;

// Flags
bool player1_turn = true;
bool buzzerTriggeredP1 = false;
bool buzzerTriggeredP2 = false;

void setup() {
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Chess Clock Ready");
  delay(1000);
  lcd.clear();
}

void loop() {
  unsigned long now = millis();

  // Display Time
  lcd.setCursor(0, 0);
  lcd.print("P1: ");
  lcd.print(player1_time / 1000);
  lcd.print("s   ");

  lcd.setCursor(0, 1);
  lcd.print("P2: ");
  lcd.print(player2_time / 1000);
  lcd.print("s   ");

  // Blink the current player's LED
  static unsigned long lastBlink = 0;
  static bool ledState = false;
  if (now - lastBlink >= 500) {
    ledState = !ledState;
    digitalWrite(led1, player1_turn ? ledState : LOW);
    digitalWrite(led2, !player1_turn ? ledState : LOW);
    lastBlink = now;
  }

  // Countdown every second
  if (now - lastUpdate >= 1000) {
    if (player1_turn && player1_time > 0) {
      player1_time -= 1000;
    }
    if (!player1_turn && player2_time > 0) {
      player2_time -= 1000;
    }
    lastUpdate = now;
  }

  // Buzzer when 5s or less remaining
  if (player1_turn && player1_time <= 5000 && !buzzerTriggeredP1) {
    tone(buzzer, 1000, 150);
    buzzerTriggeredP1 = true;
  }

  if (!player1_turn && player2_time <= 5000 && !buzzerTriggeredP2) {
    tone(buzzer, 1000, 150);
    buzzerTriggeredP2 = true;
  }

  // Button to switch turns
  if (digitalRead(button1) == LOW && player1_turn) {
    player1_turn = false;
    delay(200);  // Debounce
  }

  if (digitalRead(button2) == LOW && !player1_turn) {
    player1_turn = true;
    delay(200);  // Debounce
  }
}
