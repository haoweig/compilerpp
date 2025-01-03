#pragma once 

#include <string>
#include <vector>
#include <optional>

enum class TokenType {
    exit,
    int_lit,
    semi
};

struct Token {
    TokenType type;
    std::optional<std::string> value;
};

class Tokenizer {
public:
    // inline Tokenizer(const std::string& src);
    inline Tokenizer(std::string src): m_src(std::move(src)) {

    }
    std::vector<Token> tokenize(){
        std::vector<Token> tokens;
        std::string buf;


        while(peak().has_value()){
            if(std::isalpha(peak().value())){
                buf.push_back(consume());
                while (peak().has_value() && std::isalnum(peak().value())){
                    buf.push_back(consume());
                }
                if (buf == "exit"){
                    tokens.push_back(Token{TokenType::exit});
                    buf.clear();
                    continue;
                } else {
                    std::cerr << "You messed up!" << std::endl;
                    exit(EXIT_FAILURE);
                }
            }
            else if (std::isdigit(peak().value())) {
                buf.push_back(consume());
                while(peak().has_value() && std::isdigit(peak().value())){
                    buf.push_back (consume());
                }
                tokens.push_back(Token{TokenType::int_lit, buf});
                buf.clear();
            }
            else if (peak().value() == ';'){
                consume();
                tokens.push_back(Token{TokenType::semi, ";"});
                continue;
            }
            else if (std::isspace(peak().value())){
                consume();
                continue;
            } else {
                std::cerr << "You messed up!" << std::endl;
                exit(EXIT_FAILURE); 
            }
        }
        m_index = 0;
        return tokens;
    }

private:

    std::optional<char> peak(int ahead = 1) const {
        if (m_index + ahead > m_src.length()){
            return {};
        } else {
            return m_src.at(m_index);
        }
    }

    char consume (){
        return m_src.at(m_index++);
    }
    
    const std::string m_src; 
    int m_index = 0;
};