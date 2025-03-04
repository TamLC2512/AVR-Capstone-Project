#include <mega328p.h>
#include <delay.h>
#include <alcd.h>

#define ADC_VREF_TYPE ((0<<REFS1) | (1<<REFS0) | (0<<ADLAR))

void Display_Value(unsigned int number);

unsigned int resultADC5;
unsigned int Temp;

unsigned int read_adc(unsigned char adc_input)
{
    ADMUX=adc_input | ADC_VREF_TYPE;
    delay_us(10);
  // bat dau qua trinh chuyen doi
    ADCSRA|=(1<<ADSC);
  //cho den khi qua trinh chuyen doi ket thuc
    while ((ADCSRA & (1<<ADIF))==0);
    ADCSRA|=(1<<ADIF);
    return ADCW;
}

void main(void)
{
    #pragma optsize-
    CLKPR = (1<<CLKPCE);
    CLKPR = 0x00;
    #ifdef _OPTIMIZE_SIZE_
    #pragma optsize+
    #endif

    ADMUX=ADC_VREF_TYPE;
  // kich hoat ADC, chon bo chia 8 <=> 125.000kHz
    ADCSRA =(1<<ADEN) | (1<<ADATE) | (1<<ADPS2) | (1<<ADPS1);
    ADCSRB=(0<<ADTS2) | (0<<ADTS1) | (0<<ADTS0);

    lcd_init(16);
    lcd_gotoxy(1,0);
    lcd_puts("ADC with LM35 ");
    delay_ms(1000);
    while (1)
    {
      //du lieu duoc doc tu ADC
        resultADC5 = read_adc(5);

      //Temp = resultADC5*500/1024
        Temp = (resultADC5*125/256); //da rut gon
        lcd_gotoxy(4,1);
        lcd_putsf("Temp:");
        Display_Value(Temp);
    }
}
//Ham hien thi
void Display_Value(unsigned int number)
{
    unsigned int tram,chuc,donvi;
    tram = number/100%10;
    chuc = (number/10)%10;
    donvi = number%10;
    lcd_putchar(tram+48);
    lcd_putchar(chuc+48);
    lcd_putchar(donvi+48);
}
