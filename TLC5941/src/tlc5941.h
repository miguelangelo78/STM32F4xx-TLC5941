/*
 * tlc5941.h
 *
 *  Created on: Nov 17, 2015
 *      Author: Miguel
 */

#ifndef TLC5941_H_
#define TLC5941_H_

#include "stm32f4xx.h"

class TLC5941 {
public:
	TLC5941();

	uint8_t getXERR();
	/* Sets dot correction value (from 0 to 63) for a certain channel */
	void setDot(uint8_t channel, uint8_t dot_val);
	/* Sets brightness value (from 0 to 4095) for a certain channel */
	void setChannel(uint8_t channel, uint16_t brightness);
	/* Updates the chip by outputting the GS data */
	static void update();

private:
	enum TLCPINS {
		MODE 	= GPIO_Pin_4, /* 0 = In Dot Correction Mode | 1 = In Grayscale Mode (Output) */
		SIN 	= GPIO_Pin_7, /* Serial input to TLC5941 (Output) */
		SCLK 	= GPIO_Pin_5, /* Serial clock (Output) */
		XLAT 	= GPIO_Pin_6, /* Serial data latch into the registers (Output) */
		BLANK 	= GPIO_Pin_3, /* Turns off LEDS and signals the start of the Grayscale cycle (Output) */
		XERR 	= GPIO_Pin_2  /* XERR signals LED overheating (Input) */
	};

	/* Modes: */
	enum MD {
		MD_GS, /* Grayscale mode */
		MD_DC  /* Dot Correction Mode */
	};

	/* Send dot correction data: */
	inline void sendDot(void);
	/* Send grayscale data: */
	inline void sendGS(void);
};

#endif /* TLC5941_H_ */
