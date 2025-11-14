#include "i2c.h"

#define _XTAL_FREQ 16000000
#define I2C_QUEUE_SIZE 4

static volatile i2c_request_t queue[I2C_QUEUE_SIZE];
static volatile unsigned char qhead = 0, qtail = 0, qbusy = 0;
static volatile i2c_request_t *cur = 0;
static volatile unsigned char idx = 0;
static volatile unsigned char rx_buffer[32];

void I2C1_Init_MasterMode(unsigned long clock_freq) {
    SSP1CON1 = 0b00101000; // Enable I2C Master mode
    SSP1STAT = 0b10000000;
    SSP1CON2 = 0x00;
    SSP1ADD = (_XTAL_FREQ / (4 * clock_freq)) - 1;
    TRISCbits.TRISC3 = 1;
    TRISCbits.TRISC4 = 1;
    ANSELCbits.ANSC3 = 0;
    ANSELCbits.ANSC4 = 0;
    PIE1bits.SSP1IE = 1;
    PIR1bits.SSP1IF = 0;
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
}

unsigned char I2C1_Submit(const i2c_request_t *request) {
    unsigned char next = (qhead + 1) % I2C_QUEUE_SIZE;
    if (next == qtail) return 1; // Queue full
    queue[qhead] = *request;
    qhead = next;
    if (!qbusy) {
        qbusy = 1;
        cur = &queue[qtail];
        idx = 0;
        PIR1bits.SSP1IF = 1; // force ISR entry
    }
    return 0;
}

void __interrupt() I2C_ISR(void) {
    if (!PIR1bits.SSP1IF) return;
    PIR1bits.SSP1IF = 0;

    if (cur == 0) return;

    // WRITE transaction
    if (cur->rw == I2C_RW_WRITE) {
        if (idx == 0)        SSP1CON2bits.SEN = 1; // Start
        else if (idx == 1)   SSP1BUF = (cur->slave_addr << 1) | 0; // Write
        else if (idx == 2)   SSP1BUF = cur->reg;
        else if (idx - 3 < cur->len)  SSP1BUF = cur->buf[idx - 3];
        else {
            SSP1CON2bits.PEN = 1; // Stop
            if (cur->callback)  cur->callback(0, 0, I2C_OK);
            qtail = (qtail + 1) % I2C_QUEUE_SIZE;
            cur = (qtail != qhead) ? &queue[qtail] : 0;
            idx = 0;
            qbusy = (cur != 0);
            return;
        }
        idx++;
    // READ transaction
    } else if (cur->rw == I2C_RW_READ) {
        if (idx == 0)        SSP1CON2bits.SEN = 1;
        else if (idx == 1)   SSP1BUF = (cur->slave_addr << 1) | 0;
        else if (idx == 2)   SSP1BUF = cur->reg;
        else if (idx == 3)   SSP1CON2bits.RSEN = 1; // Restart
        else if (idx == 4)   SSP1BUF = (cur->slave_addr << 1) | 1; // Read
        else if (idx - 5 < cur->len) {
            SSP1CON2bits.RCEN = 1; // Enable receive
            while (!SSP1STATbits.BF);
            rx_buffer[idx - 5] = SSP1BUF;
            SSP1CON2bits.ACKDT = ((idx - 5) < (cur->len - 1)) ? 0 : 1;
            SSP1CON2bits.ACKEN = 1;
        }
        else {
            SSP1CON2bits.PEN = 1;
            for (unsigned char i=0; i<cur->len; i++) cur->buf[i] = rx_buffer[i];
            if (cur->callback)  cur->callback(cur->buf, cur->len, I2C_OK);
            qtail = (qtail + 1) % I2C_QUEUE_SIZE;
            cur = (qtail != qhead) ? &queue[qtail] : 0;
            idx = 0;
            qbusy = (cur != 0);
            return;
        }
        idx++;
    }
}