void setup() {
  Serial.begin(9600);

  // configure pin 4 and 7
  DDRD |= (1 << DDD4);
  DDRD |= (1 << DDD7);
}

void loop() {
  PORTD |= (1 << PORTD4);
  PORTD &= ~(1 << PORTD7);
  delay(500);
  PORTD |= (1 << PORTD7);
  PORTD &= ~(1 << PORTD4);
  delay(500);
  PORTD |= (1 << PORTD4);
  PORTD |= (1 << PORTD7);
  delay(200);
  PORTD &= ~(1 << PORTD4);
  PORTD &= ~(1 << PORTD7);
  delay(200);
}
