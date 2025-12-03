#pragma once

#include "element.h"
#include <allegro5/allegro.h>
#include "charater.h"
#include "Character2.h"
class Floor : public Elements
{
public:
    int x, y;
    int width, height;
    ALLEGRO_BITMAP* img = nullptr;

    int map_data[6][6];

    // constructor
    Floor(int label);
    ~Floor();
    // override virtual functions from Elements
    void Update() override;
    void Interact() override;
    void Draw() override;

    void LoadMap();
    void InteractCharacter(Character* target);
};
