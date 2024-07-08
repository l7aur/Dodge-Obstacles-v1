#include "functions.h"

int main() {
    InitWindow(window_width, window_height, "DODGE OBSTACLES");

    // point circle_center = {200, 200};
    // float circle_radius = 50.0f;  

    SetTargetFPS(120);
    car my_car = create_car();
    obstacle my_obstacle = create_obstacle();
    bool stop = false;
    while(!WindowShouldClose() && !stop) {
        BeginDrawing();
        ClearBackground(BACKGROUND_COLOR);
        
        print_obstacle(my_obstacle);

        print_car(my_car);
        handle_car(my_car);

        stop = handle_collision(my_car, my_obstacle);

        EndDrawing();
    }
    if(stop){
        ;
    }

    return 0;
}