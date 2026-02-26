#ifndef MAP_TRACK_ALG_H
#define MAP_TRACK_ALG_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

// ===== CONSTANTS =====
#define MAZE_SIZE 16
#define TURN_PENALTY 10

extern const int DELTA_X[4];                            // direction NESW of mouse
extern const int DELTA_Y[4];                            // direction NESW of mouse

extern int stage;
// ===== USER-DEFINED DATA TYPES =====
typedef struct 
{ 
    float direction;
    int x, y; 
} mouse_type; // dir: 0..4 (0.5 steps)

typedef struct 
{ 
    int x, y; 
} point_type;

typedef struct 
{
    uint8_t  wall;          // NESW bits: 1 = wall
    bool     known;         // cell_type discovered
    uint16_t dist;          // BFS distance
    point_type    previous_point;
    uint16_t cost[4];       // Optimal distance
} cell_type;

extern cell_type REAL_MAZE[MAZE_SIZE][MAZE_SIZE]; // Ground-truth maze (unchanged from your data)

/* ===== OUR COORDINATE SYSTEM - APPLIED FOR THE ENTIRE SYSTEM =====
O--------------> x
|
|
|
|
|
V 
y
*/

// ===== FUNCTION PROTOTYPES =====

// Initilzation value of variables mouse & maze
void initMouseMaze (mouse_type *mouse, cell_type maze[][MAZE_SIZE]);

// Explore (adjust value of mouse) the whole maze => Adjust the value of maze
void explore        (cell_type maze[][MAZE_SIZE], mouse_type *mouse);

// Auxiliary functions
void   fetchMouseData          (mouse_type mouse);
int    readSensor              (mouse_type *mouse);
void   forward                 (cell_type maze[][MAZE_SIZE], mouse_type *mouse);     // move 1 cell_type straight
void   turnRight               (mouse_type *mouse, float direction);
void   turnLeft                (mouse_type *mouse, float direction);
void   rotateTo                (mouse_type *mouse, float goal_direction);
bool   isValid                 (int x, int y);
void   setWall                 (cell_type maze[][MAZE_SIZE], mouse_type *mouse);
int    findPath                (cell_type maze[][MAZE_SIZE], int sx, int sy, int gx, int gy);
void   goTo                    (cell_type maze[][MAZE_SIZE], mouse_type *mouse, int gx, int gy);
point_type  findNearestUnknown (cell_type maze[][MAZE_SIZE], int x, int y);
void   returnToStart           (cell_type maze[][MAZE_SIZE], mouse_type *mouse);
void   calculateRoute          (cell_type maze[][MAZE_SIZE], mouse_type *mouse);
void   renderMaze              (cell_type maze[][MAZE_SIZE]);
void   goToOptimal(cell_type maze[][MAZE_SIZE], mouse_type *mouse, int gx, int gy);
int    findOptimalPath(cell_type maze[][MAZE_SIZE], int sx, int sy, int gx, int gy);
#endif /* MAP_TRACK_ALG_H */
