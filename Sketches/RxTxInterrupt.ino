const unsigned int buffer_size = 64;
char tx_buffer[buffer_size];
volatile uint8_t tx_buffer_index = 0;
volatile uint8_t tx_tail = 0;

char rx_buffer[buffer_size];
volatile uint8_t rx_buffer_index = 0;

void echo(const char* msg)
{
  while (*msg != '\0' && tx_buffer_index < buffer_size)
  {
    tx_buffer[tx_buffer_index++] = *msg++;
  }

  // If transmitter idle, kick it off
  if (!(UCSR0B & (1 << UDRIE0)))
  {
    tx_tail = 0;
    UDR0 = tx_buffer[tx_tail++];
    UCSR0B |= (1 << UDRIE0);
  }
}

ISR(USART_UDRE_vect)
{
  if (tx_tail < tx_buffer_index)
  {
    UDR0 = tx_buffer[tx_tail++];
  }
  else
  {
    tx_buffer_index = 0;
    tx_tail = 0;
    UCSR0B &= ~(1 << UDRIE0); // disable interrupt
  }
}

ISR(USART_RX_vect)
{
  char rx_byte = UDR0;
  rx_buffer[rx_buffer_index] = rx_byte;

  if (rx_byte == '\r' || rx_byte == '\n')
  {
    rx_buffer[rx_buffer_index] = '\0';
    echo("You sent: ");
    echo(rx_buffer);
    echo("\r\n");
    rx_buffer_index = 0;
  }
  else
  {
    ++rx_buffer_index;
    if (rx_buffer_index >= buffer_size - 1) // leave space for '\0'
    {
      rx_buffer_index = 0;
    }
  }
}

void setup()
{
  // baud rate
  UBRR0H = 0;
  UBRR0L = 103;

  // Enable receiver and transmitter
  UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);
  UCSR0C = 0;

  // format
  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
  UCSR0C &= ~(1 << USBS0);

  echo("Hello World!\n");
}

void loop()
{

}