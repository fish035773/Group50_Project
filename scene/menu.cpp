#include "menu.h"
#include "sceneManager.h"
#include "aboutScene.h"
#include "../global.h"
#include <cstdio>

Menu::Menu(int label)
    : Scene(label),
      font(nullptr),
      song(nullptr),
      sample_instance(nullptr),
      background(nullptr),
      title_img(nullptr),
      btn_start(nullptr),
      btn_start_hover(nullptr),
      btn_about(nullptr),
      btn_about_hover(nullptr),
      btn_quit(nullptr),
      btn_quit_hover(nullptr),
      click_sound(nullptr),
      button_clicked(false),
      delay_counter(0),
      next_window(Menu_L)
{
    // Load font
    font = al_load_ttf_font("assets/font/pirulen.ttf", 18, 0);

    // Background
    background = al_load_bitmap("assets/image/main_menu.png");

    // Title
    title_img = al_load_bitmap("assets/image/title.png");
    title_x = -175;
    title_y = 120;

    // Buttons
    btn_start = al_load_bitmap("assets/image/start.png");
    btn_start_hover = al_load_bitmap("assets/image/start_hover.png");

    btn_about = al_load_bitmap("assets/image/about.png");
    btn_about_hover = al_load_bitmap("assets/image/about_hover.png");

    btn_quit = al_load_bitmap("assets/image/quit.png");
    btn_quit_hover = al_load_bitmap("assets/image/quit_hover.png");

    // Positions
    btn_x = 50;
    btn_start_y = 320;
    btn_about_x = btn_x;
    btn_about_y = btn_start_y + 85;
    btn_quit_x = btn_x - 10;
    btn_quit_y = btn_start_y + 175;

    // Music
    song = al_load_sample("assets/sound/menu.mp3");
    sample_instance = al_create_sample_instance(song);
    al_set_sample_instance_playmode(sample_instance, ALLEGRO_PLAYMODE_LOOP);
    al_set_sample_instance_gain(sample_instance, 0.1f);
    al_attach_sample_instance_to_mixer(sample_instance, al_get_default_mixer());

    // Click sound
    click_sound = al_load_sample("assets/sound/click.mp3");  
}

Menu::~Menu()
{
    Destroy();
}

bool Menu::isHover(int x, int y, ALLEGRO_BITMAP* bmp)
{

    int mx = mouse.x;
    int my = mouse.y;
    int w = al_get_bitmap_width(bmp);
    int h = al_get_bitmap_height(bmp);
    bool inside =
        (mx >= x && mx <= x + w &&
        my >= y && my <= y + h);

    return inside;
}

bool Menu::isClicked()
{
    return mouse_state[1];
}

void Menu::Update()
{
    //printf("MENU UPDATE RUNNING\n");
    if (button_clicked) {
        delay_counter++;
        if (delay_counter >= 15) {
            scene_end = true;
            next_scene_requested = true;
            next_scene_type = next_window;

            if (next_window == Exit_L) {
                exit(0);
            } else {
                create_scene(next_window);
            }
        }
        return;
    }

    int mx = mouse.x;
    int my = mouse.y;

    // --- START ---
    if (isHover(btn_x, btn_start_y, btn_start) && isClicked()) {
        al_play_sample(click_sound, 1.0, 0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        button_clicked = true;
        next_window = GameScene_L;  
                        printf("hover=%d click=%d\n", 
       isHover(btn_x, btn_start_y, btn_start),
       isClicked());
        return;

    }

    // --- ABOUT ---
    if (isHover(btn_about_x, btn_about_y, btn_about) && isClicked()) {
        al_play_sample(click_sound, 1.0, 0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        button_clicked = true;
        next_window = About_L;
        return;
    }

    // --- QUIT ---
    if (isHover(btn_quit_x, btn_quit_y, btn_quit) && isClicked()) {
        al_play_sample(click_sound, 1.0, 0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        button_clicked = true;
        next_window = Exit_L;
        return;
    }
}

void Menu::Draw()
{
    al_draw_bitmap(background, 0, 0, 0);

    // Title
    al_draw_bitmap(title_img, title_x, title_y, 0);

    // Buttons
    al_draw_bitmap(isHover(btn_x, btn_start_y, btn_start) ? btn_start_hover : btn_start,
                   btn_x, btn_start_y, 0);

    al_draw_bitmap(isHover(btn_about_x, btn_about_y, btn_about) ? btn_about_hover : btn_about,
                   btn_about_x, btn_about_y, 0);

    al_draw_bitmap(isHover(btn_quit_x, btn_quit_y, btn_quit) ? btn_quit_hover : btn_quit,
                   btn_quit_x, btn_quit_y, 0);

    al_play_sample_instance(sample_instance);
}

void Menu::Destroy()
{
    if (font) al_destroy_font(font);
    if (background) al_destroy_bitmap(background);

    if (song) al_destroy_sample(song);
    if (sample_instance) al_destroy_sample_instance(sample_instance);

    if (title_img) al_destroy_bitmap(title_img);

    if (btn_start) al_destroy_bitmap(btn_start);
    if (btn_start_hover) al_destroy_bitmap(btn_start_hover);

    if (btn_about) al_destroy_bitmap(btn_about);
    if (btn_about_hover) al_destroy_bitmap(btn_about_hover);

    if (btn_quit) al_destroy_bitmap(btn_quit);
    if (btn_quit_hover) al_destroy_bitmap(btn_quit_hover);

    if (click_sound) al_destroy_sample(click_sound);
}
