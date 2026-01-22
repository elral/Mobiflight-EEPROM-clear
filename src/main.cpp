
#include <Arduino.h>
#include "MFEEPROM.h"

#define CLEARBYTE 0xFF;
#ifdef ARDUINO_AVR_PROMICRO16
#define TXLED0			PORTD &= ~(1<<5)
#define TXLED1			PORTD |= (1<<5)
#define RXLED0			PORTB &= ~(1<<0)
#define RXLED1			PORTB |= (1<<0)
#define TX_RX_LED_INIT	DDRD |= (1<<5), DDRB |= (1<<0), TXLED0, RXLED0
#endif

MFEEPROM MFeeprom;

void setup()
{
    MFeeprom.init();
    uint16_t bufferlength = MFeeprom.get_length();
    char     buffer[bufferlength];
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(115200);
    for (uint16_t i = 0; i < bufferlength; i++) {
        buffer[i] = CLEARBYTE;
    }
    if (MFeeprom.write_block(0, buffer, bufferlength)) {
    }
#if defined(ARDUINO_ARCH_RP2040) &&(defined PICO_RP2350A)
    rp2040.rebootToBootloader();
#endif
}

void loop()
{
#ifdef ARDUINO_AVR_PROMICRO16
  TXLED0;
  RXLED0;
  delay(1000);
  TXLED1;
  RXLED1;
  delay(1000);
#else
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
#endif
}
