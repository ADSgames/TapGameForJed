#ifndef ITEM_H
#define ITEM_H

#define COINS_PER_SECOND 0
#define COINS_PER_CLICK 1

#include <string>
#include "allegro.h"

class item
{
  public:
    item(int newX, int newY, bool newType, int newPrice, int newValue, std::string newName);
    void set_image(std::string newImage);
    void draw(BITMAP* tempBitmap, FONT* newFont);
    virtual ~item();

    bool check_click();

    std::string name;

    BITMAP *image;
    int x;
    int y;
    int value;

    int amount;

    int price;
    bool type;

  protected:
  private:
};

#endif // ITEM_H
