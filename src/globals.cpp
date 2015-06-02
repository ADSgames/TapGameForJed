#include "globals.h"

int money = 0;
int money_per_click = 1;
int money_per_second = 0;

//Check to see if an area is clicked
bool location_clicked(int min_x,int max_x,int min_y,int max_y){
    if(mouse_x>min_x && mouse_x<max_x && mouse_y>min_y && mouse_y<max_y && mouse_b & 1)
        return true;
    else return false;
}

