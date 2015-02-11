/*
 * timer.c
 *
 *  Created on: Feb 10, 2015
 *      Author: ChenGuo
 */
#include "timer.h"
#include "stm32f10x.h"
#include "ioClass.h"

volatile unsigned int sysTimer = 0;

void TIM1_init(void) {
	TIM_TimeBaseInitTypeDef timerStruct;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

	timerStruct.TIM_Period = 1000; //1ms
	timerStruct.TIM_Prescaler = 72;
	timerStruct.TIM_ClockDivision = 0;
	timerStruct.TIM_CounterMode = TIM_CounterMode_Up;
	timerStruct.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1, &timerStruct);

	TIM_ClearFlag(TIM1, TIM_FLAG_Update);
	TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM1, ENABLE);
}
void TIM2_init(void) {
	TIM_TimeBaseInitTypeDef timerStruct;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	timerStruct.TIM_Period = 1000; //1ms
	timerStruct.TIM_Prescaler = 72;
	timerStruct.TIM_ClockDivision = 0;
	timerStruct.TIM_CounterMode = TIM_CounterMode_Up;
	timerStruct.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &timerStruct);

	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM2, ENABLE);
}
void TIM1_UP_IRQHandler(void) {
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET) {
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
		sysTimer++;
	}
}
void TIM2_UP_IRQHandler(void) {
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		ioClassIRQ();
		if (sysTimer % 100 == 0) {
			if (GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_13)) {
				GPIO_ResetBits(GPIOC, GPIO_Pin_13);
			} else {
				GPIO_SetBits(GPIOC, GPIO_Pin_13);
			}
		}
	}
}

