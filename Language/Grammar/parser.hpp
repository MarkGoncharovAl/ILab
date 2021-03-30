#pragma once

#include <FlexLexer.h>
#include <iostream>
#include <string>
#include <vector>

#include "../Node/INode.hpp"
#include "parser.tab.hh"

namespace yy
{
    class driver_t final
    {
    public:
        enum class ErrorType { RUNTIME_UNDEFINED_NAME };

        driver_t (FlexLexer* plexer) : plexer_ (plexer) {}

        parser::token_type yylex (parser::semantic_type* yylval);
        bool parse ();

        void report_syntax_error(const parser::context & ctx) const;
        void report_runtime_error(const parser::context & ctx) const;

    private:
        FlexLexer* plexer_;

        const std::string file_name_;
        std::vector<std::string> code_lines_;

        void report_error_position(std::ostream& stream, const parser::location_type& location) const;
    };
} // namespace yy