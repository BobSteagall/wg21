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
//  This simple traits type and corresponding alias template are to used detect if a type is of
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
//  This traits type specifies important properties that are to be associated with an engine,
//  based on a given tag type.  These properties are represented by boolean values.
//==================================================================================================
//
template<class TAG>
struct engine_tag_traits
{
    static constexpr bool   is_scalar    = false;
    static constexpr bool   is_vector    = false;
    static constexpr bool   is_matrix    = false;
    static constexpr bool   is_readable  = false;
    static constexpr bool   is_initable  = false;
    static constexpr bool   is_resizable = false;
};

template<>
struct engine_tag_traits<scalar_engine_tag>
{
    static constexpr bool   is_scalar    = true;
    static constexpr bool   is_vector    = false;
    static constexpr bool   is_matrix    = false;
    static constexpr bool   is_readable  = false;
    static constexpr bool   is_initable  = false;
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
    static constexpr bool   is_initable  = false;
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
    static constexpr bool   is_initable  = false;
    static constexpr bool   is_resizable = false;
};

template<>
struct engine_tag_traits<initable_vector_engine_tag>
{
    static constexpr bool   is_scalar    = false;
    static constexpr bool   is_vector    = true;
    static constexpr bool   is_matrix    = false;
    static constexpr bool   is_readable  = true;
    static constexpr bool   is_writable  = true;
    static constexpr bool   is_initable  = true;
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
    static constexpr bool   is_initable  = true;
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
    static constexpr bool   is_initable  = false;
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
    static constexpr bool   is_initable  = false;
    static constexpr bool   is_resizable = false;
};

template<>
struct engine_tag_traits<initable_matrix_engine_tag>
{
    static constexpr bool   is_scalar    = false;
    static constexpr bool   is_vector    = false;
    static constexpr bool   is_matrix    = true;
    static constexpr bool   is_readable  = true;
    static constexpr bool   is_writable  = true;
    static constexpr bool   is_initable  = true;
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
    static constexpr bool   is_initable  = true;
    static constexpr bool   is_resizable = true;
};


//- These private variable templates are used as a convenience to report important engine
//  properties, using the engine_tag_traits type defined above.  They are used in the private
//  implementation below, as well as by a corresponding set of public variable templates
//  (i.e., is_*_engine_v<ET>).
//
template<class ET> inline constexpr
bool    is_scalar_engine_v = detail::engine_tag_traits<typename ET::engine_category>::is_scalar;

template<class ET> inline constexpr
bool    is_vector_engine_v = detail::engine_tag_traits<typename ET::engine_category>::is_vector;

template<class ET> inline constexpr
bool    is_matrix_engine_v = detail::engine_tag_traits<typename ET::engine_category>::is_matrix;

template<class ET> inline constexpr
bool    is_readable_engine_v = detail::engine_tag_traits<typename ET::engine_category>::is_readable;

template<class ET> inline constexpr
bool    is_writable_engine_v = detail::engine_tag_traits<typename ET::engine_category>::is_writable;

template<class ET> inline constexpr
bool    is_initable_engine_v = detail::engine_tag_traits<typename ET::engine_category>::is_initable;

template<class ET> inline constexpr
bool    is_resizable_engine_v = detail::engine_tag_traits<typename ET::engine_category>::is_resizable;

template<class ET1, class ET2> inline constexpr
bool    engines_match_v = (is_matrix_engine_v<ET1> && is_matrix_engine_v<ET2>) ||
                          (is_vector_engine_v<ET1> && is_vector_engine_v<ET2>) ||
                          (is_scalar_engine_v<ET1> && is_scalar_engine_v<ET2>);


//- These privatee alias templates are used by the vector and matrix class templates to
//  enable/disable various parts of their public interfaces via SFINAE.
//
template<class T2, class T>
using enable_if_convertible_element = enable_if_t<is_convertible_v<T2, T>, bool>;

template<class ET2, class T>
using enable_if_engine_has_convertible_element =
        enable_if_t<is_convertible_v<typename ET2::value_type, T>, bool>;

template<class ET2, class ET>
using enable_if_convertible_engine =
        enable_if_t<is_convertible_v<typename ET2::element_type, typename ET::element_type>, bool>;

template<class ET1, class ET2>
using enable_if_writable = enable_if_t<is_same_v<ET1, ET2> && is_writable_engine_v<ET1>, bool>;

template<class ET1, class ET2, class U>
using enable_if_initable = enable_if_t<is_same_v<ET1, ET2> && is_initable_engine_v<ET1>  &&
                                       is_convertible_v<U, typename ET1::value_type>, bool>;

template<class ET1, class ET2>
using enable_if_resizable = enable_if_t<is_same_v<ET1, ET2> && is_resizable_engine_v<ET1>, bool>;

template<class ET1, class ET2, class U>
using enable_if_init_list_ok = enable_if_t<is_same_v<ET1, ET2> && !is_resizable_engine_v<ET1> &&
                                           is_convertible_v<U, typename ET1::value_type>, bool>;


#ifdef LA_USE_MDSPAN
//==================================================================================================
//  The following are several detection idiom traits types, regular traits types, and alias
//  templates for determining the type aliases 'span_type' and 'const_span_type' that may be
//  present in an engine's public interface.  There are exactly three mutually-exclusive
//  possibilities, each based on the presence/absence of the type aliases:
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
//  If both aliases are void, then the results are both void.  If both aliases refer to
//  basic_mdspan, then the results are those aliases.  All other combinations are errors.
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
//  source engine type to be wrapped (ETT) and the desired tag type of the resulting non-owning
//  engine (NOETT).
//==================================================================================================
//
template<class ETT, class NOETT> struct noe_tag_chooser;

//------
//
template<>
struct noe_tag_chooser<readable_vector_engine_tag, readable_vector_engine_tag>
{
    using tag_type = readable_vector_engine_tag;
};

template<>
struct noe_tag_chooser<readable_vector_engine_tag, writable_vector_engine_tag>
{
    using tag_type = readable_vector_engine_tag;
};

//------
//
template<>
struct noe_tag_chooser<writable_vector_engine_tag, readable_vector_engine_tag>
{
    using tag_type = readable_vector_engine_tag;
};

template<>
struct noe_tag_chooser<writable_vector_engine_tag, writable_vector_engine_tag>
{
    using tag_type = writable_vector_engine_tag;
};

//------
//
template<>
struct noe_tag_chooser<initable_vector_engine_tag, readable_vector_engine_tag>
{
    using tag_type = readable_vector_engine_tag;
};

template<>
struct noe_tag_chooser<initable_vector_engine_tag, writable_vector_engine_tag>
{
    using tag_type = writable_vector_engine_tag;
};

//------
//
template<>
struct noe_tag_chooser<resizable_vector_engine_tag, readable_vector_engine_tag>
{
    using tag_type = readable_vector_engine_tag;
};

template<>
struct noe_tag_chooser<resizable_vector_engine_tag, writable_vector_engine_tag>
{
    using tag_type = writable_vector_engine_tag;
};

//------
//
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
struct noe_tag_chooser<initable_matrix_engine_tag, readable_matrix_engine_tag>
{
    using tag_type = readable_matrix_engine_tag;
};

template<>
struct noe_tag_chooser<initable_matrix_engine_tag, writable_matrix_engine_tag>
{
    using tag_type = writable_matrix_engine_tag;
};

template<>
struct noe_tag_chooser<initable_matrix_engine_tag, readable_vector_engine_tag>
{
    using tag_type = readable_vector_engine_tag;
};

template<>
struct noe_tag_chooser<initable_matrix_engine_tag, writable_vector_engine_tag>
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


//- This alias template provides a convenience interface to noe_tag_chooser.  It is used by
//  the matrix class template to determine the engine categories for view types it might return
//  representing rows, columns, submatrices, and transposes.
//
template<class ET, class NOETT>
using noe_category_t = typename noe_tag_chooser<typename ET::engine_category, NOETT>::tag_type;


//==================================================================================================
//  This traits type computes several of the nested type aliases that are used by the non-owning
//  engine (NOE) types defined by this implementation.  These nested types are determined by the
//  category tag of the source engine type to be wrapped (ET) and the tag type of the resulting
//  non-owning engine (NEWCAT).
//
//  This type (or some other compile-time computation like it) is necessary in order to determine
//  the nested type aliases 'pointer', 'reference', 'span_type', and 'const_span_type' that
//  appear in the public interfaces of the engine types as well as those of vector and matrix.
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
//  The following define a traits type and several associated facilities for determining an
//  mdspan type on behalf of a non-owning engine.
//==================================================================================================
//
//- First, these are some type alias helpers to reduce verbosity below.
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
    using source_span_type    = void;
    using rowcolumn_span_type = void;
    using submatrix_span_type = void;
    using transpose_span_type = void;
    using index_type          = void;
};


//------------------------------------------------------------------------
//- This partial specialization is used when an engine is one-dimensional.
//
template<class T, ptrdiff_t X0, class L, class A>
struct noe_mdspan_traits<basic_mdspan<T, extents<X0>, L, A>>
{
    using source_span_type    = basic_mdspan<T, extents<X0>, L, A>;
    using subvector_span_type = basic_mdspan<T, dyn_vec_extents, dyn_vec_layout, A>;
    using index_type          = typename source_span_type::index_type;
};


//------------------------------------------------------------------------
//- This partial specialization is used when an engine is two-dimensional.
//
template<class T, ptrdiff_t X0, ptrdiff_t X1, class L, class A>
struct noe_mdspan_traits<basic_mdspan<T, extents<X0, X1>, L, A>>
{
    using source_span_type    = basic_mdspan<T, extents<X0, X1>, L, A>;
    using rowcolumn_span_type = basic_mdspan<T, dyn_vec_extents, dyn_vec_layout, A>;
    using submatrix_span_type = basic_mdspan<T, dyn_mat_extents, dyn_mat_layout, A>;
    using transpose_span_type = basic_mdspan<T, dyn_mat_extents, dyn_mat_layout, A>;
    using index_type          = typename source_span_type::index_type;
};

//- The following are a helper alias template and function for specifying and returning subvector
//  spans (needed by subvector engines).
//
template<class ST>
using noe_mdspan_subvector_t = typename noe_mdspan_traits<ST>::subvector_span_type;

template<class ST, class SZ> inline constexpr
noe_mdspan_subvector_t<ST>
noe_mdspan_subvector(ST const& s, SZ idx, SZ count)
{
    using idx_t  = typename noe_mdspan_traits<ST>::index_type;
    using pair_t = pair<idx_t, idx_t>;

    pair_t  elem_pair(static_cast<idx_t>(idx), static_cast<idx_t>(idx + count));

    return subspan(s, elem_pair);
}


//- The following are a helper alias template and function for specifying and returning column
//  and row spans (needed by column and row engines).
//
template<class ST>
using noe_mdspan_rowcolumn_t = typename noe_mdspan_traits<ST>::rowcolumn_span_type;

template<class ST, class SZ> inline constexpr
noe_mdspan_rowcolumn_t<ST>
noe_mdspan_column(ST const& s, SZ col)
{
    using idx_t = typename noe_mdspan_traits<ST>::index_type;
    return subspan(s, all, static_cast<idx_t>(col));
}

template<class ST, class SZ> inline constexpr
noe_mdspan_rowcolumn_t<ST>
noe_mdspan_row(ST const& s, SZ row)
{
    using idx_t = typename noe_mdspan_traits<ST>::index_type;
    return subspan(s, static_cast<idx_t>(row), all);
}


//- The following are a helper alias template and function for specifying and returning submatrix
//  spans (needed by submatrix engines).
//
template<class ST>
using noe_mdspan_submatrix_t = typename noe_mdspan_traits<ST>::submatrix_span_type;

template<class ST, class SZ> inline constexpr
noe_mdspan_submatrix_t<ST>
noe_mdspan_submatrix(ST const& s, SZ row, SZ row_count, SZ col, SZ col_count)
{
    using idx_t  = typename noe_mdspan_traits<ST>::index_type;
    using pair_t = pair<idx_t, idx_t>;

    pair_t  row_pair(static_cast<idx_t>(row), static_cast<idx_t>(row + row_count));
    pair_t  col_pair(static_cast<idx_t>(col), static_cast<idx_t>(col + col_count));

    return subspan(s, row_pair, col_pair);
}


//- The following are a helper alias template and function for specifying and returning transpose
//  spans (needed by transpose engines).
//
template<class ST>
using noe_mdspan_transpose_t = typename noe_mdspan_traits<ST>::transpose_span_type;

template<class ST> inline constexpr
noe_mdspan_transpose_t<ST>
noe_mdspan_transpose(ST const& s)
{
    dyn_mat_extents     ext(s.extent(1), s.extent(0));
    dyn_mat_strides     str{s.stride(1), s.stride(0)};
    dyn_mat_mapping     map(ext, str);

    return noe_mdspan_transpose_t<ST>(s.data(), map);
}


//----------------------------------------------------------------------------------------------
//- This helper function is used to construct strided mdspan objects for dynamic matrix engines.
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
//  This traits type is used for choosing between three alternative traits-type parameters.  It
//  is used extensively in those sections of the private implementation where it is necessary to
//  choose between alternative arithmetic traits at compile time.
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
//  The following are some private helper functions for allocating/deallocating the memory used
//  by the dynamic vector and matrix engines.  Note that in this implementation all allocated
//  memory is default-constructed.  This means that elements lying in (currently) unused capacity
//  are also initialized; NB: this may or may not be what happens in the final version.
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


//- This alias template is used for rebinding std-conforming allocators in the dynamic engines.
//
template<class A1, class T1>
using rebind_alloc_t = typename allocator_traits<A1>::template rebind_alloc<T1>;


//==================================================================================================
//  The following is a temporary replacement for std::swap (which is constexpr in C++20).
//==================================================================================================
//
template<class T> inline constexpr
bool    is_movable_v = std::is_move_constructible_v<T> &&
                       std::is_move_assignable_v<T>;

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


//==================================================================================================
//  These helper functions are used inside engine member functions to validate a source engine
//  or initializer list prior to assignment to a destination engine.  If validation should fail,
//  they will:
//      A. cause a compile-time error when evaluated in a constexpr context; or,
//      B. cause an exception to be thrown at run-time.
//==================================================================================================
//
template<class ET, class ST> constexpr
void
check_source_engine_size(ET const& engine, ST elems)
{
    if (engine.size() != elems)
    {
        throw runtime_error("source engine size does not match destination vector engine size");
    }
}

template<class ET, class ST> constexpr
void
check_source_engine_size(ET const& engine, ST rows, ST cols)
{
    if (engine.rows() != rows  ||  engine.columns() != cols)
    {
        throw runtime_error("source engine size does not match destination matrix engine size");
    }
}


template<class T, class ST> constexpr
void
check_source_init_list(initializer_list<T> list, ST elems)
{
    if (list.size() != static_cast<size_t>(elems))
    {
        throw runtime_error("initializer_list size does not match vector engine size");
    }
}

template<class T> constexpr
void
check_source_init_list(initializer_list<initializer_list<T>> list)
{
    size_t  first_row_size = list.begin()->size();

    for (auto const row : list)
    {
        if (row.size() != first_row_size)
        {
            throw runtime_error("source 2-D initializer_list has invalid shape");
        }
    }
}

template<class T, class ST> constexpr
void
check_source_init_list(initializer_list<initializer_list<T>> list, ST rows, ST cols)
{
    size_t  first_row_size = list.begin()->size();

    for (auto row : list)
    {
        if (row.size() != first_row_size)
        {
            throw runtime_error("source 2-D initializer_list has invalid shape");
        }
    }

    if (list.size() != static_cast<size_t>(rows)  ||  first_row_size != static_cast<size_t>(cols))
    {
        throw runtime_error("source 2-D initializer_list size does not match destination matrix engine size");
    }
}

//==================================================================================================
//  These helper functions are used inside engine member functions to perform a generic assignment
//  of elements to a destination engine from a source engine or initializer list.  No checking for
//  compatible engine sizes is performed in these functions; it is assumed that the calling engine
//  will have already done such checking.
//==================================================================================================
//
template<class ET1, class ET2> constexpr
void
assign_from_vector_engine(ET1& dst, ET2 const& src)
{
    typename ET1::index_type     di = 0;
    typename ET2::index_type     si = 0;
    typename ET2::index_type     ni = src.size();

    for (;  si < ni;  ++di, ++si)
    {
        dst(di) = static_cast<typename ET1::value_type>(src(si));
    }
}

template<class ET1, class ET2> constexpr
void
assign_from_matrix_engine(ET1& dst, ET2 const& src)
{
    typename ET1::index_type     di = 0;
    typename ET1::index_type     dj = 0;
    typename ET2::index_type     si = 0;
    typename ET2::index_type     sj = 0;
    typename ET2::index_type     ni = src.rows();
    typename ET2::index_type     nj = src.columns();

    for (;  si < ni;  ++di, ++si)
    {
        for (dj = 0, sj = 0;  sj < nj;  ++dj, ++sj)
        {
            dst(di, dj) = static_cast<typename ET1::value_type>(src(si, sj));
        }
    }
}

//------
//
template<class ET, class T> constexpr void
assign_from_vector_list(ET& engine, initializer_list<T> rhs)
{
    using index_type = typename ET::index_type;
    using dest_elem = typename ET::value_type;
    using elem_iter = decltype(rhs.begin());

    index_type   di = 0;
    index_type   dn = engine.size();
    elem_iter   ep = rhs.begin();

    for (;  di < dn;  ++di, ++ep)
    {
        engine(di) = static_cast<dest_elem>(*ep);
    }
}

template<class ET, class T> constexpr void
assign_from_matrix_list(ET& engine, initializer_list<initializer_list<T>> rhs)
{
    using index_type = typename ET::index_type;
    using dest_elem = typename ET::value_type;
    using row_iter  = decltype(rhs.begin());
    using col_iter  = decltype(rhs.begin()->begin());

    index_type   di = 0;
    row_iter    rp = rhs.begin();

    for (;  di < engine.rows();  ++di, ++rp)
    {
        index_type   dj = 0;
        col_iter    cp = rp->begin();

        for (;  dj < engine.columns();  ++dj, ++cp)
        {
            engine(di, dj) = static_cast<dest_elem>(*cp);
        }
    }
}

//==================================================================================================
//  These helper functions are used to compare the contents of vector engines with that of other
//  vector engines, 1-D mdspans, and 1-D initializer lists.
//==================================================================================================
//
template<class ET1, class ET2> constexpr
bool
v_cmp_eq(ET1 const& lhs, ET2 const& rhs)
{
    using lhs_size = typename ET1::index_type;
    using rhs_size = typename ET2::index_type;

    lhs_size   i1 = 0;
    lhs_size   n1 = lhs.size();

    rhs_size   i2 = 0;
    rhs_size   n2 = rhs.size();

    if (n1 != static_cast<lhs_size>(n2)) return false;

    for (;  i1 < n1;  ++i1, ++i2)
    {
        if (lhs(i1) != rhs(i2)) return false;
    }
    return true;
}

template<class ET, class U> constexpr
bool
v_cmp_eq(ET const& lhs, initializer_list<U> rhs)
{
    if (static_cast<size_t>(lhs.size()) != rhs.size()) return false;

    using index_type = typename ET::index_type;
    using elem_iter  = decltype(rhs.begin());

    index_type  di = 0;
    index_type  dn = lhs.size();
    elem_iter   ep = rhs.begin();

    for (;  di < dn;  ++di, ++ep)
    {
        if (lhs(di) != *ep) return false;
    }
    return true;
}

#ifdef LA_USE_MDSPAN

template<class ET, class T, ptrdiff_t X0, class L, class A> constexpr
bool
v_cmp_eq(ET const& lhs, basic_mdspan<T, extents<X0>, L, A> const& rhs)
{
    using lhs_size = typename ET::index_type;
    using rhs_size = typename basic_mdspan<T, extents<X0>, L, A>::index_type;

    lhs_size    i1 = 0;
    lhs_size    e1 = lhs.size();

    rhs_size    i2 = 0;
    rhs_size    e2 = rhs.extent(0);

    if (e1 != static_cast<lhs_size>(e2)) return false;

    for (;  i1 < e1;  ++i1, ++i2)
    {
        if (lhs(i1) != rhs(i2)) return false;
    }
    return true;
}

#endif
//==================================================================================================
//  These helper functions are used to compare the contents of vector engines with that of other
//  vector engines, 1-D mdspans, and 1-D initializer lists.
//==================================================================================================
//
template<class ET1, class ET2> constexpr
bool
m_cmp_eq(ET1 const& lhs, ET2 const& rhs)
{
    using lhs_size = typename ET1::index_type;
    using rhs_size = typename ET2::index_type;

    lhs_size    i1 = 0;
    lhs_size    j1 = 0;
    lhs_size    r1 = lhs.rows();
    lhs_size    c1 = lhs.columns();

    rhs_size    i2 = 0;
    rhs_size    j2 = 0;
    rhs_size    r2 = rhs.rows();
    rhs_size    c2 = rhs.columns();

    if (r1 != static_cast<lhs_size>(r2)  ||  c1 != static_cast<lhs_size>(c2)) return false;

    for (;  i1 < r1;  ++i1, ++i2)
    {
        for (;  j1 < c1;  ++j1, ++j2)
        {
            if (lhs(i1, j1) != rhs(i2, j2)) return false;
        }
    }
    return true;
}

template<class ET, class U> constexpr
bool
m_cmp_eq(ET const& lhs, initializer_list<initializer_list<U>> rhs)
{
    size_t  first_row_size = rhs.begin()->size();

    for (auto row : rhs)
    {
        if (row.size() != first_row_size)
        {
            return false;
        }
    }

    if (static_cast<size_t>(lhs.rows()) != rhs.size()) return false;
    if (static_cast<size_t>(lhs.columns()) != first_row_size) return false;

    using index_type = typename ET::index_type;
    using row_iter   = decltype(rhs.begin());
    using col_iter   = decltype(rhs.begin()->begin());

    index_type  ei = 0;
    index_type  er = lhs.rows();
    index_type  ec = lhs.columns();
    row_iter    rp = rhs.begin();

    for (;  ei < er;  ++ei, ++rp)
    {
        index_type  ej = 0;
        col_iter    cp = rp->begin();

        for (;  ej < ec;  ++ej, ++cp)
        {
            if (lhs(ei, ej) != *cp) return false;
        }
    }
    return true;
}

#ifdef LA_USE_MDSPAN

template<class ET, class T, ptrdiff_t X0, ptrdiff_t X1, class L, class A> constexpr
bool
m_cmp_eq(ET const& lhs, basic_mdspan<T, extents<X0, X1>, L, A> const& rhs)
{
    using lhs_size = typename ET::index_type;
    using rhs_size = typename basic_mdspan<T, extents<X0, X1>, L, A>::index_type;

    lhs_size    i1 = 0;
    lhs_size    j1 = 0;
    lhs_size    r1 = lhs.rows();
    lhs_size    c1 = lhs.columns();

    rhs_size    i2 = 0;
    rhs_size    j2 = 0;
    rhs_size    r2 = rhs.extent(0);
    rhs_size    c2 = rhs.extent(1);

    if (r1 != static_cast<lhs_size>(r2)  ||  c1 != static_cast<lhs_size>(c2)) return false;

    for (;  i1 < r1;  ++i1, ++i2)
    {
        for (;  j1 < c1;  ++j1, ++j2)
        {
            if (lhs(i1, j1) != rhs(i2, j2)) return false;
        }
    }
    return true;
}

#endif

}       //- detail namespace
}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_IMPL_SUPPORT_HPP_DEFINED
