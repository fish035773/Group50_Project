#include "projectile2.h"
#include "charater.h"
#include "Character2.h"
#include "enemy.h"
#include "../shapes/Circle.h"
#include "../scene/gamescene.h" // for element label
#include "../scene/sceneManager.h" // for scene variable


/*
   [Projectile function]
*/
Elements *New_Projectile2(int label, int x, int y, int v)
{
    Projectile2 *pDerivedObj = (Projectile2 *)malloc(sizeof(Projectile2));
    Elements *pObj = New_Elements(label);
    // setting derived object member
    pDerivedObj->img = al_load_bitmap("assets/image/projectile2.png");
    pDerivedObj->width = al_get_bitmap_width(pDerivedObj->img);
    pDerivedObj->height = al_get_bitmap_height(pDerivedObj->img);
    pDerivedObj->x = x;
    pDerivedObj->y = y;
    pDerivedObj->v = v;
    pDerivedObj->damage = 1;
    pDerivedObj->hitbox = New_Circle(pDerivedObj->x + pDerivedObj->width / 2,
                                     pDerivedObj->y + pDerivedObj->height / 2,
                                     min(pDerivedObj->width, pDerivedObj->height) / 2);
    // setting the interact object
    pObj->inter_obj[pObj->inter_len++] = Floor_L;
    pObj->inter_obj[pObj->inter_len++] = Character_L;
    pObj->inter_obj[pObj->inter_len++] = Character2_L;


    pDerivedObj->is_enemy_projectile = true;


    // setting derived object function
    pObj->pDerivedObj = pDerivedObj;
    pObj->Update = Projectile2_update;
    pObj->Interact = Projectile2_interact;
    pObj->Draw = Projectile2_draw;
    pObj->Destroy = Projectile2_destroy;
   
    return pObj;
}


void Projectile2_update(Elements *self)
{
    Projectile2 *Obj = ((Projectile2 *)(self->pDerivedObj));
    _Projectile2_update_position(self, Obj->v, 0);
}


void _Projectile2_update_position(Elements *self, int dx, int dy)
{
    Projectile2 *Obj = ((Projectile2 *)(self->pDerivedObj));
    Obj->x += dx;
    Obj->y += dy;
    Shape *hitbox = Obj->hitbox;
    hitbox->update_center_x(hitbox, dx);
    hitbox->update_center_y(hitbox, dy);
}


void Projectile2_interact(Elements *self)
{
    for (int j = 0; j < self->inter_len; j++)
    {
        int inter_label = self->inter_obj[j];
        ElementVec labelEle = _Get_label_elements(scene, inter_label);
        for (int i = 0; i < labelEle.len; i++)
        {    // printf("DEBUG: inter_label = %d\n", inter_label); // 加這行！


            if(inter_label == Character_L){
               _Projectile2_interact_Character(self, labelEle.arr[i]);
            }
            else if(inter_label == Character2_L){
               _Projectile2_interact_Character(self, labelEle.arr[i]);
            }
        }
    }
}


void _Projectile2_interact_Floor(Elements *self, Elements *tar)
{
    Projectile2 *Obj = ((Projectile2 *)(self->pDerivedObj));
    if (Obj->x < 0 - Obj->width)
        self->dele = true;
    else if (Obj->x > WIDTH + Obj->width)
        self->dele = true;
}


void _Projectile2_interact_Character(Elements *self, Elements *tar)
{
    Projectile2 *Obj = ((Projectile2 *)(self->pDerivedObj));
    if (tar->label == Character_L) {
        Character *player = (Character *)(tar->pDerivedObj);
        if (player->hitbox->overlap(player->hitbox, Obj->hitbox) && player->blood > 0) {
            self->dele = true;
            player->blood -= Obj->damage;
        }
    }
    else if (tar->label == Character2_L) {
        Character2 *player2 = (Character2 *)(tar->pDerivedObj);
        if (player2->hitbox->overlap(player2->hitbox, Obj->hitbox) && player2->blood > 0) {
            self->dele = true;
            player2->blood -= Obj->damage;
        }
    }  
}


void Projectile2_draw(Elements *self)
{
    Projectile2 *Obj = ((Projectile2 *)(self->pDerivedObj));
    if (Obj->v < 0)
        al_draw_bitmap(Obj->img, Obj->x, Obj->y, ALLEGRO_FLIP_HORIZONTAL);
    else
        al_draw_bitmap(Obj->img, Obj->x, Obj->y, 0);
}


void Projectile2_destroy(Elements *self)
{
    Projectile2 *Obj = ((Projectile2 *)(self->pDerivedObj));
    al_destroy_bitmap(Obj->img);
    free(Obj->hitbox);
    free(Obj);
    free(self);
}
