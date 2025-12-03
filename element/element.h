#ifndef ELEMENT_H_INCLUDED
#define ELEMENT_H_INCLUDED
#include "../global.h"

class Elements {

public:
    void *pDerivedObj;
    int label; // the class of the element
    int id;    // the index in scene element list
    int inter_len;
    int inter_obj[MAX_ELEMENT]; // The label for the obj you want to interact
    bool dele;                  // If the object need to be deleted

    // interface for function
    virtual void Draw();
    virtual void Update();
    virtual void Interact();

    Elements(int label): pDerivedObj(this), label(label), id(0), inter_len(0), dele(false){}
    virtual ~Elements() = default;
};

Elements *New_Elements(int label);

#endif