//==================================================================================================
//  File:       engine_traits.hpp
//
//  Summary:    This header defines several private traits types, used by the vector and matrix
//              class templates, that report properties of an engine.
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_ENGINE_TRAITS_HPP_DEFINED
#define LINEAR_ALGEBRA_ENGINE_TRAITS_HPP_DEFINED

namespace STD_LA {
namespace detail {

template<class ET>  class vector_iterator;
template<class ET>  class vector_const_iterator;

struct row_or_column_tag {};

//==================================================================================================
//  Traits type that specifies important properties of an engine, based on its tag.
//==================================================================================================
//
template<class TAG>
struct tag_traits;

template<>
struct tag_traits<const_vector_engine_tag>
{
    static constexpr bool   is_vector    = true;
    static constexpr bool   is_matrix    = false;
    static constexpr bool   is_const     = true;
    static constexpr bool   is_mutable   = false;
    static constexpr bool   is_resizable = false;
};

template<>
struct tag_traits<mutable_vector_engine_tag>
{
    static constexpr bool   is_vector    = true;
    static constexpr bool   is_matrix    = false;
    static constexpr bool   is_const     = true;
    static constexpr bool   is_mutable   = true;
    static constexpr bool   is_resizable = false;
};

template<>
struct tag_traits<resizable_vector_engine_tag>
{
    static constexpr bool   is_vector    = true;
    static constexpr bool   is_matrix    = false;
    static constexpr bool   is_const     = true;
    static constexpr bool   is_mutable   = true;
    static constexpr bool   is_resizable = true;
};

template<>
struct tag_traits<const_matrix_engine_tag>
{
    static constexpr bool   is_vector    = false;
    static constexpr bool   is_matrix    = true;
    static constexpr bool   is_const     = true;
    static constexpr bool   is_mutable   = false;
    static constexpr bool   is_resizable = false;
};

template<>
struct tag_traits<mutable_matrix_engine_tag>
{
    static constexpr bool   is_vector    = false;
    static constexpr bool   is_matrix    = true;
    static constexpr bool   is_const     = true;
    static constexpr bool   is_mutable   = true;
    static constexpr bool   is_resizable = false;
};

template<>
struct tag_traits<resizable_matrix_engine_tag>
{
    static constexpr bool   is_vector    = false;
    static constexpr bool   is_matrix    = true;
    static constexpr bool   is_const     = true;
    static constexpr bool   is_mutable   = true;
    static constexpr bool   is_resizable = true;
};


//==================================================================================================
//  Traits type that chooses the correct tag for a view, given the tag of the engine type to be
//  wrapped (ETT) and the desired tag type of the view (VTT).
//==================================================================================================
//
template<class ETT, class VTT> struct view_tag_chooser;

template<>
struct view_tag_chooser<const_matrix_engine_tag, const_matrix_engine_tag>
{
    using tag_type = const_matrix_engine_tag;
};

template<>
struct view_tag_chooser<const_matrix_engine_tag, mutable_matrix_engine_tag>
{
    using tag_type = const_matrix_engine_tag;
};

template<>
struct view_tag_chooser<const_matrix_engine_tag, const_vector_engine_tag>
{
    using tag_type = const_vector_engine_tag;
};

template<>
struct view_tag_chooser<const_matrix_engine_tag, mutable_vector_engine_tag>
{
    using tag_type = const_vector_engine_tag;
};

//------
//
template<>
struct view_tag_chooser<mutable_matrix_engine_tag, const_matrix_engine_tag>
{
    using tag_type = const_matrix_engine_tag;
};

template<>
struct view_tag_chooser<mutable_matrix_engine_tag, mutable_matrix_engine_tag>
{
    using tag_type = mutable_matrix_engine_tag;
};

template<>
struct view_tag_chooser<mutable_matrix_engine_tag, const_vector_engine_tag>
{
    using tag_type = const_vector_engine_tag;
};

template<>
struct view_tag_chooser<mutable_matrix_engine_tag, mutable_vector_engine_tag>
{
    using tag_type = mutable_vector_engine_tag;
};

//------
//
template<>
struct view_tag_chooser<resizable_matrix_engine_tag, const_matrix_engine_tag>
{
    using tag_type = const_matrix_engine_tag;
};

template<>
struct view_tag_chooser<resizable_matrix_engine_tag, mutable_matrix_engine_tag>
{
    using tag_type = mutable_matrix_engine_tag;
};

template<>
struct view_tag_chooser<resizable_matrix_engine_tag, const_vector_engine_tag>
{
    using tag_type = const_vector_engine_tag;
};

template<>
struct view_tag_chooser<resizable_matrix_engine_tag, mutable_vector_engine_tag>
{
    using tag_type = mutable_vector_engine_tag;
};

template<class ET, class VTT>
using view_category_t = typename view_tag_chooser<typename ET::engine_category, VTT>::tag_type;


template<class ET, class CAT>
struct view_traits
{
    static constexpr bool   is_mutable = tag_traits<CAT>::is_mutable;

    using referent  = conditional_t<is_mutable, remove_cv_t<ET>, remove_cv_t<ET> const>;
    using reference = conditional_t<is_mutable, typename ET::reference, typename ET::const_reference>;
    using pointer   = conditional_t<is_mutable, typename ET::pointer, typename ET::const_pointer>;
    template<class DET>
    using iterator  = conditional_t<is_mutable, vector_iterator<DET>, vector_const_iterator<DET>>;
};

template<class ET, class CAT>
using view_referent_t = typename view_traits<ET,CAT>::referent;

template<class ET, class CAT>
using view_reference_t = typename view_traits<ET,CAT>::reference;

template<class ET, class CAT>
using view_pointer_t = typename view_traits<ET,CAT>::pointer;

template<class ET, class CAT, class DET>
using view_iterator_t = typename view_traits<ET,CAT>::template iterator<DET>;

template<class VTT> inline constexpr
bool    is_vector_engine_tag = VTT::value >= const_vector_engine_tag::value &&
                               VTT::value <= mutable_vector_engine_tag::value;

template<class VTT> inline constexpr
bool    is_matrix_engine_tag = VTT::value >= const_matrix_engine_tag::value &&
                               VTT::value <= mutable_matrix_engine_tag::value;


//--------------------------------------------------------------------------------------------------
//- Internally-used tag type to facilitate distinguishing elements from vectors/matrices.
//
template<class T>
struct element_tag
{
    using engine_category = scalar_engine_tag;
};


//--------------------------------------------------------------------------------------------------
//- Internally-used constexpr variables for describe an engine's category.
//
template<class ET> inline constexpr 
bool    is_scalar_engine_v = (ET::engine_category::value == scalar_engine_tag::value);

template<class ET> inline constexpr 
bool    is_vector_engine_v = tag_traits<typename ET::engine_category>::is_vector;

template<typename ET> inline constexpr 
bool    is_matrix_engine_v = tag_traits<typename ET::engine_category>::is_matrix;

template<class ET1, class ET2> inline constexpr 
bool    engines_match_v = (is_matrix_engine_v<ET1> == is_matrix_engine_v<ET2>);


//--------------------------------------------------------------------------------------------------
//- Tests to determine if an engine has mutable element indexing.
//
template<class ET> inline constexpr 
bool    is_mutable_engine_v = tag_traits<typename ET::engine_category>::is_mutable;

template<class ET1, class ET2>
using enable_if_mutable = enable_if_t<is_same_v<ET1, ET2> && is_mutable_engine_v<ET1>, bool>;


//--------------------------------------------------------------------------------------------------
//- Tests to determine if an engine is resizable.
//
template<class ET> inline constexpr 
bool    is_resizable_engine_v = tag_traits<typename ET::engine_category>::is_resizable;

template<class ET1, class ET2>
using enable_if_resizable = enable_if_t<is_same_v<ET1, ET2> && is_resizable_engine_v<ET1>, bool>;


//--------------------------------------------------------------------------------------------------
//- Tests/facilites for fixed-size engines.
//
template<class ET> inline constexpr 
bool    is_fixed_size_engine_v = !is_resizable_engine_v<ET>;

template<class ET> inline constexpr 
auto    engine_size_v = ET().size();

//------
//
template<class ET1, class ET2>
using enable_if_fixed_size = enable_if_t<is_same_v<ET1, ET2> && is_fixed_size_engine_v<ET1>, bool>;


//--------------------------------------------------------------------------------------------------
//- Temporary replacement for std::swap (which is constexpr in C++20)
//
template<class T> inline constexpr 
bool    is_movable_v = std::is_move_constructible_v<T> && std::is_move_assignable_v<T>;

template<class T> inline constexpr 
bool    is_nothrow_movable_v = std::is_nothrow_move_constructible_v<T> &&  
                               std::is_nothrow_move_assignable_v<T>;

template<class T> constexpr 
std::enable_if_t<is_movable_v<T>, void>
la_swap(T& t0, T& t1) noexcept(is_nothrow_movable_v<T>)
{
    T   t2(std::move(t0));
    t0 = std::move(t1);
    t1 = std::move(t2);
}

}       //- detail namespace
}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_ENGINE_TRAITS_HPP_DEFINED
