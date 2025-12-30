#ifndef ADC_H
#define ADC_H

#include <stdint.h>

#define ADC_CHANNEL_TEMPERATURE 8
#define ADC_CHANNEL_BANDGAP      14

#define ADC_REF_INTERNAL_1V1     0xC0
#define ADC_REF_AVCC             0x40

#define ADC_PRESCALER_MASK       0x07
#define ADC_PRESCALER_128        0x07
#define ADC_LEFT_ADJUST          0x20
#define ADC_ENABLE               0x80
#define ADC_START_CONVERSION      0x40
#define ADC_CONVERSION_COMPLETE   0x10

void ADC_Init(void);
uint16_t ADC_Read(uint8_t channel);
uint16_t ADC_ReadTemperature(void);
uint16_t ADC_ReadBandgap(void);
uint16_t ADC_CalculateVCC(void);

#endif

