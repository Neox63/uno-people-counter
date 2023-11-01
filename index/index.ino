#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64
#define SCREEN_ADDRESS 0x3C  ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Capteur d'entrée
const int trigPin1 = 2; 
const int echoPin1 = 3; 

// Capteur de sortie
const int trigPin2 = 4; 
const int echoPin2 = 5;

// indexed initial distance for entry
const int distanceEntry = 100;

//indexed initial distance for exit
const int distanceExit = 100;

unsigned long previousMillis = 0;
const long interval = 2000;

int entryCount = 0;
int exitCount = 0;
bool entryTouched = false;
bool exitTouched = false;

void setup() {
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;  // Don't proceed, loop forever
  }

  display.clearDisplay();
  display.display();
}

void loop() {
  // Capteur d'entrée
  long duration1, distance1;
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);
  duration1 = pulseIn(echoPin1, HIGH);
  distance1 = (duration1 / 2) / 29.1;

  // Capteur de sortie
  long duration2, distance2;
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
  duration2 = pulseIn(echoPin2, HIGH);
  distance2 = (duration2 / 2) / 29.1; 

  if (distance1 < distanceEntry && distance2 > distanceExit && !entryTouched) {
    if (millis() - previousMillis >= interval) {
      entryCount++;
      previousMillis = millis();
      entryTouched = true;
    }
  } else if (distance1 > distanceEntry && distance2 < distanceExit && !exitTouched) {
    if (millis() - previousMillis >= interval) {
      exitCount++;
      previousMillis = millis();
      exitTouched = true;
    }
  }

  if (distance1 > distanceEntry) {
    entryTouched = false;
  }

  if (distance2 > distanceExit) {
    exitTouched = false;
  }
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(2, 0);
  display.print("Entries : ");
  display.print(entryCount);

  display.setCursor(2, 10);
  display.print("Exits : ");
  display.print(exitCount);

  display.setCursor(2, 40);
  display.print("Entry dist : ");
  display.print(distance1);
  display.print("cm");

  display.setCursor(2, 50);
  display.print("Exit dist : ");
  display.print(distance2);
  display.print("cm");

  display.display();
}
