//==================================================================================================
//  File:       matrix_view_engine.hpp
//
//  Summary:    This header defines a non-owning engine that acts as a "view" of some subset of a
//              matrix.  The extent and kind of view are specified by a template parameter.
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_MATRIX_VIEW_ENGINE_HPP_DEFINED
#define LINEAR_ALGEBRA_MATRIX_VIEW_ENGINE_HPP_DEFINED

namespace STD_LA {
//==================================================================================================
//  MATRIX VIEW ENGINES
//==================================================================================================
//--------------------------------------------------------------------------------------------------
//  Class Template:     matrix_view_engine<ET, MVT>
//
//  This class template implements represents a non-owning engine type, which references another
//  engine object, and which presents a specific "view" of that object.
//--------------------------------------------------------------------------------------------------
//
template<class ET, class MVT>    class matrix_view_engine;


//--------------------------------------------------------------------------------------------------
//  Partial Specialization:     matrix_view_engine<ET, matrix_view::const_negation>
//
//  This partial specialization of matrix_view_engine<ET, MVT> implements a non-owning engine
//  for use by class template matrix<ET, OT>.  It provides a read-only view of a host matrix
//  engine that expresses the element-wise negation of that engine.
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
    using const_mdspan_traits = detail::mdspan_view_traits<detail::get_const_mdspan_type_t<ET>>;

    static constexpr bool   has_mdspan = const_mdspan_traits::has_mdspan;

  public:
    using engine_type        = ET;
    using owning_engine_type = detail::get_owning_engine_type_t<ET>;
    using element_type       = typename engine_type::element_type;
    using layout_type        = detail::get_layout_t<ET>;
    using reference          = typename engine_type::element_type;
    using const_reference    = typename engine_type::element_type;
    using size_type          = typename engine_type::size_type;
    using mdspan_type        = typename const_mdspan_traits::negation_mdspan_type;
    using const_mdspan_type  = typename const_mdspan_traits::negation_mdspan_type;

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
    constexpr size_type
    columns() const noexcept
    {
        if constexpr(detail::has_constexpr_columns_v<engine_type>)
        {
            return detail::engine_extents_helper<engine_type>::columns();
        }
        else
        {
            return mp_engine->columns();
        }
    }

    constexpr size_type
    rows() const noexcept
    {
        if constexpr(detail::has_constexpr_rows_v<engine_type>)
        {
            return detail::engine_extents_helper<engine_type>::rows();
        }
        else
        {
            return mp_engine->rows();
        }
    }

    constexpr size_type
    size() const noexcept
    {
        if constexpr(detail::has_constexpr_size_v<engine_type>)
        {
            return detail::engine_extents_helper<engine_type>::size();
        }
        else
        {
            return mp_engine->size();
        }
    }

    constexpr size_type
    column_capacity() const noexcept
    {
        return columns();
    }

    constexpr size_type
    row_capacity() const noexcept
    {
        return rows();
    }

    constexpr size_type
    capacity() const noexcept
    {
        return size();
    }

    //- Element access
    //
    constexpr const_reference
    operator ()(size_type i, size_type j) const
    {
        return -(*mp_engine)(i, j);
    }

    constexpr mdspan_type
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
//  for use by class template matrix<ET, OT>.  It provides a read-only view of a host matrix
//  engine that expresses the element-wise complex conjugate of that engine.
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
    using const_mdspan_traits = detail::mdspan_view_traits<detail::get_const_mdspan_type_t<ET>>;

    static constexpr bool   has_mdspan = const_mdspan_traits::has_mdspan;

  public:
    using engine_type        = ET;
    using owning_engine_type = detail::get_owning_engine_type_t<ET>;
    using element_type       = typename engine_type::element_type;
    using layout_type        = detail::get_layout_t<ET>;
    using reference          = conj_elem_type;
    using const_reference    = conj_elem_type;
    using size_type          = typename engine_type::size_type;
    using mdspan_type        = typename const_mdspan_traits::conjugate_mdspan_type;
    using const_mdspan_type  = typename const_mdspan_traits::conjugate_mdspan_type;

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
    constexpr size_type
    columns() const noexcept
    {
        if constexpr(detail::has_constexpr_columns_v<engine_type>)
        {
            return detail::engine_extents_helper<engine_type>::columns();
        }
        else
        {
            return mp_engine->columns();
        }
    }

    constexpr size_type
    rows() const noexcept
    {
        if constexpr(detail::has_constexpr_rows_v<engine_type>)
        {
            return detail::engine_extents_helper<engine_type>::rows();
        }
        else
        {
            return mp_engine->rows();
        }
    }

    constexpr size_type
    size() const noexcept
    {
        if constexpr(detail::has_constexpr_size_v<engine_type>)
        {
            return detail::engine_extents_helper<engine_type>::size();
        }
        else
        {
            return mp_engine->size();
        }
    }

    constexpr size_type
    column_capacity() const noexcept
    {
        return columns();
    }

    constexpr size_type
    row_capacity() const noexcept
    {
        return rows();
    }

    constexpr size_type
    capacity() const noexcept
    {
        return size();
    }

    //- Element access
    //
    constexpr const_reference
    operator ()(size_type i, size_type j) const
    {
        if constexpr (is_cx)
        {
            return std::conj((*mp_engine)(i, j));
        }
        else
        {
            return (*mp_engine)(i, j);
        }
    }

    constexpr mdspan_type
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
//  for use by class template matrix<ET, OT>.  It provides a read-only view of a host matrix
//  engine that expresses the complex conjugate transpose of that engine.
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
    using const_mdspan_traits = detail::mdspan_view_traits<detail::get_const_mdspan_type_t<ET>>;

    static constexpr bool   has_mdspan = const_mdspan_traits::has_mdspan;

  public:
    using engine_type        = ET;
    using owning_engine_type = detail::get_owning_engine_type_t<ET>;
    using element_type       = typename engine_type::element_type;
    using layout_type        = detail::get_transpose_layout_t<detail::get_layout_t<ET>>;
    using reference          = conj_elem_type;
    using const_reference    = conj_elem_type;
    using size_type          = typename engine_type::size_type;
    using mdspan_type        = typename const_mdspan_traits::hermitian_mdspan_type;
    using const_mdspan_type  = typename const_mdspan_traits::hermitian_mdspan_type;

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
    constexpr size_type
    columns() const noexcept
    {
        if constexpr(detail::has_constexpr_rows_v<engine_type>)
        {
            return detail::engine_extents_helper<engine_type>::rows();
        }
        else
        {
            return mp_engine->rows();
        }
    }

    constexpr size_type
    rows() const noexcept
    {
        if constexpr(detail::has_constexpr_columns_v<engine_type>)
        {
            return detail::engine_extents_helper<engine_type>::columns();
        }
        else
        {
            return mp_engine->columns();
        }
    }

    constexpr size_type
    size() const noexcept
    {
        if constexpr(detail::has_constexpr_size_v<engine_type>)
        {
            return detail::engine_extents_helper<engine_type>::size();
        }
        else
        {
            return mp_engine->size();
        }
    }

    constexpr size_type
    column_capacity() const noexcept
    {
        return rows();
    }

    constexpr size_type
    row_capacity() const noexcept
    {
        return columns();
    }

    constexpr size_type
    capacity() const noexcept
    {
        return size();
    }

    //- Element access
    //
    constexpr const_reference
    operator ()(size_type i, size_type j) const
    {
        if constexpr (is_cx)
        {
            return std::conj((*mp_engine)(j, i));
        }
        else
        {
            return (*mp_engine)(j, i);
        }
    }

    constexpr mdspan_type
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
//  for use by class template matrix<ET, OT>.  It provides a read-write view of a host matrix
//  engine that is a trivial wrapper to that engine.
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
    using const_mdspan_traits = detail::mdspan_view_traits<detail::get_mdspan_type_t<ET>>;

    static constexpr bool   has_mdspan = const_mdspan_traits::has_mdspan;

  public:
    using engine_type        = ET;
    using owning_engine_type = detail::get_owning_engine_type_t<ET>;
    using element_type       = typename engine_type::element_type;
    using layout_type        = detail::get_layout_t<ET>;
    using reference          = typename engine_type::reference;
    using const_reference    = typename engine_type::const_reference;
    using size_type          = typename engine_type::size_type;
    using mdspan_type        = detail::get_mdspan_type_t<ET>;
    using const_mdspan_type  = detail::get_const_mdspan_type_t<ET>;

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
    constexpr size_type
    columns() const noexcept
    {
        if constexpr(detail::has_constexpr_columns_v<engine_type>)
        {
            return detail::engine_extents_helper<engine_type>::columns();
        }
        else
        {
            return mp_engine->columns();
        }
    }

    constexpr size_type
    rows() const noexcept
    {
        if constexpr(detail::has_constexpr_rows_v<engine_type>)
        {
            return detail::engine_extents_helper<engine_type>::rows();
        }
        else
        {
            return mp_engine->rows();
        }
    }

    constexpr size_type
    size() const noexcept
    {
        if constexpr(detail::has_constexpr_size_v<engine_type>)
        {
            return detail::engine_extents_helper<engine_type>::size();
        }
        else
        {
            return mp_engine->size();
        }
    }

    constexpr size_type
    column_capacity() const noexcept
    {
        return columns();
    }

    constexpr size_type
    row_capacity() const noexcept
    {
        return rows();
    }

    constexpr size_type
    capacity() const noexcept
    {
        return size();
    }

    //- Element access
    //
    constexpr reference
    operator ()(size_type i, size_type j) const
    {
        return (*mp_engine)(i, j);
    }

    constexpr reference
    operator ()(size_type i) const
    requires
        detail::writable_and_1d_indexable_matrix_engine<engine_type>
    {
        return (*mp_engine)(i);
    }

    constexpr mdspan_type
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
//  for use by class template matrix<ET, OT>.  It provides a read-only view of a host matrix
//  engine that is a trivial wrapper to that engine.
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
    using const_mdspan_traits = detail::mdspan_view_traits<detail::get_const_mdspan_type_t<ET>>;

    static constexpr bool   has_mdspan = const_mdspan_traits::has_mdspan;

  public:
    using engine_type        = ET;
    using owning_engine_type = detail::get_owning_engine_type_t<ET>;
    using element_type       = typename engine_type::element_type;
    using layout_type        = detail::get_layout_t<ET>;
    using reference          = typename engine_type::const_reference;
    using const_reference    = typename engine_type::const_reference;
    using size_type          = typename engine_type::size_type;
    using mdspan_type        = detail::get_const_mdspan_type_t<ET>;
    using const_mdspan_type  = detail::get_const_mdspan_type_t<ET>;

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
    constexpr size_type
    columns() const noexcept
    {
        if constexpr(detail::has_constexpr_columns_v<engine_type>)
        {
            return detail::engine_extents_helper<engine_type>::columns();
        }
        else
        {
            return mp_engine->columns();
        }
    }

    constexpr size_type
    rows() const noexcept
    {
        if constexpr(detail::has_constexpr_rows_v<engine_type>)
        {
            return detail::engine_extents_helper<engine_type>::rows();
        }
        else
        {
            return mp_engine->rows();
        }
    }

    constexpr size_type
    size() const noexcept
    {
        if constexpr(detail::has_constexpr_size_v<engine_type>)
        {
            return detail::engine_extents_helper<engine_type>::size();
        }
        else
        {
            return mp_engine->size();
        }
    }

    constexpr size_type
    column_capacity() const noexcept
    {
        return columns();
    }

    constexpr size_type
    row_capacity() const noexcept
    {
        return rows();
    }

    constexpr size_type
    capacity() const noexcept
    {
        return size();
    }

    //- Element access
    //
    constexpr const_reference
    operator ()(size_type i, size_type j) const
    {
        return (*mp_engine)(i, j);
    }

    constexpr const_reference
    operator ()(size_type i) const
    requires
        detail::readable_and_1d_indexable_matrix_engine<engine_type>
    {
        return (*mp_engine)(i);
    }

    constexpr mdspan_type
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
//  for use by class template matrix<ET, OT>.  It provides a read-write view of a host matrix
//  engine that expresses the transpose of that engine.
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
    using mdspan_traits       = detail::mdspan_view_traits<detail::get_mdspan_type_t<ET>>;
    using const_mdspan_traits = detail::mdspan_view_traits<detail::get_const_mdspan_type_t<ET>>;

    static constexpr bool   has_mdspan = mdspan_traits::has_mdspan;

  public:
    using engine_type        = ET;
    using owning_engine_type = detail::get_owning_engine_type_t<ET>;
    using element_type       = typename engine_type::element_type;
    using layout_type        = detail::get_transpose_layout_t<detail::get_layout_t<ET>>;
    using reference          = typename engine_type::reference;
    using const_reference    = typename engine_type::const_reference;
    using size_type          = typename engine_type::size_type;
    using mdspan_type        = typename mdspan_traits::transpose_mdspan_type;
    using const_mdspan_type  = typename const_mdspan_traits::transpose_mdspan_type;

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
    constexpr size_type
    columns() const noexcept
    {
        if constexpr(detail::has_constexpr_rows_v<engine_type>)
        {
            return detail::engine_extents_helper<engine_type>::rows();
        }
        else
        {
            return mp_engine->rows();
        }
    }

    constexpr size_type
    rows() const noexcept
    {
        if constexpr(detail::has_constexpr_columns_v<engine_type>)
        {
            return detail::engine_extents_helper<engine_type>::columns();
        }
        else
        {
            return mp_engine->columns();
        }
    }

    constexpr size_type
    size() const noexcept
    {
        if constexpr(detail::has_constexpr_size_v<engine_type>)
        {
            return detail::engine_extents_helper<engine_type>::size();
        }
        else
        {
            return mp_engine->size();
        }
    }

    constexpr size_type
    column_capacity() const noexcept
    {
        return rows();
    }

    constexpr size_type
    row_capacity() const noexcept
    {
        return columns();
    }

    constexpr size_type
    capacity() const noexcept
    {
        return size();
    }

    //- Element access
    //
    constexpr reference
    operator ()(size_type i, size_type j) const
    {
        return (*mp_engine)(j, i);
    }

    constexpr reference
    operator ()(size_type i) const
    requires
        detail::writable_and_1d_indexable_matrix_engine<engine_type>
    {
        return (*mp_engine)(i);
    }

    constexpr mdspan_type
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
//  for use by class template matrix<ET, OT>.  It provides a read-only view of a host matrix
//  engine that expresses the transpose of that engine.
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
    using const_mdspan_traits = detail::mdspan_view_traits<detail::get_const_mdspan_type_t<ET>>;

    static constexpr bool   has_mdspan = const_mdspan_traits::has_mdspan;

  public:
    using engine_type        = ET;
    using owning_engine_type = detail::get_owning_engine_type_t<ET>;
    using element_type       = typename engine_type::element_type;
    using layout_type        = detail::get_transpose_layout_t<detail::get_layout_t<ET>>;
    using reference          = typename engine_type::const_reference;
    using const_reference    = typename engine_type::const_reference;
    using size_type          = typename engine_type::size_type;
    using mdspan_type        = typename const_mdspan_traits::transpose_mdspan_type;
    using const_mdspan_type  = typename const_mdspan_traits::transpose_mdspan_type;

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
    constexpr size_type
    columns() const noexcept
    {
        if constexpr(detail::has_constexpr_rows_v<engine_type>)
        {
            return detail::engine_extents_helper<engine_type>::rows();
        }
        else
        {
            return mp_engine->rows();
        }
    }

    constexpr size_type
    rows() const noexcept
    {
        if constexpr(detail::has_constexpr_columns_v<engine_type>)
        {
            return detail::engine_extents_helper<engine_type>::columns();
        }
        else
        {
            return mp_engine->columns();
        }
    }

    constexpr size_type
    size() const noexcept
    {
        if constexpr(detail::has_constexpr_size_v<engine_type>)
        {
            return detail::engine_extents_helper<engine_type>::size();
        }
        else
        {
            return mp_engine->size();
        }
    }

    constexpr size_type
    column_capacity() const noexcept
    {
        return rows();
    }

    constexpr size_type
    row_capacity() const noexcept
    {
        return columns();
    }

    constexpr size_type
    capacity() const noexcept
    {
        return size();
    }

    //- Element access
    //
    constexpr const_reference
    operator ()(size_type i, size_type j) const
    {
        return (*mp_engine)(j, i);
    }

    constexpr const_reference
    operator ()(size_type i) const
    requires
        detail::readable_and_1d_indexable_matrix_engine<engine_type>
    {
        return (*mp_engine)(i);
    }

    constexpr mdspan_type
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
//  for use by class template matrix<ET, OT>.  It provides a read-write view of a column of a
//  host matrix engine, and expresses an engine of size R x 1.
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
    using mdspan_traits       = detail::mdspan_view_traits<detail::get_mdspan_type_t<ET>>;
    using const_mdspan_traits = detail::mdspan_view_traits<detail::get_const_mdspan_type_t<ET>>;

    static constexpr bool   has_mdspan = mdspan_traits::has_mdspan;

  public:
    using engine_type        = ET;
    using owning_engine_type = detail::get_owning_engine_type_t<ET>;
    using element_type       = typename engine_type::element_type;
    using layout_type        = detail::get_layout_t<ET>;
    using reference          = typename engine_type::reference;
    using const_reference    = typename engine_type::const_reference;
    using size_type          = typename engine_type::size_type;
    using mdspan_type        = typename mdspan_traits::submatrix_mdspan_type;
    using const_mdspan_type  = typename const_mdspan_traits::submatrix_mdspan_type;

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
    matrix_view_engine(engine_type& eng, size_type ci) noexcept
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
    constexpr size_type
    columns() const noexcept
    {
        return 1;
    }

    constexpr size_type
    rows() const noexcept
    {
        if constexpr(detail::has_constexpr_rows_v<engine_type>)
        {
            return detail::engine_extents_helper<engine_type>::rows();
        }
        else
        {
            return mp_engine->rows();
        }
    }

    constexpr size_type
    size() const noexcept
    {
        return rows();
    }

    constexpr size_type
    column_capacity() const noexcept
    {
        return 1;
    }

    constexpr size_type
    row_capacity() const noexcept
    {
        return rows();
    }

    constexpr size_type
    capacity() const noexcept
    {
        return rows();
    }

    //- Element access
    //
    constexpr reference
    operator ()(size_type i, size_type) const
    {
        return (*mp_engine)(i, m_column);
    }

    constexpr reference
    operator ()(size_type i) const
    {
        return (*mp_engine)(i, m_column);
    }

    constexpr mdspan_type
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
    size_type       m_column;
};


//--------------------------------------------------------------------------------------------------
//  Partial Specialization:     matrix_view_engine<ET, matrix_view::const_column>
//
//  This partial specialization of matrix_view_engine<ET, MVT> implements a non-owning engine
//  for use by class template matrix<ET, OT>.  It provides a read-only view of a column of a
//  host matrix engine, and expresses an engine of size R x 1.
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
    using const_mdspan_traits = detail::mdspan_view_traits<detail::get_const_mdspan_type_t<ET>>;

    static constexpr bool   has_mdspan = const_mdspan_traits::has_mdspan;

  public:
    using engine_type        = ET;
    using owning_engine_type = detail::get_owning_engine_type_t<ET>;
    using element_type       = typename engine_type::element_type;
    using layout_type        = detail::get_layout_t<ET>;
    using reference          = typename engine_type::const_reference;
    using const_reference    = typename engine_type::const_reference;
    using size_type          = typename engine_type::size_type;
    using mdspan_type        = typename const_mdspan_traits::submatrix_mdspan_type;
    using const_mdspan_type  = typename const_mdspan_traits::submatrix_mdspan_type;

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
    matrix_view_engine(engine_type const& eng, size_type ci) noexcept
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
    constexpr size_type
    columns() const noexcept
    {
        return 1;
    }

    constexpr size_type
    rows() const noexcept
    {
        if constexpr(detail::has_constexpr_rows_v<engine_type>)
        {
            return detail::engine_extents_helper<engine_type>::rows();
        }
        else
        {
            return mp_engine->rows();
        }
    }

    constexpr size_type
    size() const noexcept
    {
        return rows();
    }

    constexpr size_type
    column_capacity() const noexcept
    {
        return 1;
    }

    constexpr size_type
    row_capacity() const noexcept
    {
        return rows();
    }

    constexpr size_type
    capacity() const noexcept
    {
        return rows();
    }

    //- Element access
    //
    constexpr const_reference
    operator ()(size_type i, size_type) const
    {
        return (*mp_engine)(i, m_column);
    }

    constexpr const_reference
    operator ()(size_type i) const
    {
        return (*mp_engine)(i, m_column);
    }

    constexpr mdspan_type
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
    size_type       m_column;
};


//--------------------------------------------------------------------------------------------------
//  Partial Specialization:     matrix_view_engine<ET, matrix_view::row>
//
//  This partial specialization of matrix_view_engine<ET, MVT> implements a non-owning engine
//  for use by class template matrix<ET, OT>.  It provides a read-write view of a row of a
//  host matrix engine, and expresses an engine of size 1 x C.
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
    using mdspan_traits       = detail::mdspan_view_traits<detail::get_mdspan_type_t<ET>>;
    using const_mdspan_traits = detail::mdspan_view_traits<detail::get_const_mdspan_type_t<ET>>;

    static constexpr bool   has_mdspan = mdspan_traits::has_mdspan;

  public:
    using engine_type        = ET;
    using owning_engine_type = detail::get_owning_engine_type_t<ET>;
    using element_type       = typename engine_type::element_type;
    using layout_type        = detail::get_layout_t<ET>;
    using reference          = typename engine_type::reference;
    using const_reference    = typename engine_type::const_reference;
    using size_type          = typename engine_type::size_type;
    using mdspan_type        = typename mdspan_traits::submatrix_mdspan_type;
    using const_mdspan_type  = typename const_mdspan_traits::submatrix_mdspan_type;

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
    matrix_view_engine(engine_type& eng, size_type ri) noexcept
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
    constexpr size_type
    columns() const noexcept
    {
        if constexpr(detail::has_constexpr_columns_v<engine_type>)
        {
            return detail::engine_extents_helper<engine_type>::columns();
        }
        else
        {
            return mp_engine->columns();
        }
    }

    constexpr size_type
    rows() const noexcept
    {
        return 1;
    }

    constexpr size_type
    size() const noexcept
    {
        return mp_engine->columns();
    }

    constexpr size_type
    column_capacity() const noexcept
    {
        return columns();
    }

    constexpr size_type
    row_capacity() const noexcept
    {
        return 1;
    }

    constexpr size_type
    capacity() const noexcept
    {
        return columns();
    }

    //- Element access
    //
    constexpr reference
    operator ()(size_type, size_type j) const
    {
        return (*mp_engine)(m_row, j);
    }

    constexpr reference
    operator ()(size_type j) const
    {
        return (*mp_engine)(m_row, j);
    }

    constexpr mdspan_type
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
    size_type       m_row;
};


//--------------------------------------------------------------------------------------------------
//  Partial Specialization:     matrix_view_engine<ET, matrix_view::const_row>
//
//  This partial specialization of matrix_view_engine<ET, MVT> implements a non-owning engine
//  for use by class template matrix<ET, OT>.  It provides a read-only view of a row of a
//  host matrix engine, and expresses an engine of size 1 x C.
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
    using const_mdspan_traits = detail::mdspan_view_traits<detail::get_const_mdspan_type_t<ET>>;

    static constexpr bool   has_mdspan = const_mdspan_traits::has_mdspan;

  public:
    using engine_type        = ET;
    using owning_engine_type = detail::get_owning_engine_type_t<ET>;
    using element_type       = typename engine_type::element_type;
    using layout_type        = detail::get_layout_t<ET>;
    using reference          = typename engine_type::const_reference;
    using const_reference    = typename engine_type::const_reference;
    using size_type          = typename engine_type::size_type;
    using mdspan_type        = typename const_mdspan_traits::submatrix_mdspan_type;
    using const_mdspan_type  = typename const_mdspan_traits::submatrix_mdspan_type;

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
    matrix_view_engine(engine_type const& eng, size_type ri) noexcept
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
    constexpr size_type
    columns() const noexcept
    {
        if constexpr(detail::has_constexpr_columns_v<engine_type>)
        {
            return detail::engine_extents_helper<engine_type>::columns();
        }
        else
        {
            return mp_engine->columns();
        }
    }

    constexpr size_type
    rows() const noexcept
    {
        return 1;
    }

    constexpr size_type
    size() const noexcept
    {
        return columns();
    }

    constexpr size_type
    column_capacity() const noexcept
    {
        return columns();
    }

    constexpr size_type
    row_capacity() const noexcept
    {
        return 1;
    }

    constexpr size_type
    capacity() const noexcept
    {
        return columns();
    }

    //- Element access
    //
    constexpr const_reference
    operator ()(size_type, size_type j) const
    {
        return (*mp_engine)(m_row, j);
    }

    constexpr const_reference
    operator ()(size_type j) const
    {
        return (*mp_engine)(m_row, j);
    }

    constexpr mdspan_type
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
    size_type       m_row;
};


//--------------------------------------------------------------------------------------------------
//  Partial Specialization:     matrix_view_engine<ET, matrix_view::submatrix>
//
//  This partial specialization of matrix_view_engine<ET, MVT> implements a non-owning engine
//  for use by class template matrix<ET, OT>.  It provides a read-write view of a host matrix
//  engine that expresses a sub-region of that engine.
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
    using mdspan_traits       = detail::mdspan_view_traits<detail::get_mdspan_type_t<ET>>;
    using const_mdspan_traits = detail::mdspan_view_traits<detail::get_const_mdspan_type_t<ET>>;

    static constexpr bool   has_mdspan = mdspan_traits::has_mdspan;

  public:
    using engine_type        = ET;
    using owning_engine_type = detail::get_owning_engine_type_t<ET>;
    using element_type       = typename engine_type::element_type;
    using reference          = typename engine_type::reference;
    using const_reference    = typename engine_type::const_reference;
    using size_type          = typename engine_type::size_type;
    using mdspan_type        = typename mdspan_traits::submatrix_mdspan_type;
    using const_mdspan_type  = typename const_mdspan_traits::submatrix_mdspan_type;

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
                       size_type ri, size_type rn,
                       size_type ci, size_type cn) noexcept
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
    constexpr size_type
    columns() const noexcept
    {
        return m_col_count;
    }

    constexpr size_type
    rows() const noexcept
    {
        return m_row_count;
    }

    constexpr size_type
    size() const noexcept
    {
        return m_row_count * m_col_count;
    }

    constexpr size_type
    column_capacity() const noexcept
    {
        return m_col_count;
    }

    constexpr size_type
    row_capacity() const noexcept
    {
        return m_row_count;
    }

    constexpr size_type
    capacity() const noexcept
    {
        return m_row_count * m_col_count;
    }

    //- Element access
    //
    constexpr reference
    operator ()(size_type i, size_type j) const
    {
        return (*mp_engine)(i + m_row_start, j + m_col_start);
    }

    constexpr reference
    operator ()(size_type i) const
    requires
        detail::writable_and_1d_indexable_matrix_engine<engine_type>
    {
        return (*mp_engine)(i);
    }

    constexpr mdspan_type
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
    size_type       m_row_start;
    size_type       m_row_count;
    size_type       m_col_start;
    size_type       m_col_count;
};


//--------------------------------------------------------------------------------------------------
//  Partial Specialization:     matrix_view_engine<ET, matrix_view::const_submatrix>
//
//  This partial specialization of matrix_view_engine<ET, MVT> implements a non-owning engine
//  for use by class template matrix<ET, OT>.  It provides a read-write view of a host matrix
//  engine that expresses a sub-region of that engine.
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
    using const_mdspan_traits = detail::mdspan_view_traits<detail::get_const_mdspan_type_t<ET>>;

    static constexpr bool   has_mdspan = const_mdspan_traits::has_mdspan;

  public:
    using engine_type        = ET;
    using owning_engine_type = detail::get_owning_engine_type_t<ET>;
    using element_type       = typename engine_type::element_type;
    using reference          = typename engine_type::const_reference;
    using const_reference    = typename engine_type::const_reference;
    using size_type          = typename engine_type::size_type;
    using mdspan_type        = typename const_mdspan_traits::submatrix_mdspan_type;
    using const_mdspan_type  = typename const_mdspan_traits::submatrix_mdspan_type;

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
                       size_type ri, size_type rn,
                       size_type ci, size_type cn) noexcept
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
    constexpr size_type
    columns() const noexcept
    {
        return m_col_count;
    }

    constexpr size_type
    rows() const noexcept
    {
        return m_row_count;
    }

    constexpr size_type
    size() const noexcept
    {
        return m_row_count * m_col_count;
    }

    constexpr size_type
    column_capacity() const noexcept
    {
        return m_col_count;
    }

    constexpr size_type
    row_capacity() const noexcept
    {
        return m_row_count;
    }

    constexpr size_type
    capacity() const noexcept
    {
        return m_row_count * m_col_count;
    }

    //- Element access
    //
    constexpr const_reference
    operator ()(size_type i, size_type j) const
    {
        return (*mp_engine)(i + m_row_start, j + m_col_start);
    }

    constexpr const_reference
    operator ()(size_type i) const
    requires
        detail::readable_and_1d_indexable_matrix_engine<engine_type>
    {
        return (*mp_engine)(i);
    }

    constexpr mdspan_type
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
    size_type       m_row_start;
    size_type       m_row_count;
    size_type       m_col_start;
    size_type       m_col_count;
};

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_MATRIX_VIEW_ENGINE_HPP_DEFINED
