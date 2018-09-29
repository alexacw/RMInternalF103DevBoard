#include "ch.hpp"
#include "hal.h"

#define DR16_UART_DRIVER UARTD2
#define DR16_TRANSMIT_INTERVAL_MS 17
#define DR16_RECEIVE_TIMEOUT_MS 10 //better not be divisor of DR16_TRANSMIT_INTERVAL_MS
#define DR16_RECEIVE_WAIT_MS (DR16_TRANSMIT_INTERVAL_MS - DR16_RECEIVE_TIMEOUT_MS)
#define DR16_CONNECT_TIMEOUT_MS 500
#define DR16_CONNECT_TIMEOUT_FAIL_COUNT (DR16_CONNECT_TIMEOUT_MS / DR16_TRANSMIT_INTERVAL_MS)

namespace DR16
{

extern volatile bool isConnected;

void start();

/* ----------------------- Data Struct ------------------------------------- */
typedef struct
{
	struct joystick_value_t
	{
		uint16_t ch0;
		uint16_t ch1;
		uint16_t ch2;
		uint16_t ch3;
		uint8_t s1;
		uint8_t s2;
	} rc;
	struct mouse_value_t
	{
		int16_t x;
		int16_t y;
		int16_t z;
		uint8_t press_l;
		uint8_t press_r;
	} mouse;

	struct
	{
		uint16_t v;
	} key;
} RC_Value_t;

extern volatile RC_Value_t rcValue;

//uart receive frame definition, use this for decoding the values
#pragma pack(1)
typedef union {
	struct
	{
		struct
		{
			uint8_t ch0_l : 8; //!< Byte 0
			uint8_t ch0_h : 3; //!< Byte 1
			uint8_t ch1_l : 5;
			uint8_t ch1_h : 6; //!< Byte 2
			uint8_t ch2_l : 2;
			uint8_t ch2_m : 8; //!< Byte 3
			uint8_t ch2_h : 1; //!< Byte 4
			uint8_t ch3_l : 7;
			uint8_t ch3_h : 4; //!< Byte 5
			uint8_t s1 : 2;
			uint8_t s2 : 2;
		} rc;
		struct
		{
			int16_t x;		 //!< Byte 6,7
			int16_t y;		 //!< Byte 8,9
			int16_t z;		 //!< Byte 10,11
			uint8_t press_l; //!< Byte 12
			uint8_t press_r; //!< Byte 13
		} mouse;
		struct
		{
			uint16_t v; //!< Byte 14,15
		} key;

		uint16_t resv; //!< Byte 16,17
	};
	uint8_t buf[18];
} RC_Msg_t;

/* ----------------------- RC Channel Definition---------------------------- */
#define RC_CH_VALUE_MIN ((uint16_t)364)
#define RC_CH_VALUE_MID ((uint16_t)1024)
#define RC_CH_VALUE_MAX ((uint16_t)1684)

/* ----------------------- RC Switch Definition----------------------------- */
#define RC_SW_UP ((uint16_t)1)
#define RC_SW_MID ((uint16_t)3)
#define RC_SW_DOWN ((uint16_t)2)
/* ----------------------- PC Key Definition-------------------------------- */
#define KEY_PRESSED_OFFSET_W ((uint16_t)0x01 << 0)
#define KEY_PRESSED_OFFSET_S ((uint16_t)0x01 << 1)
#define KEY_PRESSED_OFFSET_A ((uint16_t)0x01 << 2)
#define KEY_PRESSED_OFFSET_D ((uint16_t)0x01 << 3)
#define KEY_PRESSED_OFFSET_Q ((uint16_t)0x01 << 4)
#define KEY_PRESSED_OFFSET_E ((uint16_t)0x01 << 5)
#define KEY_PRESSED_OFFSET_SHIFT ((uint16_t)0x01 << 6)
#define KEY_PRESSED_OFFSET_CTRL ((uint16_t)0x01 << 7)
} // namespace DR16
