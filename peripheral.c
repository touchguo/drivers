/*
 * peripheral.c
 *
 *  Created on: Feb 10, 2015
 *      Author: ChenGuo
 */
#include "peripheral.h"
#include "stm32f10x.h"
#include "timer.h"

void sysReset(void) {
	__set_FAULTMASK(1);
	NVIC_SystemReset();
}

void NVIC_config(void) {
	NVIC_InitTypeDef nvicStruct;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

	nvicStruct.NVIC_IRQChannel = TIM1_UP_IRQn;
	nvicStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
	nvicStruct.NVIC_IRQChannelSubPriority = 0x08;
	nvicStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvicStruct);

	nvicStruct.NVIC_IRQChannel = TIM2_IRQn;
	nvicStruct.NVIC_IRQChannelSubPriority = 0x01;
	NVIC_Init(&nvicStruct);

//	nvicStruct.NVIC_IRQChannel = USART1_IRQn;
//	nvicStruct.NVIC_IRQChannelSubPriority = 0x02;
//	NVIC_Init(&nvicStruct);
//
//	nvicStruct.NVIC_IRQChannel = USART2_IRQn;
//	nvicStruct.NVIC_IRQChannelSubPriority = 0x03;
//	NVIC_Init(&nvicStruct);
//
//	nvicStruct.NVIC_IRQChannel = USART3_IRQn;
//	nvicStruct.NVIC_IRQChannelSubPriority = 0x04;
//	NVIC_Init(&nvicStruct);

//#ifndef STM32F10X_CL
//	nvicStruct.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
//	nvicStruct.NVIC_IRQChannelSubPriority = 0x05;
//	NVIC_Init(&nvicStruct);
//#else
//	nvicStruct.NVIC_IRQChannel = CAN1_RX0_IRQn;
//	nvicStruct.NVIC_IRQChannelSubPriority = 0x05;
//	NVIC_Init(&nvicStruct);
//	nvicStruct.NVIC_IRQChannel = CAN2_RX0_IRQn;
//	nvicStruct.NVIC_IRQChannelSubPriority = 0x06;
//	NVIC_Init(&nvicStruct);
//#endif
}

void peripheralInit(void) {
	GPIO_InitTypeDef gpioStruct;

	NVIC_config();
	TIM1_init();
	TIM2_init();

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	gpioStruct.GPIO_Pin = GPIO_Pin_13;
	gpioStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	gpioStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &gpioStruct);
}

