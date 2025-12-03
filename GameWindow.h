#pragma once

#include <allegro5/allegro.h>

class GameWindow {
private:
    ALLEGRO_DISPLAY* display;
    ALLEGRO_EVENT_QUEUE* event_queue;
    ALLEGRO_TIMER* timer;

    const char* title;
    bool running;

public:
    GameWindow(const char* title);
    ~GameWindow();

    bool Init();
    void Run();
    void Destroy();

private:
    void ProcessEvent(ALLEGRO_EVENT& ev);
    void Update();
    void Draw();
};
