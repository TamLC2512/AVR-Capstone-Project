#include <mega328p.h>
#include <delay.h>

char Led_7Seg[] = {
        0b01000000,
        0b01111001,
        0b00100100,
        0b00110000,
        0b00011001,
        0b00010010,
        0b00000010,
        0b01111000,
        0b00000000,
        0b00010000
};

char current_button_1, previous_button_1;
char current_button_2, previous_button_2;
char counter = 0;

void main(void)
{
#pragma optsize-
CLKPR=(1<<CLKPCE);
CLKPR=0x00;
#ifdef _OPTIMIZE_SIZE_
#pragma optsize+
#endif
  // dat muc logic cac chan PORTD voi muc thap
  PORTD = 0b11111111;
  // cau hinh cac chan PORTD la chan xuat
  DDRD  = 0b11111111;

  // cau hinh dien tro keo len ben trong MCU
  PORTB.0 = 1;
  PORTC.5 = 1;
  // cau hinh cac chan PB0 va PC6 la chan nhan
  DDRB.0  = 0;
  DDRC.5  = 0;

  while (1)
  {
    previous_button_1 = current_button_1;
    current_button_1 = PINB.0;

    previous_button_2 = current_button_2;
    current_button_2 = PINC.5;

    if(PINB.0 == 0 && previous_button_1 != 0)
    {
        if(counter < 9) counter++;
        else counter = 0;
    }
    else if(PINC.5 == 0 && previous_button_2 != 0)
    {
        if(counter > 0) counter--;
        else counter = 9;
    }
    PORTD = Led_7Seg[counter];
    delay_ms(50);
  }
}
