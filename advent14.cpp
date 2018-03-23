#include <stdio.h>
#include <string.h>
#include "advent14.hpp"
#include <set>
#include <queue>
#include <map>

int count_bits(char string[16]);
void print_bits(FILE * f, const char string[16]);
int count_regions(BinaryGrid const & count_me);
bool fill_region(BinaryGrid const & grid, std::map<std::pair<int, int>, char> & visited, int x, int y, char region_name);

int main (void) {
    char key[100];
    {
        FILE * f = fopen("input14.txt", "r");
        fscanf(f, "%s", key);
    }

    // strcpy(key, "flqrgnkx");

    char buff[200];
    char hash[16];

    int count = 0;
    BinaryGrid my_grid;
    for (int salt = 0; salt < 128; ++salt) {
        sprintf(buff, "%s-%d", key, salt);

        Rope::Hash(buff, hash);
        memcpy(my_grid.buff[salt], hash, sizeof(hash));
        count += count_bits(hash);
    }

    printf("%d\n", count);

    print(stdout, my_grid);


    printf("%d\n", count_regions(my_grid));

    return 0;
}

int count_bits( char string[16]) {
    int count = 0;
    for (int i = 0; i < 16; ++i) {
        char ch = string[i];
        for (char mask = 1; mask; mask <<= 1) {
            if (mask & ch) 
                ++count;
        }
    }
    return count;
}

void print_bits(FILE * f, const  char string[16]) {
    for (int i = 0; i < 16; ++i) {
        char ch = string[i];
        // fprintf(f, "%x", ch);
        for (unsigned char mask = '\x80'; mask; mask >>= 1) {
            if (mask & ch) {
                fputc('#', f);
            } else {
                fputc('.', f);
            }
        }
    }
    return;
}

int count_regions(BinaryGrid const & count_me) {
    std::map<std::pair<int, int>, char> visited;
    int regions = 0;

    for (int y = 0; y < count_me.height; ++y) {
        for (int x = 0; x < count_me.width; ++x) {
            if (fill_region(count_me, visited, x, y, 'a' + (regions % 26)))
                regions += 1;
        }
    }

    // Print the region map so I can see what's going on
    for (int y = 0; y < 128; ++y) {
        for (int x = 0; x < 128; ++x) {
            std::pair<int, int> xy_pair(x, y);
            bool in_grid = count_me.at(x, y);
            bool in_map = visited.count(xy_pair);
            if (in_grid != in_map) {
                printf("%d, %d broken.  in_grid: %d, in_map: %d\n", x, y, in_grid, in_map);
            } else if (in_grid && in_map) {
                printf("%c", visited[xy_pair]);
            } else {
                printf(".");
            }
        }
        printf("\n");
    }

    return regions;
}

bool fill_region(BinaryGrid const & grid, std::map<std::pair<int, int>, char> & visited, int x, int y, char region_name) {
    std::pair<int, int> xy_pair(x, y);
    
    bool result = false;

    printf("x, y: %d %d\n", x, y);
    if  (grid.at(x, y) && visited.count(xy_pair) == 0) {
        printf("Initial Hit!\n");
        // The cell isn't empty and hasn't been visited yet.
        result = true;

        std::queue<std::pair<int, int> > visit_queue;

        visit_queue.push(xy_pair);
        visited[xy_pair] = region_name;

        while (!visit_queue.empty()) {
            std::pair<int, int> visit_me_pair = visit_queue.front();
            visit_queue.pop();

            int visit_me_x = visit_me_pair.first;
            int visit_me_y = visit_me_pair.second;

            printf("Queue: %d, %d\n", visit_me_x, visit_me_y);

            // Put neighbors into the queue.
            std::vector< std::pair<int, int> > visit_us;
            visit_us.push_back(std::make_pair(visit_me_x + 1, visit_me_y));
            visit_us.push_back(std::make_pair(visit_me_x - 1, visit_me_y));
            visit_us.push_back(std::make_pair(visit_me_x, visit_me_y + 1));
            visit_us.push_back(std::make_pair(visit_me_x, visit_me_y - 1));
            for (auto ci = visit_us.begin(); ci != visit_us.end(); ++ci) {
                printf("ci = %d, %d\n", ci->first, ci->second);
                if (grid.at(ci->first, ci->second) && visited.count(*ci) == 0) {
                    printf("Hit!\n");
                    visit_queue.push(*ci);
                    visited[*ci] = region_name;
                }
            }
        }
    }

    return result;
}
