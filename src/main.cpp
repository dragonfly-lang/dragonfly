#include <iostream>
#include <map>
#include <string>

void help();
void compile(std::string file, std::map<std::string, bool> options);

std::map <std::string, bool> options = {
    {"-l", false},
    {"--lex", false},
    {"-p", false},
    {"--parse", false},
    {"-c", false},
    {"--check", false},
    {"-g", false},
    {"--gen", false},
    {"-o", false},
    {"--output", false}
};


int main(int argc, char** argv) {
    if (argc == 1 || (argc == 2 && (std::string(argv[1]) == "-h" || std::string(argv[1]) == "--help"))) {
        help();
        return 0;
    }

    std::string file = argv[argc - 1];

    FILE *f = fopen(file.c_str(), "r");
    if (!f) {
        std::cout << "Could not open file: " << file << std::endl;
        return 1;
    }

    for (int i = 1; i < argc - 1; i++) {
        std::string arg = argv[i];
        if (options.find(arg) != options.end()) {
            options[arg] = true;
        } else {
            std::cout << "Unknown option: " << arg << std::endl;
            return 1;
        }
    }

    compile(file, options);
    fclose(f);

    return 0;
}

// Called for `dragon --help`
void help() {
    std::cout << (
        "Usage: dragon <arguments> [options]\n\n"
        "Arguments:\n"
        "  <file>         The file to compile\n\n"
        "Options:\n"
        "  -h, --help     Display this information\n"
        "  -l, --lex      Run the lexer and print tokens\n"
        "  -p, --parse    Run the parser and print the AST\n"
        "  -c, --check    Run the semantic analyser and print any errors\n"
        "  -g, --gen      Generate the LLVM IR\n"
        "  -o, --output   Specify the output file\n\n"
        "By default, the compiler will run all stages and produce an executable file of the same name as the argument.\n"
        );
}