// It's like every program is a machine simulator.

#include "advent.hpp"
#include <stdio.h>

int read_cmd(FILE * f, std::string & cmd);
void exe_cmd(std::string const & run_me, std::vector<char> & programs);

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

    // Star 2: Do it 999999999 more times!
    for (unsigned long i = 0; i < 999'999'999; ++i) {
        for (std::vector<std::string>::const_iterator ci = cmds.begin(); ci != cmds.end(); ++ci) {
            exe_cmd(*ci, programs);
        }
        if (i % 1'000 == 0) {
            printf("%ld\n", i);
        }
    }

    DPRINT(programs);

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
