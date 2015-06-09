#ifndef ITEM_H
#define ITEM_H

#define COINS_PER_SECOND true
#define COINS_PER_CLICK FALSE

#include <string>
#include <sstream>
#include "allegro.h"

class item
{
  public:
    item(int newX, int newY, bool newType, int newPrice, int newValue, std::string newName);
    void set_image(std::string newImage);
    void draw(BITMAP* tempBitmap, FONT* newFont);
    virtual ~item();

    void update();

    void addY(int newY);




    std::string name;

    BITMAP *image;
    int x;
    int y;
    unsigned long long value;

    unsigned long long amount;

    unsigned long long price;

    int old_mouse_z;

    int step;
    int step_scroll;

    SAMPLE* sound;

    bool type;

  protected:
  private:
};

#endif // ITEM_H
