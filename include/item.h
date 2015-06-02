#ifndef ITEM_H
#define ITEM_H

#include <string>
#include "allegro.h"

class item
{
  public:
    item();
    void set_image(std::string newImage);
    virtual ~item();

    BITMAP *image;

  protected:
  private:
};

#endif // ITEM_H
