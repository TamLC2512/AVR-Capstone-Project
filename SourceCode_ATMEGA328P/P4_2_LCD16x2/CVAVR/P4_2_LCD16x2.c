#include <mega328p.h>
#include <alcd.h>
#include <delay.h>

//mang chua doan ky tu can hien thi
//myStr gom (16+11+16) ky tu
char myStr[] = "                HELLO WORLD                ";
int length = sizeof(myStr) - 1; //tru 1 ky tu ket thuc
int lcd_width = 16;

void main(void)
{
#pragma optsize-
CLKPR=(1<<CLKPCE);
CLKPR=0x00;
#ifdef _OPTIMIZE_SIZE_
#pragma optsize+
#endif

  lcd_init(16);
  lcd_gotoxy(2,0);
  lcd_putsf("Demo of the");
  lcd_gotoxy(0,1);
  lcd_putsf("2x16 LCD Display");
  delay_ms(2000);
  lcd_clear();
  while (1)
  {
    int i, j;
    for (i = length - lcd_width; i >= 0; i--){
        lcd_clear();
        for (j = 0; j < lcd_width; j++) {
            lcd_gotoxy(j,0);
            lcd_putchar(myStr[i + j]);
        }
        delay_ms(500);
    }
  }
}
