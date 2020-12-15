#ifndef ITEM_H
#define ITEM_H

#define COINS_PER_SECOND true
#define COINS_PER_CLICK false

#include <string>
#include <sstream>
#include "allegro.h"

class item
{
  public:
    item(int newX, int newY, bool newType, unsigned long long newPrice, unsigned long long newValue, std::string newName);
    void set_image(std::string newImage);
    void draw(BITMAP* tempBitmap, FONT* newFont);
    virtual ~item();

    void update(int newSlaveY);

    int getY();

    std::string longThingy(unsigned long long n, char sep);

    std::string name;

    BITMAP *image;
    int x;
    int y;
    long long value;

    unsigned long long amount;

    unsigned long long price;

    int old_mouse_z;

    int step;
    int step_scroll;
    int slave_y;

    SAMPLE* sound;

    bool type;

  protected:
  private:
};

#endif // ITEM_H
