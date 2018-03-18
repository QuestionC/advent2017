// Process a bunch of instructions

#include "advent.hpp"

#include <unordered_map>
#include <string.h>

int g_max;

enum CMP {
    LT, GT, EQ, NEQ, LTE, GTE, NUL
};

std::ostream & operator<< (std::ostream& out, CMP C) {
    std::string result;
    switch(C) {
        case CMP::LT:
            result = "LT";
            break;
        case CMP::GT:
            result = "GT";
            break;
        case CMP::EQ:
            result = "EQ";
            break;
        case CMP::NEQ:
            result = "NEQ";
            break;
        case CMP::LTE:
            result = "LTE";
            break;
        case CMP::GTE:
            result = "GTE";
            break;
        case CMP::NUL:
            result = "NUL";
            break;
    }

    return out << result;
}

CMP str_to_CMP(char const * s) {
    if (strcmp(s, "<") == 0) {
        return CMP::LT;
    } else if (strcmp(s, ">") == 0) {
        return CMP::GT;
    } else if (strcmp(s, "==") == 0) {
        return CMP::EQ;
    } else if (strcmp(s, "!=") == 0) {
        return CMP::NEQ;
    } else if (strcmp(s, "<=") == 0) {
        return CMP::LTE;
    } else if (strcmp(s, ">=") == 0) {
        return CMP::GTE;
    }

    return CMP::NUL;
}

struct Instruction {
    std::string store_v;
    int delta;

    std::string cond_var;
    CMP cmp;
    int cmp_amt;

    int & store(std::unordered_map<std::string, int> & memory) const {
        auto p = memory.find(store_v);
        if (p == memory.end())
            memory[store_v] = 0;

        return memory[store_v];
    }

    int & compare_value(std::unordered_map<std::string, int> & memory) const {
        auto p = memory.find(cond_var);
        if (p == memory.end())
            memory[cond_var] = 0;

        return memory[cond_var];
    }

    bool condition(std::unordered_map<std::string, int> & memory) const {
        int compare_value = this->compare_value(memory);

        DPRINT(compare_value);

        switch (cmp) {
            case CMP::LT:
                return compare_value < cmp_amt;
            case CMP::GT:
                return compare_value > cmp_amt;
            case CMP::EQ:
                return compare_value == cmp_amt;
            case CMP::NEQ:
                return compare_value != cmp_amt;
            case CMP::LTE:
                return compare_value <= cmp_amt;
            case CMP::GTE:
                return compare_value >= cmp_amt;
            case CMP::NUL:
                return false;
        }

        fprintf(stderr, "Unreachable code in Instruction::condition");
        exit(-1);
    }

    void execute(std::unordered_map<std::string, int> & memory) const {
        store(memory) += delta;
        if (store(memory) > g_max) {
            g_max = store(memory);
        }
    }
};

std::ostream& operator<< (std::ostream & out, Instruction const & print_me) {
    return out << print_me.store_v << " " << print_me.delta << " if " << print_me.cond_var << " " << print_me.cmp << " " << print_me.cmp_amt;
}

Instruction parse_instruction (std::string parse_me) {
    char const * c_str = parse_me.c_str();

    char store_v_b[10], store_cmd_b[10], cmp_v_b[10], cmp_type_b[10];
    int store_amt, cmp_amt;

    sscanf(c_str, " %s %s %d if %s %s %d", store_v_b, store_cmd_b, &store_amt, cmp_v_b, cmp_type_b, &cmp_amt);

    Instruction result;
    result.store_v = store_v_b;

    int sign;
    if (strcmp(store_cmd_b, "inc") == 0) {
        sign = 1;
    } else if (strcmp(store_cmd_b, "dec") == 0) {
        sign = -1;
    } else {
        fprintf(stderr, "Unrecognized command in instruction [%s]\n", c_str);
        exit(-1);
    }

    result.delta = sign * store_amt;

    result.cond_var = cmp_v_b;

    result.cmp = str_to_CMP(cmp_type_b);

    result.cmp_amt = cmp_amt;

    return result;
}

int main (void) {
    std::ifstream input("input8.txt");
    std::string line;

    std::vector <Instruction> code;
    std::unordered_map<std::string, int> memory;

    while (std::getline(input, line)) {
        Instruction curr_instruction = parse_instruction(line);

        DPRINT(curr_instruction);

        if (curr_instruction.condition(memory)) {
            curr_instruction.execute(memory);
        }
        printf("%s = %d\n", curr_instruction.store_v.c_str(), memory[curr_instruction.store_v]);
    }

    auto max = memory.begin();
    for (auto i = memory.begin(); i != memory.end(); ++i) {
        if (i->second > max->second) {
            max = i;
        }
    }

    DPRINT(max->second);

    DPRINT(g_max);

    return 0;
}
