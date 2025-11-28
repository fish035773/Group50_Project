#pragma once
#include "charater.h"
#include "Character2.h"
#include "floor.h"
#include "tree.h"
#include "teleport.h"
#include "ball.h"
#include "../scene/gamescene.h" // 如果需要 scene 指標

// wrapper functions
Elements* New_Character(int label);
Elements* New_Character2(int label);
Elements* New_Floor(int label);
Elements* New_Tree(int label);
Elements* New_Teleport(int label);
Elements* New_Ball(int label);
