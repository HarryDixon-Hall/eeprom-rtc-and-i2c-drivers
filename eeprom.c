// eeprom.c

#include "i2c.h"
#include "eeprom.h"

static unsigned char eeprom_read_buffer;

void eeprom_read_callback(unsigned char *data, unsigned char len, unsigned char status) {
    if (status == I2C_OK) {
        // Data is in data[0]
        // Call upper-layer callback, e.g.,
        // user_cb(data[0], status);
    }
}

void eeprom_read_byte(unsigned char addr, void (*cb)(unsigned char value, unsigned char status)) {
    i2c_request_t req = {
        .slave_addr = 0x50, // Use FT24C08A address
        .reg = addr,
        .buf = &eeprom_read_buffer,
        .len = 1,
        .rw = I2C_RW_READ,
        .callback = eeprom_read_callback // adapt to call user cb
    };
    I2C1_Submit(&req);
}

void eeprom_write_callback(unsigned char *data, unsigned char len, unsigned char status) {
    if (status == I2C_OK) {
        // Write is done; you can call an upper-layer callback here if needed
        // user_cb(status);
    }
}

void eeprom_write_byte(unsigned char addr, unsigned char value, void (*cb)(unsigned char status)) {
    static unsigned char buf[1];
    buf[0] = value;
    i2c_request_t req = {
        .slave_addr = 0x50, // FT24C08A base I2C address
        .reg = addr,
        .buf = buf,
        .len = 1,
        .rw = I2C_RW_WRITE,
        .callback = eeprom_write_callback // adapt to call user cb as needed
    };
    I2C1_submit(&req);
}
