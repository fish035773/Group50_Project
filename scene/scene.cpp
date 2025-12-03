#include "scene.h"
#include <algorithm>

Scene::Scene(int label)
    : label(label), scene_end(false) {}

Scene::~Scene() {
    Destroy();
}

void Scene::Update() {
    // 先更新所有元素
    for (auto* ele : elements) {
        if (ele && !ele->dele)
            ele->Update();
    }

    // 刪除 dele 的元素
    elements.erase(
        std::remove_if(elements.begin(), elements.end(),
            [](Elements* e) {
                if (e->dele) {
                    delete e;       // ★ 只需要 delete
                    return true;
                }
                return false;
            }),
        elements.end()
    );
}

void Scene::Draw() {
    for (auto* ele : elements) {
        if (ele && !ele->dele)
            ele->Draw();
    }
}

void Scene::Destroy() {
    for (auto* ele : elements) {
        delete ele;     // ★ 只需要 delete
    }
    elements.clear();
}

void Scene::addElement(Elements* ele) {
    if (ele)
        elements.push_back(ele);
}

void Scene::removeElement(Elements* ele) {
    elements.erase(
        std::remove(elements.begin(), elements.end(), ele),
        elements.end()
    );
}

std::vector<Elements*> Scene::getAllElements() const {
    return elements;
}

std::vector<Elements*> Scene::getElementsByLabel(int target_label) const {
    std::vector<Elements*> result;
    for (auto* ele : elements) {
        if (ele->label == target_label)
            result.push_back(ele);
    }
    return result;
}
