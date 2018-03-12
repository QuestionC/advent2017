// Count the number of digits which match the next digit in input1.txt

#include <fstream>
#include <string>
#include <iostream>
#include <stdlib.h>

int main (void) {
    std::ifstream file("input1.txt");
    std::string str;
    std::getline(file, str);

    int repeats = 0;
    int len = str.length();
    int index = 0;

    int i;
    int compare;
    for (i = 0, compare = len / 2; i < len; ++i, compare = (compare + 1) % len) {
        if (str[i] == str[compare]) {
            repeats += str[i] - '0';
        }
    }

    std::cout << repeats << '\n';

    return 0;
}
