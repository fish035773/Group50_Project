#include <allegro5/allegro_audio.h>
#include "gamescene.h"
#include "../global.h"
#include "../element/element.h"
#include "../element/charater.h"
#include "../element/floor.h"
#include "../element/teleport.h"
#include "../element/tree.h"
#include "../element/projectile.h"
#include "../element/Ball.h"
#include "../element/Character2.h"
#include "../element/elements_factory.h"
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "../element/projectile2.h"
#include "sceneManager.h"
#include "../element/enemy.h"
#include "../element/enemy2.h"
#include "../element/enemy3.h"
#define FPS 60
#define barlong 510
Scene *current_game_scene = NULL; //pointer to reference active scene
int i = 0;
/*
   [GameScene function]
*/
Scene *New_GameScene(int label)
{
    GameScene *pDerivedObj = (GameScene *)malloc(sizeof(GameScene));
    memset(pDerivedObj, 0, sizeof(GameScene));
    Scene *pObj = New_Scene(label);




    current_game_scene = pObj;
    // RESET round state
    pDerivedObj->round_counter = 1;
    pDerivedObj->round_triggered = true;
    pDerivedObj->round_timer = 0;
    pDerivedObj->enemy_defeated = false;
    pDerivedObj->round_locked = false;
   
    pDerivedObj->enemies_spawned = false;








    printf("[GameScene] Round state reset.\n");




    // Load background
    pDerivedObj->background = al_load_bitmap("assets/image/round1_scene.png");
    if (pDerivedObj->background)
        printf("[GameScene] Background loaded OK.\n");
    else
        printf("WARNING: GameScene background NOT FOUND!\n");
    // Load round images
    pDerivedObj->round_images[0] = al_load_bitmap("assets/image/round_1.png");
    pDerivedObj->round_images[1] = al_load_bitmap("assets/image/round_2.png");
    pDerivedObj->round_images[2] = al_load_bitmap("assets/image/round_3.png");




    for (int i = 0; i < 3; i++) {
        if (pDerivedObj->round_images[i])
            printf("[GameScene] Round image %d loaded OK.\n", i + 1);
        else
            printf("WARNING: Round image %d NOT FOUND!\n", i + 1);
    }
    // setting derived object member
    pDerivedObj->background = al_load_bitmap("assets/image/round1_scene.png");
    al_init_font_addon();
    al_init_ttf_addon();
    pDerivedObj->font = al_load_ttf_font("assets/font/Consolas.ttf", 24, 0);
    // Load round sounds
    pDerivedObj->round_sounds[0] = al_load_sample("assets/sound/round_1.mp3");
    pDerivedObj->round_sounds[1] = al_load_sample("assets/sound/round_2.mp3");
    pDerivedObj->round_sounds[2] = al_load_sample("assets/sound/round_3.mp3");




    for (int i = 0; i < 3; i++) {
        if (pDerivedObj->round_sounds[i])
            printf("[GameScene] Round sound %d loaded OK.\n", i + 1);
        else
            printf("WARNING: Round sound %d NOT FOUND!\n", i + 1);
    }




    // Load round BGM
    pDerivedObj->round_bgm[0] = al_load_sample("assets/sound/game_music.mp3");
    pDerivedObj->round_bgm[1] = al_load_sample("assets/sound/bgm_round2.mp3");
    pDerivedObj->round_bgm[2] = al_load_sample("assets/sound/bgm_round3.mp3");




    for (int i = 0; i < 3; i++) {
        if (pDerivedObj->round_bgm[i])
            printf("[GameScene] BGM for round %d loaded OK.\n", i + 1);
        else
            printf("WARNING: BGM for round %d NOT FOUND!\n", i + 1);
    }
    // Load BOX image
    pDerivedObj->box_image = al_load_bitmap("assets/image/box.png");
    if (pDerivedObj->box_image)
        printf("[GameScene] Box image loaded OK.\n");
    else
        printf("WARNING: Box image NOT FOUND!\n");




    // Load GENTONG image
    pDerivedObj->gentong_image = al_load_bitmap("assets/image/gentong.png");
    if (pDerivedObj->gentong_image)
        printf("[GameScene] Gentong image loaded OK.\n");
    else
        printf("WARNING: Gentong image NOT FOUND!\n");




    // Play first round sound
    if (pDerivedObj->round_sounds[0]) {
        al_play_sample(pDerivedObj->round_sounds[0], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        printf("[GameScene] Round 1 sound playing.\n");
    }




    // Play first round BGM
    if (pDerivedObj->round_bgm[0]) {
        pDerivedObj->round_bgm_instance = al_create_sample_instance(pDerivedObj->round_bgm[0]);
        al_set_sample_instance_playmode(pDerivedObj->round_bgm_instance, ALLEGRO_PLAYMODE_LOOP);
        al_attach_sample_instance_to_mixer(pDerivedObj->round_bgm_instance, al_get_default_mixer());
        al_set_sample_instance_gain(pDerivedObj->round_bgm_instance, 1.0f);
        al_play_sample_instance(pDerivedObj->round_bgm_instance);
        printf("[GameScene] Round 1 BGM playing.\n");
    } else {
        pDerivedObj->round_bgm_instance = NULL;
    }




    // Register elements
    pObj->pDerivedObj = pDerivedObj;
    pDerivedObj->scene = pObj;
    _Register_elements(pObj, New_Floor(Floor_L));
    _Register_elements(pObj, New_Teleport(Teleport_L));
    _Register_elements(pObj, New_Tree(Tree_L));
    _Register_elements(pObj, New_Character(Character_L));
    _Register_elements(pObj, New_Ball(Ball_L));
    _Register_elements(pObj, New_Character2(Character2_L));
  
   


   // int enemy3_count = 1;
   // int start_x3 = 600;
   // int spacing3 = 100;
   // for (int i = 0; i < enemy3_count; i++) {
   //     Elements *enemy3 = New_Enemy3(Enemy3_L);
   //     Enemy *e = (Enemy *)enemy3->pDerivedObj;
   //     e->x = start_x3 + i * spacing3;
   //     e->y = HEIGHT - e->height - 60;
   //     _Register_elements(pObj, enemy3);
   // }




    // setting derived object function
    pObj->Update = game_scene_update;
    pObj->Draw = game_scene_draw;
    pObj->Destroy = game_scene_destroy;




    printf("[GameScene] Scene created successfully!\n");




    return pObj;
}




void game_scene_update(Scene *self)
{
    GameScene *gs = (GameScene *)self->pDerivedObj;




    // === UPDATE ELEMENTS ===
    ElementVec allEle = _Get_all_elements(self);
    for (int i = 0; i < allEle.len; i++)
        allEle.arr[i]->Update(allEle.arr[i]);




    for (int i = 0; i < allEle.len; i++)
        allEle.arr[i]->Interact(allEle.arr[i]);




    for (int i = 0; i < allEle.len; i++){
        if (allEle.arr[i]->dele){
            _Remove_elements(self, allEle.arr[i]);
        }
    }


        // === CHECK IF ALL ENEMIES ARE DEFEATED ===
    if (!gs->round_locked && !gs->round_triggered && gs->enemies_spawned) {
        bool all_enemies_defeated = true;
        for (int i = 0; i < allEle.len; i++) {
            if (allEle.arr[i]->label == Enemy_L || allEle.arr[i]->label == Enemy2_L || allEle.arr[i]->label == Enemy3_L) {
                all_enemies_defeated = false;
                break;
            }
        }


        if (all_enemies_defeated) {
            printf("[GameScene] All enemies defeated in round %d!\n", gs->round_counter);
            gs->round_locked = true;          // Prevent re-trigger
            gs->round_advancing = true;       // Signal advancement
            gs->round_timer = 0;
        }
    }




    // === ROUND TRIGGER ===
    if (gs->round_triggered) {
        gs->round_timer++;
        if (gs->round_timer > FPS * 2) {
            gs->round_triggered = false;
            printf("[GameScene] Round %d display done.\n", gs->round_counter);
        }
    }




    


        // === ADVANCE TO NEXT ROUND ===
    if (gs->round_advancing) {
        gs->round_timer++;
        if (gs->round_timer > FPS * 3) {
            gs->round_counter++;
            if (gs->round_counter > 3) {
                printf("[GameScene] All rounds completed!\n");
                // You can call SceneSwitch or game over here
                // e.g., SceneManager_switch_to(END_SCENE_ID);
            } else {
                printf("[GameScene] Advancing to round %d...\n", gs->round_counter);
                gs->round_triggered = true;
                gs->enemies_spawned = false;
                gs->enemy_defeated = false;
                gs->round_timer = 0;
                gs->round_locked = false;
                gs->round_advancing = false;


                // Change background music
                if (gs->round_bgm_instance) {
                    al_stop_sample_instance(gs->round_bgm_instance);
                    al_destroy_sample_instance(gs->round_bgm_instance);
                }
                gs->round_bgm_instance = al_create_sample_instance(gs->round_bgm[gs->round_counter - 1]);
                al_set_sample_instance_playmode(gs->round_bgm_instance, ALLEGRO_PLAYMODE_LOOP);
                al_attach_sample_instance_to_mixer(gs->round_bgm_instance, al_get_default_mixer());
                al_play_sample_instance(gs->round_bgm_instance);


                // Play round sound
                 if (gs->round_sounds[gs->round_counter]) {
                al_play_sample(gs->round_sounds[gs->round_counter], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                printf("[GameScene] Advance to Round %d SOUND playing.\n", gs->round_counter + 1);
            	}


            }
        }
    }


    // === SPAWN ENEMIES FOR CURRENT ROUND ===
    if (!gs->round_triggered && !gs->enemies_spawned && !gs->round_locked) {
        int enemy_count = 1;  // Default to 1 enemy
        int start_x = 500;    // Default starting position
        int spacing = 100;    // Default spacing


        switch (gs->round_counter) {
            case 1: // Round 1 - Only Enemy1
                enemy_count = 3;
                start_x = 500;
                spacing = 100;
                for (int i = 0; i < enemy_count; i++) {
                    Elements *enemy = New_Enemy(Enemy_L);
                    Enemy *e = (Enemy *)enemy->pDerivedObj;
                    e->x = start_x + i * spacing;
                    e->y = HEIGHT - e->height - 60;
                    _Register_elements(self, enemy);
                    printf("[GameScene] Round 1: Enemy1 %d spawned at (%d, %d)\n",
                        i+1, e->x, e->y);
                }
                break;
               
            case 2: // Round 2 - Only Enemy2
                enemy_count = 2;
                start_x = 400;
                spacing = 150;
                for (int i = 0; i < enemy_count; i++) {
                    Elements *enemy2 = New_Enemy2(Enemy2_L);
                    Enemy *e = (Enemy *)enemy2->pDerivedObj;
                    e->x = start_x + i * spacing;
                    e->y = HEIGHT - e->height - 60;
                    _Register_elements(self, enemy2);
                    printf("[GameScene] Round 2: Enemy2 %d spawned at (%d, %d)\n",
                        i+1, e->x, e->y);
                }
                break;
               
            case 3: // Round 3 - Only Enemy3
                enemy_count = 1;
                start_x = 600;
                for (int i = 0; i < enemy_count; i++) {
                    Elements *enemy3 = New_Enemy3(Enemy3_L);
                    Enemy *e = (Enemy *)enemy3->pDerivedObj;
                    e->x = start_x;
                    e->y = HEIGHT - e->height - 60;
                    _Register_elements(self, enemy3);
                    printf("[GameScene] Round 3: Enemy3 spawned at (%d, %d)\n",
                        e->x, e->y);
                }
                break;
            }


            gs->enemies_spawned = true;
            printf("[GameScene] Round %d: %d enemies spawned\n", gs->round_counter, enemy_count);
        }


    // === CHECK IF ALL ENEMIES DEFEATED ===
    if (gs->enemies_spawned && !gs->enemy_defeated) {
        bool all_defeated = true;
       
        // Check the appropriate enemy type for current round
        int enemy_label = 0;
        switch (gs->round_counter) {
            case 1: enemy_label = Enemy_L; break;
            case 2: enemy_label = Enemy2_L; break;
            case 3: enemy_label = Enemy3_L; break;
        }
       
        ElementVec enemies = _Get_label_elements(self, enemy_label);
       
        if (enemies.len == 0) {
            all_defeated = true; // No enemies found
        } else {
            for (int i = 0; i < enemies.len; i++) {
                Enemy *e = (Enemy *)enemies.arr[i]->pDerivedObj;
                if (e->active_proj) {  // Assuming this tracks if enemy is active
                    all_defeated = false;
                    break;
                }
            }
        }
        if (all_defeated) {
            gs->enemy_defeated = true;
            printf("[GameScene] Round %d: All enemies defeated!\n", gs->round_counter);
        }
    }


    // === RESET ENEMY SPAWN FLAG WHEN ROUND ADVANCES ===
    if (gs->round_advancing) {
        gs->enemies_spawned = false;
    }




    // === MANUAL TEST enemy_defeated (still keep for debug) ===
    if (key_state[ALLEGRO_KEY_T] && gs->round_triggered == false && gs->round_advancing == false) {
        printf("[GameScene] Enemy defeated (manual TEST).\n");
        gs->enemy_defeated = true;
    }




    // === new update 15:11 2025/6/8 ===
    // === ROUND ADVANCE START (manual trigger with key N) ===
    if (gs->enemy_defeated && gs->round_advancing == false && gs->round_counter < 3) {
        if (key_state[ALLEGRO_KEY_N]) {  // Press N to advance
            if (gs->round_sounds[gs->round_counter]) {
                al_play_sample(gs->round_sounds[gs->round_counter], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                printf("[GameScene] Advance to Round %d SOUND playing.\n", gs->round_counter + 1);
            }




            gs->round_advancing = true;
            gs->advance_timer = 0;
            gs->round_locked = true;
            printf("[GameScene] Next Round button pressed → Advancing!\n");
        }
    }




    // === ROUND ADVANCING LOGIC ===
    if (gs->round_advancing) {
        if (gs->round_bgm_instance) {
            float volume = 1.0f - (gs->advance_timer / (float)(FPS * 2));
            if (volume < 0.0f) volume = 0.0f;
            al_set_sample_instance_gain(gs->round_bgm_instance, volume);
        }


        gs->advance_timer++;


        if (gs->advance_timer > FPS * 2) {
            if (gs->round_bgm_instance) {
                al_stop_sample_instance(gs->round_bgm_instance);
                al_destroy_sample_instance(gs->round_bgm_instance);
                gs->round_bgm_instance = NULL;
            }


        gs->round_counter++;


        // PLAY NEW ROUND BGM
        if (gs->round_counter <= 3 && gs->round_bgm[gs->round_counter - 1]) {
            gs->round_bgm_instance = al_create_sample_instance(gs->round_bgm[gs->round_counter - 1]);
            al_set_sample_instance_playmode(gs->round_bgm_instance, ALLEGRO_PLAYMODE_LOOP);
            al_attach_sample_instance_to_mixer(gs->round_bgm_instance, al_get_default_mixer());
            al_set_sample_instance_gain(gs->round_bgm_instance, 1.0f);
            al_play_sample_instance(gs->round_bgm_instance);
            printf("[GameScene] Round %d BGM playing.\n", gs->round_counter);
        }


        // PLAY NEW ROUND SOUND (THIS IS THE FIX!)
        if (gs->round_counter <= 3 && gs->round_sounds[gs->round_counter - 1]) {
            al_play_sample(gs->round_sounds[gs->round_counter - 1], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
            printf("[GameScene] Round %d SOUND playing.\n", gs->round_counter);
        }


        // === RESET STATE ===
        gs->round_triggered = true;
        gs->round_timer = 0;
        gs->enemy_defeated = false;
        gs->round_advancing = false;
        gs->round_locked = false;


        printf("[GameScene] Advanced to Round %d.\n", gs->round_counter);
            
        }
    }



    // === new update 15:11 2025/6/8 ===
    // === MANUAL TEST SCENE SWITCH (keys changed to Q E Z) ===
    if (key_state[ALLEGRO_KEY_Q]) {  // Victory Scene
        self->scene_end = true;
        window = VictoryScene_L;
        printf("[GameScene] Victory Scene Triggered!\n");
    }
    if (key_state[ALLEGRO_KEY_E]) {  // Death Scene
        self->scene_end = true;
        window = DeathScene_L;
        printf("[GameScene] Death Scene Triggered!\n");
    }
    if (key_state[ALLEGRO_KEY_Z]) {  // Credit Scene
        self->scene_end = true;
        window = CreditScene_L;
        printf("[GameScene] Credit Scene Triggered!\n");
    }


   
}








void game_scene_draw(Scene *self)
{
    GameScene *gs = ((GameScene *)(self->pDerivedObj));
     // === DRAW BACKGROUND ===
    al_clear_to_color(al_map_rgb(0, 0, 0));
    if (gs->background)
        al_draw_bitmap(gs->background, 0, 0, 0);




    // === DRAW FLOOR ===
    ElementVec floorVec = _Get_label_elements(self, Floor_L);
    for (int i = 0; i < floorVec.len; i++)
        floorVec.arr[i]->Draw(floorVec.arr[i]);




    // === DRAW ROUND INDICATOR ===
    if (gs->round_triggered && gs->round_counter <= 3) {
        ALLEGRO_BITMAP *img = gs->round_images[gs->round_counter - 1];
        if (img) {
            int img_w = al_get_bitmap_width(img);
            al_draw_bitmap(img, WIDTH / 2 - img_w / 2, 100, 0);
        }
    }
    // === DRAW ELEMENTS ===
    ElementVec allEle = _Get_all_elements(self);
    for (int i = 0; i < allEle.len; i++)
    {
        Elements *ele = allEle.arr[i];
        if (ele->dele) continue; // 如果元素已標記刪除，就跳過
        //printf("ele->label:%s\n", ele->label);
        // 如果是 Character2，顯示血量
        if (ele->label == Character2_L )
        {  
            //printf("draw_character2");
            Character2 *ch2 = (Character2 *)ele->pDerivedObj;
            char buf[64];
            if(ch2->blood > 0){
                ele->Draw(ele);
                sprintf(buf, "Character 2");
                al_draw_text(gs->font, al_map_rgb(255, 0, 0), ch2->x, ch2->y - 20, 0, buf);
            }
                //blood
            sprintf(buf, "Character2 Blood: %d", ch2->blood);
            al_draw_text(gs->font, al_map_rgb( 255, 0, 0), 10, 70, 0, buf);




           
           
            if(ch2->cool_J / FPS == 0){
                sprintf(buf, "J");
                al_draw_text(gs->font, al_map_rgb(255, 0, 0), 820, 70, 0, buf);
            }
            else{
                sprintf(buf, "%d", ch2->cool_J / FPS);
                al_draw_text(gs->font, al_map_rgb(200, 200, 200), 820, 70, 0, buf);
            }
            if(ch2->cool_K / FPS == 0){
                sprintf(buf, "  K");
                al_draw_text(gs->font, al_map_rgb(255, 0, 0), 820, 70, 0, buf);
            }
            else{
                sprintf(buf, "  %d", ch2->cool_K / FPS);
                al_draw_text(gs->font, al_map_rgb(200, 200, 200), 820, 70, 0, buf);
            }
            if(ch2->cool_L / FPS == 0){
                sprintf(buf, "    L");
                al_draw_text(gs->font, al_map_rgb(255, 0, 0), 820, 70, 0, buf);
            }
            else{
                sprintf(buf, "    %d", ch2->cool_L / FPS);
                al_draw_text(gs->font, al_map_rgb(200, 200, 200), 820, 70, 0, buf);
            }
            int maxblood = 100 + (ch2->level * ch2->add_blood);
            float percent = (float)ch2->blood / maxblood; // 取得百分比
            float barLength = percent * barlong; // 計算血條寬度
            al_draw_filled_rectangle(300 - 2, 70 - 2, 810 + 2, 90 + 3, al_map_rgb(0, 0, 0));//black line of blood
            al_draw_filled_rectangle(300, 70, 810, 90, al_map_rgb(150, 150, 150)); // grey background of blood
            if(ch2->blood > 0){
                al_draw_filled_rectangle(300, 70, 300 + barLength, 90, al_map_rgb(255, 0, 0));
                // 如果只想顯示一個 Character2 的血量，可在這 break;
            }




            //level
            int maxpoints = (ch2->level + 1) * ch2->add_blood;
            sprintf(buf, "Level:%d points: %d/%d", ch2->level, ch2->levelup_points, maxpoints);
            al_draw_text(gs->font, al_map_rgb( 0, 255, 0), 10, 100, 0, buf);
           
            percent = (float)ch2->levelup_points / maxpoints; // 取得百分比
            barLength = percent * barlong; // 計算血條寬度
            al_draw_filled_rectangle( 300 - 2, 100 - 2, 300 + 510 + 2, 120 + 3, al_map_rgb( 0, 0, 0));
            al_draw_filled_rectangle(300,      100,     300 + 510,     120,     al_map_rgb(150, 150, 150)); // grey background of level
            al_draw_filled_rectangle(300, 100, 300 + barLength, 120, al_map_rgb(0, 255, 0));
           
        }
        else if(ele->label == Character_L)
        {
            Character *ch = (Character *)ele->pDerivedObj;
            char buf[64];
           
            if(ch->blood > 0){
                ele->Draw(ele);
                sprintf(buf, "Character 1");
                al_draw_text(gs->font, al_map_rgb(255, 0, 0), ch->x, ch->y - 20, 0, buf);
            }
            sprintf(buf, "Character1 Blood: %d", ch->blood);
            al_draw_text(gs->font, al_map_rgb(255, 0, 0), 10, 10, 0, buf);
           




           
            if(ch->cool_X / FPS == 0){
                sprintf(buf, "X");
                al_draw_text(gs->font, al_map_rgb(255, 0, 0), 820, 10, 0, buf);
            }
            else{
                sprintf(buf, "%d", ch->cool_X / FPS);
                al_draw_text(gs->font, al_map_rgb(200, 200, 200), 820, 10, 0, buf);
            }
            if(ch->cool_C / FPS == 0){
                sprintf(buf, "  C");
                al_draw_text(gs->font, al_map_rgb(255, 0, 0), 820, 10, 0, buf);
            }
            else{
                sprintf(buf, "  %d", ch->cool_C / FPS);
                al_draw_text(gs->font, al_map_rgb(200, 200, 200), 820, 10, 0, buf);
            }
            if(ch->cool_V / FPS == 0){
                sprintf(buf, "    V");
                al_draw_text(gs->font, al_map_rgb(255, 0, 0), 820, 10, 0, buf);
            }
            else{
                sprintf(buf, "    %d", ch->cool_V / FPS);
                al_draw_text(gs->font, al_map_rgb(200, 200, 200), 820, 10, 0, buf);
            }
            int maxblood = 100 + (ch->level * ch->add_blood);
            float percent = (float)ch->blood / maxblood; // 取得百分比
            float barLength = percent * barlong; // 計算血條寬度
            al_draw_filled_rectangle(300 - 2, 10 - 2, 810 + 2, 30 + 3, al_map_rgb(0, 0, 0));//blackline of blood
            al_draw_filled_rectangle(300, 10, 810, 30, al_map_rgb(150, 150, 150)); // grey background of blood
            if(ch->blood > 0){
                al_draw_filled_rectangle(300, 10, 300 + barLength, 30, al_map_rgb(255, 0, 0));
                // 如果只想顯示一個 Character2 的血量，可在這 break;
            }
                        //level
            int maxpoints = (ch->level + 1) * ch->add_blood;
            sprintf(buf, "Level:%d points: %d/%d", ch->level, ch->levelup_points, maxpoints);
            al_draw_text(gs->font, al_map_rgb( 0, 255, 0), 10, 40, 0, buf);
            percent = (float)ch->levelup_points / maxpoints; // 取得百分比
            barLength = percent * barlong; // 計算血條寬度
            al_draw_filled_rectangle( 300 - 2, 40 - 2, 300 + 510 + 2, 60 + 3, al_map_rgb( 0, 0, 0));
            al_draw_filled_rectangle(300, 40, 300 + 510, 60, al_map_rgb(150, 150, 150)); // grey background of level
           
                al_draw_filled_rectangle(300, 40, 300 + barLength, 60, al_map_rgb(0, 255, 0));
           
           
        }
        else{
            ele->Draw(ele);
        }
   
    }
}








void game_scene_destroy(Scene *self)
{
    GameScene *Obj = (GameScene *)self->pDerivedObj;
    // Cleanup background
    if (Obj->background)
        al_destroy_bitmap(Obj->background);




    // Cleanup round images
    for (int i = 0; i < 3; i++) {
        if (Obj->round_images[i])
            al_destroy_bitmap(Obj->round_images[i]);
    }




    // Cleanup round sounds
    for (int i = 0; i < 3; i++) {
        if (Obj->round_sounds[i])
            al_destroy_sample(Obj->round_sounds[i]);
    }




    // Cleanup round BGM
    if (Obj->round_bgm_instance) {
        al_stop_sample_instance(Obj->round_bgm_instance);
        al_destroy_sample_instance(Obj->round_bgm_instance);
    }




    for (int i = 0; i < 3; i++) {
        if (Obj->round_bgm[i])
            al_destroy_sample(Obj->round_bgm[i]);
    }




    // Cleanup box & gentong
    if (Obj->box_image)
        al_destroy_bitmap(Obj->box_image);




    if (Obj->gentong_image)
        al_destroy_bitmap(Obj->gentong_image);




    // Destroy elements
    ElementVec allEle = _Get_all_elements(self);
    for (int i = 0; i < allEle.len; i++)
        allEle.arr[i]->Destroy(allEle.arr[i]);




    free(Obj);
    free(self);




    printf("[GameScene] Scene destroyed.\n");
}






