#pragma once
#include "../global.h"

class Scene;

class Elements {
public:
    static Scene* scene;

    int label;                    
    int id;                       // scene 內 index
    bool dele; 

public:
    Elements(int label);
    virtual ~Elements() = default;

    virtual void Update() {}
    virtual void Draw() {}
    virtual void Interact() {}
};
