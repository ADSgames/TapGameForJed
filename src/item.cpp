#include "item.h"
#include "globals.h"

item::item(int newX, int newY, bool newType, int newPrice, int newValue)
{
  x=newX;
  y=newY;
  type = newType;
  price = newPrice;
  value = newValue;
}
void item::set_image(std::string newImage){
  image = load_bitmap( newImage.c_str(), NULL);
}
void item::draw(BITMAP* tempBitmap,FONT* newFont){
    rect(tempBitmap,x+45,y,799,y+40,makecol(0,0,0));
    if(money<price){
            rectfill(tempBitmap,x,y,x+45,y+40,makecol(255,0,0));
    }else{
        rectfill(tempBitmap,x,y,x+45,y+40,makecol(0,255,0));

        textprintf_ex( tempBitmap,newFont, x+10,y+10, makecol(0,0,0), -1, "Buy");
    }
    rect(tempBitmap,x,y,x+45,y+40,makecol(0,0,0));

    textprintf_right_ex( tempBitmap, newFont, x-5,y+10, makecol(0,0,0), -1, "%i",price);
    textprintf_right_ex( tempBitmap, newFont, SCREEN_W-5,y, makecol(0,0,0), -1, "%i JC/S",value);

}

item::~item()
{
  //dtor
}
