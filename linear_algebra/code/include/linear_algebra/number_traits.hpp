#ifndef LINEAR_ALGEBRA_NUMBER_TRAITS_HPP_DEFINED
#define LINEAR_ALGEBRA_NUMBER_TRAITS_HPP_DEFINED

#include "forward_declarations.hpp"

namespace STD_LA {
namespace detail {
//--------------------------------------------------------------------------------------------------
//- Some common implementation details for builtin types
//--------------------------------------------------------------------------------------------------
//
struct builtin_number_traits
{
    using is_field    = std::true_type;
    using is_ring     = std::true_type;
    using is_semiring = std::true_type;
};

struct non_number_traits
{
    using is_field    = std::false_type;
    using is_ring     = std::false_type;
    using is_semiring = std::false_type;
};

template<class T>
using scalar_number_traits_helper_t =
    std::conditional_t<std::is_arithmetic_v<T>, builtin_number_traits, non_number_traits>;

}   //- detail namespace

//--------------------------------------------------------------------------------------------------
//- New traits type number_traits<T> to specify the properties modeled by a numerical type.
//--------------------------------------------------------------------------------------------------
//
template<class T>
struct number_traits : public detail::scalar_number_traits_helper_t<T>
{};

template<class T>
struct number_traits<std::complex<T>> : public number_traits<T>
{};

//--------------------------------------------------------------------------------------------------
//- New trait "is_field<T>" to detect if a numerical type models a field.
//--------------------------------------------------------------------------------------------------
//
template<class T>
struct is_field : public std::bool_constant<number_traits<T>::is_field::value>
{};

template<class T>
constexpr bool  is_field_v = is_field<T>::value;

//--------------------------------------------------------------------------------------------------
//- New trait "is_ring<T>" to detect if a numerical type models a ring.
//--------------------------------------------------------------------------------------------------
//
template<class T>
struct is_ring : public std::bool_constant<number_traits<T>::is_field::value>
{};

template<class T>
constexpr bool  is_ring_v = is_ring<T>::value;

//--------------------------------------------------------------------------------------------------
//- New trait "is_semi_ring<T>" to detect if a type models a semiring (i.e., is non-commutative).
//--------------------------------------------------------------------------------------------------
//
template<class T>
struct is_semi_ring : public std::bool_constant<number_traits<T>::is_field::value>
{};

template<class T>
constexpr bool  is_semi_ring_v = is_semi_ring<T>::value;

//--------------------------------------------------------------------------------------------------
//- New trait "is_complex<T>" to detect if a type is std::complex<T>.
//--------------------------------------------------------------------------------------------------
//
template<class T>
struct is_complex : public std::false_type
{};

template<class T>
struct is_complex<std::complex<T>> : public std::true_type
{};

template<class T>
constexpr bool  is_complex_v = is_complex<T>::value;

//--------------------------------------------------------------------------------------------------
//- New trait "is_matrix_element<T>" to detect if a type is a valid matrix/vector element.
//--------------------------------------------------------------------------------------------------
//
template<class T>
struct is_matrix_element : public std::bool_constant<std::is_arithmetic_v<T> || is_field_v<T>>
{};

template<class T>
constexpr bool  is_matrix_element_v = is_matrix_element<T>::value;

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_NUMBER_TRAITS_HPP_DEFINED
