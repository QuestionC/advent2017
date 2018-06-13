#include <string>
#include <map>
#include <vector>
#include "advent.hpp"

int last_snd = 0;

struct Command {
    std::string cmd;
    std::string A;
    std::string B;

    Command(std::string const & cmd_, std::string const & A_, std::string const & B_) : cmd(cmd_), A(A_), B(B_) {}
};

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
int process(std::vector<Command> const & commands, int & ip, std::map<std::string, long long> & registers) {
    std::string cmd = commands[ip].cmd;
    std::string A = commands[ip].A;
    std::string B = commands[ip].B;
    ++ip;

    if (cmd == "set") {
        registers[A] = value(B, registers);
    } else if (cmd == "snd") {
        last_snd = value(A, registers);
    } else if (cmd == "add") {
        registers[A] = value(A, registers) + value(B, registers);
    } else if (cmd == "mul") {
        registers[A] = value(A, registers) * value(B, registers);
    } else if (cmd == "mod") {
        registers[A] = value(A, registers) % value(B, registers);
    } else if (cmd == "rcv") {
        if (value(A, registers) != 0)
            return -1;
    } else if (cmd == "jgz") {
        if (value(A, registers) > 0)
            ip += value(B, registers) - 1;
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

    std::map<std::string, long long> registers;
    int ip = 0;

    int result = 0;
    while (result == 0) {
        print(stdout, commands[ip]);
        printf("\nip: %d\n", ip);
        result = process(commands, ip, registers);
        print(stdout, registers);
        printf("-----\n");
    }

    printf("last_snd: %d\n", last_snd);

    return 0;
}
