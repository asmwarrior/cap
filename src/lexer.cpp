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


    static auto token = string
                      | character
                      | identifier
                      | float_number
                      | integer_number
        ;


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
