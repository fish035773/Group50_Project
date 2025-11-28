#include "projectile.h"
#include "tree.h"
#include "enemy.h"
#include "enemy2.h"
#include "enemy3.h"
#include "../shapes/Circle.h"
#include "../scene/gamescene.h" // for element label
#include "../scene/sceneManager.h" // for scene variable
#include "Character2.h"
#include "charater.h"
#define damage_V 10
#define damage_L 10
#define damage_K 5 //have 3 bullet damage = 1 * 3 = 3
#define damage_C 5 //have 3 bullet damage = 1 * 3 = 3
#define damage_J 5
#define damage_X 5
/*
   [Projectile function]
*/
Elements *New_Projectile(int label, int x, int y, int v, void* owner)
{
    Projectile *pDerivedObj = (Projectile *)malloc(sizeof(Projectile));
    Elements *pObj = New_Elements(label);
    // setting derived object member
    if (label == Projectile_K) {
        pDerivedObj->img = al_load_bitmap("assets/image/projectile_k.png");
        pDerivedObj->damage = damage_K;
        pDerivedObj->origin = 2;
        pDerivedObj->owner = owner;
    } else if (label == Projectile_L) {
        pDerivedObj->img = al_load_bitmap("assets/image/projectile_l.png");
        pDerivedObj->damage = damage_L;
        pDerivedObj->origin = 2;
        pDerivedObj->owner = owner;
    } else if (label == Projectile_J) {
        pDerivedObj->img = al_load_bitmap("assets/image/projectile_j.png");
        pDerivedObj->damage = damage_J;
        pDerivedObj->origin = 2;
        pDerivedObj->owner = owner;
    } else if (label == Projectile_V){
        pDerivedObj->img = al_load_bitmap("assets/image/projectile_v.png");
        pDerivedObj->damage = damage_V;
        pDerivedObj->owner = owner;
        pDerivedObj->origin = 1;
    } else if (label == Projectile_C){
        pDerivedObj->img = al_load_bitmap("assets/image/projectile_c.png");
        pDerivedObj->damage = damage_C;
        pDerivedObj->origin = 1;
        pDerivedObj->owner = owner;
    }
    else if(label == Projectile_X){
        pDerivedObj->img = al_load_bitmap("assets/image/projectile_x.png");
        pDerivedObj->damage = damage_X;
        pDerivedObj->origin = 1;
        pDerivedObj->owner = owner;
    }
    pDerivedObj->width = al_get_bitmap_width(pDerivedObj->img);
    pDerivedObj->height = al_get_bitmap_height(pDerivedObj->img);
    pDerivedObj->x = x;
    pDerivedObj->y = y;
    pDerivedObj->v = v;
    pDerivedObj->hitbox = New_Circle(pDerivedObj->x + pDerivedObj->width / 2,
                                     pDerivedObj->y + pDerivedObj->height / 2,
                                     min(pDerivedObj->width, pDerivedObj->height) / 2);
    // setting the interact object
    pObj->inter_obj[pObj->inter_len++] = Tree_L;
    pObj->inter_obj[pObj->inter_len++] = Floor_L;
    pObj->inter_obj[pObj->inter_len++] = Enemy_L;
    pObj->inter_obj[pObj->inter_len++] = Enemy2_L;
    pObj->inter_obj[pObj->inter_len++] = Enemy3_L;
   // pObj->inter_obj[pObj->inter_len++] = Enemy3_L;
    pObj->inter_obj[pObj->inter_len++] = Character_L;
    pObj->inter_obj[pObj->inter_len++] = Character2_L;
    
    // setting derived object function
    pObj->pDerivedObj = pDerivedObj;
    pObj->Update = Projectile_update;
    pObj->Interact = Projectile_interact;
    pObj->Draw = Projectile_draw;
    pObj->Destroy = Projectile_destory;

    return pObj;
}
void Projectile_update(Elements *self)
{
    Projectile *Obj = ((Projectile *)(self->pDerivedObj));
    _Projectile_update_position(self, Obj->v, 0);
}
void _Projectile_update_position(Elements *self, int dx, int dy)
{
    Projectile *Obj = ((Projectile *)(self->pDerivedObj));
    Obj->x += dx;
    Obj->y += dy;

    Shape *hitbox = Obj->hitbox;
    hitbox->update_center_x(hitbox, dx);
    hitbox->update_center_y(hitbox, dy);
  
}
void Projectile_interact(Elements *self)
{
    for (int j = 0; j < self->inter_len; j++)
    {
        int inter_label = self->inter_obj[j];
        ElementVec labelEle = _Get_label_elements(scene, inter_label);
        for (int i = 0; i < labelEle.len; i++)
        {    // printf("DEBUG: inter_label = %d\n", inter_label); // 加這行！

            if (inter_label == Enemy_L)
            {
                _Projectile_interact_Enemy(self, labelEle.arr[i]);
               // printf(" _Projectile_interact_Enemy(self, labelEle.arr[i]);");
            }
            else if (inter_label == Enemy2_L)
            {
                _Projectile_interact_Enemy2(self, labelEle.arr[i]);
            }
            else if (inter_label == Enemy3_L)
            {
                _Projectile_interact_Enemy3(self, labelEle.arr[i]);
            }
            else if(inter_label == Character_L){
               // _Projectile_interact_character(self, labelEle.arr[i]);
            } 
            else if(inter_label == Character2_L){
               // _Projectile_interact_character(self, labelEle.arr[i]);
            }
        }
    }
}

void _Projectile_interact_Enemy(Elements *self, Elements *tar)
{
    Projectile *Obj = ((Projectile *)(self->pDerivedObj));
    if(Obj->origin == 2){//when Projectile from Character2 hit enemy
        Enemy *enemy = ((Enemy *)(tar->pDerivedObj));
        Character2 *chara2 = ((Character2 *)(Obj->owner));
        if (enemy->hitbox->overlap(enemy->hitbox, Obj->hitbox) && chara2->blood > 0 && enemy->hp > 0)
        {
            self->dele = true;
            enemy->hp -= Obj->damage;
            if(enemy->hp <= 0){
                tar->dele = true;
              
            }
            printf("hit enemy->hp : %d\n", enemy->hp);
            chara2->levelup_points++;
        }
    }
    else if(Obj->origin == 1){//when Projectile from Character1 hit enemy
        Enemy *enemy = ((Enemy *)(tar->pDerivedObj));
        Character *chara = ((Character *)(Obj->owner));
        if (enemy->hitbox->overlap(enemy->hitbox, Obj->hitbox) && chara->blood > 0 && enemy->hp > 0)
        {
            self->dele = true;
            enemy->hp -= Obj->damage;
            if(enemy->hp <= 0){
                tar->dele = true;
                
            }
            printf("hit enemy->hp : %d\n", enemy->hp);
            chara->levelup_points++;
        }
    }
}

void _Projectile_interact_Enemy2(Elements *self, Elements *tar)
{
    Projectile *Obj = ((Projectile *)(self->pDerivedObj));
    if(Obj->origin == 2){//when Projectile from Character2 hit enemy
        Enemy2 *enemy = ((Enemy2 *)(tar->pDerivedObj));
        Character2 *chara2 = ((Character2 *)(Obj->owner));
        if (enemy->hitbox->overlap(enemy->hitbox, Obj->hitbox) && chara2->blood > 0 && enemy->hp > 0)
        {
            self->dele = true;
            enemy->hp -= Obj->damage;
            if(enemy->hp <= 0){
                tar->dele = true;
            
            }

            printf("hit enemy->hp : %d\n", enemy->hp);
            chara2->levelup_points++;
        }
    }
    else if(Obj->origin == 1){//when Projectile from Character1 hit enemy
        Enemy2 *enemy = ((Enemy2 *)(tar->pDerivedObj));
        Character *chara = ((Character *)(Obj->owner));
        if (enemy->hitbox->overlap(enemy->hitbox, Obj->hitbox) && chara->blood > 0 && enemy->hp > 0)
        {
            self->dele = true;
            enemy->hp -= Obj->damage;
            if(enemy->hp <= 0){
                tar->dele = true;
              
            }
            printf("hit enemy2->hp : %d\n", enemy->hp);
            chara->levelup_points++;
        }
    }
}

void _Projectile_interact_Enemy3(Elements *self, Elements *tar)
{
    Projectile *Obj = ((Projectile *)(self->pDerivedObj));
    if(Obj->origin == 2){//when Projectile from Character2 hit enemy
        Enemy3 *enemy = ((Enemy3 *)(tar->pDerivedObj));
        Character2 *chara2 = ((Character2 *)(Obj->owner));
        if (enemy->hitbox->overlap(enemy->hitbox, Obj->hitbox) && chara2->blood > 0 && enemy->hp > 0)
        {
            self->dele = true;
            enemy->hp -= Obj->damage;
            if(enemy->hp <= 0){
                tar->dele = true;
              
            }
            printf("hit enemy3->hp : %d\n", enemy->hp);
            chara2->levelup_points++;
        }
    }
    else if(Obj->origin == 1){//when Projectile from Character1 hit enemy
        Enemy3 *enemy = ((Enemy3 *)(tar->pDerivedObj));
        Character *chara = ((Character *)(Obj->owner));
        if (enemy->hitbox->overlap(enemy->hitbox, Obj->hitbox) && chara->blood > 0 && enemy->hp > 0)
        {
            self->dele = true;
            enemy->hp -= Obj->damage;
            if(enemy->hp <= 0){
                tar->dele = true;
                
            }
            printf("hit enemy3->hp : %d\n", enemy->hp);
            chara->levelup_points++;
        }
    }
}
void _Projectile_interact_Tree(Elements *self, Elements *tar)
{
    Projectile *Obj = ((Projectile *)(self->pDerivedObj));
    Tree *tree = ((Tree *)(tar->pDerivedObj));
    if (tree->hitbox->overlap(tree->hitbox, Obj->hitbox))
    {
        self->dele = true;
    }
}
void _Projectile_interact_character(Elements *self, Elements *tar)
{
    Projectile *Obj = ((Projectile *)(self->pDerivedObj));
    if(Obj->origin == 2){//when Projectile from Character2 hit Character1
        Character *chara = ((Character *)(tar->pDerivedObj));
        Character2 *chara2 = ((Character2 *)(Obj->owner));
        if (chara->hitbox->overlap(chara->hitbox, Obj->hitbox) && chara2->blood > 0 && chara->blood > 0)
        {
            self->dele = true;
            chara->blood -= Obj->damage;
            
            chara2->levelup_points++;
        }
    }
    else if(Obj->origin == 1){//when Projectile from Character1 hit Character2
        Character2 *chara2 = ((Character2 *)(tar->pDerivedObj) );
        Character *chara = ((Character *)(Obj->owner));
        if (chara2->hitbox->overlap(chara2->hitbox, Obj->hitbox) && chara->blood > 0 && chara2->blood > 0)
        {
            self->dele = true;
            chara2->blood -= Obj->damage;
            chara->levelup_points++;
        }
    }
}
void Projectile_draw(Elements *self)
{
    Projectile *Obj = ((Projectile *)(self->pDerivedObj));
    if (Obj->v > 0)
        al_draw_bitmap(Obj->img, Obj->x, Obj->y, ALLEGRO_FLIP_HORIZONTAL);
    else
        al_draw_bitmap(Obj->img, Obj->x, Obj->y, 0);
}
void Projectile_destory(Elements *self)
{
    Projectile *Obj = ((Projectile *)(self->pDerivedObj));
    al_destroy_bitmap(Obj->img);
    free(Obj->hitbox);
    free(Obj);
    free(self);
}