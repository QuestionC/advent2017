#include "advent.hpp"

#include <stdio.h>

int main (void) {
    FILE * f = fopen("input9.txt", "r");

    int group_lvl = 0;
    bool in_garbage = false;
    int group_total = 0;
    int garbage_total = 0;
    for (;;) {
        char ch = fgetc(f);
        if (ch == EOF)
            break;

        switch(ch) {
            case '{':
                if (in_garbage)
                    ++garbage_total;

                // Start of new group
                if (!in_garbage) 
                    ++group_lvl;
                break;
            case '}':
                if (in_garbage)
                    ++garbage_total;

                // End of group
                if (!in_garbage)
                    group_total += group_lvl--;
                break;
            case '!':
                // Cancel next symbol
                (void) fgetc(f);
                break;
            case '<':
                if (in_garbage)
                    ++garbage_total;
                in_garbage = true;
                break;
            case '>':
                in_garbage = false;
                break;
            default:
                if (in_garbage)
                    ++garbage_total;
                break;
        } // End switch
    } // End for

    DPRINT(group_total);
    DPRINT(garbage_total);

    return 0;
}

