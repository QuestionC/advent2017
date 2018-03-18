// Machine simulation

#include "advent.hpp"
#include <map>

struct FirewallMachine {
    typedef std::map<int, std::pair<int, int> > rows_t;
    std::map<int, std::pair<int, int> > rows;

    void addLayer(int depth, int range) {
        rows[depth] = std::make_pair(range, 0);
    }

    void step() {
        for (auto curr_row = rows.begin(); curr_row != rows.end(); ++curr_row) {
            int const & depth = curr_row->second.first;
            int & pos = curr_row->second.second;

            // If the depth is 4, then pos goes from 0-5.  0-2 going down, 3-5
            // [0] 1 2 3 = 0
            // 0 [1] 2 3 = 1
            // 0 1 [2] 3 = 2
            // 0 1 2 [3] = 3
            // 0 1 [2] 3 = 4
            // 0 [1] 2 3 = 5

            int num_states = (depth - 1) * 2;
            pos = (pos + 1) % num_states;
        }
    }
};

void print(FILE *f, FirewallMachine const & print_me) {
    print(f, print_me.rows);
}


int main (void) {
    FILE * f = fopen("input13.txt", "r");

    int chars_read;

    int depth, range;
    FirewallMachine machine;

    while (fscanf(f, " %d : %d", &depth, &range) > 0) {
        machine.addLayer(depth, range);
    }

    int position = 0;
    FirewallMachine::rows_t::iterator iter = machine.rows.begin();

    int penalty = 0;
    for (;;) {
        DPRINT(position);
        print(f, machine);
        printf("\n-------------------\n");

        if (position == iter->first) {
            // We are on a row
            int range = iter->second.first;
            int scanner = iter->second.second;

            if (scanner == 0) {
                printf("Penalty!\n");
                // If we stepped on a row in pos0, penalty.
                penalty += position * range;
            }
            ++iter;

            if (iter == machine.rows.end())
                break;

        }

        ++position;
        machine.step();
    }

    printf("%d\n", penalty);

    return 0;
}
