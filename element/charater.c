#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include "charater.h"
#include "projectile.h"
#include "enemy.h"

#include "../scene/sceneManager.h"
#include "../shapes/Rectangle.h"
#include "../algif5/algif.h"
#include "../scene/gamescene.h"
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#define cool_v 600
#define cool_c 480
#define cool_x 300
extern Elements* player = NULL;
/*
   [Character function]
   Create a new character object (Character2), set its state and behavior.
*/
Elements *New_Character(int label)
{
    Character *pDerivedObj = (Character *)malloc(sizeof(Character));
    Elements *pObj = New_Elements(label);

    // Load character animations for 3 states: stop, move, and attack
    char state_string[3][10] = {"stop", "move", "atk1"};
    for (int i = 0; i < 3; i++)
    {
        char buffer[50];
        sprintf(buffer, "assets/image/chara_%s.gif", state_string[i]);
        pDerivedObj->gif_status[i] = algif_new_gif(buffer, -1);
    }

    // Load attack sound effect
    ALLEGRO_SAMPLE *sample = al_load_sample("assets/sound/atk_sound.wav");
    pDerivedObj->atk_Sound = al_create_sample_instance(sample);
    al_set_sample_instance_playmode(pDerivedObj->atk_Sound, ALLEGRO_PLAYMODE_ONCE);
    al_attach_sample_instance_to_mixer(pDerivedObj->atk_Sound, al_get_default_mixer());

    // Initialize character geometry and hitbox
    pDerivedObj->width = pDerivedObj->gif_status[0]->width;
    pDerivedObj->height = pDerivedObj->gif_status[0]->height;
    pDerivedObj->x = 30;
    pDerivedObj->y = HEIGHT - pDerivedObj->height - 20;
    pDerivedObj->hitbox = New_Rectangle(pDerivedObj->x,
                                        pDerivedObj->y,
                                        pDerivedObj->x + pDerivedObj->width,
                                        pDerivedObj->y + pDerivedObj->height);
    pDerivedObj->dir = false; // Direction: false = left, true = right
    pDerivedObj->state = STOP;
    pDerivedObj->new_proj = false;
    pObj->pDerivedObj = pDerivedObj;

    pDerivedObj->ground_y = pDerivedObj->y; // Set ground level for jumping
    pDerivedObj->blood = 100;               // Character health
    pDerivedObj->cool_C = 0;
    pDerivedObj->cool_V = 0;
    pDerivedObj->cool_X = 0;
    pDerivedObj->level = 0;
    pDerivedObj->levelup_points = 0;
    pDerivedObj->add_blood = 10;
    // Set object behavior functions
    pObj->Draw = Character_draw;
    pObj->Update = Character_update;
    pObj->Interact = Character_interact;
    pObj->Destroy = Character_destory;
    return pObj;
}

/*
   Update character state based on key inputs.
   Implements a simple finite state machine.
*/

void trigger_attack(Elements *self, int atk_type)
{   
    Character *chara = (Character *)(self->pDerivedObj);
    Elements *pro = NULL;
    al_play_sample_instance(chara->atk_Sound);
    // 依攻擊類型產生對應的攻擊效果
    switch (atk_type)
    {
        case 1: // J 攻擊：簡單攻擊，無 projectile，可放攻擊音效或特效
            // 例如播放音效：al_play_sample(j_attack_sound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
            if (chara->dir) // 向右攻擊
                {   
                    pro = New_Projectile(Projectile_X, chara->x + chara->width , chara->y + 30, 2, chara);
                }
                else // 向左攻擊
                {   
                
                    pro = New_Projectile(Projectile_X, chara->x - 150, chara->y + 30, -2, chara);
                }
            _Register_elements(scene, pro);
            break;
            break;

        case 2: // C 攻擊：中等攻擊，有 projectile 和額外尾巴效果
            if (chara->dir) // 向右攻擊
            {   
                pro = New_Projectile(Projectile_C, chara->x + chara->width + 10, chara->y + 70, 5, chara);
                _Register_elements(scene, pro);

                Elements *tail = New_Projectile(Projectile_C, chara->x + chara->width + 30, chara->y + 15, 3, chara);
                _Register_elements(scene, tail);
                Elements *tail2 = New_Projectile(Projectile_C, chara->x + chara->width + 30, chara->y + 125, 3, chara);
                _Register_elements(scene, tail2);
            }
            else // 向左攻擊
            {   
                pro = New_Projectile(Projectile_C, chara->x - 120, chara->y + 80, -5, chara);
                _Register_elements(scene, pro);

                Elements *tail = New_Projectile(Projectile_C, chara->x - 100, chara->y + 15, -3, chara);
                _Register_elements(scene, tail);
                Elements *tail2 = New_Projectile(Projectile_C, chara->x - 100, chara->y + 125, -3, chara);
                _Register_elements(scene, tail2);
            }
            // 播放攻擊音效
            // al_play_sample(k_attack_sound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
            break;

        case 3: // V 攻擊：最大攻擊，有 projectile_V
            if (chara->dir) // 向右攻擊
            {   
                pro = New_Projectile(Projectile_V, chara->x + chara->width , chara->y + 10, 5, chara);
            }
            else // 向左攻擊
            {   
                
                pro = New_Projectile(Projectile_V, chara->x - 180, chara->y + 10, -5, chara);
            }
            _Register_elements(scene, pro);

            // 播放攻擊音效
            // al_play_sample(l_attack_sound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
            break;

        default:
            // 無攻擊或錯誤攻擊類型，不做事
            break;
    }
}

void Character_update(Elements *self)
{   
   // printf("Character_update_in\n");
    Character *chara = (Character *)(self->pDerivedObj);
    if(chara->blood <= 0){
        self->dele = true;
    }
    if(chara->blood > 0){
    if(chara->levelup_points >= (chara->level + 1) * 10){
        chara->levelup_points -= (chara->level + 1) * 10;
        chara->level++;
        chara->blood += chara->add_blood;
    }
    enemy_charater(chara->x);
    if(chara->cool_C > 0){
        chara->cool_C--;
    }
    if(chara->cool_V > 0){
        chara->cool_V--;
    }
    if(chara->cool_X > 0){
        chara->cool_X--;
    }
    // 跳躍狀態下：處理跳躍 + 左右移動 + 空中攻擊
    if (chara->is_jumping)
    {
        double elapsed = al_get_time() - chara->jump_start_time;

        // 垂直位移：上升或下降
        if (key_state[ALLEGRO_KEY_W] && elapsed < 0.5)
            _Character_update_position(self, 0, -5);
        else if (chara->y < chara->ground_y)
            _Character_update_position(self, 0, 5);

        // 水平位移：允許邊跳邊移動
        int dx = 0;
        if (key_state[ALLEGRO_KEY_A])  { dx -= 5; chara->dir = false; }
        if (key_state[ALLEGRO_KEY_D]) { dx += 5; chara->dir = true; }
        _Character_update_position(self, dx, 0);

        // 落地
        if (chara->y >= chara->ground_y)
        {
            chara->y = chara->ground_y;
            chara->is_jumping = false;
            _Character_update_position(self, 0, 0);
        }

        // 空中攻擊觸發（只觸發一次）
        int new_atk_type = 0;
        if (key_state[ALLEGRO_KEY_X] && chara->cool_X == 0) new_atk_type = 1, chara->cool_X = cool_x;
        else if (key_state[ALLEGRO_KEY_C]) new_atk_type = 2, printf("a");
        else if (key_state[ALLEGRO_KEY_V]) new_atk_type = 3;

        if (!chara->new_proj && new_atk_type != 0)
        {
            chara->atk_type = new_atk_type;
            chara->state = ATK;
            trigger_attack(self, chara->atk_type);
            chara->new_proj = true;
        }

        return; // 空中狀態處理完畢
    }

    // 非跳躍狀態下根據狀態機更新
    switch (chara->state)
    {
        case STOP:
            if (key_state[ALLEGRO_KEY_X] && chara->cool_X == 0) { chara->state = ATK; chara->atk_type = 1; chara->new_proj = false; chara->cool_X = cool_x;}
            else if (key_state[ALLEGRO_KEY_C] && chara->cool_C == 0) { chara->state = ATK; chara->atk_type = 2; chara->new_proj = false; chara->cool_C = cool_c;}
            else if (key_state[ALLEGRO_KEY_V] && chara->cool_V == 0) { chara->state = ATK; chara->atk_type = 3; chara->new_proj = false; chara->cool_V = cool_v;}
            else if (key_state[ALLEGRO_KEY_A]) { chara->dir = false; chara->state = MOVE; }
            else if (key_state[ALLEGRO_KEY_D] || key_state[ALLEGRO_KEY_W] || key_state[ALLEGRO_KEY_S]) {
                chara->dir = true; chara->state = MOVE;
            }
            break;

        case MOVE:
        {
            int dx = 0;
            if (key_state[ALLEGRO_KEY_A]) { dx -= 5; chara->dir = false; }
            if (key_state[ALLEGRO_KEY_D]) { dx += 5; chara->dir = true; }
            _Character_update_position(self, dx, 0);

            // 跳躍觸發
            if (key_state[ALLEGRO_KEY_W] && !chara->is_jumping && chara->y == chara->ground_y)
            {
                chara->is_jumping = true;
                chara->jump_start_time = al_get_time();
                return;
            }

            // 進入攻擊狀態
            if (key_state[ALLEGRO_KEY_X] && chara->cool_X == 0) { chara->state = ATK; chara->atk_type = 1; chara->new_proj = false; chara->cool_X = cool_x ;}
            else if (key_state[ALLEGRO_KEY_C] && chara->cool_C == 0) { chara->state = ATK; chara->atk_type = 2; chara->new_proj = false; chara->cool_C = cool_c;}
            else if (key_state[ALLEGRO_KEY_V] && chara->cool_V == 0) { chara->state = ATK; chara->atk_type = 3; chara->new_proj = false; chara->cool_V = cool_v;}

            if (chara->gif_status[chara->state]->done)
                chara->state = STOP;
        }
        break;

        case ATK:
        {
            //printf("ATK2");
            // 攻擊動畫特定幀觸發 projectile
            if (chara->gif_status[ATK]->display_index == 2 && !chara->new_proj)
            {
                trigger_attack(self, chara->atk_type);
                chara->new_proj = true;
            }

            // 攻擊動畫結束後重置狀態
            if (chara->gif_status[chara->state]->done)
            {
                if (chara->is_jumping)
                    chara->state = MOVE; // 可改為 JUMP 狀態
                else
                    chara->state = STOP;

                chara->new_proj = false;
                chara->atk_type = 0;
            }
        }
        break;
    }
    }
//printf("Character_update_out\n");
}



/*
   Draw character based on current state and frame.
*/
void Character_draw(Elements *self)
{
    Character *chara = ((Character *)(self->pDerivedObj));
    ALLEGRO_BITMAP *frame = algif_get_bitmap(chara->gif_status[chara->state], al_get_time());
    if (frame)
    {
        al_draw_bitmap(frame, chara->x, chara->y, ((chara->dir) ? ALLEGRO_FLIP_HORIZONTAL : 0));
    }

    // Play sound when projectile is created during attack animation
    //if (chara->state == ATK2 && chara->gif_status[chara->state]->display_index == 2)
    //{
    //    al_play_sample_instance(chara->atk_Sound);
    //}
}

/*
   Clean up all resources used by character.
*/
void Character_destory(Elements *self)
{
    Character *Obj = ((Character *)(self->pDerivedObj));
    al_destroy_sample_instance(Obj->atk_Sound);
   for (int i = 0; i < 3; i++)
       algif_destroy_animation(Obj->gif_status[i]);
    free(Obj->hitbox);
    free(Obj);
    free(self);

    printf("free : character");
}

/*
   Helper function to update character position and hitbox.
*/
void _Character_update_position(Elements *self, int dx, int dy)
{
    Character *chara = ((Character *)(self->pDerivedObj));
    chara->x += dx;
    chara->y += dy;
    Shape *hitbox = chara->hitbox;
    hitbox->update_center_x(hitbox, dx);
    hitbox->update_center_y(hitbox, dy);
}

/*
   Placeholder for character interactions with other elements.
*/
void Character_interact(Elements *self) {}
