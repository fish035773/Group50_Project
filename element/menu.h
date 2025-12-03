
#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include "scene.h"

//Sikka's version
// TODO: adding button features in menu scene
typedef struct Menu {
    // Existing members, original part
    ALLEGRO_FONT *font;
    ALLEGRO_SAMPLE *song;
    ALLEGRO_SAMPLE_INSTANCE *sample_instance;
    ALLEGRO_BITMAP *background;
    float title_x, title_y;

    // Sikka's TODO: Title images
    ALLEGRO_BITMAP *title_img;

    // Sikka's TODO: Button images
    ALLEGRO_BITMAP *btn_start;
    ALLEGRO_BITMAP *btn_start_hover;
    ALLEGRO_BITMAP *btn_settings;
    ALLEGRO_BITMAP *btn_settings_hover;
    ALLEGRO_BITMAP *btn_quit;
    ALLEGRO_BITMAP *btn_quit_hover;
    // click sounds button
    ALLEGRO_SAMPLE *click_sound; // Sikka's TODO: this is new


    // Sikka's TODO : Button positions
    float btn_x;
    float btn_start_y;
    float btn_settings_y;
    float btn_quit_y;
    // Sikka's TODO : Button sizes
    bool button_clicked;
    int delay_counter;
    int next_window;

} Menu;

Scene *New_Menu(int label);
void menu_update(Scene *self);
void menu_draw(Scene *self);
void menu_destroy(Scene *self);

#endif
