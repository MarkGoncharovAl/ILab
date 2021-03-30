#include "Common_libs/Errors/Errors.hpp"
#include "Grammar/parser.hpp"

int yyFlexLexer::yywrap ()
{
  return 1;
}

int main ()
{
  FlexLexer* lexer = new yyFlexLexer;

  try
  {
    yy::driver_t driver (lexer);
    driver.parse ();
    LOG_trace << "Programm has successfully ended!";
  }
  catch (std::exception& err)
  {
    LOG_error << err.what ();
  }

  delete lexer;
}