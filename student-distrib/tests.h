#ifndef TESTS_H
#define TESTS_H

#include "lib.h"
#include "x86_desc.h"
#include "rtc.h"
#include "file_sys.h"
// test launcher
void launch_tests();

void rtc_handler(); //rtc_handler function in rtc.c
int open_test(); //test case for RTC open function (default hertz)
int write_test(); //test case for RTC write function (changing hertz)
#endif /* TESTS_H */
