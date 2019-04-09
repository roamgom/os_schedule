#include <stdio.h>
#include <stdlib.h>

typedef struct _Point {
    // X and Y axis
    int x;
    int y;
}Point;

Point *getsize();

int main(){
    Point *get_point;

    get_point = getsize();

    printf("X: %d | Y: %d", get_point->x, get_point->y);
    return 0;
}

Point *getsize(){
    Point *point;
    Point first;
    point = &first;
    //Point *point = (Point*) malloc(sizeof(Point));

    point->x = 10;
    point->y = 10;

    return point;
}
