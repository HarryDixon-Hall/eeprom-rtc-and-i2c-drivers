#include <xc.h>

// Register addresses (same as before)
#define BU9873_REG_SECOND       0x00
#define BU9873_REG_MINUTE       0x01
#define BU9873_REG_HOUR         0x02

// Control register bits
#define BU9873_CTRL_1           0x0E
#define BU9873_CTRL_2           0x0F
#define BU9873_CLOCK_ENABLE     0x19
#define BU9873_CTRL_12_24       0x20

#define BU9873_I2C_ADDR         0x32 //Slave address of BU9873 for I2C communication

typedef struct {
    unsigned char second;
    unsigned char minute;
    unsigned char hour;
} rtc_time_t;

//RTC (BU9873/rtc16) methods
static unsigned char dec_to_bcd(unsigned char d);
static unsigned char bcd_to_dec(unsigned char b);

void rtc_write_reg(unsigned char reg, unsigned char val);
void rtc_read_reg(unsigned char reg, unsigned char* buf, unsigned char len);
void rtc_default_config(void);
void rtc_SetTime(rtc_time_t* t);
void rtc_GetTime(rtc_time_t& t);

