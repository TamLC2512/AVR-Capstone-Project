#include <mega328p.h>
#include <delay.h>

void main(void){
    // Khai bao PORTD la ngo xuat tin hieu
    DDRD = 0b111111111;     //0xFF

    //Keo PORTD len muc logic 1, tat ca den deu tat
    PORTD = 0b11111111;     //0xFF

    // vong lap vo tan
    while(1)
    {
        int i;
        for(i=0; i<8; i++){
            PORTB = ~(1<<i);
            delay_ms(100);
        }
    }
}
