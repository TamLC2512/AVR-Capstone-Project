#include <mega328p.h>

void main(void)
{
    #pragma optsize-
    CLKPR = (1<<CLKPCE);
    CLKPR = 0x00;
    #ifdef _OPTIMIZE_SIZE_
    #pragma optsize+
    #endif

  //su dung chan PD5 dieu khien den LED
    DDRD  |= (1<<DDD5);       //DDRD.5 = 1;
    PORTD &= ~ (1<<PORTD5);   //PORTD.5 = 0;

  //che do Analog Comparator Multiplexed Input
    ADCSRB=(1<<ACME);
    while (1)
      {
        if(ACSR & (1<<ACO))   //neu V_AIN0 > V_AINT1
            PORTD &= ~ (1<<PORTD5);
        else
            PORTD |= (1<<PORTD5);
      }
}
