/* Core/Src/lcd_i2c.c */

#include "lcd_i2c.h"

// --- Pointers to timer and i2c --- (allows to use different timers/i2c)
static I2C_HandleTypeDef *lcd_i2c_handle;
static TIM_HandleTypeDef *lcd_tim_handle;


// --- Private Functions ---
static void delay_us(uint32_t us)
{
	__HAL_TIM_SET_COUNTER(lcd_tim_handle, 0); //sets TIM10 counter period do us, (prescaler equals 84)
	while (__HAL_TIM_GET_COUNTER(lcd_tim_handle) < us) {} // counts
}


static void LCD_SendInternal(uint8_t to_be_send, uint8_t flags)
/* to_be_send - 1 byte to be send
 * flags - command/data
 * [D7 D6 D5 D4] [BL EN RW RS]
 * */
{
	uint8_t data_arr[4]; //4 bytes that will be transmitted

	data_arr[0] = (to_be_send & 0xF0) | LCD_PIN_BL | LCD_PIN_EN | flags;
	data_arr[1] = (to_be_send & 0xF0) | LCD_PIN_BL | flags;
	data_arr[2] = to_be_send << 4 | LCD_PIN_BL | LCD_PIN_EN | flags;
	data_arr[3] = to_be_send << 4 | LCD_PIN_BL | flags;

	HAL_I2C_Master_Transmit(lcd_i2c_handle, LCD_ADDRESS, data_arr, 4, 100);
}


// --- Public Functions ---
void LCD_SendCommand(uint8_t command) //sending command
{
	LCD_SendInternal(command, 0);
}


void LCD_SendData(uint8_t data) //sending byte of data
{
	LCD_SendInternal(data, 1);
}


void LCD_initialize(I2C_HandleTypeDef *hi2c, TIM_HandleTypeDef *htim)
{
	lcd_i2c_handle = hi2c;
	lcd_tim_handle = htim;
	HAL_Delay(50);

	uint8_t reset_data[2];
	reset_data[0] = 0x30 | LCD_PIN_EN | LCD_PIN_BL; // 0x3C
	reset_data[1] = 0x30 | LCD_PIN_BL;              // 0x38

	HAL_I2C_Master_Transmit(lcd_i2c_handle, LCD_ADDRESS, reset_data, 2, 10);
	delay_us(5000);

	HAL_I2C_Master_Transmit(lcd_i2c_handle, LCD_ADDRESS, reset_data, 2, 10);
	delay_us(150);

	HAL_I2C_Master_Transmit(lcd_i2c_handle, LCD_ADDRESS, reset_data, 2, 10);
	delay_us(150);

	uint8_t set_4_bit[2];
	set_4_bit[0] = 0x20 | LCD_PIN_EN | LCD_PIN_BL;
	set_4_bit[1] = 0x20 | LCD_PIN_BL;

	HAL_I2C_Master_Transmit(lcd_i2c_handle, LCD_ADDRESS, set_4_bit, 2, 10);
	delay_us(150);

	LCD_SendCommand(0x28); // Function Set: 4-bit, 2 lines, 5x8
	LCD_SendCommand(0x0C); // Display ON, Cursor OFF
	LCD_SendCommand(0x06); // Entry Mode: Auto-Increment
	LCD_SendCommand(0x01); // Clear Display
	HAL_Delay(2);
}


void LCD_Clear(void) {
    LCD_SendCommand(0x01);
    HAL_Delay(2);
}
