#include "item.h"
#include "globals.h"

item::item(int newX, int newY, bool newType, int newPrice, int newValue, std::string newName)
{
  x=newX;
  y=newY;
  type = newType;
  price = newPrice;
  value = newValue;
  name = newName;
}
void item::set_image(std::string newImage){
  image = load_bitmap( newImage.c_str(), NULL);
}
void item::draw(BITMAP* tempBitmap,FONT* newFont){
    rect(tempBitmap,x+45,y,SCREEN_W-1,y+40,makecol(0,0,0));
    if(money<price){
            rectfill(tempBitmap,x,y,x+45,y+40,makecol(255,0,0));
    }else{
        rectfill(tempBitmap,x,y,x+45,y+40,makecol(0,255,0));

        textprintf_ex( tempBitmap,newFont, x+10,y+10, makecol(0,0,0), -1, "Buy");
    }
    rect(tempBitmap,x,y,x+45,y+40,makecol(0,0,0));

    textprintf_right_ex( tempBitmap, newFont, x-5,y+10, makecol(0,0,0), -1, "%i",price);
    if(type==COINS_PER_CLICK)textprintf_right_ex( tempBitmap, newFont, SCREEN_W-5,y, makecol(0,0,0), -1, "%i JC/C",value);
    if(type==COINS_PER_SECOND)textprintf_right_ex( tempBitmap, newFont, SCREEN_W-5,y, makecol(0,0,0), -1, "%i JC/S",value);

    for( int i = 0; i <amount; i++){
         draw_sprite(tempBitmap,image,x+50+(i*25),y+15);
    }

    textprintf_ex( tempBitmap, newFont, x+50,y, makecol(0,0,0), -1, "%ss: %i" ,name.c_str(),amount);


}

void item::update(){
    step++;

    if(location_clicked(x,x+45,y,y+40) && step>10){
        if(money>=price){
            money-=price;
            amount++;
            if(type==COINS_PER_CLICK){
              money_per_click+=value;
            }
            if(type==COINS_PER_SECOND){
              money_per_second+=value;
            }

            price=price*1.25;
        }
        step=0;
    }


}

item::~item()
{
  //dtor
}
