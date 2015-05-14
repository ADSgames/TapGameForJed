#include<allegro.h>
#include<alpng.h>
#include<time.h>
#include<vector>
#include<cmath>

BITMAP* buffer;
BITMAP* coin;

BITMAP* slave;
BITMAP* mine;
BITMAP* jedclone;
BITMAP* forge;
BITMAP* village;
BITMAP* planet;

BITMAP* cursor;


FONT *f1,*f2,*f3,*f4,*f5;
FONT *slabo_26, *slabo_20, *slabo_10;

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

int planets;
int planet_cost=1000000;

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

void update(){



    step++;

    for( int i = 0; i <money_particle.size(); i++){
       money_particle[i].y--;
        if(money_particle[i].y<0){
            money_particle.erase(money_particle.begin()+i);
        }
    }

    second_timer++;
    if(key[KEY_I])money+=100000;

    if((key[KEY_A] || location_clicked(450,495,5,45))  && step>10){
            step=0;
        if(money>=slave_cost){
            money-=slave_cost;
            mps+=2;
            slaves++;
            slave_cost=slave_cost+(slave_cost/4);
        }
    }

    if((key[KEY_S] || location_clicked(450,495,45,85)) && step>10){
            step=0;
        if(money>=mine_cost){
            money-=mine_cost;
            mps+=25;
            mines++;
            mine_cost=mine_cost+(mine_cost/4);
        }
    }



    if((key[KEY_D] || location_clicked(450,495,85,125)) && step>10){
            step=0;
        if(money>=jed_clone_cost){
            money-=jed_clone_cost;
            mpc=mpc*2;
            jed_clones++;
            jed_clone_cost=jed_clone_cost*2;
        }
    }

      if((key[KEY_F] || location_clicked(450,495,125,165)) && step>10){
            step=0;
        if(money>=forge_cost){
            money-=forge_cost;
            mps+=100;
            forges++;
            forge_cost=forge_cost+(forge_cost/4);
        }
    }

    if((key[KEY_G] || location_clicked(450,495,165,205)) && step>10){
        step=0;
        if(money>=village_cost){
            money-=village_cost;
            mps+=2000;
            villages++;
            village_cost=village_cost+(village_cost/4);
        }
    }

    if((key[KEY_H] || location_clicked(450,495,205,245)) && step>10){
            step=0;
        if(money>=planet_cost){
            money-=planet_cost;
            mps+=50000;
            planets++;
            planet_cost=planet_cost+(planet_cost/4);
        }
    }



     if(location_clicked(10,410,190,600) && !mouse_pressed){
        mouse_pressed=true;
        money+=mpc;
        click++;
        create_money_particle(random(30,370),random(230,500),mpc);



     }
     if(!mouse_b & 1){
        mouse_pressed=false;

     }


    if(second_timer>60){
            second_timer=0;
        money+=mps;

        if(mps!=0)create_money_particle(random(30,370),random(230,500),mps);
    }
}

void draw(){




    rectfill(buffer,0,0,SCREEN_W,SCREEN_H,makecol(255,255,255));
    textprintf_ex( buffer, slabo_26, 5,5, makecol(0,0,0), -1, "JedCoins: %i",money);
    textprintf_ex( buffer, slabo_20, 5,40, makecol(0,0,0), -1, "JC/S: %i",mps);
    textprintf_ex( buffer, slabo_20, 5,70, makecol(0,0,0), -1, "JC/C: %i",mpc);
    //textprintf_ex( buffer, font, 20,80, makecol(0,0,0), -1, "Clicks: %i",click);



    rect(buffer,495,5,799,45,makecol(0,0,0));
    rect(buffer,495,45,799,85,makecol(0,0,0));
    rect(buffer,495,85,799,125,makecol(0,0,0));
    rect(buffer,495,125,799,165,makecol(0,0,0));
    rect(buffer,495,165,799,205,makecol(0,0,0));
    rect(buffer,495,205,799,245,makecol(0,0,0));


    if(money<slave_cost)rectfill(buffer,450,5,495,45,makecol(255,0,0));
    else{
      rectfill(buffer,450,5,495,45,makecol(0,255,0));
      textprintf_ex( buffer, slabo_10, 460,15, makecol(0,0,0), -1, "Buy");
    }
    rect(buffer,450,5,495,45,makecol(0,0,0));
    textprintf_right_ex( buffer, slabo_10, 445,15, makecol(0,0,0), -1, "%i",slave_cost);
    textprintf_right_ex( buffer, slabo_10, SCREEN_W-5,5, makecol(0,0,0), -1, "2 JC/S");


    //Mines
    if(money<mine_cost)rectfill(buffer,450,45,495,85,makecol(255,0,0));
    else{
      rectfill(buffer,450,45,495,85,makecol(0,255,0));
      textprintf_ex( buffer, slabo_10, 460,55, makecol(0,0,0), -1, "Buy");
    }
    rect(buffer,450,45,495,85,makecol(0,0,0));

    textprintf_right_ex( buffer, slabo_10, 445,55, makecol(0,0,0), -1, "%i",mine_cost);
    textprintf_right_ex( buffer, slabo_10, SCREEN_W-5,45, makecol(0,0,0), -1, "10 JC/S");


    //Jed Clones
    if(money<jed_clone_cost)rectfill(buffer,450,85,495,125,makecol(255,0,0));
    else{
      rectfill(buffer,450,85,495,125,makecol(0,255,0));
      textprintf_ex( buffer, slabo_10, 460,95, makecol(0,0,0), -1, "Buy");
    }
     rect(buffer,450,85,495,125,makecol(0,0,0));

     textprintf_right_ex( buffer, slabo_10, 445,95, makecol(0,0,0), -1, "%i",jed_clone_cost);
    textprintf_right_ex( buffer, slabo_10, SCREEN_W-5,85, makecol(0,0,0), -1, "%4.0f JC/C",pow(2.0,jed_clones+1));



     //Forges
     if(money<forge_cost)rectfill(buffer,450,125,495,165,makecol(255,0,0));
     else{
        rectfill(buffer,450,125,495,165,makecol(0,255,0));
        textprintf_ex( buffer, slabo_10, 460,135, makecol(0,0,0), -1, "Buy");
     }
     rect(buffer,450,125,495,165,makecol(0,0,0));

     textprintf_right_ex( buffer, slabo_10, 445,135, makecol(0,0,0), -1, "%i",forge_cost);
     textprintf_right_ex( buffer, slabo_10, SCREEN_W-5,125, makecol(0,0,0), -1, "100 JC/S");


      //Villages
     if(money<village_cost)rectfill(buffer,450,165,495,205,makecol(255,0,0));
     else{
      rectfill(buffer,450,165,495,205,makecol(0,255,0));
      textprintf_ex( buffer, slabo_10, 460,175, makecol(0,0,0), -1, "Buy");
     }
     rect(buffer,450,165,495,205,makecol(0,0,0));

     textprintf_right_ex( buffer, slabo_10, 445,175, makecol(0,0,0), -1, "%i",village_cost);
     textprintf_right_ex( buffer, slabo_10, SCREEN_W-5,165, makecol(0,0,0), -1, "2000 JC/S");



      //Planets
      if(money<planet_cost)rectfill(buffer,450,205,495,245,makecol(255,0,0));
      else{
        rectfill(buffer,450,205,495,245,makecol(0,255,0));
        textprintf_ex( buffer, slabo_10, 460,215, makecol(0,0,0), -1, "Buy");
      }
      rect(buffer,450,205,495,245,makecol(0,0,0));

      textprintf_right_ex( buffer, slabo_10, 445,215, makecol(0,0,0), -1, "%i",planet_cost);
      textprintf_right_ex( buffer, slabo_10, 445,215, makecol(0,0,0), -1, "");

      textprintf_right_ex( buffer, slabo_10, SCREEN_W-5,205, makecol(0,0,0), -1, "50000 JC/S");


   textprintf_ex( buffer, slabo_10, 500,5, makecol(0,0,0), -1, "Slaves: %i",slaves);

    for( int i = 0; i <slaves; i++){
        draw_sprite(buffer,slave,500+i*20,20);
    }


    textprintf_ex( buffer, slabo_10, 500,45, makecol(0,0,0), -1, "Jedcoin mines: %i",mines);

    for( int i = 0; i <mines; i++){
        draw_sprite(buffer,mine,500+i*27,60);
    }


    textprintf_ex( buffer, slabo_10, 500,85, makecol(0,0,0), -1, "Jed clones: %i",jed_clones);

    for( int i = 0; i <jed_clones; i++){
        draw_sprite(buffer,jedclone,500+i*15,100);
    }

    textprintf_ex( buffer, slabo_10, 500,125, makecol(0,0,0), -1, "Jedcoin forges: %i",forges);

    for( int i = 0; i <forges; i++){
        draw_sprite(buffer,forge,500+i*25,140);
    }

    textprintf_ex( buffer, slabo_10, 500,165, makecol(0,0,0), -1, "Villages: %i",villages);

     for( int i = 0; i <villages; i++){
        draw_sprite(buffer,village,500+i*25,180);
    }


    textprintf_ex( buffer, slabo_10, 500,205, makecol(0,0,0), -1, "Planets: %i",planets);

     for( int i = 0; i <planets; i++){
        draw_sprite(buffer,planet,500+i*25,220);
    }








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

    if (!(slave = load_bitmap("images/slave.png", NULL)))
     abort_on_error("Cannot find image images/slaves.png\nPlease check your files and try again");

    if (!(mine = load_bitmap("images/mine.png", NULL)))
     abort_on_error("Cannot find image images/mine.png\nPlease check your files and try again");

    if (!(jedclone = load_bitmap("images/jedclone.png", NULL)))
     abort_on_error("Cannot find image images/jedclone.png\nPlease check your files and try again");

    if (!(forge = load_bitmap("images/forge.png", NULL)))
     abort_on_error("Cannot find image images/forge.png\nPlease check your files and try again");

    if (!(village = load_bitmap("images/village.png", NULL)))
     abort_on_error("Cannot find image images/village.png\nPlease check your files and try again");

    if (!(planet = load_bitmap("images/planet.png", NULL)))
     abort_on_error("Cannot find image images/planet.png\nPlease check your files and try again");

      if (!(cursor = load_bitmap("images/cursor.png", NULL)))
     abort_on_error("Cannot find image images/cursor.png\nPlease check your files and try again");

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
