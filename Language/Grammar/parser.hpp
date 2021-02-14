#pragma once

#include <FlexLexer.h>
#include <iostream>
#include <string>

#include "../Node/INode.hpp"
#include "parser.tab.hh"

namespace yy
{
    class driver_t final
    {
    public:
        driver_t(FlexLexer *plexer) : plexer_(plexer) {}

        parser::token_type yylex(parser::semantic_type *yylval);
        bool parse();

    private:
        FlexLexer *plexer_;
    };
} // namespace yy