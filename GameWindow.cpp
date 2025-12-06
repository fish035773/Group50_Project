#include "GameWindow.h"
#include "GAME_ASSERT.h"
#include "global.h"
#include "shapes/Shape.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "scene/sceneManager.h"
#include "scene/scene.h"

GameWindow::GameWindow(const char* title)
    : display(nullptr), event_queue(nullptr), timer(nullptr), title(title), running(true)
{}

GameWindow::~GameWindow()
{
    Destroy();
}

bool GameWindow::Init()
{
    printf("[GameWindow] Initializing...\n");

    if (!al_init()) {
        printf("Failed to initialize Allegro.\n");
        return false;
    }

    for (int i = 0; i < 8; i++) mouse_state[i] = false;

    al_init_primitives_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_image_addon();
    al_install_keyboard();
    al_install_mouse();
    al_install_audio();
    al_init_acodec_addon();
    al_reserve_samples(32);

    display = al_create_display(WIDTH, HEIGHT);
    if (!display) {
        printf("Failed to create display.\n");
        return false;
    }

    al_set_window_title(display, title);
     // initialize the icon on the display
    ALLEGRO_BITMAP *icon = al_load_bitmap("assets/image/icon.png");
    al_set_display_icon(display, icon);
    event_queue = al_create_event_queue();
    timer = al_create_timer(1.0 / FPS);

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));

    al_start_timer(timer);

    create_scene(Menu_L);

    return true;
}

void GameWindow::Run()
{
    ALLEGRO_EVENT ev;

    while (running)
    {
        al_wait_for_event(event_queue, &ev);
        ProcessEvent(ev);

        if (ev.type == ALLEGRO_EVENT_TIMER) {
            Update();
            Draw();
        }
    }
}

void GameWindow::ProcessEvent(ALLEGRO_EVENT& ev)
{
    switch (ev.type)
    {
    case ALLEGRO_EVENT_DISPLAY_CLOSE:
        running = false;
        break;

    case ALLEGRO_EVENT_KEY_DOWN:
        key_state[ev.keyboard.keycode] = true;
        break;

    case ALLEGRO_EVENT_KEY_UP:
        key_state[ev.keyboard.keycode] = false;
        break;

    case ALLEGRO_EVENT_MOUSE_AXES:
        mouse.x = ev.mouse.x;
        mouse.y = ev.mouse.y;
        break;

    case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
        mouse_state[ev.mouse.button] = true;
        break;

    case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
        mouse_state[ev.mouse.button] = false;
        break;
    }
}

void GameWindow::Update()
{
    if (current_scene)
        current_scene->Update();

    if (current_scene && current_scene->next_scene_requested)
    {
        SceneType next = current_scene->next_scene_type;

        delete current_scene;
        create_scene(next);
    }
}

void GameWindow::Draw()
{
    al_clear_to_color(al_map_rgb(100, 100, 100));

    if (current_scene)
        current_scene->Draw();

    al_flip_display();
}

void GameWindow::Destroy()
{
    if (current_scene) {
        delete current_scene;
        current_scene = nullptr;
    }

    if (timer) {
        al_destroy_timer(timer);
        timer = nullptr;
    }

    if (event_queue) {
        al_destroy_event_queue(event_queue);
        event_queue = nullptr;
    }

    if (display) {
        al_destroy_display(display);
        display = nullptr;
    }
}
