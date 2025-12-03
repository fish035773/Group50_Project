#pragma once
#include "../global.h"
class Scene;

// 當前場景（指標指向 Scene 的子類）
extern Scene* current_scene;

// 建立場景（會自動 delete 舊場景）
void create_scene(SceneType type);
