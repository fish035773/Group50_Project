#ifndef VICTORYSCENE_H_INCLUDED
#define VICTORYSCENE_H_INCLUDED

#include "scene.h"
#include "sceneManager.h"
#include "../global.h"
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include <stdbool.h>

typedef struct _VictoryScene
{
    Scene *scene;
    ALLEGRO_BITMAP *background;
    ALLEGRO_SAMPLE *music;
    ALLEGRO_SAMPLE_INSTANCE *sample_instance;
    ALLEGRO_FONT *font;
    ALLEGRO_BITMAP *you_win_img;
    int timer;

    // Return button:
    ALLEGRO_BITMAP *return_btn_img;
    ALLEGRO_BITMAP *return_btn_hover_img;
    int return_btn_x;
    int return_btn_y;
    bool return_hover;
} VictoryScene;

Scene *New_VictoryScene(int label);
void victoryScene_update(Scene *self);
void victoryScene_draw(Scene *self);
void victoryScene_destroy(Scene *self);

#endif