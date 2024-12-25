#include <iostream>
#include <fstream>
#include <string>
#include <optional>
#include <vector>
#include <sstream>
#include <filesystem>

#include "./tokenization.hpp"


std::string tokens_to_asm(const std::vector<Token>& tokens) {
    std::ostringstream output;
    output << "global _start\n_start:\n";
    for (int i = 0;i < tokens.size(); i++){
        const Token& token = tokens.at(i);
        if (token.type == TokenType::exit) {
            if (i+1 < tokens.size() && tokens.at(i+1).type == TokenType::int_lit){
                if (i+2 < tokens.size() && tokens.at(i+2).type == TokenType::semi){
                    output << "   mov rax, 60\n";
                    output << "   mov rdi, " << tokens.at(i+1).value.value() << "\n";
                    output << "   syscall";
                }
            }
        }
    }
    return output.str();
}


// char* argv[] is an array of pointers to characters (strings). 
// Each element in the array is a pointer to a char (a string).
int main(int argc , char* argv[]) {
    if (argc != 2){
        std::cerr << "Incorrect usage. Correct usage is ..." << std::endl;
        std::cerr << "hydro <input.hy>" << std::endl;
        return EXIT_FAILURE; // Exit with error code "-1
    }

    // Only input thus ifstream and not fstream or ofstream
    std::ifstream fin(argv[1]);
    // Open the file specified by argv[1] and read it.
    // If the file cannot be opened, then an error message is printed to stderr.
    if (!fin){
        std::cerr << "Could not open file: " << argv[1] << std::endl;
        return EXIT_FAILURE; // Exit with error code "-1"
    }
    std::string content((std::istreambuf_iterator<char>(fin)), std::istreambuf_iterator<char>());
    fin.close(); 
    // Print the contents of the file to the console.   
    std::cout << "File content:\n" << content;
    // The return value of the function is 0 (EXIT_SUCCESS).
    Tokenizer tokenizer(std::move(content));
    std::vector<Token>  tokens = tokenizer.tokenize();

    std::cout << tokens_to_asm(tokens) << std::endl;

    // Write the content into a file specified by argv[2]
    std::filesystem::path p = std::filesystem::current_path();
    p /= "output";
    p /= "out.asm";
    std::ofstream fout(p);
    if (!fout) {
        std::cerr << "Failed to open output file." << std::endl;
        return 1;
    }

    // Write the content into the output file
    fout << tokens_to_asm(tokens);
    fout.close();

    system("nasm -felf64 ./output/out.asm");
    system("ld -o ./output/out ./output/out.o");

    return EXIT_SUCCESS;
}
