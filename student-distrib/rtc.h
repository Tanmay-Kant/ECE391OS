#include "x86_desc.h"
#include "lib.h"
#include "types.h"
#include "i8259.h"
#ifndef RTC_H
#define RTC_H



void rtc_init(); //initializes the rtc
void rtc_handler(); 

int32_t open(const uint8_t* filename);
int32_t read(int32_t fd, void* buf, int32_t nbytes);
int32_t write (int32_t fd, const void* buf, int32_t nbytes);
int32_t close (int32_t fd);
int32_t calculateRate (uint32_t f);
#endif /* RTC_H */
