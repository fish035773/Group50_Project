#ifndef GAMESCENE_H_INCLUDED
#define GAMESCENE_H_INCLUDED
#include "scene.h"
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
/*
   [game scene object]
*/
typedef enum EleType
{
    Floor_L,
    Teleport_L,
    Tree_L,
    Character_L,
    Ball_L,
    Character2_L,
    Projectile_L,
    Projectile_K,
    Projectile_J,
    Enemy_L,
    Projectile_V,
    Projectile_C,
    Projectile_X,
  
    Projectile2_L,
    Projectile3_L,
    Enemy3_L,
    Enemy2_L
} EleType;
typedef struct _GameScene
{
 
    ALLEGRO_FONT *font;  // 👈 新增這行
    // background
    ALLEGRO_BITMAP *background;


    // round images
    ALLEGRO_BITMAP *round_images[3];


    // sounds
    ALLEGRO_SAMPLE *round_sounds[3];
    ALLEGRO_SAMPLE *round_bgm[3];
    ALLEGRO_SAMPLE_INSTANCE *round_bgm_instance;


    // box & gentong (Element images inside the gamescene).
    ALLEGRO_BITMAP *box_image;
    ALLEGRO_BITMAP *gentong_image;


    // round state
    int round_counter;
    bool round_triggered;
    int round_timer;
    bool enemy_defeated;
   

int enemy_defeated_timer;

    bool round_advancing;
    int advance_timer;


    bool round_locked; // freezing the player when the round is changed.
  


    bool enemies_spawned;
   
    
    // scene ptr
    Scene *scene;


} GameScene;
Scene *New_GameScene(int label);
void game_scene_update(Scene *self);
void game_scene_draw(Scene *self);
void game_scene_destroy(Scene *self);
void Add_Elements_to_GameScene(Elements *elem);
#endif
