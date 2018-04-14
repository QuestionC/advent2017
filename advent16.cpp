// It's like every program is a machine simulator.

#include "advent.hpp"
#include <stdio.h>

int read_cmd(FILE * f, std::string & cmd);
void exe_cmd(std::string const & run_me, std::vector<char> & programs);
std::vector<int> apply_transform(std::vector<int> const & transform_me, std::vector<int> const & transformation);

int main(void) {
    // The input file is 48k and we go through them in one pass, so let's not load it into a string.

    FILE * f = fopen("input16.txt", "r");
    std::vector<char> programs;
    for (int i = 0; i < 16; ++i) {
        programs.push_back('a' + i);
    }

    std::vector<std::string> cmds;
    std::string cmd;
    while (read_cmd(f, cmd)) {
        DPRINT(programs);
        DPRINT(cmd);
        exe_cmd(cmd, programs);
        cmds.push_back(cmd);
    }

    DPRINT(programs);

    // PART 2
    fclose(f);
    f = fopen("input16.txt", "r");

    std::vector<int> positional_swaps;
    std::vector<int> character_swaps;
    for (int i = 0; i < 16; ++i) {
        positional_swaps.push_back(i);
        character_swaps.push_back(i);
    }

    while (read_cmd(f, cmd)) {
        char const * c_str = cmd.c_str();
        int A, B;
        switch(cmd[0]) {
            case 's':
            sscanf(c_str + 1, "%d", &A);
            std::rotate(positional_swaps.rbegin(), positional_swaps.rbegin() + A, positional_swaps.rend());
            break;

            case 'x':
            sscanf(c_str + 1, "%d/%d", &A, &B);
            std::swap(positional_swaps[A], positional_swaps[B]);
            break;

            case 'p':
            A = c_str[1] - 'a';
            B = c_str[3] - 'a';
            std::swap(character_swaps[A], character_swaps[B]);
            break;
        }
    }

    print(stdout, positional_swaps);
    print(stdout, '\n');
    print(stdout, character_swaps);
    print(stdout, '\n');

    // Now that we have the swaps.  Iterate them 1'000'000'000 times

    std::vector<int> curr_positional_swaps;
    std::vector<int> curr_character_swaps;
    for (int i = 0; i < 16; ++i) {
        curr_positional_swaps.push_back(i);
        curr_character_swaps.push_back(i);
    }

    int total = 1'000'000'000;
    //int total = 1;

    // Apply the swaps binarily
    for (int mask = 1; mask <= total; mask <<= 1) {
        if (mask & total) {
            curr_positional_swaps = apply_transform(curr_positional_swaps, positional_swaps);
            curr_character_swaps = apply_transform(curr_character_swaps, character_swaps);
        }

        positional_swaps = apply_transform(positional_swaps, positional_swaps);
        character_swaps = apply_transform(character_swaps, character_swaps);
    }

    DPRINT(curr_positional_swaps);
    DPRINT(curr_character_swaps);

    // Print the result
    for (std::vector<int>::const_iterator ci = curr_positional_swaps.begin(); ci != curr_positional_swaps.end(); ++ci) {
        std::vector<int>::iterator offset = std::find(curr_character_swaps.begin(), curr_character_swaps.end(), *ci);
        printf("%c", offset - curr_character_swaps.begin() + 'a');
    }
    printf("\n");

    return 0;
}

int read_cmd(FILE * f, std::string & cmd) {
    char buff[20];

    int n = fscanf(f, "%[^,],", buff);
    cmd = std::string(buff);
    return n > 0;
}

void exe_cmd(std::string const & run_me, std::vector<char> & programs) {
    char const * c_str = run_me.c_str();
    switch(c_str[0]) {
        case 's': 
            {
                // Spin, written sX, makes X programs move from the end to the front, but maintain their order otherwise. (For example, s3 on abcde produces cdeab).
                int X;
                sscanf(c_str + 1, "%d", &X);
                std::rotate(programs.rbegin(), programs.rbegin() + X, programs.rend());
                break;
            }
        case 'x':
            {
                // Exchange, written xA/B, makes the programs at positions A and B swap places.
                int A, B;
                sscanf(run_me.c_str()+1, "%d/%d", &A, &B);
                std::swap(programs[A], programs[B]);
                break;
            }
        case 'p':
            {
                // Partner, written pA/B, makes the programs named A and B swap places.
                char A = c_str[1];
                char B = c_str[3];
                std::vector<char>::iterator A_pos = std::find(programs.begin(), programs.end(), A);
                std::vector<char>::iterator B_pos = std::find(programs.begin(), programs.end(), B);

                std::swap(*A_pos, *B_pos);
                break;
            }
    } // End switch

    return;
}

std::vector<int> apply_transform(std::vector<int> const & transform_me, std::vector<int> const & transformation) {
    std::vector<int> result;

    for (auto ci = transformation.cbegin(); ci != transformation.cend(); ++ci) {
        result.push_back(transform_me[*ci]);
    }

    return result;
}
