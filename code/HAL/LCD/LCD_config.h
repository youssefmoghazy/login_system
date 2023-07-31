/********************************************************/
/************** Name : Youssef Moghazy ******************/
/************** Date : 21/3/2023       ******************/
/************** SWC  : LCD             ******************/
/************** Version : 1.0          ******************/
/********************************************************/
#ifndef LCD_CONFIG_H
#define LCD_CONFIG_H

#define LCD_u8_CONTROL_PORT			DIO_u8_PORTD
#define LCD_u8_RS_PIN				DIO_u8_PIN5
#define LCD_u8_RW_PIN				DIO_u8_PIN6
#define LCD_u8_E_PIN				DIO_u8_PIN7

#define LCD_u8_DATA_PORT 			DIO_u8_PORTA
/** OPTIONS: 1- LCD_u8_MODE_8_BIT
 * 			 2- LCD_u8_MODE_4_BIT
 */

#define LCD_u8_MODE					LCD_u8_MODE_4_BIT
#endif
