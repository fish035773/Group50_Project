// Sikka's version:
#include "menu.h"
#include "aboutScene.h"
#include "sceneManager.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include "../global.h"
// new update 16:57 2025/07/06
void menu_update(Scene *self);

void menu_draw(Scene *self);
void menu_destroy(Scene *self);

Scene* New_Menu(int label)
{
    Menu* pDerivedObj = (Menu*)malloc(sizeof(Menu));
    Scene* pObj = New_Scene(label);

    pDerivedObj->font = al_load_ttf_font("assets/font/pirulen.ttf", 18, 0);
    pDerivedObj->background = al_load_bitmap("assets/image/main_menu.png");

    pDerivedObj->title_img = al_load_bitmap("assets/image/title.png");
    pDerivedObj->btn_start = al_load_bitmap("assets/image/start.png");
    pDerivedObj->btn_start_hover = al_load_bitmap("assets/image/start_hover.png");
    // new update 16:57 2025/07/06
    pDerivedObj->btn_about = al_load_bitmap("assets/image/about.png");
    pDerivedObj->btn_about_hover = al_load_bitmap("assets/image/about_hover.png");

    pDerivedObj->btn_quit = al_load_bitmap("assets/image/quit.png");
    pDerivedObj->btn_quit_hover = al_load_bitmap("assets/image/quit_hover.png");

    pDerivedObj->btn_x = 50;               
    pDerivedObj->title_x = -175;     
    pDerivedObj->title_y = 120;

    pDerivedObj->btn_start_y = 320;  
    // new update 16:57 2025/07/06  
    pDerivedObj->btn_about_x = pDerivedObj->btn_x;  
    pDerivedObj->btn_about_y = pDerivedObj->btn_start_y + 55 + 30;

    pDerivedObj->btn_quit_y = pDerivedObj->btn_start_y + 55 + 120;   
    pDerivedObj->btn_quit_x = pDerivedObj->btn_x - 10;


    pDerivedObj->song = al_load_sample("assets/sound/menu.mp3");
    pDerivedObj->sample_instance = al_create_sample_instance(pDerivedObj->song);
    al_set_sample_instance_playmode(pDerivedObj->sample_instance, ALLEGRO_PLAYMODE_LOOP);
    al_attach_sample_instance_to_mixer(pDerivedObj->sample_instance, al_get_default_mixer());
    al_set_sample_instance_gain(pDerivedObj->sample_instance, 0.1f);

    pDerivedObj->click_sound = al_load_sample("assets/sound/click.mp3");
    pDerivedObj->button_clicked = false;
    pDerivedObj->delay_counter = 0;
    pDerivedObj->next_window = 0;

    pObj->pDerivedObj = pDerivedObj;
    pObj->Update = menu_update;
    pObj->Draw = menu_draw;
    pObj->Destroy = menu_destroy;

    return pObj;
}

void menu_update(Scene* self)
{   
    Menu* Obj = (Menu*)(self->pDerivedObj);

    if (Obj->button_clicked) {
        Obj->delay_counter++;
        if (Obj->delay_counter >= 15) {
            self->scene_end = true;
            window = Obj->next_window;
        }
        return;
    }

    int mx = mouse.x;
    int my = mouse.y;

    if (mouse_state[1]) {
        if (mx >= Obj->btn_x && mx <= Obj->btn_x + 180) {
            if (my >= Obj->btn_start_y && my <= Obj->btn_start_y + 55) {
                al_play_sample(Obj->click_sound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                Obj->button_clicked = true;
                Obj->next_window = GameScene_L;
                printf("Next window: %d (GameScene)\n", Obj->next_window);
            }
            else if (my >= Obj->btn_quit_y && my <= Obj->btn_quit_y + 55) {
                al_play_sample(Obj->click_sound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                Obj->button_clicked = true;
                Obj->next_window = -1;
                printf("Next window: %d (Exit)\n", Obj->next_window);
            }
            else if (my >= Obj->btn_about_y && my <= Obj->btn_about_y + 55) {
                al_play_sample(Obj->click_sound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                Obj->button_clicked = true;
                Obj->next_window = About_L;
                printf("Next window: %d (About)\n", Obj->next_window);
            }
        }
        mouse_state[1] = false;
    }
}

void menu_draw(Scene* self)
{
    Menu* Obj = (Menu*)(self->pDerivedObj);

    // Background
    al_draw_bitmap(Obj->background, 0, 0, 0);

    // Title → PNG 380x100
    al_draw_bitmap(Obj->title_img, Obj->title_x, Obj->title_y, 0);

    int mx = mouse.x;
    int my = mouse.y;

    float button_target_width = 180;
    float button_target_height = 55;

    // START BUTTON
    if (mx >= Obj->btn_x && mx <= Obj->btn_x + button_target_width && my >= Obj->btn_start_y && my <= Obj->btn_start_y + button_target_height)
        al_draw_bitmap(Obj->btn_start_hover, Obj->btn_x, Obj->btn_start_y, 0);
    else
        al_draw_bitmap(Obj->btn_start, Obj->btn_x, Obj->btn_start_y, 0);
    // new update 16:57 2025/07/06
    // ABOUT BUTTON
    if (mx >= Obj->btn_about_x && mx <= Obj->btn_about_x + button_target_width &&
        my >= Obj->btn_about_y && my <= Obj->btn_about_y + button_target_height)
        al_draw_bitmap(Obj->btn_about_hover, Obj->btn_about_x, Obj->btn_about_y, 0);
    else
        al_draw_bitmap(Obj->btn_about, Obj->btn_about_x, Obj->btn_about_y, 0);

    // QUIT BUTTON
    if (mx >= Obj->btn_quit_x && mx <= Obj->btn_quit_x + button_target_width && my >= Obj->btn_quit_y && my <= Obj->btn_quit_y + button_target_height)
        al_draw_bitmap(Obj->btn_quit_hover, Obj->btn_quit_x, Obj->btn_quit_y, 0);
    else
        al_draw_bitmap(Obj->btn_quit, Obj->btn_quit_x, Obj->btn_quit_y, 0);

    // Play music
    al_play_sample_instance(Obj->sample_instance);
}


void menu_destroy(Scene* self)
{
    Menu* Obj = (Menu*)(self->pDerivedObj);
    al_destroy_font(Obj->font);
    al_destroy_bitmap(Obj->background);
    al_destroy_sample(Obj->song);
    al_destroy_sample_instance(Obj->sample_instance);
    al_destroy_bitmap(Obj->title_img);
    al_destroy_bitmap(Obj->btn_start);
    al_destroy_bitmap(Obj->btn_start_hover);
    
    // new update 16:57 2025/07/06
    al_destroy_bitmap(Obj->btn_about);
    al_destroy_bitmap(Obj->btn_about_hover);

    al_destroy_bitmap(Obj->btn_quit);
    al_destroy_bitmap(Obj->btn_quit_hover);
    al_destroy_sample(Obj->click_sound);
    free(Obj);
}