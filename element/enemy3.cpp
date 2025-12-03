#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include "enemy3.h"
#include "projectile3.h"
#include "../scene/sceneManager.h"
#include "../shapes/Rectangle.h"
#include "../algif5/algif.h"
#include "../scene/gamescene.h"
#include "../element/charater.h"
#include "../element/projectile3.h"
#include <stdio.h>
#include <stdbool.h>
#define maxhp 100



Elements *New_Enemy3(int label) {
    Enemy3 *pDerivedObj = (Enemy3 *)malloc(sizeof(Enemy3));
    Elements *pObj = New_Elements(label);
    



    // Load animations
    char state_string[5][10] = {"stop", "move", "attack","throw", "dead"};
    for (int i = 0; i < 5; i++) {
        char buffer[100];
        sprintf(buffer, "assets/image/enemy3_%s.gif", state_string[i]);
        printf("Loading GIF: %s\n", buffer);
        pDerivedObj->gif_status[i] = algif_load_animation(buffer);
        if (!pDerivedObj->gif_status[i]) {
            printf("Failed to load GIF: %s\n", buffer);
        }
    }




    // Load sound
    ALLEGRO_SAMPLE *atk_sample = al_load_sample("assets/sound/hit_sound.mp3");
    pDerivedObj->atk_sound = al_create_sample_instance(atk_sample);
    al_set_sample_instance_playmode(pDerivedObj->atk_sound, ALLEGRO_PLAYMODE_ONCE);
    al_attach_sample_instance_to_mixer(pDerivedObj->atk_sound, al_get_default_mixer());


     ALLEGRO_SAMPLE *throw_sample = al_load_sample("assets/sound/throw_sound.mp3");
    pDerivedObj->throw_sound = al_create_sample_instance(throw_sample);
    al_set_sample_instance_playmode(pDerivedObj->throw_sound, ALLEGRO_PLAYMODE_ONCE);
    al_attach_sample_instance_to_mixer(pDerivedObj->throw_sound, al_get_default_mixer());


    // Setup position & size
    pDerivedObj->width = pDerivedObj->gif_status[0]->width;
    pDerivedObj->height = pDerivedObj->gif_status[0]->height;
    pDerivedObj->x = 500;
    pDerivedObj->y = HEIGHT - pDerivedObj->height + 100;




    // Hitbox
    pDerivedObj->hitbox = New_Rectangle(pDerivedObj->x, pDerivedObj->y,
                                        pDerivedObj->x + pDerivedObj->width,
                                        pDerivedObj->y + pDerivedObj->height);




    // Init state
    pDerivedObj->dir = true;
    pDerivedObj->state = ENEMY3_IDLE;
    pDerivedObj->active_proj = false;
    pDerivedObj->move_counter = 0;




    pObj->pDerivedObj = pDerivedObj;
    pObj->Draw = Enemy3_draw;
    pObj->Update = Enemy3_update;
    pObj->Interact = Enemy3_interact;
    pObj->Destroy = Enemy3_destroy;




    // Status
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



int player_center_x3;
void enemy_charater3(int dx){
    
    player_center_x3 =  dx;
}
void Enemy3_update(Elements *self) {
    Enemy3 *enemy3 = (Enemy3 *)(self->pDerivedObj);
   
    int speed = 5;
    


    int enemy3_center_x = enemy3->x + enemy3->width / 2;
   
    int dx = player_center_x3 - enemy3_center_x;

     
    double current_time = al_get_time();
    if (!enemy3->can_attack && current_time - enemy3->spawn_time >= 1.0)
        enemy3->can_attack = true;


    if (enemy3->can_attack) {
        enemy3->dir = dx >= 0;
        if (abs(dx) <= ENEMY3_MELEE_RANGE) {
            if (enemy3->state != ENEMY3_ATK) {
                enemy3->state = ENEMY3_ATK;
                enemy3->active_proj = false;
                enemy3->move_counter = 0;
            }
        } else if (abs(dx) <= ENEMY3_ATTACK_RANGE) {
            if (enemy3->state != ENEMY3_THROW) {
                enemy3->state = ENEMY3_THROW;
                enemy3->active_proj = false;
                enemy3->move_counter = 0;
            }
        }
    }

    
    if (enemy3->state == ENEMY3_DEAD) {
        if (enemy3->death_time == 0)
            enemy3->death_time = al_get_time();
        else if (enemy3->gif_status[ENEMY3_DEAD]->done &&
                 al_get_time() - enemy3->death_time >= 1.0)
            self->dele = true;
        return;


    } else if (enemy3->state == ENEMY3_IDLE) {
        if (abs(dx) <= ENEMY3_ATTACK_RANGE) {
            enemy3->chasing = true;
            enemy3->state = ENEMY3_ATK;
            enemy3->active_proj = false;
            enemy3->move_counter = 0;
        } else {
            enemy3->state = ENEMY3_MOVE;
        }


    } else if (enemy3->state == ENEMY3_MOVE) {
    if (!enemy3->chasing) {
        if (abs(dx) <= ENEMY3_ATTACK_RANGE) {
            enemy3->chasing = true; // Start chasing
        } else {
            // Patrol behavior
            if (enemy3->dir == false) {
                _Enemy3_update_position(self, -speed, 0);
                if (enemy3->x < 500) {
                    enemy3->dir = true;
                }
            } else {
                _Enemy3_update_position(self, speed, 0);
                if (enemy3->x > 700) {
                    enemy3->dir = false;
                }
            }
            enemy3->state = ENEMY3_MOVE;
            return;
        }
    }
    

    // If chasing is true, move toward player
    enemy3->dir = dx >= 0;
    int move_speed = (dx > 0) ? speed : -speed;
    _Enemy3_update_position(self, move_speed, 0);
    enemy3->state = ENEMY3_MOVE;
}






     else if (enemy3->state == ENEMY3_ATK) {
        int current_frame = enemy3->gif_status[ENEMY3_ATK]->display_index;


        if (current_frame == 2 && !enemy3->active_proj) {
            int proj_x = enemy3->dir ? (enemy3->x + enemy3->width - 100) : (enemy3->x + 100 - enemy3->width);
            Elements *pro = New_Projectile3(Projectile3_L,
                                            proj_x,
                                            enemy3->y + enemy3->height / 2 + 20,
                                            enemy3->dir ? 5 : -5);
            ((Projectile3 *)pro->pDerivedObj)->is_enemy_projectile = true;
            _Register_elements(scene, pro);
            enemy3->active_proj = true;
            al_play_sample_instance(enemy3->atk_sound); // Sound only once
        }


        if (enemy3->gif_status[ENEMY3_ATK]->done) {
            enemy3->gif_status[ENEMY3_ATK]->done = false;
            enemy3->gif_status[ENEMY3_ATK]->display_index = 0;
            enemy3->active_proj = false;


            if (abs(dx) <= ENEMY3_ATTACK_RANGE) {
                enemy3->state = ENEMY3_ATK;
                enemy3->chasing = true;
            } else {
                enemy3->state = ENEMY3_MOVE;
                enemy3->chasing = true;
            }
        }


    } else if (enemy3->state == ENEMY3_THROW) {
        int current_frame = enemy3->gif_status[ENEMY3_THROW]->display_index;


        if (current_frame == 2 && !enemy3->active_proj) {
            int proj_x = enemy3->dir ? (enemy3->x + enemy3->width - 100) : (enemy3->x + 20);
            Elements *pro = New_Projectile3(Projectile3_L,
                                            proj_x,
                                            enemy3->y + enemy3->height / 2 + 20,
                                            enemy3->dir ? 5 : -5);
            ((Projectile3 *)pro->pDerivedObj)->is_enemy_projectile = true;
            _Register_elements(scene, pro);
            enemy3->active_proj = true;
            al_play_sample_instance(enemy3->throw_sound);
        }


        if (enemy3->gif_status[ENEMY3_THROW]->done) {
            enemy3->gif_status[ENEMY3_THROW]->done = false;
            enemy3->gif_status[ENEMY3_THROW]->display_index = 0;
            enemy3->active_proj = false;


            if (abs(dx) <= ENEMY3_MELEE_RANGE) {
                enemy3->state = ENEMY3_ATK;
            } else {
                enemy3->state = ENEMY3_MOVE;
            }
        }
    }
}




void Enemy3_draw(Elements *self) {
    Enemy3 *enemy3 = (Enemy3 *)(self->pDerivedObj);
    ALLEGRO_BITMAP *frame = algif_get_bitmap(enemy3->gif_status[enemy3->state], al_get_time());
    
    if(enemy3->hp > 0){
    if (frame) {
        int flags = enemy3->dir ? 0 : ALLEGRO_FLIP_HORIZONTAL;
        int draw_y = enemy3->y + 50; // Adjust this offset as needed


        if (enemy3->got_hit && al_get_time() - enemy3->hit_time < 0.2) {
            al_draw_tinted_bitmap(frame, al_map_rgba(255, 0, 0, 200),
                                 enemy3->x, draw_y - 10, flags); // Apply SAME flip to hit effect
        } else {
            al_draw_bitmap(frame, enemy3->x, draw_y, flags); // Normal draw
        }
    }

    
    // HP bar (adjust Y to match)
    int bar_width = 100;
    int bar_height = 10;
    int bar_x = enemy3->x + 80;
    int bar_y = enemy3->y + 80; // Match draw_y
    float hp_ratio = (float)enemy3->hp / maxhp;
    al_draw_filled_rectangle(bar_x, bar_y, bar_x + (int)(bar_width * 1),
                           bar_y + bar_height, al_map_rgb(255, 255, 255));
    al_draw_filled_rectangle(bar_x, bar_y, bar_x + (int)(bar_width * hp_ratio),
                           bar_y + bar_height, al_map_rgb(150, 0, 0));

}
}




void Enemy3_destroy(Elements *self) {
    Enemy3 *enemy3 = (Enemy3 *)(self->pDerivedObj);
    if (enemy3->atk_sound) al_destroy_sample_instance(enemy3->atk_sound);
    if(enemy3->throw_sound) al_destroy_sample_instance(enemy3->throw_sound);
    for (int i = 0; i < 5; i++)
        if (enemy3->gif_status[i]) algif_destroy_animation(enemy3->gif_status[i]);
    if (enemy3->hitbox) free(enemy3->hitbox);
    free(enemy3);
    free(self);
}




void _Enemy3_update_position(Elements *self, int dx, int dy) {
    Enemy3 *enemy3 = (Enemy3 *)(self->pDerivedObj);
    enemy3->x += dx;
    enemy3->y += dy;
    if (enemy3->hitbox) {
        enemy3->hitbox->update_center_x(enemy3->hitbox, dx);
        enemy3->hitbox->update_center_y(enemy3->hitbox, dy);
    }
}




void Enemy3_interact(Elements *self) {
    Enemy3 *enemy3 = (Enemy3 *)(self->pDerivedObj);
    if (!enemy3->alive || enemy3->dying) return;




    extern Scene *scene;
    for (int i = 0; i < MAX_ELEMENT; i++) {
        EPNode *node = scene->ele_list[i];
        while (node != NULL) {
            Elements *obj = node->ele;
            EPNode *next_node = node->next;




            if ((obj->label == Projectile3_Right || obj->label == Projectile3_Left || obj->label == Projectile3_L) &&
                !((Projectile3 *)obj->pDerivedObj)->is_enemy_projectile &&
                enemy3->hitbox->overlap(enemy3->hitbox, ((Projectile3 *)obj->pDerivedObj)->hitbox)) {
               
                obj->dele = true;
                enemy3->got_hit = true;
                enemy3->hit_time = al_get_time();
                enemy3->hp--;




                if (enemy3->hp <= 0 && !enemy3->dying) {
                    enemy3->alive = false;
                    enemy3->dying = true;
                    enemy3->state = ENEMY3_DEAD;
                    enemy3->gif_status[ENEMY3_DEAD]->display_index = 0;
                    enemy3->gif_status[ENEMY3_DEAD]->done = false;
                    enemy3->death_time = al_get_time();
                }
            }
            node = next_node;
        }
    }
}















