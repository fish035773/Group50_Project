#ifndef SCENEMANAGER_H_INCLUDED
#define SCENEMANAGER_H_INCLUDED
#include "scene.h"

typedef enum SceneType
{
    Menu_L = 0,
    GameScene_L = 1,
    StartScene_L = 2,
    VictoryScene_L = 3,
    DeathScene_L = 4,
    CreditScene_L = 5,
    // new update 16:57 2025/07/06
    //About scene
    About_L = 6    
} SceneType;
extern Scene *scene;
extern int window;

void create_scene(SceneType type);
#endif