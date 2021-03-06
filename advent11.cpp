#include "advent.hpp"
#include <string.h>

int distance(int ns, int ew) {
    ns = abs(ns);
    ew = abs(ew);

    int steps = 0;
    steps = ew;
    ns -= ew;
    if (ns > 0) {
        steps += ns/2;
    }

    return steps;
}

int main (void) {
    FILE * f = fopen("input11.txt", "r");

    char direction[10];
    int n;
    
    int ns = 0;
    int ew = 0;
    int max_steps = 0;

    while ((n = fscanf(f, "%[^,\n],", direction)) > 0) {
        DPRINT(std::string(direction));
        if (strcmp(direction, "ne") == 0) {
            ++ns;
            ++ew;
        }
        else if (strcmp(direction, "n") == 0) {
            ns += 2;
        }
        else if (strcmp(direction, "nw") == 0) {
            ++ns;
            --ew;
        }
        else if (strcmp(direction, "se") == 0) {
            --ns;
            ++ew;
        }
        else if (strcmp(direction, "s") == 0) {
            ns -= 2;
        }
        else if (strcmp(direction, "sw") == 0) {
            --ns;
            --ew;
        }

        int d = distance(ns, ew);
        if (d > max_steps)
            max_steps = d;
    }

    DPRINT(ns);
    DPRINT(ew);

    DPRINT(distance(ns, ew));
    DPRINT(max_steps);

    return 0;
}
