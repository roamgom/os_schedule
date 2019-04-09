#include <stdio.h>

int swap(int *first, int *second);

int main() {
    int cola = 10;
    int cider = 30;
    
    swap(&cola, &cider);

    printf("cola: %d\n", cola);
    printf("cider: %d", cider);
    
}

int swap(int *first, int *second) {
    int empty_glass;
    // *first ^= *second;
    // *second ^= *first; 
    // *first ^= *second;
   empty_glass = *first;
   *first = *second;
   *second = empty_glass;
}