//لعبة الثعبان الجائع الكلاسيكيةعادت من جديد بشكل اخر في قناة الورشة 
//التصميم و المخطط و الكود بحسابي على gethub 
// رابط الكود -- https://github.com/Warsha249/hungry-snake-game 
//مكتبة (LedControl) موجوده على نفس الرابط 
#include <LedControl.h>

// تعريف دبابيس التحكم
#define BUZZER_PIN 3
#define JOYSTICK_X A0
#define JOYSTICK_Y A1
#define JOYSTICK_BUTTON 2
#define SPEED_POT A2

// إعدادات شاشة LED
const int MATRIX_SIZE = 8;
const int MESSAGE_SPEED = 4;
LedControl lc = LedControl(12, 11, 10, 1); // DIN, CLK, CS

// متغيرات اللعبة
int snakeX[64], snakeY[64];
int snakeLength = 3;
int foodX, foodY;
int direction = 0; // 0=يمين, 1=يسار, 2=أعلى, 3=أسفل
int lastDirection = 0;
bool gameOver = false;
bool win = false;
unsigned long lastMoveTime = 0;
int moveDelay = 300;
unsigned long lastFoodBlink = 0;
bool foodVisible = true;
const int foodBlinkInterval = 300;

// تعريفات الأصوات
const int shortBeep = 800;
const int longBeep = 600;

// رسائل العرض
const PROGMEM bool snakeMessage[8][56] = {
  {0,1,1,1,1,1,0,1,0,0,0,0,0,1,1,0,0,1,1,1,1,1,0,1,0,0,0,1,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,1,0,0,0,1,0,1,1,0,0,0,0,1,1,0,0,1,0,0,0,1,0,1,0,0,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,1,0,0,0,0,0,1,0,1,0,0,0,1,1,0,0,1,0,0,0,1,0,1,0,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,1,1,1,1,1,0,1,0,0,1,0,0,1,1,0,0,1,1,1,1,1,0,1,1,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,1,0,1,0,0,0,1,0,1,1,0,0,1,0,0,0,1,0,1,0,1,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,1,0,1,0,0,0,0,1,1,1,0,0,1,0,0,0,1,0,1,0,0,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,1,0,0,0,1,0,1,0,0,0,0,0,1,1,0,0,1,0,0,0,1,0,1,0,0,0,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,1,1,1,1,1,0,1,0,0,0,0,0,1,1,0,0,1,0,0,0,1,0,1,0,0,0,0,1,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};

const PROGMEM bool gameOverMessage[8][90] = {
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,1,1,0,0,0,1,1,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,0,0,0,1,1,0,0,1,1,0,0,1,1,1,1,1,1,0,0,1,1,1,1,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,1,0,1,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,1,1,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,1,1,1,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,1,1,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,1,1,1,1,0,0,1,1,0,1,0,1,1,0,0,1,1,1,1,1,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,1,1,1,0,0,0,1,1,1,1,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,1,1,0,1,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,1,1,0,0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,1,1,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,1,1,1,1,1,1,0,0,1,1,0,0,1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0}
};

const PROGMEM bool scoreMessage[8][58] = {
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,1,1,1,1,1,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,1,1,1,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,1,1,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,1,1,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,1,1,0,0,1,1,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0}
};

const PROGMEM bool digits[][8][8] = {
  {
    {0,0,0,0,0,0,0,0},
    {0,0,1,1,1,1,0,0},
    {0,1,1,0,0,1,1,0},
    {0,1,1,0,1,1,1,0},
    {0,1,1,1,0,1,1,0},
    {0,1,1,0,0,1,1,0},
    {0,1,1,0,0,1,1,0},
    {0,0,1,1,1,1,0,0}
  },
  {
    {0,0,0,0,0,0,0,0},
    {0,0,0,1,1,0,0,0},
    {0,0,0,1,1,0,0,0},
    {0,0,1,1,1,0,0,0},
    {0,0,0,1,1,0,0,0},
    {0,0,0,1,1,0,0,0},
    {0,0,0,1,1,0,0,0},
    {0,1,1,1,1,1,1,0}
  },
  {
    {0,0,0,0,0,0,0,0},
    {0,0,1,1,1,1,0,0},
    {0,1,1,0,0,1,1,0},
    {0,0,0,0,0,1,1,0},
    {0,0,0,0,1,1,0,0},
    {0,0,1,1,0,0,0,0},
    {0,1,1,0,0,0,0,0},
    {0,1,1,1,1,1,1,0}
  },
  {
    {0,0,0,0,0,0,0,0},
    {0,0,1,1,1,1,0,0},
    {0,1,1,0,0,1,1,0},
    {0,0,0,0,0,1,1,0},
    {0,0,0,1,1,1,0,0},
    {0,0,0,0,0,1,1,0},
    {0,1,1,0,0,1,1,0},
    {0,0,1,1,1,1,0,0}
  },
  {
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,1,1,0,0},
    {0,0,0,1,1,1,0,0},
    {0,0,1,0,1,1,0,0},
    {0,1,0,0,1,1,0,0},
    {0,1,1,1,1,1,1,0},
    {0,0,0,0,1,1,0,0},
    {0,0,0,0,1,1,0,0}
  },
  {
    {0,0,0,0,0,0,0,0},
    {0,1,1,1,1,1,1,0},
    {0,1,1,0,0,0,0,0},
    {0,1,1,1,1,1,0,0},
    {0,0,0,0,0,1,1,0},
    {0,0,0,0,0,1,1,0},
    {0,1,1,0,0,1,1,0},
    {0,0,1,1,1,1,0,0}
  },
  {
    {0,0,0,0,0,0,0,0},
    {0,0,1,1,1,1,0,0},
    {0,1,1,0,0,1,1,0},
    {0,1,1,0,0,0,0,0},
    {0,1,1,1,1,1,0,0},
    {0,1,1,0,0,1,1,0},
    {0,1,1,0,0,1,1,0},
    {0,0,1,1,1,1,0,0}
  },
  {
    {0,0,0,0,0,0,0,0},
    {0,1,1,1,1,1,1,0},
    {0,1,1,0,0,1,1,0},
    {0,0,0,0,1,1,0,0},
    {0,0,0,0,1,1,0,0},
    {0,0,0,1,1,0,0,0},
    {0,0,0,1,1,0,0,0},
    {0,0,0,1,1,0,0,0}
  },
  {
    {0,0,0,0,0,0,0,0},
    {0,0,1,1,1,1,0,0},
    {0,1,1,0,0,1,1,0},
    {0,1,1,0,0,1,1,0},
    {0,0,1,1,1,1,0,0},
    {0,1,1,0,0,1,1,0},
    {0,1,1,0,0,1,1,0},
    {0,0,1,1,1,1,0,0}
  },
  {
    {0,0,0,0,0,0,0,0},
    {0,0,1,1,1,1,0,0},
    {0,1,1,0,0,1,1,0},
    {0,1,1,0,0,1,1,0},
    {0,0,1,1,1,1,1,0},
    {0,0,0,0,0,1,1,0},
    {0,1,1,0,0,1,1,0},
    {0,0,1,1,1,1,0,0}
  }
};

void setup() {
  Serial.begin(9600);
  lc.shutdown(0, false);
  lc.setIntensity(0, 8);
  lc.clearDisplay(0);
  pinMode(JOYSTICK_BUTTON, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);
  showSnakeMessage();
  playStartSound();
  startGame();
}

void loop() {
  if (!gameOver && !win) {
    readJoystick();
    if (millis() - lastMoveTime > moveDelay) {
      moveSnake();
      drawSnake();
      lastMoveTime = millis();
    }
    updateFoodBlink();
  } else {
    if (gameOver) {
      unrollSnake();
      showScoreMessage(snakeLength);
      showGameOverMessage();
    } else if (win) {
      unrollSnake();
      showScoreMessage(snakeLength);
    }
    while (digitalRead(JOYSTICK_BUTTON) == HIGH) delay(100);
    delay(200);
    startGame();
  }
}

void startGame() {
  snakeLength = 3;
  for (int i = 0; i < snakeLength; i++) {
    snakeX[i] = 3 - i;
    snakeY[i] = 3;
  }
  generateFood();
  direction = 0;
  lastDirection = 0;
  gameOver = false;
  win = false;
  foodVisible = true;
  lc.clearDisplay(0);
  drawSnake();
  drawFood();
}

void generateFood() {
  bool onSnake;
  do {
    onSnake = false;
    foodX = random(0, MATRIX_SIZE);
    foodY = random(0, MATRIX_SIZE);
    for (int i = 0; i < snakeLength; i++) {
      if (snakeX[i] == foodX && snakeY[i] == foodY) {
        onSnake = true;
        break;
      }
    }
  } while (onSnake);
  foodVisible = true;
  lastFoodBlink = millis();
}

void drawSnake() {
  lc.clearDisplay(0);
  for (int i = 0; i < snakeLength; i++) {
    lc.setLed(0, snakeY[i], snakeX[i], true);
  }
  if(foodVisible) drawFood();
}

void drawFood() {
  lc.setLed(0, foodY, foodX, true);
}

void readJoystick() {
  int xValue = analogRead(JOYSTICK_X);
  int yValue = analogRead(JOYSTICK_Y);
  
  if (xValue < 300 && lastDirection != 0) direction = 1;
  else if (xValue > 700 && lastDirection != 1) direction = 0;
  else if (yValue < 300 && lastDirection != 3) direction = 2;
  else if (yValue > 700 && lastDirection != 2) direction = 3;
  
  moveDelay = map(analogRead(SPEED_POT), 0, 1023, 100, 500);
  
  if (digitalRead(JOYSTICK_BUTTON) == LOW && gameOver) {
    delay(200);
    startGame();
  }
}

void moveSnake() {
  int prevX = snakeX[0];
  int prevY = snakeY[0];
  int newX = prevX;
  int newY = prevY;
  
  switch (direction) {
    case 0: newX++; break;
    case 1: newX--; break;
    case 2: newY--; break;
    case 3: newY++; break;
  }
  
  if (newX < 0) newX = MATRIX_SIZE - 1;
  if (newX >= MATRIX_SIZE) newX = 0;
  if (newY < 0) newY = MATRIX_SIZE - 1;
  if (newY >= MATRIX_SIZE) newY = 0;
  
  for (int i = 0; i < snakeLength; i++) {
    if (snakeX[i] == newX && snakeY[i] == newY) {
      gameOver = true;
      playGameOverSound();
      return;
    }
  }
  
  bool ateFood = (newX == foodX && newY == foodY);
  if (ateFood) {
    snakeLength++;
    playEatSound();
    generateFood();
    if (snakeLength >= 64) win = true;
  }
  
  for (int i = snakeLength - 1; i > 0; i--) {
    snakeX[i] = snakeX[i - 1];
    snakeY[i] = snakeY[i - 1];
  }
  
  snakeX[0] = newX;
  snakeY[0] = newY;
  
  if (!ateFood) lc.setLed(0, snakeY[snakeLength], snakeX[snakeLength], false);
  lastDirection = direction;
}

void updateFoodBlink() {
  if (millis() - lastFoodBlink > foodBlinkInterval) {
    foodVisible = !foodVisible;
    lastFoodBlink = millis();
    if(foodVisible) drawFood();
    else lc.setLed(0, foodY, foodX, false);
  }
}

void playStartSound() {
  tone(BUZZER_PIN, shortBeep, 100);
  delay(100);
  tone(BUZZER_PIN, shortBeep, 100);
  delay(100);
  tone(BUZZER_PIN, longBeep, 200);
  delay(200);
  noTone(BUZZER_PIN);
  delay(100);
  tone(BUZZER_PIN, shortBeep, 100);
  delay(100);
  tone(BUZZER_PIN, shortBeep, 100);
  delay(100);
  tone(BUZZER_PIN, longBeep, 200);
  delay(200);
  noTone(BUZZER_PIN);
}

void playEatSound() {
  tone(BUZZER_PIN, shortBeep, 50);
  delay(50);
  tone(BUZZER_PIN, shortBeep+200, 50);
  delay(50);
  noTone(BUZZER_PIN);
}

void playGameOverSound() {
  tone(BUZZER_PIN, longBeep, 300);
  delay(300);
  tone(BUZZER_PIN, longBeep-200, 500);
  delay(500);
  noTone(BUZZER_PIN);
}

void showSnakeMessage() {
  for (int d = 0; d < sizeof(snakeMessage[0]) - 7; d++) {
    for (int col = 0; col < 8; col++) {
      delay(MESSAGE_SPEED);
      for (int row = 0; row < 8; row++) {
        lc.setLed(0, row, col, pgm_read_byte(&(snakeMessage[row][col + d])));
      }
    }
  }
  lc.clearDisplay(0);
}

void showGameOverMessage() {
  for (int d = 0; d < sizeof(gameOverMessage[0]) - 7; d++) {
    for (int col = 0; col < 8; col++) {
      delay(MESSAGE_SPEED);
      for (int row = 0; row < 8; row++) {
        lc.setLed(0, row, col, pgm_read_byte(&(gameOverMessage[row][col + d])));
      }
    }
  }
  lc.clearDisplay(0);
}

void showScoreMessage(int score) {
  if (score < 0 || score > 99) return;
  int second = score % 10;
  int first = (score / 10) % 10;

  for (int d = 0; d < sizeof(scoreMessage[0]) + 2 * sizeof(digits[0][0]); d++) {
    for (int col = 0; col < 8; col++) {
      delay(MESSAGE_SPEED);
      for (int row = 0; row < 8; row++) {
        if (d <= sizeof(scoreMessage[0]) - 8) {
          lc.setLed(0, row, col, pgm_read_byte(&(scoreMessage[row][col + d])));
        }
        int c = col + d - sizeof(scoreMessage[0]) + 6;
        if (score < 10) c += 8;
        if (c >= 0 && c < 8) {
          if (first > 0) lc.setLed(0, row, col, pgm_read_byte(&(digits[first][row][c])));
        } else {
          c -= 8;
          if (c >= 0 && c < 8) {
            lc.setLed(0, row, col, pgm_read_byte(&(digits[second][row][c])));
          }
        }
      }
    }
  }
  lc.clearDisplay(0);
}

void unrollSnake() {
  lc.setLed(0, foodY, foodX, false);
  delay(600);
  for (int i = 1; i <= snakeLength; i++) {
    for (int row = 0; row < 8; row++) {
      for (int col = 0; col < 8; col++) {
        bool found = false;
        for (int j = 0; j < snakeLength; j++) {
          if (snakeX[j] == col && snakeY[j] == row && j == snakeLength - i) {
            found = true;
            break;
          }
        }
        if (found) {
          lc.setLed(0, row, col, false);
          delay(100);
        }
      }
    }
  }
}
