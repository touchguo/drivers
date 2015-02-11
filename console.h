/*
 * console.h
 *
 *  Created on: Feb 11, 2015
 *      Author: ChenGuo
 */

#ifndef CONSOLE_H_
#define CONSOLE_H_

typedef struct _command{
	char *cmd;
	void (*fun)(void);
}command;

#define CMD_BUFMAX     16
#define GET_CMD_DELAY  1
#define PARAM_NUM      3

extern volatile unsigned char CmdbufCount;
extern char CmdBuf[CMD_BUFMAX];
extern char *parameter[PARAM_NUM];
extern unsigned char paramNum;

extern command *consoleCMD;
extern command commandTable[];

void ConsolePutChar(char ch);
void ConsolePrint(char *str);
void ConsoleConfig(void);
void GetCmdmand(unsigned char *ch);
unsigned char ReadCommand(void);

void ChangeValueToStr(char mode,char *str,unsigned int *value);
void ChangeStrToValue(char mode,char *str,unsigned int *value);

void Console(void);
void console(char *cmd);
void commandInit(void);


#endif /* CONSOLE_H_ */
