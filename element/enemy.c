#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include "enemy.h"
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
Elements *New_Enemy(int label)
{
    Enemy *pDerivedObj = (Enemy *)malloc(sizeof(Enemy));
    Elements *pObj = New_Elements(label);
    // setting derived object member
    // load character images
    char state_string[4][10] = {"stop", "move", "attack", "dead"};
    for (int i = 0; i < 4; i++)
    {
        char buffer[100];
        sprintf(buffer, "assets/image/enemy_%s.gif", state_string[i]);
        printf("Loading GIF: %s\n", buffer);
        pDerivedObj->gif_status[i] = algif_new_gif(buffer, -1);
    }
    // load effective sound
    ALLEGRO_SAMPLE *sample = al_load_sample("assets/sound/fire_atk_sound.mp3");
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
    pDerivedObj->state = ENEMY_IDLE;
    pDerivedObj->active_proj = false;
    pDerivedObj->move_counter = 0;




    pObj->pDerivedObj = pDerivedObj;
    // setting derived object function
    pObj->Draw = Enemy_draw;
    pObj->Update = Enemy_update;
    pObj->Interact = Enemy_interact;
    pObj->Destroy = Enemy_destroy;




    pDerivedObj->hp = maxhp;              
    pDerivedObj->alive = true;
    pDerivedObj->got_hit = false;
    pDerivedObj->hit_time = 0;
    pDerivedObj->dying = false;
    pDerivedObj->death_time = 0;




    pDerivedObj->spawn_time = al_get_time();
    pDerivedObj->can_attack = false;
    pDerivedObj->chasing = false;
    return pObj;
}
int player_center_x;
void enemy_charater(int dx){
    
    player_center_x =  dx;
}

void Enemy_update(Elements *self)
{
    // use the idea of finite state machine to deal with different state
    Enemy *enemy = ((Enemy *)(self->pDerivedObj));
    extern Elements *player;
  
    if(enemy->hp > 0){
    int speed = 2;
 

   
    int enemy_center_x = enemy->x + enemy->width / 2;
   
   
    int dx = player_center_x - enemy_center_x;


   // Enemy_draw(self);

    double current_time = al_get_time();
   // printf("Enemy_update_current_time\n");
    if (!enemy->can_attack && current_time - enemy->spawn_time >= 1.0) {
        enemy->can_attack = true; // allow attacking after 1 second
    }




    // **Always check attack range first regardless of state**
    if (enemy->can_attack && abs(dx) <= ENEMY_ATTACK_RANGE)
    {
        enemy->dir = dx >= 0;




        // If not already attacking, switch to attack
        if (enemy->state != ENEMY_ATK)
        {   
           // printf("Enemy_update_ENEMY_ATK\n");
            enemy->state = ENEMY_ATK;
            enemy->active_proj = false;
            enemy->move_counter = 0;
        }
    }


   
    if (enemy->state == ENEMY_DEAD) {
        //printf("enemy->state == ENEMY_DEAD\n");
        if (enemy->death_time == 0)
            enemy->death_time = al_get_time(); // mark when death started
           
        else if (enemy->gif_status[ENEMY_DEAD]->done && al_get_time() - enemy->death_time >= 1.0) // wait 1 second
            self->dele = true;
        return;
    }
    else if (enemy->state == ENEMY_IDLE)
    {
        if (abs(dx) <= ENEMY_ATTACK_RANGE)
        {
            enemy->chasing = true;
            enemy->state = ENEMY_ATK;
            enemy->active_proj = false;
            enemy->move_counter = 0;
        }
        else
        {
            enemy->state = ENEMY_MOVE;
        }
    }
    else if (enemy->state == ENEMY_MOVE)
    {
        if (enemy->chasing) {
        // Chase player regardless of patrol area
        enemy->dir = (dx >= 0);
        int move_speed = (dx > 0) ? speed : -speed;
        _Enemy_update_position(self, move_speed, 0);
    } else {
        // Normal patrol between 400 and 700
        if (enemy->dir == false) {
            _Enemy_update_position(self, -speed, 0);
            if (enemy->x < 400)
                enemy->dir = true;
        } else {
            _Enemy_update_position(self, speed, 0);
            if (enemy->x > 700)
                enemy->dir = false;
        }
    }  
    }
    else if (enemy->state == ENEMY_ATK)
    {
        int current_frame = enemy->gif_status[ENEMY_ATK]->display_index;

        //printf("enemy->state == ENEMY_ATK");


        // Launch projectile once at frame 2
        if (current_frame == 2 && !enemy->active_proj)
        {
            Elements *pro;
            int projectile_x;
            if (enemy->dir) {
            projectile_x = enemy->x +enemy->width - 100;  // Or enemy_center_x - offset
            } else {
                projectile_x = enemy->x + 20;  // symmetric to right
            }




            pro = New_Projectile2(Projectile2_L,
                    projectile_x,
                    enemy->y + enemy->height / 2 - 20,
                    enemy->dir ? 5 : -5);


       
            ((Projectile2 *)pro->pDerivedObj)->is_enemy_projectile = true;
            _Register_elements(scene, pro);
            enemy->active_proj = true;
        }




        // When attack animation finishes, transition based on distance
        if (enemy->gif_status[ENEMY_ATK]->done)
        {
            enemy->gif_status[ENEMY_ATK]->done = false;  // reset animation
            enemy->gif_status[ENEMY_ATK]->display_index = 0; // restart from first frame
            enemy->active_proj = false;




            // If player still in attack range, repeat attack
            if (abs(dx) <= ENEMY_ATTACK_RANGE) {
                enemy->chasing = true;
                enemy->state = ENEMY_ATK; // stay in attack
            }
            else {
                enemy->chasing = true; // still chasing
                enemy->state = ENEMY_MOVE; // go chase the player
            }
        }
    }
   // printf("Enemy_update_out\n");
    }
}
void Enemy_draw(Elements *self)
{
    // with the state, draw corresponding image
    Enemy *enemy = ((Enemy *)(self->pDerivedObj));
    if(enemy->hp > 0){
    ALLEGRO_BITMAP *frame = algif_get_bitmap(enemy->gif_status[enemy->state], al_get_time());
    if (frame)
    {
        al_draw_bitmap(frame, enemy->x, enemy->y, (enemy->dir ? 0 : ALLEGRO_FLIP_HORIZONTAL));
    }
    if (enemy->state == ENEMY_ATK && enemy->gif_status[enemy->state]->display_index == 2)
    {
        al_play_sample_instance(enemy->atk_sound);
    }
    



    // Draw the red HP bar
    int bar_width = 100;
    int bar_height = 10;
    int bar_x = enemy->x + 80;
    int bar_y = enemy->y; // position above the enemy




    float hp_ratio = (float)enemy->hp / maxhp; // max HP = 3, change if needed
    al_draw_filled_rectangle(bar_x, bar_y, bar_x + (int)(bar_width * 1), bar_y + bar_height, al_map_rgb(225, 225, 225));
    al_draw_filled_rectangle(bar_x, bar_y, bar_x + (int)(bar_width * hp_ratio), bar_y + bar_height, al_map_rgb(150, 0, 0));
   
    //printf("\nhp_ratio : %f\n", hp_ratio);




    if (enemy->state == ENEMY_ATK && enemy->gif_status[enemy->state]->display_index == 2)
    {
        al_play_sample_instance(enemy->atk_sound);
    }


    if (enemy->got_hit && al_get_time() - enemy->hit_time < 0.2) {
        al_draw_tinted_bitmap(frame, al_map_rgba(255, 0, 0, 200), enemy->x, enemy->y, (enemy->dir ? 0 : ALLEGRO_FLIP_HORIZONTAL));
    } else {
        al_draw_bitmap(frame, enemy->x, enemy->y, (enemy->dir ? 0 : ALLEGRO_FLIP_HORIZONTAL));
    }

}
}
void Enemy_destroy(Elements *self)
{
    Enemy *enemy = ((Enemy *)(self->pDerivedObj));
    al_destroy_sample_instance(enemy->atk_sound);
    for (int i = 0; i < 4; i++)
        algif_destroy_animation(enemy->gif_status[i]);
    free(enemy->hitbox);
    free(enemy);
    free(self);
}




void _Enemy_update_position(Elements *self, int dx, int dy)
{
    Enemy *enemy = ((Enemy *)(self->pDerivedObj));
    enemy->x += dx;
    enemy->y += dy;
    Shape *hitbox = enemy->hitbox;
    hitbox->update_center_x(hitbox, dx);
    hitbox->update_center_y(hitbox, dy);
}




void Enemy_interact(Elements *self) {
    Enemy *enemy = (Enemy *)(self->pDerivedObj);
    if (!enemy->alive || enemy->dying) return;


    extern Scene *scene;
    for (int i = 0; i < MAX_ELEMENT; i++) {
        EPNode *node = scene->ele_list[i];
        while (node != NULL) {
            Elements *obj = node->ele;
            EPNode *next_node = node->next;


            if ((obj->label == Projectile2_Right || obj->label == Projectile2_Left || obj->label == Projectile2_L) &&
                !((Projectile2 *)obj->pDerivedObj)->is_enemy_projectile &&  // ← Key change!
                enemy->hitbox->overlap(enemy->hitbox, ((Projectile2 *)obj->pDerivedObj)->hitbox)) {


                obj->dele = true; // Delete projectile


                // Visual feedback (flash + damage)
                enemy->got_hit = true;
                enemy->hit_time = al_get_time();
                enemy->hp--;


                // Death handling
                if (enemy->hp <= 0 && !enemy->dying) {
                    enemy->alive = false;
                    enemy->dying = true;
                    enemy->state = ENEMY_DEAD;
                    enemy->gif_status[ENEMY_DEAD]->display_index = 0;
                    enemy->gif_status[ENEMY_DEAD]->done = false;
                    enemy->death_time = al_get_time();
                }
                return;
            }
            node = next_node;
        }
    }
}

