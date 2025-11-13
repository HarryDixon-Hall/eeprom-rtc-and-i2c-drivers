#include <xc.h> // include processor files - each processor file is guarded.
// eeprom_cfg.h - EEPROM configuration storage

// EEPROM Memory Map (addresses 0x00 to 0xFF available)
#define EEPROM_ADDR_TEMP_LOWER      0x00    // Lower temperature limit
#define EEPROM_ADDR_TEMP_UPPER      0x01    // Upper temperature limit
#define EEPROM_ADDR_WINDOW_START_H  0x02    // Heating window start hour
#define EEPROM_ADDR_WINDOW_START_M  0x03    // Heating window start minute
#define EEPROM_ADDR_WINDOW_END_H    0x04    // Heating window end hour
#define EEPROM_ADDR_WINDOW_END_M    0x05    // Heating window end minute
#define EEPROM_ADDR_WINDOW_ENABLED  0x06    // Window enabled flag
#define EEPROM_ADDR_ALARM_HOUR      0x07    // Alarm hour
#define EEPROM_ADDR_ALARM_MINUTE    0x08    // Alarm minute
#define EEPROM_ADDR_ALARM_ENABLED   0x09    // Alarm enabled flag
#define EEPROM_ADDR_ALARM_TONE      0x0A    // Selected tone (0-1) TWO MELODIES
#define EEPROM_ADDR_HEATING_ENABLED 0x0B    // Heating system ON/OFF
#define EEPROM_ADDR_INIT_FLAG       0xFF    // EEPROM Initialization flag (0xAA = initialized)

#define EEPROM_INIT_SIGNATURE   0xAA        // Signature to check if EEPROM initialized

#define BU9873_I2C_ADDR         0x50 //Slave address of external eeprom FT24C08A

// Configuration structure
typedef struct {
    unsigned char temp_lower;       // Lower temperature threshold (e.g., 22)
    unsigned char temp_upper;       // Upper temperature threshold (e.g., 26)
    unsigned char htime_upper;      // End for heating time window 
    unsigned char alarm_hour;       // Alarm hour (0-23)
    unsigned char alarm_minute;     // Alarm minute (0-59)
    unsigned char alarm_enabled;    // Alarm ON/OFF
    unsigned char alarm_tone;       // Tone selection (0-2)
    unsigned char heating_enabled;  // Heating system ON/OFF
} SystemConfig;

// Function prototypes
void EEPROM_Init_Check(void);
void EEPROM_Save_Config(SystemConfig *config);
void EEPROM_Load_Config(SystemConfig *config);
void EEPROM_Set_Defaults(SystemConfig *config);
void EEPROM_Write_Byte(unsigned char address, unsigned char data);
unsigned char EEPROM_Read_Byte(unsigned char address);

#endif // EEPROM_CFG_H
