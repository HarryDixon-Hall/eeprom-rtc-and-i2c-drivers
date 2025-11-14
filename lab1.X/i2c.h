#ifndef I2C_H
#define I2C_H

#include <xc.h>

// I2C transaction direction
#define I2C_RW_WRITE 0
#define I2C_RW_READ  1

// I2C status codes
#define I2C_OK    0 //data still to come
#define I2C_NACK  1 //no more data to come
#define I2C_ERR   2 //i2c communication failure

// Transaction request structure
typedef struct {
    unsigned char slave_addr; // 7-bit device address
    unsigned char reg;        // Device register or memory pointer
    unsigned char *buf;       // Pointer to data buffer
    unsigned char len;        // Number of bytes in buffer
    unsigned char rw;         // I2C_RW_WRITE or I2C_RW_READ
    void (*callback)(unsigned char *data, unsigned char len, unsigned char status);
} i2c_request_t;

// API structure were i2c takes jobs from main and drivers
//main loop will continue cycle while i2c handles interrupt job
void I2C1_Init_MasterMode(unsigned long i2c_hz);
unsigned char I2C1_Submit(const i2c_request_t *request);
void __interrupt() I2C_ISR(void);

#endif