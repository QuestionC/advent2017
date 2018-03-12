/**
  The spiral now accumulates adjacent ring values like
    
    5  4  2
   10  1  1
   11 23 25

   1 = 1
   2 = 1 + 1
   4 = 1 + 1 + 2
   5 = 4 + 1
   10 = 5 + 4 + 1
   11 = 10 + 1

   Output the first value larger than puzzle input.
  */

#include <fstream>
#include <iostream>
#include <math.h>
#include <vector>

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

std::ostream & operator<< (std::ostream & out, std::vector<int> V) {
    out << "[";

    if (V.empty()) {
        return out << "]\n";
    }
    
    std::vector<int>::const_iterator ci = V.begin();
    out << *ci++;

    for (; ci != V.end(); ++ci) {
        out << ", " << *ci;
    }

    out << "]\n";

    return out;
}

// This is called when we add an element to the ring vector
// Abuse/enforce that each new element is the sum of at most 4 old elements.
void add2Vec(std::vector<int> & V, int elem1 = 0, int elem2 = 0, int elem3 = 0, int elem4 = 0) {
    int new_val = elem1 + elem2 + elem3 + elem4;

    std::cerr << elem1 << " + " << elem2 << " + " << elem3 << " + " << elem4 << " = " << new_val << "\n";

    V.push_back(new_val);
}

void generate_ring(std::vector<int> const & inner_ring, std::vector<int> & outer_ring) {
    // Special case first ring
    if (inner_ring.size() == 1) {
        int vals[] = {1, 2, 4, 5, 10, 11, 23, 25};
        std::vector<int> new_ring(vals, vals + 8);
        outer_ring = new_ring;
        return;
    }

    // Normal case
    std::vector<int> new_ring;

    /**
      I use this notation for side lens

      3 2 1 3
      1     2
      2     1
      3 1 2 3

      So when I say side_len 3, that would conventionally mean side len 4
    */


    int inner_side_len = inner_ring.size() / 4;
    int outer_side_len = inner_side_len + 2;

    DPRINT(inner_side_len);
    DPRINT(outer_side_len);

    // Let's build each side one at a time

    // EAST
    int inner_offset = 0;
    
    // Special case first element
    add2Vec (
            new_ring,
            inner_ring.back(),
            inner_ring[inner_offset]
            );

    // Second element is special too because it touches inner_ring.back()
    add2Vec(
            new_ring,
            inner_ring.back(),
            inner_ring[inner_offset],
            inner_ring[inner_offset + 1],
            new_ring.back()
           );

    // Each successive element until the last 2 touches 3 inner elements and the most recent outer element
    while (new_ring.size() < outer_side_len - 2) {
        add2Vec(new_ring,
                inner_ring[inner_offset],
                inner_ring[inner_offset + 1],
                inner_ring[inner_offset + 2],
                new_ring.back()
               );
        ++inner_offset;
    }

    // Next-to last only touches 2 inner ring elements
    add2Vec(new_ring,
            inner_ring[inner_offset],
            inner_ring[inner_offset + 1],
            new_ring.back()
           );
    ++inner_offset;

    // Last element is corner, only touches one inner element
    add2Vec(new_ring,
            inner_ring[inner_offset],
            new_ring.back()
           );

    // NORTH

    // Special case first element.  It touches 2 inner elements and one new element around the corner
    add2Vec(new_ring,
            new_ring[new_ring.size() - 2],
            inner_ring[inner_offset],
            inner_ring[inner_offset + 1],
            new_ring.back()
           );

    // Wall
    while (new_ring.size() < outer_side_len * 2 - 2) {
        add2Vec(new_ring,
                inner_ring[inner_offset],
                inner_ring[inner_offset + 1],
                inner_ring[inner_offset + 2],
                new_ring.back()
               );
        ++inner_offset;
    }
    
    // Next-to last only touches 2 inner ring elements
    add2Vec(new_ring,
            inner_ring[inner_offset],
            inner_ring[inner_offset + 1],
            new_ring.back()
           );
    ++inner_offset;

    // Last element is corner, only touches one inner element
    add2Vec(new_ring,
            inner_ring[inner_offset],
            new_ring.back()
           );

    // WEST
    // Special case first element.  It touches 2 inner elements and 2 outer elements
    add2Vec(new_ring,
            new_ring[new_ring.size() - 2],
            inner_ring[inner_offset],
            inner_ring[inner_offset + 1],
            new_ring.back()
           );

    // Wall
    while (new_ring.size() < outer_side_len * 3 - 2) {
        add2Vec(new_ring,
                inner_ring[inner_offset],
                inner_ring[inner_offset + 1],
                inner_ring[inner_offset + 2],
                new_ring.back()
               );
        ++inner_offset;
    }
    
    // Next-to last only touches 2 inner ring elements
    add2Vec(new_ring,
            inner_ring[inner_offset],
            inner_ring[inner_offset + 1],
            new_ring.back()
           );
    ++inner_offset;

    // Last element is corner, only touches one inner element
    add2Vec(new_ring,
            inner_ring[inner_offset],
            new_ring.back()
           );

    // SOUTH
    // Special case first element.  It touches 2 inner elements and 2 outer elements
    add2Vec(new_ring,
            new_ring[new_ring.size() - 2],
            inner_ring[inner_offset],
            inner_ring[inner_offset + 1],
            new_ring.back()
           );

    // Wall
    while (new_ring.size() < outer_side_len * 4 - 2) {
        add2Vec(new_ring,
                inner_ring[inner_offset],
                inner_ring[inner_offset + 1],
                inner_ring[inner_offset + 2],
                new_ring.back()
               );
        ++inner_offset;
    }

    // Next-to last SOUTH touches 2 inner ring elements and first outer ring element
    add2Vec(new_ring,
            inner_ring[inner_offset],
            inner_ring[inner_offset + 1],
            new_ring[0],
            new_ring.back()
           );
    ++inner_offset;

    // SOUTH Last element is corner, only touches one inner element and first outer ring element
    add2Vec(new_ring,
            inner_ring[inner_offset],
            new_ring[0],
            new_ring.back()
           );

    outer_ring = new_ring;

    return;
}

int main (void) {
    int val = get_input();

    std::vector<int> inner_ring, outer_ring;
    inner_ring.push_back(1);
    generate_ring (inner_ring, outer_ring);

    while (val > outer_ring.back()) {
        std::swap(inner_ring, outer_ring);
        generate_ring(inner_ring, outer_ring);
    }

    std::vector<int>::const_iterator ci;
    for (ci = outer_ring.begin(); val > *ci; ++ci);

    std::cout << *ci;

    return 0;
}
