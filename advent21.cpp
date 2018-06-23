#include "advent.hpp"
#include <stdlib.h>
#include <string.h>
#include <exception>

class Pattern {
    public:
        size_t size, output_size;
        char pattern[5][5], output_pattern[5][5];
        int m_num_lights;
        std::vector<std::string> output_substrings;

        Pattern(char const * S);


        int num_lights() const;
        void print(FILE * f) const;
        void print_output(FILE * f) const;
        char const * c_str() const;
        char const * c_output() const;


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

    m_num_lights = 0; 
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (pattern[i][j] == '#') {
                ++m_num_lights;
            }
        }
    }

    // Build the output substrings
    if (size == 2) {
        std::string buff;
        buff += output_pattern[0];
        buff += output_pattern[1];
        buff += output_pattern[2];
        output_substrings.push_back(buff);
    } else if (size == 3) {
        std::string buff;

        buff.append(output_pattern[0],2);
        buff.append(output_pattern[1],2);
        output_substrings.push_back(buff);

        buff.clear();
        buff.append(output_pattern[0]+2, 2);
        buff.append(output_pattern[1]+2, 2);
        output_substrings.push_back(buff);

        buff.clear();
        buff.append(output_pattern[2],2);
        buff.append(output_pattern[3],2);
        output_substrings.push_back(buff);

        buff.clear();
        buff.append(output_pattern[2]+2, 2);
        buff.append(output_pattern[3]+2, 2);
        output_substrings.push_back(buff);
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

int count_lights(std::vector<Pattern> const & patterns, std::vector<int> const & ids) {
    int total = 0;
    for (auto i = ids.begin(); i != ids.end(); ++i) {
        total += patterns[*i].num_lights();
    }
    return total;
}

std::vector<int> iterate_patterns(std::map<int, std::vector<int> > pattern2output, std::vector<int> const & ids) {
    std::vector<int> result;
    for (auto i = ids.begin(); i != ids.end(); ++i) {
        for (auto j = pattern2output[*i].begin(); j != pattern2output[*i].end(); ++j) {
            result.push_back(*j);
        }
    }
    return result;
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

    print(stdout, patterns[26].output_substrings);
    printf("\n");

    /*
    printf (patterns[26].ismatch("..###....")?"True":"False");
    printf (patterns[26].ismatch("..#.#..#.")?"True":"False");
    printf (patterns[26].ismatch("..#.#.#..")?"True":"False");
    */

    std::map<int, std::vector<int> > pattern2output;

    for (int i = 0; i < patterns.size(); ++i) {
        std::vector<int> curr_pattern_output;
        for (auto j = patterns[i].output_substrings.begin(); j != patterns[i].output_substrings.end(); ++j) {
            int k;
            for (k = 0; k < patterns.size(); ++k) {
                if (patterns[k].ismatch(j->c_str()))
                    break;
            }
            if (k == patterns.size()) {
                throw std::runtime_error("Found invalid subpattern");
            }
            curr_pattern_output.push_back(k);
        }
        pattern2output[i]=curr_pattern_output;
    }

    print(stdout, pattern2output[26]);
    printf("\n");

    char const * start_pattern = ".#...####";
    char start_pattern_id;
    for (start_pattern_id = 0; start_pattern_id < patterns.size(); ++start_pattern_id) {
        if (patterns[start_pattern_id].ismatch(start_pattern))
            break;
    }
    if (start_pattern_id == patterns.size()) {
        throw std::runtime_error("Couldn't find a start pattern");
    }

    std::vector<int> curr_patterns;
    curr_patterns.push_back(start_pattern_id);

    print(stdout, curr_patterns);
    printf("\n");

    printf ("%d lights\n", count_lights(patterns, curr_patterns));
    
    for (int i = 0; i < 6; ++i) {
        printf ("Iteration %d\n", i);
        print (stdout, curr_patterns);
        printf("\n");
        printf ("%d lights\n", count_lights(patterns, curr_patterns));
        curr_patterns = iterate_patterns(pattern2output, curr_patterns);
    }

    return 0;
}
