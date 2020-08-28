//==================================================================================================
//  File:       matrix_view_engine.hpp
//
//  Summary:    This header defines an engine that acts as a "view" of a subset of a matrix.
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_MATRIX_VIEW_ENGINE_HPP_DEFINED
#define LINEAR_ALGEBRA_MATRIX_VIEW_ENGINE_HPP_DEFINED

namespace STD_LA {
namespace detail {
//--------------------------------------------------------------------------------------------------
//  Types:  passthru_accessor<T, WA>
//          negation_accessor<T, WA>
//          conjugation_accessor<T, WW>
//
//  These are specialized accessor policy types used for negation, transpose, and hermitian views.
//  They wrap another accessor type WA for an element type T.  They differ in how they provide
//  access to the underlying element.
//--------------------------------------------------------------------------------------------------
//
template<class T, class WA = MDSPAN_NS::accessor_basic<T>>
struct passthru_accessor
{
    using offset_policy = passthru_accessor;
    using element_type  = T;
    using reference     = typename WA::reference;
    using pointer       = typename WA::pointer;

    constexpr pointer
    offset(pointer p, ptrdiff_t i) const noexcept
    {
        return WA().offset(p, i);
    }

    constexpr reference
    access(pointer p, ptrdiff_t i) const noexcept
    {
        return WA().access(p, i);
    }

    constexpr pointer
    decay(pointer p) const noexcept
    {
        return WA().decay(p);
    }
};

template<class T, class WA = MDSPAN_NS::accessor_basic<T>>
struct negation_accessor
{
    using offset_policy = negation_accessor;
    using element_type  = T;
    using reference     = T;
    using pointer       = typename WA::pointer;

    constexpr pointer
    offset(pointer p, ptrdiff_t i) const noexcept
    {
        return WA().offset(p, i);
    }

    constexpr reference
    access(pointer p, ptrdiff_t i) const noexcept
    {
        return -(WA().access(p, i));
    }

    constexpr pointer
    decay(pointer p) const noexcept
    {
        return WA().decay(p);
    }
};

template<class T, class WA = MDSPAN_NS::accessor_basic<T>>
struct conjugation_accessor
{
    using offset_policy = conjugation_accessor;
    using element_type  = T;
    using reference     = T;
    using pointer       = typename WA::pointer;

    constexpr pointer
    offset(pointer p, ptrdiff_t i) const noexcept
    {
        return WA().offset(p, i);
    }

    constexpr reference
    access(pointer p, ptrdiff_t i) const noexcept
    {
        return std::conj(WA().access(p, i));
    }

    constexpr pointer
    decay(pointer p) const noexcept
    {
        return WA().decay(p);
    }
};


//------------------------------------------------------------------------
//- The actual noe_mdspan_traits type, with partial specializations below.
//
template<class T>
struct mve_mdspan_traits;

//- This partial specialization is used when no span interface is desired.
//
template<>
struct mve_mdspan_traits<void>
{
    static constexpr bool   has_mdspan = false;

    using subvector_mdspan_type = void;
    using negation_mdspan_type  = void;
    using hermitian_mdspan_type = void;
    using submatrix_mdspan_type = void;
    using transpose_mdspan_type = void;
};

//- This partial specialization is used when an engine is one-dimensional.
//
template<class T, ptrdiff_t X0, class L, class A>
struct mve_mdspan_traits<basic_mdspan<T, extents<X0>, L, A>>
{
    static constexpr bool   has_mdspan = true;

    using dyn_extents = extents<dynamic_extent>;
    using dyn_strides = array<typename dyn_extents::index_type, 1>;
    using dyn_layout  = layout_stride<dynamic_extent>;
    using dyn_mapping = typename dyn_layout::template mapping<dyn_extents>;

    using negation_mdspan_type  = basic_mdspan<T, dyn_extents, dyn_layout, negation_accessor<T>>;
    using subvector_mdspan_type = basic_mdspan<T, dyn_extents, dyn_layout, A>;
};

//- These partial specializations are used when an engine is two-dimensional.
//
template<class T, ptrdiff_t X0, ptrdiff_t X1, class L, class A>
struct mve_mdspan_traits<basic_mdspan<T, extents<X0, X1>, L, A>>
{
    static constexpr bool   has_mdspan = true;

    using dyn_extents = extents<dynamic_extent, dynamic_extent>;
    using dyn_strides = array<typename dyn_extents::index_type, 2>;
    using dyn_layout  = layout_stride<dynamic_extent, dynamic_extent>;
    using dyn_mapping = typename dyn_layout::template mapping<dyn_extents>;

    using negation_mdspan_type  = basic_mdspan<T, dyn_extents, dyn_layout, negation_accessor<T, A>>;
    using hermitian_mdspan_type = basic_mdspan<T, dyn_extents, dyn_layout, conjugation_accessor<T, A>>;
    using transpose_mdspan_type = basic_mdspan<T, dyn_extents, dyn_layout, A>;
    using submatrix_mdspan_type = basic_mdspan<T, dyn_extents, dyn_layout, A>;

    template<class EST>
    static constexpr negation_mdspan_type
    make_negation_mdspan(EST const& s)
    {
        dyn_extents     ext(s.extent(0), s.extent(1));
        dyn_strides     str{s.stride(0), s.stride(1)};
        dyn_mapping     map(ext, str);

        return negation_mdspan_type(s.data(), map, negation_accessor<T, A>());
    }

    template<class EST>
    static constexpr hermitian_mdspan_type
    make_hermitian_mdspan(EST const& s)
    {
        dyn_extents     ext(s.extent(1), s.extent(0));
        dyn_strides     str{s.stride(1), s.stride(0)};
        dyn_mapping     map(ext, str);

        return hermitian_mdspan_type(s.data(), map, conjugation_accessor<T, A>());
    }

    template<class EST>
    static constexpr transpose_mdspan_type
    make_transpose_mdspan(EST const& s)
    {
        dyn_extents     ext(s.extent(1), s.extent(0));
        dyn_strides     str{s.stride(1), s.stride(0)};
        dyn_mapping     map(ext, str);

        return transpose_mdspan_type(s.data(), map, A());
    }
};

}       //- detail namespace
//==================================================================================================
//--------------------------------------------------------------------------------------------------
//  Class:      matrix_view
//
//  This public type is a container of tag sub-types whose purpose is to specify the functionality
//  of a matrix or vector view when used as a template argument to matrix_view_engine<ET, MVT>
//--------------------------------------------------------------------------------------------------
//
struct matrix_view
{
    struct subvector {};
    struct const_subvector {};

    struct const_negation {};
    struct const_hermitian {};

    struct transpose {};
    struct const_transpose {};

    struct submatrix {};
    struct const_submatrix {};

    struct column {};
    struct const_column {};

    struct row {};
    struct const_row {};
};


//--------------------------------------------------------------------------------------------------
//  Class Template:     matrix_view_engine<ET, MVT>
//
//  This partial specialization of matrix_storage_engine<T,X,A,L> implements an owning engine
//  for use by class template basic_vector<ET, OT>.  Specifically, it models a mathematical
//  vector having N elements, employing allocator A, and having element layout L.
//--------------------------------------------------------------------------------------------------
//
template<class ET, class MVT>    class matrix_view_engine;


template<class ET>
requires
    detail::readable_matrix_engine<ET>
class matrix_view_engine<ET, matrix_view::const_negation>
{
    using this_type        = matrix_view_engine;
    using engine_pointer   = ET const*;
    using support_traits   = detail::matrix_engine_support;
    using engine_span_type = detail::get_const_mdspan_type_t<ET>;
    using mdspan_traits    = detail::mve_mdspan_traits<engine_span_type>;

    static constexpr bool   has_mdspan = mdspan_traits::has_mdspan;

  public:
    using engine_type        = ET;
    using owning_engine_type = detail::get_owning_engine_type_t<ET>;
    using element_type       = typename engine_type::element_type;
    using reference          = typename engine_type::element_type;
    using const_reference    = typename engine_type::element_type;
    using index_type         = typename engine_type::index_type;
    using span_type          = typename mdspan_traits::negation_mdspan_type;
    using const_span_type    = span_type;

    //- Construct/copy/destroy
    //
    ~matrix_view_engine() noexcept = default;

    constexpr matrix_view_engine() noexcept
    :   mp_engine(nullptr)
    {}
    constexpr matrix_view_engine(matrix_view_engine&&) noexcept = default;
    constexpr matrix_view_engine(matrix_view_engine const&) = default;

    constexpr matrix_view_engine&     operator =(matrix_view_engine&&) noexcept = default;
    constexpr matrix_view_engine&     operator =(matrix_view_engine const&) = default;

    explicit constexpr
    matrix_view_engine(engine_type const& eng)
    :   mp_engine(&eng)
    {}

    //- Status
    //
    constexpr bool
    is_valid() const noexcept
    {
        return mp_engine != nullptr;
    }

    //- Size and capacity reporting.
    //
    constexpr index_type
    columns() const noexcept
    {
        return mp_engine->columns();
    }

    constexpr index_type
    rows() const noexcept
    {
        return mp_engine->rows();
    }

    constexpr index_type
    size() const noexcept
    {
        return mp_engine->size();
    }

    constexpr index_type
    column_capacity() const noexcept
    {
        return mp_engine->columns();
    }

    constexpr index_type
    row_capacity() const noexcept
    {
        return mp_engine->rows();
    }

    constexpr index_type
    capacity() const noexcept
    {
        return mp_engine->size();
    }

    //- Element access
    //
    constexpr reference
    operator ()(index_type i, index_type j) const
    {
        return -((*mp_engine)(i, j));
    }

    constexpr span_type
    span() const noexcept
    requires
        this_type::has_mdspan
    {
        return mdspan_traits::make_negation_mdspan(mp_engine->span());
    }

    //- Modifiers
    //
    constexpr void
    swap(matrix_view_engine& rhs) noexcept
    {
        support_traits::swap(mp_engine, rhs.mp_engine);
    }

  private:
    engine_type const*  mp_engine;
};


template<class ET>
requires
    detail::readable_matrix_engine<ET>
class matrix_view_engine<ET, matrix_view::const_hermitian>
{
    static constexpr bool   is_cx = detail::is_complex_v<typename ET::element_type>;

    using conj_el_type   = conditional_t<is_cx, typename ET::element_type, typename ET::const_reference>;
    using support_traits   = detail::matrix_engine_support;
    using engine_pointer = ET const*;

  public:
    using engine_type        = ET;
    using owning_engine_type = detail::get_owning_engine_type_t<ET>;
    using element_type       = typename engine_type::element_type;
    using reference          = conj_el_type;
    using const_reference    = conj_el_type;
    using index_type         = typename engine_type::index_type;
    using span_type          = detail::get_mdspan_type_t<ET>;
    using const_span_type    = detail::get_const_mdspan_type_t<ET>;

    //- Construct/copy/destroy
    //
    ~matrix_view_engine() noexcept = default;

    constexpr matrix_view_engine() noexcept
    :   mp_engine(nullptr)
    {}
    constexpr matrix_view_engine(matrix_view_engine&&) noexcept = default;
    constexpr matrix_view_engine(matrix_view_engine const&) = default;

    constexpr matrix_view_engine&     operator =(matrix_view_engine&&) noexcept = default;
    constexpr matrix_view_engine&     operator =(matrix_view_engine const&) = default;

    explicit constexpr
    matrix_view_engine(engine_type const& eng)
    :   mp_engine(&eng)
    {}

    //- Status
    //
    constexpr bool
    is_valid() const noexcept
    {
        return mp_engine != nullptr;
    }

    //- Size and capacity reporting.
    //
    constexpr index_type
    columns() const noexcept
    {
        return mp_engine->rows();
    }

    constexpr index_type
    rows() const noexcept
    {
        return mp_engine->columns();
    }

    constexpr index_type
    size() const noexcept
    {
        return mp_engine->size();
    }

    constexpr index_type
    column_capacity() const noexcept
    {
        return mp_engine->rows();
    }

    constexpr index_type
    row_capacity() const noexcept
    {
        return mp_engine->columns();
    }

    constexpr index_type
    capacity() const noexcept
    {
        return mp_engine->size();
    }

    //- Element access
    //
    constexpr reference
    operator ()(index_type i, index_type j) const
    {
        if constexpr (is_cx)
            return std::conj((*mp_engine)(j, i));
        else
            return (*mp_engine)(j, i);
    }

    //- Modifiers
    //
    constexpr void
    swap(matrix_view_engine& rhs) noexcept
    {
        support_traits::swap(mp_engine, rhs.mp_engine);
    }

  private:
    engine_pointer  mp_engine;
};


template<class ET>
requires
    detail::writable_matrix_engine<ET>
class matrix_view_engine<ET, matrix_view::transpose>
{
    using this_type           = matrix_view_engine;
    using engine_pointer      = ET*;
    using support_traits      = detail::matrix_engine_support;
    using mdspan_traits       = detail::mve_mdspan_traits<detail::get_mdspan_type_t<ET>>;
    using const_mdspan_traits = detail::mve_mdspan_traits<detail::get_const_mdspan_type_t<ET>>;

    static constexpr bool   has_mdspan = mdspan_traits::has_mdspan;

  public:
    using engine_type        = ET;
    using owning_engine_type = detail::get_owning_engine_type_t<ET>;
    using element_type       = typename engine_type::element_type;
    using reference          = typename engine_type::reference;
    using const_reference    = typename engine_type::const_reference;
    using index_type         = typename engine_type::index_type;
    using span_type          = typename mdspan_traits::transpose_mdspan_type;
    using const_span_type    = typename const_mdspan_traits::transpose_mdspan_type;

    //- Construct/copy/destroy
    //
    ~matrix_view_engine() noexcept = default;

    constexpr matrix_view_engine() noexcept
    :   mp_engine(nullptr)
    {}
    constexpr matrix_view_engine(matrix_view_engine&&) noexcept = default;
    constexpr matrix_view_engine(matrix_view_engine const&) = default;

    constexpr matrix_view_engine&     operator =(matrix_view_engine&&) noexcept = default;
    constexpr matrix_view_engine&     operator =(matrix_view_engine const&) = default;

    explicit constexpr
    matrix_view_engine(engine_type& eng)
    :   mp_engine(&eng)
    {}

    //- Status
    //
    constexpr bool
    is_valid() const noexcept
    {
        return mp_engine != nullptr;
    }

    //- Size and capacity reporting.
    //
    constexpr index_type
    columns() const noexcept
    {
        return mp_engine->rows();
    }

    constexpr index_type
    rows() const noexcept
    {
        return mp_engine->columns();
    }

    constexpr index_type
    size() const noexcept
    {
        return mp_engine->size();
    }

    constexpr index_type
    column_capacity() const noexcept
    {
        return mp_engine->rows();
    }

    constexpr index_type
    row_capacity() const noexcept
    {
        return mp_engine->columns();
    }

    constexpr index_type
    capacity() const noexcept
    {
        return mp_engine->size();
    }

    //- Element access
    //
    constexpr reference
    operator ()(index_type i, index_type j) const
    {
        return (*mp_engine)(j, i);
    }

    constexpr reference
    operator ()(index_type i) const
    requires
        detail::writable_and_1d_indexable_matrix_engine<engine_type>
    {
        return (*mp_engine)(i);
    }

    constexpr span_type
    span() const noexcept
    requires
        this_type::has_mdspan
    {
        return mdspan_traits::make_transpose_mdspan(mp_engine->span());
    }

    //- Modifiers
    //
    constexpr void
    swap(matrix_view_engine& rhs) noexcept
    {
        support_traits::swap(mp_engine, rhs.mp_engine);
    }

  private:
    engine_pointer  mp_engine;
};


template<class ET>
requires
    detail::readable_matrix_engine<ET>
class matrix_view_engine<ET, matrix_view::const_transpose>
{
    using this_type      = matrix_view_engine;
    using engine_pointer = ET const*;
    using support_traits = detail::matrix_engine_support;
    using mdspan_traits  = detail::mve_mdspan_traits<detail::get_const_mdspan_type_t<ET>>;

    static constexpr bool   has_mdspan = mdspan_traits::has_mdspan;

  public:
    using engine_type        = ET;
    using owning_engine_type = detail::get_owning_engine_type_t<ET>;
    using element_type       = typename engine_type::element_type;
    using reference          = typename engine_type::const_reference;
    using const_reference    = typename engine_type::const_reference;
    using index_type         = typename engine_type::index_type;
    using span_type          = typename mdspan_traits::transpose_mdspan_type;
    using const_span_type    = span_type;

    //- Construct/copy/destroy
    //
    ~matrix_view_engine() noexcept = default;

    constexpr matrix_view_engine() noexcept
    :   mp_engine(nullptr)
    {}
    constexpr matrix_view_engine(matrix_view_engine&&) noexcept = default;
    constexpr matrix_view_engine(matrix_view_engine const&) = default;

    constexpr matrix_view_engine&     operator =(matrix_view_engine&&) noexcept = default;
    constexpr matrix_view_engine&     operator =(matrix_view_engine const&) = default;

    explicit constexpr
    matrix_view_engine(engine_type const& eng)
    :   mp_engine(&eng)
    {}

    //- Status
    //
    constexpr bool
    is_valid() const noexcept
    {
        return mp_engine != nullptr;
    }

    //- Size and capacity reporting.
    //
    constexpr index_type
    columns() const noexcept
    {
        return mp_engine->rows();
    }

    constexpr index_type
    rows() const noexcept
    {
        return mp_engine->columns();
    }

    constexpr index_type
    size() const noexcept
    {
        return mp_engine->size();
    }

    constexpr index_type
    column_capacity() const noexcept
    {
        return mp_engine->rows();
    }

    constexpr index_type
    row_capacity() const noexcept
    {
        return mp_engine->columns();
    }

    constexpr index_type
    capacity() const noexcept
    {
        return mp_engine->size();
    }

    //- Element access
    //
    constexpr const_reference
    operator ()(index_type i, index_type j) const
    {
        return (*mp_engine)(j, i);
    }

    constexpr const_reference
    operator ()(index_type i) const
    requires
        detail::readable_and_1d_indexable_matrix_engine<engine_type>
    {
        return (*mp_engine)(i);
    }

    constexpr span_type
    span() const noexcept
    requires
        this_type::has_mdspan
    {
        return mdspan_traits::make_transpose_mdspan(mp_engine->span());
    }

    //- Modifiers
    //
    constexpr void
    swap(matrix_view_engine& rhs) noexcept
    {
        support_traits::swap(mp_engine, rhs.mp_engine);
    }

  private:
    engine_pointer  mp_engine;
};


template<class ET>
requires
    detail::writable_matrix_engine<ET>
class matrix_view_engine<ET, matrix_view::column>
{
    using support_traits   = detail::matrix_engine_support;
    using engine_pointer = ET*;

  public:
    using engine_type        = ET;
    using owning_engine_type = detail::get_owning_engine_type_t<ET>;
    using element_type       = typename engine_type::element_type;
    using reference          = typename engine_type::reference;
    using const_reference    = typename engine_type::const_reference;
    using index_type         = typename engine_type::index_type;
    using span_type          = detail::get_mdspan_type_t<ET>;
    using const_span_type    = detail::get_const_mdspan_type_t<ET>;

    //- Construct/copy/destroy
    //
    ~matrix_view_engine() noexcept = default;

    constexpr matrix_view_engine() noexcept
    :   mp_engine(nullptr)
    ,   m_column(0)
    {}
    constexpr matrix_view_engine(matrix_view_engine&&) noexcept = default;
    constexpr matrix_view_engine(matrix_view_engine const&) = default;

    constexpr matrix_view_engine&     operator =(matrix_view_engine&&) noexcept = default;
    constexpr matrix_view_engine&     operator =(matrix_view_engine const&) = default;

    explicit constexpr
    matrix_view_engine(engine_type& eng, index_type ci) noexcept
    :   mp_engine(&eng)
    ,   m_column(ci)
    {}

    //- Status
    //
    constexpr bool
    is_valid() const noexcept
    {
        return mp_engine != nullptr;
    }

    //- Size and capacity reporting.
    //
    constexpr index_type
    columns() const noexcept
    {
        return 1;
    }

    constexpr index_type
    rows() const noexcept
    {
        return mp_engine->rows();
    }

    constexpr index_type
    size() const noexcept
    {
        return mp_engine->rows();
    }

    constexpr index_type
    column_capacity() const noexcept
    {
        return 1;
    }

    constexpr index_type
    row_capacity() const noexcept
    {
        return mp_engine->rows();
    }

    constexpr index_type
    capacity() const noexcept
    {
        return mp_engine->rows();
    }

    //- Element access
    //
    constexpr reference
    operator ()(index_type i, index_type) const
    {
        return (*mp_engine)(i, m_column);
    }

    constexpr reference
    operator ()(index_type i) const
    {
        return (*mp_engine)(i, m_column);
    }

    //- Modifiers
    //
    constexpr void
    swap(matrix_view_engine& rhs) noexcept
    {
        support_traits::swap(*this, rhs);
    }

  private:
    engine_pointer  mp_engine;
    index_type      m_column;
};


template<class ET>
requires
    detail::readable_matrix_engine<ET>
class matrix_view_engine<ET, matrix_view::const_column>
{
    using support_traits   = detail::matrix_engine_support;
    using engine_pointer = ET const*;

  public:
    using engine_type        = ET;
    using owning_engine_type = detail::get_owning_engine_type_t<ET>;
    using element_type       = typename engine_type::element_type;
    using reference          = typename engine_type::const_reference;
    using const_reference    = typename engine_type::const_reference;
    using index_type         = typename engine_type::index_type;
    using span_type          = detail::get_mdspan_type_t<ET>;
    using const_span_type    = detail::get_const_mdspan_type_t<ET>;

    //- Construct/copy/destroy
    //
    ~matrix_view_engine() noexcept = default;

    constexpr matrix_view_engine() noexcept
    :   mp_engine(nullptr)
    ,   m_column(0)
    {}
    constexpr matrix_view_engine(matrix_view_engine&&) noexcept = default;
    constexpr matrix_view_engine(matrix_view_engine const&) = default;

    constexpr matrix_view_engine&     operator =(matrix_view_engine&&) noexcept = default;
    constexpr matrix_view_engine&     operator =(matrix_view_engine const&) = default;

    explicit constexpr
    matrix_view_engine(engine_type const& eng, index_type ci) noexcept
    :   mp_engine(&eng)
    ,   m_column(ci)
    {}

    //- Status
    //
    constexpr bool
    is_valid() const noexcept
    {
        return mp_engine != nullptr;
    }

    //- Size and capacity reporting.
    //
    constexpr index_type
    columns() const noexcept
    {
        return 1;
    }

    constexpr index_type
    rows() const noexcept
    {
        return mp_engine->rows();
    }

    constexpr index_type
    size() const noexcept
    {
        return mp_engine->rows();
    }

    constexpr index_type
    column_capacity() const noexcept
    {
        return 1;
    }

    constexpr index_type
    row_capacity() const noexcept
    {
        return mp_engine->rows();
    }

    constexpr index_type
    capacity() const noexcept
    {
        return mp_engine->rows();
    }

    //- Element access
    //
    constexpr const_reference
    operator ()(index_type i, index_type) const
    {
        return (*mp_engine)(i, m_column);
    }

    constexpr const_reference
    operator ()(index_type i) const
    {
        return (*mp_engine)(i, m_column);
    }

    //- Modifiers
    //
    constexpr void
    swap(matrix_view_engine& rhs) noexcept
    {
        support_traits::swap(*this, rhs);
    }

  private:
    engine_pointer  mp_engine;
    index_type      m_column;
};


template<class ET>
requires
    detail::writable_matrix_engine<ET>
class matrix_view_engine<ET, matrix_view::row>
{
    using support_traits   = detail::matrix_engine_support;
    using engine_pointer = ET*;

  public:
    using engine_type        = ET;
    using owning_engine_type = detail::get_owning_engine_type_t<ET>;
    using element_type       = typename engine_type::element_type;
    using reference          = typename engine_type::reference;
    using const_reference    = typename engine_type::const_reference;
    using index_type         = typename engine_type::index_type;
    using span_type          = detail::get_mdspan_type_t<ET>;
    using const_span_type    = detail::get_const_mdspan_type_t<ET>;

    //- Construct/copy/destroy
    //
    ~matrix_view_engine() noexcept = default;

    constexpr matrix_view_engine() noexcept
    :   mp_engine(nullptr)
    ,   m_row(0)
    {}
    constexpr matrix_view_engine(matrix_view_engine&&) noexcept = default;
    constexpr matrix_view_engine(matrix_view_engine const&) = default;

    constexpr matrix_view_engine&     operator =(matrix_view_engine&&) noexcept = default;
    constexpr matrix_view_engine&     operator =(matrix_view_engine const&) = default;

    explicit constexpr
    matrix_view_engine(engine_type& eng, index_type ri) noexcept
    :   mp_engine(&eng)
    ,   m_row(ri)
    {}

    //- Status
    //
    constexpr bool
    is_valid() const noexcept
    {
        return mp_engine != nullptr;
    }

    //- Size and capacity reporting.
    //
    constexpr index_type
    columns() const noexcept
    {
        return mp_engine->columns();
    }

    constexpr index_type
    rows() const noexcept
    {
        return 1;
    }

    constexpr index_type
    size() const noexcept
    {
        return mp_engine->columns();
    }

    constexpr index_type
    column_capacity() const noexcept
    {
        return mp_engine->columns();
    }

    constexpr index_type
    row_capacity() const noexcept
    {
        return 1;
    }

    constexpr index_type
    capacity() const noexcept
    {
        return mp_engine->columns();
    }

    //- Element access
    //
    constexpr reference
    operator ()(index_type, index_type j) const
    {
        return (*mp_engine)(m_row, j);
    }

    constexpr reference
    operator ()(index_type j) const
    {
        return (*mp_engine)(m_row, j);
    }

    //- Modifiers
    //
    constexpr void
    swap(matrix_view_engine& rhs) noexcept
    {
        support_traits::swap(*this, rhs);
    }

  private:
    engine_pointer  mp_engine;
    index_type      m_row;
};


template<class ET>
requires
    detail::readable_matrix_engine<ET>
class matrix_view_engine<ET, matrix_view::const_row>
{
    using support_traits   = detail::matrix_engine_support;
    using engine_pointer = ET const*;

  public:
    using engine_type        = ET;
    using owning_engine_type = detail::get_owning_engine_type_t<ET>;
    using element_type       = typename engine_type::element_type;
    using reference          = typename engine_type::const_reference;
    using const_reference    = typename engine_type::const_reference;
    using index_type         = typename engine_type::index_type;
    using span_type          = detail::get_mdspan_type_t<ET>;
    using const_span_type    = detail::get_const_mdspan_type_t<ET>;

    //- Construct/copy/destroy
    //
    ~matrix_view_engine() noexcept = default;

    constexpr matrix_view_engine() noexcept
    :   mp_engine(nullptr)
    ,   m_row(0)
    {}
    constexpr matrix_view_engine(matrix_view_engine&&) noexcept = default;
    constexpr matrix_view_engine(matrix_view_engine const&) = default;

    constexpr matrix_view_engine&     operator =(matrix_view_engine&&) noexcept = default;
    constexpr matrix_view_engine&     operator =(matrix_view_engine const&) = default;

    explicit constexpr
    matrix_view_engine(engine_type const& eng, index_type ri) noexcept
    :   mp_engine(&eng)
    ,   m_row(ri)
    {}

    //- Status
    //
    constexpr bool
    is_valid() const noexcept
    {
        return mp_engine != nullptr;
    }

    //- Size and capacity reporting.
    //
    constexpr index_type
    columns() const noexcept
    {
        return mp_engine->columns();
    }

    constexpr index_type
    rows() const noexcept
    {
        return 1;
    }

    constexpr index_type
    size() const noexcept
    {
        return mp_engine->columns();
    }

    constexpr index_type
    column_capacity() const noexcept
    {
        return mp_engine->columns();
    }

    constexpr index_type
    row_capacity() const noexcept
    {
        return 1;
    }

    constexpr index_type
    capacity() const noexcept
    {
        return mp_engine->columns();
    }

    //- Element access
    //
    constexpr const_reference
    operator ()(index_type, index_type j) const
    {
        return (*mp_engine)(m_row, j);
    }

    constexpr const_reference
    operator ()(index_type j) const
    {
        return (*mp_engine)(m_row, j);
    }

    //- Modifiers
    //
    constexpr void
    swap(matrix_view_engine& rhs) noexcept
    {
        support_traits::swap(*this, rhs);
    }

  private:
    engine_pointer  mp_engine;
    index_type      m_row;
};


template<class ET>
requires
    detail::writable_matrix_engine<ET>
class matrix_view_engine<ET, matrix_view::submatrix>
{
    using support_traits   = detail::matrix_engine_support;
    using engine_pointer = ET*;

  public:
    using engine_type        = ET;
    using owning_engine_type = detail::get_owning_engine_type_t<ET>;
    using element_type       = typename engine_type::element_type;
    using reference          = typename engine_type::reference;
    using const_reference    = typename engine_type::const_reference;
    using index_type         = typename engine_type::index_type;
    using span_type          = detail::get_mdspan_type_t<ET>;
    using const_span_type    = detail::get_const_mdspan_type_t<ET>;

    //- Construct/copy/destroy
    //
    ~matrix_view_engine() noexcept = default;

    constexpr matrix_view_engine() noexcept
    :   mp_engine(nullptr)
    ,   m_row_start(0)
    ,   m_row_count(0)
    ,   m_col_start(0)
    ,   m_col_count(0)
    {}
    constexpr matrix_view_engine(matrix_view_engine&&) noexcept = default;
    constexpr matrix_view_engine(matrix_view_engine const&) = default;

    constexpr matrix_view_engine&     operator =(matrix_view_engine&&) noexcept = default;
    constexpr matrix_view_engine&     operator =(matrix_view_engine const&) = default;

    explicit constexpr
    matrix_view_engine(engine_type& eng, index_type ri, index_type rn, index_type ci, index_type cn) noexcept
    :   mp_engine(&eng)
    ,   m_row_start(ri)
    ,   m_row_count(rn)
    ,   m_col_start(ci)
    ,   m_col_count(cn)
    {}

    //- Status
    //
    constexpr bool
    is_valid() const noexcept
    {
        return mp_engine != nullptr;
    }

    //- Size and capacity reporting.
    //
    constexpr index_type
    columns() const noexcept
    {
        return m_col_count;
    }

    constexpr index_type
    rows() const noexcept
    {
        return m_row_count;
    }

    constexpr index_type
    size() const noexcept
    {
        return m_row_count * m_col_count;
    }

    constexpr index_type
    column_capacity() const noexcept
    {
        return m_col_count;
    }

    constexpr index_type
    row_capacity() const noexcept
    {
        return m_row_count;
    }

    constexpr index_type
    capacity() const noexcept
    {
        return m_row_count * m_col_count;
    }

    //- Element access
    //
    constexpr reference
    operator ()(index_type i, index_type j) const
    {
        return (*mp_engine)(i + m_row_start, j + m_col_start);
    }

    constexpr reference
    operator ()(index_type i) const
    requires
        detail::writable_and_1d_indexable_matrix_engine<engine_type>
    {
        return (*mp_engine)(i);
    }

    //- Modifiers
    //
    constexpr void
    swap(matrix_view_engine& rhs) noexcept
    {
        support_traits::swap(*this, rhs);
    }

  private:
    engine_pointer  mp_engine;
    index_type      m_row_start;
    index_type      m_row_count;
    index_type      m_col_start;
    index_type      m_col_count;
};


template<class ET>
requires
    detail::readable_matrix_engine<ET>
class matrix_view_engine<ET, matrix_view::const_submatrix>
{
    using support_traits   = detail::matrix_engine_support;
    using engine_pointer = ET const*;

  public:
    using engine_type        = ET;
    using owning_engine_type = detail::get_owning_engine_type_t<ET>;
    using element_type       = typename engine_type::element_type;
    using reference          = typename engine_type::const_reference;
    using const_reference    = typename engine_type::const_reference;
    using index_type         = typename engine_type::index_type;
    using span_type          = detail::get_mdspan_type_t<ET>;
    using const_span_type    = detail::get_const_mdspan_type_t<ET>;

    //- Construct/copy/destroy
    //
    ~matrix_view_engine() noexcept = default;

    constexpr matrix_view_engine() noexcept
    :   mp_engine(nullptr)
    ,   m_row_start(0)
    ,   m_row_count(0)
    ,   m_col_start(0)
    ,   m_col_count(0)
    {}
    constexpr matrix_view_engine(matrix_view_engine&&) noexcept = default;
    constexpr matrix_view_engine(matrix_view_engine const&) = default;

    constexpr matrix_view_engine&     operator =(matrix_view_engine&&) noexcept = default;
    constexpr matrix_view_engine&     operator =(matrix_view_engine const&) = default;

    explicit constexpr
    matrix_view_engine(engine_type const& eng, index_type ri, index_type rn, index_type ci, index_type cn) noexcept
    :   mp_engine(&eng)
    ,   m_row_start(ri)
    ,   m_row_count(rn)
    ,   m_col_start(ci)
    ,   m_col_count(cn)
    {}

    //- Status
    //
    constexpr bool
    is_valid() const noexcept
    {
        return mp_engine != nullptr;
    }

    //- Size and capacity reporting.
    //
    constexpr index_type
    columns() const noexcept
    {
        return m_col_count;
    }

    constexpr index_type
    rows() const noexcept
    {
        return m_row_count;
    }

    constexpr index_type
    size() const noexcept
    {
        return m_row_count * m_col_count;
    }

    constexpr index_type
    column_capacity() const noexcept
    {
        return m_col_count;
    }

    constexpr index_type
    row_capacity() const noexcept
    {
        return m_row_count;
    }

    constexpr index_type
    capacity() const noexcept
    {
        return m_row_count * m_col_count;
    }

    //- Element access
    //
    constexpr const_reference
    operator ()(index_type i, index_type j) const
    {
        return (*mp_engine)(i + m_row_start, j + m_col_start);
    }

    constexpr const_reference
    operator ()(index_type i) const
    requires
        detail::readable_and_1d_indexable_matrix_engine<engine_type>
    {
        return (*mp_engine)(i);
    }

    //- Modifiers
    //
    constexpr void
    swap(matrix_view_engine& rhs) noexcept
    {
        support_traits::swap(*this, rhs);
    }

  private:
    engine_pointer  mp_engine;
    index_type      m_row_start;
    index_type      m_row_count;
    index_type      m_col_start;
    index_type      m_col_count;
};

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_MATRIX_VIEW_ENGINE_HPP_DEFINED
