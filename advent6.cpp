#include "advent.hpp"

// Basically building a pits and stones machine.

#include <algorithm>
#include <unordered_set>

std::vector<int> get_input() {
    std::ifstream input("input6.txt");
    int i;
    std::vector<int> result;
    while (input >> i) {
        result.push_back(i);
    }

    return result;
}

void distribute(std::vector<int> & bank) {
    std::vector<int>::iterator curr = std::max_element(bank.begin(), bank.end());

    int val = *curr;
    *curr = 0;
    
    for (;;) {
        if (++curr == bank.end()) {
            curr = bank.begin();
        }

        if (val == 0) {
            break;
        }

        *curr += 1;
        val -= 1;
    }

    return;
}

int main (void) {
    std::vector<int> memory_bank = get_input();

    std::unordered_set<std::string> seen;

    seen.insert(ToString(memory_bank));

    int cycles = 1;
    for (;;) {
        distribute(memory_bank);

        std::string s = ToString(memory_bank);
        if (seen.count(s) != 0) {
            break;
        }

        seen.insert(s);
        cycles += 1;
    }

    std::cout << cycles << "\n";

    return 0;
}
