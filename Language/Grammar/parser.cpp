#include "parser.hpp"

namespace yy
{
    parser::token_type driver_t::yylex(parser::semantic_type *yylval)
    {
        parser::token_type tt = static_cast<parser::token_type>(plexer_->yylex());

        switch (tt)
        {
        case yy::parser::token_type::DOUBLE:
            yylval->as<double>() = std::stod(std::string{plexer_->YYText()});
            break;

        case yy::parser::token_type::NAME:
            yylval->as<std::string *>() = new std::string{plexer_->YYText()};
            break;

        case yy::parser::token_type::COMPARING:
            yylval->as<INode::ComparingOp>() = INode::DefineCompType(plexer_->YYText());
            break;
        default:
            break;
        }

        return tt;
    }

    bool driver_t::parse()
    {
        parser parser(this);
        bool res = parser.parse();
        return !res;
    }
} // namespace yy