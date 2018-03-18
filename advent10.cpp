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

    std::vector<int> dense_hash() {
        std::vector<int> result;

        for (int i = 0; i < 256; i += 16) {
            int curr_value = 0;
            for (int j = 0; j < 16; ++j) {
                curr_value ^= circular_array[i + j];
            }
            result.push_back(curr_value);
        }

        return result;
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
    std::vector<int> V;

    while ((n = getc(f)) != EOF && n != '\n') {
        DPRINT(n);
        V.push_back(n);
    }

    V.push_back(17);
    V.push_back(31);
    V.push_back(73);
    V.push_back(47);
    V.push_back(23);

    DPRINT(V);

    for (int i = 0; i < 64; ++i) {
        for (auto p = V.begin(); p != V.end(); ++p) {
            R.knot(*p);
        }
    }

    std::vector<int> dense_hash = R.dense_hash();

    DPRINT(dense_hash);

    for (std::vector<int>::const_iterator ci = dense_hash.begin(); ci != dense_hash.end(); ++ci) {
        printf("%x", *ci);
    }
    printf("\n");

    return 0;
}
