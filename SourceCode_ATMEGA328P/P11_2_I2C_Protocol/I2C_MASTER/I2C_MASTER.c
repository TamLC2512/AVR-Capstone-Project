#include <mega328p.h>
#include <io.h>
#include <delay.h>

//dia chi cua SLAVE
#define SLAVE_ADDRESS 0x20

void I2C_START() {
    //gui tin hieu START
    TWCR = (1 << TWSTA) | (1 << TWEN) | (1 << TWINT);
    while (!(TWCR & (1 << TWINT)));
}

void I2C_STOP() {
    //gui tin hieu STOP
    TWCR = (1 << TWSTO) | (1 << TWEN) | (1 << TWINT);
    while (TWCR & (1 << TWSTO));
}

void I2C_WRITE(unsigned int data) {
    TWDR = data;
    TWCR = (1 << TWEN) | (1 << TWINT);
    while (!(TWCR & (1 << TWINT)));
}
void main(void)
{
#pragma optsize-
CLKPR = (1<<CLKPCE);
CLKPR = 0x00;
#ifdef _OPTIMIZE_SIZE_
#pragma optsize+
#endif
//nut nhan chan PA2
    DDRB &= ~(1<<DDB0);
    PORTB |= (1<<PORTB0);

    //toc do giao tiep 100 kHz
    TWSR = 0x00;
    TWBR = 32;

    while (1) {
        if (!(PINB & (1 << PINB0))) {
            delay_ms(50);   //delay chong doi nut nhan
            if (!(PINB & (1 << PINB0))) {
                PORTB ^= (1<<7);
                I2C_START();
                //MASTER gui dia chi SLAVE muon giao tiep
                I2C_WRITE(SLAVE_ADDRESS << 1);
                //MASTER gui gia tri 1
                I2C_WRITE(0x01);
                I2C_STOP();
                delay_ms(200);
            }
        }
    }
}
