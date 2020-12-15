#include "globals.h"

// Thing to manipulate strings
#include <sstream>

unsigned long long money = 0;
unsigned long long money_per_click = 1;
unsigned long long money_per_second = 0;

// Function to see if a rectangular area has been clicked.
// Used in the coin and by the item boxes.
bool location_clicked(int min_x, int max_x, int min_y, int max_y) {
  if (mouse_x > min_x && mouse_x < max_x && mouse_y > min_y &&
      mouse_y < max_y && mouse_b & 1)
    return true;
  else
    return false;
}

bool location_right_clicked(int min_x, int max_x, int min_y, int max_y) {
  if (mouse_x > min_x && mouse_x < max_x && mouse_y > min_y &&
      mouse_y < max_y && mouse_b & 2)
    return true;
  else
    return false;
}

bool location_middle_clicked(int min_x, int max_x, int min_y, int max_y) {
  if (mouse_x > min_x && mouse_x < max_x && mouse_y > min_y &&
      mouse_y < max_y && mouse_b & 4)
    return true;
  else
    return false;
}

// A function to make an error message popup box.
// Used if an image is not found.
void abort_on_error(const char* message) {
  set_window_title("Error!");
  if (screen != NULL) {
    set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
  }
  allegro_message("%s.\n %s\n", message, allegro_error);
  exit(-1);
}
