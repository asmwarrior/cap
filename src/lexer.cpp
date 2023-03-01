#include "lexer.hpp"
#include "parserlib.hpp"


namespace cap {


    using namespace parserlib;


    using parse_context = parserlib::parse_context<input_view<>, TOKEN>;


    using rule = parserlib::rule<parse_context>;


    static auto whitespace_character = range(0, 32);


    static auto block_comment = terminal("/*") >> *(!terminal("*/") >> range(0, 255)) >> terminal("*/");


    static auto line_comment = terminal("//") >> *(!terminal("\n") >> range(0, 255)) >> ('\n' | eof);


    static auto whitespace = whitespace_character
                           | block_comment
                           | line_comment;


    static auto string = (terminal('"') >> *(!terminal('"') >> range(0, 255)) >> terminal('"')) == TOKEN::STRING;

    
    static auto character = (terminal("'") >> (!terminal("'") >> range(0, 255)) >> terminal("'")) == TOKEN::CHARACTER;


    static auto letter = range('a', 'z') | range('A', 'Z');


    static auto digit = range('0', '9');


    static auto identifier = ((letter | '_') >> *(letter | digit | '_')) == TOKEN::IDENTIFIER;


    static auto float_number = (*digit >> terminal('.') >> -one_of("eE") >> -one_of("+-") >> +digit) == TOKEN::FLOAT;


    static auto integer_number = (+digit) == TOKEN::INTEGER;


    static auto tilde                  = (terminal('~')) == TOKEN::TILDE;
    static auto backtick               = (terminal('`')) == TOKEN::BACKTICK;
    static auto exclamation_mark       = (terminal('!')) == TOKEN::EXCLAMATION_MARK;
    static auto at_sign                = (terminal('@')) == TOKEN::AT_SIGN;
    static auto number_sign            = (terminal('#')) == TOKEN::NUMBER_SIGN;
    static auto dollar_sign            = (terminal('$')) == TOKEN::DOLLAR_SIGN;
    static auto percent                = (terminal('%')) == TOKEN::PERCENT;
    static auto caret                  = (terminal('^')) == TOKEN::CARET;
    static auto ambersand              = (terminal('&')) == TOKEN::AMBERSAND;
    static auto star                   = (terminal('*')) == TOKEN::STAR;
    static auto opening_parenthesis    = (terminal('(')) == TOKEN::OPENING_PARENTHESIS;
    static auto closing_parenthesis    = (terminal(')')) == TOKEN::CLOSING_PARENTHESIS;
    static auto underscore             = (terminal('_')) == TOKEN::UNDERSCORE;
    static auto minus                  = (terminal('-')) == TOKEN::MINUS;
    static auto plus                   = (terminal('+')) == TOKEN::PLUS;
    static auto equals                 = (terminal('=')) == TOKEN::EQUALS;
    static auto opening_curly_bracket  = (terminal('{')) == TOKEN::OPENING_CURLY_BRACKET;
    static auto opening_square_bracket = (terminal('[')) == TOKEN::OPENING_SQUARE_BRACKET;
    static auto closing_curly_bracket  = (terminal('}')) == TOKEN::CLOSING_CURLY_BRACKET;
    static auto closing_square_bracket = (terminal(']')) == TOKEN::CLOSING_SQUARE_BRACKET;
    static auto vertical_bar           = (terminal('|')) == TOKEN::VERTICAL_BAR;
    static auto backslash              = (terminal('\\')) == TOKEN::BACKSLASH;
    static auto colon                  = (terminal(':')) == TOKEN::COLON;
    static auto semicolon              = (terminal(';')) == TOKEN::SEMICOLON;
    static auto double_quote           = (terminal('"')) == TOKEN::DOUBLE_QUOTE;
    static auto single_quote           = (terminal('\'')) == TOKEN::SINGLE_QUOTE;
    static auto less_than              = (terminal('<')) == TOKEN::LESS_THAN;
    static auto comma                  = (terminal(',')) == TOKEN::COMMA;
    static auto greater_than           = (terminal('>')) == TOKEN::GREATER_THAN;
    static auto dot                    = (terminal('.')) == TOKEN::DOT;
    static auto question_mark          = (terminal('?')) == TOKEN::QUESTION_MARK;
    static auto slash                  = (terminal('/')) == TOKEN::SLASH;


    static auto token = string
                      | character
                      | identifier
                      | float_number
                      | integer_number
                      | tilde
                      | backtick
                      | exclamation_mark
                      | at_sign
                      | number_sign
                      | dollar_sign
                      | percent
                      | caret
                      | ambersand
                      | star
                      | opening_parenthesis
                      | closing_parenthesis
                      | underscore
                      | minus
                      | plus
                      | equals
                      | opening_curly_bracket
                      | opening_square_bracket
                      | closing_curly_bracket
                      | closing_square_bracket
                      | vertical_bar
                      | backslash
                      | colon
                      | semicolon
                      | double_quote
                      | single_quote
                      | less_than
                      | comma
                      | greater_than
                      | dot
                      | question_mark
                      | slash;


    static auto lexer = *(whitespace | token);


    //tokenize
    void tokenize(const std::string& input, std::vector<Token>& output, std::vector<Error>& errors) {
        //reset the output variables
        output.clear();
        errors.clear();

        //create parse context
        auto pc = parse_context(input);

        //parse 
        const bool ok = parse(lexer, pc);

        //if error, set the remaining text as not recognized
        if (!ok) {
            errors.push_back(Error{ Position{ pc.position.line(), pc.position.column() }, "syntax error" });
        }

        //convert matches to tokens
        for (const auto& match : pc.matches) {
            output.push_back(Token{ match.tag, Position{ match.begin.line(), match.begin.column() }, match.input() });
        }

    }


} //namespace cap
