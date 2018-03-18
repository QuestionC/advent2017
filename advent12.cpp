#include "advent.hpp"
#include <stdio.h>

#include <unordered_set>
#include <queue>


int main (void) {
    FILE * f = fopen("input12.txt", "r");
    char buff[200];
    char *p = buff;
    size_t line_length = 200;

    std::vector<std::vector<int> > connections;
    while (getline(&p, &line_length, f) > 0) {
        int str_offset;
        int characters_read;
        std::vector<int> to_vector;

        int from;
        int to;
        sscanf(buff, "%d <-> %d%n", &from, &to, &str_offset);

        to_vector.push_back(to);

        DPRINT(buff);
        while (sscanf(buff + str_offset, ", %d%n", &to, &characters_read) > 0) {
            DPRINT(buff+str_offset);
            DPRINT(characters_read);
            to_vector.push_back(to);
            str_offset += characters_read;
        }

        connections.push_back(to_vector);
    }

    std::unordered_set<int> group0;
    std::queue<int> queue;

    queue.push(0);
    group0.insert(0);

    while (!queue.empty()) {
        int item = queue.front();
        queue.pop();

        std::vector<int> neighbors = connections[item];
        printf ("%d -> ", item);
        std::cout << neighbors;
        for (std::vector<int>::const_iterator ci = neighbors.begin(); ci != neighbors.end(); ++ci) {
            if (group0.count(*ci) == 0) {
                queue.push(*ci);
                group0.insert(*ci);
            }
        }
        printf("\n");
    }

    printf("%d\n", group0.size());

    return 0;
}
