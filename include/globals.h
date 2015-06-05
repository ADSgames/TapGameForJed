#ifndef GLOBALS_H
#define GLOBALS_H
#include <allegro.h>

extern unsigned long long money;
extern unsigned long long money_per_click;
extern unsigned long long money_per_second;

std::string longToString(unsigned long long newLong);

bool location_clicked(int min_x,int max_x,int min_y,int max_y);

bool location_right_clicked(int min_x,int max_x,int min_y,int max_y);

#endif
