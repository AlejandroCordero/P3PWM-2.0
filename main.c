#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

uint8_t adc_value;

void ADC_Init()
{
	ADMUX=(1<<REFS1);											 // Selecting internal reference voltage (1.1V)
	ADCSRA=(1<<ADIE)|(1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); // Enable ADC also set Prescaler as 128 and AD interruptions are enabled
	ADMUX=0x40|(1<<ADLAR);										 // Channel A0 selected
	ADCSRA|=(1<<ADSC);											 // ADSC is set to 1 to start next conversion
}

void PWM_Setup()
{
	TCCR0A = (1<<WGM00)|(1<<WGM01)|(1<<COM0B1);					 // WMG01 and WMG00 are set as 1 to make TIMERB work in FAST PWM. OC0 is set at BOTTOM.
	TCCR0B = (1<<CS00);											 // Prescaler=0
}

void Ports_Setup()
{
	DDRG=(1<<PG5);												// Digital pin 4 (PG5) is set as output (1)
	DDRF=0b00000000;										    // Analog input pin 0 (PF0) is set as input (0)
}

int main(void)
{
	Ports_Setup();
	PWM_Setup();
	ADC_Init();
	sei();

	while (1)
	{
		OCR0B = adc_value;
		_delay_ms(50);
	}
}

ISR(ADC_vect)
{
	adc_value=ADCH;												// 8 bit precision
	ADCSRA |= (1<<ADSC);										// ADSC is set to 1 to start next conversion
}