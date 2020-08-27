//==================================================================================================
//  File:       matrix_view_engine.hpp
//
//  Summary:    This header defines an engine that acts as a "view" of a subset of a matrix.
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_MATRIX_VIEW_ENGINE_HPP_DEFINED
#define LINEAR_ALGEBRA_MATRIX_VIEW_ENGINE_HPP_DEFINED

namespace STD_LA {

struct matrix_view
{
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


template<class ET, class VF>    class matrix_view_engine;


template<class ET>
requires
    detail::readable_matrix_engine<ET>
class matrix_view_engine<ET, matrix_view::const_negation>
{
    using support_type   = detail::matrix_engine_support;
    using engine_pointer = ET const*;

  public:
    using engine_type        = ET;
    using owning_engine_type = detail::get_owning_engine_type_t<ET>;
    using element_type       = typename engine_type::element_type;
    using reference          = typename engine_type::element_type;
    using const_reference    = typename engine_type::element_type;
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

    //- Modifiers
    //
    constexpr void
    swap(matrix_view_engine& rhs) noexcept
    {
        support_type::swap(mp_engine, rhs.mp_engine);
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
    using support_type   = detail::matrix_engine_support;
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
        support_type::swap(mp_engine, rhs.mp_engine);
    }

  private:
    engine_pointer  mp_engine;
};


template<class ET>
requires
    detail::writable_matrix_engine<ET>
class matrix_view_engine<ET, matrix_view::transpose>
{
    using support_type   = detail::matrix_engine_support;
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

    //- Modifiers
    //
    constexpr void
    swap(matrix_view_engine& rhs) noexcept
    {
        support_type::swap(mp_engine, rhs.mp_engine);
    }

  private:
    engine_pointer  mp_engine;
};


template<class ET>
requires
    detail::readable_matrix_engine<ET>
class matrix_view_engine<ET, matrix_view::const_transpose>
{
    using support_type   = detail::matrix_engine_support;
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

    //- Modifiers
    //
    constexpr void
    swap(matrix_view_engine& rhs) noexcept
    {
        support_type::swap(mp_engine, rhs.mp_engine);
    }

  private:
    engine_pointer  mp_engine;
};


template<class ET>
requires
    detail::writable_matrix_engine<ET>
class matrix_view_engine<ET, matrix_view::column>
{
    using support_type   = detail::matrix_engine_support;
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
        support_type::swap(*this, rhs);
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
    using support_type   = detail::matrix_engine_support;
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
        support_type::swap(*this, rhs);
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
    using support_type   = detail::matrix_engine_support;
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
        support_type::swap(*this, rhs);
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
    using support_type   = detail::matrix_engine_support;
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
        support_type::swap(*this, rhs);
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
    using support_type   = detail::matrix_engine_support;
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
        support_type::swap(*this, rhs);
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
    using support_type   = detail::matrix_engine_support;
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
        support_type::swap(*this, rhs);
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
