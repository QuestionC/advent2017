// For each row in input2.txt, find the difference between the max and min value
// Return the sum of the max and min value for each line

#include <fstream>
#include <iostream>
#include <sstream>

int main(void) {
    std::ifstream input("input2.txt");
    std::string line;

    int checksum_total = 0;
    while(std::getline(input, line)) {
        std::stringstream stream(line);

        int max, min;
        stream >> max;
        min = max;

        int curr_num;
        while (stream >> curr_num) {
            if (max < curr_num) {
                max = curr_num;
            }
            else if (min > curr_num) {
                min = curr_num;
            }
        }
        checksum_total += max - min;
    }

    std::cout << checksum_total;
}
