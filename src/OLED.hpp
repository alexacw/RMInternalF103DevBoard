/**
 * @file OLED.hpp
 * @author some taobao shop with name decoded wrong
 * @brief 
 * @version 0.1
 * @date 2018-11-27
 * 
 */

#include "ch.h"
#include "hal.h"
#include "hal_uart_lld.h"
#include "stdlib.h"

#define SIZE 16
#define XLevelL 0x00
#define XLevelH 0x10
#define Max_Column 128
#define Max_Row 64
#define Brightness 0xFF
#define X_WIDTH 128
#define Y_WIDTH 64

#define OLED_SCLK_Clr() palClearLine(OLED_SCLK_LINE)
#define OLED_SCLK_Set() palSetLine(OLED_SCLK_LINE)

#define OLED_SDIN_Clr() palClearLine(OLED_SDIN_LINE) //DIN
#define OLED_SDIN_Set() palSetLine(OLED_SDIN_LINE)

#define OLED_RST_Clr() palClearLine(OLED_RST_LINE) //RES
#define OLED_RST_Set() palSetLine(OLED_RST_LINE)

#define OLED_DC_Clr() palClearLine(OLED_DC_LINE) //DC
#define OLED_DC_Set() palSetLine(OLED_DC_LINE)

//chip select pin not used
//#define OLED_CS_Clr()  palClearPad(GPIOA, 6)//CS
//#define OLED_CS_Set()  palSetPad(GPIOA, 6)

//logic levels of the D/C pin in command/data mode
#define OLED_CMD 0
#define OLED_DATA 1

/**
 * @brief 
 * The oled module's pixels are accessed by page and columns,
 * where there is total 7 pages, each associated with 8 rows (8 x coordinates)
 * page here represent the page and x represent the column number 
 */

/**
 * @brief 
 * 
 * @param dat 
 * @param cmd 
 */
void OLED_WR_Byte(uint8_t dat, uint8_t cmd);
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(uint8_t x, uint8_t page, uint8_t t);
void OLED_Fill(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t dot);
void OLED_ShowChar(uint8_t x, uint8_t page, uint8_t chr, bool bigLetter);
void OLED_ShowNum(uint8_t x, uint8_t page, uint32_t num, uint8_t len, uint8_t size, bool bigFont);
void OLED_ShowString(uint8_t x, uint8_t page, const char *p, bool bigLetter);
void OLED_Set_Pos(unsigned char x, unsigned char page);
void OLED_ShowCHinese(uint8_t x, uint8_t page, uint8_t no);
