#include "aboutScene.h"
#include "sceneManager.h"
#include "../global.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

AboutScene::AboutScene(int label)
    : Scene(label), howtoplay_bg(nullptr)
{
    howtoplay_bg = al_load_bitmap("assets/image/howtoplay.png");
}

AboutScene::~AboutScene()
{
    Destroy();
}

void AboutScene::Update()
{
    // ESC 或滑鼠左鍵 → 回到 Menu
    if (key_state[ALLEGRO_KEY_ESCAPE] || mouse_state[1]) {
        create_scene(Menu_L);
        scene_end = true;
    }
}

void AboutScene::Draw()
{
    al_clear_to_color(al_map_rgb(0, 0, 0));

    if (howtoplay_bg) {
        al_draw_scaled_bitmap(
            howtoplay_bg,
            0, 0,
            al_get_bitmap_width(howtoplay_bg),
            al_get_bitmap_height(howtoplay_bg),
            0, 0,
            WIDTH,
            HEIGHT,
            0
        );
    }
}

void AboutScene::Destroy()
{
    if (howtoplay_bg) {
        al_destroy_bitmap(howtoplay_bg);
        howtoplay_bg = nullptr;
    }
}
