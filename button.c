/*
 * button.c
 *
 *  Created on: Feb 11, 2015
 *      Author: ChenGuo
 */
#include "button.h"
#include "stm32f10x.h"

/*               ______                ________________________________________
 * click               |______________|
 *                     |<-CLICK_TIME->|<-EFFECT_TIME->|
 *               ______                _____________            _______________
 * double click        |______________|             |________|
 *                                                           |<-EFFECT_TIME->|
 *               ______                                           _____________
 * long click          |_________________________________________|
 *                     |<-------------LONGCLICK_TIME------------>|
 *
 */

void buttonScan(buttonType *button){
	unsigned char temp = 0;
	button->shakeTimer++;
	if(button->shakeTimer > SHAKE_TIME){//delay for reduce shake
		temp = button->readIoValue();
		if(temp ^ button->lastValue){
			button->lastValue = temp;
			if(button->lastValue){
				button->reg &= ~IS_PRESS;//button release flag
			}else{
				button->reg |= IS_PRESS;//button press flag
				button->reg++;//mark press time
			}
		}
		button->shakeTimer = 0;
	}
	if(button->reg & IS_PRESS){
		if(button->pressTimer == LONGCLICK_TIME){
			button->reg |= LONGCLICK;
			button->reg &= 0xf0;
		}
		button->pressTimer++;
		button->releaseTimer = 0;
	}else{
		if(button->releaseTimer > EFFECT_TIME){
			if(!(button->reg & LONGCLICK)){
				switch(button->reg & 0x0f){
				case 0x01:
					button->reg |= CLICK;
					break;
				case 0x02:
					button->reg |= DOUBLECLICK;
					break;
				}
				button->reg &= 0xf0;
			}
		}
		button->pressTimer = 0;
		button->releaseTimer++;
	}
}
unsigned char readButtonStatus(buttonType *button,unsigned char flag){
	if(button->reg & flag){
		button->reg &= ~flag;
		return 0x01;
	}
	return 0x00;
}
