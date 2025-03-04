#include <mega328p.h>
#include <io.h>
#include <delay.h>

#define SLAVE_ADDRESS 0x20
unsigned int received_data = 0;

void I2C_INIT_SLAVE() {
    //gan dia chi vao thanh ghi TWAR
    TWAR = (SLAVE_ADDRESS << 1);
    //kich hoat I2C, bit ACK
    TWCR = (1 << TWEN) | (1 << TWEA) | (1 << TWINT);
}
unsigned int I2C_WAIT() {
    while (!(TWCR & (1 << TWINT)));
    return TWSR & 0xF8;  //tra ve 5 bit cao cua TWSR
}
unsigned int I2C_RECEIVE() {
    return TWDR;
}
void I2C_ACKNOWLEDGE() {
    TWCR = (1 << TWEN) | (1 << TWEA) | (1 << TWINT);
}
void I2C_RESET() {
    TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWEA);
}
void main() {
    // Crystal Oscillator division factor: 1
    #pragma optsize-
    CLKPR=(1<<CLKPCE);
    CLKPR= 0x00;
    #ifdef _OPTIMIZE_SIZE_
    #pragma optsize+
    #endif

    //khai bao chan PB0 dieu khien LED
    DDRB |= (1 << DDB0);
    PORTB &= ~(1 << PORTB0);

    I2C_INIT_SLAVE();

    while (1) {
        if (I2C_WAIT() == 0x60) {
            I2C_ACKNOWLEDGE();
        }
        if (I2C_WAIT() == 0x80) {
            received_data = I2C_RECEIVE();
            I2C_ACKNOWLEDGE();
        }
        if (I2C_WAIT() == 0xA0) {
            I2C_RESET();
        }
        if (received_data == 0x01) {
            PORTB ^= (1 << PORTB0);
            delay_ms(50);
            received_data = 0;
        }
    }
}