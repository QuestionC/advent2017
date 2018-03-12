// Count the number of digits which match the next digit in input1.txt

#include <fstream>
#include <string>
#include <iostream>
#include <stdlib.h>

int main (void) {
    std::ifstream file("input1.txt");
    std::string str;
    std::getline(file, str);

    char const * c_str = str.c_str();

    int repeats = 0;
    for (char const * ch = c_str; *ch != '\0' && *ch != '\n'; ++ch) {
        char const * next = ch + 1;
        if (*next != '\0' && *next != '\n') {
            if (*ch == *next) {
                // std::cout << *ch << " match in position " << ch - c_str << '\n';
                repeats += *ch - '0';
            }
        }
        else { // end of string
            if (*ch == c_str[0]) {
                // std::cout << *ch << " match at end" << '\n';
                repeats += *ch - '0';
            }
        }
    }

    std::cout << repeats << '\n';

    return 0;
}
