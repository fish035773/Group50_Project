#include "sceneManager.h"
#include "../global.h"
#include "menu.h"
#include "gamescene.h"
#include "scene.h"
#include "victoryScene.h"
#include "deathScene.h"
#include "creditScene.h"
#include "startScene.h"
#include "aboutScene.h"

Scene* current_scene = nullptr;

void create_scene(SceneType type)
{
    switch (type)
    {
    case Menu_L:
        current_scene = new Menu(Menu_L);
        break;

    case GameScene_L:
        current_scene = new GameScene(GameScene_L);
        break;

    case StartScene_L:
        current_scene = new StartScene(StartScene_L);
        break;

    case VictoryScene_L:
        current_scene = new VictoryScene(VictoryScene_L);
        break;

    case DeathScene_L:
        current_scene = new DeathScene(DeathScene_L);
        break;

    case CreditScene_L:
        current_scene = new CreditScene(CreditScene_L);
        break;

    case About_L:
        current_scene = new AboutScene(About_L);
        break;
    }

    Elements::scene = current_scene;
}
