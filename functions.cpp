#include "functions.h"
#include <cmath>

int score_int;

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
    //DEBUG
    // if(X != 0 || Y != 0)
    //     std::cout << "handled movement\n" << X << ' ' << Y << '\n';
}

obstacle create_obstacle() {
    std::vector<std::pair<point, point>> rectangles;
    Color color = {255, 255, 255, 255};

    rectangles.push_back({point(400, 400), point(500, 500)});

    return obstacle(400, 500, 400, 500, rectangles, color);
}

obstacle create_obstacle(point x, point y, Color c) {
    std::vector<std::pair<point, point>> rectangles;
    rectangles.push_back({x, y});

    return obstacle(x.x, y.x, x.y, y.y, rectangles, c);
}

void print_obstacle(obstacle o) {
    for(auto i: o.rectangles)
        DrawRectangle(i.first.x, i.first.y, i.second.x - i.first.x, i.second.y - i.first.y, o.obstacle_color);
}

void print_obstacles(std::vector<obstacle> o) {
    for(auto i: o)
        print_obstacle(i);
}

bool handle_obstacle(obstacle& o) {
    bool removable = true;
    for(auto& i: o.rectangles) {
        i.first.x -= PIXELS_PER_MOVE;
        i.second.x -= PIXELS_PER_MOVE;
        if(i.second.x > 0)
            removable = false;
    }
    return removable;
}


void handle_obstacles(std::vector<obstacle>& o) {
    for(std::vector<obstacle>::iterator i = o.begin(); i != o.end();) {
        bool to_be_removed = handle_obstacle(*i);
        if(to_be_removed)
            o.erase(i), score_int++;
        else
            i++;       
    }
}

int how_many_intersection_points_between_line_and_circle(point center, float r, point p0, point p1) {
    int alphap = p1.x - p0.x;
    int betap = p1.y - p0.y;
    float a = alphap * alphap + betap * betap;
    float b = 2 * (alphap * (p0.x - center.x) + betap * (p0.y - center.y));
    float c = p0.x * p0.x + center.x * center.x - 2 * p0.x * center.x +
              p0.y * p0.y + center.y * center.y - 2 * p0.y * center.y +
              - r * r;  
    float delta = b * b - 4 * a * c;
    if(delta < 0)
        return 0;
    float t1 = (-b + std::sqrt(delta)) / (2 * a);
    float t2 = (-b - std::sqrt(delta)) / (2 * a);
    int var = 0;
    if(0 <= t1 && t1 <= 1)
        var++;
    if(0 <= t2 && t2 <= 1 && t1 != t2)
        var++;
    return var;
}

bool do_lines_intersect(point p1, point p2, point p3, point p4) {
    if(p1.x > p2.x)
        std::swap(p1, p2);
    if(p1.y > p2.y)
        std::swap(p1, p2);
    if(p3.x > p4.x)
        std::swap(p3, p4);
    if(p3.y > p4.y)
        std::swap(p3, p4);
    int denom = (p1.x - p2.x) * (p3.y - p4.y) - (p1.y - p2.y) * (p3.x - p4.x);
    if(denom == 0) return false;
 
    int numx = (p1.x * p2.y - p1.y * p2.x) * (p3.x - p4.x) - (p1.x - p2.x) * (p3.x * p4.y - p3.y * p4.x);
    int numy = (p1.x * p2.y - p1.y * p2.x) * (p3.y - p4.y) - (p1.y - p2.y) * (p3.x * p4.y - p3.y * p4.x);

    float x = numx / denom;
    float y = numy / denom;

    if(p1.x <= x && x <= p2.x && p3.x <= x && x <= p4.x
    && p1.y <= y && y <= p2.y && p3.y <= y && y <= p4.y)
        return true;
    return false;
}

bool handle_collision(car c, std::vector<obstacle> o) {
    if(o.empty())
        return false;
    for(auto obj: o) {
        for(auto r: obj.rectangles) {
            //check rectangles
            for(auto car_element: c.rectangles) {
                if(do_lines_intersect(car_element.first, point(car_element.second.x, car_element.first.y), r.first, point(r.second.x, r.first.y))
                || do_lines_intersect(car_element.first, point(car_element.second.x, car_element.first.y), point(r.second.x, r.first.y), r.second)
                || do_lines_intersect(car_element.first, point(car_element.second.x, car_element.first.y), r.second, point(r.first.x, r.second.y))
                || do_lines_intersect(car_element.first, point(car_element.second.x, car_element.first.y), point(r.first.x, r.second.y), r.first)

                || do_lines_intersect(point(car_element.second.x, car_element.first.y), car_element.second, r.first, point(r.second.x, r.first.y))
                || do_lines_intersect(point(car_element.second.x, car_element.first.y), car_element.second, point(r.second.x, r.first.y), r.second)
                || do_lines_intersect(point(car_element.second.x, car_element.first.y), car_element.second, r.second, point(r.first.x, r.second.y))
                || do_lines_intersect(point(car_element.second.x, car_element.first.y), car_element.second, point(r.first.x, r.second.y), r.first)

                || do_lines_intersect(car_element.second, point(car_element.first.x, car_element.second.y), r.first, point(r.second.x, r.first.y))
                || do_lines_intersect(car_element.second, point(car_element.first.x, car_element.second.y), point(r.second.x, r.first.y), r.second)
                || do_lines_intersect(car_element.second, point(car_element.first.x, car_element.second.y), r.second, point(r.first.x, r.second.y))
                || do_lines_intersect(car_element.second, point(car_element.first.x, car_element.second.y), point(r.first.x, r.second.y), r.first)

                || do_lines_intersect(point(car_element.first.x, car_element.second.y), car_element.first, r.first, point(r.second.x, r.first.y))
                || do_lines_intersect(point(car_element.first.x, car_element.second.y), car_element.first, point(r.second.x, r.first.y), r.second)
                || do_lines_intersect(point(car_element.first.x, car_element.second.y), car_element.first, r.second, point(r.first.x, r.second.y))
                || do_lines_intersect(point(car_element.first.x, car_element.second.y), car_element.first, point(r.first.x, r.second.y), r.first)) {
                    //the rectangles intersect the object
                    return true;
                }
            }

            //check circles
            if(how_many_intersection_points_between_line_and_circle(c.center1, c.radius, r.first, point(r.second.x, r.first.y)) > 0
            || how_many_intersection_points_between_line_and_circle(c.center1, c.radius, point(r.second.x, r.first.y), r.second) > 0
            || how_many_intersection_points_between_line_and_circle(c.center1, c.radius, r.second, point(r.first.x, r.second.y)) > 0
            || how_many_intersection_points_between_line_and_circle(c.center1, c.radius, point(r.first.x, r.second.y), r.first) > 0) {
                //the wheels intersect the object
                return true;
            }
        }
    }
    return false;
}