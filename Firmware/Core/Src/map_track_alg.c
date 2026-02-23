#include "map_track_alg.h"
#include "ssd1306.h"
#include "drv8833.h"
#include "ssd1306_fonts.h"
#include "vl53l0x.h"
/* NESW */
const int DELTA_X[4] = {0, 1, 0, -1};
const int DELTA_Y[4] = {-1, 0, 1, 0};
#define FLT_TEXT_SIZE (1 + 1 + 1 +    8   + 1 + 1 + 5    + 1)
char buffer1[FLT_TEXT_SIZE],buffer2[FLT_TEXT_SIZE], buffer3[FLT_TEXT_SIZE], buffer4[FLT_TEXT_SIZE];
statInfo_t_VL53L0X sensorFront, sensorLeft, sensorRight;

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
            forward(maze, mouse);
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

void forward(cell_type maze[][MAZE_SIZE], mouse_type *mouse)
{
    printf("Gone to x: %i y:%i\n", mouse->x, mouse->y);
    float d = mouse->direction;
    if (d >= 0.5f && d <= 1.5f) mouse->y--; // N
    if (d >= 2.5f && d <= 3.5f) mouse->y++; // S
    if (d >= 1.5f && d <= 2.5f) mouse->x++; // E
    if ((d >= 3.5f && d <= 4.0f) || (d >= 0.0f && d <= 0.5f)) mouse->x--; // W

    ssd1306_Fill(Black);               // xoa man hinh

    ssd1306_SetCursor(1, 0);
    ssd1306_WriteString("Cell:", Font_7x10, White);
    ssd1306_SetCursor(30, 0);
    sprintf(buffer1, "%u", maze[mouse->y][mouse->x].wall); // Convert cell to string
    ssd1306_WriteString(buffer1, Font_7x10, White);

    ssd1306_SetCursor(1, 9);
    ssd1306_WriteString("Coor:", Font_7x10, White);
    ssd1306_SetCursor(30, 9);
    sprintf(buffer2, "(%d,%d)", mouse->x, mouse->y); // Convert x, y to string
    ssd1306_WriteString(buffer2, Font_7x10, White);

    ssd1306_SetCursor(1, 18);
    ssd1306_WriteString("Dirt:", Font_7x10, White);
    ssd1306_SetCursor(30, 18);
    sprintf(buffer3, "%f", mouse->direction); // Convert direction to string
    ssd1306_WriteString(buffer3, Font_7x10, White);

    ssd1306_SetCursor(1, 27);
    ssd1306_WriteString("Stage:", Font_7x10, White);
    ssd1306_SetCursor(30, 27);
    sprintf(buffer4, "%d", stage); // Convert direction to string
    ssd1306_WriteString(buffer4, Font_7x10, White);

    ssd1306_UpdateScreen();

    RightMotor_SetSpeed(78);   // Motor A chạy thuận 78%
    LeftMotor_SetSpeed(78);  // Motor B chạy nghịch 78%

    Update_Encoder_Speeds(); // Theo dõi omegaA, omegaB trong Live Expressions
    HAL_Delay(FORWARD_TIME);
}


void turnRight(mouse_type *mouse, float direction)
{
    rightTurn();
	mouse->direction = fmodf(mouse->direction + direction, 4.0f);
    if (mouse->direction < 0.0f) mouse->direction += 4.0f;
}

void turnLeft(mouse_type *mouse, float direction)
{
	leftTurn();
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

// Khai báo lấy biến khoảng cách từ main.c sang
extern uint16_t distance1;
extern uint16_t distance2;
extern uint16_t distance3;

// Map các biến vào vị trí cảm biến (hãy đổi lại nếu phần cứng của bạn nối khác)
#define DIST_LEFT   distance1
#define DIST_FRONT  distance2
#define DIST_RIGHT  distance3

// Ngưỡng phát hiện tường (tính bằng mm)
#define WALL_THRESHOLD_MM 120

int readSensor(mouse_type *mouse)
{
    uint8_t wall = 0x00;

    // Kiểm tra xem khoảng cách đọc được có nhỏ hơn ngưỡng tường hay không
    bool wall_front = (DIST_FRONT < WALL_THRESHOLD_MM);
    bool wall_left  = (DIST_LEFT  < WALL_THRESHOLD_MM);
    bool wall_right = (DIST_RIGHT < WALL_THRESHOLD_MM);

    // Dựa vào hướng hiện tại của chuột để set các bit Tường (Bit 3:N, 2:E, 1:S, 0:W)
    int current_dir = (int)(mouse->direction);

    switch (current_dir) {
        case 1: // Chuột đang hướng Bắc (North)
            if (wall_front) wall |= (1 << 3); // North
            if (wall_right) wall |= (1 << 2); // East
            if (wall_left)  wall |= (1 << 0); // West
            break;

        case 2: // Chuột đang hướng Đông (East)
            if (wall_front) wall |= (1 << 2); // East
            if (wall_right) wall |= (1 << 1); // South
            if (wall_left)  wall |= (1 << 3); // North
            break;

        case 3: // Chuột đang hướng Nam (South)
            if (wall_front) wall |= (1 << 1); // South
            if (wall_right) wall |= (1 << 0); // West
            if (wall_left)  wall |= (1 << 2); // East
            break;

        case 4: // Chuột đang hướng Tây (West)
        case 0: // West (xử lý wrap-around)
            if (wall_front) wall |= (1 << 0); // West
            if (wall_right) wall |= (1 << 3); // North
            if (wall_left)  wall |= (1 << 1); // South
            break;
    }

    return wall;
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
            forward(maze, mouse);
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

    while (front < rear) // discover all maze
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

int findOptimalPath(cell_type maze[][MAZE_SIZE], int sx, int sy, int gx, int gy)
{
	#define TURN_PENALTY 10
	#define INT_MAX 5 // giả định
	for (int y = 0; y < MAZE_SIZE; y++)
    {
        for (int x = 0; x < MAZE_SIZE; x++)
        {
            maze[y][x].previous_point = (point_type){-1, -1};
            for(int d = 0; d<4; d++) maze[y][x].cost[d] = UINT16_MAX;
        }
    }

    struct { uint8_t x, y, dir; } queue[MAZE_SIZE * MAZE_SIZE * 4];
    int head = 0, tail = 0;

    for (int d = 0; d < 4; d++) {
        maze[sy][sx].cost[d] = 0;
        queue[tail++] = (typeof(queue[0])){sx, sy, d};
    }

    while (head != tail)
    {
        uint8_t cx = queue[head].x;
        uint8_t cy = queue[head].y;
        uint8_t cd = queue[head].dir;
        head = (head + 1) % (MAZE_SIZE * MAZE_SIZE * 4);

        for (int nextDir = 0; nextDir < 4; nextDir++) {
            // Wall check
            if ((maze[cy][cx].wall >> (3 - nextDir)) & 1) continue;

            int nx = cx + DELTA_X[nextDir];
            int ny = cy + DELTA_Y[nextDir];

            if (isValid(nx, ny) && (maze[ny][nx].known || (nx == gx && ny == gy))) {
                // Logic: If new route is shorter than recorded, update and queue neighbors
                int weight = (nextDir == cd) ? 1 : (1 + TURN_PENALTY);
                int newDist = maze[cy][cx].cost[cd] + weight;

                if (newDist < maze[ny][nx].cost[nextDir]) {
                    maze[ny][nx].cost[nextDir] = newDist;
                    maze[ny][nx].previous_point = (point_type){cx, cy};

                    // Add to queue for propagation
                    queue[tail] = (typeof(queue[0])){ (uint8_t)nx, (uint8_t)ny, (uint8_t)nextDir };
                    tail = (tail + 1) % (MAZE_SIZE * MAZE_SIZE * 4);
                }
            }
        }
    }

    // Return the cheapest cost among all directions at goal
    int minGoalCost = INT_MAX;
    for (int d = 0; d < 4; d++) {
        if (maze[gy][gx].cost[d] < minGoalCost) minGoalCost = maze[gy][gx].cost[d];
    }

    return (minGoalCost >= INT_MAX) ? -1 : minGoalCost;
}


void goToOptimal(cell_type maze[][MAZE_SIZE], mouse_type *mouse, int gx, int gy)
{
    int total_cost = findOptimalPath(maze, mouse->x, mouse->y, gx, gy);

    if (total_cost == -1) return;

    point_type path[MAZE_SIZE * MAZE_SIZE];
    int count = 0;

    int x = gx, y = gy;
    path[count++] = (point_type){x, y};

    while (x != mouse->x || y != mouse->y)
    {
        point_type p = maze[y][x].previous_point;

        if (p.x == -1 || p.y == -1) break;

        path[count++] = p;
        x = p.x;
        y = p.y;
    }

    for (int i = count - 2; i >= 0; i--)
    {
        point_type next = path[i];
        int dx = next.x - mouse->x;
        int dy = next.y - mouse->y;

        int dir = -1; // 0=N, 1=E, 2=S, 3=W
        if      (dx ==  0 && dy == -1) dir = 0; // North
        else if (dx ==  1 && dy ==  0) dir = 1; // East
        else if (dx ==  0 && dy ==  1) dir = 2; // South
        else if (dx == -1 && dy ==  0) dir = 3; // West

        if (dir != -1)
        {
            rotateTo(mouse, (float)dir + 1);
            forward(maze, mouse);

            setWall(maze, mouse);
        }
    }
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
