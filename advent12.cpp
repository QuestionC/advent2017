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

    std::unordered_set<int> visited;
    std::queue<int> queue;
    int groups = 0;
    for (int i = 0; i < connections.size(); ++i) {
        if (visited.count(i) == 0) {
            ++groups;
            queue.push(i);
            visited.insert(i);
    
            while (!queue.empty()) {
                int item = queue.front();
                queue.pop();

                std::vector<int> const & neighbors = connections[item];
                for (std::vector<int>::const_iterator ci = neighbors.begin(); ci != neighbors.end(); ++ci) {
                    if (visited.count(*ci) == 0) {
                        queue.push(*ci);
                        visited.insert(*ci);
                    }
                }
            }
        }
    }

    printf("%d\n", groups);

    return 0;
}
