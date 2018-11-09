//////////////////////////////////////////////////////////////////////////////////
//±Ÿ³ÌÐòÖ»¹©Ñ§Ï°Ê¹ÓÃ£¬ÎŽŸ­×÷ÕßÐí¿É£¬²»µÃÓÃÓÚÆäËüÈÎºÎÓÃÍŸ
//ÖÐŸ°Ô°µç×Ó
//µêÆÌµØÖ·£ºhttp://shop73023976.taobao.com/?spm=2013.1.0.0.M4PqC2
//
//  ÎÄ Œþ Ãû   : main.c
//  °æ ±Ÿ ºÅ   : v2.0
//  ×÷    Õß   : HuangKai
//  Éú³ÉÈÕÆÚ   : 2014-0101
//  ×îœüÐÞžÄ   :
//  ¹ŠÄÜÃèÊö   : OLED 4œÓ¿ÚÑÝÊŸÀý³Ì(51ÏµÁÐ)
//              ËµÃ÷:
//              ----------------------------------------------------------------
//              GND    µçÔŽµØ
//              VCC  œÓ5V»ò3.3vµçÔŽ
//              D0   œÓPA5£šSCL£©
//              D1   œÓPA7£šSDA£©
//              RES  œÓPB0
//              DC   œÓPB1
//              CS   œÓPA4
//              ----------------------------------------------------------------
// ÐÞžÄÀúÊ·   :
// ÈÕ    ÆÚ   :
// ×÷    Õß   : HuangKai
// ÐÞžÄÄÚÈÝ   : ŽŽœšÎÄŒþ
//°æÈšËùÓÐ£¬µÁ°æ±ØŸ¿¡£
//Copyright(C) ÖÐŸ°Ô°µç×Ó2014/3/16
//All rights reserved
//******************************************************************************/
#ifndef __OLED_H
#define __OLED_H

#include "ch.h"
#include "hal.h"
#include "hal_uart_lld.h"
#include "stdlib.h"

//OLEDÄ£ÊœÉèÖÃ
//0:4ÏßŽ®ÐÐÄ£Êœ
//1:²¢ÐÐ8080Ä£Êœ
#define OLED_MODE 0
#define SIZE 16
#define XLevelL 0x00
#define XLevelH 0x10
#define Max_Column 128
#define Max_Row 64
#define Brightness 0xFF
#define X_WIDTH 128
#define Y_WIDTH 64

//-----------------OLED¶Ë¿Ú¶šÒå----------------

#define OLED_SCLK_Clr() palClearPad(GPIOB, 12) //CLK
#define OLED_SCLK_Set() palSetPad(GPIOB, 12)

#define OLED_SDIN_Clr() palClearPad(GPIOB, 13) //DIN
#define OLED_SDIN_Set() palSetPad(GPIOB, 13)

#define OLED_RST_Clr() palClearPad(GPIOC, 15) //RES
#define OLED_RST_Set() palSetPad(GPIOC, 15)

#define OLED_DC_Clr() palClearPad(GPIOC, 14) //DC
#define OLED_DC_Set() palSetPad(GPIOC, 14)

//#define OLED_CS_Clr()  palClearPad(GPIOA, 6)//CS
//#define OLED_CS_Set()  palSetPad(GPIOA, 6)

#define OLED_CMD 0  //ÐŽÃüÁî
#define OLED_DATA 1 //ÐŽÊýŸÝ

//OLED¿ØÖÆÓÃº¯Êý
void OLED_WR_Byte(uint8_t dat, uint8_t cmd);
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(uint8_t x, uint8_t y, uint8_t t);
void OLED_Fill(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t dot);
void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr);
void OLED_ShowNum(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t size);
void OLED_ShowString(uint8_t x, uint8_t y, char *p);
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_ShowCHinese(uint8_t x, uint8_t y, uint8_t no);
void OLED_DrawBMP(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char BMP[]);
#endif
