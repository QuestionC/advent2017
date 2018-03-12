#include "advent.hpp"

// Find the number of lines in input4.txt with no duplicate words

#include <unordered_set>
#include <algorithm>

int main (void) {
    std::ifstream input("input4.txt");
    std::string line;

    int valid = 0;

    while (std::getline(input, line)) {
        std::stringstream stream(line);
        std::unordered_set<std::string> seen_words;

        std::string curr_word;

        bool valid_pass = true;
        while (stream >> curr_word) {
            // If we sort the word before inserting it catches anagrams
            std::sort(curr_word.begin(), curr_word.end());


            DPRINT(curr_word);
            if (seen_words.find(curr_word) != seen_words.end()) {
                valid_pass = false;
                std::cerr << "FAIL\n";
                break;
            }

            seen_words.insert(curr_word);
        }

        if (valid_pass) {
            std::cerr << "PASS\n";
            valid += 1;
        }
    }

    std::cout << valid << "\n";
}
