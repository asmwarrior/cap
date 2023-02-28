#include <iostream>
#include "parserlib.hpp"
#include "lexer.hpp"

using namespace std;
using namespace cap;


int main() {
    const std::string testFile = parserlib::load_file("test.cap");
    std::vector<Token> tokens;
    std::vector<Error> errors;
    tokenize(testFile, tokens, errors);
    return 0;
}
