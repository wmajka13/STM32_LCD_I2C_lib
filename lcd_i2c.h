/* Core/Inc/lcd_i2c.h */

#ifndef LCD_I2C_H_
#define LCD_I2C_H_

#include "stm32f4xx_hal.h"

// --- Macros ---
#define LCD_ADDRESS (0x27 << 1)

#define LCD_PIN_RS    (1 << 0)
#define LCD_PIN_RW    (1 << 1)
#define LCD_PIN_EN    (1 << 2)
#define LCD_PIN_BL    (1 << 3)


// --- Function Prototypes ---

void LCD_SendCommand(uint8_t command);										//sending command
void LCD_SendData(uint8_t data);											//sending byte of data
void LCD_initialize(I2C_HandleTypeDef *hi2c, TIM_HandleTypeDef *htim); 		//Reseting the LCD and setting 4-bit communication
void LCD_Clear(); 															//Clears the screen



#endif /* LCD_I2C_H_ */
