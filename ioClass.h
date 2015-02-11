/*
 * ioClass.h
 *
 *  Created on: Feb 10, 2015
 *      Author: ChenGuo
 */

#ifndef IOCLASS_H_
#define IOCLASS_H_

#define IO_ACTI 0xff
#define IO_STOP 0x00

typedef struct _io_option
{
  volatile unsigned short timer;
  volatile unsigned short width;
  volatile unsigned short period;
  volatile unsigned char count;
  struct _io_option *next;
}io_option;

typedef struct _io_class
{
  void (*init)(void);
  void (*ioH)(void);
  void (*ioL)(void);
  volatile unsigned char optNum;
  struct _io_option *opt;
  struct _io_class *next;
}io_class;

void ioClassInit(io_class *ojb);
void ioClassSet(io_class *ojb,\
		unsigned short width,\
		unsigned short period,\
		unsigned char count);
void ioClassFreeOpt(io_class *ojb);
void ioClassIRQ(void);

#endif /* IOCLASS_H_ */
