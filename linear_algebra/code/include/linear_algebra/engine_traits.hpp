#ifndef LINEAR_ALGEBRA_ENGINE_TRAITS_HPP_DEFINED
#define LINEAR_ALGEBRA_ENGINE_TRAITS_HPP_DEFINED

namespace STD_LA {
namespace detail {
//- Tests to determine if an engine has mutable element indexing.
//
template<class ET>
constexpr bool  has_mutable_tag_v = ET::engine_category::value >= mutable_matrix_engine_tag::value;

template<class ET1, class ET2>
constexpr bool  is_mutable_engine_v = is_same_v<ET1, ET2> && has_mutable_tag_v<ET1>;


template<class ET1, class ET2>
using enable_if_mutable = enable_if_t<is_mutable_engine_v<ET1, ET2>, bool>;


//- Tests to determine if an engine is resizable.
//
template<class ET>
constexpr bool  has_resizable_tag_v = ET::engine_category::value >= resizable_matrix_engine_tag::value;

template<class ET1, class ET2>
constexpr bool  is_resizable_engine_v = is_same_v<ET1, ET2> && has_resizable_tag_v<ET1>;


template<class ET1, class ET2>
using enable_if_resizable = enable_if_t<is_resizable_engine_v<ET1, ET2>, bool>;

template<class A1, class T1>
using rebind_alloc_t = typename allocator_traits<A1>::template rebind_alloc<T1>;

//------
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

}       //- detail namespace
}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_ENGINE_TRAITS_HPP_DEFINED
