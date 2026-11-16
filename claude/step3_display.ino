/*
  STEP 3 - The display on the tick             (this becomes the real project sketch)

  HOW TO START
    1. Save a copy of your finished step 2 sketch under the name "metronome".
       From now on that is the project. Every later step adds to it.
    2. Add the 1 kHz tick from step 1 to it, on a different timer from the
       beat. Type it again rather than pasting: you should need the datasheet
       less this time.
    3. Copy the pieces below into it and fill them in.

  GOAL
    Four digits show the tempo with one decimal place. A fifth digit shows the
    beat number. Nothing flickers, nothing ghosts, and the beat from step 2 is
    as accurate as it was before.

  RULES
    - scanStep() lights ONE digit and returns. No loops over digits, no delays.
    - scanStep() only copies bytes that are already prepared. All arithmetic
      on the tempo happens in showTempo(), in main code.
    - Use port writes for the segments.
    - No display library.

  NAMES YOU WILL NEED
    DDRx  PORTx                       (I/O-Ports chapter)
    the Mega pin mapping table        (which header pins make one whole port?)
    bit masks:  |=   &= ~   <<
    / and %  on uint16_t              (main code only)

  WIRING DECISIONS TO MAKE FIRST (write them in the comment block below)
    - Which port carries segments a, b, c, d, e, f, g and the decimal point?
    - Which pins switch the five digits, and are they on a port that a handler
      can change safely? (Field guide, chapter 8, "non-atomic ports".)
    - Is your display common cathode or common anode? What level lights a
      segment, and what level selects a digit?
    - What resistor value, and what is the worst-case current in a digit pin?

  PASS CHECK
    See the build-steps doc.
*/

// ---------- wiring (fill in) ----------
//   segments a..g, dp  ->  port ___ bits 0..7  = Arduino pins ___ ___ ___ ___ ___ ___ ___ ___
//   digit 0 (leftmost) ->  pin ___      digit 1 -> pin ___      digit 2 -> pin ___
//   digit 3            ->  pin ___      beat digit -> pin ___
//   resistors: ___ ohms on each segment line

const uint8_t NUM_DIGITS = 5;             // 4 tempo digits + 1 beat digit

// One byte per numeral: which segments are lit. Bit 0 = a ... bit 6 = g.
// TODO: fill in the ten patterns for YOUR wiring and polarity.
const uint8_t SEG_PATTERN[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

const uint8_t SEG_DP    = 0;              // TODO: the decimal-point bit
const uint8_t SEG_BLANK = 0;              // TODO: the pattern for "all segments off"

// What the display should show right now, already converted to segment bytes.
// Main code writes it; the tick handler reads it.
volatile uint8_t frame[NUM_DIGITS];

void displaySetup() {
  // TODO: segment port and digit pins as outputs, everything off.
}

// Called from the 1 kHz tick handler, once per tick (or once per N ticks:
// decide what scan rate you want and justify it).
void scanStep() {
  // TODO, in this order:
  //   1. all digits off
  //   2. put the next digit's byte from frame[] on the segment port
  //   3. that digit on
  //   4. remember which digit comes next
}

// Main code only. tempoTenths = 1200 should show "120.0".
void showTempo(uint16_t tempoTenths) {
  // TODO: split into four digits, look up patterns, add the decimal point.
  //       Decide what 60.0 looks like: leading zero or blank?
  //       Write the results into frame[0..3].
}

// Main code only. beat = 1..9.
void showBeat(uint8_t beat) {
  // TODO: write one pattern into frame[4].
}

/*
  TEST HARNESS for loop() (provided). Paste it into loop() while testing; it
  replaces the Serial tempo entry from step 2 for now. It walks the display
  through every tempo quickly, with a slow main loop, so a dropped or frozen
  digit is easy to spot.

      static uint16_t t = 300;
      static uint8_t  b = 1;
      delay(40);                       // slow main loop on purpose
      t += 7;  if (t > 9999) t = 300;
      b += 1;  if (b > 9)    b = 1;
      setTempo(t);
      showTempo(t);
      showBeat(b);
*/
