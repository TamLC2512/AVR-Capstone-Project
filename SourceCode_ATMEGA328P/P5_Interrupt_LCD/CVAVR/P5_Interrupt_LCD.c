#include <mega328p.h>
#include <alcd.h>
#include <delay.h>
#include <stdlib.h>

unsigned char counter = 0;
unsigned char myStr[3];

interrupt [EXT_INT0] void ext_int0_isr(void)
{
    counter++;    //dem len sau moi lan ngat
    itoa(counter, myStr);
    lcd_gotoxy(0,1);
    lcd_puts("Counter:");
    lcd_puts(myStr);
}

void main(void)
{
    #pragma optsize-
    CLKPR=(1<<CLKPCE);
    CLKPR=0x00;
    #ifdef _OPTIMIZE_SIZE_
    #pragma optsize+
    #endif

    lcd_init(16);
    lcd_gotoxy(3,0);
    lcd_puts("INTERRUPT");
    lcd_gotoxy(0,1);
    lcd_puts("Counter:");
    itoa(counter, myStr);
    lcd_puts(myStr);

  // su dung chan PD2 de doc nut nhan
  // cau hinh dien tro keo len chan PD2
    PORTD |= (1<<PORTD2);   //PORTD.2 = 1;
    DDRD  &= ~(1<<DDD2);    //DDRD.2 = 0;

  //su dung ngat 0, lay canh xuong
    EICRA = (1<<ISC01) | (0<<ISC00);
    EIMSK = (0<<INT1) | (1<<INT0);
    EIFR  = (0<<INTF1) | (1<<INTF0);

    #asm("sei")
    while (1){}
}
