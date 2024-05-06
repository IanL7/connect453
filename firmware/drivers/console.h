/*
 * console.h
 *
 *  Created on: Jan 21, 2022
 *      Author: Joe Krachey
 */

#ifndef CONSOLE_H_
#define CONSOLE_H_

#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"
#include "cy_retarget_io.h"


#define PIN_CONSOLE_TX 	P6_5
#define PIN_CONSOLE_RX	P6_4


#define DEBUG_MESSAGE_MAX_LEN   (500u)
#define INT_PRIORITY_CONSOLE	3


extern volatile bool ALERT_CONSOLE_RX;

extern char pcInputString[];
extern int16_t cInputIndex;
extern char inputString[];

/* Public Function API */
void console_init(void);

void returnData(char*);

#endif /* CONSOLE_H_ */
