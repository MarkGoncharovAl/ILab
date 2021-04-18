/*************************************************************************
 *
 * Matrix_other.cpp
 *
 * Created by dmitry
 * 22.02.2021
 *
 ***/

#include "Matrix.h"

#define orCase(a) \
case matrix::Order::a: \
res = #a; \
break;

namespace matrix
{

	std::string toString(Order order_)
	{
		std::string res;

		switch (order_)
		{
			orCase(Row);
			orCase(Column);
		}

		return res;
	}


	std::ostream& operator << (std::ostream& stream_, matrix::Order order_)
	{
		return stream_ << toString(order_);
	}

}//namespace matrix