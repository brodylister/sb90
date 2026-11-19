/*
  STEP 1 - A heartbeat you can trust                     (blank start, about 1 hour)

  GOAL
    A hardware timer interrupts 1000 times a second. Its handler toggles a pin
    and counts ticks. The main loop below is deliberately slow and messy, and
    the tick must not care.

  RULES
    - Do not use millis(), micros(), tone() or any timer library.
    - Do not touch Timer0 (Arduino's own clock lives there).
    - Leave the slow code in loop() alone.

  NAMES YOU WILL NEED (look them up in the 16-bit timer chapter of the datasheet)
    TCCRnA  TCCRnB  TCNTn  OCRnA  TIMSKn
    WGMn3:0 (which table row is "CTC, TOP = OCRnA"?)
    CSn2:0  (the Clock Select table)
    OCIEnA
    ISR(TIMERn_COMPA_vect)
    PINx    (what does writing a 1 to a PIN register do?)
    volatile
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) { ... }

  PASS CHECK
    1. Scope on PROBE_PIN shows a square wave with edges exactly 1.000 ms apart.
    2. The edges do not move while the main loop prints and delays.
    3. The Serial Monitor prints a tick count once a second, and each count is
       exactly 1000 more than the one before.

  THEN BREAK IT ON PURPOSE
    Add this to loop():   noInterrupts(); delayMicroseconds(300); interrupts();
    Trigger the scope on one edge, zoom in on the next edge, and turn on
    persistence. What do you see, and how wide is it? Remove the line afterwards.
*/

#include <util/atomic.h>

const uint8_t PROBE_PIN = 22;     // PA0 on the Mega. Scope probe goes here.

// TODO: declare the tick counter the handler will increment.
//       How wide should it be if it must not wrap for a whole rehearsal?
volatile uint32_t tick_cnt = 0;
bool ready = 0;
uint32_t i = 0;

void setup() {
  Serial.begin(115200);

  // TODO 1: make PROBE_PIN an output.
  pinMode(PROBE_PIN, OUTPUT);
  // TODO 2: configure a 16-bit timer of your choice for a 1 kHz
  //         compare-match interrupt. Work out the prescaler and TOP on paper
  //         first and write your arithmetic in a comment here.
  
  // Configure timer1: 
  // CS1[2:0] = 0b010 for prescale 8. This gives us 2MHz clock, so TOP = 0d2000.
  // For mode 4 (Clear Timer on Compare (CTC)) WGM1[3:0] = 0b0100 
  noInterrupts();
  TCCR1A = 0; // clears default and sets wgm11 and wgm10 as desired (to zero)
  TCCR1B = 0; // clears default and sets cs12 and cs10 and wgm13 as desired
  TCCR1B |=  (1 << WGM12); // wgm12
  TCCR1B |=  (1 << CS11); // cs11
  OCR1A = 1999; // 2000 cycles (technically TOP = 1999)
  TIMSK1 |=  (1 << OCIE1A); // ocie1a (interrupt) triggers program $0022
  TCNT1 = 0; // timer starts at zero
  interrupts();
}

// TODO 3: write the handler. It should toggle the probe pin and count the tick.
//         Nothing else.

// Specific name for the interrupt we triggered
ISR(TIMER1_COMPA_vect) {
  PINA = (1 << PINA0);
  tick_cnt += 1;
}

void loop() {
  // ----- deliberately slow, messy main code: leave this as it is -----
  delay(37);
  Serial.println(F("main loop is busy doing something slow..."));
  // --------------------------------------------------------------------

  // TODO 4: once a second, measured by YOUR tick count, print the tick count.
  //         Read the counter safely. Why is a plain read not safe here?
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
    i = tick_cnt; // tick_cnt is greater than 1 byte (4). A plain read could get interrupted in the middle of the read,
                  // and if it happens on rollover, we could get a non-existent value.
  }
  if (i % 1000 >= 950) {
      ready = 1;
  }

  while (ready) {
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
      i = tick_cnt;
    }
    if (i % 1000 == 0) {
      Serial.println(i);
      ready = 0;
    }
  }

  noInterrupts();
  delayMicroseconds(400);
  interrupts();
}
