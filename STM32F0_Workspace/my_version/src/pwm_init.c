/*
 * pwm_init.c
 *
 *  Created on: Mar 8, 2016
 *      Author: Benjamin
 */


#include "pwm_init.h"
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"


void tim1_pwm_init(void){

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

	    GPIO_InitTypeDef GPIO_InitStruct_PWM;
		GPIO_InitStruct_PWM.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStruct_PWM.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStruct_PWM.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_11|GPIO_Pin_13|GPIO_Pin_14;
		GPIO_InitStruct_PWM.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_InitStruct_PWM.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOE, &GPIO_InitStruct_PWM);

	    TIM_TimeBaseInitTypeDef timerInitStructure;
	    timerInitStructure.TIM_Prescaler = 164;
	    timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	    timerInitStructure.TIM_Period = 25000;
	    timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	    TIM_TimeBaseInit(TIM1, &timerInitStructure);

	    TIM_Cmd(TIM1, ENABLE);

	    TIM_OCInitTypeDef OCInitStructure;
	    OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	    OCInitStructure.TIM_Pulse = 23900;
	    OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	    OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	    OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	    OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	    OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;

	    TIM_OC1Init(TIM1, &OCInitStructure);
	    TIM_OC2Init(TIM1, &OCInitStructure);
	    TIM_OC3Init(TIM1, &OCInitStructure);
	    TIM_OC4Init(TIM1, &OCInitStructure);

	    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
	    TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
	    TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);
	    TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);

	    GPIO_PinAFConfig(GPIOE, GPIO_PinSource9,  GPIO_AF_TIM1);
	    GPIO_PinAFConfig(GPIOE, GPIO_PinSource11,  GPIO_AF_TIM1);
	    GPIO_PinAFConfig(GPIOE, GPIO_PinSource13,  GPIO_AF_TIM1);
	    GPIO_PinAFConfig(GPIOE, GPIO_PinSource14,  GPIO_AF_TIM1);

	    TIM_CtrlPWMOutputs(TIM1, ENABLE);

}
