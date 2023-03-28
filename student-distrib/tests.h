#ifndef TESTS_H
#define TESTS_H

#include "lib.h"
#include "x86_desc.h"
#include "rtc.h"
// test launcher
void launch_tests();

void rtc_handler();
int open_test();
int write_test();
#endif /* TESTS_H */
