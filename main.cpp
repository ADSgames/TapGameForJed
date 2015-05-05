#include<allegro.h>
#include<alpng.h>
#include<time.h>

BITMAP* buffer;
BITMAP* coin;

bool close_button_pressed;
bool mouse_pressed;
int money = 1;
int mps = 0;
int slaves;
int slave_cost=100;
int second_timer;

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

// Random number generator. Use int random(highest,lowest);
int random(int newLowest, int newHighest)
{
  int lowest = newLowest, highest = newHighest;
  int range = (highest - lowest) + 1;
  int randomNumber = lowest+int(range*rand()/(RAND_MAX + 1.0));
  return randomNumber;
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

void update(){
    second_timer++;
    if(key[KEY_A]){
        if(money>=100){
            money-=100;
            mps+=10;
            slaves++;
            slave_cost=slave_cost+(slave_cost/2);
        }
    }

     if(mouse_b & 1 && !mouse_pressed){
        mouse_pressed=true;
        money++;


     }
     if(!mouse_b & 1)
        mouse_pressed=false;
    if(second_timer>60){
            second_timer=0;
        money+=mps;
    }
}

void draw(){

    rectfill(buffer,0,0,SCREEN_W,SCREEN_H,makecol(255,255,255));
    textprintf_ex( buffer, font, 20,40, makecol(0,0,0), -1, "JedCoins: %i",money);
    textprintf_ex( buffer, font, 20,60, makecol(0,0,0), -1, "JC/S: %i",mps);
    textprintf_ex( buffer, font, 400,60, makecol(0,0,0), -1, "Slaves: %i",slaves);

    textprintf_ex( buffer, font, 20,100, makecol(0,0,0), -1, "Press A to buy 1 slave: %i$ for 10 JC/S.",slave_cost);
    if(!mouse_pressed)draw_sprite(buffer,coin,200,150);
    if(mouse_pressed)stretch_sprite(buffer,coin,220,170,360,360);
    draw_sprite(screen,buffer,0,0);


}







void setup(){
    buffer=create_bitmap(800,600);


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

   if (!(coin = load_bitmap("images/coin.png", NULL)))
     abort_on_error("Cannot find image coin.png\nPlease check your files and try again");
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
