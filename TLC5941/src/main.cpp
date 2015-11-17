#include "tlc5941.h"

int main(void)
{
	TLC5941 tlc;
	tlc.init();

	/* Magenta color: */
	tlc.setChannel(0, 0xFF);
	tlc.setChannel(2, 0xFF);

	for(;;)
		tlc.update();
}
