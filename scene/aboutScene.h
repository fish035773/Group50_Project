#pragma once
#include "scene.h"
#include <allegro5/allegro.h>

class AboutScene : public Scene {
private:
    ALLEGRO_BITMAP* howtoplay_bg;

public:
    AboutScene(int label);
    virtual ~AboutScene();

    virtual void Update() override;
    virtual void Draw() override;
    virtual void Destroy() override;
};
