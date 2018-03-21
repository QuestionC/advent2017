#include <stdio.h>
#include <string.h>
#include "advent14.hpp"

int count_bits(char string[16]);
void print_bits(FILE * f, const char string[16]);

int main (void) {
    char key[100];
    {
        FILE * f = fopen("input14.txt", "r");
        fscanf(f, "%s", key);
    }

    char buff[200];
    char hash[16];

    int count = 0;
    for (int salt = 0; salt < 128; ++salt) {
        sprintf(buff, "%s-%d", key, salt);

        Rope::Hash(buff, hash);
        count += count_bits(hash);
        print_bits(stdout, hash);
        printf("\n");
    }


    printf("%d\n", count);
    return 0;
}

int count_bits( char string[16]) {
    int count = 0;
    for (int i = 0; i < 16; ++i) {
        char ch = string[i];
        for (char mask = 1; mask; mask <<= 1) {
            if (mask & ch) 
                ++count;
        }
    }
    return count;
}

void print_bits(FILE * f, const  char string[16]) {
    for (int i = 0; i < 16; ++i) {
        char ch = string[i];
        // fprintf(f, "%x", ch);
        for (unsigned char mask = '\x80'; mask; mask >>= 1) {
            if (mask & ch) {
                fputc('#', f);
            } else {
                fputc('.', f);
            }
        }
    }
    return;
}
