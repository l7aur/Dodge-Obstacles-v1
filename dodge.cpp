#include "functions.h"
#include <pthread.h>
#include <unistd.h>
#include <string>

car my_car = create_car();
std::vector<obstacle> my_obstacles;    

void * obstacle_generation(void * arg) {
    sleep(2);

    while(true) {
        point p1, p2;
        p1.x = 700;
        p2.x = 800;
        p1.y = rand() % 500 + 1;
        p2.y = p1.y + 100;
        obstacle o = create_obstacle(p1, p2, GREEN);
        my_obstacles.push_back(o);
        sleep(2);
    }
    return NULL;
}

void stop_game(pthread_t thread) {
    pthread_cancel(thread);
    ClearBackground(BACKGROUND_COLOR);
    DrawText("GAME OVER!", 320, 250, 20, WHITE);
    EndDrawing();
    sleep(1);
}

void int_to_char(int x, char * s) {
    if(x == 0) {
        s[0] = '0';
        s[1] = '\0';
        return;
    }
    int i = 0;
    while(x) {
        s[i++] = (char)(x % 10 + '0');
        x /= 10;
    }
    s[i] = '\0';
}

int main() {
    InitWindow(window_width, window_height, "DODGE OBSTACLES");

    SetTargetFPS(80);

    pthread_t obstacle_thread;
    if(pthread_create(&obstacle_thread, NULL, obstacle_generation, NULL) < 0) {
        printf("Error!\n");
        exit(-1);
    }

    score_int = 0;
    char score_char[20] = "SCORE: "; 
    bool stop = false;
    while(!stop) {
        BeginDrawing();
        ClearBackground(BACKGROUND_COLOR);
        
        int_to_char(score_int, score_char + 7);
        DrawText(score_char, 10, 10, 20, WHITE);

        print_obstacles(my_obstacles);
        print_car(my_car);

        stop = handle_collision(my_car, my_obstacles) | WindowShouldClose();

        if(stop)
            stop_game(obstacle_thread);
        else {
            handle_obstacles(my_obstacles);
            handle_car(my_car);
            EndDrawing();
        }
    }

    return 0;
}