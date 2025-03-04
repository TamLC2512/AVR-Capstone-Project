#include <mega328p.h>
#include <delay.h>
#include <alcd.h>
#include <stdint.h>
// Voltage Reference: AVCC pin
#define ADC_VREF_TYPE ((0<<REFS1)
                        |(1<<REFS0)|(0<<ADLAR))
void Display_Value(unsigned int number);
float Analog_to_Temp(unsigned int val);
float map(uint16_t input, uint16_t in_min,
uint16_t in_max, uint16_t out_min, uint16_t out_max);
unsigned int ADC0;
float Temp = 0;
//unsigned int adc_value;
//float voltage, temperature;
unsigned int read_adc(unsigned char adc_input){
    ADMUX=adc_input | ADC_VREF_TYPE;
    delay_us(10);
    // Start the AD conversion
    ADCSRA|=(1<<ADSC);
    // Wait for the AD conversion to complete
    while ((ADCSRA & (1<<ADIF))==0);
    ADCSRA|=(1<<ADIF);
    return ADCW;
}
void main(void)
{
    #pragma optsize-
    CLKPR=(1<<CLKPCE);
    CLKPR=0x00;
    #ifdef _OPTIMIZE_SIZE_
    #pragma optsize+
    #endif
    // ADC initialization
    ADMUX=ADC_VREF_TYPE;
    ADCSRA=(1<<ADEN)|(1<<ADATE)
            |(1<<ADPS1)|(1<<ADPS0);
    ADCSRB=(0<<ADTS2)|(0<<ADTS1)|(0<<ADTS0);
    lcd_init(16);
    lcd_gotoxy(3,0);
    lcd_puts("ECT Sensor");
    lcd_gotoxy(4,1);
    lcd_puts("Temp:");
    while (1)
    {
        ADC0 = read_adc(0);
        Temp = Analog_to_Temp(ADC0);

        lcd_gotoxy(9,1);
        Display_Value((unsigned int)(Temp));
        lcd_gotoxy(11,1);
        lcd_puts("oC");
    }
}
float map(uint16_t input, uint16_t in_min,
uint16_t in_max,uint16_t out_min,uint16_t out_max){
    return ((input - in_min) * (out_max - out_min))
    / (in_max - in_min) + out_min;
}
float Analog_to_Temp(unsigned int val){
    if(val >= 330 && val <= 415){
        return map(val, 330, 415, 18, 25);
    }
    else if(val > 415 && val <= 450){
        return map(val, 415, 450, 25, 29);
    }
    else if(val > 450 && val <= 490){
        return map(val, 450, 490, 29, 35);
    }
    else if(val > 490 && val <= 550){
        return map(val, 490, 550, 35, 40);
    }
    else if(val > 550 && val <= 620){
        return map(val, 550, 620, 40, 46);
    }
    else if(val > 620 && val <= 665){
        return map(val, 620, 665, 46, 51);
    }
    else if(val > 670 && val <= 760){
        return map(val, 665, 707, 51, 60);
    }
    else if(val > 760 && val <= 805){
        return map(val, 760, 805, 60, 65);
    }
    else if(val > 805 && val <= 840){
        return map(val, 805, 840, 65, 70);
    }
    else if(val > 840 && val <= 862){
        return map(val, 840, 855, 70, 74);
    }
    else if(val > 862){
        return 75;
    }
    else if(val < 330){
        return 17;
    }
}
void Display_Value(unsigned int number)
{
    unsigned int chuc,donvi;
    chuc = (number/10)%10;
    donvi = number%10;
    lcd_putchar(chuc+48);
    lcd_putchar(donvi+48);
}