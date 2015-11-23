/*
 * port.h
 *
 *  Created on: Nov 16, 2015
 *      Author: Miguel
 */

#ifndef PORT_H_
#define PORT_H_

/* Port handling macros: */
#define PA(val) GPIOA->ODR = (BitAction)val /* Output to port */
#define PAB(pin, val) GPIO_WriteBit(GPIOA, (pin), ((BitAction)val)) /* Output to port's pin */
#define PAI(pin) GPIO_ReadInputData(GPIOA) /* Read entire port */
#define PABI(pin) GPIO_ReadInputDataBit(GPIOA, (pin)) /* Read port's pin */

/* General purpose Port initializer function (one port at a time): */
inline void init_gpio(GPIO_TypeDef * gpio, uint32_t out_pins, uint32_t in_pins) {
	/* Default Port initializations: */
	GPIO_InitTypeDef port_out;
	port_out.GPIO_Pin = out_pins;
	port_out.GPIO_Mode = GPIO_Mode_OUT;
	port_out.GPIO_OType = GPIO_OType_PP;
	port_out.GPIO_PuPd = GPIO_PuPd_DOWN;
	port_out.GPIO_Speed = GPIO_Speed_100MHz;

	GPIO_InitTypeDef port_in;
	port_in.GPIO_Pin = in_pins;
	port_in.GPIO_Mode = GPIO_Mode_IN;
	port_in.GPIO_OType = GPIO_OType_PP;
	port_in.GPIO_PuPd = GPIO_PuPd_DOWN;
	port_in.GPIO_Speed = GPIO_Speed_100MHz;

	/* Initialize PORTA only: */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	if(out_pins) /* Won't initialize if out_pins is 0 */
		GPIO_Init(gpio, &port_out);
	if(in_pins) /* Won't initialize if in_pins is 0 */
		GPIO_Init(gpio, &port_in);
}

#endif /* PORT_H_ */
