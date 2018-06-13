#include "advent.hpp"

#define MAGIC 376

int main (void) {
    int len = 1;
    int firstVals[MAGIC + 1];
    firstVals[0] = 0;

    int pos = 0;

    for (int i = 1; i <= 50'000'000; ++i) {
        int next = (pos + MAGIC) % len + 1;

        if (next < MAGIC) {
            std::move(firstVals + next, firstVals + MAGIC - 1, firstVals + next + 1);
            firstVals[next] = i;
        }

        pos = next;
        ++len;
    }

    DPRINT(firstVals[0]);
    DPRINT(firstVals[1]);
    DPRINT(firstVals[2]);

    return 0;
}
