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
    using is_field    = true_type;
    using is_nc_ring  = true_type;
    using is_ring     = true_type;
    using is_semiring = true_type;
};

struct non_number_traits
{
    using is_field    = false_type;
    using is_nc_ring  = false_type;
    using is_ring     = false_type;
    using is_semiring = false_type;
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
struct is_field : public bool_constant<number_traits<T>::is_field::value>
{};

template<class T>
constexpr bool  is_field_v = is_field<T>::value;

//--------------------------------------------------------------------------------------------------
//- New trait "is_nc_ring<T>" to detect if a numerical type models a non_commutative ring.
//--------------------------------------------------------------------------------------------------
//
template<class T>
struct is_nc_ring : public bool_constant<number_traits<T>::is_field::value>
{};

template<class T>
constexpr bool  is_nc_ring_v = is_ring<T>::value;

//--------------------------------------------------------------------------------------------------
//- New trait "is_ring<T>" to detect if a numerical type models a ring.
//--------------------------------------------------------------------------------------------------
//
template<class T>
struct is_ring : public bool_constant<number_traits<T>::is_field::value>
{};

template<class T>
constexpr bool  is_ring_v = is_ring<T>::value;

//--------------------------------------------------------------------------------------------------
//- New trait "is_semi_ring<T>" to detect if a type models a semiring.
//--------------------------------------------------------------------------------------------------
//
template<class T>
struct is_semi_ring : public bool_constant<number_traits<T>::is_field::value>
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

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_NUMBER_TRAITS_HPP_DEFINED
