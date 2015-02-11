/*
 * console.c
 *
 *  Created on: Feb 11, 2015
 *      Author: ChenGuo
 */
#include "console.h"
#include "stm32f10x.h"
#include "string.h"

void doNothing(void){;}
volatile unsigned char getCmdTimeUp = 0;
volatile unsigned char CmdbufCount = 0;
char CmdBuf[CMD_BUFMAX] = {0};
char *parameter[PARAM_NUM] = {0};
unsigned char paramNum = 0;
command *consoleCMD = commandTable;
command commandTable[] = {

		{0,doNothing},//end of command
};


void ConsolePutChar(char ch){
	USART1->DR = ch;
	while(!(USART1->SR & USART_FLAG_TXE));
}
void ConsolePrint(char *str){
	unsigned int len = 0;
	unsigned int i = 0;
	len = strlen(str);
	for(i=0;i<len;i++){
		if(str[i] == '\n'){
			ConsolePutChar(0x0d);
			ConsolePutChar(0x0a);
		}else{
			ConsolePutChar(str[i]);
		}
	}
}
void USART1_config(unsigned int baudrate){
	USART_InitTypeDef usartStruct;
	GPIO_InitTypeDef gpioStruct;

	RCC->APB2ENR |= RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA |\
			RCC_APB2Periph_USART1;
	//USART1_TX
	gpioStruct.GPIO_Pin = GPIO_Pin_9;
	gpioStruct.GPIO_Speed = GPIO_Speed_50MHz;
	gpioStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA,&gpioStruct);
	//USART1_RX
	gpioStruct.GPIO_Pin = GPIO_Pin_10;
	gpioStruct.GPIO_Speed = GPIO_Speed_50MHz;
	gpioStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA,&gpioStruct);

	usartStruct.USART_BaudRate = baudrate;
	usartStruct.USART_WordLength = USART_WordLength_8b;
	usartStruct.USART_StopBits = USART_StopBits_1;
	usartStruct.USART_Parity = USART_Parity_No;
	usartStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	usartStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART1,&usartStruct);
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	USART_Cmd(USART1,ENABLE);
}
void USART2_config(unsigned int baudrate){
	USART_InitTypeDef usartStruct;
	GPIO_InitTypeDef gpioStruct;

	RCC->APB2ENR |= RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA;
	RCC->APB1ENR |= RCC_APB1Periph_USART2;
	//USART2_TX
	gpioStruct.GPIO_Pin = GPIO_Pin_2;
	gpioStruct.GPIO_Speed = GPIO_Speed_50MHz;
	gpioStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA,&gpioStruct);
	//USART2_RX
	gpioStruct.GPIO_Pin = GPIO_Pin_3;
	gpioStruct.GPIO_Speed = GPIO_Speed_50MHz;
	gpioStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA,&gpioStruct);

	usartStruct.USART_BaudRate = baudrate;
	usartStruct.USART_WordLength = USART_WordLength_8b;
	usartStruct.USART_StopBits = USART_StopBits_1;
	usartStruct.USART_Parity = USART_Parity_No;
	usartStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	usartStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART2,&usartStruct);
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
	USART_Cmd(USART2,ENABLE);
}
void USART3_config(unsigned int baudrate){
	USART_InitTypeDef usartStruct;
	GPIO_InitTypeDef gpioStruct;

	RCC->APB2ENR |= RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB;
	RCC->APB1ENR |= RCC_APB1Periph_USART3;
	//USART2_TX
	gpioStruct.GPIO_Pin = GPIO_Pin_10;
	gpioStruct.GPIO_Speed = GPIO_Speed_50MHz;
	gpioStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA,&gpioStruct);
	//USART2_RX
	gpioStruct.GPIO_Pin = GPIO_Pin_11;
	gpioStruct.GPIO_Speed = GPIO_Speed_50MHz;
	gpioStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA,&gpioStruct);

	usartStruct.USART_BaudRate = baudrate;
	usartStruct.USART_WordLength = USART_WordLength_8b;
	usartStruct.USART_StopBits = USART_StopBits_1;
	usartStruct.USART_Parity = USART_Parity_No;
	usartStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	usartStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART3,&usartStruct);
	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);
	USART_Cmd(USART3,ENABLE);
}

void ConsoleConfig(void){
	USART1_config(115200);
	commandInit();
}
void GetCmdmand(unsigned char *ch){
	CmdBuf[(CmdbufCount++) % CMD_BUFMAX] = *ch;
	getCmdTimeUp = GET_CMD_DELAY;
}
unsigned char ReadCommand(void){
	unsigned char i = 0;
	unsigned char count = 0;
	char str[10] = {0};
	unsigned int temp = 0;
	if(getCmdTimeUp){
		getCmdTimeUp--;
	}else{
		if(CmdbufCount){
			paramNum = 0;
			for(i=0;i<(CmdbufCount % CMD_BUFMAX);i++){
				if((CmdBuf[i] != ' ') && (CmdBuf[i] != '\0')){
					if(count == 0){
						parameter[paramNum++] = &CmdBuf[i];
					}
					count++;
				}else{
					CmdBuf[i] = '\0';
					count = 0;
				}
				if(paramNum == PARAM_NUM){
					ConsolePrint("You are input too many parameter,maximum surport ");
					ChangeValusToStr('d',str,&temp);
					ConsolePrint(str);
					ConsolePrint(" parameters\n");
					break;
				}
			}

			CmdBuf[CmdbufCount % CMD_BUFMAX] = '\0';
			CmdbufCount = 0;
			return 1;
		}
	}
	return 0;
}

void ChangeValueToStr(char mode,char *str,unsigned int *value){

}
void ChangeStrToValue(char mode,char *str,unsigned int *value){

}
unsigned long DJBHash(char *str){
	unsigned long hash = 5381;
	while(*str != '\0'){
		hash += (hash << 5) + *str;
		str++;
	}
	return (hash & 0x7fffffff);
}
unsigned long SDMHash(char *str){
	unsigned long hash = 0;
	while(*str != '\0'){
		hash = *str + (hash << 6) +(hash << 16) - hash;
		str++;
	}
	return (hash & 0x7fffffff);
}
unsigned long ELFHash(char *str){
	unsigned long hash = 0;
	unsigned long x = 0;
	while(*str != '\0'){
		hash = (hash << 4) + *str;
		if((x = hash & 0xf0000000L) != 0){
			hash ^= (x >> 24);
			hash &= ~x;
		}
		str++;
	}
	return (hash & 0x7fffffff);
}

void Console(void){
	if(ReadCommand()){
		console(parameter[0]);
	}
}
void console(char *cmd){
	unsigned long value = 0;
	unsigned char i = 0;

	value = DJBHash(cmd);
	while((consoleCMD+i)->fun != doNothing){
		if((consoleCMD+i)->cmd == (char*)value){
			(consoleCMD+i)->fun();
			break;
		}
		i++;
	}
}
void commandInit(void){
	unsigned long hash = 0;
	unsigned char i = 0;

	consoleCMD = commandTable;
	while((consoleCMD+i)->fun != doNothing){
		hash = DJBHash((consoleCMD+i)->cmd);
		(consoleCMD+i)->cmd = (char*)hash;
		i++;
	}
}

