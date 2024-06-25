#include "raylib.h"

struct point{
    int x;
    int y;
};

int main() {
    const int window_height = 600, window_width = 800;
    InitWindow(window_width, window_height, "AXE GAME");

    struct point circle_center = {200, 200};
    float circle_radius = 50.0f;  

    SetTargetFPS(120);
    while(!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(GREEN);
        DrawCircle(circle_center.x, circle_center.y, circle_radius, BLUE);

        if(IsKeyDown(KEY_S) && circle_center.y < window_height - circle_radius) {
            circle_center.y++;
        }
        if(IsKeyDown(KEY_W) && circle_center.y > circle_radius) {
            circle_center.y--;
        }
        if(IsKeyDown(KEY_A) && circle_center.x > circle_radius) {
            circle_center.x--;
        }
        if(IsKeyDown(KEY_D) && circle_center.x < window_width - circle_radius) {
            circle_center.x++;
        }


        EndDrawing();
    }

    return 0;
}