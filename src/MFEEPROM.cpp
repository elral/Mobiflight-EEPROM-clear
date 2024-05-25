//
// MFEEPROM.cpp
//
// (C) MobiFlight Project 2022
//

#include <Arduino.h>
#include "MFEEPROM.h"
#include <EEPROM.h>

#define EEPROM_SIZE 4096

MFEEPROM::MFEEPROM() {}

void MFEEPROM::init(void)
{
#if defined(ARDUINO_ARCH_RP2040)
    EEPROM.begin(EEPROM_SIZE);
#endif
#if defined(ARDUINO_ARCH_STM32)
    eeprom_buffer_fill();
#endif
    _eepromLength = EEPROM.length();
}

uint16_t MFEEPROM::get_length(void)
{
    return _eepromLength;
}

bool MFEEPROM::read_block(uint16_t adr, char data[], uint16_t len)
{
    if (adr + len > _eepromLength) return false;
    for (uint16_t i = 0; i < len; i++) {
        data[i] = read_char(adr + i);
    }
    return true;
}

bool MFEEPROM::write_block(uint16_t adr, char data[], uint16_t len)
{
    if (adr + len > _eepromLength) return false;
    for (uint16_t i = 0; i < len; i++) {
#if defined(ARDUINO_ARCH_STM32)
        eeprom_buffered_write_byte(adr + i, data[i]);
#else
        EEPROM.put(adr + i, data[i]);
#endif
    }
#if defined(ARDUINO_ARCH_RP2040)
    EEPROM.commit();
#endif
#if defined(ARDUINO_ARCH_STM32)
    eeprom_buffer_flush();
#endif
    return true;
}

char MFEEPROM::read_char(uint16_t adr)
{
    if (adr >= _eepromLength) return 0;
#if defined(ARDUINO_ARCH_STM32)
            return eeprom_buffered_read_byte(adr);
#else
    return EEPROM.read(adr);
#endif
}

bool MFEEPROM::write_byte(uint16_t adr, char data)
{
    if (adr >= _eepromLength) return false;
#if defined(ARDUINO_ARCH_STM32)
    eeprom_buffered_write_byte(adr, data);
#else
    EEPROM.put(adr, data);
#endif
#if defined(ARDUINO_ARCH_RP2040)
    EEPROM.commit();
#endif
#if defined(ARDUINO_ARCH_STM32)
    eeprom_buffer_flush();
#endif
    return true;
}

// MFEEPROM.cpp
