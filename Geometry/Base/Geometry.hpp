#pragma once
#include "Surface.hpp"
#include "Triangle.hpp"

namespace MLib
{
    bool Intersect (const Triangle<double>& lhs , const Triangle<double>& rhs);
}

bool MLib::Intersect
(const Triangle<double>& lhs , const Triangle<double>& rhs)
{
    return (lhs.InterSect(rhs) && rhs.InterSect(lhs));
}