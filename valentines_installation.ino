#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

//================================
// POEM 
//================================

const char *poem[] = {
  "I think it's pretty obvious",
  "But just in case, let me ask",
  "Would you be my Valentine's?",
  "Someone I can share my life with",
  "Through all the good and bad",
  "So won't you be mine?",
};

const int numberOfLines = sizeof(poem) / sizeof(poem[0]); // Number of lines in the poem
int currentLine = 0; // Current line of the poem being displayed
unsigned long textTimer = 0; // Timer for text update
const unsigned long textInterval = 2000; // Time in ms to display each line

//================================
// HEART
//================================

struct Heart {
  int x, y;
  int size; // Diameter of the circles in the heart
  int speed;
};

const int numberOfHearts = 10;
Heart hearts[numberOfHearts];

void initializeHeart(int index) {
  hearts[index].size = random(8, 20);
  hearts[index].x = random(0, tft.width());
  hearts[index].y = tft.height() + hearts[index].size; // Start off screen
  hearts[index].speed = random(5, 10);
}

void drawHeart(int x, int y, int size) {
  int radius = size / 4;
  tft.fillCircle(x - radius, y + radius, radius, TFT_RED);
  tft.fillCircle(x + radius, y + radius, radius, TFT_RED);
  tft.fillTriangle(x - size / 2, y + radius, x + size / 2, y + radius, x, y + size/1.5, TFT_RED);
}

void setup() {
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);

  randomSeed(analogRead(0)); // Initialize random number generator

  // Initialize hearts with random properties
  for (int i = 0; i < numberOfHearts; i++) {
    initializeHeart(i);
  }

  // Initialize poem
  tft.setTextSize(1.5);
  tft.setTextColor(TFT_WHITE, TFT_BLACK); // Text color and background
  textTimer = millis(); // Initialize timer for text display
}

void loop() {
  tft.fillScreen(TFT_BLACK); // Clear
  // tft.fillRect(0, tft.height() - 30, tft.width(), 30, TFT_BLACK);

  for (int i = 0; i < numberOfHearts; i++) {
    // Update heart position
    hearts[i].y -= hearts[i].speed;
    // Draw heart
    drawHeart(hearts[i].x, hearts[i].y, hearts[i].size);
    // Check if heart is off the screen, then reinitialize
    if (hearts[i].y < -hearts[i].size) {
      initializeHeart(i);
    }
  }

  if (millis() - textTimer > textInterval) {
    textTimer = millis(); // Reset the timer
    currentLine = (currentLine + 1) % numberOfLines; // Move to the next line
  }
  // Display the text centered at the bottom of the screen
  tft.setTextDatum(MC_DATUM);
  tft.drawString(poem[currentLine], tft.width() / 2, tft.height() / numberOfLines + (currentLine)*(tft.height() / (numberOfLines+1)));
  // tft.drawString(poem[currentLine], tft.width() / 2, tft.height() / 2);
  
  delay(100); // Delay to control animation speed
}
