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

// An exercise in class derivation.  My god how ugly.
struct Generator2 : public Generator {
    const uint64_t multiple;

    Generator2(uint64_t Factor, uint64_t Multiple, uint64_t seed) : Generator(Factor, seed), multiple(Multiple) {}

    void step(void) {
        for (Generator::step(); val % multiple != 0; Generator::step());
    }
};

bool judge(uint64_t A, uint64_t B) {
    return (A & 0xFFFF) == (B & 0xFFFF);
}

int main (void) {
    // Normally I'd parse the input but it's a weird format for 2 numbers.
    Generator genA(16807, 883);
    Generator genB(48271, 879);

    /*
    // Example
    Generator genA(16807, 65);
    Generator genB(48271, 8921);
    */


    int matches = 0;

    for (int i = 0; i < 40'000'000; ++i) {
        genA.step();
        genB.step();

        if (judge(genA.val, genB.val))
            matches++;
    }

    DPRINT(matches);
   
    Generator2 gen2A(16807, 4, 883);
    Generator2 gen2B(48271, 8, 879);
  
    /*
    // Example 
    Generator2 gen2A(16807, 4, 65);
    Generator2 gen2B(48271, 8, 8921);
    */
    
    int matches2 = 0;
    for (int i = 0; i < 5'000'000; ++i) {
        gen2A.step();
        gen2B.step();

        if (judge(gen2A.val, gen2B.val)) {
            matches2++;
        }
    }
    
    DPRINT(matches2);

    return 0;
}
