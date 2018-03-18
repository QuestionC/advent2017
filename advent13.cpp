// Machine simulation

#include "advent.hpp"
#include <map>

struct FirewallMachine;
void print(FILE *f, FirewallMachine const & print_me);

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

    struct Walker {
        FirewallMachine * machine;
        int position;
        rows_t::const_iterator iter;

        Walker(FirewallMachine * F) : machine(F), position(0), iter(machine->rows.begin()) {
        }

        Walker() {}
        

        bool caught() {
            if (position == iter->first) {
                int range = iter->second.first;
                int scanner = iter->second.second;

                if (scanner == 0) {
                    return true;
                }
            }
            return false;
        }

        bool next() {
            if (position == iter->first) {
                ++iter;
            }
            ++position;

            return iter != machine->rows.end();
        }
    };

    Walker walker() {
        return Walker(this);
    }
};

void print(FILE *f, FirewallMachine const & print_me) {
    print(f, print_me.rows);
}

void print(FILE * f, FirewallMachine::Walker const & print_me) {
    fprintf (f, "position: %d, iterator: ", print_me.position);
    print (f, *print_me.iter);
}

int main (void) {
    FILE * f = fopen("input13.txt", "r");

    int chars_read;

    FirewallMachine machine;

    { // Build the machine
        int depth, range;

        while (fscanf(f, " %d : %d", &depth, &range) > 0) {
            machine.addLayer(depth, range);
        }
    } // Done building machine

    FirewallMachine backup = machine;
    FirewallMachine::Walker my_walker = machine.walker();

    int penalty = 0;
    for (;;) {
        if (my_walker.caught()) {
            penalty += my_walker.position * my_walker.iter->second.first;
        }

        if (!my_walker.next())
            break;

        machine.step();
    }

    printf("%d\n", penalty);

    // Now let's spin up a lot of walkers and kill them when they fail.  If a walker makes it to the end, report it.

    machine = backup;

    std::map<int, FirewallMachine::Walker> walkers;
    std::map<int, FirewallMachine::Walker>::iterator iter;
    for (int i = 0; ; ++i) {
        // The key is the delay for the walker.  We spawn a new walker every cycle.
        walkers[i] = machine.walker();

        //print(stdout, machine);
        //printf("\n");
    
        for (iter = walkers.begin(); iter != walkers.end(); /* ++iter */) {
            if (iter->second.caught()) {
                // printf ("Erasing delay %d due to pos %d\n", iter->first, iter->second.position);
                walkers.erase(iter++);
            }
            else {
                if (iter->second.next() == false) {
                    // Walker completed
                    break;
                }
                iter++;
            }
        }

        if (iter != walkers.end()) {
            // We terminated loop due to break, meaning we found solution.
            printf ("Breaking out\n");
            break;
        }

        machine.step();
    }

    print(stdout, *iter);
    printf("\n");

    return 0;
}
