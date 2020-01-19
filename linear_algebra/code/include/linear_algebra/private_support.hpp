//==================================================================================================
//  File:       private_support.hpp
//
//  Summary:    This header defines several private traits types, alias templates, variable
//              templates, and functions that support the rest of this implementation.
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_IMPL_SUPPORT_HPP_DEFINED
#define LINEAR_ALGEBRA_IMPL_SUPPORT_HPP_DEFINED

namespace STD_LA {
namespace detail {

struct special_ctor_tag {};

//==================================================================================================
//  This simple traits type and corresponding alias template to used detect if a type is of
//  the form std::complex<T>.
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
//  This traits type specifies important properties to be associated with an engine, based on a
//  given tag type.  These properties are represented by boolean values.
//==================================================================================================
//
template<class TAG>
struct engine_tag_traits;

template<>
struct engine_tag_traits<scalar_engine_tag>
{
    static constexpr bool   is_scalar    = true;
    static constexpr bool   is_vector    = false;
    static constexpr bool   is_matrix    = false;
    static constexpr bool   is_readable  = false;
    static constexpr bool   is_writable  = false;
    static constexpr bool   is_resizable = false;
};

template<>
struct engine_tag_traits<readable_vector_engine_tag>
{
    static constexpr bool   is_scalar    = false;
    static constexpr bool   is_vector    = true;
    static constexpr bool   is_matrix    = false;
    static constexpr bool   is_readable  = true;
    static constexpr bool   is_writable  = false;
    static constexpr bool   is_resizable = false;
};

template<>
struct engine_tag_traits<writable_vector_engine_tag>
{
    static constexpr bool   is_scalar    = false;
    static constexpr bool   is_vector    = true;
    static constexpr bool   is_matrix    = false;
    static constexpr bool   is_readable  = true;
    static constexpr bool   is_writable  = true;
    static constexpr bool   is_resizable = false;
};

template<>
struct engine_tag_traits<resizable_vector_engine_tag>
{
    static constexpr bool   is_scalar    = false;
    static constexpr bool   is_vector    = true;
    static constexpr bool   is_matrix    = false;
    static constexpr bool   is_readable  = true;
    static constexpr bool   is_writable  = true;
    static constexpr bool   is_resizable = true;
};

template<>
struct engine_tag_traits<readable_matrix_engine_tag>
{
    static constexpr bool   is_scalar    = false;
    static constexpr bool   is_vector    = false;
    static constexpr bool   is_matrix    = true;
    static constexpr bool   is_readable  = true;
    static constexpr bool   is_writable  = false;
    static constexpr bool   is_resizable = false;
};

template<>
struct engine_tag_traits<writable_matrix_engine_tag>
{
    static constexpr bool   is_scalar    = false;
    static constexpr bool   is_vector    = false;
    static constexpr bool   is_matrix    = true;
    static constexpr bool   is_readable  = true;
    static constexpr bool   is_writable  = true;
    static constexpr bool   is_resizable = false;
};

template<>
struct engine_tag_traits<resizable_matrix_engine_tag>
{
    static constexpr bool   is_scalar    = false;
    static constexpr bool   is_vector    = false;
    static constexpr bool   is_matrix    = true;
    static constexpr bool   is_readable  = true;
    static constexpr bool   is_writable  = true;
    static constexpr bool   is_resizable = true;
};


//- These variable templates are used as a shorthand to report important engine properties, using
//  the engine_tag_traits type defined above.  They are used in the private implementation below,
//  as well as by a corresponsing set of public variable templates (is_*_engine_v<ET>).
//
template<class ET> inline constexpr
bool    is_scalar_v = detail::engine_tag_traits<typename ET::engine_category>::is_scalar;

template<class ET> inline constexpr
bool    is_vector_v = detail::engine_tag_traits<typename ET::engine_category>::is_vector;

template<class ET> inline constexpr
bool    is_matrix_v = detail::engine_tag_traits<typename ET::engine_category>::is_matrix;

template<class ET> inline constexpr
bool    is_readable_v = detail::engine_tag_traits<typename ET::engine_category>::is_readable;

template<class ET> inline constexpr
bool    is_writable_v = detail::engine_tag_traits<typename ET::engine_category>::is_writable;

template<class ET> inline constexpr
bool    is_resizable_v = detail::engine_tag_traits<typename ET::engine_category>::is_resizable;

template<class ET1, class ET2> inline constexpr
bool    engines_match_v = (is_matrix_v<ET1> && is_matrix_v<ET2>) ||
                          (is_vector_v<ET1> && is_vector_v<ET2>) ||
                          (is_scalar_v<ET1> && is_scalar_v<ET2>)  ;


//- These alias templates are used to enable/disable various parts of the vector and matrix public
//  interfaces via SFINAE.
//
template<class ET1, class ET2>
using enable_if_writable = enable_if_t<is_same_v<ET1, ET2> && is_writable_v<ET1>, bool>;

template<class ET1, class ET2>
using enable_if_resizable = enable_if_t<is_same_v<ET1, ET2> && is_resizable_v<ET1>, bool>;

template<class ET1, class ET2, class U>
using enable_if_init_list_ok = enable_if_t<is_same_v<ET1, ET2> && !is_resizable_v<ET1> &&
                                           is_convertible_v<U, typename ET1::value_type>, bool>;


#ifdef LA_USE_MDSPAN
//==================================================================================================
//  The following are several detection idiom traits types, regular traits types, and alias
//  templates for determining the type aliases span_type and const_span_type that should be
//  with an engine's public interface.  There are exactly three mutually-exclusive possibilities,
//  each based on the presence/absence of the type aliases:
//    1. Both aliases are missing      --> associated aliases are void
//    2. Both aliases are void         --> associated aliases are void
//    3. Both aliases are basic_mdspan --> associated aliases are of the form basic_mdspan<T,X,L,A>
//==================================================================================================
//
//- This detection idiom traits type and corresponding variable template are used to determine
//  whether or not an engine has the nested type alias span_type in its public interface.
//
template<typename ET, typename = void>
struct has_span_type
:   public false_type
{};

template<typename ET>
struct has_span_type<ET, std::void_t<typename ET::span_type>>
:   public true_type
{};

template<class ET> inline constexpr
bool    has_span_type_v = has_span_type<ET>::value;


//- This detection idiom traits type and corresponding variable template are used to determine
//  whether or not an engine has the nested type alias const_span_type in its public interface.
//
template<typename ET, typename = void>
struct has_const_span_type
:   public false_type
{};

template<typename ET>
struct has_const_span_type<ET, std::void_t<typename ET::const_span_type>>
:   public true_type
{};

template<class ET> inline constexpr
bool    has_const_span_type_v = has_const_span_type<ET>::value;


//- This traits type is used to determine whether or not two types have the correct forms as
//  a pair of nested type aliases for mdspan; and if so, to express those types in its public
//  interface.  Both types must be void, or both must be an instance of basic_mdspan.
//
template<typename SA0, typename SA1>
struct extract_span_types
:   public false_type
{};

template<>
struct extract_span_types<void, void>
:   public true_type
{
    using span_type       = void;
    using const_span_type = void;
};

template<class T0, class X0, class L0, class A0, class T1, class X1, class L1, class A1>
struct extract_span_types<basic_mdspan<T0, X0, L0, A0>, basic_mdspan<T1, X1, L1, A1>>
:   public true_type
{
    using span_type       = basic_mdspan<T0, X0, L0, A0>;
    using const_span_type = basic_mdspan<T1, X1, L1, A1>;
};


//- This traits type is used to determine the span_type and const_span_type aliases from an
//  engine's public interface.  If both aliases are missing, then the results are both void.
//  If both aliases are void, then the results are both void.  If both aliases are for instances
//  of basic_mdspan, then the results are those aliases.  All other combinations are errors.
//
template<bool, bool, class ET>
struct engine_span_types;

template<class ET>
struct engine_span_types<false, false, ET>
:   public true_type
{
    using span_type       = void;
    using const_span_type = void;
};

template<class ET>
struct engine_span_types<false, true, ET>
:   public false_type
{};

template<class ET>
struct engine_span_types<true, false, ET>
:   public false_type
{};

template<class ET>
struct engine_span_types<true, true, ET>
:   public extract_span_types<typename ET::span_type, typename ET::const_span_type>
{};


//- This alias template is used by vector and matrix in static_assert statements to ensure
//  the nested type aliases span_type and const_span_type follow the rules outlined above.
//
template<class ET> inline constexpr
bool    has_valid_span_alias_form_v =
            engine_span_types<has_span_type_v<ET>, has_const_span_type_v<ET>, ET>::value;


//- These alias templates are used by vector, matrix, and the non-owning engine traits defined
//  below to determine the span_type and const_span_type aliases associated with an engine type.
//
template<class ET>
using engine_span_t =
        typename engine_span_types<has_span_type_v<ET>, has_const_span_type_v<ET>, ET>::span_type;

template<class ET>
using engine_const_span_t =
        typename engine_span_types<has_span_type_v<ET>, has_const_span_type_v<ET>, ET>::const_span_type;


//- This alias template is used by vector and matrix to SFINAE in/out the span() member function,
//  based on the mdspan support provided by the underlying engine.
//
template<class ET1, class ET2>
using enable_if_spannable = enable_if_t<is_same_v<ET1, ET2> && has_span_type_v<ET1>, bool>;


#endif
//==================================================================================================
//  This traits type chooses the correct tag for a non-owning engine (NOE), given the tag of the
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

//- Alias template used as a convenience interface to noe_tag_chooser.
//
template<class ET, class VTT>
using noe_category_t = typename noe_tag_chooser<typename ET::engine_category, VTT>::tag_type;


//==================================================================================================
//  This traits type computes several of the nested type aliases that are used by the non-owning
//  engine (NOE) types defined in this implementation.  These nested types are determined by the
//  tag type of the source engine type to be wrapped (ET) and the tag type of the resulting
//  non-owning engine (NEWCAT).
//
//  This type (or some other compile-time computation like it) is necessary in order to determine
//  the nested type aliases 'pointer', 'reference', 'span_type', and 'const_span_type'.
//==================================================================================================
//
template<class ET, class NEWCAT>
struct noe_traits
{
    static constexpr bool   is_writable = engine_tag_traits<NEWCAT>::is_writable;

    using referent  = conditional_t<is_writable, remove_cv_t<ET>, remove_cv_t<ET> const>;
    using element   = conditional_t<is_writable, typename ET::element_type, typename ET::element_type const>;
    using reference = conditional_t<is_writable, typename ET::reference, typename ET::const_reference>;
    using pointer   = conditional_t<is_writable, typename ET::pointer, typename ET::const_pointer>;

#ifdef LA_USE_MDSPAN
    using span_type       = conditional_t<is_writable, engine_span_t<ET>, engine_const_span_t<ET>>;
    using const_span_type = engine_const_span_t<ET>;
#endif
};

//- These alias templates provide a convenience interface to noe_traits.
//
template<class ET, class NEWCAT>
using noe_referent_t = typename noe_traits<ET, NEWCAT>::referent;

template<class ET, class NEWCAT>
using noe_element_t = typename noe_traits<ET, NEWCAT>::element;

template<class ET, class NEWCAT>
using noe_reference_t = typename noe_traits<ET, NEWCAT>::reference;

template<class ET, class NEWCAT>
using noe_pointer_t = typename noe_traits<ET, NEWCAT>::pointer;

#ifdef LA_USE_MDSPAN
template<class ET, class NEWCAT>
using noe_mdspan_t = typename noe_traits<ET, NEWCAT>::span_type;

template<class ET, class NEWCAT>
using noe_const_mdspan_t = typename noe_traits<ET, NEWCAT>::const_span_type;
#endif


#ifdef LA_USE_MDSPAN
//==================================================================================================
//  Traits type for specifying an mdspan type on behalf of a non-owning engine.
//==================================================================================================
//
//- Some alias template helpers to reduce verbosity below.
//
using dyn_mat_extents = extents<dynamic_extent, dynamic_extent>;
using dyn_mat_strides = array<typename dyn_mat_extents::index_type, 2>;
using dyn_mat_layout  = layout_stride<dynamic_extent, dynamic_extent>;
using dyn_mat_mapping = typename dyn_mat_layout::template mapping<dyn_mat_extents>;

using dyn_vec_extents = extents<dynamic_extent>;
using dyn_vec_strides = array<typename dyn_vec_extents::index_type, 1>;
using dyn_vec_layout  = layout_stride<dynamic_extent>;
using dyn_vec_mapping = typename dyn_mat_layout::template mapping<dyn_vec_extents>;

//- The actual traits type, with partial specializations below.
//
template<class T>
struct noe_mdspan_traits;

//- This partial specialization is used when no span interface is desired.
//
template<>
struct noe_mdspan_traits<void>
{
    using src_span_type = void;
    using col_span_type = void;
    using row_span_type = void;
    using sub_span_type = void;
    using tr_span_type  = void;
};

//- This partial specialization is used when an owning engine is fixed-size.
//
template<class T, ptrdiff_t X0, ptrdiff_t X1, class L, class A>
struct noe_mdspan_traits<basic_mdspan<T, extents<X0, X1>, L, A>>
{
    using src_span_type = basic_mdspan<T, extents<X0, X1>, layout_right, A>;
    using col_span_type = basic_mdspan<T, dyn_vec_extents, dyn_vec_layout, A>;
    using row_span_type = basic_mdspan<T, dyn_vec_extents, dyn_vec_layout, A>;
    using sub_span_type = basic_mdspan<T, dyn_mat_extents, dyn_mat_layout, A>;
    using tr_span_type  = basic_mdspan<T, extents<X1, X0>, layout_right, A>;
    using index_type    = typename src_span_type::index_type;

    static constexpr col_span_type  col_span(src_span_type const& s, index_type col);
    static constexpr row_span_type  row_span(src_span_type const& s, index_type row);
    static constexpr sub_span_type  sub_span(src_span_type const& s, index_type row, index_type col,
                                                                     index_type rows, index_type cols);
    static constexpr tr_span_type   tr_span(src_span_type const& s);
};


template<class T, ptrdiff_t X0, ptrdiff_t X1, class L, class A> constexpr
typename noe_mdspan_traits<basic_mdspan<T, extents<X0, X1>, L, A>>::col_span_type
noe_mdspan_traits<basic_mdspan<T, extents<X0, X1>, L, A>>::col_span
(src_span_type const& s, index_type col)
{
    return subspan(s, all, col);
}

template<class T, ptrdiff_t X0, ptrdiff_t X1, class L, class A> constexpr
typename noe_mdspan_traits<basic_mdspan<T, extents<X0, X1>, L, A>>::row_span_type
noe_mdspan_traits<basic_mdspan<T, extents<X0, X1>, L, A>>::row_span
(src_span_type const& s, index_type row)
{
    return subspan(s, row, all);
}

template<class T, ptrdiff_t X0, ptrdiff_t X1, class L, class A> constexpr
typename noe_mdspan_traits<basic_mdspan<T, extents<X0, X1>, L, A>>::sub_span_type
noe_mdspan_traits<basic_mdspan<T, extents<X0, X1>, L, A>>::sub_span
(src_span_type const& s, index_type row, index_type row_count, index_type col, index_type col_count)
{
    using pair_t = pair<index_type, index_type>;
    return subspan(s, pair_t(row, row + row_count), pair(col, col + col_count));
}

template<class T, ptrdiff_t X0, ptrdiff_t X1, class L, class A> constexpr
typename noe_mdspan_traits<basic_mdspan<T, extents<X0, X1>, L, A>>::tr_span_type
noe_mdspan_traits<basic_mdspan<T, extents<X0, X1>, L, A>>::tr_span(src_span_type const& s)
{
    return tr_span_type(s.data());
}


//- This partial specialization is used when an owning engine is dynamic.
//
template<class T, class A>
struct noe_mdspan_traits<basic_mdspan<T, dyn_mat_extents, dyn_mat_layout, A>>
{
    using src_span_type = basic_mdspan<T, dyn_mat_extents, dyn_mat_layout, A>;
    using col_span_type = basic_mdspan<T, dyn_vec_extents, dyn_vec_layout, A>;
    using row_span_type = basic_mdspan<T, dyn_vec_extents, dyn_vec_layout, A>;
    using sub_span_type = basic_mdspan<T, dyn_mat_extents, dyn_mat_layout, A>;
    using tr_span_type  = basic_mdspan<T, dyn_mat_extents, dyn_mat_layout, A>;
    using index_type    = typename src_span_type::index_type;

    static constexpr col_span_type  col_span(src_span_type const& s, index_type col);
    static constexpr row_span_type  row_span(src_span_type const& s, index_type row);
    static constexpr sub_span_type  sub_span(src_span_type const& s, index_type row, index_type col,
                                                                     index_type rows, index_type cols);
    static constexpr tr_span_type   tr_span(src_span_type const& s);
};

template<class T, class A> constexpr
typename noe_mdspan_traits<basic_mdspan<T, dyn_mat_extents, dyn_mat_layout, A>>::col_span_type
noe_mdspan_traits<basic_mdspan<T, dyn_mat_extents, dyn_mat_layout, A>>::col_span
(src_span_type const& s, index_type col)
{
    return subspan(s, all, col);
}

template<class T, class A> constexpr
typename noe_mdspan_traits<basic_mdspan<T, dyn_mat_extents, dyn_mat_layout, A>>::row_span_type
noe_mdspan_traits<basic_mdspan<T, dyn_mat_extents, dyn_mat_layout, A>>::row_span
(src_span_type const& s, index_type row)
{
    return subspan(s, row, all);
}

template<class T, class A> constexpr
typename noe_mdspan_traits<basic_mdspan<T, dyn_mat_extents, dyn_mat_layout, A>>::sub_span_type
noe_mdspan_traits<basic_mdspan<T, dyn_mat_extents, dyn_mat_layout, A>>::sub_span
(src_span_type const& s, index_type row, index_type row_count, index_type col, index_type col_count)
{
    using pair_t = pair<index_type, index_type>;
    return subspan(s, pair_t(row, row + row_count), pair(col, col + col_count));
}

template<class T, class A> constexpr
typename noe_mdspan_traits<basic_mdspan<T, dyn_mat_extents, dyn_mat_layout, A>>::tr_span_type
noe_mdspan_traits<basic_mdspan<T, dyn_mat_extents, dyn_mat_layout, A>>::tr_span(src_span_type const& s)
{
    dyn_mat_extents     ext(s.extent(1), s.extent(0));
    dyn_mat_strides     str{s.stride(1), s.stride(0)};
    dyn_mat_mapping     map(ext, str);

    return tr_span_type(s.data(), map);
}


//- Helper alias and function for column spans (needed by column engines).
//
template<class ST>
using noe_mdspan_column_t = typename noe_mdspan_traits<ST>::col_span_type;

template<class ST, class SZ> inline constexpr
typename noe_mdspan_traits<ST>::col_span_type
noe_mdspan_column(ST const& s, SZ col)
{
    using idx_t = typename noe_mdspan_traits<ST>::index_type;
    return noe_mdspan_traits<ST>::col_span(s, static_cast<idx_t>(col));
}

//- Helper alias and function for row spans (needed by row engines).
//
template<class ST>
using noe_mdspan_row_t = typename noe_mdspan_traits<ST>::row_span_type;

template<class ST, class SZ> inline constexpr
typename noe_mdspan_traits<ST>::row_span_type
noe_mdspan_row(ST const& s, SZ row)
{
    using idx_t = typename noe_mdspan_traits<ST>::index_type;
    return noe_mdspan_traits<ST>::row_span(s, static_cast<idx_t>(row));
}

//- Helper alias and function for submatrix spans (needed by submatrix engines).
//
template<class ST>
using noe_mdspan_submatrix_t = typename noe_mdspan_traits<ST>::sub_span_type;

template<class ST, class SZ> inline constexpr
typename noe_mdspan_traits<ST>::sub_span_type
noe_mdspan_submatrix(ST const& s, SZ row, SZ row_count, SZ col, SZ col_count)
{
    using idx_t = typename noe_mdspan_traits<ST>::index_type;

    return noe_mdspan_traits<ST>::sub_span(s,
                                           static_cast<idx_t>(row), static_cast<idx_t>(row_count),
                                           static_cast<idx_t>(col), static_cast<idx_t>(col_count));
}

//- Helper alias and function for transpose spans (needed by transpose engines).
//
template<class ST>
using noe_mdspan_transpose_t = typename noe_mdspan_traits<ST>::tr_span_type;

template<class ST> inline constexpr
typename noe_mdspan_traits<ST>::tr_span_type
noe_mdspan_transpose(ST const& s)
{
    return noe_mdspan_traits<ST>::tr_span(s);
}

//- Helper function to build strided mdspan objects for matrices.
//
template<class T, class ST> inline constexpr
basic_mdspan<T, dyn_mat_extents, dyn_mat_layout>
make_dyn_span(T* pdata, ST rows, ST cols, ST row_stride, ST col_stride = 1u)
{
    using idx_t = typename dyn_mat_extents::index_type;

    dyn_mat_extents     extents(static_cast<idx_t>(rows), static_cast<idx_t>(cols));
    dyn_mat_strides     strides{static_cast<idx_t>(row_stride), static_cast<idx_t>(col_stride)};
    dyn_mat_mapping     mapping(extents, strides);

    return basic_mdspan<T, dyn_mat_extents, dyn_mat_layout>(pdata, mapping);
}

#endif
//==================================================================================================
//  Traits type for choosing between three alternative traits-type parameters.  This is used
//  extensively in the private implementation for selecting arithmetic traits at compile time.
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
//  vector and matrix engines.  Note that in this implementation all allocated memory is default-
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

//- Alias template used for rebinding allocators.
//
template<class A1, class T1>
using rebind_alloc_t = typename allocator_traits<A1>::template rebind_alloc<T1>;


//==================================================================================================
//  Temporary replacement for std::swap (which is constexpr in C++20)
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
