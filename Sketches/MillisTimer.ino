#include <avr/io.h>
#include <avr/interrupt.h>

volatile unsigned long system_millis = 0;

// Timer0 CTC, fires every 1 ms
ISR(TIMER0_COMPA_vect) {
    system_millis++;
}

void initTimer0() {
    // Set Timer0 to CTC mode (Clear Timer on Compare Match)
    TCCR0A = (1 << WGM01);
    
    // Set prescaler to 64 and start the timer
    TCCR0B = (1 << CS01) | (1 << CS00);
    
    // Load the compare value for 1ms intervals (249)
    OCR0A = 249;
    
    // Enable Timer0 Compare Match A interrupt
    TIMSK0 = (1 << OCIE0A);
    
    // Enable global interrupts
    sei();
}

// Custom millis() function ensuring atomic read of 32-bit variable
unsigned long custom_millis() {
    unsigned long m;
    cli(); // Clear interrupts temporarily to safely read 4 bytes
    m = system_millis;
    sei(); // Restore interrupts
    return m;
}

// Custom non-blocking style delay wrapper
void custom_delay(unsigned long ms) {
    unsigned long start = custom_millis();
    while (custom_millis() - start < ms) {
        // Wait loop
    }
}

void setup() {
    Serial.begin(9600);
    initTimer0(); // Initialize our custom hardware timer
}

void loop() {
    unsigned long currentMillis = custom_millis();
    Serial.print("Uptime (ms): ");
    Serial.println(currentMillis);
    
    custom_delay(1000); // Wait 1 second using our custom driver
}