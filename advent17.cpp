#include "advent.hpp"

struct Node {
    int val;
    Node * next;
};

#define MAGIC 376

int main (void) {
    int len = 1;
    Node * N = new Node();
    N->val = 0;
    N->next = N;

    for (int i = 1; i <= 2017; ++i) {
        int advance = MAGIC % len;
        for (int j = 0; j < advance; ++j) {
            N = N->next;
        }

        Node * new_node = new Node();
        new_node->val = i;
        new_node->next = N->next;
        N->next = new_node;
        N = new_node;
        ++len;
    }

    DPRINT(N->val);
    DPRINT(N->next->val);
    DPRINT(N->next->next->val);
}
