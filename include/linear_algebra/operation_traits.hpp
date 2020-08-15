//==================================================================================================
//  File:       operation_traits.hpp
//
//  Summary:    This header defines library's default operation traits type and a customization
//              point that is specializable by users.
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_OPERATION_TRAITS_HPP_DEFINED
#define LINEAR_ALGEBRA_OPERATION_TRAITS_HPP_DEFINED

namespace STD_LA {
namespace detail {
//--------------------------------------------------------------------------------------------------
//- Forward declarations of the default individual element/allocator/layout/engine/arithmetic
//  traits types referred to by matrix_operation_traits.  Forward declared in order to be
//  referenced in matrix_operation_traits below.
//--------------------------------------------------------------------------------------------------
//
template<class OT, class T1, class T2>      struct addition_element_traits;
template<class OT, class ET1, class ET2>    struct addition_engine_traits;
template<class OT, class OP1, class OP2>    struct addition_arithmetic_traits;

template<class OT, class T1, class T2>      struct subtraction_element_traits;
template<class OT, class ET1, class ET2>    struct subtraction_engine_traits;
template<class OT, class OP1, class OP2>    struct subtraction_arithmetic_traits;

template<class OT, class T1, class T2>      struct multiplication_element_traits;
template<class OT, class ET1, class ET2>    struct multiplication_engine_traits;
template<class OT, class OP1, class OP2>    struct multiplication_arithmetic_traits;

template<class OT, class T1, class T2>      struct division_element_traits;
template<class OT, class ET1, class ET2>    struct division_engine_traits;
template<class OT, class OP1, class OP2>    struct division_arithmetic_traits;

template<class OT, class AT1, class AT2, class T>  struct allocation_traits;

template<class OT, class LT1, class LT2>   struct addition_layout_traits;
template<class OT, class LT1, class LT2>   struct multiplication_layout_traits;


}       //- detail namespace
//==================================================================================================
//==================================================================================================
//  Class:  matrix_operation_traits
//
//  This class provides a set of nested type aliases which, in turn, provide for element
//  promotion, allocator promotion, layout promotion, engine promotion, and actual peform
//  computation for the four basic arithmetic operations (addition, subtraction, multiplication,
//  and scalar division).
//--------------------------------------------------------------------------------------------------
//
struct matrix_operation_traits
{
    //- Addition
    //
    template<class OTR, class T1, class T2>
    using addition_element_traits    = detail::addition_element_traits<OTR, T1, T2>;

    template<class OTR, class AT1, class AT2, class T>
    using addition_allocation_traits = detail::allocation_traits<OTR, AT1, AT2, T>;

    template<class OTR, class LT1, class LT2>
    using addition_layout_traits     = detail::addition_layout_traits<OTR, LT1, LT2>;

    template<class OTR, class ET1, class ET2>
    using addition_engine_traits     = detail::addition_engine_traits<OTR, ET1, ET2>;

    template<class OTR, class OP1, class OP2>
    using addition_arithmetic_traits = detail::addition_arithmetic_traits<OTR, OP1, OP2>;

    //- Subtraction
    //
    template<class OTR, class T1, class T2>
    using subtraction_element_traits    = detail::subtraction_element_traits<OTR, T1, T2>;

    template<class OTR, class AT1, class AT2, class T>
    using subtraction_allocation_traits = detail::allocation_traits<OTR, AT1, AT2, T>;

    template<class OTR, class LT1, class LT2>
    using subtraction_layout_traits     = detail::addition_layout_traits<OTR, LT1, LT2>;

    template<class OTR, class ET1, class ET2>
    using subtraction_engine_traits     = detail::subtraction_engine_traits<OTR, ET1, ET2>;

    template<class OTR, class OP1, class OP2>
    using subtraction_arithmetic_traits = detail::subtraction_arithmetic_traits<OTR, OP1, OP2>;

    //- Multiplication
    //
    template<class OTR, class T1, class T2>
    using multiplication_element_traits    = detail::multiplication_element_traits<OTR, T1, T2>;

    template<class OTR, class AT1, class AT2, class T>
    using multiplication_allocation_traits = detail::allocation_traits<OTR, AT1, AT2, T>;

    template<class OTR, class LT1, class LT2>
    using multiplication_layout_traits     = detail::multiplication_layout_traits<OTR, LT1, LT2>;

    template<class OTR, class ET1, class ET2>
    using multiplication_engine_traits     = detail::multiplication_engine_traits<OTR, ET1, ET2>;

    template<class OTR, class OP1, class OP2>
    using multiplication_arithmetic_traits = detail::multiplication_arithmetic_traits<OTR, OP1, OP2>;

    //- Division
    //
    template<class OT, class T1, class T2>
    using division_element_traits    = detail::division_element_traits<OT, T1, T2>;

    template<class OTR, class AT1, class AT2, class T>
    using division_allocation_traits = detail::allocation_traits<OTR, AT1, AT2, T>;

    template<class OTR, class LT1, class LT2>
    using division_layout_traits     = detail::addition_layout_traits<OTR, LT1, LT2>;

    template<class OTR, class T1, class T2>
    using division_engine_traits     = detail::division_engine_traits<OTR, T1, T2>;

    template<class OTR, class T1, class T2>
    using division_arithmetic_traits = detail::division_arithmetic_traits<OTR, T1, T2>;
};


//--------------------------------------------------------------------------------------------------
//  Class Template:     matrix_operation_traits
//
//  This class template is a customization point, which can be specialized by users, that is
//  used by the arithmetic operators to select the operation traits type to be used in computing
//  that operation's result type.
//--------------------------------------------------------------------------------------------------
//
template<class T1, class T2>    struct matrix_operation_traits_selector;

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

template<>
struct matrix_operation_traits_selector<void, void>
{
    using traits_type = matrix_operation_traits;
};

template<>
struct matrix_operation_traits_selector<void, matrix_operation_traits>
{
    using traits_type = matrix_operation_traits;
};

template<>
struct matrix_operation_traits_selector<matrix_operation_traits, void>
{
    using traits_type = matrix_operation_traits;
};

//- Alias template interface to trait.
//
template<class T1, class T2>
using select_matrix_operation_traits_t =
        typename matrix_operation_traits_selector<T1,T2>::traits_type;


}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_OPERATION_TRAITS_HPP_DEFINED
