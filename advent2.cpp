// For each row in input2.txt, find the difference between the max and min value
// Return the sum of the max and min value for each line

// For the second star, the checksum is the division of the only 2 numbers that evenly divide in each line

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

// See if N divides any number in the vector or vice versa and return the division.
// Return 0 if there is no division
int divides_check(int N, std::vector<int> & V) {
    std::vector<int>::const_iterator ci;
    for (ci = V.begin(); ci != V.end(); ++ci) {
        if (N % *ci == 0) {
            return N / *ci;
        }
        else if (*ci % N == 0) {
            return *ci / N;
        }
    }

    return 0;
}

int main(void) {
    std::ifstream input("input2.txt");
    std::string line;

    int checksum_total = 0;
    while(std::getline(input, line)) {
        std::stringstream stream(line);

        std::vector<int> values;

        int curr_num;
        while (stream >> curr_num) {
            int N = divides_check(curr_num, values);
            if (N == 0) {
                values.push_back(curr_num);
            }
            else { // Success!
                checksum_total += N;
                break;
            }
        }
        if (!stream) {
            std::cerr << "We got to the end of stream without dividing!\n";
        }
    }

    std::cout << checksum_total;
}
