const uint16_t time_compare = 31250;

// The ATmega328P has three hardware timers
// Timer 0 (8-bit), Timer 1 (16-bit), and Timer 2 (8-bit).
// They run independently of your loop() code in the background.
void setup() {
  Serial.begin(9600);

  // configure pin 9 as output
  DDRB |= (1 << DDB1);

  TCCR1A = 0;
  TCCR1B = 0;

  // Set Timer 1 to CTC Mode (Clear Timer on Compare Match)
  // WGM12 bit turns on CTC mode
  TCCR1B |= (1 << WGM12);

  // 256 prescaler
  TCCR1B |= (1 << CS12);
  TCCR1B &= ~(1 << CS11);
  TCCR1B &= ~(1 << CS10);

  // set timer count to 0 and compare value to our number
  TCNT1 = 0;
  OCR1A = time_compare;

  // enable timer interrupt
  TIMSK1 |= (1 << OCIE1A);

  // enable global interrupts
  sei();
}

ISR(TIMER1_COMPA_vect) {
  // flip
  PORTB ^= (1 << PORTB1);
}

void loop() {
  delay(500);
}
