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
//          conjugate_accessor<T, WW>
//
//  These are specialized mdspan accessor policy types used for negation, transpose, and hermitian
//  views.  They wrap another accessor type WA for an element type T.  They differ in how they
//  provide access to the underlying element.
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
struct conjugate_accessor
{
    using offset_policy = conjugate_accessor;
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


//--------------------------------------------------------------------------------------------------
//  Class Template:     mve_mdspan_traits<T>
//
//  This traits class template provides services for determining the types, and computing the
//  values, of mdspan object that can index the set of elements represented by a view object.
//--------------------------------------------------------------------------------------------------
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


//- This partial specialization is used when a host engine is one-dimensional.
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
    using conjugate_mdspan_type = basic_mdspan<T, dyn_extents, dyn_layout, conjugate_accessor<T, A>>;
    using subvector_mdspan_type = basic_mdspan<T, dyn_extents, dyn_layout, A>;

    template<class EST>
    static constexpr negation_mdspan_type
    make_negation(EST const& s)
    {
        dyn_extents     ext(s.extent(0));
        dyn_strides     str{s.stride(0)};
        dyn_mapping     map(ext, str);

        return negation_mdspan_type(s.data(), map, negation_accessor<T, A>());
    }

    template<class EST>
    static constexpr conjugate_mdspan_type
    make_conjugate(EST const& s)
    {
        dyn_extents     ext(s.extent(0));
        dyn_strides     str{s.stride(0)};
        dyn_mapping     map(ext, str);

        return conjugate_mdspan_type(s.data(), map, conjugate_accessor<T, A>());
    }

    template<class EST, class S1, class S2>
    static constexpr subvector_mdspan_type
    make_subvector(EST const& s, S1 start, S2 count)
    {
        using idx_t  = decltype(dynamic_extent);
        using pair_t = std::pair<idx_t, idx_t>;

        pair_t  elem_set(static_cast<idx_t>(start), static_cast<idx_t>(start + count));

        if constexpr (std::is_same_v<EST, subvector_mdspan_type>)
        {
            return subspan(s, elem_set);
        }
        else
        {
            dyn_extents     ext(s.extent(0));
            dyn_strides     str{s.stride(0)};
            dyn_mapping     map(ext, str);

            return subspan(subvector_mdspan_type(s.data(), map), elem_set);
        }
    }
};


//- This partial specialization is used when a host engine is two-dimensional.
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
    using conjugate_mdspan_type = basic_mdspan<T, dyn_extents, dyn_layout, conjugate_accessor<T, A>>;
    using hermitian_mdspan_type = basic_mdspan<T, dyn_extents, dyn_layout, conjugate_accessor<T, A>>;
    using transpose_mdspan_type = basic_mdspan<T, dyn_extents, dyn_layout, A>;
    using submatrix_mdspan_type = basic_mdspan<T, dyn_extents, dyn_layout, A>;

    template<class EST>
    static constexpr negation_mdspan_type
    make_negation(EST const& s)
    {
        dyn_extents     ext(s.extent(0), s.extent(1));
        dyn_strides     str{s.stride(0), s.stride(1)};
        dyn_mapping     map(ext, str);

        return negation_mdspan_type(s.data(), map, negation_accessor<T, A>());
    }

    template<class EST>
    static constexpr conjugate_mdspan_type
    make_conjugate(EST const& s)
    {
        dyn_extents     ext(s.extent(0), s.extent(1));
        dyn_strides     str{s.stride(0), s.stride(1)};
        dyn_mapping     map(ext, str);

        return conjugate_mdspan_type(s.data(), map, conjugate_accessor<T, A>());
    }

    template<class EST>
    static constexpr hermitian_mdspan_type
    make_hermitian(EST const& s)
    {
        dyn_extents     ext(s.extent(1), s.extent(0));
        dyn_strides     str{s.stride(1), s.stride(0)};
        dyn_mapping     map(ext, str);

        return hermitian_mdspan_type(s.data(), map, conjugate_accessor<T, A>());
    }

    template<class EST>
    static constexpr transpose_mdspan_type
    make_transpose(EST const& s)
    {
        dyn_extents     ext(s.extent(1), s.extent(0));
        dyn_strides     str{s.stride(1), s.stride(0)};
        dyn_mapping     map(ext, str);

        return transpose_mdspan_type(s.data(), map, A());
    }

    template<class EST, class S1, class S2, class S3, class S4>
    static constexpr submatrix_mdspan_type
    make_submatrix(EST const& s, S1 row, S2 row_count, S3 col, S4 col_count)
    {
        using idx_t  = decltype(dynamic_extent);
        using pair_t = std::pair<idx_t, idx_t>;

        pair_t  row_set(static_cast<idx_t>(row), static_cast<idx_t>(row + row_count));
        pair_t  col_set(static_cast<idx_t>(col), static_cast<idx_t>(col + col_count));

        if constexpr (std::is_same_v<EST, submatrix_mdspan_type>)
        {
            return subspan(s, row_set, col_set);
        }
        else
        {
            dyn_extents     ext(s.extent(0), s.extent(1));
            dyn_strides     str{s.stride(0), s.stride(1)};
            dyn_mapping     map(ext, str);

            return subspan(submatrix_mdspan_type(s.data(), map), row_set, col_set);
        }
    }
};

}       //- detail namespace

//==================================================================================================
//  MATRIX VIEW ENGINES
//==================================================================================================
//--------------------------------------------------------------------------------------------------
//  Partial Specialization:     matrix_view_engine<ET, matrix_view::const_negation>
//
//  This partial specialization of matrix_view_engine<ET, MVT> implements a non-owning engine
//  for use by class template basic_matrix<ET, OT>.  It provides a read-only view of a host
//  matrix engine that expresses the element-wise negation of that engine.
//--------------------------------------------------------------------------------------------------
//
template<class ET>
requires
    detail::readable_matrix_engine<ET>
class matrix_view_engine<ET, matrix_view::const_negation>
{
    using this_type           = matrix_view_engine;
    using engine_pointer      = ET const*;
    using support_traits      = detail::matrix_engine_support;
    using const_mdspan_traits = detail::mve_mdspan_traits<detail::get_const_mdspan_type_t<ET>>;

    static constexpr bool   has_mdspan = const_mdspan_traits::has_mdspan;

  public:
    using engine_type        = ET;
    using owning_engine_type = detail::get_owning_engine_type_t<ET>;
    using element_type       = typename engine_type::element_type;
    using reference          = typename engine_type::element_type;
    using const_reference    = typename engine_type::element_type;
    using index_type         = typename engine_type::index_type;
    using span_type          = typename const_mdspan_traits::negation_mdspan_type;
    using const_span_type    = typename const_mdspan_traits::negation_mdspan_type;

    //- Construct/copy/destroy
    //
    ~matrix_view_engine() noexcept = default;

    constexpr matrix_view_engine() noexcept
    :   mp_engine(nullptr)
    {}
    constexpr matrix_view_engine(matrix_view_engine&&) noexcept = default;
    constexpr matrix_view_engine(matrix_view_engine const&) = default;

    constexpr matrix_view_engine&   operator =(matrix_view_engine&&) noexcept = default;
    constexpr matrix_view_engine&   operator =(matrix_view_engine const&) = default;

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
    constexpr const_reference
    operator ()(index_type i, index_type j) const
    {
        return -((*mp_engine)(i, j));
    }

    constexpr span_type
    span() const noexcept
    requires
        this_type::has_mdspan
    {
        return const_mdspan_traits::make_negation(mp_engine->span());
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


//--------------------------------------------------------------------------------------------------
//  Partial Specialization:     matrix_view_engine<ET, matrix_view::const_conjugate>
//
//  This partial specialization of matrix_view_engine<ET, MVT> implements a non-owning engine
//  for use by class template basic_matrix<ET, OT>.  It provides a read-only view of a host
//  matrix engine that expresses the element-wise complex conjugate of that engine.
//--------------------------------------------------------------------------------------------------
//
template<class ET>
requires
    detail::readable_matrix_engine<ET>
class matrix_view_engine<ET, matrix_view::const_conjugate>
{
    static constexpr bool   is_cx = detail::is_complex_v<typename ET::element_type>;

    using this_type           = matrix_view_engine;
    using engine_pointer      = ET const*;
    using conj_elem_type      = conditional_t<is_cx, typename ET::element_type, typename ET::const_reference>;
    using support_traits      = detail::matrix_engine_support;
    using const_mdspan_traits = detail::mve_mdspan_traits<detail::get_const_mdspan_type_t<ET>>;

    static constexpr bool   has_mdspan = const_mdspan_traits::has_mdspan;

  public:
    using engine_type        = ET;
    using owning_engine_type = detail::get_owning_engine_type_t<ET>;
    using element_type       = typename engine_type::element_type;
    using reference          = conj_elem_type;
    using const_reference    = conj_elem_type;
    using index_type         = typename engine_type::index_type;
    using span_type          = typename const_mdspan_traits::conjugate_mdspan_type;
    using const_span_type    = typename const_mdspan_traits::conjugate_mdspan_type;

    //- Construct/copy/destroy
    //
    ~matrix_view_engine() noexcept = default;

    constexpr matrix_view_engine() noexcept
    :   mp_engine(nullptr)
    {}
    constexpr matrix_view_engine(matrix_view_engine&&) noexcept = default;
    constexpr matrix_view_engine(matrix_view_engine const&) = default;

    constexpr matrix_view_engine&   operator =(matrix_view_engine&&) noexcept = default;
    constexpr matrix_view_engine&   operator =(matrix_view_engine const&) = default;

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
    constexpr const_reference
    operator ()(index_type i, index_type j) const
    {
        if constexpr (is_cx)
            return std::conj((*mp_engine)(i, j));
        else
            return (*mp_engine)(i, j);
    }

    constexpr span_type
    span() const noexcept
    requires
        this_type::has_mdspan
    {
        return const_mdspan_traits::make_conjugate(mp_engine->span());
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


//--------------------------------------------------------------------------------------------------
//  Partial Specialization:     matrix_view_engine<ET, matrix_view::const_hermitian>
//
//  This partial specialization of matrix_view_engine<ET, MVT> implements a non-owning engine
//  for use by class template basic_matrix<ET, OT>.  It provides a read-only view of a host
//  matrix engine that expresses the complex conjugate transpose of that engine.
//--------------------------------------------------------------------------------------------------
//
template<class ET>
requires
    detail::readable_matrix_engine<ET>
class matrix_view_engine<ET, matrix_view::const_hermitian>
{
    static constexpr bool   is_cx = detail::is_complex_v<typename ET::element_type>;

    using this_type           = matrix_view_engine;
    using engine_pointer      = ET const*;
    using conj_elem_type      = conditional_t<is_cx, typename ET::element_type, typename ET::const_reference>;
    using support_traits      = detail::matrix_engine_support;
    using const_mdspan_traits = detail::mve_mdspan_traits<detail::get_const_mdspan_type_t<ET>>;

    static constexpr bool   has_mdspan = const_mdspan_traits::has_mdspan;

  public:
    using engine_type        = ET;
    using owning_engine_type = detail::get_owning_engine_type_t<ET>;
    using element_type       = typename engine_type::element_type;
    using reference          = conj_elem_type;
    using const_reference    = conj_elem_type;
    using index_type         = typename engine_type::index_type;
    using span_type          = typename const_mdspan_traits::hermitian_mdspan_type;
    using const_span_type    = typename const_mdspan_traits::hermitian_mdspan_type;

    //- Construct/copy/destroy
    //
    ~matrix_view_engine() noexcept = default;

    constexpr matrix_view_engine() noexcept
    :   mp_engine(nullptr)
    {}
    constexpr matrix_view_engine(matrix_view_engine&&) noexcept = default;
    constexpr matrix_view_engine(matrix_view_engine const&) = default;

    constexpr matrix_view_engine&   operator =(matrix_view_engine&&) noexcept = default;
    constexpr matrix_view_engine&   operator =(matrix_view_engine const&) = default;

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
        if constexpr (is_cx)
            return std::conj((*mp_engine)(j, i));
        else
            return (*mp_engine)(j, i);
    }

    constexpr span_type
    span() const noexcept
    requires
        this_type::has_mdspan
    {
        return const_mdspan_traits::make_hermitian(mp_engine->span());
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


//--------------------------------------------------------------------------------------------------
//  Partial Specialization:     matrix_view_engine<ET, matrix_view::identity>
//
//  This partial specialization of matrix_view_engine<ET, MVT> implements a non-owning engine
//  for use by class template basic_matrix<ET, OT>.  It provides a read-write view of a host
//  matrix engine that is the same as that engine.
//--------------------------------------------------------------------------------------------------
//
template<class ET>
requires
    detail::writable_matrix_engine<ET>
class matrix_view_engine<ET, matrix_view::identity>
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
    using span_type          = typename engine_type::span_type;
    using const_span_type    = typename engine_type::const_span_type;

    //- Construct/copy/destroy
    //
    ~matrix_view_engine() noexcept = default;

    constexpr matrix_view_engine() noexcept
    :   mp_engine(nullptr)
    {}
    constexpr matrix_view_engine(matrix_view_engine&&) noexcept = default;
    constexpr matrix_view_engine(matrix_view_engine const&) = default;

    constexpr matrix_view_engine&   operator =(matrix_view_engine&&) noexcept = default;
    constexpr matrix_view_engine&   operator =(matrix_view_engine const&) = default;

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
        return (*mp_engine)(i, j);
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
        return mp_engine->span();
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


//--------------------------------------------------------------------------------------------------
//  Partial Specialization:     matrix_view_engine<ET, matrix_view::const_identity>
//
//  This partial specialization of matrix_view_engine<ET, MVT> implements a non-owning engine
//  for use by class template basic_matrix<ET, OT>.  It provides a read-only view of a host
//  matrix engine that is the same as that engine.
//--------------------------------------------------------------------------------------------------
//
template<class ET>
requires
    detail::readable_matrix_engine<ET>
class matrix_view_engine<ET, matrix_view::const_identity>
{
    using this_type           = matrix_view_engine;
    using engine_pointer      = ET const*;
    using support_traits      = detail::matrix_engine_support;
    using const_mdspan_traits = detail::mve_mdspan_traits<detail::get_const_mdspan_type_t<ET>>;

    static constexpr bool   has_mdspan = const_mdspan_traits::has_mdspan;

  public:
    using engine_type        = ET;
    using owning_engine_type = detail::get_owning_engine_type_t<ET>;
    using element_type       = typename engine_type::element_type;
    using reference          = typename engine_type::const_reference;
    using const_reference    = typename engine_type::const_reference;
    using index_type         = typename engine_type::index_type;
    using span_type          = typename engine_type::const_span_type;
    using const_span_type    = typename engine_type::const_span_type;

    //- Construct/copy/destroy
    //
    ~matrix_view_engine() noexcept = default;

    constexpr matrix_view_engine() noexcept
    :   mp_engine(nullptr)
    {}
    constexpr matrix_view_engine(matrix_view_engine&&) noexcept = default;
    constexpr matrix_view_engine(matrix_view_engine const&) = default;

    constexpr matrix_view_engine&   operator =(matrix_view_engine&&) noexcept = default;
    constexpr matrix_view_engine&   operator =(matrix_view_engine const&) = default;

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
    constexpr const_reference
    operator ()(index_type i, index_type j) const
    {
        return (*mp_engine)(i, j);
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
        return mp_engine->span();
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


//--------------------------------------------------------------------------------------------------
//  Partial Specialization:     matrix_view_engine<ET, matrix_view::transpose>
//
//  This partial specialization of matrix_view_engine<ET, MVT> implements a non-owning engine
//  for use by class template basic_matrix<ET, OT>.  It provides a read-write view of a host
//  matrix engine that expresses the transpose of that engine.
//--------------------------------------------------------------------------------------------------
//
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

    constexpr matrix_view_engine&   operator =(matrix_view_engine&&) noexcept = default;
    constexpr matrix_view_engine&   operator =(matrix_view_engine const&) = default;

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
        return mdspan_traits::make_transpose(mp_engine->span());
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


//--------------------------------------------------------------------------------------------------
//  Partial Specialization:     matrix_view_engine<ET, matrix_view::const_transpose>
//
//  This partial specialization of matrix_view_engine<ET, MVT> implements a non-owning engine
//  for use by class template basic_matrix<ET, OT>.  It provides a read-only view of a host
//  matrix engine that expresses the transpose of that engine.
//--------------------------------------------------------------------------------------------------
//
template<class ET>
requires
    detail::readable_matrix_engine<ET>
class matrix_view_engine<ET, matrix_view::const_transpose>
{
    using this_type           = matrix_view_engine;
    using engine_pointer      = ET const*;
    using support_traits      = detail::matrix_engine_support;
    using const_mdspan_traits = detail::mve_mdspan_traits<detail::get_const_mdspan_type_t<ET>>;

    static constexpr bool   has_mdspan = const_mdspan_traits::has_mdspan;

  public:
    using engine_type        = ET;
    using owning_engine_type = detail::get_owning_engine_type_t<ET>;
    using element_type       = typename engine_type::element_type;
    using reference          = typename engine_type::const_reference;
    using const_reference    = typename engine_type::const_reference;
    using index_type         = typename engine_type::index_type;
    using span_type          = typename const_mdspan_traits::transpose_mdspan_type;
    using const_span_type    = typename const_mdspan_traits::transpose_mdspan_type;

    //- Construct/copy/destroy
    //
    ~matrix_view_engine() noexcept = default;

    constexpr matrix_view_engine() noexcept
    :   mp_engine(nullptr)
    {}
    constexpr matrix_view_engine(matrix_view_engine&&) noexcept = default;
    constexpr matrix_view_engine(matrix_view_engine const&) = default;

    constexpr matrix_view_engine&   operator =(matrix_view_engine&&) noexcept = default;
    constexpr matrix_view_engine&   operator =(matrix_view_engine const&) = default;

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
        return const_mdspan_traits::make_transpose(mp_engine->span());
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


//--------------------------------------------------------------------------------------------------
//  Partial Specialization:     matrix_view_engine<ET, matrix_view::column>
//
//  This partial specialization of matrix_view_engine<ET, MVT> implements a non-owning engine
//  for use by class template basic_matrix<ET, OT>.  It provides a read-write view of a column
//  of a host matrix engine, and expresses an engine of size R x 1.
//--------------------------------------------------------------------------------------------------
//
template<class ET>
requires
    detail::writable_matrix_engine<ET>
class matrix_view_engine<ET, matrix_view::column>
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
    using span_type          = typename mdspan_traits::submatrix_mdspan_type;
    using const_span_type    = typename const_mdspan_traits::submatrix_mdspan_type;

    //- Construct/copy/destroy
    //
    ~matrix_view_engine() noexcept = default;

    constexpr matrix_view_engine() noexcept
    :   mp_engine(nullptr)
    ,   m_column(0)
    {}
    constexpr matrix_view_engine(matrix_view_engine&&) noexcept = default;
    constexpr matrix_view_engine(matrix_view_engine const&) = default;

    constexpr matrix_view_engine&   operator =(matrix_view_engine&&) noexcept = default;
    constexpr matrix_view_engine&   operator =(matrix_view_engine const&) = default;

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

    constexpr span_type
    span() const noexcept
    requires
        this_type::has_mdspan
    {
        return mdspan_traits::make_submatrix(mp_engine->span(), 0, mp_engine->rows(), m_column, 1);
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


//--------------------------------------------------------------------------------------------------
//  Partial Specialization:     matrix_view_engine<ET, matrix_view::const_column>
//
//  This partial specialization of matrix_view_engine<ET, MVT> implements a non-owning engine
//  for use by class template basic_matrix<ET, OT>.  It provides a read-only view of a column
//  of a host matrix engine, and expresses an engine of size R x 1.
//--------------------------------------------------------------------------------------------------
//
template<class ET>
requires
    detail::readable_matrix_engine<ET>
class matrix_view_engine<ET, matrix_view::const_column>
{
    using this_type           = matrix_view_engine;
    using engine_pointer      = ET const*;
    using support_traits      = detail::matrix_engine_support;
    using const_mdspan_traits = detail::mve_mdspan_traits<detail::get_const_mdspan_type_t<ET>>;

    static constexpr bool   has_mdspan = const_mdspan_traits::has_mdspan;

  public:
    using engine_type        = ET;
    using owning_engine_type = detail::get_owning_engine_type_t<ET>;
    using element_type       = typename engine_type::element_type;
    using reference          = typename engine_type::const_reference;
    using const_reference    = typename engine_type::const_reference;
    using index_type         = typename engine_type::index_type;
    using span_type          = typename const_mdspan_traits::submatrix_mdspan_type;
    using const_span_type    = typename const_mdspan_traits::submatrix_mdspan_type;

    //- Construct/copy/destroy
    //
    ~matrix_view_engine() noexcept = default;

    constexpr matrix_view_engine() noexcept
    :   mp_engine(nullptr)
    ,   m_column(0)
    {}
    constexpr matrix_view_engine(matrix_view_engine&&) noexcept = default;
    constexpr matrix_view_engine(matrix_view_engine const&) = default;

    constexpr matrix_view_engine&   operator =(matrix_view_engine&&) noexcept = default;
    constexpr matrix_view_engine&   operator =(matrix_view_engine const&) = default;

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

    constexpr span_type
    span() const noexcept
    requires
        this_type::has_mdspan
    {
        return const_mdspan_traits::make_submatrix(mp_engine->span(), 0, mp_engine->rows(), m_column, 1);
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


//--------------------------------------------------------------------------------------------------
//  Partial Specialization:     matrix_view_engine<ET, matrix_view::row>
//
//  This partial specialization of matrix_view_engine<ET, MVT> implements a non-owning engine
//  for use by class template basic_matrix<ET, OT>.  It provides a read-write view of a row
//  of a host matrix engine, and expresses an engine of size 1 x C.
//--------------------------------------------------------------------------------------------------
//
template<class ET>
requires
    detail::writable_matrix_engine<ET>
class matrix_view_engine<ET, matrix_view::row>
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
    using span_type          = typename mdspan_traits::submatrix_mdspan_type;
    using const_span_type    = typename const_mdspan_traits::submatrix_mdspan_type;

    //- Construct/copy/destroy
    //
    ~matrix_view_engine() noexcept = default;

    constexpr matrix_view_engine() noexcept
    :   mp_engine(nullptr)
    ,   m_row(0)
    {}
    constexpr matrix_view_engine(matrix_view_engine&&) noexcept = default;
    constexpr matrix_view_engine(matrix_view_engine const&) = default;

    constexpr matrix_view_engine&   operator =(matrix_view_engine&&) noexcept = default;
    constexpr matrix_view_engine&   operator =(matrix_view_engine const&) = default;

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

    constexpr span_type
    span() const noexcept
    requires
        this_type::has_mdspan
    {
        return mdspan_traits::make_submatrix(mp_engine->span(), m_row, 1, 0, mp_engine->columns());
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


//--------------------------------------------------------------------------------------------------
//  Partial Specialization:     matrix_view_engine<ET, matrix_view::const_row>
//
//  This partial specialization of matrix_view_engine<ET, MVT> implements a non-owning engine
//  for use by class template basic_matrix<ET, OT>.  It provides a read-only view of a row
//  of a host matrix engine, and expresses an engine of size 1 x C.
//--------------------------------------------------------------------------------------------------
//
template<class ET>
requires
    detail::readable_matrix_engine<ET>
class matrix_view_engine<ET, matrix_view::const_row>
{
    using this_type           = matrix_view_engine;
    using engine_pointer      = ET const*;
    using support_traits      = detail::matrix_engine_support;
    using const_mdspan_traits = detail::mve_mdspan_traits<detail::get_const_mdspan_type_t<ET>>;

    static constexpr bool   has_mdspan = const_mdspan_traits::has_mdspan;

  public:
    using engine_type        = ET;
    using owning_engine_type = detail::get_owning_engine_type_t<ET>;
    using element_type       = typename engine_type::element_type;
    using reference          = typename engine_type::const_reference;
    using const_reference    = typename engine_type::const_reference;
    using index_type         = typename engine_type::index_type;
    using span_type          = typename const_mdspan_traits::submatrix_mdspan_type;
    using const_span_type    = typename const_mdspan_traits::submatrix_mdspan_type;

    //- Construct/copy/destroy
    //
    ~matrix_view_engine() noexcept = default;

    constexpr matrix_view_engine() noexcept
    :   mp_engine(nullptr)
    ,   m_row(0)
    {}
    constexpr matrix_view_engine(matrix_view_engine&&) noexcept = default;
    constexpr matrix_view_engine(matrix_view_engine const&) = default;

    constexpr matrix_view_engine&   operator =(matrix_view_engine&&) noexcept = default;
    constexpr matrix_view_engine&   operator =(matrix_view_engine const&) = default;

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

    constexpr span_type
    span() const noexcept
    requires
        this_type::has_mdspan
    {
        return const_mdspan_traits::make_submatrix(mp_engine->span(), m_row, 1, 0, mp_engine->columns());
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


//--------------------------------------------------------------------------------------------------
//  Partial Specialization:     matrix_view_engine<ET, matrix_view::submatrix>
//
//  This partial specialization of matrix_view_engine<ET, MVT> implements a non-owning engine
//  for use by class template basic_matrix<ET, OT>.  It provides a read-write view of a host
//  matrix engine that expresses a sub-region of that engine.
//--------------------------------------------------------------------------------------------------
//
template<class ET>
requires
    detail::writable_matrix_engine<ET>
class matrix_view_engine<ET, matrix_view::submatrix>
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
    using span_type          = typename mdspan_traits::submatrix_mdspan_type;
    using const_span_type    = typename const_mdspan_traits::submatrix_mdspan_type;

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

    constexpr matrix_view_engine&   operator =(matrix_view_engine&&) noexcept = default;
    constexpr matrix_view_engine&   operator =(matrix_view_engine const&) = default;

    explicit constexpr
    matrix_view_engine(engine_type& eng,
                       index_type ri, index_type rn,
                       index_type ci, index_type cn) noexcept
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

    constexpr span_type
    span() const noexcept
    requires
        this_type::has_mdspan
    {
        return mdspan_traits::make_submatrix(mp_engine->span(),
                                             m_row_start, m_row_count,
                                             m_col_start, m_col_count);
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


//--------------------------------------------------------------------------------------------------
//  Partial Specialization:     matrix_view_engine<ET, matrix_view::const_submatrix>
//
//  This partial specialization of matrix_view_engine<ET, MVT> implements a non-owning engine
//  for use by class template basic_matrix<ET, OT>.  It provides a read-write view of a host
//  matrix engine that expresses a sub-region of that engine.
//--------------------------------------------------------------------------------------------------
//
template<class ET>
requires
    detail::readable_matrix_engine<ET>
class matrix_view_engine<ET, matrix_view::const_submatrix>
{
    using this_type           = matrix_view_engine;
    using engine_pointer      = ET const*;
    using support_traits      = detail::matrix_engine_support;
    using const_mdspan_traits = detail::mve_mdspan_traits<detail::get_const_mdspan_type_t<ET>>;

    static constexpr bool   has_mdspan = const_mdspan_traits::has_mdspan;

  public:
    using engine_type        = ET;
    using owning_engine_type = detail::get_owning_engine_type_t<ET>;
    using element_type       = typename engine_type::element_type;
    using reference          = typename engine_type::const_reference;
    using const_reference    = typename engine_type::const_reference;
    using index_type         = typename engine_type::index_type;
    using span_type          = typename const_mdspan_traits::submatrix_mdspan_type;
    using const_span_type    = typename const_mdspan_traits::submatrix_mdspan_type;

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

    constexpr matrix_view_engine&   operator =(matrix_view_engine&&) noexcept = default;
    constexpr matrix_view_engine&   operator =(matrix_view_engine const&) = default;

    explicit constexpr
    matrix_view_engine(engine_type const& eng,
                       index_type ri, index_type rn,
                       index_type ci, index_type cn) noexcept
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

    constexpr span_type
    span() const noexcept
    requires
        this_type::has_mdspan
    {
        return const_mdspan_traits::make_submatrix(mp_engine->span(),
                                                   m_row_start, m_row_count,
                                                   m_col_start, m_col_count);
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
