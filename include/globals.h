#ifndef GLOBALS_H
#define GLOBALS_H
#include <allegro.h>

extern long money;
extern int money_per_click;
extern int money_per_second;

bool location_clicked(int min_x,int max_x,int min_y,int max_y);

bool location_right_clicked(int min_x,int max_x,int min_y,int max_y);

#endif
