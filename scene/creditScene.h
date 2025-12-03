#pragma once
#include "scene.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

class CreditScene : public Scene {
private:
    ALLEGRO_BITMAP* background;

    ALLEGRO_SAMPLE* music;
    ALLEGRO_SAMPLE_INSTANCE* music_instance;

    ALLEGRO_FONT* font;

    float scroll_pos;
    float scroll_speed;
    int total_text_height;

    float scene_timer;  // bonus timer

public:
    CreditScene(int label);
    virtual ~CreditScene();

    virtual void Update() override;
    virtual void Draw() override;
    virtual void Destroy() override;

private:
    int calculateTextHeight();
};
