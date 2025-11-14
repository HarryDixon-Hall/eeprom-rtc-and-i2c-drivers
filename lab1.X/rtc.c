// rtc.c

#include "i2c.h"
#include "rtc.h"

static rtc_datetime_t rtc_dt_buf;
static void (*rtc_user_cb)(rtc_datetime_t *dt) = 0; // Function pointer for user callback

static void rtc_read_datetime_callback(unsigned char *data, unsigned char len, unsigned char status) {
    if (status == I2C_OK) {
        rtc_dt_buf.second = bcd_to_dec(data[0]);
        rtc_dt_buf.minute = bcd_to_dec(data[1]);
        rtc_dt_buf.hour   = bcd_to_dec(data[2]);
        rtc_dt_buf.weekday= bcd_to_dec(data[3]);
        rtc_dt_buf.day    = bcd_to_dec(data[4]);
        rtc_dt_buf.month  = bcd_to_dec(data[5]);
        rtc_dt_buf.year   = bcd_to_dec(data[6]);
        // Pass pointer to filled struct to main/app
        if (rtc_user_cb) rtc_user_cb(&rtc_dt_buf);
        rtc_user_cb = 0; // Optionally clear to prevent re-use
    }
}

// Now this takes the user callback and stores it for later
void rtc_read_datetime(void (*cb)(rtc_datetime_t *dt)) {
    static unsigned char buffer[7];
    rtc_user_cb = cb; // Store the user?s function to call later
    i2c_request_t req = {
        .slave_addr = BU9873_I2C_ADDR,
        .reg = 0x00,
        .buf = buffer,
        .len = 7,
        .rw = I2C_RW_READ,
        .callback = rtc_read_datetime_callback
    };
    I2C1_Submit(&req);
}

static void (*rtc_datetime_write_user_cb)(unsigned char status) = 0;

// Internal adapter: called by I2C when write completes
static void rtc_datetime_write_callback(unsigned char *data, unsigned char len, unsigned char status) {
    if (rtc_datetime_write_user_cb) {
        rtc_datetime_write_user_cb(status);  // Call user-provided callback with completion status
        rtc_datetime_write_user_cb = 0;      // Optionally clear after use for safety
    }
}

void rtc_write_datetime(unsigned char addr, rtc_datetime_t* dt, void (*cb)(unsigned char status)) {
    static unsigned char buffer[7];
    buffer[0] = dec_to_bcd(dt->second);
    buffer[1] = dec_to_bcd(dt->minute);
    buffer[2] = dec_to_bcd(dt->hour);
    buffer[3] = dec_to_bcd(dt->weekday);
    buffer[4] = dec_to_bcd(dt->day);
    buffer[5] = dec_to_bcd(dt->month);
    buffer[6] = dec_to_bcd(dt->year);

    rtc_datetime_write_user_cb = cb; // Save user's callback pointer

    i2c_request_t req = {
        .slave_addr = BU9873_I2C_ADDR,
        .reg = addr,
        .buf = buffer,
        .len = 7,
        .rw = I2C_RW_WRITE,
        .callback = rtc_datetime_write_callback // Adapter
    };
    I2C1_Submit(&req);
}




// Helper functions
static unsigned char dec_to_bcd(unsigned char d) { return ((d / 10) << 4) | (d % 10); }
static unsigned char bcd_to_dec(unsigned char b) { return ((b >> 4) * 10) + (b & 0x0F); }