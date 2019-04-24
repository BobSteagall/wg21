//==================================================================================================
//  File:       arithmetic_traits.hpp
//
//  Summary:    This header defines several private traits types that are used by the various
//              arithmetic traits types, public and private, to implement arithmetic operators.
//==================================================================================================
//
#ifndef MATRIX_ARITHMETIC_TRAITS_HPP_DEFINED
#define MATRIX_ARITHMETIC_TRAITS_HPP_DEFINED

namespace STD_LA {
namespace detail {
//--------------------------------------------------------------------------------------------------
//- Helper traits for choosing between three traits-type parameters.
//
template<class T1, class T2, class DEF>
struct non_void_traits_chooser;

template<class T1, class DEF>
struct non_void_traits_chooser<T1, void, DEF>
{
    using traits_type = T1;
};

template<class T2, class DEF>
struct non_void_traits_chooser<void, T2, DEF>
{
    using traits_type = T2;
};

template<class DEF>
struct non_void_traits_chooser<void, void, DEF>
{
    using traits_type = DEF;
};


//--------------------------------------------------------------------------------------------------
//- Helper traits for rebinding standard-conformant allocators.
//
template<class A1, class T1>
using rebind_alloc_t = typename allocator_traits<A1>::template rebind_alloc<T1>;


//--------------------------------------------------------------------------------------------------
//- Internally-used tag type to facilitate distinguishing elements from vectors/matrices.
//
template<class T>
struct element_tag
{
    using engine_category = integral_constant<int, 0>;
};

//--------------------------------------------------------------------------------------------------
//- Internally-used constexpr variables for describe an engine's category.
//
template<class ET>
constexpr bool  is_scalar_engine_v = (ET::engine_category::value == element_tag<ET>::engine_category::value);

template<class ET>
constexpr bool  is_vector_engine_v = (ET::engine_category::value < const_matrix_engine_tag::value);

template<typename ET>
constexpr bool  is_matrix_engine_v = (ET::engine_category::value >= const_matrix_engine_tag::value);

template<class ET1, class ET2>
constexpr bool  engines_match_v = (is_vector_engine_v<ET1> && is_vector_engine_v<ET2>) ||
                                  (is_matrix_engine_v<ET1> && is_matrix_engine_v<ET2>);

}       //- detail namespace

template<class ET, class OT> inline
constexpr bool
result_requires_resize(vector<ET,OT> const&)
{
    return vector<ET, OT>::engine_type::is_resizable::value == true  &&
           vector<ET, OT>::engine_type::is_fixed_size::value == false;
}


template<class ET, class OT> inline
constexpr bool
is_fixed_size(vector<ET,OT> const&)
{
    return vector<ET,OT>::engine_type::is_fixed_size::value;
}

template<class ET, class OT> inline
constexpr bool
is_fixed_size(matrix<ET,OT> const&)
{
    return matrix<ET,OT>::engine_type::is_fixed_size::value;
}

template<class ET, class OT> inline
constexpr bool
is_resizable(vector<ET,OT> const&)
{
    return vector<ET,OT>::engine_type::is_resizable::value;
}

template<class ET, class OT> inline
constexpr bool
is_resizable(matrix<ET,OT> const&)
{
    return matrix<ET,OT>::engine_type::is_resizable::value;
}

}       //- STD_LA namespace
#endif  //- MATRIX_ARITHMETIC_TRAITS_HPP_DEFINED
