#include <mega328p.h>
#include <delay.h>
#include <alcd.h>
#include <stdio.h>
#include <stdint.h>

#define TCNT1  0xC1 
#define PWM_R  6     //PD6 (OC0A)
#define PWM_L  5     //PD5 (OC0B)

#define pulsesPerRevolution 2000 //encoder co 2000 rang
#define sampleTime 0.01          //thoi gian lay mau

// Voltage Reference: AREF pin
#define ADC_VREF_TYPE ((0<<REFS1)|(0<<REFS0)|(0<<ADLAR))

void PID_Calculate();
void pwmPulse(int pwm);
//void Display_Value(unsigned int number);
void UART_putchar(char data);
void UART_Init();
void data_write( char* data, unsigned int size );

//char error[3], deSp[5], currSp[5];
volatile long pulseCount = 0;    //bien dem xung

float Kp = 0.2;
float Ki = 0.35;
float Kd = 0.0001;
float input, error, output, setpoint;

float maxOutput = 255;
float minOutput = 0;

int input_LCD, err_LCD;

// Read the AD conversion result
unsigned int read_adc(unsigned char adc_input)
{
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

// INT0 Mode: Rising Edge
EICRA=(0<<ISC11)|(0<<ISC10)|(1<<ISC01)|(1<<ISC00);
EIMSK=(0<<INT1) | (1<<INT0);
EIFR=(0<<INTF1) | (1<<INTF0);
PCICR=(0<<PCIE2) | (0<<PCIE1) | (0<<PCIE0);

//cau hinh chan doc Phase A, B la chan nhan;
//co dien tro keo len
//Phase A:INT0, Phase B:INT1
DDRD = (0<<DDD2) | (0<<DDD3);
PORTD = (1<<PORTD2) | (1<<PORTD3);

//chan xuat PWM, chan PD6, PD5
DDRD.6 = 1;
DDRD.5 = 1;
//chan xac dinh chieu quay
DDRD.7 = 1;
DDRB.0 = 1;
PORTD.7 = 1;
PORTB.0 = 1;

// Timer/Counter 0 initialization
// Clock value: 31.250 kHz
// Fast PWM, Prescaler: 256
TCCR0A=(1<<COM0A1)|(1<<WGM01)|(1<<WGM00);
TCCR0B=(0<<WGM02)|(0<<CS02)|(1<<CS01)|(0<<CS00);
TCNT0=0xC2;
OCR0A=0x00;
OCR0B=0x00;

// Timer/Counter 1 initialization
// Normal mode, Prescaler: 8
TCCR1A=(0<<WGM11)|(0<<WGM10);
TCCR1B=(0<<WGM13)|(0<<WGM12)|(0<<CS12)|(0<<CS11)|(1<<CS10);
//gia tri ban dau cua TCNT1 = 45535
TCNT1H = 0xC1;
TCNT1L = 0x80;

// Timer/Counter 2 initialization
// Normal mode, Prescaler:1024, tao delay 25ms
ASSR=(0<<EXCLK) | (0<<AS2);
TCCR2A=0x00;
TCCR2B=(0<<WGM22)|(1<<CS22)|(0<<CS21)|(0<<CS20);
TCNT2=0x06;

// Timer/Counter 0 Interrupt(s) initialization
TIMSK0=(0<<OCIE0B) | (0<<OCIE0A) | (0<<TOIE0);
// Timer/Counter 1 Interrupt(s) initialization
TIMSK1=(0<<ICIE1)|(0<<OCIE1B)|(0<<OCIE1A)|(1<<TOIE1);
// Timer/Counter 2 Interrupt(s) initialization
TIMSK2=(0<<OCIE2B) | (0<<OCIE2A) | (1<<TOIE2);

// ADC initialization
// ADC Voltage Reference: AREF pin
ADMUX=ADC_VREF_TYPE;
ADCSRA=(1<<ADEN)|(1<<ADATE)
|(0<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
ADCSRB=(0<<ADTS2)|(0<<ADTS1)|(0<<ADTS0);

lcd_init(16);
DDRB.4 = 1;
PORTB.4 = 0;

DDRD.1 = 1;
UART_Init();
#asm("sei")
    while (1){
        setpoint = (float)read_adc(0);
    }
}
void UART_Init() {

    UBRR0H = 0;
    UBRR0L = 8;
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);
}
void UART_putchar(char data){
    while(!(UCSR0A & (1<<UDRE0)));
    UDR0 = data;
}
void data_write(char* data, unsigned int size ){
    int i =0;
    for (i = 0; i < size; i++ )
    {
    UART_putchar(data[i]);
    }
}
void PID_Calculate(){
    static float lastError = 0;
    static float pPart = 0;
    static float iPart = 0;
    static float dPart = 0;
    static long lastPulseCount = 0;
    long currentPulseCount = pulseCount;
    float pulses=currentPulseCount-lastPulseCount;
    lastPulseCount = currentPulseCount;

    input = (float)(pulses / pulsesPerRevolution)
     * (60.0 / sampleTime); // RPM
    error = setpoint - input;
    pPart = Kp * error;
    iPart += Ki * error * sampleTime;
    dPart = Kd * (error - lastError) / sampleTime;
    output = pPart + iPart + dPart;
    lastError = error;

    if (iPart > maxOutput) {
        iPart = maxOutput;
    }
    else if (iPart < minOutput) {
        iPart = minOutput;
    }
    //output saturation
    if (output > maxOutput) {
        output = maxOutput;
    }
    else if (output < minOutput) {
        output = minOutput;
    }
    pwmPulse((int)output);
    //pwmPulse(20);
    input_LCD = (int)(input);
    err_LCD = (int)(error);
}
void pwmPulse(int pwm){
    OCR0A = pwm;
}
interrupt [EXT_INT0] void ext_int0_isr(void)
{
    pulseCount++;
}
// Timer1 overflow interrupt service routine
interrupt [TIM1_OVF] void timer1_ovf_isr(void)
{   static long counter = 0;
    if (counter >= 10)
    {
    PID_Calculate();
    counter = 0;   
    }    
    TCNT1H = 0xC1;
    TCNT1L = 0x80; 
    counter++;
    
}
interrupt [TIM2_OVF] void timer2_ovf_isr(void)
{
    static long counter = 0;
    if(counter >= 20){
        float data[4];
        data[0] = setpoint;
        data[1] = input;
        data[2] = error;
        data[3] = output;
        UART_putchar(0xBB);
        data_write((char*)&data, 16);
        UART_putchar(0xCC);
        counter = 0;
    }
    TCNT2 = 0x06;
    counter++;
}
