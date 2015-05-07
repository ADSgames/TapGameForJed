#include<allegro.h>
#include<alpng.h>
#include<time.h>
#include<vector>

BITMAP* buffer;
BITMAP* coin;

BITMAP* slave;
BITMAP* mine;
BITMAP* jedclone;
BITMAP* forge;

FONT *f1,*f2,*f3,*f4,*f5,*slabo_12;

bool close_button_pressed;
bool mouse_pressed;
int money = 1;
int mps = 0;
int mpc = 1;
int slaves;
int slave_cost=100;
int second_timer;
int click;

int mines;
int mine_cost=500;

int jed_clones;
int jed_clone_cost=1000;

int forges;
int forge_cost=1000;

int villages;
int village_cost=12500;

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
    int lifespan;
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

    step++;

    for( int i = 0; i <money_particle.size(); i++){
        //money_particle[i].lifespan++;
       money_particle[i].y--;
        if(money_particle[i].y<0){
            money_particle.erase(money_particle.begin()+i);
        }
    }

    second_timer++;
    if(key[KEY_I])money+=1000;

    if(key[KEY_A] && step>10){
            step=0;
        if(money>=slave_cost){
            money-=slave_cost;
            mps+=10;
            slaves++;
            slave_cost=slave_cost+(slave_cost/4);
        }
    }

    if(key[KEY_S] && step>10){
            step=0;
        if(money>=mine_cost){
            money-=mine_cost;
            mps+=25;
            mines++;
            mine_cost=mine_cost+(mine_cost/4);
        }
    }



    if(key[KEY_D]){
        if(money>=jed_clone_cost){
            money-=jed_clone_cost;
            mpc=mpc*2;
            jed_clones++;
            jed_clone_cost=jed_clone_cost*2;
        }
    }

      if(key[KEY_F]){
        if(money>=forge_cost){
            money-=forge_cost;
            mps+=100;
            forges++;
            forge_cost=forge_cost+(forge_cost/4);
        }
    }

    if(key[KEY_G]){
        if(money>=village_cost){
            money-=village_cost;
            mps+=1000;
            villages++;
            village_cost=village_cost+(village_cost/4);
        }
    }



     if(mouse_b & 1 && !mouse_pressed){
        mouse_pressed=true;
        money+=mpc;
        click++;
        create_money_particle(random(200,600),random(150,500),mpc);



     }
     if(!mouse_b & 1){
        mouse_pressed=false;

     }
    if(second_timer>60){
            second_timer=0;
        money+=mps;

        if(mps!=0)create_money_particle(random(200,600),random(150,500),mps);
    }
}

void draw(){



    rectfill(buffer,0,0,SCREEN_W,SCREEN_H,makecol(255,255,255));
    textprintf_ex( buffer, font, 20,40, makecol(0,0,0), -1, "JedCoins: %i",money);
    textprintf_ex( buffer, font, 20,60, makecol(0,0,0), -1, "JC/S: %i",mps);
    textprintf_ex( buffer, font, 20,70, makecol(0,0,0), -1, "JC/C: %i",mpc);
    textprintf_ex( buffer, font, 20,80, makecol(0,0,0), -1, "Clicks: %i",click);

    textprintf_ex( buffer, font, 500,10, makecol(0,0,0), -1, "Slaves: %i",slaves);
    for( int i = 0; i <slaves; i++){
        draw_sprite(buffer,slave,500+i*20,15);
    }


    textprintf_ex( buffer, font, 500,50, makecol(0,0,0), -1, "Jedcoin Mines: %i",mines);

    for( int i = 0; i <mines; i++){
        draw_sprite(buffer,mine,500+i*27,55);
    }


    textprintf_ex( buffer, font, 500,85, makecol(0,0,0), -1, "Jed Clones %i",jed_clones);

    for( int i = 0; i <jed_clones; i++){
        draw_sprite(buffer,jedclone,500+i*15,90);
    }

    textprintf_ex( buffer, font, 500,130, makecol(0,0,0), -1, "Jedcoin Forges %i",forges);

    for( int i = 0; i <forges; i++){
        draw_sprite(buffer,forge,500+i*25,135);
    }

    textprintf_ex( buffer, font, 500,160, makecol(0,0,0), -1, "Villages %i",villages);



    textprintf_ex( buffer, font, 20,100, makecol(0,0,0), -1, "Press A to buy 1 slave: %i$ for 10 JC/S.",slave_cost);
    textprintf_ex( buffer, font, 20,110, makecol(0,0,0), -1, "Press S to buy 1 Jedcon Mine: %i$ for 25 JC/S.",mine_cost);
    textprintf_ex( buffer, font, 20,120, makecol(0,0,0), -1, "Press D to buy 1 Jed clone: %i$ for %i JC/C.",jed_clone_cost,mpc*2);
    textprintf_ex( buffer, font, 20,130, makecol(0,0,0), -1, "Press F to buy 1 Jedcoin Forge: %i$ for 100 JC/C.",forge_cost);
    textprintf_ex( buffer, font, 20,140, makecol(0,0,0), -1, "Press G to buy 1 Jedcoin Village: %i$ for 1000 JC/C.",village_cost);



    if(!mouse_pressed)draw_sprite(buffer,coin,200,150);
    if(mouse_pressed)stretch_sprite(buffer,coin,220,170,360,360);

    for( int i = 0; i <money_particle.size(); i++){

        textprintf_ex( buffer, font, money_particle[i].x,money_particle[i].y, makecol(0,0,0), -1, "$%i",money_particle[i].value);
    }


    draw_sprite(screen,buffer,0,0);


}







void setup(){
    buffer=create_bitmap(800,600);


     // Load fonts
  f1 = load_font("fonts/slabo_12.pcx", NULL, NULL);
  f2 = extract_font_range(f1, ' ', 'A'-1);
  f3 = extract_font_range(f1, 'A', 'Z');
  f4 = extract_font_range(f1, 'Z'+1, 'z');
  slabo_12 = merge_fonts(f4, f5 = merge_fonts(f2, f3));

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

    if (!(coin = load_bitmap("images/coin.png", NULL)))
     abort_on_error("Cannot find image images/coin.png\nPlease check your files and try again");

    if (!(slave = load_bitmap("images/slave.png", NULL)))
     abort_on_error("Cannot find image images/slaves.png\nPlease check your files and try again");

    if (!(mine = load_bitmap("images/mine.png", NULL)))
     abort_on_error("Cannot find image images/mine.png\nPlease check your files and try again");

    if (!(jedclone = load_bitmap("images/jedclone.png", NULL)))
     abort_on_error("Cannot find image images/jedclone.png\nPlease check your files and try again");

    if (!(forge = load_bitmap("images/forge.png", NULL)))
     abort_on_error("Cannot find image images/forge.png\nPlease check your files and try again");

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
