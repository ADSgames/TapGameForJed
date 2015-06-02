#include "item.h"

item::item()
{
  //ctor
}
void item::set_image(std::string newImage){
  image = load_bitmap( newImage.c_str(), NULL);
}
item::~item()
{
  //dtor
}
