#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdio.h>

unsigned int ADC_read(unsigned char channel) {
    ADMUX = ADMUX|(channel & 0x0f);
    ADCSRA|=(1<<ADSC);
    while(!(ADCSRA & (1<<ADIF)));
    ADCSRA|=(1<<ADIF);
    return (ADC);
}

int main(void){
    printf_init();
    printf("Hello World\n");

    ADMUX=(1<<REFS0);
    ADCSRA=(1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); // Prescaler 128

    while(1) {
        printf("Hello World\n");
        
        int i = ADC_read(0);
        printf("ADC Value: %d\n", i);
        _delay_ms(1000);
    }
}
