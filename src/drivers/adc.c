#include "adc.h"
#include <avr/io.h>
#include <util/delay.h>

void ADC_Init(void)
{
    ADCSRA = (ADCSRA & ~ADC_PRESCALER_MASK) | ADC_PRESCALER_128;
    
    ADCSRA |= ADC_ENABLE;
    
    ADMUX = ADC_REF_INTERNAL_1V1;
    
    _delay_ms(1);
}

uint16_t ADC_Read(uint8_t channel)
{
    ADMUX = ADC_REF_INTERNAL_1V1 | (channel & 0x0F);
    
    ADCSRA |= ADC_START_CONVERSION;
    
    while (ADCSRA & ADC_START_CONVERSION);
    
    ADCSRA |= ADC_CONVERSION_COMPLETE;
    
    uint16_t result = ADCL;
    result |= (ADCH << 8);
    
    return result;
}

uint16_t ADC_ReadTemperature(void)
{
    return ADC_Read(ADC_CHANNEL_TEMPERATURE);
}

uint16_t ADC_ReadBandgap(void)
{
    uint8_t old_mux = ADMUX;
    ADMUX = ADC_REF_AVCC | (ADC_CHANNEL_BANDGAP & 0x0F);
    
    ADCSRA |= ADC_START_CONVERSION;
    
    while (ADCSRA & ADC_START_CONVERSION);
    
    ADCSRA |= ADC_CONVERSION_COMPLETE;
    
    uint16_t result = ADCL;
    result |= (ADCH << 8);
    
    ADMUX = old_mux;
    
    return result;
}

uint16_t ADC_CalculateVCC(void)
{
    uint16_t bandgap_adc = ADC_ReadBandgap();
    
    if (bandgap_adc == 0) {
        return 0;
    }
    
    uint32_t vcc_mv = ((uint32_t)1100 * 1024) / bandgap_adc;
    
    return (uint16_t)vcc_mv;
}

