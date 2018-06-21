#include "advent.hpp"
#include <string.h>
#include <queue>

char map[255][255];

int main (void) {
    FILE * f = fopen("input19.txt", "r");
   
    int i = 0;
    char buff[255];
    char * p = buff;
    size_t line_length = 255;

    while(getline(&p, &line_length, f) > 0) {
        strcpy(map[i++], p);
    }

    int j;
    for (j = 0; j < 255; ++j) {
        if (map[0][j] == '|') {
            break;
        }
    }

    int start_x = j;
    int start_y = 0;

    printf ("%d, %d\n", start_x, start_y);

    int x = start_x;
    int y = start_y;
    int dir_x = 0;
    int dir_y = 1;
    int steps = 0;

    std::vector<char> visited;

    for(;;) {
        steps++;
        y += dir_y;
        x += dir_x;

        char curr = map[y][x];

        if (isspace(curr)) {
            break;
        } else if (isalpha(curr)) {
            visited.push_back(curr);
        } else if (curr == '+') {
            if (dir_x == 0) {
                if (!isspace(map[y][x + 1])) {
                    dir_x = 1;
                    dir_y = 0;
                } else if (!isspace(map[y][x - 1])) {
                    dir_x = -1;
                    dir_y = 0;
                } else {
                    printf ("Hit a '+' with nowhere to turn at (%d, %d)\n", x, y);
                    exit(-1);
                }
            } else if (dir_y == 0) {
                if (!isspace(map[y + 1][x])) {
                    dir_x = 0;
                    dir_y = 1;
                } else if (!isspace(map[y - 1][x])) {
                    dir_x = 0;
                    dir_y = -1;
                } else {
                    printf ("Hit a '+' with nowhere to turn at (%d, %d)\n", x, y);
                    exit(-1);
                }
            } else {
                printf ("Hit a '+' but dir_x and dir_y are both 0 somehow at (%d, %d)!\n", x, y);
                exit(-1);
            }
        } else if (curr == '|' || curr == '-') {
        }
        else {
            printf ("Dunno how to handle char %c at (%d, %d)\n", x, y);
            exit(-1);
        }
    }

    for (auto i = visited.begin(); i != visited.end(); ++i) {
        printf("%c\n", *i);
    }

    printf ("%d steps\n", steps);

    return 0;
}
