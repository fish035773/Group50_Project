#include "victoryScene.h"

Scene *New_VictoryScene(int label)
{
    VictoryScene *pDerivedObj = (VictoryScene *)malloc(sizeof(VictoryScene));
    Scene *pObj = New_Scene(label);

    printf("[VictoryScene] Creating Victory Scene...\n");

    // Load background
    pDerivedObj->background = al_load_bitmap("assets/image/victory_bg.png");
    if (pDerivedObj->background)
        printf("[VictoryScene] Background loaded OK.\n");
    else
        printf("WARNING: VictoryScene background NOT FOUND!\n");

    // Load music
    pDerivedObj->music = al_load_sample("assets/sound/victory_music.mp3");
    if (pDerivedObj->music) {
        printf("[VictoryScene] Victory music loaded OK.\n");
        pDerivedObj->sample_instance = al_create_sample_instance(pDerivedObj->music);
        if (pDerivedObj->sample_instance) {
            al_set_sample_instance_playmode(pDerivedObj->sample_instance, ALLEGRO_PLAYMODE_ONCE);
            al_attach_sample_instance_to_mixer(pDerivedObj->sample_instance, al_get_default_mixer());
            al_play_sample_instance(pDerivedObj->sample_instance);
            printf("[VictoryScene] Music instance playing.\n");
        } else {
            printf("WARNING: Failed to create sample instance!\n");
        }
    } else {
        printf("WARNING: VictoryScene music NOT FOUND!\n");
        pDerivedObj->sample_instance = NULL;
    }

    // Load font (optional)
    pDerivedObj->font = al_load_ttf_font("assets/font/pirulen.ttf", 48, 0);
    if (pDerivedObj->font)
        printf("[VictoryScene] Font loaded OK.\n");
    else
        printf("WARNING: VictoryScene font NOT FOUND!\n");

    // Load YOU WIN image
    pDerivedObj->you_win_img = al_load_bitmap("assets/image/you_win.png");
    if (pDerivedObj->you_win_img)
        printf("[VictoryScene] YOU WIN image loaded OK.\n");
    else
        printf("WARNING: YOU WIN image NOT FOUND!\n");

    // Load Return button
    pDerivedObj->return_btn_img = al_load_bitmap("assets/image/return.png");
    pDerivedObj->return_btn_hover_img = al_load_bitmap("assets/image/return_hover.png");
    pDerivedObj->return_btn_x = 20;
    pDerivedObj->return_btn_y = 20;
    pDerivedObj->return_hover = false;

    if (pDerivedObj->return_btn_img && pDerivedObj->return_btn_hover_img)
       printf("[VictoryScene] Return button images loaded OK.\n");
    else
        printf("WARNING: Return button images NOT FOUND!\n");

    // Timer setup
    pDerivedObj->timer = 0;

    // Set scene functions
    pObj->pDerivedObj = pDerivedObj;
    pObj->Update = victoryScene_update;
    pObj->Draw = victoryScene_draw;
    pObj->Destroy = victoryScene_destroy;

  //  printf("[VictoryScene] Scene created successfully!\n");

    return pObj;
}

void victoryScene_update(Scene *self)
{
    VictoryScene *obj = (VictoryScene *)(self->pDerivedObj);
    obj->timer++;

    // Cek hover
    int mx = mouse.x;
    int my = mouse.y;
    int btn_w = al_get_bitmap_width(obj->return_btn_img);
    int btn_h = al_get_bitmap_height(obj->return_btn_img);

    obj->return_hover = (mx >= obj->return_btn_x && mx <= obj->return_btn_x + btn_w &&
                         my >= obj->return_btn_y && my <= obj->return_btn_y + btn_h);

    // Cek click
    if (mouse_state[1] && obj->return_hover) {
        self->scene_end = true;
        window = Menu_L;
      //  printf("[VictoryScene] Return button clicked! Going to Menu.\n");
    }

    // Auto back to Menu after 10 sec
    if (obj->timer > FPS * 10)
    {
        self->scene_end = true;
        window = Menu_L;
        //printf("[VictoryScene] Auto-return to Menu after 10 sec.\n");
    }
}

void victoryScene_draw(Scene *self)
{
    VictoryScene *obj = (VictoryScene *)(self->pDerivedObj);

    // Draw background
    if (obj->background) {
        al_draw_bitmap(obj->background, 0, 0, 0);
    } else {
        al_clear_to_color(al_map_rgb(0, 0, 0)); // fallback
    }

    // Draw YOU WIN image CENTERED dynamically
    if (obj->you_win_img) {
        // ukuran PNG
        float img_width = al_get_bitmap_width(obj->you_win_img);
        float img_height = al_get_bitmap_height(obj->you_win_img);

        // target size (500x100)
        float target_width = 500;
        float target_height = 100;

        // posisi tengah
        float pos_x = (WIDTH - target_width) / 2;
        float pos_y = (HEIGHT - target_height) / 2;

        // draw scaled bitmap
        al_draw_scaled_bitmap(
            obj->you_win_img,
            0, 0, img_width, img_height,
            pos_x, pos_y,
            target_width, target_height,
            0
        );
    }
}


void victoryScene_destroy(Scene *self)
{
    VictoryScene *obj = (VictoryScene *)(self->pDerivedObj);

   // printf("[VictoryScene] Destroying scene...\n");

    if (obj->background) al_destroy_bitmap(obj->background);
    if (obj->music) al_destroy_sample(obj->music);
    if (obj->sample_instance) al_destroy_sample_instance(obj->sample_instance);
    if (obj->font) al_destroy_font(obj->font);
    if (obj->you_win_img) al_destroy_bitmap(obj->you_win_img);
    if (obj->return_btn_img) al_destroy_bitmap(obj->return_btn_img);
    if (obj->return_btn_hover_img) al_destroy_bitmap(obj->return_btn_hover_img);

    free(obj);
    free(self);

   // printf("[VictoryScene] Scene destroyed.\n");
}