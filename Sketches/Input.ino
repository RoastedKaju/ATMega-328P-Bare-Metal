void setup() {
  Serial.begin(9600);

  DDRD |= (1 << DDD4);

  // input pin mode, 0 in bit 2
  DDRD &= ~(1 << DDD2);
  // set internal pull up by setting 1 to PORTD2
  PORTD |= (1 << PORTD2);
}

void loop() {
  // When you want to read the state of physical pins, you don't use PORT
  // Instead, you use the PIN input registers (like PINB, PIND, etc.).
  if (PIND & (1 << PIND2)) {
    PORTD |= (1 << PORTD4);
  }
  else {
    PORTD &= ~(1 << PORTD4);
  }
}
