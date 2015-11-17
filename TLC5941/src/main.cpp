#include "tlc5941.h"
#include <stdlib.h>

class TLCRGB {
public:
	#define INC(v, delta, max, min) if(((v)+=(delta)) >= (max)) (v) = (min);
	#define DEC(v, delta, max, min) if(((v)-=(delta)) >= (max)) (v) = (min);

	typedef struct {
		/* Float allows us to have more precision on the control of the brightness,
		 * however it is casted into uint16_t in the end */
		float r = 0, g = 0, b = 0;
	} rgb_t;

	rgb_t rgb;

	TLCRGB(int rgb_index) : index(rgb_index), rgb_off(index * 3) { }
	TLCRGB() {}

	void init(int rgb_index) {
		index = rgb_index;
		rgb_off = index * 3;
	}

	void update() {
		tlc.setChannel(rgb_off, (uint16_t)rgb.r);
		tlc.setChannel(rgb_off + 1, (uint16_t)rgb.g);
		tlc.setChannel(rgb_off + 2, (uint16_t)rgb.b);
		tlc.update();
	}

	void update(rgb_t rgb_struct) {
		set(rgb_struct);
		update();
	}

	void set(rgb_t rgb_struct) {
		rgb.r = rgb_struct.r;
		rgb.g = rgb_struct.g;
		rgb.b = rgb_struct.b;
	}

	void set(uint16_t r, uint16_t g, uint16_t b) {
		rgb.r = r;
		rgb.g = g;
		rgb.b = b;
	}

	void setR(uint16_t r) {
		rgb.r = r;
	}

	void setG(uint16_t g) {
		rgb.g = g;
	}

	void setB(uint16_t b) {
		rgb.b = b;
	}

private:
	int index;
	char rgb_off; /* RGB offset on the TLC5941 */
	TLC5941 tlc;
};


void led_test() {
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
					INC(leds[i].rgb.r, rand() % 3, rand()%0xFF+0x7F, rand()%0xFF);
				} else {
					DEC(leds[i].rgb.r, rand() % 3, rand()%0xFF+0x7F, rand()%0xFF);
				}
				break;
			case 1:
				if(rand()%5 > 1) {
					INC(leds[i].rgb.g, rand() % 3,rand()%0xFF+0x7F, rand()%0xFF);
				} else {
					DEC(leds[i].rgb.g, rand() % 3,rand()%0xFF+0x7F, rand()%0xFF);
				}
				break;
			case 2:
				if(rand()%5 > 1) {
					INC(leds[i].rgb.b, rand() % 3, rand()%0xFF+0x7F, rand()%0xFF);
				} else {
					DEC(leds[i].rgb.b, rand() % 3, rand()%0xFF+0x7F, rand()%0xFF);
				}
				break;
			}
		}

		update_all_leds();
	}
}

int main(void)
{
	SystemInit();

	led_test();
}
