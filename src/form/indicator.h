#pragma once
#ifndef _INDICATOR_H_
#define _INDICATOR_H_

#include <stdint.h>
#include <config/settings.h>


void init_indicator();

void up_btn_led_on();
void down_btn_led_on();
void led_off();

void error_view();


#endif