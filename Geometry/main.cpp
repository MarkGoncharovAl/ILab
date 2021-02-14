#include <iostream>
#include "Geometry.h"

int main()
{
	geometry::Point_t<3> point1 = { 0, 0, 2 };
	geometry::Point_t<3> point2 = { 1, 1, 0.345 };
	geometry::Point_t<3> point3 = { 0.5432, 65, 1};
	

	geometry::Surface3_t surf(point1, point2, point3);
	surf.dump(std::cout, "Hah");
	
	std::cout << geometry::ScalMult(geometry::Vector_t<3>{point1, point2}, surf.get_normal()) << "\n";
	std::cout << geometry::ScalMult(geometry::Vector_t<3>{point3, point2}, surf.get_normal()) << "\n";

	std::cout << "NEXT TEST!!!\n\n\n";

	geometry::Vector_t<3> vec = { 1, 2, 3 };
	std::cout << ScalMult(vec - vec.get_projection_on(surf.get_normal()), surf.get_normal());
}