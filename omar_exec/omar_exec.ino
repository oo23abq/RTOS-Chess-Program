#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Arduino_FreeRTOS.h>
#include <semphr.h>

// LCD Setup
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Pins
const int ledWhite = 10;
const int ledBlack = 11;
const int buttonWhite = 2;
const int buttonBlack = 3;

// State
volatile int timeWhite = 300;
volatile int timeBlack = 300;
volatile bool isWhiteTurn = true;

// Semaphores for button events
SemaphoreHandle_t semWhitePressed;
SemaphoreHandle_t semBlackPressed;

void setup() {
  pinMode(ledWhite, OUTPUT);
  pinMode(ledBlack, OUTPUT);
  pinMode(buttonWhite, INPUT_PULLUP);
  pinMode(buttonBlack, INPUT_PULLUP);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Speed Chess Game");
  delay(2000);
  lcd.clear();

  // Create semaphores
  semWhitePressed = xSemaphoreCreateBinary();
  semBlackPressed = xSemaphoreCreateBinary();

  // Create tasks
  xTaskCreate(TaskInput,     "Input",     128, NULL, 3, NULL);
  xTaskCreate(TaskCountdown, "Countdown", 128, NULL, 2, NULL);
  xTaskCreate(TaskDisplay,   "Display",   128, NULL, 1, NULL);
}

void loop() {
  // Empty — RTOS handles execution
}

void TaskInput(void *pvParameters) {
  bool whitePrev = HIGH;
  bool blackPrev = HIGH;

  for (;;) {
    // White button detection
    bool whiteState = digitalRead(buttonWhite);
    if (whitePrev == HIGH && whiteState == LOW && isWhiteTurn) {
      xSemaphoreGive(semWhitePressed);
    }
    whitePrev = whiteState;

    // Black button detection
    bool blackState = digitalRead(buttonBlack);
    if (blackPrev == HIGH && blackState == LOW && !isWhiteTurn) {
      xSemaphoreGive(semBlackPressed);
    }
    blackPrev = blackState;

    vTaskDelay(10 / portTICK_PERIOD_MS);  // debounce + low CPU load
  }
}

void TaskCountdown(void *pvParameters) {
  for (;;) {
    vTaskDelay(1000 / portTICK_PERIOD_MS);  // 1s delay

    if (isWhiteTurn && timeWhite > 0) timeWhite--;
    else if (!isWhiteTurn && timeBlack > 0) timeBlack--;

    // Endgame check
    if (timeWhite <= 0 || timeBlack <= 0) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Time's up!");
      digitalWrite(ledWhite, LOW);
      digitalWrite(ledBlack, LOW);
      vTaskSuspend(NULL);  // freeze this task
    }
  }
}

void TaskDisplay(void *pvParameters) {
  for (;;) {
    // Update LEDs
    digitalWrite(ledWhite, isWhiteTurn ? HIGH : LOW);
    digitalWrite(ledBlack, isWhiteTurn ? LOW : HIGH);

    // Update LCD
    lcd.setCursor(0, 0);
    lcd.print("W: ");
    lcd.print(timeWhite);
    lcd.print("s     ");

    lcd.setCursor(0, 1);
    lcd.print("B: ");
    lcd.print(timeBlack);
    lcd.print("s     ");

    vTaskDelay(200 / portTICK_PERIOD_MS);  // update every 200ms
  }
}

// ISR-safe button logic (runs via semaphores)
void handleWhitePress() {
  if (xSemaphoreTake(semWhitePressed, 0) == pdTRUE) {
    isWhiteTurn = false;
  }
}

void handleBlackPress() {
  if (xSemaphoreTake(semBlackPressed, 0) == pdTRUE) {
    isWhiteTurn = true;
  }
}


/*
==============================
🕒 Estimated Execution Times:
------------------------------
- Task 1: Read Button Input      ~1 ms
- Task 2: Update LEDs            ~0.5 ms
- Task 3: Display Times (LCD)    ~5 ms
- Task 4: Countdown Logic        ~1 ms (runs every 1000 ms)

==============================
📊 Worst-Case Response Times:
------------------------------
- Task 1 (Input Read): ~0 ms → executes first  ➤ ✅ Response = ~1 ms
- Task 2 (LED Update): waits for Task 1        ➤ ✅ Response = ~1.5 ms
- Task 3 (LCD Display): waits for 1 & 2        ➤ ✅ Response = ~6.5 ms
- Task 4 (Countdown): not in frame, triggered by 1s timer ➤ ✅ ~1 ms after 1s
==============================
*/
