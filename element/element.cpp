#include "element.h"
#include "../scene/sceneManager.h"
Scene* Elements::scene = nullptr;

Elements::Elements(int label)
    : label(label), id(0), dele(false)
{
}
