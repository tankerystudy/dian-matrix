/*
 *  common_def.c
 *
 *  Common definition functions implementaion file.
 *
 *  Created by tankery.chen@gmail.com
 *  Copyleft @ 2010 Coogle @ Dian Group
 */
#include "common_def.h"


void sleep(int time)
{
    byte i;
    do
    {
        for (i= 0; i < 250; i++)
            ;
    } while (--time != 0);
	LED2 = 0;
}
