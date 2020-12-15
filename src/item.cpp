#include "item.h"
#include "globals.h"

#include <loadpng.h>

// Item contructor
item::item(int newX,
           int newY,
           bool newType,
           unsigned long long newPrice,
           unsigned long long newValue,
           std::string newName) {
  sound = load_sample("audio/sound_buy.wav");
  x = newX;
  y = newY;
  type = newType;
  price = newPrice;
  value = newValue;
  name = newName;
}

// Returns y to main
int item::getY() {
  return y;
}
// Code from the internet to add commas in variables
std::string item::longThingy(unsigned long long n, char sep = ',') {
  std::stringstream fmt;
  fmt << n;
  std::string s = fmt.str();

  s.reserve(s.length() + s.length() / 3);

  // loop until the end of the string and use j to keep track of every
  // third loop starting taking into account the leading x digits (this probably
  // can be rewritten in terms of just i, but it seems more clear when you use
  // a seperate variable)
  for (unsigned int i = 0, j = 3 - s.length() % 3; i < s.length(); ++i, ++j)
    if (i != 0 && j % 3 == 0)
      s.insert(i++, 1, sep);

  return s;
}

// Loads image
void item::set_image(std::string newImage) {
  if (!(image = load_png(newImage.c_str(), NULL)))
    abort_on_error(
        "Cannot find file audio/sound_click.wav \n Please check your files and "
        "try again");
}

// Draws everything
void item::draw(BITMAP* tempBitmap, FONT* newFont) {
  rect(tempBitmap, x + 45, y, SCREEN_W - 1, y + 40, makecol(0, 0, 0));
  if (money < price) {
    rectfill(tempBitmap, x, y, x + 45, y + 40, makecol(255, 0, 0));
  } else {
    rectfill(tempBitmap, x, y, x + 45, y + 40, makecol(0, 255, 0));

    textprintf_ex(tempBitmap, newFont, x + 10, y + 10, makecol(0, 0, 0), -1,
                  "Buy");
  }
  if (name == "Jed" && amount > 0) {
    rectfill(tempBitmap, x, y, x + 45, y + 40, makecol(0, 0, 200));
  }
  rect(tempBitmap, x, y, x + 45, y + 40, makecol(0, 0, 0));

  //
  textprintf_right_ex(tempBitmap, newFont, x - 5, y + 10, makecol(0, 0, 0), -1,
                      "%s", longThingy(price).c_str());

  if (type == COINS_PER_CLICK)
    textprintf_right_ex(tempBitmap, newFont, SCREEN_W - 5, y,
                        makecol(200, 0, 0), -1, "%s JC/C",
                        longThingy(value).c_str());
  if (type == COINS_PER_SECOND)
    textprintf_right_ex(tempBitmap, newFont, SCREEN_W - 5, y,
                        makecol(0, 100, 0), -1, "%s JC/S",
                        longThingy(value).c_str());

  for (unsigned int i = 0; i < amount; i++) {
    draw_sprite(tempBitmap, image, x + 50 + (i * 25), y + 15);
  }

  textprintf_ex(tempBitmap, newFont, x + 50, y, makecol(0, 0, 0), -1,
                "%ss: %lu", name.c_str(), amount);
}

// Runs logic for buying stuff
void item::update(int newSlaveY) {
  slave_y = newSlaveY;

  step++;
  step_scroll++;

  if (mouse_z > old_mouse_z && slave_y < 0) {
    y += 40;
  }
  if (mouse_z < old_mouse_z && slave_y > -280) {
    y -= 40;
  }

  old_mouse_z = mouse_z;

  if ((name == "Jed" && amount == 0) || name != "Jed") {
    if (location_clicked(x, x + 45, y, y + 40) && step > 10) {
      if (money >= price) {
        play_sample(sound, 255, 125, 1000, 0);
        money -= price;
        amount++;
        if (type == COINS_PER_CLICK) {
          money_per_click += value;
        }
        if (type == COINS_PER_SECOND) {
          money_per_second += value;
        }

        price = price * 1.25;
      }
      step = 0;
    }

    if (location_right_clicked(x, x + 45, y, y + 40)) {
      if (money >= price) {
        play_sample(sound, 255, 125, 1000, 0);
        money -= price;
        amount++;
        if (type == COINS_PER_CLICK) {
          money_per_click += value;
        }
        if (type == COINS_PER_SECOND) {
          money_per_second += value;
        }

        price = price * 1.25;
      }
    }
  }

  if (location_middle_clicked(x, x + 45, y, y + 40) && step > 10) {
  }
}

// Deconstructor
item::~item() {
  destroy_bitmap(image);
}
