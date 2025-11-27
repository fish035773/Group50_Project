#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include "scene.h"

// Sikka's version
// TODO: adding button features in menu scene
typedef struct Menu {
    // Existing members, original part
    ALLEGRO_FONT* font;
    ALLEGRO_SAMPLE* song;
    ALLEGRO_SAMPLE_INSTANCE* sample_instance;
    ALLEGRO_BITMAP* background;

    // Title
    ALLEGRO_BITMAP* title_img;
    float title_x, title_y;

    // Start button
    ALLEGRO_BITMAP* btn_start;
    ALLEGRO_BITMAP* btn_start_hover;

    // new update 16:57 2025/07/06
    // About button 
    ALLEGRO_BITMAP* btn_about;
    ALLEGRO_BITMAP* btn_about_hover;

    // Quit button
    ALLEGRO_BITMAP* btn_quit;
    ALLEGRO_BITMAP* btn_quit_hover;

    // Button positions
    int btn_x;
    int btn_start_y;
    int btn_about_x;
    int btn_about_y;
    int btn_quit_x;
    int btn_quit_y;

    // Sound
    ALLEGRO_SAMPLE* click_sound;

    // Button click logic
    bool button_clicked;
    int delay_counter;
    int next_window;

} Menu;

Scene *New_Menu(int label);
#endif