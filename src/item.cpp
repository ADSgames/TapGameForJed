#include "item.h"

item::item()
{
  //ctor
}
void item::set_image(std::string newImage){
  image = load_bitmap( newImage.c_str(), NULL);
}
void item::draw(BITMAP* tempBitmap){
    draw_sprite(tempBitmap,image,300,20);

}

item::~item()
{
  //dtor
}
