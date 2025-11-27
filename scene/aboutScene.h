#ifndef ABOUTSCENE_H_INCLUDED
#define ABOUTSCENE_H_INCLUDED

#include "scene.h"

// Declare function-nya disini:

Scene* New_AboutScene(int label);
void about_scene_update(Scene* self);
void about_scene_draw(Scene* self);
void about_scene_destroy(Scene* self);

#endif