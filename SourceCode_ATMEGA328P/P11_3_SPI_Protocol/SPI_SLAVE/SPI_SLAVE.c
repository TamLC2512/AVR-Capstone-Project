#include <mega328p.h>
#include <alcd.h>

#define MOSI    3
#define MISO    4
#define SCK     5
#define SS      2

int count;

void SPI_Slave_Init(){
//    SPCR = (1<<SPIE) | (1<<SPE);
    SPCR = (1<<SPE);

    DDRB &= ~ ((1<<MOSI)|(1<<SCK)|(1<<SS)); //MOSI, SCK, SS= Input
    DDRB |= (1<<MISO);                      //MISO = Output
}

char SPI_Slave_Receive(){
    while(!(SPSR & (1<<SPIF)));             //Hoan thanh nhan
    return (SPDR);                          //tra ve gia tri nhan
}

void main(void)
{
// Crystal Oscillator division factor: 1
#pragma optsize-
CLKPR=(1<<CLKPCE);
CLKPR=0x00;
#ifdef _OPTIMIZE_SIZE_
#pragma optsize+
#endif
lcd_init(16);

SPI_Slave_Init();
lcd_gotoxy(2,0);
lcd_putsf("SPI Tutorial");
lcd_gotoxy(5,1);
lcd_putsf("Data:");

while (1)
      {
        count = SPI_Slave_Receive();
        lcd_gotoxy(5,1);
        lcd_putsf("Data:");
        lcd_putchar(count);
      }
}
