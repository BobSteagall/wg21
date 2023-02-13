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

template<class COT2>
struct matrix_operation_traits_selector<void, COT2>
{
    using traits_type = COT2;
};

template<class COT1>
struct matrix_operation_traits_selector<COT1, void>
{
    using traits_type = COT1;
};

template<class COT1>
struct matrix_operation_traits_selector<COT1, COT1>
{
    using traits_type = COT1;
};


//- Alias template interface to traits result.
//
template<class COT1, class COT2>
using select_matrix_operation_traits_t =
        typename matrix_operation_traits_selector<COT1,COT2>::traits_type;


//--------------------------------------------------------------------------------------------------
//  Alias Templates:    matrix_<op>_<level>_traits_t
//
//  These alias templates extract a given traits type from a set of template parameters.
//--------------------------------------------------------------------------------------------------
//
template<typename OT, typename MT1, typename MT2>
using matrix_addition_element_traits_t = detail::addition_element_traits_t<OT, MT1, MT2>;

template<typename OT, typename MT1, typename MT2>
using matrix_addition_layout_traits_t = detail::addition_layout_traits_t<OT, MT1, MT2>;

template<typename OT, typename MT1, typename MT2>
using matrix_addition_engine_traits_t = detail::addition_engine_traits_t<OT, MT1, MT2>;

template<typename OT, typename MT1, typename MT2>
using matrix_addition_arithmetic_traits_t = detail::addition_arithmetic_traits_t<OT, MT1, MT2>;

//------
//
template<typename OT, typename MT1, typename MT2>
using matrix_subtraction_element_traits_t = detail::subtraction_element_traits_t<OT, MT1, MT2>;

template<typename OT, typename MT1, typename MT2>
using matrix_subtraction_layout_traits_t = detail::subtraction_layout_traits_t<OT, MT1, MT2>;

template<typename OT, typename MT1, typename MT2>
using matrix_subtraction_engine_traits_t = detail::subtraction_engine_traits_t<OT, MT1, MT2>;

template<typename OT, typename MT1, typename MT2>
using matrix_subtraction_arithmetic_traits_t = detail::subtraction_arithmetic_traits_t<OT, MT1, MT2>;

//------
//
template<typename OT, typename MT1, typename MT2>
using matrix_multiplication_element_traits_t = detail::multiplication_element_traits_t<OT, MT1, MT2>;

template<typename OT, typename MT1, typename MT2>
using matrix_multiplication_layout_traits_t = detail::multiplication_layout_traits_t<OT, MT1, MT2>;

template<typename OT, typename MT1, typename MT2>
using matrix_multiplication_engine_traits_t = detail::multiplication_engine_traits_t<OT, MT1, MT2>;

template<typename OT, typename MT1, typename MT2>
using matrix_multiplication_arithmetic_traits_t = detail::multiplication_arithmetic_traits_t<OT, MT1, MT2>;

//------
//
template<typename OT, typename MT1, typename MT2>
using matrix_division_element_traits_t = detail::division_element_traits_t<OT, MT1, MT2>;

template<typename OT, typename MT1, typename MT2>
using matrix_division_layout_traits_t = detail::division_layout_traits_t<OT, MT1, MT2>;

template<typename OT, typename MT1, typename MT2>
using matrix_division_engine_traits_t = detail::division_engine_traits_t<OT, MT1, MT2>;

template<typename OT, typename MT1, typename MT2>
using matrix_division_arithmetic_traits_t = detail::division_arithmetic_traits_t<OT, MT1, MT2>;



}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_OPERATION_TRAITS_HPP_DEFINED
