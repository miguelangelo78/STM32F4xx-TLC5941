/*
 * tlcrgb.h
 *
 *  Created on: Nov 18, 2015
 *      Author: Miguel
 */

#ifndef TLCRGB_H_
#define TLCRGB_H_

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

	TLCRGB(int rgb_index);
	TLCRGB();

	void init(int rgb_index);

	void update();
	void update(rgb_t rgb_struct);

	void set(rgb_t rgb_struct);
	void set(uint16_t r, uint16_t g, uint16_t b);
	void setR(uint16_t r);
	void setG(uint16_t g);
	void setB(uint16_t b);

	static void led_test();

	TLC5941 getTLC5941();

private:
	int index;
	char rgb_off; /* RGB offset on the TLC5941 */
	TLC5941 tlc;
};

#endif /* TLCRGB_H_ */
