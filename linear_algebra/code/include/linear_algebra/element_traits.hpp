#ifndef LINEAR_ALGEBRA_ELEMENT_TRAITS_HPP_DEFINED
#define LINEAR_ALGEBRA_ELEMENT_TRAITS_HPP_DEFINED

namespace STD_LA {
namespace detail {
//--------------------------------------------------------------------------------------------------
//- Helper traits for performing element type promotion.
//--------------------------------------------------------------------------------------------------
//
template<class T1, class T2>
struct matrix_element_promotion_helper
{
    static_assert(is_arithmetic_v<T1> && is_arithmetic_v<T2>);
    using type = decltype(T1() * T2());
};

template<class T1, class T2>
using matrix_element_promotion_helper_t = typename matrix_element_promotion_helper<T1, T2>::type;

}   //- detail namespace


template<class T1>
struct matrix_element_negation_traits
{
    using type = decltype(-declval<T1>());
};

template<class T1, class T2>
struct matrix_element_addition_traits
{
    using traits_category = matrix_element_addition_traits_tag;
    using result_type     = decltype(declval<T1>() + declval<T2>());
};

template<class T1, class T2>
struct matrix_element_subtraction_traits
{
    using type = decltype(declval<T1>() - declval<T2>());
};

template<class T1, class T2>
struct matrix_element_multiplication_traits
{
    using type = decltype(declval<T1>() * declval<T2>());
};

template<class T1, class T2>
using matrix_element_multiplication_t = typename matrix_element_multiplication_traits<T1, T2>::type;

//--------------------------------------------------------------------------------------------------
//- Unfortunately, in C++17 complex<T> only permits arithmetical expressions between
//  homogeneous element types: for example, expressions like complex<float>*complex<float>
//  and float*complex<float> are permitted, but double*complex<float> is not.  This macro
//  enforces this homogeneity in the traits types below.  Hopefully complex<T> will
//  support heterogeneous expressions at some point in the near future.
//--------------------------------------------------------------------------------------------------
//
#define ENFORCE_COMPLEX_OPERAND_HOMOGENEITY

#ifdef ENFORCE_COMPLEX_OPERAND_HOMOGENEITY
    #define ASSERT_COMPLEX_OPERAND_HOMOGENEITY(T1,T2)   static_assert(is_same_v<T1, T2>)
#else
    #define ASSERT_COMPLEX_OPERAND_HOMOGENEITY(T1,T2)
#endif

//--------------------------------------------------------------------------------------------------
//- Traits type matrix_element_promotion<T1,T2> to perform element type promotion for arithmetical
//  expressions, using the helper from above.
//--------------------------------------------------------------------------------------------------
//
template<class T1, class T2>
struct matrix_element_promotion
{
    using type = detail::matrix_element_promotion_helper_t<T1, T2>;
};

template<class T1, class T2>
struct matrix_element_promotion<T1, complex<T2>>
{
    ASSERT_COMPLEX_OPERAND_HOMOGENEITY(T1, T2);
    using type = complex<detail::matrix_element_promotion_helper_t<T1, T2>>;
};

template<class T1, class T2>
struct matrix_element_promotion<complex<T1>, T2>
{
    ASSERT_COMPLEX_OPERAND_HOMOGENEITY(T1, T2);
    using type = complex<detail::matrix_element_promotion_helper_t<T1, T2>>;
};

template<class T1, class T2>
struct matrix_element_promotion<complex<T1>, complex<T2>>
{
    ASSERT_COMPLEX_OPERAND_HOMOGENEITY(T1, T2);
    using type = complex<detail::matrix_element_promotion_helper_t<T1, T2>>;
};

template<class T1, class T2>
using matrix_element_promotion_t = typename matrix_element_promotion<T1, T2>::type;

#undef ASSERT_COMPLEX_OPERAND_HOMOGENEITY

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_ELEMENT_TRAITS_HPP_DEFINED
