#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include "enemy2.h"

#include "projectile2.h"
#include "../scene/sceneManager.h"
#include "../shapes/Rectangle.h"
#include "../algif5/algif.h"
#include "../scene/gamescene.h"
#include "../element/charater.h"
#include "../element/projectile2.h"
#include <stdio.h>
#include <stdbool.h>
#define maxhp 50
/*
   [Character function]
*/
Elements *New_Enemy2(int label)
{
    Enemy2 *pDerivedObj = (Enemy2 *)malloc(sizeof(Enemy2));
    Elements *pObj = New_Elements(label);
    // setting derived object member
    // load character images
    char state_string[4][10] = {"stop", "move", "attack", "dead"};
    for (int i = 0; i < 4; i++)
    {
        char buffer[100];
        sprintf(buffer, "assets/image/enemy2_%s.gif", state_string[i]);
        printf("Loading GIF: %s\n", buffer);
        pDerivedObj->gif_status[i] = algif_load_animation(buffer);
        if (!pDerivedObj->gif_status[i]) {
            printf("Failed to load GIF: %s\n", buffer);
        }
    }
    // load effective sound
    ALLEGRO_SAMPLE *sample = al_load_sample("assets/sound/enemy_sword_atk_sound.mp3");
    pDerivedObj->atk_sound = al_create_sample_instance(sample);
    al_set_sample_instance_playmode(pDerivedObj->atk_sound, ALLEGRO_PLAYMODE_ONCE);
    al_attach_sample_instance_to_mixer(pDerivedObj->atk_sound, al_get_default_mixer());


    // initial the geometric information of character
    pDerivedObj->width = pDerivedObj->gif_status[0]->width;
    pDerivedObj->height = pDerivedObj->gif_status[0]->height;
    pDerivedObj->x = 400;
    pDerivedObj->y = HEIGHT - pDerivedObj->height - 60;
    pDerivedObj->hitbox = New_Rectangle(pDerivedObj->x,
                                        pDerivedObj->y,
                                        pDerivedObj->x + pDerivedObj->width,
                                        pDerivedObj->y + pDerivedObj->height);
    pDerivedObj->dir = true; // true: face to right, false: face to left
    // initial the animation component
    pDerivedObj->state = ENEMY2_IDLE;
    pDerivedObj->active_proj = false;
    pDerivedObj->move_counter = 0;


    pObj->pDerivedObj = pDerivedObj;
    // setting derived object function
    pObj->Draw = Enemy2_draw;
    pObj->Update = Enemy2_update;
    pObj->Interact = Enemy2_interact;
    pObj->Destroy = Enemy2_destroy;


    pDerivedObj->hp = maxhp ;              
    pDerivedObj->alive = true;
    pDerivedObj->got_hit = false;
    pDerivedObj->hit_time = 0;
    pDerivedObj->dying = false;
    pDerivedObj->death_time = 0;


    pDerivedObj->spawn_time = al_get_time();
    pDerivedObj->can_attack = false;
    pDerivedObj->chasing = false;
    printf("enemy2 inited");
    return pObj;
}
int player_center_x2;
void enemy2_charater(int dx){
    
    player_center_x2 =  dx;
}
void Enemy2_update(Elements *self)
{
    // use the idea of finite state machine to deal with different state
    Enemy2 *enemy2 = ((Enemy2 *)(self->pDerivedObj));
    extern Elements *player;
    if(enemy2->hp > 0){

    int speed = 3;


    // Calculate distance between enemy2 and player on x-axis (horizontal)
    int enemy2_center_x = enemy2->x + enemy2->width / 2;
    
    int dx = player_center_x2 - enemy2_center_x;




    double current_time = al_get_time();
    if (!enemy2->can_attack && current_time - enemy2->spawn_time >= 1.0) {
        enemy2->can_attack = true; // allow attacking after 1 second
    }




    // **Always check attack range first regardless of state**
    if (enemy2->can_attack && abs(dx) <= ENEMY2_ATTACK_RANGE)
    {
        enemy2->dir = dx >= 0;


        // If not already attacking, switch to attack
        if (enemy2->state != ENEMY2_ATK)
        {
            enemy2->state = ENEMY2_ATK;
            enemy2->active_proj = false;
            enemy2->move_counter = 0;
        }
    }
   
    if (enemy2->state == ENEMY2_DEAD) {
        if (enemy2->death_time == 0)
            enemy2->death_time = al_get_time(); // mark when death started
           
        else if (enemy2->gif_status[ENEMY2_DEAD]->done && al_get_time() - enemy2->death_time >= 1.0) // wait 1 second
            self->dele = true;
        return;
    }
    else if (enemy2->state == ENEMY2_IDLE)
    {
        if (abs(dx) <= ENEMY2_ATTACK_RANGE)
        {
            enemy2->chasing = true;
            enemy2->state = ENEMY2_ATK;
            enemy2->active_proj = false;
            enemy2->move_counter = 0;
        }
        else
        {
            enemy2->state = ENEMY2_MOVE;
        }
    }
    else if (enemy2->state == ENEMY2_MOVE)
    {
        if (enemy2->chasing) {
        // Chase player regardless of patrol area
        enemy2->dir = (dx >= 0);
        int move_speed = (dx > 0) ? speed : -speed;
        _Enemy2_update_position(self, move_speed, 0);
    } else {
        // Normal patrol between 400 and 700
        if (enemy2->dir == false) {
            _Enemy2_update_position(self, -speed, 0);
            if (enemy2->x < 400)
                enemy2->dir = true;
        } else {
            _Enemy2_update_position(self, speed, 0);
            if (enemy2->x > 700)
                enemy2->dir = false;
        }
    }  
    }
    else if (enemy2->state == ENEMY2_ATK)
    {


        int current_frame = enemy2->gif_status[ENEMY2_ATK]->display_index;




        // Launch projectile once at frame 2
        if (current_frame == 2 && !enemy2->active_proj)
        {
            Elements *pro;
            int projectile_x;
            if (enemy2->dir) {
            projectile_x = enemy2->x +enemy2->width - 100;  // Or enemy_center_x - offset
            } else {
                projectile_x = enemy2->x + 20;  // symmetric to right
            }




            pro = New_Projectile2(Projectile2_L,
                    projectile_x,
                    enemy2->y + enemy2->height / 2 - 20,
                    enemy2->dir ? 5 : -5);


       
            ((Projectile2 *)pro->pDerivedObj)->is_enemy_projectile = true;
            _Register_elements(scene, pro);
            enemy2->active_proj = true;
        }
        // When attack animation finishes, transition based on distance
        if (enemy2->gif_status[ENEMY2_ATK]->done)
        {
            enemy2->gif_status[ENEMY2_ATK]->done = false;  // reset animation
            enemy2->gif_status[ENEMY2_ATK]->display_index = 0; // restart from first frame
            enemy2->active_proj = false;


            // If player still in attack range, repeat attack
            if (abs(dx) <= ENEMY2_ATTACK_RANGE) {
                enemy2->chasing = true;
                enemy2->state = ENEMY2_ATK; // stay in attack
            }
            else {
                enemy2->chasing = true; // still chasing
                enemy2->state = ENEMY2_MOVE; // go chase the player
            }
        }
    }
}
}
void Enemy2_draw(Elements *self)
{
    // with the state, draw corresponding image
    Enemy2 *enemy2 = ((Enemy2 *)(self->pDerivedObj));
    ALLEGRO_BITMAP *frame = algif_get_bitmap(enemy2->gif_status[enemy2->state], al_get_time());
    if(enemy2->hp > 0){
    if (frame)
    {
        al_draw_bitmap(frame, enemy2->x, enemy2->y, (enemy2->dir ? 0 : ALLEGRO_FLIP_HORIZONTAL));
    }
    if (enemy2->state == ENEMY2_ATK && enemy2->gif_status[enemy2->state]->display_index == 2)
    {
        al_play_sample_instance(enemy2->atk_sound);
    }




    // Draw the red HP bar
    int bar_width = 100;
    int bar_height = 10;
    int bar_x = enemy2->x + 50;
    int bar_y = enemy2->y; // position above the enemy2




    float hp_ratio = (float)enemy2->hp / maxhp; // max HP = 3, change if needed
    al_draw_filled_rectangle(bar_x, bar_y, bar_x + (int)(bar_width * 1),
                           bar_y + bar_height, al_map_rgb(255, 255, 255));
    al_draw_filled_rectangle(bar_x, bar_y, bar_x + (int)(bar_width * hp_ratio), bar_y + bar_height, al_map_rgb(150, 0, 0));




    if (enemy2->state == ENEMY2_ATK && enemy2->gif_status[enemy2->state]->display_index == 2)
    {
        al_play_sample_instance(enemy2->atk_sound);
    }




    if (enemy2->got_hit && al_get_time() - enemy2->hit_time < 0.2) {
        al_draw_tinted_bitmap(frame, al_map_rgba(255, 0, 0, 200), enemy2->x, enemy2->y, (enemy2->dir ? 0 : ALLEGRO_FLIP_HORIZONTAL));
    } else {
        al_draw_bitmap(frame, enemy2->x, enemy2->y, (enemy2->dir ? 0 : ALLEGRO_FLIP_HORIZONTAL));
    }


    }

}
void Enemy2_destroy(Elements *self)
{
    Enemy2 *enemy2 = ((Enemy2 *)(self->pDerivedObj));
    al_destroy_sample_instance(enemy2->atk_sound);
    for (int i = 0; i < 4; i++)
        algif_destroy_animation(enemy2->gif_status[i]);
    free(enemy2->hitbox);
    free(enemy2);
    free(self);
}




void _Enemy2_update_position(Elements *self, int dx, int dy)
{
    Enemy2 *enemy2 = ((Enemy2 *)(self->pDerivedObj));
    enemy2->x += dx;
    enemy2->y += dy;
    Shape *hitbox = enemy2->hitbox;
    hitbox->update_center_x(hitbox, dx);
    hitbox->update_center_y(hitbox, dy);
}




void Enemy2_interact(Elements *self)
{
    Enemy2 *enemy2 = (Enemy2 *)(self->pDerivedObj);
    if (!enemy2->alive || enemy2->dying) return;


    extern Scene *scene;
    for (int i = 0; i < MAX_ELEMENT; i++) {
        EPNode *node = scene->ele_list[i];
        while (node != NULL) {
            Elements *obj = node->ele;
            EPNode *next_node = node->next;


            if (obj->label == Projectile2_Right || obj->label == Projectile2_Left || obj->label == Projectile2_L) {
                Projectile2 *proj = (Projectile2 *)(obj->pDerivedObj);
                Shape *proj_hitbox = proj->hitbox;


                // Skip enemy projectiles
                if (proj->is_enemy_projectile) {
                    node = next_node;
                    continue;
                }


                if (enemy2->hitbox->overlap(enemy2->hitbox, proj_hitbox)) {
                    obj->dele = true;








                    // Take damage and flash red
                    enemy2->hp--;
                    enemy2->got_hit = true;
                    enemy2->hit_time = al_get_time();


                    if (enemy2->hp <= 0 && !enemy2->dying) {
                        enemy2->alive = false;
                        enemy2->dying = true;
                        enemy2->state = ENEMY2_DEAD;
                        enemy2->gif_status[ENEMY2_DEAD]->display_index = 0;
                        enemy2->gif_status[ENEMY2_DEAD]->done = false;
                        enemy2->death_time = al_get_time();
                        return;
                    }
                    return;
                }
            }
            node = node->next;
        }
    }
}









