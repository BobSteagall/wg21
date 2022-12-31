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

template<typename OT, typename U, typename V>
using extract_addition_element_traits_t = typename detail::addition_element_traits_extractor<OT, U, V>::type;

template<typename OT, typename U, typename V>
using extract_addition_layout_traits_t = typename detail::addition_layout_traits_extractor<OT, U, V>::type;

template<typename OT, typename U, typename V>
using extract_addition_engine_traits_t = typename detail::addition_engine_traits_extractor<OT, U, V>::type;

template<typename OT, typename U, typename V>
using extract_addition_arithmetic_traits_t = typename detail::addition_arithmetic_traits_extractor<OT, U, V>::type;

//------
//
template<typename OT, typename U, typename V>
using extract_subtraction_element_traits_t = typename detail::subtraction_element_traits_extractor<OT, U, V>::type;

template<typename OT, typename U, typename V>
using extract_subtraction_layout_traits_t = typename detail::subtraction_layout_traits_extractor<OT, U, V>::type;

template<typename OT, typename U, typename V>
using extract_subtraction_engine_traits_t = typename detail::subtraction_engine_traits_extractor<OT, U, V>::type;

template<typename OT, typename U, typename V>
using extract_subtraction_arithmetic_traits_t = typename detail::subtraction_arithmetic_traits_extractor<OT, U, V>::type;

//------
//
template<typename OT, typename U, typename V>
using extract_multiplication_element_traits_t = typename detail::multiplication_element_traits_extractor<OT, U, V>::type;

template<typename OT, typename U, typename V>
using extract_multiplication_layout_traits_t = typename detail::multiplication_layout_traits_extractor<OT, U, V>::type;

template<typename OT, typename U, typename V>
using extract_multiplication_engine_traits_t = typename detail::multiplication_engine_traits_extractor<OT, U, V>::type;

template<typename OT, typename U, typename V>
using extract_multiplication_arithmetic_traits_t = typename detail::multiplication_arithmetic_traits_extractor<OT, U, V>::type;

//------
//
template<typename OT, typename U, typename V>
using extract_division_element_traits_t = typename detail::division_element_traits_extractor<OT, U, V>::type;

template<typename OT, typename U, typename V>
using extract_division_layout_traits_t = typename detail::division_layout_traits_extractor<OT, U, V>::type;

template<typename OT, typename U, typename V>
using extract_division_engine_traits_t = typename detail::division_engine_traits_extractor<OT, U, V>::type;

template<typename OT, typename U, typename V>
using extract_division_arithmetic_traits_t = typename detail::division_arithmetic_traits_extractor<OT, U, V>::type;


//--------------------------------------------------------------------------------------------------
//  Class:  matrix_operation_traits
//
//  This class provides a set of nested type aliases which, in turn, provide for element
//  promotion, allocator promotion, layout promotion, engine promotion, and actually perform
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

    template<class OTR, class ET1, class ET2>
    using addition_engine_traits     = detail::addition_engine_traits<OTR, ET1, ET2>;

    template<class OTR, class OP1, class OP2>
    using addition_arithmetic_traits = detail::addition_arithmetic_traits<OTR, OP1, OP2>;

    //- Subtraction
    //
    template<class OTR, class T1, class T2>
    using subtraction_element_traits    = detail::subtraction_element_traits<OTR, T1, T2>;

    template<class OTR, class ET1, class ET2>
    using subtraction_engine_traits     = detail::subtraction_engine_traits<OTR, ET1, ET2>;

    template<class OTR, class OP1, class OP2>
    using subtraction_arithmetic_traits = detail::subtraction_arithmetic_traits<OTR, OP1, OP2>;

    //- Multiplication
    //
    template<class OTR, class T1, class T2>
    using multiplication_element_traits    = detail::multiplication_element_traits<OTR, T1, T2>;

    template<class OTR, class ET1, class ET2>
    using multiplication_engine_traits     = detail::multiplication_engine_traits<OTR, ET1, ET2>;

    template<class OTR, class OP1, class OP2>
    using multiplication_arithmetic_traits = detail::multiplication_arithmetic_traits<OTR, OP1, OP2>;

    //- Division
    //
    template<class OTR, class T1, class T2>
    using division_element_traits    = detail::division_element_traits<OTR, T1, T2>;

    template<class OTR, class T1, class T2>
    using division_engine_traits     = detail::division_engine_traits<OTR, T1, T2>;

    template<class OTR, class T1, class T2>
    using division_arithmetic_traits = detail::division_arithmetic_traits<OTR, T1, T2>;
};


//--------------------------------------------------------------------------------------------------
//  Class Template:     matrix_operation_traits_selector
//
//  This class template is a customization point, which may be specialized by users, that is used
//  by the arithmetic operators to select the operation traits type to be used in computing that
//  operator's result type.
//--------------------------------------------------------------------------------------------------
//
template<class COT1, class COT2>    struct matrix_operation_traits_selector;

template<>
struct matrix_operation_traits_selector<void, void>
{
    using traits_type = void;
};

template<>
struct matrix_operation_traits_selector<void, matrix_operation_traits>
{
    using traits_type = void;
};

template<class COT2>
struct matrix_operation_traits_selector<void, COT2>
{
    using traits_type = COT2;
};

//------
//
template<>
struct matrix_operation_traits_selector<matrix_operation_traits, void>
{
    using traits_type = void;
};

template<>
struct matrix_operation_traits_selector<matrix_operation_traits, matrix_operation_traits>
{
    using traits_type = void;
};

template<class COT2>
struct matrix_operation_traits_selector<matrix_operation_traits, COT2>
{
    using traits_type = COT2;
};

//------
//
template<class COT1>
struct matrix_operation_traits_selector<COT1, void>
{
    using traits_type = COT1;
};


template<class COT1>
struct matrix_operation_traits_selector<COT1, matrix_operation_traits>
{
    using traits_type = COT1;
};

//------
//
template<class COT1>
struct matrix_operation_traits_selector<COT1, COT1>
{
    using traits_type = COT1;
};


//- Alias template interface to trait.
//
template<class COT1, class COT2>
using select_matrix_operation_traits_t =
        typename matrix_operation_traits_selector<COT1,COT2>::traits_type;


}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_OPERATION_TRAITS_HPP_DEFINED
