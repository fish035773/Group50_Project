#include "sceneManager.h"
#include "../global.h"
#include "menu.h"
#include "gamescene.h"
#include "victoryScene.h"
#include "deathScene.h"
#include "creditScene.h"
#include "startScene.h"
#include "aboutScene.h"

Scene *scene = NULL;

void create_scene(SceneType type)
{
    switch (type)
    {
    case Menu_L:
        scene = New_Menu(Menu_L);
        printf("[SceneManager] Created Menu Scene.\n");
        break;

    case GameScene_L:
        scene = new GameScene(GameScene_L);
        printf("[SceneManager] Created Game Scene.\n");
        break;

    case StartScene_L:
        scene = New_StartScene(StartScene_L);
        printf("[SceneManager] Created Start Scene.\n");
        break;

    case VictoryScene_L:
        scene = New_VictoryScene(VictoryScene_L);
        printf("[SceneManager] Created Victory Scene.\n");
        break;

    case DeathScene_L:
        scene = New_DeathScene(DeathScene_L);
        printf("[SceneManager] Created Death Scene.\n");
        break;

    case CreditScene_L:
        scene = New_CreditScene(CreditScene_L);
        printf("[SceneManager] Created Credit Scene.\n");
        break;
    // new update 16:57 2025/07/06
    case About_L:    
        scene = New_AboutScene(About_L);
        printf("[SceneManager] Created About Scene.\n");
        break;

    default:
        printf("WARNING: Unknown SceneType %d!\n", type);
        scene = NULL;
        break;
    }
}
