#ifndef CHARATER_H_INCLUDED
#define CHARATER_H_INCLUDED


#include <allegro5/allegro_audio.h>
#include "element.h"
#include "../shapes/Shape.h"
#include "../algif5/algif.h"
#include <stdbool.h>




// Define the possible states of the character
typedef enum CharacterType
{
    STOP = 0,   // Character is idle
    MOVE,       // Character is moving
    ATK        // Character is attacking
} CharacterType;

// Structure for character object
typedef struct _Character
{
    int x, y;                      // Position of the character
    int width, height;            // Dimensions of the character's sprite
    bool dir;                     // Facing direction: true = right, false = left
    int state;                    // Current state (STOP, MOVE, ATK)
    ALGIF_ANIMATION *gif_status[3]; // GIF animations for each state: stop, move, attack
    ALLEGRO_SAMPLE_INSTANCE *atk_Sound; // Attack sound effect
    int anime;                    // Animation counter (not always used)
    int anime_time;               // Total time animation lasts
    bool new_proj;                // Whether a new projectile has been created
    Shape *hitbox;                // Hitbox shape for collision detection
    bool is_jumping;             // Is the character currently jumping?
    double jump_start_time;      // When the jump started
    int ground_y;                // Y-position representing the ground level
    int blood;                   // Health points of the character
    int atk_type;
    int cool_V;
    int cool_C;
    int cool_X;
    int add_blood;
    int level;
    int levelup_points;
} Character;

// Define types of attacks (not used in main struct yet, could be for future combat system)
typedef enum {
    ATK_WEAK,
    ATK_MEDIUM,
    ATK_STRONG
} AttackType;

// Function to create and initialize a new Character2 object
Elements *New_Character(int label);

// Update function to handle character behavior and state transitions
void Character_update(Elements *self);

// Placeholder for handling character interactions with other objects
void Character_interact(Elements *self);

// Draw the character's current animation frame on screen
void Character_draw(Elements *self);

// Free all resources used by the character
void Character_destory(Elements *self);

// Helper function to move character and update its hitbox accordingly
void _Character_update_position(Elements *self, int dx, int dy);

#endif
