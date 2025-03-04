//Giao tiep SPI - MASTER
#include <mega328p.h>

#define MOSI    3   //chan PB3
#define MISO    4   //chan PB4
#define SCK     5   //chan PB5
#define SS      2   //chan PB2

volatile char receivedData;

void UART_Init(){
    //kich hoat nhan du lieu
    //cho phep ngat khi nhan
    UCSR0B = (1<<RXEN0) | (1<<RXCIE0);
    //cau hinh du lieu: 8 bit
    UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);
    UCSR0A = 0x00;

    //chon toc do Baud Rate, 9600bps
    UBRR0H=0x00;
    UBRR0L=0x33;
}

void SPI_MASTER_Init(){
    //Khoi tao SPI MASTER
    DDRB |= (1<<MOSI)|(1<<SCK)|(1<<SS); //MOSI, SCK, SS = Output
    DDRB &= ~(1<<MISO);                 //MISO = Input
    PORTB |= (1<<SS);                   //dien tro keo len

    //SPI Enable, Master, F/128
    SPCR |= (1<<SPE)|(1<<MSTR)| (1<<SPR1)|(1<<SPR0);
}

void SPI_MASTER_Transmit(unsigned char data) {
    SPDR = data;                        //ghi du lieu vao thanh ghi du lieu
    while (!(SPSR & (1<<SPIF)));        //hoan thanh gui
}

void main(void)
{
#pragma optsize-
CLKPR=(1<<CLKPCE);
CLKPR=(0<<CLKPCE) | (0<<CLKPS3) | (0<<CLKPS2) | (0<<CLKPS1) | (0<<CLKPS0);
#ifdef _OPTIMIZE_SIZE_
#pragma optsize+
#endif

UART_Init();
SPI_MASTER_Init();
#asm("sei")

while (1){}
}

//Nhan duoc Data tu PC se gui sang SLAVE thong qua giao thuc SPI
interrupt [USART_RXC] void USART_RX_Complete(void) {
    receivedData = UDR0;

    PORTB &= ~(1<<SS);      //keo chan SS xuong muc thap de truyen
    SPI_MASTER_Transmit(receivedData);
    PORTB |= (1<<SS);       //keo chan SS len khi ket thuc truyen
}