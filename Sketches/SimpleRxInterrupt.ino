
void USART_TX(char c) {
  while (!(UCSR0A & (1 << UDRE0)));
  UDR0 = c;
}

void USART_print(const char* str) {
  while (*str != '\0') {
    USART_TX(*str);
    str++;
  }
}

void setup() {
  // turn on transmitters and set interrupt flag
  UCSR0B =  (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);
  // agree on data format, size and paraity
  // UMSELn1 = 0, UMSELn0 = 0 for Async
  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // 8 bit
  UCSR0C &= ~(1 << USBS0); // 1 bit stop
  // set baud rate
  UBRR0H = 0;
  UBRR0L = 103;

  delay(1000);

  USART_print("Hello World!\n");
}

const uint8_t BUFFER_SIZE = 64;
char rxBuffer[BUFFER_SIZE];
volatile uint8_t bufferIndex = 0;

ISR(USART_RX_vect) {
  char c = UDR0;

  // store it in our ring buffer
  if (c == '\n' || c == '\r') {
    rxBuffer[bufferIndex] = '\0'; // null terminate

    // Print the collected string back
    USART_print("You sent: ");
    USART_print(rxBuffer);
    USART_print("\r\n");

    // Reset the buffer index for the next message
    bufferIndex = 0;
  }
  else {
    if (bufferIndex < BUFFER_SIZE - 1) {
      rxBuffer[bufferIndex] = c;
      bufferIndex++;
    }
  }
}

void loop() {


}
