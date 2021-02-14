// A Bison parser, made by GNU Bison 3.5.1.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2020 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.

// Undocumented macros, especially those whose name start with YY_,
// are private implementation details.  Do not rely on them.





#include "parser.tab.hh"


// Unqualified %code blocks.
#line 18 "parser.y"

#include "../Grammar/parser.hpp"
INode::IScope_t* cur_scope = INode::CreateScope();
namespace yy {parser::token_type yylex(parser::semantic_type* yylval, driver_t* driver);}

#line 51 "parser.tab.cc"


#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

// Whether we are compiled with exception support.
#ifndef YY_EXCEPTIONS
# if defined __GNUC__ && !defined __EXCEPTIONS
#  define YY_EXCEPTIONS 0
# else
#  define YY_EXCEPTIONS 1
# endif
#endif



// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << '\n';                       \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yystack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YYUSE (Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void> (0)
# define YY_STACK_PRINT()                static_cast<void> (0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

namespace yy {
#line 123 "parser.tab.cc"


  /// Build a parser object.
  parser::parser (yy::driver_t* driver_yyarg)
#if YYDEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr),
#else
    :
#endif
      driver (driver_yyarg)
  {}

  parser::~parser ()
  {}

  parser::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------------.
  | Symbol types.  |
  `---------------*/

  // basic_symbol.
#if 201103L <= YY_CPLUSPLUS
  template <typename Base>
  parser::basic_symbol<Base>::basic_symbol (basic_symbol&& that)
    : Base (std::move (that))
    , value ()
  {
    switch (this->type_get ())
    {
      case 5: // COMPARING
        value.move< INode::ComparingOp > (std::move (that.value));
        break;

      case 25: // stms
      case 26: // stm
      case 27: // scope
      case 28: // close_sc
      case 29: // if
      case 30: // while
      case 31: // output
      case 32: // assign
      case 33: // right_part_expr
      case 34: // lval
      case 35: // math_expr
      case 36: // mult_expr
      case 37: // brace_expr
      case 38: // condition
      case 39: // condition_and
      case 40: // condition_brace
        value.move< INode::INode_t* > (std::move (that.value));
        break;

      case 3: // DOUBLE
        value.move< double > (std::move (that.value));
        break;

      case 4: // NAME
        value.move< std::string* > (std::move (that.value));
        break;

      default:
        break;
    }

  }
#endif

  template <typename Base>
  parser::basic_symbol<Base>::basic_symbol (const basic_symbol& that)
    : Base (that)
    , value ()
  {
    switch (this->type_get ())
    {
      case 5: // COMPARING
        value.copy< INode::ComparingOp > (YY_MOVE (that.value));
        break;

      case 25: // stms
      case 26: // stm
      case 27: // scope
      case 28: // close_sc
      case 29: // if
      case 30: // while
      case 31: // output
      case 32: // assign
      case 33: // right_part_expr
      case 34: // lval
      case 35: // math_expr
      case 36: // mult_expr
      case 37: // brace_expr
      case 38: // condition
      case 39: // condition_and
      case 40: // condition_brace
        value.copy< INode::INode_t* > (YY_MOVE (that.value));
        break;

      case 3: // DOUBLE
        value.copy< double > (YY_MOVE (that.value));
        break;

      case 4: // NAME
        value.copy< std::string* > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

  }



  template <typename Base>
  bool
  parser::basic_symbol<Base>::empty () const YY_NOEXCEPT
  {
    return Base::type_get () == empty_symbol;
  }

  template <typename Base>
  void
  parser::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move (s);
    switch (this->type_get ())
    {
      case 5: // COMPARING
        value.move< INode::ComparingOp > (YY_MOVE (s.value));
        break;

      case 25: // stms
      case 26: // stm
      case 27: // scope
      case 28: // close_sc
      case 29: // if
      case 30: // while
      case 31: // output
      case 32: // assign
      case 33: // right_part_expr
      case 34: // lval
      case 35: // math_expr
      case 36: // mult_expr
      case 37: // brace_expr
      case 38: // condition
      case 39: // condition_and
      case 40: // condition_brace
        value.move< INode::INode_t* > (YY_MOVE (s.value));
        break;

      case 3: // DOUBLE
        value.move< double > (YY_MOVE (s.value));
        break;

      case 4: // NAME
        value.move< std::string* > (YY_MOVE (s.value));
        break;

      default:
        break;
    }

  }

  // by_type.
  parser::by_type::by_type ()
    : type (empty_symbol)
  {}

#if 201103L <= YY_CPLUSPLUS
  parser::by_type::by_type (by_type&& that)
    : type (that.type)
  {
    that.clear ();
  }
#endif

  parser::by_type::by_type (const by_type& that)
    : type (that.type)
  {}

  parser::by_type::by_type (token_type t)
    : type (yytranslate_ (t))
  {}

  void
  parser::by_type::clear ()
  {
    type = empty_symbol;
  }

  void
  parser::by_type::move (by_type& that)
  {
    type = that.type;
    that.clear ();
  }

  int
  parser::by_type::type_get () const YY_NOEXCEPT
  {
    return type;
  }


  // by_state.
  parser::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

  parser::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
  parser::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
  parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  parser::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

  parser::symbol_number_type
  parser::by_state::type_get () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return empty_symbol;
    else
      return yystos_[+state];
  }

  parser::stack_symbol_type::stack_symbol_type ()
  {}

  parser::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state))
  {
    switch (that.type_get ())
    {
      case 5: // COMPARING
        value.YY_MOVE_OR_COPY< INode::ComparingOp > (YY_MOVE (that.value));
        break;

      case 25: // stms
      case 26: // stm
      case 27: // scope
      case 28: // close_sc
      case 29: // if
      case 30: // while
      case 31: // output
      case 32: // assign
      case 33: // right_part_expr
      case 34: // lval
      case 35: // math_expr
      case 36: // mult_expr
      case 37: // brace_expr
      case 38: // condition
      case 39: // condition_and
      case 40: // condition_brace
        value.YY_MOVE_OR_COPY< INode::INode_t* > (YY_MOVE (that.value));
        break;

      case 3: // DOUBLE
        value.YY_MOVE_OR_COPY< double > (YY_MOVE (that.value));
        break;

      case 4: // NAME
        value.YY_MOVE_OR_COPY< std::string* > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

#if 201103L <= YY_CPLUSPLUS
    // that is emptied.
    that.state = empty_state;
#endif
  }

  parser::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s)
  {
    switch (that.type_get ())
    {
      case 5: // COMPARING
        value.move< INode::ComparingOp > (YY_MOVE (that.value));
        break;

      case 25: // stms
      case 26: // stm
      case 27: // scope
      case 28: // close_sc
      case 29: // if
      case 30: // while
      case 31: // output
      case 32: // assign
      case 33: // right_part_expr
      case 34: // lval
      case 35: // math_expr
      case 36: // mult_expr
      case 37: // brace_expr
      case 38: // condition
      case 39: // condition_and
      case 40: // condition_brace
        value.move< INode::INode_t* > (YY_MOVE (that.value));
        break;

      case 3: // DOUBLE
        value.move< double > (YY_MOVE (that.value));
        break;

      case 4: // NAME
        value.move< std::string* > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

    // that is emptied.
    that.type = empty_symbol;
  }

#if YY_CPLUSPLUS < 201103L
  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    switch (that.type_get ())
    {
      case 5: // COMPARING
        value.copy< INode::ComparingOp > (that.value);
        break;

      case 25: // stms
      case 26: // stm
      case 27: // scope
      case 28: // close_sc
      case 29: // if
      case 30: // while
      case 31: // output
      case 32: // assign
      case 33: // right_part_expr
      case 34: // lval
      case 35: // math_expr
      case 36: // mult_expr
      case 37: // brace_expr
      case 38: // condition
      case 39: // condition_and
      case 40: // condition_brace
        value.copy< INode::INode_t* > (that.value);
        break;

      case 3: // DOUBLE
        value.copy< double > (that.value);
        break;

      case 4: // NAME
        value.copy< std::string* > (that.value);
        break;

      default:
        break;
    }

    return *this;
  }

  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    switch (that.type_get ())
    {
      case 5: // COMPARING
        value.move< INode::ComparingOp > (that.value);
        break;

      case 25: // stms
      case 26: // stm
      case 27: // scope
      case 28: // close_sc
      case 29: // if
      case 30: // while
      case 31: // output
      case 32: // assign
      case 33: // right_part_expr
      case 34: // lval
      case 35: // math_expr
      case 36: // mult_expr
      case 37: // brace_expr
      case 38: // condition
      case 39: // condition_and
      case 40: // condition_brace
        value.move< INode::INode_t* > (that.value);
        break;

      case 3: // DOUBLE
        value.move< double > (that.value);
        break;

      case 4: // NAME
        value.move< std::string* > (that.value);
        break;

      default:
        break;
    }

    // that is emptied.
    that.state = empty_state;
    return *this;
  }
#endif

  template <typename Base>
  void
  parser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
  parser::yy_print_ (std::ostream& yyo,
                                     const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YYUSE (yyoutput);
    symbol_number_type yytype = yysym.type_get ();
#if defined __GNUC__ && ! defined __clang__ && ! defined __ICC && __GNUC__ * 100 + __GNUC_MINOR__ <= 408
    // Avoid a (spurious) G++ 4.8 warning about "array subscript is
    // below array bounds".
    if (yysym.empty ())
      std::abort ();
#endif
    yyo << (yytype < yyntokens_ ? "token" : "nterm")
        << ' ' << yytname_[yytype] << " (";
    YYUSE (yytype);
    yyo << ')';
  }
#endif

  void
  parser::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  parser::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  parser::yypop_ (int n)
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  parser::debug_level_type
  parser::debug_level () const
  {
    return yydebug_;
  }

  void
  parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  parser::state_type
  parser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - yyntokens_] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - yyntokens_];
  }

  bool
  parser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  parser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  parser::operator() ()
  {
    return parse ();
  }

  int
  parser::parse ()
  {
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The return value of parse ().
    int yyresult;

#if YY_EXCEPTIONS
    try
#endif // YY_EXCEPTIONS
      {
    YYCDEBUG << "Starting parse\n";


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, YY_MOVE (yyla));

  /*-----------------------------------------------.
  | yynewstate -- push a new symbol on the stack.  |
  `-----------------------------------------------*/
  yynewstate:
    YYCDEBUG << "Entering state " << int (yystack_[0].state) << '\n';

    // Accept?
    if (yystack_[0].state == yyfinal_)
      YYACCEPT;

    goto yybackup;


  /*-----------.
  | yybackup.  |
  `-----------*/
  yybackup:
    // Try to take a decision without lookahead.
    yyn = yypact_[+yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token: ";
#if YY_EXCEPTIONS
        try
#endif // YY_EXCEPTIONS
          {
            yyla.type = yytranslate_ (yylex (&yyla.value, driver));
          }
#if YY_EXCEPTIONS
        catch (const syntax_error& yyexc)
          {
            YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
            error (yyexc);
            goto yyerrlab1;
          }
#endif // YY_EXCEPTIONS
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.type_get ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.type_get ())
      {
        goto yydefault;
      }

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", state_type (yyn), YY_MOVE (yyla));
    goto yynewstate;


  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[+yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;


  /*-----------------------------.
  | yyreduce -- do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_ (yystack_[yylen].state, yyr1_[yyn]);
      /* Variants are always initialized to an empty instance of the
         correct type. The default '$$ = $1' action is NOT applied
         when using variants.  */
      switch (yyr1_[yyn])
    {
      case 5: // COMPARING
        yylhs.value.emplace< INode::ComparingOp > ();
        break;

      case 25: // stms
      case 26: // stm
      case 27: // scope
      case 28: // close_sc
      case 29: // if
      case 30: // while
      case 31: // output
      case 32: // assign
      case 33: // right_part_expr
      case 34: // lval
      case 35: // math_expr
      case 36: // mult_expr
      case 37: // brace_expr
      case 38: // condition
      case 39: // condition_and
      case 40: // condition_brace
        yylhs.value.emplace< INode::INode_t* > ();
        break;

      case 3: // DOUBLE
        yylhs.value.emplace< double > ();
        break;

      case 4: // NAME
        yylhs.value.emplace< std::string* > ();
        break;

      default:
        break;
    }



      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
#if YY_EXCEPTIONS
      try
#endif // YY_EXCEPTIONS
        {
          switch (yyn)
            {
  case 2:
#line 65 "parser.y"
        {cur_scope->calc();}
#line 834 "parser.tab.cc"
    break;

  case 3:
#line 69 "parser.y"
        {yylhs.value.as < INode::INode_t* > () = yystack_[0].value.as < INode::INode_t* > ();}
#line 840 "parser.tab.cc"
    break;

  case 4:
#line 73 "parser.y"
        {
            cur_scope = cur_scope->Duplicate();
        }
#line 848 "parser.tab.cc"
    break;

  case 5:
#line 79 "parser.y"
        {
            yylhs.value.as < INode::INode_t* > () = cur_scope;
            cur_scope = cur_scope->Reset();
        }
#line 857 "parser.tab.cc"
    break;

  case 6:
#line 86 "parser.y"
        {cur_scope->AddBranch(yystack_[0].value.as < INode::INode_t* > ());}
#line 863 "parser.tab.cc"
    break;

  case 7:
#line 88 "parser.y"
        {cur_scope->AddBranch(yystack_[0].value.as < INode::INode_t* > ());}
#line 869 "parser.tab.cc"
    break;

  case 8:
#line 90 "parser.y"
        {cur_scope->AddBranch(yystack_[0].value.as < INode::INode_t* > ());}
#line 875 "parser.tab.cc"
    break;

  case 9:
#line 97 "parser.y"
        {yylhs.value.as < INode::INode_t* > () = yystack_[0].value.as < INode::INode_t* > ();}
#line 881 "parser.tab.cc"
    break;

  case 10:
#line 99 "parser.y"
        {yylhs.value.as < INode::INode_t* > () = yystack_[0].value.as < INode::INode_t* > ();}
#line 887 "parser.tab.cc"
    break;

  case 11:
#line 101 "parser.y"
        {yylhs.value.as < INode::INode_t* > () = yystack_[0].value.as < INode::INode_t* > ();}
#line 893 "parser.tab.cc"
    break;

  case 12:
#line 103 "parser.y"
        {yylhs.value.as < INode::INode_t* > () = yystack_[0].value.as < INode::INode_t* > ();}
#line 899 "parser.tab.cc"
    break;

  case 13:
#line 109 "parser.y"
        {yylhs.value.as < INode::INode_t* > () = INode::CreateCommonOp(yystack_[3].value.as < INode::INode_t* > (), INode::CommonOp::Assign, yystack_[1].value.as < INode::INode_t* > ());}
#line 905 "parser.tab.cc"
    break;

  case 14:
#line 115 "parser.y"
        {yylhs.value.as < INode::INode_t* > () = cur_scope->AddName(*(yystack_[0].value.as < std::string* > ())); delete yystack_[0].value.as < std::string* > ();}
#line 911 "parser.tab.cc"
    break;

  case 15:
#line 121 "parser.y"
        {yylhs.value.as < INode::INode_t* > () = yystack_[0].value.as < INode::INode_t* > ();}
#line 917 "parser.tab.cc"
    break;

  case 16:
#line 123 "parser.y"
        {yylhs.value.as < INode::INode_t* > () = INode::CreateCommonOp(nullptr, INode::CommonOp::Input);}
#line 923 "parser.tab.cc"
    break;

  case 17:
#line 129 "parser.y"
        {yylhs.value.as < INode::INode_t* > () = INode::CreateMathOp(yystack_[2].value.as < INode::INode_t* > (), INode::MathOp::Plus, yystack_[0].value.as < INode::INode_t* > ());}
#line 929 "parser.tab.cc"
    break;

  case 18:
#line 131 "parser.y"
        {yylhs.value.as < INode::INode_t* > () = INode::CreateMathOp(yystack_[2].value.as < INode::INode_t* > (), INode::MathOp::Minus, yystack_[0].value.as < INode::INode_t* > ());}
#line 935 "parser.tab.cc"
    break;

  case 19:
#line 133 "parser.y"
        {yylhs.value.as < INode::INode_t* > () = yystack_[0].value.as < INode::INode_t* > ();}
#line 941 "parser.tab.cc"
    break;

  case 20:
#line 139 "parser.y"
        {yylhs.value.as < INode::INode_t* > () = INode::CreateMathOp(yystack_[2].value.as < INode::INode_t* > (), INode::MathOp::Mult, yystack_[0].value.as < INode::INode_t* > ());}
#line 947 "parser.tab.cc"
    break;

  case 21:
#line 141 "parser.y"
        {yylhs.value.as < INode::INode_t* > () = INode::CreateMathOp(yystack_[2].value.as < INode::INode_t* > (), INode::MathOp::Div, yystack_[0].value.as < INode::INode_t* > ());}
#line 953 "parser.tab.cc"
    break;

  case 22:
#line 143 "parser.y"
        {yylhs.value.as < INode::INode_t* > () = yystack_[0].value.as < INode::INode_t* > ();}
#line 959 "parser.tab.cc"
    break;

  case 23:
#line 149 "parser.y"
        {yylhs.value.as < INode::INode_t* > () = yystack_[1].value.as < INode::INode_t* > ();}
#line 965 "parser.tab.cc"
    break;

  case 24:
#line 151 "parser.y"
        {yylhs.value.as < INode::INode_t* > () = INode::CreateDouble(yystack_[0].value.as < double > ());}
#line 971 "parser.tab.cc"
    break;

  case 25:
#line 153 "parser.y"
        {yylhs.value.as < INode::INode_t* > () = cur_scope->GetName(*(yystack_[0].value.as < std::string* > ()));}
#line 977 "parser.tab.cc"
    break;

  case 26:
#line 159 "parser.y"
        {yylhs.value.as < INode::INode_t* > () = INode::CreateConditionOp(yystack_[2].value.as < INode::INode_t* > (), INode::ConditionOp::Or, yystack_[0].value.as < INode::INode_t* > ());}
#line 983 "parser.tab.cc"
    break;

  case 27:
#line 161 "parser.y"
        {yylhs.value.as < INode::INode_t* > () = yystack_[0].value.as < INode::INode_t* > ();}
#line 989 "parser.tab.cc"
    break;

  case 28:
#line 167 "parser.y"
        {yylhs.value.as < INode::INode_t* > () = yylhs.value.as < INode::INode_t* > () = INode::CreateConditionOp(yystack_[2].value.as < INode::INode_t* > (), INode::ConditionOp::And, yystack_[0].value.as < INode::INode_t* > ());}
#line 995 "parser.tab.cc"
    break;

  case 29:
#line 169 "parser.y"
        {yylhs.value.as < INode::INode_t* > () = yystack_[0].value.as < INode::INode_t* > ();}
#line 1001 "parser.tab.cc"
    break;

  case 30:
#line 175 "parser.y"
        {yylhs.value.as < INode::INode_t* > () = INode::CreateConditionOp(yystack_[1].value.as < INode::INode_t* > (), INode::ConditionOp::Not);}
#line 1007 "parser.tab.cc"
    break;

  case 31:
#line 177 "parser.y"
        {yylhs.value.as < INode::INode_t* > () = yystack_[1].value.as < INode::INode_t* > ();}
#line 1013 "parser.tab.cc"
    break;

  case 32:
#line 179 "parser.y"
        {yylhs.value.as < INode::INode_t* > () = INode::CreateCompOp(yystack_[2].value.as < INode::INode_t* > (), yystack_[1].value.as < INode::ComparingOp > (), yystack_[0].value.as < INode::INode_t* > ());}
#line 1019 "parser.tab.cc"
    break;

  case 33:
#line 185 "parser.y"
        {yylhs.value.as < INode::INode_t* > () = INode::CreateIf(yystack_[2].value.as < INode::INode_t* > (), yystack_[0].value.as < INode::INode_t* > ());}
#line 1025 "parser.tab.cc"
    break;

  case 34:
#line 191 "parser.y"
        {yylhs.value.as < INode::INode_t* > () = INode::CreateWhile(yystack_[2].value.as < INode::INode_t* > (), yystack_[0].value.as < INode::INode_t* > ());}
#line 1031 "parser.tab.cc"
    break;

  case 35:
#line 197 "parser.y"
        {yylhs.value.as < INode::INode_t* > () = INode::CreateCommonOp( yystack_[1].value.as < INode::INode_t* > (), INode::CommonOp::Output);}
#line 1037 "parser.tab.cc"
    break;


#line 1041 "parser.tab.cc"

            default:
              break;
            }
        }
#if YY_EXCEPTIONS
      catch (const syntax_error& yyexc)
        {
          YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
          error (yyexc);
          YYERROR;
        }
#endif // YY_EXCEPTIONS
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;
      YY_STACK_PRINT ();

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, YY_MOVE (yylhs));
    }
    goto yynewstate;


  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        error (yysyntax_error_ (yystack_[0].state, yyla));
      }


    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.type_get () == yyeof_)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:
    /* Pacify compilers when the user code never invokes YYERROR and
       the label yyerrorlab therefore never appears in user code.  */
    if (false)
      YYERROR;

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    goto yyerrlab1;


  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    {
      stack_symbol_type error_token;
      for (;;)
        {
          yyn = yypact_[+yystack_[0].state];
          if (!yy_pact_value_is_default_ (yyn))
            {
              yyn += yy_error_token_;
              if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yy_error_token_)
                {
                  yyn = yytable_[yyn];
                  if (0 < yyn)
                    break;
                }
            }

          // Pop the current state because it cannot handle the error token.
          if (yystack_.size () == 1)
            YYABORT;

          yy_destroy_ ("Error: popping", yystack_[0]);
          yypop_ ();
          YY_STACK_PRINT ();
        }


      // Shift the error token.
      error_token.state = state_type (yyn);
      yypush_ ("Shifting", YY_MOVE (error_token));
    }
    goto yynewstate;


  /*-------------------------------------.
  | yyacceptlab -- YYACCEPT comes here.  |
  `-------------------------------------*/
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;


  /*-----------------------------------.
  | yyabortlab -- YYABORT comes here.  |
  `-----------------------------------*/
  yyabortlab:
    yyresult = 1;
    goto yyreturn;


  /*-----------------------------------------------------.
  | yyreturn -- parsing is finished, return the result.  |
  `-----------------------------------------------------*/
  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
#if YY_EXCEPTIONS
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack\n";
        // Do not try to display the values of the reclaimed symbols,
        // as their printers might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
#endif // YY_EXCEPTIONS
  }

  void
  parser::error (const syntax_error& yyexc)
  {
    error (yyexc.what ());
  }

  // Generate an error message.
  std::string
  parser::yysyntax_error_ (state_type, const symbol_type&) const
  {
    return YY_("syntax error");
  }


  const signed char parser::yypact_ninf_ = -13;

  const signed char parser::yytable_ninf_ = -1;

  const signed char
  parser::yypact_[] =
  {
      42,   -13,    -8,     2,     5,    34,   -13,   -13,   -13,   -13,
     -13,    20,    17,     1,     1,   -13,   -13,     5,    36,     7,
      29,   -13,   -13,   -13,    42,     3,   -13,     1,    19,    37,
      44,    40,    41,    47,    49,   -13,     5,     5,     5,     5,
      18,   -13,    52,   -13,    10,    51,     1,     5,    53,     1,
       1,    53,   -13,   -13,   -13,   -13,   -13,   -13,   -13,   -13,
     -13,    55,   -13,   -13,   -13,   -13,   -13,   -13
  };

  const signed char
  parser::yydefact_[] =
  {
       0,    14,     0,     0,     0,     2,     6,    10,    11,    12,
       9,     0,     0,     0,     0,    24,    25,     0,     0,    19,
      22,     4,     7,     8,     0,     0,     1,     0,     0,     0,
       0,    27,    29,     0,     0,    35,     0,     0,     0,     0,
       0,    16,     0,    15,     0,     0,     0,     0,     0,     0,
       0,     0,    23,    17,    18,    20,    21,     5,     3,    13,
      31,     0,    32,    34,    26,    28,    33,    30
  };

  const signed char
  parser::yypgoto_[] =
  {
     -13,    38,    -3,   -12,   -13,   -13,   -13,   -13,   -13,   -13,
     -13,    -4,   -13,    14,   -11,    21,    13,   -13,   -13
  };

  const signed char
  parser::yydefgoto_[] =
  {
      -1,     5,     6,    23,    58,     7,     8,     9,    10,    42,
      11,    29,    19,    20,    30,    31,    32,    12,    24
  };

  const signed char
  parser::yytable_[] =
  {
      18,    13,    22,    33,    15,    16,    15,    16,    15,    16,
      27,    14,    17,    34,    17,    47,    45,    26,    41,    28,
      52,    43,     1,    44,     2,     3,    36,    37,    46,    21,
      57,     4,    53,    54,    25,    61,    63,    22,     1,    66,
       2,     3,    47,    62,    35,    21,     1,     4,     2,     3,
      38,    39,    55,    56,    48,     4,    49,    51,    50,    52,
      59,    60,    40,    65,    21,    67,     0,     0,     0,     0,
      64
  };

  const signed char
  parser::yycheck_[] =
  {
       4,     9,     5,    14,     3,     4,     3,     4,     3,     4,
       9,     9,     9,    17,     9,     5,    27,     0,    15,    18,
      10,    25,     4,    27,     6,     7,    19,    20,     9,    11,
      12,    13,    36,    37,    14,    46,    48,    40,     4,    51,
       6,     7,     5,    47,     8,    11,     4,    13,     6,     7,
      21,    22,    38,    39,    10,    13,    16,    10,    17,    10,
       8,    10,    24,    50,    11,    10,    -1,    -1,    -1,    -1,
      49
  };

  const signed char
  parser::yystos_[] =
  {
       0,     4,     6,     7,    13,    25,    26,    29,    30,    31,
      32,    34,    41,     9,     9,     3,     4,     9,    35,    36,
      37,    11,    26,    27,    42,    14,     0,     9,    18,    35,
      38,    39,    40,    38,    35,     8,    19,    20,    21,    22,
      25,    15,    33,    35,    35,    38,     9,     5,    10,    16,
      17,    10,    10,    35,    35,    37,    37,    12,    28,     8,
      10,    38,    35,    27,    39,    40,    27,    10
  };

  const signed char
  parser::yyr1_[] =
  {
       0,    24,    41,    27,    42,    28,    25,    25,    25,    26,
      26,    26,    26,    32,    34,    33,    33,    35,    35,    35,
      36,    36,    36,    37,    37,    37,    38,    38,    39,    39,
      40,    40,    40,    29,    30,    31
  };

  const signed char
  parser::yyr2_[] =
  {
       0,     2,     1,     3,     1,     1,     1,     2,     2,     1,
       1,     1,     1,     4,     1,     1,     1,     3,     3,     1,
       3,     3,     1,     3,     1,     1,     3,     1,     3,     1,
       4,     3,     3,     5,     5,     3
  };


#if YYDEBUG
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const parser::yytname_[] =
  {
  "$end", "error", "$undefined", "DOUBLE", "NAME", "COMPARING", "WHILE",
  "IF", "SCOLON", "LB", "RB", "LB_FIG", "RB_FIG", "OUTPUT", "ASSIGN",
  "INPUT", "OR", "AND", "NOT", "PLUS", "MINUS", "MULT", "DIV", "MOD",
  "$accept", "stms", "stm", "scope", "close_sc", "if", "while", "output",
  "assign", "right_part_expr", "lval", "math_expr", "mult_expr",
  "brace_expr", "condition", "condition_and", "condition_brace", "program",
  "open_sc", YY_NULLPTR
  };


  const unsigned char
  parser::yyrline_[] =
  {
       0,    64,    64,    68,    72,    78,    85,    87,    89,    96,
      98,   100,   102,   108,   114,   120,   122,   128,   130,   132,
     138,   140,   142,   148,   150,   152,   158,   160,   166,   168,
     174,   176,   178,   184,   190,   196
  };

  // Print the state stack on the debug stream.
  void
  parser::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << int (i->state);
    *yycdebug_ << '\n';
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  parser::yy_reduce_print_ (int yyrule)
  {
    int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):\n";
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG

  parser::token_number_type
  parser::yytranslate_ (int t)
  {
    // YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to
    // TOKEN-NUM as returned by yylex.
    static
    const token_number_type
    translate_table[] =
    {
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23
    };
    const int user_token_number_max_ = 278;

    if (t <= 0)
      return yyeof_;
    else if (t <= user_token_number_max_)
      return translate_table[t];
    else
      return yy_undef_token_;
  }

} // yy
#line 1420 "parser.tab.cc"

#line 199 "parser.y"


namespace yy {
parser::token_type yylex(parser::semantic_type* yylval, driver_t* driver) {return driver->yylex(yylval);
}

void parser::error(const std::string& s) {std::cout << s << std::endl;}
}
