#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <vector>
#include <utility>
#include <iostream>
#include "raylib.h"

#define BACKGROUND_COLOR BLACK
#define FOREGROUND_COLOR RED
#define PIXELS_PER_MOVE 5

typedef struct __point {
    int x;
    int y;
    __point() {
        x = 0;
        y = 0;
    }
    __point(int a, int b) {
        x = a;
        y = b;
    }
} point;

typedef struct __object {
    std::vector<point> points;
    __object(std::vector<point> p) {
        points = p;
    }
} object;

typedef struct __car {
    int max_x, max_y;
    int min_x, min_y;
    float radius;
    point center1, center2;
    std::vector<std::pair<point, point>> rectangles;
    Color car_color;
    __car(int x_m, int x_M, int y_m, int y_M,
        float r, point w1, point w2, std::vector<std::pair<point, point>> rects, Color clr)
        : max_x(x_M), max_y(y_M), min_x(x_m), min_y(y_m), 
        radius(r), center1(w1), center2(w2), rectangles(rects), car_color(clr) {}
} car;

typedef struct __obstacle {
    int x_min, x_max, y_min, y_max;
    std::vector<std::pair<point, point>> rectangles;
    Color obstacle_color;
    __obstacle(int x_m, int x_M, int y_m, int y_M,
        std::vector<std::pair<point, point>> rects, Color clr) :
        x_min(x_m), x_max(x_M), y_min(y_m), y_max(y_M),
        rectangles(rects), obstacle_color(clr) {}
} obstacle;

const int window_height = 600;
const int window_width = 800;
extern int score_int;

car create_car();
void print_car(car c);
void handle_car(car& c);
bool handle_obstacle(obstacle& o);
void handle_obstacles(std::vector<obstacle>& o);

obstacle create_obstacle();
obstacle create_obstacle(point x, point y, Color c);
void print_obstacle(obstacle o);
void print_obstacles(std::vector<obstacle> o);

bool handle_collision(car c, std::vector<obstacle> o);

#endif