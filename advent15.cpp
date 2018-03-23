// Advent 15, Dueling Generators

#include "advent.hpp"
#include <stdint.h>

// The generators are like rudimentary RNGs.  You multiply and modulo over and over.
// You always modulo by 2147483647 aka 0x7FFFFFFF.
// The multiply factors are 16807 and 48271, so we're guaranteed that an 8-bit integer will be able to hold the result each time thank god.
struct Generator {
    const uint64_t factor;
    const uint64_t divisor = 0x7FFFFFFF;
    uint64_t val;

    Generator(uint64_t Factor, uint64_t seed) : factor(Factor), val(seed) {}

    void step(void) {
        val = (val * factor) % divisor;
    }
};


bool judge(uint64_t A, uint64_t B) {
    return (A & 0xFFFF) == (B & 0xFFFF);
}

int main (void) {
    // Normally I'd parse the input but it's a weird format for 2 numbers.
    Generator genA(16807, 883);
    Generator genB(48271, 879);

    int matches = 0;

    for (int i = 0; i < 40'000'000; ++i) {
        genA.step();
        genB.step();

        if (judge(genA.val, genB.val))
            matches++;
    }

    DPRINT(matches);

    return 0;
}
