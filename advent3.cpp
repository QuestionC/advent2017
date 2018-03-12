/**
  Make a spiral like this...
  5 4 3
  6 1 2
  7 8 9

  Count the Manhattan distance from input value to 1
  */

#include <fstream>
#include <iostream>
#include <math.h>

#define DPRINT(X) std::cerr << #X ": " << X << '\n';

int get_input() {
    std::ifstream file("input3.txt");

    int result;
    file >> result;
    return result;
}

// Transform a number to its coordinate.
std::pair<int, int> get_coord(int val) {
    // The first ring goes to 1**2
    // The second ring goes to 3**2
    // The third ring goes to 5**2
    // &ct

    if (val == 1) {
        return std::make_pair(0, 0);
    }

    // The number's not that big and I want to avoid floating point errors
    int ringnum = 1;
    int old_ring_max = 0;
    int new_ring_max;

    for (ringnum = 0; ; ++ringnum) {
        int box_length = ringnum * 2 + 1;
        old_ring_max = new_ring_max;
        new_ring_max = box_length * box_length;
        if (new_ring_max > val) {
            break;
        }
    }

    // Now old_ring_max < val < new_ring_max
    // The values in (old_ring_max, new_ring_max] form a box.

    // The box has 4 sides, each of length (new_ring_max - old_ring_max)/4
    // The right side starts at (ringnum, -(ring_dist-1)) and goes north
    // The top side starts at (ringnum-1, ring_dist) and goes west
    // The left side starts at (-ringnum, ring_dist-1) and goes south
    // The south side starts at (-(ringnum -1), -ring_dist) and goes east
    // Note that (N+2)**2 - N**2 = 4N + 4 always divides 4

    int side_len = ringnum * 2;
    int side = (val - old_ring_max) / side_len;
    int side_remainder = (val - old_ring_max) % side_len;

    int x, y;
    switch(side) {
        case 0: // E
            x = ringnum;
            y = -ringnum + side_remainder;
            break;
        case 1: // N
            x = ringnum - side_remainder;
            y = ringnum;
            break;
        case 2: // W
            x = -ringnum;
            y = ringnum - side_remainder;
            break;
        case 3: // S
            x = -ringnum + side_remainder;
            y = -ringnum;
            break;
    }

    return std::make_pair(x, y);
}

std::ostream & operator<< (std::ostream & out, std::pair<int, int> P) {
    out << "(" << P.first << ", " << P.second << ")\n";
    return out;
}

int main (void) {
    int val = get_input();

    auto coord = get_coord(val);

    DPRINT(coord);

    // Manhattan
    std::cout << abs(coord.first) + abs(coord.second) << '\n';

    return 0;
}
