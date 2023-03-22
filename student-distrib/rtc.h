#ifndef RTC_H
#define RTC_H

#include "lib.h"
#include "i8259.h"

void rtc_init(); //initializes the rtc
void rtc_handler(); 

#endif /* RTC_H */
