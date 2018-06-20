#include <string>
#include <map>
#include <vector>
#include "advent.hpp"
#include <queue>

struct Command {
    std::string cmd;
    std::string A;
    std::string B;

    Command(std::string const & cmd_, std::string const & A_, std::string const & B_) : cmd(cmd_), A(A_), B(B_) {}
};

int sent_by_1 = 0;

void print(FILE * f, Command const & C) {
    fprintf(f, "%s %s %s", C.cmd.c_str(), C.A.c_str(), C.B.c_str());
}

long long value(std::string S, std::map<std::string, long long> const & registers) {
    long long val;
    int n = sscanf(S.c_str(), "%lld", &val);

    if (n == 0) {
        val = registers.at(S);
    }

    return val;
}

// Process the command pointed at by ip with the given registers.
// Returns the result of a rcv command, or 0 for all other commands
int process(std::vector<Command> const & commands, int & ip0, std::map<std::string, long long> & registers0, std::queue<long long> & queue0to1, int & ip1, std::map<std::string, long long> & registers1, std::queue<long long> & queue1to0) {

    bool queue0lock = false;
    {
        std::map<std::string, long long> & registers = registers0;
        int & ip = ip0;
        std::string cmd = commands[ip].cmd;
        std::string A = commands[ip].A;
        std::string B = commands[ip].B;
        ++ip;
        std::queue<long long> & queue_out = queue0to1;
        std::queue<long long> & queue_in = queue1to0;

        printf("T1: %s %s %s\n", cmd.c_str(), A.c_str(), B.c_str());

        if (cmd == "set") {
            registers[A] = value(B, registers);
        } else if (cmd == "snd") {
            queue_out.push(value(A, registers));
        } else if (cmd == "add") {
            registers[A] = value(A, registers) + value(B, registers);
        } else if (cmd == "mul") {
            registers[A] = value(A, registers) * value(B, registers);
        } else if (cmd == "mod") {
            registers[A] = value(A, registers) % value(B, registers);
        } else if (cmd == "rcv") {
            if (!queue_in.empty()) {
                registers[A] = queue_in.front();
                queue_in.pop();
            }
            else {
                --ip;
                queue0lock=true;
            }
        } else if (cmd == "jgz") {
            if (value(A, registers) > 0)
                ip += value(B, registers) - 1;
        }
    }

    if (queue0lock == false) {
        return 0;
    }

    {
        std::map<std::string, long long> & registers = registers1;
        int & ip = ip1;
        std::string cmd = commands[ip].cmd;
        std::string A = commands[ip].A;
        std::string B = commands[ip].B;
        ++ip;
        std::queue<long long> & queue_out = queue1to0;
        std::queue<long long> & queue_in = queue0to1;

        printf("T2: %s %s %s\n", cmd.c_str(), A.c_str(), B.c_str());

        if (cmd == "set") {
            registers[A] = value(B, registers);
        } else if (cmd == "snd") {
            queue_out.push(value(A, registers));
            sent_by_1++;
        } else if (cmd == "add") {
            registers[A] = value(A, registers) + value(B, registers);
        } else if (cmd == "mul") {
            registers[A] = value(A, registers) * value(B, registers);
        } else if (cmd == "mod") {
            registers[A] = value(A, registers) % value(B, registers);
        } else if (cmd == "rcv") {
            if (!queue_in.empty()) {
                registers[A] = queue_in.front();
                queue_in.pop();
            }
            else {
                --ip;

                return -1;
            }
        } else if (cmd == "jgz") {
            if (value(A, registers) > 0)
                ip += value(B, registers) - 1;
        }
    }

    return 0;
    
}

int main(void) {
    FILE * f = fopen("input18.txt", "r");
    char buff[200];
    char *p = buff;
    size_t line_length = 200;

    std::vector<Command> commands;

    while(getline(&p, &line_length, f) > 0) {
        char cmd[10];
        char A[10];
        char B[10];
        int n = sscanf(p, "%s %s %s", cmd, A, B);

        if (n == 2) {
            printf("%s(%s)\n", cmd, A);
        }
        else if (n == 3) {
            printf("%s(%s, %s)\n", cmd, A, B);
        }
        else {
            printf ("AAAAAAAH\n");
            return -1;
        }

        commands.push_back(Command(cmd, A, B));
    }


    std::map<std::string, long long> registers0;
    int ip0 = 0;
    std::map<std::string, long long> registers1;
    int ip1 = 0;
    registers0["p"] = 0;
    registers1["p"] = 1;

    std::queue<long long> queue0to1;
    std::queue<long long> queue1to0;

    for (;;) {
        int result = process(commands, ip0, registers0, queue0to1, ip1, registers1, queue1to0);
        if (result != 0)
            break;
    }

    printf("sent_by_1: %d\n", sent_by_1);

    return 0;
}
