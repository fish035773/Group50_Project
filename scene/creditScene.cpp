#include "creditScene.h"
#include "sceneManager.h"
#include "../global.h"

// Credit text
static const char* credit_text[] = {
    "Group 50 - Final Project","",
    "Project Members:",
    "吳是悅, 郭宥均",
    "",
    "Music Tracks:","",
    "Menu: Wuthering Waves - A Fairy Tale OST","",
    "Round1: BrunuhVille - Spirit of the Wild","",
    "Round2: Two Steps from Hell - None Shall Live","",
    "Round3: Two Steps from Hell - Unforgiven","",
    "Credit Track: Two Steps from Hell - Never Give Up on Your Dreams",
    "",
    "--- THE END ---",
    nullptr
};

CreditScene::CreditScene(int label)
    : Scene(label),
      background(nullptr),
      music(nullptr),
      music_instance(nullptr),
      font(nullptr),
      scroll_pos(HEIGHT),
      scroll_speed(0.5f),
      scene_timer(0.0f),
      total_text_height(0)
{
    background = al_load_bitmap("assets/image/credit_bg.png");

    music = al_load_sample("assets/sound/credit_music.mp3");
    if (music) {
        music_instance = al_create_sample_instance(music);
        al_set_sample_instance_playmode(music_instance, ALLEGRO_PLAYMODE_LOOP);
        al_attach_sample_instance_to_mixer(music_instance, al_get_default_mixer());
        al_play_sample_instance(music_instance);
    }

    font = al_load_ttf_font("assets/font/NotoSansTC-Regular.ttf", 24, 0);

    int count = 0;
    while (credit_text[count]) count++;
    total_text_height = count * 32;
}

CreditScene::~CreditScene()
{
    Destroy();
}

void CreditScene::Update()
{
    scroll_pos -= scroll_speed;
    scene_timer += 1.0f / FPS;

    if (scroll_pos < -total_text_height) {
        create_scene(Menu_L);
        if(music_instance){
            al_stop_sample_instance(music_instance);
        }
        scene_end = true;
    }

    if (key_state[ALLEGRO_KEY_ESCAPE]) {
        create_scene(Menu_L);
        scene_end = true;
    }
}

void CreditScene::Draw()
{   
   

    if (background)
        al_draw_bitmap(background, 0, 0, 0);

    float y = scroll_pos;

    for (int i = 0; credit_text[i]; i++) {
        al_draw_text(font,
                     al_map_rgb(255,255,255),
                     WIDTH/2,
                     y,
                     ALLEGRO_ALIGN_CENTER,
                     credit_text[i]);
        y += 32;
    }
}

void CreditScene::Destroy()
{
    if (background) al_destroy_bitmap(background);
    if (music_instance) al_destroy_sample_instance(music_instance);
    if (music) al_destroy_sample(music);
    if (font) al_destroy_font(font);
}
