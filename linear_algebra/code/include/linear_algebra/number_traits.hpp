//==================================================================================================
//  File:       number_traits.hpp
//
//  Summary:    This header defines several public traits types for reporting the properties of
//              numeric types.  It also provides the primary template for the number_traits
//              customization point, intended to be specialize-able by users.
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_NUMBER_TRAITS_HPP_DEFINED
#define LINEAR_ALGEBRA_NUMBER_TRAITS_HPP_DEFINED

namespace STD_LA {
namespace detail {
//--------------------------------------------------------------------------------------------------
//- Some common implementation details for builtin types
//--------------------------------------------------------------------------------------------------
//
struct builtin_number_traits
{
    using is_field     = true_type;
    using is_nc_ring   = true_type;
    using is_ring      = true_type;
    using is_semi_ring = true_type;
};

struct non_number_traits
{
    using is_field     = false_type;
    using is_nc_ring   = false_type;
    using is_ring      = false_type;
    using is_semi_ring = false_type;
};

template<class T>
using scalar_number_traits_helper_t =
    conditional_t<is_arithmetic_v<T>, builtin_number_traits, non_number_traits>;

}   //- detail namespace

//--------------------------------------------------------------------------------------------------
//- New traits type number_traits<T> to specify the properties modeled by a numerical type.
//--------------------------------------------------------------------------------------------------
//
template<class T>
struct number_traits : public detail::scalar_number_traits_helper_t<T>
{};

template<class T>
struct number_traits<complex<T>> : public number_traits<T>
{};

//--------------------------------------------------------------------------------------------------
//- New trait "is_field<T>" to detect if a numerical type models a field.
//--------------------------------------------------------------------------------------------------
//
template<class T>
struct is_field : public number_traits<T>::is_field //bool_constant<number_traits<T>::is_field::value>
{};

template<class T>
constexpr bool  is_field_v = is_field<T>::value;

//--------------------------------------------------------------------------------------------------
//- New trait "is_nc_ring<T>" to detect if a numerical type models a non_commutative ring.
//--------------------------------------------------------------------------------------------------
//
template<class T>
struct is_nc_ring : public bool_constant<number_traits<T>::is_nc_ring::value>
{};

template<class T>
constexpr bool  is_nc_ring_v = is_ring<T>::value;

//--------------------------------------------------------------------------------------------------
//- New trait "is_ring<T>" to detect if a numerical type models a ring.
//--------------------------------------------------------------------------------------------------
//
template<class T>
struct is_ring : public bool_constant<number_traits<T>::is_ring::value>
{};

template<class T>
constexpr bool  is_ring_v = is_ring<T>::value;

//--------------------------------------------------------------------------------------------------
//- New trait "is_semi_ring<T>" to detect if a type models a semiring.
//--------------------------------------------------------------------------------------------------
//
template<class T>
struct is_semi_ring : public bool_constant<number_traits<T>::is_semi_ring::value>
{};

template<class T>
constexpr bool  is_semi_ring_v = is_semi_ring<T>::value;

//--------------------------------------------------------------------------------------------------
//- New trait "is_complex<T>" to detect if a type is complex<T>.
//--------------------------------------------------------------------------------------------------
//
template<class T>
struct is_complex : public false_type
{};

template<class T>
struct is_complex<complex<T>> : public true_type
{};

template<class T>
constexpr bool  is_complex_v = is_complex<T>::value;

//--------------------------------------------------------------------------------------------------
//- New trait "is_matrix_element<T>" to detect if a type is a valid matrix/vector element.
//--------------------------------------------------------------------------------------------------
//
template<class T>
struct is_matrix_element : public bool_constant<is_arithmetic_v<T> || is_field_v<T>>
{};

template<class T>
constexpr bool  is_matrix_element_v = is_matrix_element<T>::value;

#if 0
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
#endif

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_NUMBER_TRAITS_HPP_DEFINED
