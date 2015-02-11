/*
 * button.h
 *
 *  Created on: Feb 11, 2015
 *      Author: ChenGuo
 */

#ifndef BUTTON_H_
#define BUTTON_H_

//define time unit: ms
#define SHAKE_TIME     10
#define CLICK_TIME     500
#define EFFECT_TIME    250
#define LONGCLICK_TIME 3000
//define button status
#define LONGCLICK      0x80
#define DOUBLECLICK    0x40
#define CLICK          0x20
#define IS_PRESS       0x10

/*
 * define the button's reggistor
 * bit[7]: long click flag
 * bit[6]: double click flag
 * bit[5]: click flag
 * bit[4]: is press flag
 * bit[3:0]: press time counter
 */
typedef struct _button{
	unsigned char (*readIoValue);
	unsigned char lastValue;
	unsigned char reg;
	unsigned char shakeTimer;
	unsigned int releaseTimer;
	unsigned int pressTimer;
}buttonType;

void buttonScan(buttonType *button);
unsigned char readButtonStatus(buttonType *button,unsigned char flag);

#endif /* BUTTON_H_ */
