#pragma once
#include "scene.h"
#include "../global.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>

class DeathScene : public Scene {
private:
    ALLEGRO_BITMAP* background;
    ALLEGRO_BITMAP* game_over_img;

    ALLEGRO_SAMPLE* music;
    ALLEGRO_SAMPLE_INSTANCE* sample_instance;

    ALLEGRO_FONT* font;

    int timer;

public:
    DeathScene(int label);
    virtual ~DeathScene();

    virtual void Update() override;
    virtual void Draw() override;
    virtual void Destroy() override;
};
