#include "global.h"
#include "shapes/Shape.h"

// variables for global usage
const double FPS = 80.0;
const int WIDTH = 1100;
const int HEIGHT = 672;
int window = 0;
ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_EVENT event;
ALLEGRO_TIMER *fps = NULL;
bool key_state[ALLEGRO_KEY_MAX] = {false};
bool mouse_state[8] = { false };

Point mouse;
bool debug_mode = true;
bool scene_switched = false;

