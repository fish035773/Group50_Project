#include "elements_factory.h"

// Character
Elements* New_Character2(int label) {
    Character2* pDerivedObj = new Character2();
    Elements* pObj = New_Elements(label);
    pObj->pDerivedObj = pDerivedObj;

    pObj->Draw = [](Elements* e) { ((Character2*)e->pDerivedObj)->draw(); };
    pObj->Update = [](Elements* e) { ((Character2*)e->pDerivedObj)->update(); };
    pObj->Interact = [](Elements* e) { ((Character2*)e->pDerivedObj)->interact(); };
    pObj->Destroy = [](Elements* e) { delete (Character2*)e->pDerivedObj; free(e); };

    return pObj;
}
Elements* New_Character(int label) {
    Character* pDerivedObj = new Character();
    Elements* pObj = New_Elements(label);
    pObj->pDerivedObj = pDerivedObj;

    pObj->Draw = [](Elements* e) { ((Character*)e->pDerivedObj)->draw(); };
    pObj->Update = [](Elements* e) { ((Character*)e->pDerivedObj)->update(); };
    pObj->Interact = [](Elements* e) { ((Character*)e->pDerivedObj)->interact(); };
    pObj->Destroy = [](Elements* e) { delete (Character*)e->pDerivedObj; free(e); };

    return pObj;
}



