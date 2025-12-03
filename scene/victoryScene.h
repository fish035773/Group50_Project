#pragma once
#include "scene.h"
#include "../global.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>

class VictoryScene : public Scene {
private:
    ALLEGRO_BITMAP* background;
    ALLEGRO_SAMPLE* music;
    ALLEGRO_SAMPLE_INSTANCE* sample_instance;
    ALLEGRO_FONT* font;

    ALLEGRO_BITMAP* you_win_img;

    int timer;

    // Return button
    ALLEGRO_BITMAP* return_btn_img;
    ALLEGRO_BITMAP* return_btn_hover_img;
    int return_btn_x;
    int return_btn_y;
    bool return_hover;

public:
    VictoryScene(int label);
    virtual ~VictoryScene();

    virtual void Update() override;
    virtual void Draw() override;
    virtual void Destroy() override;

private:
    bool isHover(int x, int y, ALLEGRO_BITMAP* bmp);
    bool isClicked();
};
