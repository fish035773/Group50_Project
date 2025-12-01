#ifndef GLOBAL_H_INCLUDED
#define GLOBAL_H_INCLUDED

#include <iostream>
#include <allegro5/allegro.h>
#include "shapes/Point.h"

#define GAME_TERMINATE -1
#define debug(x)                                                            \
    if (x)                                                                  \
    {                                                                       \
        printf("check at line number %d in file %s\n", __LINE__, __FILE__); \
    }

#define MAX_ELEMENT 100

template<typename T>
inline T min(T a, T b) { return (a > b) ? b : a; }

template<typename T>
inline T max(T a, T b) { return (a < b) ? b : a; }

extern const double FPS;
extern const int WIDTH;
extern const int HEIGHT;
extern int window;
extern bool key_state[ALLEGRO_KEY_MAX];
extern bool mouse_state[ALLEGRO_MOUSE_MAX_EXTRA_AXES];
extern Point mouse;
extern ALLEGRO_EVENT_QUEUE* event_queue;
extern ALLEGRO_EVENT event;
extern ALLEGRO_TIMER* fps;
extern bool debug_mode;
extern int current_round;
extern int round_count;
extern bool scene_switched;

#endif
