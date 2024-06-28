#include "types.h"

car create_car();
void print_car(car c);
void handle_car(car& c);

obstacle create_obstacle();
void print_obstacle(obstacle o);

bool handle_collision(car c, obstacle o);