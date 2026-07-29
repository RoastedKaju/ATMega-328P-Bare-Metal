// DDR is data direction register
// PORT is actual pin, use DDR in setup to set pin modes and use PORT in Loop to change voltage
// bit shift left until you reah the pin number and set that to one then OR or AND.
// DDD2 is simply 2, it is Data direction bit 2.
// PORTD2 is also 2.
// so all you are doing is 00000100 when you do this (1 << DDD2)

// Suppose you want to target pin 13 then that exists on port B, so you would need to set bit 5 like DDB5 of DDRB
// And your ports will become PORTB and PORTB5

void setup() {
  Serial.begin(9600);

  // Set pin mode to output
  // DDR D is the Data direction register for port D which contains values from 0-7
  // Port D, data direction for pin 2
  // 0 means input and for output you need the bit to be 1
  DDRD |= (1 << DDD2);

}

void loop() {
  // To turn on the LED set the pin 2 to HIGH
  Serial.println(PORTD2);
  PORTD |= (1 << PORTD2);
  delay(1000);
  // Set the bit to LOW
  PORTD &= ~(1 << PORTD2);
  delay(1000);
}
