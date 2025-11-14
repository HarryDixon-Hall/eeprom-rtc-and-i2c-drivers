#include <xc.h>

// Register addresses (same as before)
#define BU9873_REG_SECOND       0x00 //0-59 in BCD
#define BU9873_REG_MINUTE       0x01 //0-59 in BCD
#define BU9873_REG_HOUR         0x02 //0-23 in BCD or 1-12 in BCD depending 12/24 mode
#define BU9873_REG_WEEKDAY      0x03 //0-6 mon-sun
#define BU9873_REG_DAY          0x04 //1-31 whole month
#define BU9873_REG_MONTH        0x05 //1-12
#define BU9873_REG_YEAR         0x06 //00-99  (as in 2000 to 2099)


// Control register bits
#define BU9873_CTRL_1           0x0E //Enable alarm A/B + CT rate
#define BU9873_CTRL_2           0x0F //12/24 mode + clock enable

#define BU9873_I2C_ADDR         0x32 //Slave address of BU9873 for I2C communication

typedef struct {
    unsigned char second;
    unsigned char minute;
    unsigned char hour;
    unsigned char weekday;
    unsigned char day;
    unsigned char month;
    unsigned char year;
} rtc_datetime_t;

//RTC (BU9873/rtc16) methods
static unsigned char dec_to_bcd(unsigned char d);
static unsigned char bcd_to_dec(unsigned char b);

void rtc_write_reg(unsigned char reg, unsigned char val);
void rtc_read_reg(unsigned char reg, unsigned char* buf, unsigned char len);
void rtc_default_config(void);
void rtc_SetTime(rtc_datetime_t* t);
void rtc_GetTime(rtc_datetime_t* t);