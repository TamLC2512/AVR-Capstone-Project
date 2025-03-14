#include <mega328p.h>
#include <alcd.h>
volatile char receivedData;
interrupt [USART_RXC] void USART_RX_Complete(void){
  //Lay du lieu tu thanh ghi UDR0
    receivedData = UDR0;
    lcd_gotoxy(5,1);
    lcd_putchar(receivedData);
}
void main(void)
{
    #pragma optsize-
    CLKPR = (1<<CLKPCE);
    CLKPR = 0x00;
    #ifdef _OPTIMIZE_SIZE_
    #pragma optsize+
    #endif

  //kich hoat nhan du lieu
  //cho phep ngat khi nhan
    UCSR0B = (1<<RXEN0) | (1<<RXCIE0);
    //cau hinh du lieu: 8 bit
    UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);
    UCSR0A = 0x00;

  //chon toc do Baud Rate, 9600bps
    UBRR0H=0x00;
    UBRR0L=0x33;

    lcd_init(16);
    lcd_clear();
    lcd_gotoxy(2,0);
    lcd_putsf("DATA FROM PC");
    lcd_gotoxy(0,1);
    lcd_putsf("CHAR:");
    #asm("sei")
while (1){}
}
