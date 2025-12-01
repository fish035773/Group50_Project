#include "elements_factory.h"

Elements* New_Character(int label) {
    return new Character(label);
}

Elements* New_Character2(int label) {
    return new Character2(label);
}

Elements* New_Floor(int label) {
    return new Floor(label);
}

Elements* New_Tree(int label) {
    return new Tree(label);
}

Elements* New_Teleport(int label) {
    return new Teleport(label);
}

Elements* New_Ball(int label) {
    return new Ball(label);
}


