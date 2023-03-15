#include <iostream>
#include "parserlib/util.hpp"
#include "parser.hpp"

using namespace std;
using namespace cap;


int main() {
    //load file
    const std::string testFile = parserlib::load_file("test.cap");

    //errors
    std::vector<Error> errors;

    //convert text to tokens
    std::vector<Token> tokens;
    tokenize(testFile, tokens, errors);

    //convert tokens to ast
    std::vector<ASTNodePtr> ast;
    parse(tokens, ast, errors);

    return 0;
}
