#include "advent.hpp"

// Emulate an instruction machine that jumps around itself

std::vector<int> get_input(void) {
    std::vector<int> result;

    std::ifstream input("input5.txt");

    int i;

    while (input >> i) {
        result.push_back(i);
    }

    return result;
}

int main (void) {
    std::vector<int> offsets = get_input();

    int instruction_pointer = 0;
    int num_steps = 0;

    while (instruction_pointer >= 0 && instruction_pointer < offsets.size()) {
        int distance = offsets[instruction_pointer];

        if (distance >= 3) {
            offsets[instruction_pointer]--;
        }
        else {
            offsets[instruction_pointer]++;
        }
        instruction_pointer += distance;

        num_steps++;
    }

    std::cout << num_steps;

    return 0;
}
