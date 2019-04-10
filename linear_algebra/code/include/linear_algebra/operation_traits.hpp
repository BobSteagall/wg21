#ifndef LINEAR_ALGEBRA_OPERATION_TRAITS_HPP_DEFINED
#define LINEAR_ALGEBRA_OPERATION_TRAITS_HPP_DEFINED

namespace STD_LA {
//=================================================================================================
//                                  **** OPERATION TRAITS ****
//=================================================================================================
//  Traits type that refers to the four basic arithmetic traits types.
//=================================================================================================
//
struct default_matrix_operation_traits
{
    template<class T1, class T2>
    using element_promotion_traits = matrix_element_promotion<T1, T2>;

    //- Default element promotion traits.
    //
    template<class T1>
    using element_negation_traits = matrix_element_negation_traits<T1>;

    template<class T1, class T2>
    using element_addition_traits = matrix_element_addition_traits<T1, T2>;

    template<class T1, class T2>
    using element_subtraction_traits = matrix_element_subtraction_traits<T1, T2>;

    template<class T1, class T2>
    using element_multiplication_traits = matrix_element_multiplication_traits<T1, T2>;

    //- Default engine promotion traits.
    //
    template<class ET1>
    using engine_negation_traits = matrix_engine_negation_traits<ET1>;

    template<class OTR, class ET1, class ET2>
    using engine_addition_traits = matrix_engine_addition_traits<OTR, ET1, ET2>;

    template<class ET1, class ET2>
    using engine_subtraction_traits = matrix_engine_subtraction_traits<ET1, ET2>;

    template<class ET1, class ET2>
    using engine_multiplication_traits = matrix_engine_multiplication_traits<ET1, ET2>;

    //- Default arithmetic operation traits.
    //
    template<class OP1, class OTR>
    using negation_traits = matrix_negation_traits<OP1, OTR>;

    template<class OTR, class OP1, class OP2>
    using addition_traits = matrix_addition_traits<OTR, OP1, OP2>;

    template<class OP1, class OP2, class OTR>
    using subtraction_traits = matrix_subtraction_traits<OP1, OP2, OTR>;

    template<class OP1, class OP2, class OTR>
    using multiplication_traits = matrix_multiplication_traits<OP1, OP2, OTR>;
};

//=================================================================================================
//                             **** OPERATION TRAITS SELECTION ****
//=================================================================================================
//  Traits type that selects the set of operator traits to use in an expression.
//=================================================================================================
//
template<class T1, class T2>
struct matrix_operation_traits_selector;

template<class T1>
struct matrix_operation_traits_selector<T1, T1>
{
    using traits_type = T1;
};

template<class T1>
struct matrix_operation_traits_selector<T1, default_matrix_operation_traits>
{
    using traits_type = T1;
};

template<class T1>
struct matrix_operation_traits_selector<default_matrix_operation_traits, T1>
{
    using traits_type = T1;
};

template<>
struct matrix_operation_traits_selector<default_matrix_operation_traits, default_matrix_operation_traits>
{
    using traits_type = default_matrix_operation_traits;
};

template<class T1>
struct matrix_operation_traits_selector<T1, default_matrix_operations>
{
    using traits_type = T1;
};

template<class T1>
struct matrix_operation_traits_selector<default_matrix_operations, T1>
{
    using traits_type = T1;
};

template<>
struct matrix_operation_traits_selector<default_matrix_operations, default_matrix_operations>
{
    using traits_type = default_matrix_operations;
};

//- Alias interface to trait.
//
template<class T1, class T2>
using matrix_operation_traits_selector_t = typename matrix_operation_traits_selector<T1,T2>::traits_type;

//------
//
/*
template<class T1, class T2, class DEF>
struct non_void_traits_chooser;

template<class T1, class DEF>
struct non_void_traits_chooser<T1, void, DEF>
{
    using type = T1;
};

template<class T2, class DEF>
struct non_void_traits_chooser<void, T2, DEF>
{
    using type = T2;
};

template<class DEF>
struct non_void_traits_chooser<void, void, DEF>
{
    using type = DEF;
};

//------
//
template<typename OT, typename = void>
struct extract_element_addition_traits_stf : public std::false_type
{
    using type = void;
};

template<typename OT>
struct extract_element_addition_traits_stf<OT, void_t<decltype(std::declval<typename OT::element_addition_traits>())>>  : public std::true_type
{
    using type = typename OT::element_addition_traits;
};


template<typename OT, typename T1, typename T2, typename = void>
struct extract_element_addition_traits_mtf : public std::false_type
{
    using type = void;
};

template<typename OT, typename T1, typename T2>
struct extract_element_addition_traits_mtf<OT, T1, T2, void_t<decltype(std::declval<typename OT::template element_addition_traits<T1, T2>>())>> : public std::true_type
{
    using type = typename OT::template element_addition_traits<T1, T2>;
};


template<template<typename, typename> typename OT, typename T1, typename T2, typename = void>
struct extract_element_addition_traits_ttf : public std::false_type
{
    using type = void;
};

template<template<typename, typename> typename OT, typename T1, typename T2>
struct extract_element_addition_traits_ttf<OT, T1, T2, void_t<decltype(std::declval<typename OT<T1, T2>::element_addition_traits>())>> : public std::true_type
{
    using type = typename OT<T1, T2>::element_addition_traits;
};


template<typename OT, typename T1, typename T2>
struct extract_element_addition_traits
{
    using CT1 = typename extract_element_addition_traits_stf<OT>::type;
    using CT2 = typename extract_element_addition_traits_mtf<OT, T1, T2>::type;
    using DEF = matrix_element_addition_traits<T1, T2>;

    using type = typename non_void_traits_chooser<CT1, CT2, DEF>::type;
};

template<typename OT, typename T1, typename T2>
using extract_element_addition_traits_t = typename extract_element_addition_traits<OT, T1, T2>::type;


//------
//
template<typename OT, typename = void>
struct extract_engine_addition_traits_stf
{
    using type = void;
};

template<typename OT>
struct extract_engine_addition_traits_stf<OT, void_t<decltype(std::declval<typename OT::engine_addition_traits>())>>
{
    using type = typename OT::engine_addition_traits;
};


template<typename OT, typename ET1, typename ET2, typename = void>
struct extract_engine_addition_traits_mtf
{
    using type = void;
};

template<typename OT, typename ET1, typename ET2>
struct extract_engine_addition_traits_mtf<OT, ET1, ET2, void_t<decltype(std::declval<typename OT::template engine_addition_traits<ET1, ET2>>())>>
{
    using type = typename OT::template engine_addition_traits<ET1, ET2>;
};


template<typename OT, typename ET1, typename ET2>
struct extract_engine_addition_traits
{
    using CET1 = typename extract_engine_addition_traits_stf<OT>::type;
    using CET2 = typename extract_engine_addition_traits_mtf<OT, ET1, ET2>::type;
    using DEF  = matrix_engine_addition_traits<OT, ET1, ET2>;

    using type = typename non_void_traits_chooser<CET1, CET2, DEF>::type;
};

template<typename OT, typename ET1, typename ET2>
using extract_engine_addition_traits_t = typename extract_engine_addition_traits<OT, ET1, ET2>::type;


//------
//
template<typename OT, typename = void>
struct extract_addition_traits_stf
{
    using type = void;
};

template<typename OT>
struct extract_addition_traits_stf<OT, void_t<decltype(std::declval<typename OT::addition_traits>())>>
{
    using type = typename OT::addition_traits;
};


template<typename OT, typename OP1, typename OP2, typename = void>
struct extract_addition_traits_mtf
{
    using type = void;
};

template<typename OT, typename OP1, typename OP2>
struct extract_addition_traits_mtf<OT, OP1, OP2, void_t<decltype(std::declval<typename OT::template addition_traits<OP1, OP2>>())>>
{
    using type = typename OT::template addition_traits<OP1, OP2>;
};


template<typename OT, typename OP1, typename OP2>
struct extract_addition_traits
{
    using CT1 = typename extract_addition_traits_stf<OT>::type;
    using CT2 = typename extract_addition_traits_mtf<OT, OP1, OP2>::type;
    using DEF  = matrix_addition_traits<OT, OP1, OP2>;

    using type = typename non_void_traits_chooser<CT1, CT2, DEF>::type;
};

template<typename OT, typename OP1, typename OP2>
using extract_addition_traits_t = typename extract_addition_traits<OT, OP1, OP2>::type;

*/
#if 0

given operator traits types OT1 and OT2:

if OT1 and OT2 are both default
    result = default
else if OT1 == OT2
    result = OT2
else if exactly OT1 == default or OT2 == default
    result = default | OT
else
    result = user-supplied operator traits selector chooses

#endif

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_OPERATION_TRAITS_HPP_DEFINED
