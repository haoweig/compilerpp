#include <iostream>
#include <fstream>
#include <string>
#include <optional>
#include <vector>

enum class TokenType {
    _return,
    int_lit,
    semi
};

struct Token {
    TokenType type;
    std::optional<std::string> value;
};

std::vector<Token> tokenize(const std::string& str){
    std::vector<Token> tokens;
    std::string buf;
    for (int i =0; i < str.length(); i++){
        char c = str.at(i);
        if (std::isalpha(c)){
            buf.push_back(c);
            i++;
            while (std::isalnum(str.at(i))){
                buf.push_back(str.at(i));
                i++;
            }
            i--;
            if (buf == "return"){
                tokens.push_back(Token{TokenType::_return, buf});
                buf.clear();
                continue;
            } else {
                std::cerr << "You messed up!" << std::endl;
                exit(EXIT_FAILURE);
            }
        }
        else if (std::isdigit(c)){
            buf.push_back(c);
            i++;
            while (std::isdigit(str.at(i))){
                buf.push_back(str.at(i));
                i++;
            }
            i--;
            tokens.push_back(Token{TokenType::int_lit, buf});
            buf.clear();
            // continue;
        }
        else if (c == ';'){
            tokens.push_back(Token{TokenType::semi, ";"});
            continue;
        }
        // Handle other cases like punctuation and whitespace (
        else if (std::isspace(c)){
            continue;
        } else {
            std::cerr << "You messed up!" << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    return tokens;
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
    std::vector<Token>  tokens = tokenize(content);

    return EXIT_SUCCESS;
}
