#include "item.h"

item::item(int newX, int newY)
{
  x=newX;
  y=newY;
}
void item::set_image(std::string newImage){
  image = load_bitmap( newImage.c_str(), NULL);
}
void item::draw(BITMAP* tempBitmap){
    draw_sprite(tempBitmap,image,x,y);

}

item::~item()
{
  //dtor
}
