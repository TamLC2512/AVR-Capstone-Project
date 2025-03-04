#include <mega328p.h>
#include <alcd.h>
#include <stdio.h>

unsigned char counter = 0;
int value = 0;
char myStr[10];
interrupt [TIM0_OVF] void timer0_overflow_isr(void)
{
    TCNT0 = 60;          //dat lai gia tri cho bo dem
    if(counter > 4){
        PORTD ^= (1<<PORTD0);//dao trang thai chan LED
        counter = 0;
    }
    else{
        counter++;
    }
}
interrupt [TIM1_COMPA] void timer1_compareA_isr(void)
{
    value++;
    sprintf(myStr, "%d", value);
    lcd_gotoxy(10,1);
    lcd_puts(myStr);
}
void main(void)
{
    #pragma optsize-
    CLKPR =(1<<CLKPCE);
    CLKPR = 0x00;
    #ifdef _OPTIMIZE_SIZE_
    #pragma optsize+
    #endif

    DDRD.0 = 1;     //ngo ra dieu khien LED
    PORTD.0 = 0;    //LED tat
  //Delay dung Timer 0
  //che do Normal, bo chia 1024
    TCCR0A=(0<<WGM01) | (0<<WGM00);
    TCCR0B=(0<<WGM02) | (1<<CS02) | (0<<CS01) | (1<<CS00);
  //gia tri ban dau cua bo dem
    TCNT0=60;

    TIMSK0 |= (1<<TOIE0);  //cho phep ngat khi xay ra tran

  //Dem xung dung Timer 1
  //che do CTC, xung ngoai, lay canh xuong
    DDRD.5 = 0;
    PORTD.5 = 0;
    TCCR1A=(0<<WGM11) | (0<<WGM10);
    TCCR1B=(1<<WGM12) | (1<<CS12) | (1<<CS11) | (0<<CS10);
  //gia tri ban dau cua bo dem bang 0
    TCNT1H=0x00;
    TCNT1L=0x00;
  //dem den 10 se xay ra ngat
    OCR1AH=0x00;
    OCR1AL=0x10;
  //cho phep ngat khi xay ra so sanh
    TIMSK1 |= (1<<OCIE1A);

    lcd_init(16);
    lcd_gotoxy(3,0);
    lcd_puts("Counter 1");
    lcd_gotoxy(3,1);
    lcd_puts("Value: ");
    #asm("sei")
while (1){}
}
