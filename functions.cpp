#include "functions.h"

car create_car() {
    //hardcoded coordinates
    point center1(150, 230);
    point center2(250, 230);
    float radius = 20.0f;
    Color car_color = {255, 0, 0, 255};
    std::vector<std::pair<point, point>> rectangles;
    rectangles.push_back({point(100, 180), point(300, 230)});
    rectangles.push_back({point(160, 140), point(240, 180)});
    return car(100, 300, 140, center1.y + radius,
        radius, center1, center2, rectangles, car_color);
}

void print_car(car c) {
    DrawCircle(c.center1.x, c.center1.y, c.radius, c.car_color);
    DrawCircle(c.center2.x, c.center2.y, c.radius, c.car_color);
    for(auto i: c.rectangles)
        DrawRectangle(i.first.x, i.first.y, i.second.x - i.first.x, i.second.y - i.first.y, c.car_color);
}

void handle_car(car& c) {
    int Y = 0, X = 0;
    if(IsKeyDown(KEY_W) && c.min_y - PIXELS_PER_MOVE >= 0)
        Y = -PIXELS_PER_MOVE;
    if(IsKeyDown(KEY_S) && c.max_y + PIXELS_PER_MOVE <= window_height)
        Y = PIXELS_PER_MOVE;
    if(IsKeyDown(KEY_A) && c.min_x - PIXELS_PER_MOVE >= 0)
        X = -PIXELS_PER_MOVE;
    if(IsKeyDown(KEY_D) && c.max_x + PIXELS_PER_MOVE <= window_width)
        X = PIXELS_PER_MOVE;
    c.center1.x += X, c.center2.x += X;
    c.center1.y += Y, c.center2.y += Y;
    c.max_x += X, c.min_x += X;
    c.max_y += Y, c.min_y += Y;
    for(auto& i: c.rectangles) {
        i.first.x += X, i.first.y += Y;
        i.second.x += X, i.second.y += Y;
    }
    if(X != 0 || Y != 0)
        std::cout << "handled movement\n" << X << ' ' << Y << '\n';
}

obstacle create_obstacle() {
    std::vector<std::pair<point, point>> rectangles;
    Color color = {255, 255, 255, 255};

    rectangles.push_back({point(400, 400), point(500, 500)});

    return obstacle(400, 500, 400, 500, rectangles, color);
}

void print_obstacle(obstacle o) {
    for(auto i: o.rectangles)
        DrawRectangle(i.first.x, i.first.y, i.second.x - i.first.x, i.second.y - i.first.y, o.obstacle_color);
}

bool handle_collision(car c, obstacle o) {
    //check circles
    
    //check rectangles
    //use line intersection for your mental health
}