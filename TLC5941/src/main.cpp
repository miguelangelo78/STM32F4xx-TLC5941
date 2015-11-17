#include "tlc5941.h"
#include <vector>

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

#define LED_COUNT 3
TLCRGB leds[LED_COUNT];

#define update_all_leds() for(int i = 0; i < LED_COUNT; i++) leds[i].update();

int main(void)
{
	SystemInit();

	for(int i = 0; i < LED_COUNT; i++)
		leds[i].init(i);

	for(;;) {
		INC(leds[0].rgb.r, 0.5f, 0xFF, 0);
		INC(leds[1].rgb.g, 0.5f, 0xFF, 0);
		INC(leds[2].rgb.b, 0.5f, 0xFF, 0);

		update_all_leds();
	}
}
