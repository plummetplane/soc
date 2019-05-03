#include <stdint.h>

#include "../serial/i2c.h"

#include "ssd1306.h"

#define SSD1306_CMD_CHRG_PUMP 0x8d
#define SSD1306_CHRG_PUMP_ENABLE 0x14
#define SSD1306_CHRG_PUMP_DISABLE 0x10

#define SSD1306_CMD_SET_CONTRAST_CTRL 0x81
#define SSD1306_CMD_DISP_RESUME 0xa4
#define SSD1306_CMD_DISP_ON 0xa5
#define SSD1306_CMD_NORMAL 0xa6
#define SSD1306_CMD_INVERTED 0xa7
#define SSD1306_CMD_PWR_OFF 0xae
#define SSD1306_CMD_PWR_ON 0xaf

#define SSD1306_CMD_SCROLL_HOR_RIGHT 0x26
#define SSD1306_CMD_SCROLL_HOR_LEFT 0x27
#define SSD1306_CMD_SCROLL_VER_HOR_RIGHT 0x29
#define SSD1306_CMD_SCROLL_VER_HOR_LEFT 0x2a
#define SSD1306_CMD_SCROLL_DEACTIVATE 0x2e
#define SSD1306_CMD_SCROLL_ACTIVATE 0x2f
#define SSD1306_CMD_SCROLL_VER_SET_AREA 0xa3

#define SSD1306_CMD_ADDR_LOW_COL 0x00
#define SSD1306_CMD_ADDR_HIGH_COL 0x10
#define SSD1306_CMD_ADDR_MODE 0x20
#define SSD1306_CMD_ADDR_ADDR_COL 0x21
#define SSD1306_CMD_ADDR_PAGE 0x22
#define SSD1306_CMD_ADDR_PAGE_START 0xb0

#define SSD1306_CMD_HW_START_LINE 0x40
#define SSD1306_CMD_HW_SEG_REMAP 0xa0
#define SSD1306_CMD_HW_MUX_RATIO 0xa8
#define SSD1306_CMD_HW_COM_OUT_DIR_NORMAL 0xc0
#define SSD1306_CMD_HW_COM_OUT_DIR_REVERSED 0xc8
#define SSD1306_CMD_HW_OFFSET 0xd3
#define SSD1306_CMD_HW_COM_CONFIG 0xda

#define SSD1306_CMD_CLK_DIV_OSC_FREQ 0xd5
#define SSD1306_CMD_PRECHRG_PERIOD 0xd9
#define SSD1306_CMD_VCOMH_DESEL_LEVEL 0xdb
#define SSD1306_CMD_NOP 0xe3

uint8_t ssd1306_addr(uint8_t addr) {
	/* i2c address */
	addr &= 0x01;
	addr |= 0x3c;

	return addr;
}

void ssd1306_init(uint8_t i2c, uint8_t addr, uint8_t mux_rat) {
	const uint8_t init_cmd[] = {
		SSD1306_CMD_PWR_OFF,
		SSD1306_CMD_CLK_DIV_OSC_FREQ,
		0xf0,
		SSD1306_CMD_HW_MUX_RATIO,
		mux_rat,
		SSD1306_CMD_HW_OFFSET,
		0x00,
		SSD1306_CMD_HW_START_LINE | 0x00,
		SSD1306_CMD_CHRG_PUMP,
		SSD1306_CHRG_PUMP_ENABLE,
		SSD1306_CMD_ADDR_MODE,
		0x00,
		SSD1306_CMD_HW_COM_CONFIG, /* TODO: might need to be passed as an argument */
		0x12,
		SSD1306_CMD_PRECHRG_PERIOD, /* let's stick to defaults */
		0x22,
		SSD1306_CMD_VCOMH_DESEL_LEVEL,
		0x30,
		SSD1306_CMD_DISP_RESUME,
		//SSD1306_CMD_DISP_ON, /* power on the whole display - testing purposes */
		SSD1306_CMD_NORMAL,
		SSD1306_CMD_PWR_ON
	};

	/* i2c address */
	//addr &= 0x01;
	//addr |= 0x3c;

	i2c_init_master(i2c);

	i2c_transfer(i2c, ssd1306_addr(addr), init_cmd, sizeof(init_cmd), 0, 0);
}

/*
 * transmit constructed framebuffer to the screen
 */
void ssd1306_transmit_buffer(uint8_t i2c, uint8_t addr, uint8_t *buf, uint16_t size) {
	uint8_t send_framebuf[1 + size]; 

	send_framebuf[0] = 0x40;

	for (uint16_t i = 0; i < size; i++) {
		send_framebuf[i + 1] = buf[i];
	}

	//addr &= 0x01;
	//addr |= 0x3c;

	i2c_transfer(i2c, ssd1306_addr(addr), send_framebuf, sizeof(send_framebuf), 0, 0);
}

/*
 * set up column/page range to update only part of the screen
 */
void ssd1306_set_col_addr(uint8_t i2c, uint8_t addr, uint8_t start, uint8_t end) {
	const uint8_t set_col_cmd[3] = {
		SSD1306_CMD_ADDR_ADDR_COL,
		start,
		end
	};

	i2c_transfer(i2c, ssd1306_addr(addr), set_col_cmd, sizeof(set_col_cmd), 0, 0);
}

void ssd1306_set_page_addr(uint8_t i2c, uint8_t addr, uint8_t start, uint8_t end) {
	const uint8_t set_page_cmd[3] = {
		SSD1306_CMD_ADDR_PAGE,
		start,
		end
	};

	i2c_transfer(i2c, ssd1306_addr(addr), set_page_cmd, sizeof(set_page_cmd), 0, 0);
}
