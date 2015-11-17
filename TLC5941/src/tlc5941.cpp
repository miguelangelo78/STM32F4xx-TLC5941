/*
 * tlc5941.cpp
 *
 *  Created on: Nov 17, 2015
 *      Author: Miguel
 */
#include "tlc5941.h"
#include "port.h"
#include <string.h>

/* TLC5941 Datasheet: http://www.ti.com/lit/ds/symlink/tlc5941.pdf */

/* Channel sizes and counts: */
#define TLC5941_COUNT 1
#define GS_CHANNEL_LEN 12 /* Each GS channel is 12 bits */
#define DC_CHANNEL_LEN 6 /* Each DC channel is 6 bits */
#define GS_SIZE 192 /* Total size in bits of Grayscale PWM control */
#define DC_SIZE 96 /* Total size in bits of Dot Correction */

/* Helper macros for controlling the GPIO: */
#define readpin(pin) PDBI((pin)) /* Reads state of pin, same as PDBI, but forcing the port */
#define PULSE(pin) { PDB(pin, 1); PDB(pin, 0); } /* Send a pulse to a pin */
#define SET_BLANK(blank) PDB(BLANK, blank)
#define SET_MODE(md) { PDB(MODE, md); mode = md; }

/* There is no need for hardcoding all the 1's and 0's here. This is here for reference purpose */
/* Dot correction data: */
uint8_t dcData[TLC5941_COUNT * DC_SIZE] = {
	// MSB         LSB
	1, 1, 1, 1, 1, 1,  // Channel 15
	1, 1, 1, 1, 1, 1,  // Channel 14
	1, 1, 1, 1, 1, 1,  // Channel 13
	1, 1, 1, 1, 1, 1,  // Channel 12
	1, 1, 1, 1, 1, 1,  // Channel 11
	1, 1, 1, 1, 1, 1,  // Channel 10
	1, 1, 1, 1, 1, 1,  // Channel 9
	1, 1, 1, 1, 1, 1,  // Channel 8
	1, 1, 1, 1, 1, 1,  // Channel 7
	1, 1, 1, 1, 1, 1,  // Channel 6
	1, 1, 1, 1, 1, 1,  // Channel 5
	1, 1, 1, 1, 1, 1,  // Channel 4
	1, 1, 1, 1, 1, 1,  // Channel 3
	1, 1, 1, 1, 1, 1,  // Channel 2
	1, 1, 1, 1, 1, 1,  // Channel 1
	1, 1, 1, 1, 1, 1,  // Channel 0
};

/* Grayscale data */
uint8_t gsData[TLC5941_COUNT * GS_SIZE] = {
	// MSB                           LSB
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // Channel 15
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // Channel 14
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // Channel 13
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // Channel 12
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // Channel 11
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // Channel 10
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // Channel 9
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // Channel 8
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // Channel 7
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // Channel 6
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // Channel 5
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // Channel 4
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // Channel 3
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // Channel 2
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // Channel 1
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // Channel 0
};

uint8_t * byte_to_binary(int x, int bitcount) {
	static uint8_t b[GS_CHANNEL_LEN]; /* I picked the biggest channel */
	for (int z = (1 << bitcount), i = 0; z > 0; z >>= 1)
		b[i++] = (x & z) == z;
	return b;
}

TLC5941::TLC5941() {
	init_gpio(GPIOD, MODE | SIN | SCLK | XLAT | BLANK, XERR);
	PD(0);

	SET_MODE(MD_DC);
	SET_BLANK(1);

	/* Send default dot correction data: */
	sendDot();
}

uint8_t TLC5941::getXERR() {
	return readpin(XERR);
}

/* Sets dot correction value (from 0 to 64) for a certain channel */
void TLC5941::setDot(uint8_t channel, uint8_t dot_val) {
	int startOff = DC_SIZE - (channel * DC_CHANNEL_LEN) - DC_CHANNEL_LEN;
	uint8_t * new_dc_dat = byte_to_binary(dot_val, DC_CHANNEL_LEN);
	memcpy(gsData + startOff, new_dc_dat, DC_CHANNEL_LEN);
}

/* Sets brightness value (from 0 to 4095, 12 BITS (IMPORTANT)) for a certain channel */
void TLC5941::setChannel(uint8_t channel, uint16_t brightness) {
	int startOff = GS_SIZE - (channel * GS_CHANNEL_LEN) - GS_CHANNEL_LEN;
	uint8_t * new_gs_dat = byte_to_binary(brightness, GS_CHANNEL_LEN);
	memcpy(gsData + startOff, new_gs_dat, GS_CHANNEL_LEN);
}

/* Updates the chip by outputting the GS data */
void TLC5941::update() {
	sendGS();
}

/* Send dot correction data: */
void TLC5941::sendDot(void) {
	SET_MODE(MD_DC);

	for(uint8_t ctr = 0;;) {
		if(ctr > TLC5941_COUNT * DC_SIZE - 1) {
			PULSE(XLAT);
			break;
		} else {
			char val = dcData[ctr++] ? 1 : 0;
			PDB(SIN, val);
			PULSE(SCLK);
		}
	}
}

/* Send grayscale data: */
void TLC5941::sendGS(void) {
	uint8_t first_cycle_fl = 0;
	uint8_t data_ctr = 0;

	if(mode) {
		SET_MODE(MD_GS);
		first_cycle_fl = 1;
	}

	SET_BLANK(0);
	for(uint16_t gsclk_ctr = 0; gsclk_ctr <= 4095; gsclk_ctr++)
		if(!(data_ctr > TLC5941_COUNT * GS_SIZE - 1)) {
			char val = gsData[data_ctr++] ? 1 : 0;
			PDB(SIN, val);
			PULSE(SCLK);
		}

	/* End of GS cycle */
	SET_BLANK(1);
	PULSE(XLAT);
	if(first_cycle_fl)
	PULSE(SCLK);
}
