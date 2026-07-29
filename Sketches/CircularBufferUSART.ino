#include <ctype.h>
#include <string.h>

const int BUFFER_SIZE = 64;

struct RingBuffer
{
  char buffer[BUFFER_SIZE];
  size_t head = 0;
  size_t tail = 0;
  size_t count = 0;
  bool full = false;

  void push(char c)
  {
    if (full)
    {
      tail = (tail + 1) % BUFFER_SIZE;
    }
    else
    {
      ++count;
    }

    buffer[head] = c;

    head = (head + 1) % BUFFER_SIZE;
    full = (head == tail);
  }

  bool pop(char &c)
  {
    if (empty())
    {
      return false;
    }

    c = buffer[tail];
    full = false;
    tail = (tail + 1) % BUFFER_SIZE;
    --count;

    return true;
  }

  bool empty()
  {
    return (!full && (head == tail));
  }

  void reset() {
    head = 0;
    tail = 0;
    count = 0;
    full = false;
  }
};

RingBuffer rxBuffer{};
RingBuffer txBuffer{};

void uart_init() {
  // turn on transmitters and set interrupt flags
  UCSR0B =  (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);
  // agree on data format, size and paraity
  // UMSELn1 = 0, UMSELn0 = 0 for Async
  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // 8 bit
  UCSR0C &= ~(1 << USBS0); // 1 bit stop
  // set baud rate
  UBRR0H = 0;
  UBRR0L = 103;
}

void uart_write(const char* str) {
  bool start = txBuffer.empty();  // check if we need to kick off

  while (*str != '\0') {
    txBuffer.push(*str);
    ++str;
  }

  if (start) {
    char c;
    if (txBuffer.pop(c)) {
      UDR0 = c;  // kick off transmission
    }
  }

  UCSR0B |= (1 << UDRIE0);  // enable interrupt
}


void setup()
{
  uart_init();
  sei();

  uart_write("Hello World\n");
  delay(3000);
  uart_write("Hello World\n");
}

ISR(USART_RX_vect) {
  char c = UDR0;

  // store it in our ring buffer
  if (c == '\n' || c == '\r') {
    rxBuffer.push('\0');

    // Print the collected string back
    uart_write("You sent: ");
    uart_write(rxBuffer.buffer);
    uart_write("\r\n");

    rxBuffer.reset();
  }
  else {
    rxBuffer.push(c);
  }
}

ISR(USART_UDRE_vect) {
  char c;
  if (txBuffer.empty()) {
    UCSR0B &= ~(1 << UDRIE0);
  } else {
    txBuffer.pop(c);
    UDR0 = c;
  }
}


void loop() {}
