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

	void init(void);
	inline uint8_t getXERR();
	/* Sets dot correction value (from 0 to 64) for a certain channel */
	void setDot(uint8_t channel, uint8_t dot_val);
	/* Sets brightness value (from 0 to 4096) for a certain channel */
	void setChannel(uint8_t channel, uint16_t brightness);
	/* Updates the chip by outputting the DC and GS data */
	void update();

private:
	uint8_t mode; /* Keeps track of the value outputted into the MODE pin */

	enum TLCPINS {
		MODE 	= GPIO_Pin_0, /* 0 = In Dot Correction Mode | 1 = In Grayscale Mode (Output) */
		SIN 	= GPIO_Pin_1, /* Serial input to TLC5941 (Output) */
		SCLK 	= GPIO_Pin_2, /* Serial clock (Output) */
		XLAT 	= GPIO_Pin_3, /* Serial data latch into the registers (Output) */
		BLANK 	= GPIO_Pin_4, /* Turns off LEDS and signals the start of the Grayscale cycle (Output) */
		XERR 	= GPIO_Pin_5  /* XERR signals LED overheating (Input) */
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
