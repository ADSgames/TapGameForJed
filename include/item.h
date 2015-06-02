#ifndef ITEM_H
#define ITEM_H

#define COINS_PER_SECOND true
#define COINS_PER_CLICK FALSE

#include <string>
#include "allegro.h"

class item
{
  public:
    item(int newX, int newY, bool newType, int newPrice, int newValue, std::string newName);
    void set_image(std::string newImage);
    void draw(BITMAP* tempBitmap, FONT* newFont);
    virtual ~item();

    void update();

    std::string name;

    BITMAP *image;
    int x;
    int y;
    int value;

    int amount;

    int price;

    int step;

    bool type;

  protected:
  private:
};

#endif // ITEM_H
