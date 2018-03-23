#ifndef advent14_HPP
#define advent14_HPP

#include <vector>

// Depends on advent10.  Looks like we just count the bits in the hash

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

    static void Hash(char const * key, char result[16]) {
        Rope R;

        char salt[] = {17, 31, 73, 47, 23};

        for (int i = 0; i < 64; ++i) {
            for (char const * ch = key; *ch != '\0' && *ch != '\n'; ++ch) {
                R.knot(*ch);
            }

            for (int j = 0; j < sizeof(salt); ++j) {
                R.knot(salt[j]);
            }
        }

        std::vector<int> dense_hash = R.dense_hash();

        for (int i = 0; i < 16; ++i) {
            result[i] = dense_hash[i];
        }

        return;
    }
};

// 128x128 grid of bits.
struct BinaryGrid {
    char buff[128][16];
    const int height = 128;
    const int width = 128;

    bool at(int x, int y) const {
        if (x < 0 || y < 0 || x >= width || y >= width)
            return false;

        unsigned offset = x / 8;
        unsigned char mask = u'\x80' >> (x % 8);

        return buff[y][offset] & mask;
    }

    BinaryGrid(char B[128][16]) {
        memcpy(buff, B, 128 * 16);
    }

    BinaryGrid() {
    }
};

inline void print(FILE * f, BinaryGrid const & print_me) {
    for (int y = 0; y < print_me.height; ++y) {
        for (int x = 0; x < print_me.width; ++x) {
            if (print_me.at(x, y)) {
                fputc('#', f);
            } else {
                fputc('.', f);
            }
        }
        fputc('\n', f);
    }
}

#endif // advent14_HPP
