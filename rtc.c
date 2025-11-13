// rtc.c

#include "i2c.h"
#include "rtc.h"

static rtc_time_t rtc_time_buf;

void rtc_time_read_callback(unsigned char *data, unsigned char len, unsigned char status) {
    if (status == I2C_OK) {
        rtc_time_buf.second = bcd_to_dec(data[0]);
        rtc_time_buf.minute = bcd_to_dec(data[1]);
        rtc_time_buf.hour   = bcd_to_dec(data[2]);
    }
    // Call upper-layer cb here if needed (e.g., set via state or function pointer)
}

void rtc_get_time(void (*cb)(rtc_time_t* t)) {
    static unsigned char buffer[3];
    i2c_request_t req = {
        .slave_addr = BU9873_I2C_ADDR,
        .reg = BU9873_REG_SECOND,
        .buf = buffer,
        .len = 3,
        .rw = I2C_RW_READ,
        .callback = rtc_time_read_callback
    };
    i2c1_submit(&req);
    // Optionally, store cb for later use to call upper layers
}

void rtc_set_time(rtc_time_t* t, void (*cb)(unsigned char status)) {
    static unsigned char buffer[3];
    buffer[0] = dec_to_bcd(t->second);
    buffer[1] = dec_to_bcd(t->minute);
    buffer[2] = dec_to_bcd(t->hour);
    i2c_request_t req = {
        .slave_addr = BU9873_I2C_ADDR,
        .reg = BU9873_REG_SECOND,
        .buf = buffer,
        .len = 3,
        .rw = I2C_RW_WRITE,
        .callback = cb // your write-done handler
    };
    i2c1_submit(&req);
}

// Helper functions
static unsigned char dec_to_bcd(unsigned char d) { return ((d / 10) << 4) | (d % 10); }
static unsigned char bcd_to_dec(unsigned char b) { return ((b >> 4) * 10) + (b & 0x0F); }
