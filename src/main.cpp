#include <Arduino.h>
#define NORTH_PIN 10
#define EAST_PIN  11

#define EAST_RED_PIN     4
#define EAST_YELLOW_PIN  3
#define EAST_GREEN_PIN   2

#define NORTH_RED_PIN    5
#define NORTH_YELLOW_PIN 6
#define NORTH_GREEN_PIN  7

// ---------------- FSM STATES ----------------
#define goN   0
#define waitN 1
#define goE   2
#define waitE 3

struct State {
  byte Out;
  unsigned long Time;
  byte Next[4];
};
typedef const struct State STyp;

// Tabel FSM original
STyp FSM[4] = {
  {0b100001, 10,    {goN, waitN, goN, waitN}},  // GO_N
  {0b100010, 2000,  {goE, goE, goE, goE}},      // WAIT_N
  {0b001100, 10,    {goE, goE, waitE, waitE}},  // GO_E
  {0b010100, 2000,  {goN, goN, goN, goN}}       // WAIT_E
};

int FSM_State = goN;
int lastState = -1;

// ---------------- STATE NAME ----------------
const char* getStateName(int s) {
  switch (s) {
    case goN:   return "GO_N (Nord verde)";
    case waitN: return "WAIT_N (Nord galben)";
    case goE:   return "GO_E (Est verde)";
    case waitE: return "WAIT_E (Est galben)";
  }
  return "UNKNOWN";
}

void printStateChange() {
  if (FSM_State != lastState) {
    Serial.print("Stare noua: ");
    Serial.println(getStateName(FSM_State));
    lastState = FSM_State;
  }
}

// ---------------- INPUT ----------------
int GetInput(void) {
  return (digitalRead(NORTH_PIN) << 1) | digitalRead(EAST_PIN);
}

// ---------------- OUTPUT ----------------
void SetOutput(byte out) {
  const int pins[] = {
    EAST_RED_PIN, EAST_YELLOW_PIN, EAST_GREEN_PIN,
    NORTH_RED_PIN, NORTH_YELLOW_PIN, NORTH_GREEN_PIN
  };

  for (int i = 0; i < 6; i++) {
    digitalWrite(pins[i], (out >> (5 - i)) & 1);
  }
}

// ---------------- BLINK pentru WAIT ----------------
void blinkYellow(int yellowPin, int redPin, int greenPin) {
  // Stinge verdele/rosul
  digitalWrite(redPin, HIGH);
  digitalWrite(greenPin, LOW);

  for (int i = 0; i < 6; i++) {   // 6 clipiri × 0.5 sec = 3 sec
    digitalWrite(yellowPin, HIGH);
    delay(250);
    digitalWrite(yellowPin, LOW);
    delay(250);
  }
}

// ---------------- SETUP ----------------
void setup() {
  Serial.begin(115200);

  pinMode(NORTH_PIN, INPUT);
  pinMode(EAST_PIN,  INPUT);

  int pins[] = {
    EAST_RED_PIN, EAST_YELLOW_PIN, EAST_GREEN_PIN,
    NORTH_RED_PIN, NORTH_YELLOW_PIN, NORTH_GREEN_PIN
  };
  for (int i = 0; i < 6; i++) pinMode(pins[i], OUTPUT);

  Serial.println("=== Sistem pornit ===");
}

// ---------------- LOOP ----------------
void loop() {

  printStateChange();

  // ---------------- WAIT_N — Galben Nord blink ----------------
  if (FSM_State == waitN) {
    blinkYellow(NORTH_YELLOW_PIN, NORTH_RED_PIN, NORTH_GREEN_PIN);
  }
  // ---------------- WAIT_E — Galben Est blink ----------------
  else if (FSM_State == waitE) {
    blinkYellow(EAST_YELLOW_PIN, EAST_RED_PIN, EAST_GREEN_PIN);
  }
  else {
    // Stări normale GO_N / GO_E
    SetOutput(FSM[FSM_State].Out);
    delay(FSM[FSM_State].Time);
  }

  int input = GetInput();
  FSM_State = FSM[FSM_State].Next[input];
}