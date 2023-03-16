#ifndef CAP_LEXER_HPP
#define CAP_LEXER_HPP


#include <vector>
#include "Error.hpp"


namespace cap {


    /**
     * Token types.
     */
    enum class TOKEN {
        TYPEDEF,
        DOUBLE,
        STRUCT,
        CHAR,
        ENUM,
        VOID,
        INT,
        STRING,
        CHARACTER,
        IDENTIFIER,
        FLOAT,
        INTEGER,
        TILDE,
        BACKTICK,
        EXCLAMATION_MARK,
        AT_SIGN,
        NUMBER_SIGN,
        DOLLAR_SIGN,
        PERCENT,
        CARET,
        AMBERSAND,
        STAR,
        OPENING_PARENTHESIS,
        CLOSING_PARENTHESIS,
        UNDERSCORE,
        MINUS,
        PLUS,
        EQUALS,
        OPENING_CURLY_BRACKET,
        OPENING_SQUARE_BRACKET,
        CLOSING_CURLY_BRACKET,
        CLOSING_SQUARE_BRACKET,
        VERTICAL_BAR,
        BACKSLASH,
        COLON,
        SEMICOLON,
        DOUBLE_QUOTE,
        SINGLE_QUOTE,
        LESS_THAN,
        COMMA,
        GREATER_THAN,
        DOT,
        QUESTION_MARK,
        SLASH
    };


    /**
     * Token structure.
     */
    struct Token {
        TOKEN token;
        Position position;
        std::string_view content;

        bool operator == (const TOKEN& other) const {
            return token == other;
        }
    };


    /**
     * Tokenization function.
     * @param input input.
     * @param output output.
     * @param errors errors.
     */
    void tokenize(const std::string& input, std::vector<Token>& output, std::vector<Error>& errors);


} //namespace cap


#endif //CAP_LEXER_HPP
