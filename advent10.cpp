#include "advent.hpp"
#include <stdio.h>

// Implement a wacky cryptographic function

struct Rope {
    int circular_array[256];
    int skip = 0;
    int position = 0;

    Rope() {
        int i;
        for (i = 0; i < 256; ++i) {
            circular_array[i] = i;
        }
    }

    void knot(int length) {
        // Reverse the numbers in range [position, position + length - 1]
        int end = position + length - 1; // [position, end]
        int curr_position = position;
        while (curr_position < end) {
            int P = curr_position % 256;
            int E = end % 256;

            std::swap(circular_array[P], circular_array[E]);
            ++curr_position;
            --end;
        }

        // Advance position
        position = position + length + skip;
        position %= 256;

        // Increment skip
        skip++;
        skip %= 256;
    }
};

std::ostream & operator<< (std::ostream & out, Rope const & print_me) {
    out << "Position: " << print_me.position << "\n";
    out << "Skip: " << print_me.skip << "\n";
    std::vector<int> V(print_me.circular_array, print_me.circular_array + 256);
    out << V;

    return out;
}

int main(void) {
    FILE * f = fopen("input10.txt", "r");
    Rope R;

    int n;
    int length;
    while ((n = fscanf(f, "%d,", &length)) > 0) {
        R.knot(length);

        std::cout << length << "\n";
        std::cout << R;
    }

    printf ("%d\n", R.circular_array[0] * R.circular_array[1]);

    return 0;
}
