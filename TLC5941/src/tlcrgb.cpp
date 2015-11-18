/*
 * tlcrgb.c
 *
 *  Created on: Nov 18, 2015
 *      Author: Miguel
 */

#include "tlcrgb.h"

TLCRGB::TLCRGB(int rgb_index) : index(rgb_index), rgb_off(index * 3) { }
TLCRGB::TLCRGB() {}

void TLCRGB::init(int rgb_index) {
	index = rgb_index;
	rgb_off = index * 3;
}

void TLCRGB::update() {
	tlc.setChannel(rgb_off, (uint16_t)rgb.r);
	tlc.setChannel(rgb_off + 1, (uint16_t)rgb.g);
	tlc.setChannel(rgb_off + 2, (uint16_t)rgb.b);
	tlc.update();
}

void TLCRGB::update(rgb_t rgb_struct) {
	set(rgb_struct);
	update();
}

void TLCRGB::set(rgb_t rgb_struct) {
	rgb.r = rgb_struct.r;
	rgb.g = rgb_struct.g;
	rgb.b = rgb_struct.b;
}

void TLCRGB::set(uint16_t r, uint16_t g, uint16_t b) {
	rgb.r = r;
	rgb.g = g;
	rgb.b = b;
}

void TLCRGB::setR(uint16_t r) {
	rgb.r = r;
}

void TLCRGB::setG(uint16_t g) {
	rgb.g = g;
}

void TLCRGB::setB(uint16_t b) {
	rgb.b = b;
}

TLC5941 TLCRGB::getTLC5941() {
	return tlc;
}

void TLCRGB::led_test() {
	#define LED_COUNT 3
	TLCRGB leds[LED_COUNT];

	#define update_all_leds() for(int i = 0; i < LED_COUNT; i++) leds[i].update();

	for(int i = 0; i < LED_COUNT; i++)
		leds[i].init(i);

	for(;;) {
		for(int i=0;i<LED_COUNT;i++) {
			switch(rand() % 3) {
			case 0:
				if(rand()%5 > 1) {
					INC(leds[i].rgb.r, rand() % 3, rand()%0xFF+0x7F, 0);
				} else {
					DEC(leds[i].rgb.r, rand() % 3, rand()%0xFF+0x7F, 0);
				}
				break;
			case 1:
				if(rand()%5 > 1) {
					INC(leds[i].rgb.g, rand() % 3,rand()%0xFF+0x7F, 0);
				} else {
					DEC(leds[i].rgb.g, rand() % 3,rand()%0xFF+0x7F, 0);
				}
				break;
			case 2:
				if(rand()%5 > 1) {
					INC(leds[i].rgb.b, rand() % 3, rand()%0xFF+0x7F, 0);
				} else {
					DEC(leds[i].rgb.b, rand() % 3, rand()%0xFF+0x7F, 0);
				}
				break;
			}
		}

		update_all_leds();
	}
}

