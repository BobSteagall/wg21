//==================================================================================================
//  File:       engine_traits.hpp
//
//  Summary:    This header defines several private traits types, alias templates, variable
//              templates, and functions that are used by the rest of this implementation.
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_IMPL_SUPPORT_HPP_DEFINED
#define LINEAR_ALGEBRA_IMPL_SUPPORT_HPP_DEFINED

namespace STD_LA {
namespace detail {

template<class ET>  class vector_iterator;
template<class ET>  class vector_const_iterator;

struct special_ctor_tag {};

//==================================================================================================
//- Traits type to detect if a type is std::complex<T>.
//==================================================================================================
//
template<class T>
struct is_complex : public false_type
{};

template<class T>
struct is_complex<complex<T>> : public true_type
{};

template<class T>  constexpr
bool    is_complex_v = is_complex<T>::value;


//==================================================================================================
//  Traits type that specifies important properties of an engine, based on its tag.
//==================================================================================================
//
template<class TAG>
struct tag_traits;

template<>
struct tag_traits<scalar_engine_tag>
{
    static constexpr bool   is_scalar    = true;
    static constexpr bool   is_vector    = false;
    static constexpr bool   is_matrix    = false;
    static constexpr bool   is_readable  = false;
    static constexpr bool   is_writable  = false;
    static constexpr bool   is_resizable = false;
};

template<>
struct tag_traits<readable_vector_engine_tag>
{
    static constexpr bool   is_scalar    = false;
    static constexpr bool   is_vector    = true;
    static constexpr bool   is_matrix    = false;
    static constexpr bool   is_readable  = true;
    static constexpr bool   is_writable  = false;
    static constexpr bool   is_resizable = false;
};

template<>
struct tag_traits<writable_vector_engine_tag>
{
    static constexpr bool   is_scalar    = false;
    static constexpr bool   is_vector    = true;
    static constexpr bool   is_matrix    = false;
    static constexpr bool   is_readable  = true;
    static constexpr bool   is_writable  = true;
    static constexpr bool   is_resizable = false;
};

template<>
struct tag_traits<resizable_vector_engine_tag>
{
    static constexpr bool   is_scalar    = false;
    static constexpr bool   is_vector    = true;
    static constexpr bool   is_matrix    = false;
    static constexpr bool   is_readable  = true;
    static constexpr bool   is_writable  = true;
    static constexpr bool   is_resizable = true;
};

template<>
struct tag_traits<readable_matrix_engine_tag>
{
    static constexpr bool   is_scalar    = false;
    static constexpr bool   is_vector    = false;
    static constexpr bool   is_matrix    = true;
    static constexpr bool   is_readable  = true;
    static constexpr bool   is_writable  = false;
    static constexpr bool   is_resizable = false;
};

template<>
struct tag_traits<writable_matrix_engine_tag>
{
    static constexpr bool   is_scalar    = false;
    static constexpr bool   is_vector    = false;
    static constexpr bool   is_matrix    = true;
    static constexpr bool   is_readable  = true;
    static constexpr bool   is_writable  = true;
    static constexpr bool   is_resizable = false;
};

template<>
struct tag_traits<resizable_matrix_engine_tag>
{
    static constexpr bool   is_scalar    = false;
    static constexpr bool   is_vector    = false;
    static constexpr bool   is_matrix    = true;
    static constexpr bool   is_readable  = true;
    static constexpr bool   is_writable  = true;
    static constexpr bool   is_resizable = true;
};

//- Variable templates used to determine important engine properties, based on the engine's
//  category tag.
//
template<class ET> inline constexpr
bool    is_scalar_v = detail::tag_traits<typename ET::engine_category>::is_scalar;

template<class ET> inline constexpr
bool    is_vector_v = detail::tag_traits<typename ET::engine_category>::is_vector;

template<class ET> inline constexpr
bool    is_matrix_v = detail::tag_traits<typename ET::engine_category>::is_matrix;

template<class ET> inline constexpr
bool    is_readable_v = detail::tag_traits<typename ET::engine_category>::is_readable;

template<class ET> inline constexpr
bool    is_writable_v = detail::tag_traits<typename ET::engine_category>::is_writable;

template<class ET> inline constexpr
bool    is_resizable_v = detail::tag_traits<typename ET::engine_category>::is_resizable;

template<class ET1, class ET2> inline constexpr
bool    engines_match_v = (is_matrix_v<ET1> && is_matrix_v<ET2>) ||
                          (is_vector_v<ET1> && is_vector_v<ET2>) ||
                          (is_scalar_v<ET1> && is_scalar_v<ET2>)  ;

//- Alias templates used to enable various parts of the vector and matrix interfaces via SFINAE.
//
template<class ET1, class ET2>
using enable_if_writable = enable_if_t<is_same_v<ET1, ET2> && is_writable_v<ET1>, bool>;

template<class ET1, class ET2>
using enable_if_resizable = enable_if_t<is_same_v<ET1, ET2> && is_resizable_v<ET1>, bool>;

template<class ET1, class ET2>
using enable_if_fixed_size = enable_if_t<is_same_v<ET1, ET2> && !is_resizable_v<ET1>, bool>;


//==================================================================================================
//  Traits type that chooses the correct tag for a non-owning engine (NOE), given the tag of the
//  source engine type to be wrapped (ETT) and the desired tag type of the non-owning engine (VTT).
//==================================================================================================
//
template<class ETT, class VTT> struct noe_tag_chooser;

template<>
struct noe_tag_chooser<readable_matrix_engine_tag, readable_matrix_engine_tag>
{
    using tag_type = readable_matrix_engine_tag;
};

template<>
struct noe_tag_chooser<readable_matrix_engine_tag, writable_matrix_engine_tag>
{
    using tag_type = readable_matrix_engine_tag;
};

template<>
struct noe_tag_chooser<readable_matrix_engine_tag, readable_vector_engine_tag>
{
    using tag_type = readable_vector_engine_tag;
};

template<>
struct noe_tag_chooser<readable_matrix_engine_tag, writable_vector_engine_tag>
{
    using tag_type = readable_vector_engine_tag;
};

//------
//
template<>
struct noe_tag_chooser<writable_matrix_engine_tag, readable_matrix_engine_tag>
{
    using tag_type = readable_matrix_engine_tag;
};

template<>
struct noe_tag_chooser<writable_matrix_engine_tag, writable_matrix_engine_tag>
{
    using tag_type = writable_matrix_engine_tag;
};

template<>
struct noe_tag_chooser<writable_matrix_engine_tag, readable_vector_engine_tag>
{
    using tag_type = readable_vector_engine_tag;
};

template<>
struct noe_tag_chooser<writable_matrix_engine_tag, writable_vector_engine_tag>
{
    using tag_type = writable_vector_engine_tag;
};

//------
//
template<>
struct noe_tag_chooser<resizable_matrix_engine_tag, readable_matrix_engine_tag>
{
    using tag_type = readable_matrix_engine_tag;
};

template<>
struct noe_tag_chooser<resizable_matrix_engine_tag, writable_matrix_engine_tag>
{
    using tag_type = writable_matrix_engine_tag;
};

template<>
struct noe_tag_chooser<resizable_matrix_engine_tag, readable_vector_engine_tag>
{
    using tag_type = readable_vector_engine_tag;
};

template<>
struct noe_tag_chooser<resizable_matrix_engine_tag, writable_vector_engine_tag>
{
    using tag_type = writable_vector_engine_tag;
};

//- Variable template used as a convenience interface to noe_tag_chooser.
//
template<class ET, class VTT>
using noe_category_t = typename noe_tag_chooser<typename ET::engine_category, VTT>::tag_type;


//==================================================================================================
//  Traits type that computes several of the nested type aliases that are used by the non-owning
//  engine (NOE) types defined in this implementation.  These nested types are determined from
//  the tag of the source engine type to be wrapped (ET) and the desired tag type of the resulting
//  non-owning engine (CAT).  The type of 'iterator' is determined additionally by considering
//  the destination engine type (DET).
//
//  This type (or some other compile-time computation like it) is necessary in order to determine
//  the nested type aliases 'reference' and 'iterator'.
//==================================================================================================
//
template<class ET, class CAT>
struct noe_traits
{
    static constexpr bool   is_writable = tag_traits<CAT>::is_writable;

    using referent  = conditional_t<is_writable, remove_cv_t<ET>, remove_cv_t<ET> const>;
    using element   = conditional_t<is_writable, typename ET::element_type, typename ET::element_type const>;
    using reference = conditional_t<is_writable, typename ET::reference, typename ET::const_reference>;
    using pointer   = conditional_t<is_writable, typename ET::pointer, typename ET::const_pointer>;

    template<class DET>
    using iterator  = conditional_t<is_writable, vector_iterator<DET>, vector_const_iterator<DET>>;

#ifdef LA_USE_MDSPAN
    using span_type = conditional_t<is_writable, typename ET::span_type, typename ET::const_span_type>;
#endif
};

//- Alias template used as a convenience interface to noe_traits.
//
template<class ET, class CAT>
using noe_referent_t = typename noe_traits<ET,CAT>::referent;

template<class ET, class CAT>
using noe_element_t = typename noe_traits<ET,CAT>::element;

template<class ET, class CAT>
using noe_reference_t = typename noe_traits<ET,CAT>::reference;

template<class ET, class CAT>
using noe_pointer_t = typename noe_traits<ET,CAT>::pointer;

template<class ET, class CAT, class DET>
using noe_iterator_t = typename noe_traits<ET,CAT>::template iterator<DET>;

#ifdef LA_USE_MDSPAN
template<class ET, class CAT>
using noe_mdspan_t = typename noe_traits<ET,CAT>::span_type;
#endif

//- Detection trait and convenience alias template for determining whether an engine has
//  iteration in its public interface.
//
template<typename T, typename = void>
struct has_iteration
:   std::false_type
{};

template<typename T>
struct has_iteration<T, std::void_t<decltype(std::declval<T>().begin()),
                                    decltype(std::declval<T>().end()),
                                    decltype(std::declval<T>().cbegin()),
                                    decltype(std::declval<T>().cend()) >>
:   std::true_type
{};

template<class ET> inline constexpr
bool    has_iteration_v = has_iteration<ET>::value;

//------
//
template<bool HasIter, class ET>
struct get_engine_iter;

template<class ET>
struct get_engine_iter<true, ET>
{
    using m_iter_type = typename ET::iterator;
    using c_iter_type = typename ET::const_iterator;
};

template<class ET>
struct get_engine_iter<false, ET>
{
    using m_iter_type = noe_iterator_t<ET, typename ET::engine_category, ET>;
    using c_iter_type = vector_const_iterator<ET>;
};

template<bool HasIter, class ET>
using engine_m_iter_t = typename get_engine_iter<HasIter, ET>::m_iter_type;

template<bool HasIter, class ET>
using engine_c_iter_t = typename get_engine_iter<HasIter, ET>::c_iter_type;

#ifdef LA_USE_MDSPAN
//==================================================================================================
//  Traits type for specifying an mdspan type on behalf of a non-owning engine.
//==================================================================================================
//
template<class T>
struct noe_mdspan_traits;


template<class T, ptrdiff_t X0, ptrdiff_t X1, class L, class A>
struct noe_mdspan_traits<basic_mdspan<T, extents<X0, X1>, L, A>>
{
    using src_span_type = basic_mdspan<T, extents<X0, X1>, layout_right, A>;
    using tr_span_type  = basic_mdspan<T, extents<X1, X0>, layout_right, A>;

    static constexpr tr_span_type   tr_span(src_span_type const& s);
};


template<class T, ptrdiff_t X0, ptrdiff_t X1, class L, class A> constexpr
typename noe_mdspan_traits<basic_mdspan<T, extents<X0, X1>, L, A>>::tr_span_type
noe_mdspan_traits<basic_mdspan<T, extents<X0, X1>, L, A>>::tr_span(src_span_type const& s)
{
    return tr_span_type(s.data());
}


using dyn_extents = extents<dynamic_extent, dynamic_extent>;
using dyn_strides = array<typename dyn_extents::index_type, 2>;
using dyn_layout  = layout_stride<dynamic_extent, dynamic_extent>;
using dyn_mapping = typename dyn_layout::template mapping<dyn_extents>;

template<class T, class A>
struct noe_mdspan_traits<basic_mdspan<T, dyn_extents, dyn_layout, A>>
{
    using src_span_type = basic_mdspan<T, dyn_extents, dyn_layout, A>;
    using tr_span_type  = basic_mdspan<T, dyn_extents, dyn_layout, A>;

    static constexpr tr_span_type   tr_span(src_span_type const& s);
};

template<class T, class A> constexpr
typename noe_mdspan_traits<basic_mdspan<T, dyn_extents, dyn_layout, A>>::tr_span_type
noe_mdspan_traits<basic_mdspan<T, dyn_extents, dyn_layout, A>>::tr_span(src_span_type const& s)
{
    dyn_extents     ext(s.extent(1), s.extent(0));
    dyn_strides     str{s.stride(1), s.stride(0)};
    dyn_mapping     map(ext, str);

    return tr_span_type(s.data(), map);
}


template<class ST>
using noe_mdspan_transpose_t = typename noe_mdspan_traits<ST>::tr_span_type;

template<class ST> inline constexpr
typename noe_mdspan_traits<ST>::tr_span_type
noe_mdspan_transpose(ST const& s)
{
    return noe_mdspan_traits<ST>::tr_span(s);
}

template<class T, class ST> inline constexpr
basic_mdspan<T, dyn_extents, dyn_layout>
make_dyn_span(T* pdata, ST rows, ST cols, ST row_stride, ST col_stride = 1u)
{
    using idx_t = typename dyn_extents::index_type;

    dyn_extents     extents(static_cast<idx_t>(rows), static_cast<idx_t>(cols));
    dyn_strides     strides{static_cast<idx_t>(row_stride), static_cast<idx_t>(col_stride)};
    dyn_mapping     mapping(extents, strides);

    return basic_mdspan<T, dyn_extents, dyn_layout>(pdata, mapping);
}

#endif
//==================================================================================================
//  Traits type for choosing between three alternative traits-type parameters.  This is used
//  extensively in the private implementation when selecting arithmetic traits at compile time.
//==================================================================================================
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


//==================================================================================================
//  Some private helper functions for allocating/deallocating the memory used by the dynamic
//  vector and matrix engines defined elsewhere.  Note that all memory thus allocated is default-
//  constructed.  This means that elements lying in (currently) unused capacity are also
//  initialized, which may or may not be what happens in the final version.
//==================================================================================================
//
template<class AT>
typename allocator_traits<AT>::pointer
allocate(AT& alloc, size_t n)
{
    auto    p_dst = allocator_traits<AT>::allocate(alloc, n);

    try
    {
        uninitialized_value_construct_n(p_dst, n);
    }
    catch (...)
    {
        allocator_traits<AT>::deallocate(alloc, p_dst, n);
        throw;
    }
    return p_dst;
}

template<class AT>
typename allocator_traits<AT>::pointer
allocate(AT& alloc, size_t n, typename allocator_traits<AT>::const_pointer p_src)
{
    auto    p_dst = allocator_traits<AT>::allocate(alloc, n);

    try
    {
        uninitialized_copy_n(p_src, n, p_dst);
    }
    catch (...)
    {
        allocator_traits<AT>::deallocate(alloc, p_dst, n);
        throw;
    }
    return p_dst;
}

template<class AT>
void
deallocate(AT& alloc, typename allocator_traits<AT>::pointer p_dst, size_t n) noexcept
{
    if (p_dst != nullptr)
    {
        destroy_n(p_dst, n);
        allocator_traits<AT>::deallocate(alloc, p_dst, n);
    }
}

//- Alias template used for convenience when rebinding allocators.
//
template<class A1, class T1>
using rebind_alloc_t = typename allocator_traits<A1>::template rebind_alloc<T1>;


//==================================================================================================
//- Temporary replacement for std::swap (which is constexpr in C++20)
//==================================================================================================
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
#endif  //- LINEAR_ALGEBRA_IMPL_SUPPORT_HPP_DEFINED
