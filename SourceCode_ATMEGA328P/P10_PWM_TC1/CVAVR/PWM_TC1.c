#include <mega328p.h>

unsigned long int TOP = 19999;
unsigned long int STA_VAL = 999; //gia tri ban dau cua bo dem - 0 do
interrupt [EXT_INT0] void ext_int0_isr(void)
{
    // neu la 0 do thi keo len 90 do va nguoc lai
    if(((OCR1AH << 8) | OCR1AL)==999){
        OCR1AH = (1999 >> 8);
        OCR1AL = 1999 & 0xFF;
    }
    else{
        OCR1AH = (999 >> 8);
        OCR1AL = 999 & 0xFF;
    }
}
void main(void)
{
#pragma optsize-
CLKPR=(1<<CLKPCE);
CLKPR=(0<<CLKPCE) | (0<<CLKPS3) | (0<<CLKPS2) | (0<<CLKPS1) | (0<<CLKPS0);
#ifdef _OPTIMIZE_SIZE_
#pragma optsize+
#endif

DDRB.1 = 1;     //ngo ra chan OC1A, dieu khien servo
PORTD.2 = 1;    //chan PD2 la ngo vao, co dien tro keo len

TCCR1A=(1<<COM1A1) | (1<<WGM11);
TCCR1B=(1<<WGM13) | (1<<WGM12) | (1<<CS11);

// do ICR1 la thanh ghi 16bit nen phai chia ra de luu tru
ICR1H = (TOP >> 8);
ICR1L = TOP & 0xFF;

// gia tri ban dau - 0 do
OCR1AH = (STA_VAL >> 8);
OCR1AL = STA_VAL & 0xFF;

//ngat 0, lay canh xuong
EICRA |= (1<<ISC01);
EIMSK |= (1<<INT0);

#asm("sei")

while (1){}
}
