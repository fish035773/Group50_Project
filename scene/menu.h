#pragma once
#include "scene.h"
#include "../global.h"
#include "sceneManager.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>

class Menu : public Scene {
private:
    ALLEGRO_FONT* font;
    ALLEGRO_SAMPLE* song;
    ALLEGRO_SAMPLE_INSTANCE* sample_instance;

    // Background
    ALLEGRO_BITMAP* background;

    // Title
    ALLEGRO_BITMAP* title_img;
    float title_x;
    float title_y;

    // Buttons
    ALLEGRO_BITMAP* btn_start;
    ALLEGRO_BITMAP* btn_start_hover;

    ALLEGRO_BITMAP* btn_about;
    ALLEGRO_BITMAP* btn_about_hover;

    ALLEGRO_BITMAP* btn_quit;
    ALLEGRO_BITMAP* btn_quit_hover;

    // Button positions
    int btn_x;
    int btn_start_y;
    int btn_about_x;
    int btn_about_y;
    int btn_quit_x;
    int btn_quit_y;

    // Sound
    ALLEGRO_SAMPLE* click_sound;

    // Click delay
    bool button_clicked;
    int delay_counter;
    SceneType next_window;

public:
    Menu(int label);
    virtual ~Menu();

    virtual void Update() override;
    virtual void Draw() override;
    virtual void Destroy() override;

private:
    bool isHover(int x, int y, ALLEGRO_BITMAP* bmp);
    // 新增文字版滑鼠判定
   
    bool isClicked();
};
