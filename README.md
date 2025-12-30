# ATmega328P Internal Temperature Monitor

This project reads the internal temperature sensor built into the ATmega328P microcontroller on an Arduino Uno. Instead of using external sensors, it taps into the chip's own temperature-sensing hardware (ADC channel 8) to measure how hot the silicon die is. I simply used a Kalman Filter to get rid of the noisy data.

You interact with it through a simple serial interface at 9600 baud. Type `status` to see the current temperature and supply voltage, or `help` to see available commands. The code is written in C without any Arduino libraries, so it's a good example of how to work directly with AVR hardware registers.

