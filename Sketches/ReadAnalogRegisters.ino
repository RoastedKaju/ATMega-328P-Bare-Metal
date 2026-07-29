void setup() {
  Serial.begin(9600);

  // Set A0 as input
  DDRC &= ~(1 << DDC0);

  // Configure ADC
  ADMUX = (1 << REFS0);              // Reference = AVcc, channel = ADC0 (A0)
  ADCSRA = (1 << ADEN)               // Enable ADC
         | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Prescaler = 128
}

uint16_t readADC(uint8_t channel) {
  // Select channel (0–5 for A0–A5)
  ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);

  // Start conversion
  ADCSRA |= (1 << ADSC);

  // Wait until conversion finishes
  while (ADCSRA & (1 << ADSC));

  // Read result (10-bit)
  // return (ADCL | (ADCH << 8));
  // Read the 16-bit ADC register directly (handles ADCL/ADCH order safely)
  return ADC;
}

void loop() {
  uint16_t value = readADC(0);   // Read A0
  Serial.println(value);         // 0–1023
  delay(200);
}
