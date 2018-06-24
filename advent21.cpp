#include "advent.hpp"
#include <stdlib.h>
#include <string.h>
#include <exception>

// Helper class to represent a square set of bits
class Image {
    public:
    std::vector<bool> data;
    int size;

    Image();
    Image(char const * S, int size);

    void resize(int size);
    bool at(int x, int y) const;
    char char_at(int x, int y) const;
    std::string char_at(int x0, int y0, int x1, int y1) const;
    void blit(int x0, int y0, int x1, int y1, char const * S);

    int num_lights() const;
};

Image::Image() {
    size = 0;
}

Image::Image(char const * S, int _size) {
    size = _size;
    data.resize(size * size);
    blit(0, 0, size, size, S);
}

void Image::resize(int _size) {
    size = _size;
    data.resize(size * size);
}

bool Image::at(int x, int y) const {
    return data[y * size + x];
}

char Image::char_at(int x, int y) const {
    return data[y * size + x] ? '#':'.';
}

std::string Image::char_at(int x0, int y0, int x1, int y1) const {
    std::string output;
    for (auto y = y0; y < y1; ++y) {
        for (auto x = x0; x < x1; ++x) {
            output += char_at(x, y);
        }
    }
    return output;
}

void Image::blit(int x0, int y0, int x1, int y1, char const * S) {
    auto curr_char = S;
    for (auto y = y0; y < y1; ++y) {
        for (auto x = x0; x < x1; ++x) {
            if (*curr_char == '#')
                data[y * size + x] = true;
            else if (*curr_char == '.')
                data[y * size + x] = false;
            else
                throw std::runtime_error("Invalid char in Image::blit");
            ++curr_char;
        }
    }
}

int Image::num_lights() const {
    return std::count(data.begin(), data.end(), true);
} 
void print (FILE * f, Image const & print_me) {
    printf("print(f, Image)\n");
    putc('\n', f);

    auto ci = print_me.data.cbegin();
    for (int i = 0; i < print_me.size; ++i) {
        for (int j = 0; j < print_me.size; ++j) {
            putc(*ci++?'#':'.', f);
        }
        putc('\n', f);
    }
}

class Pattern {
    public:
        size_t size, output_size;
        char pattern[5][5], output_pattern[5][5];
        char output_flat[25];
        int m_num_lights;

        Pattern(char const * S);


        int num_lights() const;
        void print(FILE * f) const;
        void print_output(FILE * f) const;

        bool ismatch(char const * S);
};

Pattern::Pattern(char const * S) {
    char input[25];
    char output[25];

    sscanf(S, "%s => %s", input, output);

    char * token;
    char * saveptr;
    int i;

    // Parse the input pattern
    for (
            token = strtok_r(input, "/", &saveptr), i = 0; 
            token != NULL; 
            token = strtok_r(NULL, "/", &saveptr), ++i) 
    {
        strcpy(pattern[i], token);
    }
    
    size = i;

    // Parse the output pattern
    for (
            token = strtok_r(output, "/", &saveptr), i = 0; 
            token != NULL; 
            token = strtok_r(NULL, "/", &saveptr), ++i) 
    {
        strcpy(output_pattern[i], token);
    }

    output_size = i;

    // Populate output_flat
    for (int i = 0; i < output_size; ++i) {
        strcpy(output_flat + i * output_size, output_pattern[i]);
    }

    m_num_lights = 0; 
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (pattern[i][j] == '#') {
                ++m_num_lights;
            }
        }
    }

}

void Pattern::print(FILE * f) const {
    for (int i = 0; i < size; ++i) {
        printf("\n%s", pattern[i]);
    }
}

void Pattern::print_output(FILE * f) const {
    for (int i = 0; i < output_size; ++i) {
        printf("\n%s", output_pattern[i]);
    }
}

void print(FILE * f, Pattern const & P) {
    P.print(f);
}

int Pattern::num_lights(void) const {
    return m_num_lights;
}

// TRUE if we can flip or rotate S to match this pattern's input
// S is just the pattern without newlines, so it's gonna be 4 or 9 characters long
bool Pattern::ismatch(char const * S) {
    if (strlen(S) != size * size) {
        return false;
    }

    char const * curr;

fail0: 
    curr = S;
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (pattern[i][j] != *curr)
                goto fail1;
            ++curr;
        }
    }
    return true;

fail1:
    curr = S;
    for (int i = size - 1; i >= 0; --i) {
        for (int j = 0; j < size; ++j) {
            if (pattern[i][j] != *curr)
                goto fail2;
            ++curr;
        }
    }
    return true;

fail2:
    curr = S;
    for (int i = 0; i < size; ++i) {
        for (int j = size - 1; j >= 0; --j) {
            if (pattern[i][j] != *curr)
                goto fail3;
            ++curr;
        }
    }
    return true;

fail3:
    curr = S;
    for (int i = size - 1; i >= 0; --i) {
        for (int j = size - 1; j >= 0; --j) {
            if (pattern[i][j] != *curr)
                goto fail4;
            ++curr;
        }
    }
    return true;

fail4: 
    curr = S;
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (pattern[j][i] != *curr)
                goto fail5;
            ++curr;
        }
    }
    return true;

fail5:
    curr = S;
    for (int i = size - 1; i >= 0; --i) {
        for (int j = 0; j < size; ++j) {
            if (pattern[j][i] != *curr)
                goto fail6;
            ++curr;
        }
    }
    return true;

fail6:
    curr = S;
    for (int i = 0; i < size; ++i) {
        for (int j = size - 1; j >= 0; --j) {
            if (pattern[j][i] != *curr)
                goto fail7;
            ++curr;
        }
    }
    return true;

fail7:
    curr = S;
    for (int i = size - 1; i >= 0; --i) {
        for (int j = size - 1; j >= 0; --j) {
            if (pattern[j][i] != *curr)
                goto fail8;
            ++curr;
        }
    }
    return true;

fail8:
    return false;
}

Image iterate_image(Image const & input, std::vector<Pattern> const & patterns) {
    Image output;

    int input_block_size;
    int output_block_size;
    if (input.size % 2 == 0) {
        input_block_size = 2;
        output_block_size = 3;
    } else if (input.size % 3 == 0) {
        input_block_size = 3;
        output_block_size = 4;
    } else {
        throw std::runtime_error("iterate_image with non-divisable size");
    }
        
    int num_blocks = input.size / input_block_size;

    output.resize(num_blocks * output_block_size);

    for (int y = 0; y < num_blocks; ++y) {
        for (int x = 0; x < num_blocks; ++x) {
            std::string input_block = input.char_at(x * input_block_size, y * input_block_size, (x + 1) * input_block_size, (y + 1) * input_block_size);

            auto ci = std::find_if(patterns.cbegin(), patterns.cend(), [& input_block](auto p) { return p.ismatch(input_block.c_str()); } );

            output.blit(x * output_block_size, y * output_block_size, (x + 1) * output_block_size, (y + 1) * output_block_size, ci->output_flat); 
        }
    }

    return output;
}

int main (void) {
    FILE * f = fopen("input21.txt", "r");
    char * buff = (char *) malloc(255);
    size_t line_length = 255;

    std::vector<Pattern> patterns;

    while (getline(&buff, &line_length, f) > 0) {
        patterns.push_back(Pattern(buff));
    }

    free(buff);

    patterns[26].print(stdout);
    printf ("\nnum_lights: %d\n", patterns[26].num_lights());
    patterns[26].print_output(stdout);
    printf("\n");

    printf ("output_flat: %s\n", patterns[26].output_flat);

    /*
    printf (patterns[26].ismatch("..###....")?"True":"False");
    printf (patterns[26].ismatch("..#.#..#.")?"True":"False");
    printf (patterns[26].ismatch("..#.#.#..")?"True":"False");
    */

    Image I(".#...####", 3);
   
    for (int i = 0; i < 6; ++i) {
        printf("Iteration %d\n", i);
        print(stdout, I);
        printf("%d Lights\n", I.num_lights());
        printf("-----\n");

        I = iterate_image(I, patterns);
    }

    return 0;
}
