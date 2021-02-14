%language "c++"

%skeleton "lalr1.cc"
%defines
%define api.value.type variant
%param {yy::driver_t* driver}
%code requires
{
#include <string>

#include "../Node/INode.hpp"

// forward decl of argument to parser
namespace yy { class driver_t; }
}

%code
{
#include "../Grammar/parser.hpp"
INode::IScope_t* cur_scope = INode::CreateScope();
namespace yy {parser::token_type yylex(parser::semantic_type* yylval, driver_t* driver);}
}

%token <double> DOUBLE
%token <std::string*> NAME

%token <INode::ComparingOp> COMPARING

%token WHILE IF
%token SCOLON LB RB LB_FIG RB_FIG
%token OUTPUT
%right ASSIGN
%left INPUT
%left OR
%left AND
%left NOT
%left PLUS MINUS
%left MULT DIV MOD

%nterm <INode::INode_t*> stms;
%nterm <INode::INode_t*> stm;
%nterm <INode::INode_t*> scope;
%nterm <INode::INode_t*> close_sc;

%nterm <INode::INode_t*> if;
%nterm <INode::INode_t*> while;
%nterm <INode::INode_t*> output;

%nterm <INode::INode_t*> assign;
%nterm <INode::INode_t*> right_part_expr;
%nterm <INode::INode_t*> lval;

%nterm <INode::INode_t*> math_expr;
%nterm <INode::INode_t*> mult_expr;
%nterm <INode::INode_t*> brace_expr;

%nterm <INode::INode_t*> condition;
%nterm <INode::INode_t*> condition_and;
%nterm <INode::INode_t*> condition_brace;

%%

program:    
stms                                
        {cur_scope->calc();};

scope:      
open_sc stms close_sc               
        {$$ = $3;};

open_sc:    
LB_FIG                              
        {
            cur_scope = cur_scope->Duplicate();
        };

close_sc:   
RB_FIG 
        {
            $$ = cur_scope;
            cur_scope = cur_scope->Reset();
        };

stms: 
stm                                 
        {cur_scope->AddBranch($1);};
| stms stm                            
        {cur_scope->AddBranch($2);};
| stms scope                          
        {cur_scope->AddBranch($2);};




stm: 
assign                              
        {$$ = $1;};
| if                                  
        {$$ = $1;};
| while                               
        {$$ = $1;};
| output                              
        {$$ = $1;};



assign:     
lval ASSIGN right_part_expr SCOLON             
        {$$ = INode::CreateCommonOp($1, INode::CommonOp::Assign, $3);};



lval: 
NAME                                
        {$$ = cur_scope->AddName(*($1)); delete $1;};



right_part_expr:
math_expr
        {$$ = $1;}
| INPUT
        {$$ = INode::CreateCommonOp(nullptr, INode::CommonOp::Input);}



math_expr: 
mult_expr PLUS math_expr   
        {$$ = INode::CreateMathOp($1, INode::MathOp::Plus, $3);};
| mult_expr MINUS math_expr    
        {$$ = INode::CreateMathOp($1, INode::MathOp::Minus, $3);};
| mult_expr
        {$$ = $1;} 



mult_expr: 
brace_expr MULT brace_expr                       
        {$$ = INode::CreateMathOp($1, INode::MathOp::Mult, $3);};
| brace_expr DIV brace_expr                       
        {$$ = INode::CreateMathOp($1, INode::MathOp::Div, $3);};
| brace_expr
        {$$ = $1;}



brace_expr: 
LB math_expr RB
        {$$ = $2;}
| DOUBLE
        {$$ = INode::CreateDouble($1);}
| NAME
        {$$ = cur_scope->GetName(*($1));}



condition: 
condition_and OR condition_and
        {$$ = INode::CreateConditionOp($1, INode::ConditionOp::Or, $3);}
| condition_and
        {$$ = $1;}



condition_and: 
condition_brace AND condition_brace
        {$$ = $$ = INode::CreateConditionOp($1, INode::ConditionOp::And, $3);}
| condition_brace
        {$$ = $1;}



condition_brace: 
NOT LB condition RB
        {$$ = INode::CreateConditionOp($3, INode::ConditionOp::Not);}
| LB condition RB
        {$$ = $2;}
| math_expr COMPARING math_expr
        {$$ = INode::CreateCompOp($1, $2, $3);}



if: 
IF LB condition RB scope                  
        {$$ = INode::CreateIf($3, $5);};



while: 
WHILE LB condition RB scope               
        {$$ = INode::CreateWhile($3, $5);};



output: 
OUTPUT math_expr SCOLON                   
        {$$ = INode::CreateCommonOp( $2, INode::CommonOp::Output);};       

%%

namespace yy {
parser::token_type yylex(parser::semantic_type* yylval, driver_t* driver) {return driver->yylex(yylval);
}

void parser::error(const std::string& s) {std::cout << s << std::endl;}
}