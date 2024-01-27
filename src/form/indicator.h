#pragma once
#ifndef _INDICATOR_H_
#define _INDICATOR_H_

void init_indicator();

/*
 @args: sel - 0 - stop, 1 - up, 2 - down
*/
void led_write(uint8_t sel);


#endif