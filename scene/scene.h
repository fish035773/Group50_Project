#pragma once
#include "../global.h"
#include "../element/element.h"
#include <vector>
class Scene {
public:
    int label;
    bool scene_end;

protected:
    std::vector<Elements*> elements;

public:
    Scene(int label);
    virtual ~Scene();

    // 基本行為（可被子類別 override）
    virtual void Update();
    virtual void Draw();
    virtual void Destroy();

    // 元素操作（新增 / 移除 / 查詢）
    void addElement(Elements* ele);
    void removeElement(Elements* ele);

    std::vector<Elements*> getAllElements() const;
    std::vector<Elements*> getElementsByLabel(int target_label) const;
};