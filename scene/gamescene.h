#pragma once
#include "scene.h"
#include "../element/element.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

/*
   [Element Types] — Keep enum
*/
enum EleType {
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
};

/*
   [GameScene] — Pure C++ Class
   Inherits from Scene
*/
class GameScene : public Scene
{
public:

   // ===== UI Font =====
   ALLEGRO_FONT* font = nullptr;

   // ===== Background =====
   ALLEGRO_BITMAP* background[3] = {nullptr};

   // ===== Round Images =====
   ALLEGRO_BITMAP* round_images[3] = {nullptr};

   // ===== Sounds =====
   ALLEGRO_SAMPLE* round_sounds[3] = {nullptr};
   ALLEGRO_SAMPLE* round_bgm[3]    = {nullptr};
   ALLEGRO_SAMPLE_INSTANCE* round_bgm_instance = nullptr;

   // ===== Element images inside the scene =====
   ALLEGRO_BITMAP* box_image = nullptr;
   ALLEGRO_BITMAP* gentong_image = nullptr;

   // ===== Round state =====
   int  round_counter   = 1;
   bool round_triggered = true;
   int  round_timer     = 0;

   bool enemy_defeated  = false;
   int  enemy_defeated_timer = 0;

   bool round_advancing = false;
   int  advance_timer   = 0;
   
   bool round_locked = false;     // freezing player when round changes
   bool enemies_spawned = false;

   GameScene(int label);
   virtual ~GameScene();

   void Update() override;
   void Draw() override;
};
