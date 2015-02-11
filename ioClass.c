/*
 * ioClass.c
 *
 *  Created on: Feb 10, 2015
 *      Author: ChenGuo
 */
#include "ioClass.h"
#include "stdlib.h"
#include "alloca.h"

io_class *ioOjbB = NULL;
static unsigned char ioOjbNum = 0;
io_option *ioOptT = NULL;
io_option *ioOptB = NULL;

void addOjbIntoList(io_class *ojb) {
	if (ioOjbNum == 0) {
		ioOjbB = ojb;
		ojb->next = ioOjbB;
	} else {
		ojb->next = ioOjbB->next;
		ioOjbB->next = ojb;
		ioOjbB = ojb;
	}
	ioOjbNum++;
}

void ioClassInit(io_class *ojb) {
	ojb->init();
	addOjbIntoList(ojb);
}
void ioClassSet(io_class *ojb, unsigned short width, unsigned short period,
		unsigned char count) {
	ioOptB = (io_option *) malloc(sizeof(io_option));
	if (ioOptB != NULL) {
		ioOptB->timer = 0;
		ioOptB->width = width;
		ioOptB->period = period;
		ioOptB->count = count;
		ioOptB->next = NULL;

		if (ojb->optNum == 0) {
			ioOptT = ioOptB;
			ojb->opt = ioOptT;
		} else {
			ioOptT->next = ioOptB;
			ioOptT = ioOptT->next;
		}
		ojb->optNum++;
	}
}
void ioClassFreeOpt(io_class *ojb) {
	if (ojb->optNum) {
		ojb->optNum--;
		io_option *p = ojb->opt;
		ojb->opt = ojb->opt->next;
		free(p);
	}
}
/*         _______         _______         _______         _______
 * _______|       |_______|       |_______|       |_______|       |_______
 *        |<width>|
 *        |<-----period-->|
 *        |<------------------count------------------------------>|
 */
void ioClassIRQ(void) {
	unsigned char i = ioOjbNum;
	while (i--) {
		ioOjbB = ioOjbB->next;
		if (ioOjbB->optNum) {
			if (ioOjbB->opt->count) {
				if (ioOjbB->opt->timer == 0) {
					ioOjbB->opt->timer++;
					ioOjbB->ioH();
				} else {
					if (ioOjbB->opt->timer == ioOjbB->opt->width) {
						ioOjbB->ioL();
					}
					if (ioOjbB->opt->timer == ioOjbB->opt->period) {
						ioOjbB->opt->timer = 0;
						if (ioOjbB->opt->count == IO_ACTI) {
							ioOjbB->ioH();
						} else {
							ioOjbB->opt->count--;
							if (ioOjbB->opt->count) {
								ioOjbB->ioH();
							} else {
								ioOjbB->ioL();
								ioClassFreeOpt(ioOjbB);
							}
						}
					}
					ioOjbB->opt->timer++;
				}
			}
		}
	}
}

