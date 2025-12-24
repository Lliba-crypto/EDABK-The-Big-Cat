#include "map_track_alg.h"

/* NESW */
const int DELTA_X[4] = {0, 1, 0, -1};
const int DELTA_Y[4] = {-1, 0, 1, 0};

/* Ground-truth maze */
cell_type REAL_MAZE[MAZE_SIZE][MAZE_SIZE] =
{
    // y = 0
    {
        {0b1101, 0, 0, {0, 0}}, {0b1101, 0, 0, {0, 0}}, {0b1011, 0, 0, {0, 0}}, {0b1100, 0, 0, {0, 0}},
        {0b1011, 0, 0, {0, 0}}, {0b1010, 0, 0, {0, 0}}, {0b1010, 0, 0, {0, 0}}, {0b1100, 0, 0, {0, 0}},
        {0b1101, 0, 0, {0, 0}}, {0b1011, 0, 0, {0, 0}}, {0b1100, 0, 0, {0, 0}}, {0b1101, 0, 0, {0, 0}},
        {0b1011, 0, 0, {0, 0}}, {0b1010, 0, 0, {0, 0}}, {0b1100, 0, 0, {0, 0}}, {0b1101, 0, 0, {0, 0}}
    },
    // y = 1
    {
        {0b0011, 0, 0, {0, 0}}, {0b0010, 0, 0, {0, 0}}, {0b1010, 0, 0, {0, 0}}, {0b0010, 0, 0, {0, 0}},
        {0b1100, 0, 0, {0, 0}}, {0b1101, 0, 0, {0, 0}}, {0b1101, 0, 0, {0, 0}}, {0b0101, 0, 0, {0, 0}},
        {0b0101, 0, 0, {0, 0}}, {0b1101, 0, 0, {0, 0}}, {0b0101, 0, 0, {0, 0}}, {0b0101, 0, 0, {0, 0}},
        {0b1101, 0, 0, {0, 0}}, {0b1101, 0, 0, {0, 0}}, {0b0011, 0, 0, {0, 0}}, {0b0100, 0, 0, {0, 0}}
    },
    // y = 2
    {
        {0b1011, 0, 0, {0, 0}}, {0b1010, 0, 0, {0, 0}}, {0b1010, 0, 0, {0, 0}}, {0b1000, 0, 0, {0, 0}},
        {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}},
        {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}},
        {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b1000, 0, 0, {0, 0}}, {0b0100, 0, 0, {0, 0}}
    },
    // y = 3
    {
        {0b1001, 0, 0, {0, 0}}, {0b1000, 0, 0, {0, 0}}, {0b1000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}},
        {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}},
        {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}},
        {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0100, 0, 0, {0, 0}}
    },
    // y = 4
    {
        {0b0001, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}},
        {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}},
        {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}},
        {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0100, 0, 0, {0, 0}}
    },
    // y = 5
    {
        {0b0001, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}},
        {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}},
        {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}},
        {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0100, 0, 0, {0, 0}}
    },
    // y = 6
    {
        {0b0001, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}},
        {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}},
        {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}},
        {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0100, 0, 0, {0, 0}}
    },
    // y = 7
    {
        {0b0001, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}},
        {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}},
        {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}},
        {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0100, 0, 0, {0, 0}}
    },
    // y = 8
    {
        {0b0001, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}},
        {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}},
        {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}},
        {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0100, 0, 0, {0, 0}}
    },
    // y = 9
    {
        {0b0001, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}},
        {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}},
        {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}},
        {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0100, 0, 0, {0, 0}}
    },
    // y = 10
    {
        {0b0001, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}},
        {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}},
        {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}},
        {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0100, 0, 0, {0, 0}}
    },
    // y = 11
    {
        {0b0001, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}},
        {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}},
        {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}},
        {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0100, 0, 0, {0, 0}}
    },
    // y = 12
    {
        {0b0001, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}},
        {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}},
        {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}},
        {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0100, 0, 0, {0, 0}}
    },
    // y = 13
    {
        {0b0001, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}},
        {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}},
        {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}},
        {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0100, 0, 0, {0, 0}}
    },
    // y = 14
    {
        {0b0001, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}},
        {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}},
        {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}},
        {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0000, 0, 0, {0, 0}}, {0b0100, 0, 0, {0, 0}}
    },
    // y = 15
    {
        {0b0011, 0, 0, {0, 0}}, {0b0010, 0, 0, {0, 0}}, {0b0010, 0, 0, {0, 0}}, {0b0010, 0, 0, {0, 0}},
        {0b0010, 0, 0, {0, 0}}, {0b0010, 0, 0, {0, 0}}, {0b0010, 0, 0, {0, 0}}, {0b0010, 0, 0, {0, 0}},
        {0b0010, 0, 0, {0, 0}}, {0b0010, 0, 0, {0, 0}}, {0b0010, 0, 0, {0, 0}}, {0b0010, 0, 0, {0, 0}},
        {0b0010, 0, 0, {0, 0}}, {0b0010, 0, 0, {0, 0}}, {0b0010, 0, 0, {0, 0}}, {0b0110, 0, 0, {0, 0}}
    }
};

//  ===== FUNCTIONS DEFINITION =====

void initMouseMaze(mouse_type *mouse, cell_type maze[][MAZE_SIZE])
{
    *mouse = (mouse_type){2.0f, 0, 0}; // direction=East, at (0,0)

    // Initialize our undiscovered map
    for (int y = 0; y < MAZE_SIZE; y++)
    {
        for (int x = 0; x < MAZE_SIZE; x++)
        {
            maze[y][x].dist = 255;
            maze[y][x].previous_point = (point_type){-1, -1};
            maze[y][x].known = false;
            maze[y][x].wall = 0x0F; // unknown -> all walls assumed
        }
    }

    // Start cell_type known
    maze[0][0].dist = 0;
    maze[0][0].previous_point = (point_type){0, 0};
    maze[0][0].known = true;

    setWall(maze, mouse); // Read sensors at the start

    // Render the ground-truth maze (for debugging/sim view)
    renderMaze(REAL_MAZE);

    printf("Initialized\n");
    fetchMouseData(*mouse);
}

void explore(cell_type maze[][MAZE_SIZE], mouse_type *mouse)
{
    while (1)
    {
        // If nothing unknown remains, we are done
        point_type target = findNearestUnknown(maze, mouse->x, mouse->y);
        if (target.x == -1)
        {
            printf("Exploration completed\n");
            break;
        }

        int prev_x = mouse->x, prev_y = mouse->y;

        // Try to step into one adjacent, unexplored, and unblocked cell.
        int x = mouse->x, y = mouse->y;
        bool moved = false;
        for (int dir = 0; dir < 4; dir++)
        {
            int nx = x + DELTA_X[dir];
            int ny = y + DELTA_Y[dir];
            if (!isValid(nx, ny)) continue;
            if (maze[ny][nx].known) continue; // don't enter explored cells
            // Wall check: if there is a wall in the intended direction from the current cell, skip
            if ((maze[y][x].wall >> (3 - dir)) & 1) continue; // 1 = wall present

            rotateTo(mouse, (float) dir + 1); // NESW are 1,2,3,4
            forward(mouse);
            setWall(maze, mouse);
            moved = true;
            break;
        }

        // If explore() couldn't move (still at the same position), navigate toward the nearest unknown
        if (!moved || (mouse->x == prev_x && mouse->y == prev_y))
        {
            goTo(maze, mouse, target.x, target.y);
        }
    }
}

// Other functions

void fetchMouseData(mouse_type mouse)
{
    printf("Mouse data:\n");
    printf("    x: %d\n", mouse.x);
    printf("    y: %d\n", mouse.y);

    float d = mouse.direction;
    printf("    Direction: ");
    if (d == 1.0f) printf("North      (%.1f)", d);
    else if (d == 1.5f) printf("North East (%.1f)", d);
    else if (d == 2.0f) printf("East       (%.1f)", d);
    else if (d == 2.5f) printf("South East (%.1f)", d);
    else if (d == 3.0f) printf("South      (%.1f)", d);
    else if (d == 3.5f) printf("South West (%.1f)", d);
    else if (d == 4.0f || d == 0.0f) printf("West       (%.1f)", d);
    else if (d == 0.5f) printf("North West (%.1f)", d);
    printf("\n");
}

int readSensor(mouse_type *mouse)
{
    return REAL_MAZE[mouse->y][mouse->x].wall;
}

void forward(mouse_type *mouse)
{
    printf("Gone to x: %i y:%i\n", mouse->x, mouse->y);
    float d = mouse->direction;
    if (d >= 0.5f && d <= 1.5f) mouse->y--; // N
    if (d >= 2.5f && d <= 3.5f) mouse->y++; // S
    if (d >= 1.5f && d <= 2.5f) mouse->x++; // E
    if ((d >= 3.5f && d <= 4.0f) || (d >= 0.0f && d <= 0.5f)) mouse->x--; // W
}

void turnRight(mouse_type *mouse, float direction)
{
    mouse->direction = fmodf(mouse->direction + direction, 4.0f);
    if (mouse->direction < 0.0f) mouse->direction += 4.0f;
}

void turnLeft(mouse_type *mouse, float direction)
{
    mouse->direction = fmodf(mouse->direction - direction, 4.0f);
    if (mouse->direction < 0.0f) mouse->direction += 4.0f;
}

void rotateTo(mouse_type *mouse, float goal_direction)
{
    printf("Rotating to %.2f\n", goal_direction);
    float g = fmodf(goal_direction, 4.0f);
    if (g < 0.0f) g += 4.0f;

    float diff = g - mouse->direction;
    if (diff > 2.0f) diff -= 4.0f;
    if (diff < -2.0f) diff += 4.0f;

    if (diff > 0) turnRight(mouse, diff);
    else if (diff < 0) turnLeft(mouse, -diff);

    mouse->direction = fmodf(roundf(mouse->direction * 2.0f) / 2.0f, 4.0f);
    if (mouse->direction < 0.0f) mouse->direction += 4.0f;
}

bool isValid(int x, int y)
{
    return x >= 0 && y >= 0 && x < MAZE_SIZE && y < MAZE_SIZE;
}

void setWall(cell_type maze[][MAZE_SIZE], mouse_type *mouse)
{
    int x = mouse->x, y = mouse->y;
    maze[y][x].known = true;
    maze[y][x].wall = (uint8_t) readSensor(mouse);
}

int findPath(cell_type maze[][MAZE_SIZE], int sx, int sy, int gx, int gy)
{
    for (int y = 0; y < MAZE_SIZE; y++)
    {
        for (int x = 0; x < MAZE_SIZE; x++)
        {
            maze[y][x].dist = 255;
            maze[y][x].previous_point = (point_type){-1, -1};
        }
    }

    point_type queue[MAZE_SIZE * MAZE_SIZE];
    int front = 0, rear = 0;
    bool visited[MAZE_SIZE][MAZE_SIZE] = {false};

    maze[sy][sx].dist = 0;
    visited[sy][sx] = true;
    queue[rear++] = (point_type){sx, sy};

    while (front < rear)
    {
        point_type cur = queue[front++];
        int x = cur.x, y = cur.y;

        if (x == gx && y == gy) return maze[y][x].dist;

        for (int dir = 0; dir < 4; dir++)
        {
            if ((maze[y][x].wall >> (3 - dir)) & 1) continue; // wall blocks
            int nx = x + DELTA_X[dir];
            int ny = y + DELTA_Y[dir];
            if (isValid(nx, ny) && !visited[ny][nx] &&
                (maze[ny][nx].known || (nx == gx && ny == gy)))
            {
                visited[ny][nx] = true;
                maze[ny][nx].dist = (uint16_t) (maze[y][x].dist + 1);
                maze[ny][nx].previous_point = (point_type){x, y};
                queue[rear++] = (point_type){nx, ny};
            }
        }
    }
    return -1;
}

void goTo(cell_type maze[][MAZE_SIZE], mouse_type *mouse, int gx, int gy)
{
    int path_length = findPath(maze, mouse->x, mouse->y, gx, gy);
    if (path_length == -1) return;

    point_type path[256];
    int rear = 0;

    int x = gx, y = gy;
    path[rear++] = (point_type){x, y};

    while (x != mouse->x || y != mouse->y)
    {
        point_type p = maze[y][x].previous_point;
        path[rear++] = p;
        x = p.x;
        y = p.y;
    }

    // Move from the start -> goal following the reconstructed path in reverse order
    for (int i = path_length - 1; i >= 0 && i < rear; i--)
    {
        point_type next = path[i];
        int dx = next.x - mouse->x;
        int dy = next.y - mouse->y;

        int dir = -1; // 0=N,1=E,2=S,3=W
        if (dx == 0 && dy == -1) dir = 0; // North
        else if (dx == 1 && dy == 0) dir = 1; // East
        else if (dx == 0 && dy == 1) dir = 2; // South
        else if (dx == -1 && dy == 0) dir = 3; // West

        if (dir != -1)
        {
            // Rotate to face the direction and move forward one cell
            rotateTo(mouse, (float) dir + 1);
            forward(mouse);
            setWall(maze, mouse);
        }
    }
}

point_type findNearestUnknown(cell_type maze[][MAZE_SIZE], int x, int y)
{
    point_type queue[MAZE_SIZE * MAZE_SIZE];
    bool visited[MAZE_SIZE][MAZE_SIZE] = {0};
    int front = 0, rear = 0;

    queue[rear++] = (point_type){x, y};
    visited[y][x] = true;

    while (front < rear)
    {
        point_type cur = queue[front++];
        if (!maze[cur.y][cur.x].known) return cur;

        for (int dir = 0; dir < 4; dir++)
        {
            int nx = cur.x + DELTA_X[dir];
            int ny = cur.y + DELTA_Y[dir];
            if (!isValid(nx, ny)) continue;
            if (visited[ny][nx]) continue;
            if ((maze[cur.y][cur.x].wall >> (3 - dir)) & 1) continue; // blocked
            visited[ny][nx] = true;
            queue[rear++] = (point_type){nx, ny};
        }
    }
    return (point_type){-1, -1};
}

void returnToStart(cell_type maze[][MAZE_SIZE], mouse_type *mouse)
{
    goTo(maze, mouse, 0, 0);
}

void calculateRoute(cell_type maze[][MAZE_SIZE], mouse_type *mouse)
{
}

void renderMaze(cell_type maze[][MAZE_SIZE])
{
    for (int y = 0; y < MAZE_SIZE; y++)
    {
        for (int x = 0; x < MAZE_SIZE; x++)
        {
            printf("+");
            printf((maze[y][x].wall >> 3) & 1 ? "--" : "  ");
            printf("+");
        }
        printf("\n");

        for (int x = 0; x < MAZE_SIZE; x++)
        {
            printf((maze[y][x].wall >> 0) & 1 ? "|" : " ");
            printf("  ");
            printf((maze[y][x].wall >> 2) & 1 ? "|" : " ");
        }
        printf("\n");

        for (int x = 0; x < MAZE_SIZE; x++)
        {
            printf("+");
            printf((maze[y][x].wall >> 1) & 1 ? "--" : "  ");
            printf("+");
        }
        printf("\n");
    }
}
