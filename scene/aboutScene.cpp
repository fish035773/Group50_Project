#include "aboutScene.h"
#include "sceneManager.h"
#include "../global.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

Scene* New_AboutScene(int label)
{
    Scene* pObj = New_Scene(label);

    // Load background image
    ALLEGRO_BITMAP* howtoplay_bg = al_load_bitmap("assets/image/howtoplay.png");

    // Simpan di pObj->pDerivedObj (boleh, atau langsung pakai global static variable)
    pObj->pDerivedObj = howtoplay_bg;

    // Assign function
    pObj->Update = about_scene_update;
    pObj->Draw = about_scene_draw;
    pObj->Destroy = about_scene_destroy;

    return pObj;
}

void about_scene_update(Scene* self)
{
    // Tekan ESC atau mouse click → balik ke Menu
    if (key_state[ALLEGRO_KEY_ESCAPE] || mouse_state[1])
    {
        create_scene(Menu_L);
    }
}

void about_scene_draw(Scene* self)
{
    // Clear screen
    al_clear_to_color(al_map_rgb(0, 0, 0));

    ALLEGRO_BITMAP* howtoplay_bg = (ALLEGRO_BITMAP*)(self->pDerivedObj);

    // Draw scaled supaya FIT ke window
    al_draw_scaled_bitmap(howtoplay_bg,
        0, 0,
        al_get_bitmap_width(howtoplay_bg), al_get_bitmap_height(howtoplay_bg),
        0, 0,
        WIDTH, HEIGHT,
        0
    );
}

void about_scene_destroy(Scene* self)
{
    ALLEGRO_BITMAP* howtoplay_bg = (ALLEGRO_BITMAP*)(self->pDerivedObj);

    if (howtoplay_bg)
        al_destroy_bitmap(howtoplay_bg);

    free(self);
}