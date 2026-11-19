/*
  STEP 2 - An accurate beat                    (paper first, then code; 2 to 3 hours)

  GOAL
    One short pulse per beat on CLICK_PIN, at any tempo from 30.0 to 999.9 BPM,
    with drift inside 0.1 BPM and jitter you have measured. No display, no
    buttons, no subdivisions yet. Tempo is typed into the Serial Monitor.

  DO THE PAPER EXERCISE IN THE BUILD-STEPS DOC BEFORE WRITING ANY CODE.
  The numbers you work out there go into the TODOs below.

  RULES
    - Tempo is an integer in tenths of a BPM (1200 means 120.0). No floats in
      timing code. (The test harness uses one float to parse what you type.
      That is fine: it is not timing code.)
    - No division inside a handler.
    - Schedule each beat from the previous IDEAL beat time, never from "now".
    - Do not touch Timer0.

  NAMES YOU MAY NEED (depends on the method you chose on paper)
    Everything from step 1, plus:
    Normal mode (WGM = 0), TCNTn, OCRnA / OCRnB / OCRnC
    COMnx1:0 and the "Compare Output Mode, non-PWM" table
    TIFRn and OCFnA (how do you clear a flag?)
    uint32_t arithmetic and the UL suffix on constants

  PASS CHECK
    See the build-steps doc. In short: correct period on the scope at 30.0,
    120.0 and 999.9; no hiccup when the tempo changes; a drift log over 20
    minutes; a jitter number from the scope.
*/

#include <util/atomic.h>

// ---------- your design decisions (from the paper exercise) ----------

const uint32_t TIMER_HZ = 0;        // TODO: counts per second of the timer you chose
const uint16_t TEMPO_MIN = 300;     // tenths of a BPM
const uint16_t TEMPO_MAX = 9999;

const uint8_t CLICK_PIN = 23;       // scope channel 1. If the timer hardware will
                                    // drive the pin, change this to that timer's
                                    // output pin.
const uint8_t PROBE_PIN = 22;       // scope channel 2: high while your handler runs

// ---------- shared between main code and the handler ----------

// TODO: declare what the handler needs in order to schedule the next beat.
//       Mark each one volatile. Put a comment beside every one that is wider
//       than one byte.

volatile uint8_t  beatFlag  = 0;    // handler sets it, loop() clears it
volatile uint32_t beatCount = 0;    // handler increments it once per beat

// ---------- tempo to timing numbers (main code only) ----------

// TODO: write a function that turns a tempo in tenths of a BPM into the
//       number or numbers your method needs. This is where the division lives.
//       Watch for 32-bit overflow in the intermediate product.


void setTempo(uint16_t tempoTenths) {
  // TODO 1: clamp to TEMPO_MIN .. TEMPO_MAX
  // TODO 2: compute the new timing numbers (interrupts stay ON for this)
  // TODO 3: hand them to the handler so it can never see half of an update
}

void setup() {
  Serial.begin(115200);
  pinMode(CLICK_PIN, OUTPUT);
  pinMode(PROBE_PIN, OUTPUT);

  // TODO 4: timer setup. Decide what the very first beat should do:
  //         click at once, or one interval after start?

  setTempo(1200);
}

// TODO 5: the handler. Raise PROBE_PIN on entry and lower it on exit.
//         It must: produce or arm the click, work out when the next beat is
//         due, increment beatCount, and set beatFlag.
//         How does the click pulse END? Decide who is responsible for that.


void loop() {
  // ----- deliberately slow main code: leave it in while you test -----
  delay(37);

  // ----- test harness (provided): type 120.0 or 999.9 and press Enter -----
  if (Serial.available()) {
    float bpm = Serial.parseFloat();
    while (Serial.available()) Serial.read();
    if (bpm > 0) {
      uint16_t t = (uint16_t)(bpm * 10.0 + 0.5);
      setTempo(t);
      Serial.print(F("tempo (tenths) = "));
      Serial.println(t);
    }
  }

  // ----- drift log (provided): turn on "Show timestamp" in the Serial Monitor -----
  // Prints one line each time the beat count passes another multiple of 100.
  static uint32_t nextReport = 0;
  if (beatFlag) {
    beatFlag = 0;
    uint32_t n;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) { n = beatCount; }
    if (n >= nextReport) {
      Serial.print(F("beat "));
      Serial.println(n);
      nextReport = (n / 100 + 1) * 100;
    }
  }
}
