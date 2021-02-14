%option c++

%{
#include "../Grammar/parser.tab.hh"
%}

DIGIT1 [1-9]
DIGIT [0-9]

DOUBLE ([-]?{DIGIT1}{DIGIT}*(.{DIGIT}+)?)|[0]

BQ  [>][=]
LQ  [<][=]
EQ  [=][=]
NEQ [!][=]
COMPARING {BQ}|{LQ}|{EQ}|{NEQ}|[><]

ASSIGN [=]
NOT    [!]

NAME [_a-zA-Z][a-zA-Z0-9]*

%%
[ \t\n\v]+  //skip this

"while"     return yy::parser::token_type::WHILE;
"if"        return yy::parser::token_type::IF;

"print"     return yy::parser::token_type::OUTPUT;

";"         return yy::parser::token_type::SCOLON;
"{"         return yy::parser::token_type::LB_FIG;
"}"         return yy::parser::token_type::RB_FIG;
"("         return yy::parser::token_type::LB;
")"         return yy::parser::token_type::RB;

"?"         return yy::parser::token_type::INPUT;

"||"        return yy::parser::token_type::OR;
"&&"        return yy::parser::token_type::AND;

"+"         return yy::parser::token_type::PLUS;
"-"         return yy::parser::token_type::MINUS;

"*"         return yy::parser::token_type::MULT;
"/"         return yy::parser::token_type::DIV;

{DOUBLE}    return yy::parser::token_type::DOUBLE;
{NAME}      return yy::parser::token_type::NAME;

{COMPARING} return yy::parser::token_type::COMPARING;
{NOT}       return yy::parser::token_type::NOT;
{ASSIGN}    return yy::parser::token_type::ASSIGN;

.           printf("AAAAAA");
                
%%