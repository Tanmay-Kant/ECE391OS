#include "x86_desc.h"
#include "lib.h"
#include "types.h"
#include "i8259.h"
#ifndef RTC_H
#define RTC_H



void rtc_init(); //initializes the rtc
void rtc_handler(); 

int32_t rtc_open(const uint8_t* filename); //opens file and initializes Hz value
int32_t rtc_read(int32_t fd, void* buf, int32_t nbytes); //Reads in data and handles flag
int32_t rtc_write (int32_t fd, const void* buf, int32_t nbytes); //Writes out rate value 
int32_t rtc_close (int32_t fd); //Closes file appropriately
int32_t calculateRate (uint32_t f); //Helper function that calculates the rate given frequency
#endif /* RTC_H */
