# STM32 HD44780 I2C LCD Driver

A lightweight, optimized, and modular C driver library for 16x2 LCDs (HD44780 controller) using an I2C interface (PCF8574) on STM32 microcontrollers (tested on Nucleo-F411RE).

- Low-Level I2C Control: Manual assembly of data frames for the PCF8574 expander, involving bitwise manipulation of RS, RW, EN, and BL flags.
- printf Retargeting: Enables the use of the standard C library printf() function to output text directly to the LCD.
- Newline Handling: Automatically maps the newline character (\n) to the DDRAM memory address of the second line (0xC0), ensuring correct cursor positioning.
- Precise Hardware Delays: Utilizes a hardware timer (TIM10) to generate accurate microsecond delays.
- Hardware Abstraction: The library accepts pointers to I2C_HandleTypeDef and TIM_HandleTypeDef structures, facilitating easy code portability between different I2C instances and General Purpose Timers.

Pinout:
| STM32 Pin | LCD I2C Module |
|-----------|----------------|
| PB10      | SCL            |
| PB3       | SDA            |
| 5V        | VCC            |
| GND       | GND            |

Example:
printf() retargeting:
```c
int __io_putchar(int ch)
{
	if (ch == '\n') {
		LCD_SendCommand(0xC0); //going into the new line 0x40 + 0x80 = 0xC0
	}
	else {
		LCD_SendData((uint8_t)ch);
	    }
    return 1;
}
```
Initialization and writing:
```c
LCD_initialize(&hi2c2, &htim10);
printf("github.com/\nwmajka13");
fflush(stdout); //can be omitted when buffering is turned off
```
