//==================================================================================================
//  File:       operation_traits.hpp
//
//  Summary:    This header defines library's default operation traits type and a customization
//              point that is specializable by users.
//
//              Class matrix_operation_traits provides a set of nested type aliases which, in
//              turn, provide for element promotion, engine promotion, and computation for the
//              four basic arithmetic operations (addition, subtraction, negation, multiplication).
//
//              Class template matrix_operation_traits_selector is a customization point,
//              which can be specialized by users, that is used by the arithmetic operators
//              to select the operation traits to be used in computing that operation's result
//              type.
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_OPERATION_TRAITS_HPP_DEFINED
#define LINEAR_ALGEBRA_OPERATION_TRAITS_HPP_DEFINED

namespace STD_LA {
//==================================================================================================
//                                  **** OPERATION TRAITS ****
//==================================================================================================
//  Traits type that refers to the four basic arithmetic traits types.
//==================================================================================================
//
struct matrix_operation_traits
{
    //- Addition
    //
    template<class OTR, class T1, class T2>
    using addition_element_traits = matrix_addition_element_traits<OTR, T1, T2>;

    template<class OTR, class ET1, class ET2>
    using addition_engine_traits = matrix_addition_engine_traits<OTR, ET1, ET2>;

    template<class OTR, class OP1, class OP2>
    using addition_arithmetic_traits = matrix_addition_arithmetic_traits<OTR, OP1, OP2>;

    //- Subtraction
    //
    template<class OTR, class T1, class T2>
    using subtraction_element_traits = matrix_subtraction_element_traits<OTR, T1, T2>;

    template<class OTR, class ET1, class ET2>
    using subtraction_engine_traits = matrix_subtraction_engine_traits<OTR, ET1, ET2>;

    template<class OTR, class OP1, class OP2>
    using subtraction_arithmetic_traits = matrix_subtraction_arithmetic_traits<OTR, OP1, OP2>;

    //- Multiplication
    //
    template<class OTR, class T1, class T2>
    using multiplication_element_traits = matrix_multiplication_element_traits<OTR, T1, T2>;

    template<class OTR, class ET1, class ET2>
    using multiplication_engine_traits = matrix_multiplication_engine_traits<OTR, ET1, ET2>;

    template<class OTR, class OP1, class OP2>
    using multiplication_arithmetic_traits = matrix_multiplication_arithmetic_traits<OTR, OP1, OP2>;

    //- Division
    //
    template<class OT, class T1, class T2>
    using division_element_traits = matrix_division_element_traits<OT, T1, T2>;

    template<class OTR, class T1, class T2>
    using division_engine_traits = matrix_division_engine_traits<OTR, T1, T2>;

    template<class OTR, class T1, class T2>
    using division_arithmetic_traits = matrix_division_arithmetic_traits<OTR, T1, T2>;
};

//==================================================================================================
//                             **** OPERATION TRAITS SELECTION ****
//==================================================================================================
//  Traits type that selects the set of operator traits that is used in an arithmetic expression.
//==================================================================================================
//
template<class T1, class T2>
struct matrix_operation_traits_selector;

template<class T1>
struct matrix_operation_traits_selector<T1, T1>
{
    using traits_type = T1;
};

template<class T1>
struct matrix_operation_traits_selector<T1, matrix_operation_traits>
{
    using traits_type = T1;
};

template<class T1>
struct matrix_operation_traits_selector<matrix_operation_traits, T1>
{
    using traits_type = T1;
};

template<>
struct matrix_operation_traits_selector<matrix_operation_traits, matrix_operation_traits>
{
    using traits_type = matrix_operation_traits;
};

//- Alias template interface to trait.
//
template<class T1, class T2>
using select_matrix_operation_traits_t = typename matrix_operation_traits_selector<T1,T2>::traits_type;


}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_OPERATION_TRAITS_HPP_DEFINED
