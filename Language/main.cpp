#include "Grammar/parser.hpp"

int yyFlexLexer::yywrap() {
  return 1;
}

int main() {
    FlexLexer* lexer = new yyFlexLexer;
    yy::driver_t driver(lexer);
    driver.parse();
}