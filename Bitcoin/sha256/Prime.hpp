#pragma once
#include <iostream>
#include <array>
#include <cmath>

namespace MLib
{
    //inner realization
    //using is below
    namespace PrivatePrime
    {
        template<size_t num , size_t del>
        struct CheckIsPrime
        {
            static constexpr bool
                value = (num % del != 0)
                && CheckIsPrime<num , del - 1>::value;
        };

        template<size_t num>
        struct CheckIsPrime<num , 2>
        {
            static constexpr bool
                value = (num % 2 != 0);
        };

        template<size_t num>
        struct IsPrime
        {
            static constexpr bool
                value = CheckIsPrime < num , num / 2 >::value;
        };

        template<>
        struct IsPrime<0>
        {
            static constexpr bool value = false;
        };
        template<>
        struct IsPrime<1>
        {
            static constexpr bool value = false;
        };
        template<>
        struct IsPrime<2>
        {
            static constexpr bool value = true;
        };
        template<>
        struct IsPrime<3>
        {
            static constexpr bool value = true;
        };

        template<size_t start_size , size_t size = start_size , size_t number = 2 , bool prime = true , size_t... numbers>
        struct PrimeArray;

        template<size_t start_size , size_t size , size_t number , size_t... numbers>
        struct PrimeArray<start_size , size , number , true , numbers...> :
            PrimeArray<start_size , size - 1 , number + 1 , IsPrime<number + 1>::value , numbers... , number>
        {
        };

        template<size_t start_size , size_t size , size_t number , size_t... numbers>
        struct PrimeArray<start_size , size , number , false , numbers...> :
            PrimeArray<start_size , size , number + 1 , IsPrime<number + 1>::value , numbers...>
        {
        };

        template<size_t start_size , size_t number , size_t... numbers>
        struct PrimeArray<start_size , 0 , number , true , numbers...>
        {
            static constexpr std::array<size_t , start_size> value { numbers... };
        };
    }

    //USING
    template<size_t num>
    constexpr bool isPrime_v = PrivatePrime::IsPrime<num>::value;
    template <size_t num>
    constexpr std::array<size_t , num> PrimeArray_v = PrivatePrime::PrimeArray<num>::value;

    /*
    Example:
    for(int i : MLib::PrimeArray_v<20>){
        std::cout << i << std::endl;
    }
    */
}