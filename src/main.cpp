#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <optional>

void help();
std::optional<std::string> compile(const std::string& contents, const std::map<std::string, bool>& options);

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
    if (argc < 2 || (argc == 2 && (std::string(argv[1]) == "-h" || std::string(argv[1]) == "--help"))) {
        help();
        return 0;
    }

    std::string filename;
    std::string outputFilename;
    bool outputFilenameSpecified = false;

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (options.find(arg) != options.end()) {
            options[arg] = true;
            if (arg == "-o" || arg == "--output") {
                if (i + 1 < argc) {
                    outputFilename = argv[++i]; 
                    outputFilenameSpecified = true;
                } else {
                    std::cerr << "Error: No output file specified after " << arg << "\n";
                    return 1;
                }
            }
        } else if (i == argc - 1) { 
            filename = arg;
        } else {
            std::cerr << "Unknown option: " << arg << "\n";
            return 1;
        }
    }

    if (filename.empty()) {
        std::cerr << "Error: No input file specified.\n";
        return 1;
    }

    std::ifstream file(filename);
    if (!file) {
        std::cout << "Could not open file: " << filename << std::endl;
        return 1;
    }

    std::string contents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    auto output = compile(contents, options);
    if (!output) {
        std::cerr << "Compilation failed.\n";
        return 1;
    }

    if (outputFilenameSpecified) {
        std::cout << "Output has been written to: " << outputFilename << "\n";
        std::ofstream outputFile(outputFilename);
    } else {
        outputFilename = filename.substr(0, filename.find_last_of('.'));
        std::cout << "Output has been  written to: " << outputFilename << "\n";
        std::ofstream outputFile(outputFilename);
    }

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

std::optional<std::string> compile(const std::string& contents, const std::map<std::string, bool>& options) {
    return "";
}