#include<allegro.h>
#include<alpng.h>
#include<time.h>
#include<vector>
#include<cmath>

#include "item.h"
#include "globals.h"

BITMAP* buffer;
BITMAP* coin;

BITMAP* cursor;

FONT *f1,*f2,*f3,*f4,*f5;
FONT *slabo_26, *slabo_20, *slabo_10;

bool close_button_pressed;
bool mouse_pressed;

int money = 0;
int money_per_second=0;
int money_per_click=1;
int second_timer;
int click;

int step;

// FPS System
volatile int ticks = 0;
const int updates_per_second = 60;
volatile int game_time = 0;

int fps;
int frames_done;
int old_time;

void ticker(){
  ticks++;
}
END_OF_FUNCTION(ticker)

void game_time_ticker(){
  game_time++;
}
END_OF_FUNCTION(ticker)

void close_button_handler(void){
  close_button_pressed = TRUE;
}
END_OF_FUNCTION(close_button_handler)

struct money_particles{
    int x;
    int y;
    int value;
};

std::vector<money_particles> money_particle;

// Random number generator. Use int random(highest,lowest);
int random(int newLowest, int newHighest)
{
  int lowest = newLowest, highest = newHighest;
  int range = (highest - lowest) + 1;
  int randomNumber = lowest+int(range*rand()/(RAND_MAX + 1.0));
  return randomNumber;
}
void create_money_particle(int newX, int newY, int newValue){
    money_particles newMoneyParticle;
    newMoneyParticle.x = newX;
    newMoneyParticle.y = newY;
    newMoneyParticle.value = newValue;

    money_particle.push_back(newMoneyParticle);

}
//Check to see if an area is clicked
bool location_clicked(int min_x,int max_x,int min_y,int max_y){
    if(mouse_x>min_x && mouse_x<max_x && mouse_y>min_y && mouse_y<max_y && mouse_b & 1)
        return true;
    else return false;
}

//A function to streamline error reporting in file loading
void abort_on_error(const char *message){
	 set_window_title("Error!");
	 if (screen != NULL){
	    set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
	 }
	 allegro_message("%s.\n %s\n", message, allegro_error);
	 exit(-1);
}

//Declare the items
item slave(450,5,COINS_PER_SECOND,100,2,"Slave");
item mine(450,45,COINS_PER_SECOND,500,25,"JedCoin Mine");
item clone(450,85,COINS_PER_CLICK,100,3,"Jed Clone");
item forge(450,125,COINS_PER_SECOND,1500,100,"JedCoin Forge");
item village(450,165,COINS_PER_SECOND,12500,2000,"Village");
item planet(450,205,COINS_PER_SECOND,1000000,50000,"Planet");





void update(){


    slave.update();
    mine.update();
    clone.update();
    forge.update();
    village.update();
    planet.update();

    step++;

    for( int i = 0; i <money_particle.size(); i++){
       money_particle[i].y--;
        if(money_particle[i].y<0){
            money_particle.erase(money_particle.begin()+i);
        }
    }

    second_timer++;
    if(key[KEY_I])money+=100000;

    if(location_clicked(10,410,190,600) && !mouse_pressed){
        mouse_pressed=true;
        money+=money_per_click;
        click++;
        create_money_particle(random(30,370),random(230,500),money_per_click);



     }
     if(!mouse_b & 1){
        mouse_pressed=false;

     }


    if(second_timer>60){
            second_timer=0;
        money+=money_per_second;

        if(money_per_second!=0)create_money_particle(random(30,370),random(230,500),money_per_second);
    }
}

void draw(){




    rectfill(buffer,0,0,SCREEN_W,SCREEN_H,makecol(255,255,255));
    textprintf_ex( buffer, slabo_26, 5,5, makecol(0,0,0), -1, "JedCoins: %i",money);
    textprintf_ex( buffer, slabo_20, 5,40, makecol(0,0,0), -1, "JC/S: %i",money_per_second);
    textprintf_ex( buffer, slabo_20, 5,70, makecol(0,0,0), -1, "JC/C: %i",money_per_click);
    //textprintf_ex( buffer, font, 20,80, makecol(0,0,0), -1, "Clicks: %i",click);


    slave.draw(buffer,slabo_10);
    mine.draw(buffer,slabo_10);
    clone.draw(buffer,slabo_10);
    forge.draw(buffer,slabo_10);
    village.draw(buffer,slabo_10);
    planet.draw(buffer,slabo_10);


    if(!mouse_pressed)draw_sprite(buffer,coin,10,190);
    if(mouse_pressed)stretch_sprite(buffer,coin,30,210,360,360);

    for( int i = 0; i <money_particle.size(); i++){

        textprintf_ex( buffer, slabo_10, money_particle[i].x,money_particle[i].y, makecol(0,100,0), -1, "$%i",money_particle[i].value);
    }

    draw_sprite(buffer,cursor,mouse_x,mouse_y);

    draw_sprite(screen,buffer,0,0);




}







void setup(){

    buffer=create_bitmap(800,600);


    // Load fonts
    f1 = load_font("fonts/slabo_20.pcx", NULL, NULL);
    f2 = extract_font_range(f1, ' ', 'A'-1);
    f3 = extract_font_range(f1, 'A', 'Z');
    f4 = extract_font_range(f1, 'Z'+1, 'z');
    slabo_20 = merge_fonts(f4, f5 = merge_fonts(f2, f3));

    f1 = load_font("fonts/slabo_10.pcx", NULL, NULL);
    f2 = extract_font_range(f1, ' ', 'A'-1);
    f3 = extract_font_range(f1, 'A', 'Z');
    f4 = extract_font_range(f1, 'Z'+1, 'z');
    slabo_10 = merge_fonts(f4, f5 = merge_fonts(f2, f3));

    f1 = load_font("fonts/slabo_26.pcx", NULL, NULL);
    f2 = extract_font_range(f1, ' ', 'A'-1);
    f3 = extract_font_range(f1, 'A', 'Z');
    f4 = extract_font_range(f1, 'Z'+1, 'z');
    slabo_26 = merge_fonts(f4, f5 = merge_fonts(f2, f3));



    // Destroy temporary fonts
    destroy_font(f1);
    destroy_font(f2);
    destroy_font(f3);
    destroy_font(f4);
    destroy_font(f5);


    srand(time(NULL));

     // Setup for FPS system
    LOCK_VARIABLE(ticks);
    LOCK_FUNCTION(ticker);
    install_int_ex(ticker, BPS_TO_TIMER(updates_per_second));

    LOCK_VARIABLE(game_time);
    LOCK_FUNCTION(game_time_ticker);
    install_int_ex(game_time_ticker, BPS_TO_TIMER(10));

    // Close button
    LOCK_FUNCTION(close_button_handler);
    set_close_button_callback(close_button_handler);

     set_display_switch_mode(SWITCH_BACKGROUND);

    if (!(coin = load_bitmap("images/coin.png", NULL)))
     abort_on_error("Cannot find image images/coin.png\nPlease check your files and try again");

    if (!(cursor = load_bitmap("images/cursor.png", NULL)))
     abort_on_error("Cannot find image images/cursor.png\nPlease check your files and try again");


    slave.set_image( "images/slave.png");
    mine.set_image( "images/mine.png");
    clone.set_image( "images/jedclone.png");
    village.set_image( "images/village.png");
    forge.set_image( "images/forge.png");
    planet.set_image( "images/planet.png");
}






int main(){

  allegro_init();
  alpng_init();
  install_timer();
  install_keyboard();
  install_mouse();
  set_color_depth(32);


  set_gfx_mode(GFX_AUTODETECT_WINDOWED,800,600, 0, 0);
  install_sound(DIGI_AUTODETECT,MIDI_AUTODETECT,".");



  set_window_title("Jed's tap game");
  setup();


      while(!key[KEY_ESC] && !close_button_pressed){
        while(ticks == 0){
            rest(1);
        }
    while(ticks > 0){
        int old_ticks = ticks;

        update();

        ticks--;
        if(old_ticks <= ticks){
            break;
        }
    }
        if(game_time - old_time >= 10){
            fps = frames_done;
            frames_done = 0;
            old_time = game_time;
        }
        draw();
    }


	return 0;
}
END_OF_MAIN()
