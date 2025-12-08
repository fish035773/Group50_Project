#include "victoryScene.h"
#include "sceneManager.h"
#include "../global.h"
#include <cstdio>

VictoryScene::VictoryScene(int label): 
    Scene(label),
    background(nullptr),
    music(nullptr),
    sample_instance(nullptr),
    font(nullptr),
    you_win_img(nullptr),
    return_btn_img(nullptr),
    return_btn_hover_img(nullptr),
    return_btn_x(20),
    return_btn_y(20),
    timer(0),
    return_hover(false)
{
    printf("[VictoryScene] Creating Victory Scene...\n");

    // Background
    background = al_load_bitmap("assets/image/victory_bg.png");
    if (!background)
        printf("WARNING: Victory bg NOT FOUND!\n");

    // Music
    music = al_load_sample("assets/sound/victory_music.mp3");
    if (music) {
        sample_instance = al_create_sample_instance(music);
        if (sample_instance) {
            al_set_sample_instance_playmode(sample_instance, ALLEGRO_PLAYMODE_ONCE);
            al_attach_sample_instance_to_mixer(sample_instance, al_get_default_mixer());
            al_play_sample_instance(sample_instance);
        }
    } else {
        printf("WARNING: victory_music.mp3 NOT FOUND!\n");
    }

    // Font
    font = al_load_ttf_font("assets/font/pirulen.ttf", 48, 0);
    if (!font)
        printf("WARNING: Victory font NOT FOUND!\n");

    // YOU WIN image
   // you_win_img = al_load_bitmap("assets/image/you_win.png");
   // if (!you_win_img)
      //  printf("WARNING: you_win.png NOT FOUND!\n");

    // Return button
    //return_btn_img = al_load_bitmap("assets/image/return.png");
   // return_btn_hover_img = al_load_bitmap("assets/image/return_hover.png");
   // if (!return_btn_img || !return_btn_hover_img)
      //  printf("WARNING: Return button images NOT FOUND!\n");

    timer = 0;
}

VictoryScene::~VictoryScene()
{
    Destroy();
}

bool VictoryScene::isHover(int x, int y, ALLEGRO_BITMAP* bmp)
{
    int mx = mouse.x;
    int my = mouse.y;

    int w = al_get_bitmap_width(bmp);
    int h = al_get_bitmap_height(bmp);

    return mx >= x && mx <= x + w &&
           my >= y && my <= y + h;
}

bool VictoryScene::isClicked()
{
    return mouse_state[1];
}

void VictoryScene::Update()
{
    timer++;

    // Hover detection
   // return_hover = isHover(return_btn_x, return_btn_y, return_btn_img);

    // Click → return to menu
  //  if (return_hover && isClicked()) {
    //    create_scene(Menu_L);
    //    scene_end = true;
    //    return;
   // }

    // Auto return after 10 sec
   // if (timer > FPS * 10) {
        //create_scene(Menu_L);
       // scene_end = true;
       // return;
   // }

    // ESC also returns
    if (key_state[ALLEGRO_KEY_ESCAPE]) {
        create_scene(Menu_L);
        if(music){
            al_stop_sample_instance(sample_instance);
            al_destroy_sample_instance(sample_instance);
            printf("[VictoryScene] BGM stopped and destroyed.\n");
        }
        scene_end = true;
    }
}

void VictoryScene::Draw()
{
    // Background
    if (background)
        al_draw_bitmap(background, 0, 0, 0);
    else
        al_clear_to_color(al_map_rgb(0,0,0));

    // YOU WIN image centered
   // if (you_win_img) {
       // float img_w = al_get_bitmap_width(you_win_img);
       // float img_h = al_get_bitmap_height(you_win_img);

      //  float target_w = 500;
      //  float target_h = 100;

       // float pos_x = (WIDTH - target_w) / 2;
       // float pos_y = (HEIGHT - target_h) / 2;

       // al_draw_scaled_bitmap(
          //  you_win_img,
          //  0, 0, img_w, img_h,
          //  pos_x, pos_y,
          //  target_w, target_h,
         //   0
      //  );
   // }

    // Return button
   // if (return_hover)
        //al_draw_bitmap(return_btn_hover_img, return_btn_x, return_btn_y, 0);
   // else
       // al_draw_bitmap(return_btn_img, return_btn_x, return_btn_y, 0);
}

void VictoryScene::Destroy()
{
    if (background) al_destroy_bitmap(background);
    if (music) al_destroy_sample(music);
    if (sample_instance) al_destroy_sample_instance(sample_instance);
    if (font) al_destroy_font(font);
    if (you_win_img) al_destroy_bitmap(you_win_img);
    if (return_btn_img) al_destroy_bitmap(return_btn_img);
    if (return_btn_hover_img) al_destroy_bitmap(return_btn_hover_img);

    background = nullptr;
    music = nullptr;
    sample_instance = nullptr;
    font = nullptr;
    you_win_img = nullptr;
    return_btn_img = nullptr;
    return_btn_hover_img = nullptr;
}
