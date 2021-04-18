#include <iostream>
#include <array>
#include "MySycl/MySycl.hpp"

class vector_addition;

int main(int, char**) {
   std::array<float, 4> a = { 1.0, 2.0, 3.0, 4.0 };
   std::array<float, 4> b = { 4.0, 3.0, 2.0, 1.0 };
   std::array<float, 4> c = { 0.0, 0.0, 0.0, 0.0 };

   Msycl::MySycl sycl;
   std::cout << "Running on " << sycl.GetDevicName() << "\n";
   {
      auto&& a_sycl = Msycl::CreateLinBuf(a);
      auto&& b_sycl = Msycl::CreateLinBuf(b);
      auto&& c_sycl = Msycl::CreateLinBuf(c);
  
      sycl.submit([&] (cl::sycl::handler& cgh) {
         auto a_acc = a_sycl.get_access<cl::sycl::access::mode::read>(cgh);
         auto b_acc = b_sycl.get_access<cl::sycl::access::mode::read>(cgh);
         auto c_acc = c_sycl.get_access<cl::sycl::access::mode::discard_write>(cgh);

         cgh.parallel_for<class vector_addition>(sycl::range<1>{4}, [=] (sycl::id<1> i) {
         c_acc[i] = a_acc[i] + b_acc[i];
         });
      });
   }
   std::cout << "  A { " << a[0] << ", " << a[1] << ", " << a[2] << ", " << a[3] << " }\n"
        << "+ B { " << b[0] << ", " << b[1] << ", " << b[2] << ", " << b[3] << " }\n"
        << "------------------\n"
        << "= C { " << c[0] << ", " << c[1] << ", " << c[2] << ", " << c[3] << " }"
        << std::endl;
		
   return 0;
}