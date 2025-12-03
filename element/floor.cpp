#include "floor.h"
#include "charater.h"
#include "Character2.h"
#include "element.h"
#include "../scene/gamescene.h"
#include "../scene/sceneManager.h"
#include <cstdio>

Floor::Floor(int label)
    : Elements(label)
{
    img = al_load_bitmap("assets/image/floor.png");

    if (img) {
        width = al_get_bitmap_width(img);
        height = al_get_bitmap_height(img);
    }

    x = 0;
    y = 0;

    LoadMap();
}
Floor::~Floor()
{
    if (img)
        al_destroy_bitmap(img);
}

void Floor::LoadMap()
{
    FILE* data = fopen("assets/map/gamescene_map.txt", "r");
    if (!data) {
        printf("[Floor] ERROR: map file not found!\n");
        return;
    }

    for (int i = 0; i < 6; i++)
        for (int j = 0; j < 6; j++)
            fscanf(data, "%d", &map_data[i][j]);

    fclose(data);
}

void Floor::Update()
{
    // No update logic needed
}

void Floor::Interact() {
    const auto& all = scene->getAllElements();

    // 逐一檢查是否是 Character
    for (Elements* e : all) {
        if (auto* ch = dynamic_cast<Character*>(e)) {
            InteractCharacter(ch); 
        }
    }
}



// --------------------------------------------------
// Character boundary constraints
// --------------------------------------------------
void Floor::InteractCharacter(Character* tar)
{

    int right_limit = WIDTH - tar->width / 2;
    int left_limit = 0 - tar->width / 2;

    if (tar->x < left_limit)
        tar->update_position(left_limit - tar->x, 0);

    else if (tar->x > right_limit)
        tar->update_position(right_limit - tar->x, 0);
}

// --------------------------------------------------
// Draw tile map
// --------------------------------------------------
void Floor::Draw()
{
    if (!img) return;

    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            if (map_data[i][j]) {
                al_draw_bitmap(
                    img,
                    x + j * width,
                    y + i * height,
                    0
                );
            }
        }
    }
}
