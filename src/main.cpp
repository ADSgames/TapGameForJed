/*
  _____                           __      __            _____ _
 |  __ \                          \ \    / /           / ____| |
 | |  | | __ _ _ __  _ __  _   _   \ \  / /_ _ _ __   | (___ | |_ ___ _ __ ___  _ __
 | |  | |/ _` | '_ \| '_ \| | | |   \ \/ / _` | '_ \   \___ \| __/ _ \ '_ ` _ \| '_ \
 | |__| | (_| | | | | | | | |_| |    \  / (_| | | | |  ____) | ||  __/ | | | | | |_) |
 |_____/ \__,_|_| |_|_| |_|\__, |     \/ \__,_|_| |_| |_____/ \__\___|_| |_| |_| .__/
                            __/ |                                              | |
                           |___/                                               |_|

  �2015 A.D.S. Games
  adsgames.net

*/

//Imports Allegro game library.
//Provides routines for graphics and sound.
#include<allegro.h>

//Imports Allegro PNG.
//An extension of Allegro to import .png files.
#include<loadpng.h>

//Imports Windows time routines.
//Used to seed the random number generator based on system clock.
#include<time.h>

//Imports vectors, used for the money floating particles when the coin is clicked.
#include<vector>

//Used for file I/O, for rapidxml
#include <fstream>
#include <iostream>

//Attaches these files so it can share variables between item.cpp and main.cpp
#include "item.h"
#include "globals.h"

//Imports rapidxml code to load the items
#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_print.hpp"

//Declares the buffer. Everything is drawn to this bitmap,
//and this bitmap is drawn to the screen.
//Prevents flickering and makes it smooth.
BITMAP* buffer;

//Declares the two images.
//The item images are declared in the item class.
BITMAP* coin;
BITMAP* cursor;

//Declares the sounds used in the game.
SAMPLE* sound_click;
SAMPLE* sound_buy;

//Declares the temporary fonts used to create the other fonts.
FONT *f1,*f2,*f3,*f4,*f5;

//Declares the fonts used in game in different sizes.
FONT *slabo_26, *slabo_20, *slabo_10;

//Used to close the program when the "X" is pressed.
bool close_button_pressed;

//This is true while the mouse is held down.
//It is used to prevent rapid clicking when the mouse is held down on the coin.
bool mouse_pressed;

int slave_y;

//Game variables.
int second_timer;
int click;

// FPS System variables.
volatile int ticks = 0;
const int updates_per_second = 60;
volatile int game_time = 0;

bool won = false;

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

//Handles the "X" button on the window.
void close_button_handler(void){
  close_button_pressed = TRUE;
}
END_OF_FUNCTION(close_button_handler)

//A structure to hold the money particles data.
struct money_particles{
    int x;
    int y;
    unsigned long long value;
};

template <class newType>

newType string_to_number(std::string newString){
  newType result;
  std::stringstream(newString) >> result;
  return result;
}



//A vector of money_particles.
//Allows for infinite amounts of money particles to be used at once.
std::vector<money_particles> money_particle;

std::vector<item> items;



//Random number generator function.
//Used for the random location of money particles
int random(int newLowest, int newHighest)
{
  int lowest = newLowest, highest = newHighest;
  int range = (highest - lowest) + 1;
  int randomNumber = lowest+int(range*rand()/(RAND_MAX + 1.0));
  return randomNumber;
}

//Function to create a money particle.
void create_money_particle(int newX, int newY, int newValue){

    //This takes the arguments from the function and makes a new particle from them.
    money_particles newMoneyParticle;
    newMoneyParticle.x = newX;
    newMoneyParticle.y = newY;
    newMoneyParticle.value = newValue;


    //Puts the new particle into the vector.
    money_particle.push_back(newMoneyParticle);

}
void load_data(){

    rapidxml::xml_document<> doc;
    rapidxml::xml_node<> * root_node;
    // Read the xml file into a vector


    std::ifstream theFile("data/items.xml");
    std::vector<char> xml_buffer((std::istreambuf_iterator<char>(theFile)), std::istreambuf_iterator<char>());
    xml_buffer.push_back('\0');
    // Parse the buffer using the xml file parsing library into doc
    doc.parse<0>(&xml_buffer[0]);
    // Find our root node
    root_node = doc.first_node("data");

    std::string image;
    std::string position;
    std::string type;
    std::string price;
    std::string value;
    std::string name;

    //Iterate over the data
    for (rapidxml::xml_node<> * item_node = root_node->first_node("Item"); item_node; item_node = item_node->next_sibling())
    {

        for(rapidxml::xml_node<> * properties_node = item_node->first_node("Properties"); properties_node; properties_node = properties_node->next_sibling())
        {
          image = properties_node->first_attribute("image")->value();
          position = properties_node->first_attribute("position")->value();
          type = properties_node->first_attribute("type")->value();
          price = properties_node->first_attribute("price")->value();
          value = properties_node->first_attribute("value")->value();
          name = properties_node->first_attribute("name")->value();

          bool type_bool;
          if(type=="COINS_PER_SECOND")
            type_bool = true;
          else
            type_bool = false;


          item newItem(550,40*string_to_number<int>(position),type_bool,(unsigned long long)string_to_number<unsigned long long>(price),string_to_number<unsigned long long>(value),name);
          newItem.set_image("images/"+image);

          items.push_back(newItem);
        }

    }

}



//Code from the internet
std::string number_fmt(unsigned long long n, char sep = ',') {
    std::stringstream fmt;
    fmt << n;
    std::string s = fmt.str();
    s.reserve(s.length() + s.length() / 3);

    // loop until the end of the string and use j to keep track of every
    // third loop starting taking into account the leading x digits (this probably
    // can be rewritten in terms of just i, but it seems more clear when you use
    // a seperate variable)
    for (unsigned int i = 0, j = 3 - s.length() % 3; i < s.length(); ++i, ++j)
        if (i != 0 && j % 3 == 0)
            s.insert(i++, 1, sep);

    return s;
}

//Creates an item based on the item class
//The arguments are x position, y position, money type, initial cost, amount of money received, and name.
item slave(550,0,COINS_PER_SECOND,100,2,"Slave");
item press(550,40,COINS_PER_CLICK,100,2,"JedCoin Press");
item workstation(550,80,COINS_PER_SECOND,750,10,"Workstation");
item mine(550,120,COINS_PER_SECOND,2000,25,"JedCoin Mine");
item forge(550,160,COINS_PER_SECOND,20000,100,"JedCoin Forge");
item jmocrop(550,200,COINS_PER_SECOND,35000,300,"Jedetically Modified Crop");
item clone(550,240,COINS_PER_CLICK,40000,100,"Jed Clone");
item powerplant(550,280,COINS_PER_SECOND,50000,750,"Nuclear Power Plant");
item village(550,320,COINS_PER_SECOND,100000,2000,"Village");
item spacestation(550,360,COINS_PER_SECOND,1250000,15000,"Space Station");
item moon(550,400,COINS_PER_SECOND,2000000,50000,"Moon");
item jedos(550,440,COINS_PER_CLICK,7500000,5000,"JeDOS AI");
item teleporter(550,480,COINS_PER_SECOND,100000000,500000,"Space Teleporter");
item magnet(550,520,COINS_PER_CLICK,150000000,25000,"Electromagnetic Coin Magnifier");
item robot(550,560,COINS_PER_SECOND,500000000,1000000,"Killer Robot");
item darkcoin(550,600,COINS_PER_SECOND,1000000000,3000000,"Dark Matter Coin");
item starship(550,640,COINS_PER_SECOND,2500000000,6000000,"Starship Fleet");
item planet(550,680,COINS_PER_SECOND,7500000000,20000000,"Distant Inhabitable Planet");
item blackhole(550,720,COINS_PER_SECOND,10000000000,80000000,"Black Hole");
item jedsalt(550,760,COINS_PER_SECOND,150000000000,200000000,"Jed Salt");
item jed(550,800,COINS_PER_SECOND,100000000000000,30000000000,"Jed");
item cookie(550,840,COINS_PER_SECOND,7,0,"Depressed Cookie");

//Update loop handles the whole game's logic.
void update(){

    for(unsigned int i=0; i<items.size(); i++){
      items[i].update(slave_y);
    }

    slave_y = slave.getY();

    //Updates each item.
    slave.update(slave_y);
    press.update(slave_y);
    workstation.update(slave_y);
    mine.update(slave_y);
    clone.update(slave_y);
    forge.update(slave_y);
    jmocrop.update(slave_y);
    powerplant.update(slave_y);
    village.update(slave_y);
    spacestation.update(slave_y);
    moon.update(slave_y);
    jedos.update(slave_y);
    teleporter.update(slave_y);
    magnet.update(slave_y);
    jedsalt.update(slave_y);
    robot.update(slave_y);
    darkcoin.update(slave_y);
    starship.update(slave_y);
    planet.update(slave_y);
    blackhole.update(slave_y);
    jed.update(slave_y);
    cookie.update(slave_y);

    if(jed.amount>0 && !won){
        allegro_message("You win!");
        won=true;
    }

    //Iterates through the vector stack and moves them, and if they are off the screen, delete them.
    for( unsigned int i = 0; i <money_particle.size(); i++){
        money_particle[i].y--;
        if(money_particle[i].y<0){
            money_particle.erase(money_particle.begin()+i);
        }
    }

    //The second timer counts to 60 and goes back to 0.
    //The game runs at 60 frames per second, and this counts every frame.
    //After it reaches 60, a second has gone by.
    second_timer++;

    //Gives me money (cheats).
    if(key[KEY_I])
     money+=100000;

    if(key[KEY_O])
      money=money*2;

    //Checks if the coin has been clicked, and adds money per click to total money, and creates a money particle.
    if((location_clicked(10,410,190,600) || location_middle_clicked(10,410,190,600) || location_right_clicked(10,410,190,600)) && !mouse_pressed){
        play_sample(sound_click,255,125,1000,0);
        mouse_pressed=true;
        money+=money_per_click;
        click++;
        create_money_particle(random(30,370),random(230,500),money_per_click);
    }

    //Makes sure one mouse click is one click on the coin (no holding mouse!).
    if(!mouse_b & 1){
        mouse_pressed=false;
    }

    //This runs once every second.
    //It adds the money per second to total money.
    if(second_timer>60){

        second_timer=0;
        money+=money_per_second;

        //If the player is getting money per second, create a money particle.
        if(money_per_second>0)
          create_money_particle(random(30,370),random(230,500),money_per_second);
    }
}

//Draw loop handles drawing everything to the screen.
void draw(){

    //White background.
    rectfill(buffer,0,0,SCREEN_W,SCREEN_H,makecol(255,255,255));

    for(unsigned int i=0; i<items.size(); i++){
      items[i].draw(buffer,slabo_10);
    }



    //Prints stats to the screen.
    //number_fmt(money);
    textprintf_ex( buffer, slabo_20, 5,5, makecol(0,0,0), -1, "JedCoins: %s",number_fmt(money).c_str());
    textprintf_ex( buffer, slabo_20, 5,30, makecol(0,0,0), -1, "JC/S: %s",number_fmt(money_per_second).c_str());
    textprintf_ex( buffer, slabo_20, 5,60, makecol(0,0,0), -1, "JC/C: %s",number_fmt(money_per_click).c_str());
    //textprintf_ex( buffer, font, 20,80, makecol(0,0,0), -1, "Clicks: %i",click);

    //Draws all the items.
    slave.draw(buffer,slabo_10);
    press.draw(buffer,slabo_10);
    workstation.draw(buffer,slabo_10);
    mine.draw(buffer,slabo_10);
    clone.draw(buffer,slabo_10);
    forge.draw(buffer,slabo_10);
    jmocrop.draw(buffer,slabo_10);
    powerplant.draw(buffer,slabo_10);
    village.draw(buffer,slabo_10);
    spacestation.draw(buffer,slabo_10);
    moon.draw(buffer,slabo_10);
    jedos.draw(buffer,slabo_10);
    teleporter.draw(buffer,slabo_10);
    magnet.draw(buffer,slabo_10);
    jedsalt.draw(buffer,slabo_10);
    robot.draw(buffer,slabo_10);
    darkcoin.draw(buffer,slabo_10);
    starship.draw(buffer,slabo_10);
    planet.draw(buffer,slabo_10);
    jed.draw(buffer,slabo_10);
    blackhole.draw(buffer,slabo_10);
    cookie.draw(buffer,slabo_10);


    //Draws the coin if its not clicked.
    if(!mouse_pressed)draw_sprite(buffer,coin,10,190);

    //Draws the coin if it is clicked.
    if(mouse_pressed)stretch_sprite(buffer,coin,30,210,360,360);

    //Iterate through all the money particles in the vector and draws each.

    for(unsigned int i = 0; i <money_particle.size(); i++){
        textprintf_ex( buffer, slabo_10, money_particle[i].x,money_particle[i].y, makecol(0,100,0), -1, "$%s",number_fmt(money_particle[i].value).c_str());
    }


    //Draw the cursor.
    draw_sprite(buffer,cursor,mouse_x,mouse_y);

    //Draw the buffer to the screen.
    //Everything in the game is drawn to the buffer.
    //The buffer is drawn to the screen.
    //This prevents flickering and makes it smoother.
    draw_sprite(screen,buffer,0,0);

}

//Setup function runs once, when the program starts.
//Handles loading images and fonts.
void setup(){

    for(unsigned int i=0; i<items.size(); i++){

    }

    buffer=create_bitmap(SCREEN_W,SCREEN_H);

    // Load fonts from file and make them usable for Allegro.
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

    // Destroy temporary fonts used to create fonts.
    destroy_font(f1);
    destroy_font(f2);
    destroy_font(f3);
    destroy_font(f4);
    destroy_font(f5);

    //Seed random number based on system clock.
    srand(time(NULL));

    //Setup for FPS system.
    LOCK_VARIABLE(ticks);
    LOCK_FUNCTION(ticker);
    install_int_ex(ticker, BPS_TO_TIMER(updates_per_second));

    LOCK_VARIABLE(game_time);
    LOCK_FUNCTION(game_time_ticker);
    install_int_ex(game_time_ticker, BPS_TO_TIMER(10));

    //Setup "X" button.
    LOCK_FUNCTION(close_button_handler);
    set_close_button_callback(close_button_handler);

    //Allows the application to keep running when its out of focus.
    set_display_switch_mode(SWITCH_BACKGROUND);

    //Load the two images from file.
    if (!(coin = load_png("images/coin.png", NULL)))
     abort_on_error("Cannot find image images/coin.png\nPlease check your files and try again");

    if (!(cursor = load_png("images/cursor.png", NULL)))
     abort_on_error("Cannot find image images/cursor.png\nPlease check your files and try again");

    //Load item images from file.
    slave.set_image( "images/slave.png");
    workstation.set_image( "images/workstation.png");
    mine.set_image( "images/mine.png");
    clone.set_image( "images/jedclone.png");
    press.set_image( "images/press.png");
    forge.set_image( "images/forge.png");
    powerplant.set_image( "images/powerplant.png");
    village.set_image( "images/village.png");
    spacestation.set_image( "images/spacestation.png");
    moon.set_image( "images/moon.png");
    jedos.set_image( "images/jedos.png");
    teleporter.set_image( "images/teleporter.png");
    magnet.set_image( "images/magnet.png");
    jedsalt.set_image( "images/jedsalt.png");
    jmocrop.set_image( "images/jmocrop.png");
    robot.set_image( "images/robot.png");
    darkcoin.set_image( "images/darkcoin.png");
    starship.set_image( "images/starship.png");
    planet.set_image( "images/planet.png");
    blackhole.set_image( "images/blackhole.png");
    jed.set_image( "images/jed.png");
    cookie.set_image( "images/cookie.png");

    //Load sounds from file
    if( !(sound_click = load_sample("audio/sound_click.wav")))
        abort_on_error( "Cannot find file audio/sound_click.wav \n Please check your files and try again");

    if( !(sound_buy = load_sample("audio/sound_buy.wav")))
        abort_on_error( "Cannot find file audio/sound_buy.wav \n Please check your files and try again");
}

int main(){

    //Initialize Allegro library.
    allegro_init();

    //Install hardware.
    install_timer();
    install_keyboard();
    install_mouse();

    //Setup graphics.
    set_color_depth(32);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED,1000,600, 0, 0);

    //Setup sound.
    install_sound(DIGI_AUTODETECT,MIDI_AUTODETECT,".");

    set_window_title("Jed Clicker");

    // load_data();

    //Sets up the game.
    setup();

    //Runs the game loop while nothing ends the program.
    while(!key[KEY_ESC] && !close_button_pressed){
        while(ticks == 0){
            rest(1);
        }
        //FPS system.
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
