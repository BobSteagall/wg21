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
    using engine_type     = ET;
    using element_type    = typename engine_type::element_type;
    using value_type      = typename engine_type::value_type;
    using reference       = typename engine_type::element_type;
    using const_reference = typename engine_type::element_type;
    using index_type      = typename engine_type::index_type;

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
    using engine_type     = ET;
    using element_type    = typename engine_type::element_type;
    using value_type      = typename engine_type::value_type;
    using reference       = conj_el_type;
    using const_reference = conj_el_type;
    using index_type      = typename engine_type::index_type;

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
    using engine_type     = ET;
    using element_type    = typename engine_type::element_type;
    using value_type      = typename engine_type::value_type;
    using reference       = typename engine_type::reference;
    using const_reference = typename engine_type::const_reference;
    using index_type      = typename engine_type::index_type;

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
    using engine_type     = ET;
    using element_type    = typename engine_type::element_type;
    using value_type      = typename engine_type::value_type;
    using reference       = typename engine_type::const_reference;
    using const_reference = typename engine_type::const_reference;
    using index_type      = typename engine_type::index_type;

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
    using engine_type     = ET;
    using element_type    = typename engine_type::element_type;
    using value_type      = typename engine_type::value_type;
    using reference       = typename engine_type::reference;
    using const_reference = typename engine_type::const_reference;
    using index_type      = typename engine_type::index_type;

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
    using engine_type     = ET;
    using element_type    = typename engine_type::element_type;
    using value_type      = typename engine_type::value_type;
    using reference       = typename engine_type::const_reference;
    using const_reference = typename engine_type::const_reference;
    using index_type      = typename engine_type::index_type;

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
    using engine_type     = ET;
    using element_type    = typename engine_type::element_type;
    using value_type      = typename engine_type::value_type;
    using reference       = typename engine_type::reference;
    using const_reference = typename engine_type::const_reference;
    using index_type      = typename engine_type::index_type;

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
    using engine_type     = ET;
    using element_type    = typename engine_type::element_type;
    using value_type      = typename engine_type::value_type;
    using reference       = typename engine_type::const_reference;
    using const_reference = typename engine_type::const_reference;
    using index_type      = typename engine_type::index_type;

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
        1;
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
        1;
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
    using engine_type     = ET;
    using element_type    = typename engine_type::element_type;
    using value_type      = typename engine_type::value_type;
    using reference       = typename engine_type::reference;
    using const_reference = typename engine_type::const_reference;
    using index_type      = typename engine_type::index_type;

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
    using engine_type     = ET;
    using element_type    = typename engine_type::element_type;
    using value_type      = typename engine_type::value_type;
    using reference       = typename engine_type::const_reference;
    using const_reference = typename engine_type::const_reference;
    using index_type      = typename engine_type::index_type;

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

#if 0
#undef LA_MVET2
#define LA_MVET2    matrix_view_engine<NVET,MCT,VFT>

//=================================================================================================
//  Matrix negation engine, meant to act as read-only view of a matrix's negation in expressions,
//  to help avoid unnecessary allocation and/or element copying.
//==================================================================================================
//
//- This partial specialization is for the first-level negation view, which points directly to
//  an instance of owning engine type ET.
//
template<class ET>
class matrix_view_engine<ET, readable_matrix_engine_tag, negation_view_tag>
{
    static_assert(is_matrix_engine_v<ET>);
    using engine_const_span_type = detail::noe_const_mdspan_t<ET, readable_matrix_engine_tag>;

  public:
    //- Type aliases pertaining to the underlying engine type.
    //
    using engine_type        = ET;
    using owning_engine_type = ET;

    //- Type aliases pertaining to this view engine type.
    //
    using engine_category  = readable_matrix_engine_tag;
    using value_type       = typename engine_type::value_type;
    using element_type     = typename engine_type::element_type;
    using pointer          = typename engine_type::const_pointer;
    using const_pointer    = typename engine_type::const_pointer;
    using reference        = typename engine_type::element_type;
    using const_reference  = typename engine_type::element_type;
    using difference_type  = typename engine_type::difference_type;
    using index_type       = typename engine_type::index_type;
    using index_tuple_type = typename engine_type::index_tuple_type;
    using span_type        = detail::noe_mdspan_negation_t<engine_const_span_type>;
    using const_span_type  = span_type;

    //- Construct/copy/destroy
    //
    ~matrix_view_engine() noexcept = default;

    constexpr matrix_view_engine() noexcept;
    constexpr matrix_view_engine(matrix_view_engine&&) noexcept = default;
    constexpr matrix_view_engine(matrix_view_engine const&) noexcept = default;
    explicit constexpr matrix_view_engine(engine_type const& eng);

    constexpr matrix_view_engine&    operator =(matrix_view_engine&&) noexcept = default;
    constexpr matrix_view_engine&    operator =(matrix_view_engine const&) noexcept = default;

    //- Status
    //
    constexpr bool              is_valid() const noexcept;

    //- Capacity
    //
    constexpr index_type        columns() const noexcept;
    constexpr index_type        rows() const noexcept;
    constexpr index_tuple_type  size() const noexcept;

    constexpr index_type        column_capacity() const noexcept;
    constexpr index_type        row_capacity() const noexcept;
    constexpr index_tuple_type  capacity() const noexcept;

    //- Element access
    //
    constexpr reference         operator ()(index_type i, index_type j) const;

    //- Data access
    //
    constexpr owning_engine_type const&     owning_engine() const noexcept;

    template<class ET2 = ET, detail::enable_if_spannable<ET, ET2> = true>
    constexpr span_type         span() const noexcept;

    //- Modifiers
    //
    constexpr void              swap(matrix_view_engine& rhs) noexcept;

  private:
    engine_type const*  mp_engine;      //- Points to an owning engine
};

//------------------------
//- Construct/copy/destroy
//
template<class ET> constexpr
matrix_view_engine<ET, readable_matrix_engine_tag, negation_view_tag>::matrix_view_engine() noexcept
:   mp_engine(nullptr)
{}

template<class ET> constexpr
matrix_view_engine<ET, readable_matrix_engine_tag, negation_view_tag>::matrix_view_engine
(engine_type const& eng)
:   mp_engine(&eng)
{}

//----------
//- Status
//
template<class ET> constexpr auto
matrix_view_engine<ET, readable_matrix_engine_tag, negation_view_tag>::is_valid() const noexcept
-> bool
{
    return mp_engine != nullptr;
}

//----------
//- Capacity
//
template<class ET> constexpr auto
matrix_view_engine<ET, readable_matrix_engine_tag, negation_view_tag>::columns() const noexcept
-> index_type
{
    return mp_engine->columns();
}

template<class ET> constexpr auto
matrix_view_engine<ET, readable_matrix_engine_tag, negation_view_tag>::rows() const noexcept
-> index_type
{
    return mp_engine->rows();
}

template<class ET> constexpr auto
matrix_view_engine<ET, readable_matrix_engine_tag, negation_view_tag>::size() const noexcept
-> index_tuple_type
{
    return mp_engine->size();
}

template<class ET> constexpr auto
matrix_view_engine<ET, readable_matrix_engine_tag, negation_view_tag>::column_capacity() const noexcept
-> index_type
{
    return mp_engine->columns();
}

template<class ET> constexpr auto
matrix_view_engine<ET, readable_matrix_engine_tag, negation_view_tag>::row_capacity() const noexcept
-> index_type
{
    return mp_engine->rows();
}

template<class ET> constexpr auto
matrix_view_engine<ET, readable_matrix_engine_tag, negation_view_tag>::capacity() const noexcept
-> index_tuple_type
{
    return mp_engine->size();
}

//----------------
//- Element access
//
template<class ET> constexpr auto
matrix_view_engine<ET, readable_matrix_engine_tag, negation_view_tag>::operator ()
(index_type i, index_type j) const
-> reference
{
    return -(*mp_engine)(i, j);
}

//-------------
//- Data access
//
template<class ET> constexpr auto
matrix_view_engine<ET, readable_matrix_engine_tag, negation_view_tag>::owning_engine() const noexcept
-> owning_engine_type const&
{
    return mp_engine->owning_engine();
}

template<class ET>
template<class ET2, detail::enable_if_spannable<ET, ET2>> constexpr auto
matrix_view_engine<ET, readable_matrix_engine_tag, negation_view_tag>::span() const noexcept
-> span_type
{
    return detail::noe_mdspan_negation(mp_engine->span());
}

//-----------
//- Modifiers
//
template<class ET> constexpr void
matrix_view_engine<ET, readable_matrix_engine_tag, negation_view_tag>::swap(matrix_view_engine& rhs) noexcept
{
    std::swap(mp_engine, rhs.mp_engine);
}

//--------------------------------------------------------------------------------------------------
//- This partial specialization is for second-level and higher negation views.
//
template<class NVET, class MCT, class VFT>
class matrix_view_engine<matrix_view_engine<NVET,MCT,VFT>, readable_matrix_engine_tag, negation_view_tag>
{
    using engine_const_span_type = typename matrix_view_engine<NVET,MCT,VFT>::const_span_type;

  public:
    //- Type aliases pertaining to the underlying engine type.
    //
    using engine_type        = matrix_view_engine<NVET,MCT,VFT>;
    using owning_engine_type = typename engine_type::owning_engine_type;

    //- Type aliases pertaining to this view engine type.
    //
    using engine_category  = readable_matrix_engine_tag;
    using value_type       = typename engine_type::value_type;
    using element_type     = typename engine_type::element_type;
    using pointer          = typename engine_type::const_pointer;
    using const_pointer    = typename engine_type::const_pointer;
    using reference        = typename engine_type::element_type;
    using const_reference  = typename engine_type::element_type;
    using difference_type  = typename engine_type::difference_type;
    using index_type       = typename engine_type::index_type;
    using index_tuple_type = typename engine_type::index_tuple_type;
    using span_type        = detail::noe_mdspan_negation_t<engine_const_span_type>;
    using const_span_type  = span_type;

    //- Construct/copy/destroy
    //
    ~matrix_view_engine() noexcept = default;

    constexpr matrix_view_engine() noexcept = default;
    constexpr matrix_view_engine(matrix_view_engine&&) noexcept = default;
    constexpr matrix_view_engine(matrix_view_engine const&) noexcept = default;

    explicit constexpr matrix_view_engine(engine_type const& eng);
    explicit constexpr matrix_view_engine(owning_engine_type const& eng);

    constexpr matrix_view_engine&    operator =(matrix_view_engine&&) noexcept = default;
    constexpr matrix_view_engine&    operator =(matrix_view_engine const&) noexcept = default;

    //- Status
    //
    constexpr bool              is_valid() const noexcept;

    //- Capacity
    //
    constexpr index_type        columns() const noexcept;
    constexpr index_type        rows() const noexcept;
    constexpr index_tuple_type  size() const noexcept;

    constexpr index_type        column_capacity() const noexcept;
    constexpr index_type        row_capacity() const noexcept;
    constexpr index_tuple_type  capacity() const noexcept;

    //- Element access
    //
    constexpr reference         operator ()(index_type i, index_type j) const;

    //- Data access
    //
    constexpr owning_engine_type const&     owning_engine() const noexcept;

    template<class ET2 = LA_MVET2, detail::enable_if_spannable<LA_MVET2, ET2> = true>
    constexpr span_type         span() const noexcept;

    //- Modifiers
    //
    constexpr void              swap(matrix_view_engine& rhs) noexcept;

  private:
    engine_type     m_engine;   //- A non-owning (view) engine
};

//------------------------
//- Construct/copy/destroy
//
template<class NVET, class MCT, class VFT> constexpr
matrix_view_engine<LA_MVET2, readable_matrix_engine_tag, negation_view_tag>::matrix_view_engine
(engine_type const& eng)
:   m_engine(eng)
{}

template<class NVET, class MCT, class VFT> constexpr
matrix_view_engine<LA_MVET2, readable_matrix_engine_tag, negation_view_tag>::matrix_view_engine
(owning_engine_type const& eng)
:   m_engine(eng)
{}

//----------
//- Status
//
template<class NVET, class MCT, class VFT> constexpr bool
matrix_view_engine<LA_MVET2, readable_matrix_engine_tag, negation_view_tag>::is_valid() const noexcept
{
    return m_engine.is_valid();
}

//----------
//- Capacity
//
template<class NVET, class MCT, class VFT> constexpr auto
matrix_view_engine<LA_MVET2, readable_matrix_engine_tag, negation_view_tag>::columns() const noexcept
-> index_type
{
    return m_engine.columns();
}

template<class NVET, class MCT, class VFT> constexpr auto
matrix_view_engine<LA_MVET2, readable_matrix_engine_tag, negation_view_tag>::rows() const noexcept
-> index_type
{
    return m_engine.rows();
}

template<class NVET, class MCT, class VFT> constexpr auto
matrix_view_engine<LA_MVET2, readable_matrix_engine_tag, negation_view_tag>::size() const noexcept
-> index_tuple_type
{
    return m_engine.size();
}

template<class NVET, class MCT, class VFT> constexpr auto
matrix_view_engine<LA_MVET2, readable_matrix_engine_tag, negation_view_tag>::column_capacity() const noexcept
-> index_type
{
    return m_engine.columns();
}

template<class NVET, class MCT, class VFT> constexpr auto
matrix_view_engine<LA_MVET2, readable_matrix_engine_tag, negation_view_tag>::row_capacity() const noexcept
-> index_type
{
    return m_engine.rows();
}

template<class NVET, class MCT, class VFT> constexpr auto
matrix_view_engine<LA_MVET2, readable_matrix_engine_tag, negation_view_tag>::capacity() const noexcept
-> index_tuple_type
{
    return m_engine.size();
}

//----------------
//- Element access
//
template<class NVET, class MCT, class VFT> constexpr auto
matrix_view_engine<LA_MVET2, readable_matrix_engine_tag, negation_view_tag>::operator ()
(index_type i, index_type j) const
-> reference
{
    return -m_engine(i, j);
}

//-------------
//- Data access
//
template<class NVET, class MCT, class VFT> constexpr auto
matrix_view_engine<LA_MVET2, readable_matrix_engine_tag, negation_view_tag>::owning_engine() const noexcept
-> owning_engine_type const&
{
    return m_engine.owning_engine();
}

template<class NVET, class MCT, class VFT>
template<class ET2, detail::enable_if_spannable<LA_MVET2, ET2>> constexpr auto
matrix_view_engine<LA_MVET2, readable_matrix_engine_tag, negation_view_tag>::span() const noexcept
-> span_type
{
    return detail::noe_mdspan_negation(m_engine.span());
}

//-----------
//- Modifiers
//
template<class NVET, class MCT, class VFT> constexpr void
matrix_view_engine<LA_MVET2, readable_matrix_engine_tag, negation_view_tag>::swap(matrix_view_engine& rhs) noexcept
{
    std::swap(m_engine, rhs.m_engine);
}


//==================================================================================================
//  Matrix transpose engine, meant to act as a read-only view of a matrix's transpose in
//  expressions, to help avoid unnecessary allocation and/or element copying.
//==================================================================================================
//
//- This partial specialization is for the first-level transpose view, which points directly to
//  an instance of owning engine type ET.
//
template<class ET>
class matrix_view_engine<ET, readable_matrix_engine_tag, transpose_view_tag>
{
    static_assert(is_matrix_engine_v<ET>);
    using engine_const_span_type = detail::noe_const_mdspan_t<ET, readable_matrix_engine_tag>;

  public:
    //- Type aliases pertaining to the underlying engine type.
    //
    using engine_type        = ET;
    using owning_engine_type = engine_type;

    //- Type aliases pertaining to this view engine type.
    //
    using engine_category  = readable_matrix_engine_tag;
    using element_type     = typename engine_type::element_type;
    using value_type       = typename engine_type::value_type;
    using pointer          = typename engine_type::const_pointer;
    using const_pointer    = typename engine_type::const_pointer;
    using reference        = typename engine_type::const_reference;
    using const_reference  = typename engine_type::const_reference;
    using difference_type  = typename engine_type::difference_type;
    using index_type       = typename engine_type::index_type;
    using index_tuple_type = typename engine_type::index_tuple_type;
    using span_type        = detail::noe_mdspan_transpose_t<engine_const_span_type>;
    using const_span_type  = span_type;

    //- Construct/copy/destroy
    //
    ~matrix_view_engine() noexcept = default;

    constexpr matrix_view_engine();
    constexpr matrix_view_engine(matrix_view_engine&&) noexcept = default;
    constexpr matrix_view_engine(matrix_view_engine const&) = default;
    explicit constexpr matrix_view_engine(engine_type const& eng);

    constexpr matrix_view_engine&     operator =(matrix_view_engine&&) noexcept = default;
    constexpr matrix_view_engine&     operator =(matrix_view_engine const&) = default;

    //- Status
    //
    constexpr bool              is_valid() const noexcept;

    //- Capacity
    //
    constexpr index_type        columns() const noexcept;
    constexpr index_type        rows() const noexcept;
    constexpr index_tuple_type  size() const noexcept;

    constexpr index_type        column_capacity() const noexcept;
    constexpr index_type        row_capacity() const noexcept;
    constexpr index_tuple_type  capacity() const noexcept;

    //- Element access
    //
    constexpr reference         operator ()(index_type i, index_type j) const;

    //- Data access
    //
    constexpr owning_engine_type const&     owning_engine() const noexcept;

    template<class ET2 = ET, detail::enable_if_spannable<ET, ET2> = true>
    constexpr span_type         span() const noexcept;

    //- Modifiers
    //
    constexpr void              swap(matrix_view_engine& rhs) noexcept;

  private:
    engine_type const*  mp_engine;      //- Points to an owning engine
};

//------------------------
//- Construct/copy/destroy
//
template<class ET> constexpr
matrix_view_engine<ET, readable_matrix_engine_tag, transpose_view_tag>::matrix_view_engine()
:   mp_engine(nullptr)
{}

template<class ET> constexpr
matrix_view_engine<ET, readable_matrix_engine_tag, transpose_view_tag>::matrix_view_engine
(engine_type const& eng)
:   mp_engine(&eng)
{}

//----------
//- Status
//
template<class ET> constexpr auto
matrix_view_engine<ET, readable_matrix_engine_tag, transpose_view_tag>::is_valid() const noexcept
-> bool
{
    return mp_engine != nullptr;
}

//----------
//- Capacity
//
template<class ET> constexpr auto
matrix_view_engine<ET, readable_matrix_engine_tag, transpose_view_tag>::columns() const noexcept
-> index_type
{
    return mp_engine->rows();
}

template<class ET> constexpr auto
matrix_view_engine<ET, readable_matrix_engine_tag, transpose_view_tag>::rows() const noexcept
-> index_type
{
    return mp_engine->columns();
}

template<class ET> constexpr auto
matrix_view_engine<ET, readable_matrix_engine_tag, transpose_view_tag>::size() const noexcept
-> index_tuple_type
{
    return index_tuple_type(mp_engine->columns(), mp_engine->rows());
}

template<class ET> constexpr auto
matrix_view_engine<ET, readable_matrix_engine_tag, transpose_view_tag>::column_capacity() const noexcept
-> index_type
{
    return mp_engine->rows();
}

template<class ET> constexpr auto
matrix_view_engine<ET, readable_matrix_engine_tag, transpose_view_tag>::row_capacity() const noexcept
-> index_type
{
    return mp_engine->columns();
}

template<class ET> constexpr auto
matrix_view_engine<ET, readable_matrix_engine_tag, transpose_view_tag>::capacity() const noexcept
-> index_tuple_type
{
    return index_tuple_type(mp_engine->columns(), mp_engine->rows());
}

//----------------
//- Element access
//
template<class ET> constexpr auto
matrix_view_engine<ET, readable_matrix_engine_tag, transpose_view_tag>::operator ()
(index_type i, index_type j) const
-> reference
{
    return (*mp_engine)(j, i);
}

//-------------
//- Data access
//
template<class ET> constexpr auto
matrix_view_engine<ET, readable_matrix_engine_tag, transpose_view_tag>::owning_engine() const noexcept
-> owning_engine_type const&
{
    return mp_engine->owning_engine();
}

template<class ET>
template<class ET2, detail::enable_if_spannable<ET, ET2>> constexpr auto
matrix_view_engine<ET, readable_matrix_engine_tag, transpose_view_tag>::span() const noexcept
-> span_type
{
    return detail::noe_mdspan_transpose(mp_engine->span());
}

//-----------
//- Modifiers
//
template<class ET> constexpr void
matrix_view_engine<ET, readable_matrix_engine_tag, transpose_view_tag>::swap(matrix_view_engine& rhs) noexcept
{
    if (&rhs != this)
    {
        std::swap(mp_engine, rhs.mp_engine);
    }
}


//--------------------------------------------------------------------------------------------------
//- This partial specialization is for second-level and higher transpose views.
//
template<class NVET, class MCT, class VFT>
class matrix_view_engine<matrix_view_engine<NVET,MCT,VFT>, readable_matrix_engine_tag, transpose_view_tag>
{
    using engine_const_span_type = typename matrix_view_engine<NVET,MCT,VFT>::const_span_type;

  public:
    //- Type aliases pertaining to the underlying engine type.
    //
    using engine_type        = matrix_view_engine<NVET,MCT,VFT>;
    using owning_engine_type = typename engine_type::owning_engine_type;

    //- Type aliases pertaining to this view engine type.
    //
    using engine_category  = readable_matrix_engine_tag;
    using value_type       = typename engine_type::value_type;
    using element_type     = typename engine_type::element_type;
    using pointer          = typename engine_type::const_pointer;
    using const_pointer    = typename engine_type::const_pointer;
    using reference        = typename engine_type::element_type;
    using const_reference  = typename engine_type::element_type;
    using difference_type  = typename engine_type::difference_type;
    using index_type       = typename engine_type::index_type;
    using index_tuple_type = typename engine_type::index_tuple_type;
    using span_type        = detail::noe_mdspan_transpose_t<engine_const_span_type>;
    using const_span_type  = span_type;

    //- Construct/copy/destroy
    //
    ~matrix_view_engine() noexcept = default;

    constexpr matrix_view_engine() noexcept = default;
    constexpr matrix_view_engine(matrix_view_engine&&) noexcept = default;
    constexpr matrix_view_engine(matrix_view_engine const&) noexcept = default;

    explicit constexpr matrix_view_engine(engine_type const& eng);
    explicit constexpr matrix_view_engine(owning_engine_type const& eng);

    constexpr matrix_view_engine&    operator =(matrix_view_engine&&) noexcept = default;
    constexpr matrix_view_engine&    operator =(matrix_view_engine const&) noexcept = default;

    //- Status
    //
    constexpr bool              is_valid() const noexcept;

    //- Capacity
    //
    constexpr index_type        columns() const noexcept;
    constexpr index_type        rows() const noexcept;
    constexpr index_tuple_type  size() const noexcept;

    constexpr index_type        column_capacity() const noexcept;
    constexpr index_type        row_capacity() const noexcept;
    constexpr index_tuple_type  capacity() const noexcept;

    //- Element access
    //
    constexpr reference         operator ()(index_type i, index_type j) const;

    //- Data access
    //
    constexpr owning_engine_type const&     owning_engine() const noexcept;

    template<class ET2 = LA_MVET2, detail::enable_if_spannable<LA_MVET2, ET2> = true>
    constexpr span_type         span() const noexcept;

    //- Modifiers
    //
    constexpr void              swap(matrix_view_engine& rhs) noexcept;

  private:
    engine_type     m_engine;   //- A non-owning (view) engine
};

//------------------------
//- Construct/copy/destroy
//
template<class NVET, class MCT, class VFT> constexpr
matrix_view_engine<LA_MVET2, readable_matrix_engine_tag, transpose_view_tag>::matrix_view_engine
(engine_type const& eng)
:   m_engine(eng)
{}

template<class NVET, class MCT, class VFT> constexpr
matrix_view_engine<LA_MVET2, readable_matrix_engine_tag, transpose_view_tag>::matrix_view_engine
(owning_engine_type const& eng)
:   m_engine(eng)
{}

//----------
//- Status
//
template<class NVET, class MCT, class VFT> constexpr bool
matrix_view_engine<LA_MVET2, readable_matrix_engine_tag, transpose_view_tag>::is_valid() const noexcept
{
    return m_engine.is_valid();
}

//----------
//- Capacity
//
template<class NVET, class MCT, class VFT> constexpr auto
matrix_view_engine<LA_MVET2, readable_matrix_engine_tag, transpose_view_tag>::columns() const noexcept
-> index_type
{
    return m_engine.rows();
}

template<class NVET, class MCT, class VFT> constexpr auto
matrix_view_engine<LA_MVET2, readable_matrix_engine_tag, transpose_view_tag>::rows() const noexcept
-> index_type
{
    return m_engine.columns();
}

template<class NVET, class MCT, class VFT> constexpr auto
matrix_view_engine<LA_MVET2, readable_matrix_engine_tag, transpose_view_tag>::size() const noexcept
-> index_tuple_type
{
    return index_tuple_type(m_engine.columns(), m_engine.rows());
}

template<class NVET, class MCT, class VFT> constexpr auto
matrix_view_engine<LA_MVET2, readable_matrix_engine_tag, transpose_view_tag>::column_capacity() const noexcept
-> index_type
{
    return m_engine.rows();
}

template<class NVET, class MCT, class VFT> constexpr auto
matrix_view_engine<LA_MVET2, readable_matrix_engine_tag, transpose_view_tag>::row_capacity() const noexcept
-> index_type
{
    return m_engine.columns();
}

template<class NVET, class MCT, class VFT> constexpr auto
matrix_view_engine<LA_MVET2, readable_matrix_engine_tag, transpose_view_tag>::capacity() const noexcept
-> index_tuple_type
{
    return index_tuple_type(m_engine.columns(), m_engine.rows());
}

//----------------
//- Element access
//
template<class NVET, class MCT, class VFT> constexpr auto
matrix_view_engine<LA_MVET2, readable_matrix_engine_tag, transpose_view_tag>::operator ()
(index_type i, index_type j) const
-> reference
{
    return m_engine(j, i);
}

//-------------
//- Data access
//
template<class NVET, class MCT, class VFT> constexpr auto
matrix_view_engine<LA_MVET2, readable_matrix_engine_tag, transpose_view_tag>::owning_engine() const noexcept
-> owning_engine_type const&
{
    return m_engine.owning_engine();
}

template<class NVET, class MCT, class VFT>
template<class ET2, detail::enable_if_spannable<LA_MVET2, ET2>> constexpr auto
matrix_view_engine<LA_MVET2, readable_matrix_engine_tag, transpose_view_tag>::span() const noexcept
-> span_type
{
    return detail::noe_mdspan_transpose(m_engine.span());
}

//-----------
//- Modifiers
//
template<class NVET, class MCT, class VFT> constexpr void
matrix_view_engine<LA_MVET2, readable_matrix_engine_tag, transpose_view_tag>::swap(matrix_view_engine& rhs) noexcept
{
    std::swap(m_engine, rhs.m_engine);
}

/*
//==================================================================================================
//  Matrix hermitian engine, meant to act as a read-only view of a matrix's conjugate transpose
//  in expressions, to help avoid unnecessary allocation and/or element copying.
//==================================================================================================
//
template<class ET>
class matrix_view_engine<ET, readable_matrix_engine_tag, hermitian_view_tag>
{
    static_assert(is_matrix_engine_v<ET>);

  public:
    //- Types
    //
    using engine_category  = readable_matrix_engine_tag;
    using element_type     = typename ET::element_type;
    using value_type       = typename ET::value_type;
    using pointer          = typename ET::const_pointer;
    using const_pointer    = typename ET::const_pointer;
    using reference        = typename ET::const_reference;
    using const_reference  = typename ET::const_reference;
    using difference_type  = typename ET::difference_type;
    using index_type       = typename ET::index_type;
    using index_tuple_type = extents<dynamic_extent, dynamic_extent>;
    using span_type        = detail::noe_mdspan_transpose_t<detail::noe_const_mdspan_t<ET, readable_matrix_engine_tag>>;
    using const_span_type  = detail::noe_mdspan_transpose_t<detail::noe_const_mdspan_t<ET, readable_matrix_engine_tag>>;
    using referent_type    = detail::noe_engine_t<ET, readable_matrix_engine_tag>;

    //- Type aliases pertaining to the underlying engine type.
    //

    //- Construct/copy/destroy
    //
    ~matrix_view_engine() noexcept = default;

    constexpr matrix_view_engine();
    constexpr matrix_view_engine(matrix_view_engine&&) noexcept = default;
    constexpr matrix_view_engine(matrix_view_engine const&) = default;
    explicit constexpr matrix_view_engine(referent_type& eng);

    constexpr matrix_view_engine&     operator =(matrix_view_engine&&) noexcept = default;
    constexpr matrix_view_engine&     operator =(matrix_view_engine const&) = default;

    //- Status
    //
    constexpr bool      is_valid() const noexcept;

    //- Capacity
    //
    constexpr index_type        columns() const noexcept;
    constexpr index_type        rows() const noexcept;
    constexpr index_tuple_type  size() const noexcept;

    constexpr index_type        column_capacity() const noexcept;
    constexpr index_type        row_capacity() const noexcept;
    constexpr index_tuple_type  capacity() const noexcept;

    //- Element access
    //
    constexpr reference     operator ()(index_type i, index_type j) const;

    //- Data access
    //
    template<class ET2 = ET, detail::enable_if_spannable<ET, ET2> = true>
    constexpr span_type     span() const noexcept;

    //- Modifiers
    //
    constexpr void          swap(matrix_view_engine& rhs) noexcept;

  private:
    template<class ET2, class OT2>  friend class matrix;

    referent_type*  mp_engine;
};

//------------------------
//- Construct/copy/destroy
//
template<class ET> constexpr
matrix_view_engine<ET, readable_matrix_engine_tag, hermitian_view_tag>::matrix_view_engine()
:   mp_engine(nullptr)
{}

//----------
//- Status
//
template<class ET> constexpr
bool
matrix_view_engine<ET, readable_matrix_engine_tag, hermitian_view_tag>::is_valid() const noexcept
{
    return mp_engine != nullptr;
}

//----------
//- Capacity
//
template<class ET> constexpr
typename matrix_view_engine<ET, readable_matrix_engine_tag, hermitian_view_tag>::index_type
matrix_view_engine<ET, readable_matrix_engine_tag, hermitian_view_tag>::columns() const noexcept
{
    return mp_engine->rows();
}

template<class ET> constexpr
typename matrix_view_engine<ET, readable_matrix_engine_tag, hermitian_view_tag>::index_type
matrix_view_engine<ET, readable_matrix_engine_tag, hermitian_view_tag>::rows() const noexcept
{
    return mp_engine->columns();
}

template<class ET> constexpr
typename matrix_view_engine<ET, readable_matrix_engine_tag, hermitian_view_tag>::index_tuple_type
matrix_view_engine<ET, readable_matrix_engine_tag, hermitian_view_tag>::size() const noexcept
{
    return index_tuple_type(mp_engine->columns(), mp_engine->rows());
}

template<class ET> constexpr
typename matrix_view_engine<ET, readable_matrix_engine_tag, hermitian_view_tag>::index_type
matrix_view_engine<ET, readable_matrix_engine_tag, hermitian_view_tag>::column_capacity() const noexcept
{
    return mp_engine->rows();
}

template<class ET> constexpr
typename matrix_view_engine<ET, readable_matrix_engine_tag, hermitian_view_tag>::index_type
matrix_view_engine<ET, readable_matrix_engine_tag, hermitian_view_tag>::row_capacity() const noexcept
{
    return mp_engine->columns();
}

template<class ET> constexpr
typename matrix_view_engine<ET, readable_matrix_engine_tag, hermitian_view_tag>::index_tuple_type
matrix_view_engine<ET, readable_matrix_engine_tag, hermitian_view_tag>::capacity() const noexcept
{
    return index_tuple_type(mp_engine->columns(), mp_engine->rows());
}

//----------------
//- Element access
//
template<class ET> constexpr
typename matrix_view_engine<ET, readable_matrix_engine_tag, hermitian_view_tag>::reference
matrix_view_engine<ET, readable_matrix_engine_tag, hermitian_view_tag>::operator ()(index_type i, index_type j) const
{
    return conj((*mp_engine)(j, i));
}

//-------------
//- Data access
//
template<class ET>
template<class ET2, detail::enable_if_spannable<ET, ET2>> constexpr
typename matrix_view_engine<ET, readable_matrix_engine_tag, hermitian_view_tag>::span_type
matrix_view_engine<ET, readable_matrix_engine_tag, hermitian_view_tag>::span() const noexcept
{
    return detail::noe_mdspan_transpose(mp_engine->span());
}

//-----------
//- Modifiers
//
template<class ET> constexpr
void
matrix_view_engine<ET, readable_matrix_engine_tag, hermitian_view_tag>::swap(matrix_view_engine& rhs) noexcept
{
    if (&rhs != this)
    {
        std::swap(mp_engine, rhs.mp_engine);
    }
}

template<class ET> constexpr
matrix_view_engine<ET, readable_matrix_engine_tag, hermitian_view_tag>::matrix_view_engine(referent_type& eng)
:   mp_engine(&eng)
{}
*/

//==================================================================================================
//  Sub-matrix engine, meant to act as a read/write view of a subset of a submatrix in expressions,
//  to help avoid unnecessary allocation and/or element copying.
//==================================================================================================
//
//- This partial specialization is for a first-level readable submatrix view, which points
//  directly to an instance of owning engine type ET.
//
template<class ET>
class matrix_view_engine<ET, readable_matrix_engine_tag, subset_view_tag>
{
    static_assert(is_matrix_engine_v<ET>);
    using engine_const_span_type = detail::noe_const_mdspan_t<ET, readable_matrix_engine_tag>;

  public:
    //- Type aliases pertaining to the underlying engine type.
    //
    using engine_type        = ET;
    using owning_engine_type = engine_type;

    //- Type aliases pertaining to this view engine type.
    //
    using engine_category  = readable_matrix_engine_tag;
    using element_type     = typename engine_type::element_type;
    using value_type       = typename engine_type::value_type;
    using pointer          = typename engine_type::const_pointer;
    using const_pointer    = typename engine_type::const_pointer;
    using reference        = typename engine_type::const_reference;
    using const_reference  = typename engine_type::const_reference;
    using difference_type  = typename engine_type::difference_type;
    using index_type       = typename engine_type::index_type;
    using index_tuple_type = typename engine_type::index_tuple_type;
    using span_type        = detail::noe_mdspan_submatrix_t<engine_const_span_type>;
    using const_span_type  = span_type;

    //- Construct/copy/destroy
    //
    ~matrix_view_engine() noexcept = default;

    constexpr matrix_view_engine();
    constexpr matrix_view_engine(matrix_view_engine&&) noexcept = default;
    constexpr matrix_view_engine(matrix_view_engine const&) noexcept = default;
    constexpr matrix_view_engine(engine_type const& eng,
                                 index_type ri, index_type rn, index_type ci, index_type cn);

    constexpr matrix_view_engine&     operator =(matrix_view_engine&&) noexcept = default;
    constexpr matrix_view_engine&     operator =(matrix_view_engine const&) noexcept = default;

    //- Status
    //
    constexpr bool              is_valid() const noexcept;

    //- Capacity
    //
    constexpr index_type        columns() const noexcept;
    constexpr index_type        rows() const noexcept;
    constexpr index_tuple_type  size() const noexcept;

    constexpr index_type        column_capacity() const noexcept;
    constexpr index_type        row_capacity() const noexcept;
    constexpr index_tuple_type  capacity() const noexcept;

    //- Element access
    //
    constexpr reference         operator ()(index_type i, index_type j) const;

    //- Data access
    //
    constexpr owning_engine_type const&     owning_engine() const noexcept;

    template<class ET2 = ET, detail::enable_if_spannable<ET, ET2> = true>
    constexpr span_type         span() const noexcept;

    //- Modifiers
    //
    constexpr void              swap(matrix_view_engine& rhs) noexcept;

  private:
    engine_type const*  mp_engine;
    index_type          m_row_start;
    index_type          m_row_count;
    index_type          m_col_start;
    index_type          m_col_count;
};

//------------------------
//- Construct/copy/destroy
//
template<class ET> constexpr
matrix_view_engine<ET, readable_matrix_engine_tag, subset_view_tag>::matrix_view_engine()
:   mp_engine(nullptr)
,   m_row_start(0)
,   m_row_count(0)
,   m_col_start(0)
,   m_col_count(0)
{}

template<class ET> constexpr
matrix_view_engine<ET, readable_matrix_engine_tag, subset_view_tag>::matrix_view_engine
(engine_type const& eng, index_type ri, index_type rn, index_type ci, index_type cn)
:   mp_engine(&eng)
,   m_row_start(ri)
,   m_row_count(rn)
,   m_col_start(ci)
,   m_col_count(cn)
{}

//----------
//- Status
//
template<class ET> constexpr auto
matrix_view_engine<ET, readable_matrix_engine_tag, subset_view_tag>::is_valid() const noexcept
-> bool
{
    return mp_engine != nullptr;
}

//----------
//- Capacity
//
template<class ET> constexpr auto
matrix_view_engine<ET, readable_matrix_engine_tag, subset_view_tag>::columns() const noexcept
-> index_type
{
    return m_col_count;
}

template<class ET> constexpr auto
matrix_view_engine<ET, readable_matrix_engine_tag, subset_view_tag>::rows() const noexcept
-> index_type
{
    return m_row_count;
}

template<class ET> constexpr auto
matrix_view_engine<ET, readable_matrix_engine_tag, subset_view_tag>::size() const noexcept
-> index_tuple_type
{
    return index_tuple_type(m_row_count, m_col_count);
}

template<class ET> constexpr auto
matrix_view_engine<ET, readable_matrix_engine_tag, subset_view_tag>::column_capacity() const noexcept
-> index_type
{
    return m_col_count;
}

template<class ET> constexpr auto
matrix_view_engine<ET, readable_matrix_engine_tag, subset_view_tag>::row_capacity() const noexcept
-> index_type
{
    return m_row_count;
}

template<class ET> constexpr auto
matrix_view_engine<ET, readable_matrix_engine_tag, subset_view_tag>::capacity() const noexcept
-> index_tuple_type
{
    return index_tuple_type(m_row_count, m_col_count);
}

//----------------
//- Element access
//
template<class ET> constexpr auto
matrix_view_engine<ET, readable_matrix_engine_tag, subset_view_tag>::operator ()
(index_type i, index_type j) const
-> reference
{
    return (*mp_engine)(i + m_row_start, j + m_col_start);
}

//-------------
//- Data access
//
template<class ET> constexpr auto
matrix_view_engine<ET, readable_matrix_engine_tag, subset_view_tag>::owning_engine() const noexcept
-> owning_engine_type const&
{
    return mp_engine->owning_engine();
}

template<class ET>
template<class ET2, detail::enable_if_spannable<ET, ET2>> constexpr auto
matrix_view_engine<ET, readable_matrix_engine_tag, subset_view_tag>::span() const noexcept
-> span_type
{
    return detail::noe_mdspan_submatrix(mp_engine->span(), m_row_start, m_row_count,
                                                           m_col_start, m_col_count);
}

//-----------
//- Modifiers
//
template<class ET> constexpr void
matrix_view_engine<ET, readable_matrix_engine_tag, subset_view_tag>::swap(matrix_view_engine& rhs) noexcept
{
    if (&rhs != this)
    {
        std::swap(mp_engine,   rhs.mp_engine);
        std::swap(m_row_start, rhs.m_row_start);
        std::swap(m_row_count, rhs.m_row_count);
        std::swap(m_col_start, rhs.m_col_start);
        std::swap(m_col_count, rhs.m_col_count);
    }
}

//--------------------------------------------------------------------------------------------------
//- This partial specialization is for a first-level writable submatrix view, which points
//  directly to an instance of owning engine type ET.
//
template<class ET>
class matrix_view_engine<ET, writable_matrix_engine_tag, subset_view_tag>
{
    static_assert(is_matrix_engine_v<ET> && is_writable_engine_v<ET>);
    using engine_span_type       = detail::noe_mdspan_t<ET, writable_matrix_engine_tag>;
    using engine_const_span_type = detail::noe_mdspan_t<ET, readable_matrix_engine_tag>;

  public:
    //- Type aliases pertaining to the underlying engine type.
    //
    using engine_type        = ET;
    using owning_engine_type = engine_type;

    //- Type aliases pertaining to this view engine type.
    //
    using engine_category  = writable_matrix_engine_tag;
    using element_type     = typename engine_type::element_type;
    using value_type       = typename engine_type::value_type;
    using pointer          = typename engine_type::pointer;
    using const_pointer    = typename engine_type::const_pointer;
    using reference        = typename engine_type::reference;
    using const_reference  = typename engine_type::const_reference;
    using difference_type  = typename engine_type::difference_type;
    using index_type       = typename engine_type::index_type;
    using index_tuple_type = typename engine_type::index_tuple_type;
    using span_type        = detail::noe_mdspan_submatrix_t<engine_span_type>;
    using const_span_type  = detail::noe_mdspan_submatrix_t<engine_const_span_type>;

    //- Construct/copy/destroy
    //
    ~matrix_view_engine() noexcept = default;

    constexpr matrix_view_engine() noexcept;
    constexpr matrix_view_engine(matrix_view_engine&&) noexcept = default;
    constexpr matrix_view_engine(matrix_view_engine const&) noexcept = default;
    constexpr matrix_view_engine(engine_type& eng,
                                 index_type ri, index_type rn, index_type ci, index_type cn);

    constexpr matrix_view_engine&     operator =(matrix_view_engine&&) noexcept = default;
    constexpr matrix_view_engine&     operator =(matrix_view_engine const&) noexcept = default;

    template<class ET2>
    constexpr matrix_view_engine&     operator =(ET2 const& rhs);
    template<class U>
    constexpr matrix_view_engine&     operator =(initializer_list<initializer_list<U>> rhs);

    //- Status
    //
    constexpr bool              is_valid() const noexcept;

    //- Capacity
    //
    constexpr index_type        columns() const noexcept;
    constexpr index_type        rows() const noexcept;
    constexpr index_tuple_type  size() const noexcept;

    constexpr index_type        column_capacity() const noexcept;
    constexpr index_type        row_capacity() const noexcept;
    constexpr index_tuple_type  capacity() const noexcept;

    //- Element access
    //
    constexpr reference         operator ()(index_type i, index_type j) const;

    //- Data access
    //
    constexpr owning_engine_type&   owning_engine() const noexcept;

    template<class ET2 = ET, detail::enable_if_spannable<ET, ET2> = true>
    constexpr span_type         span() const noexcept;

    //- Modifiers
    //
    constexpr void              swap(matrix_view_engine& rhs) noexcept;

  private:
    engine_type*    mp_engine;
    index_type      m_row_start;
    index_type      m_row_count;
    index_type      m_col_start;
    index_type      m_col_count;
};

//------------------------
//- Construct/copy/destroy
//
template<class ET> constexpr
matrix_view_engine<ET, writable_matrix_engine_tag, subset_view_tag>::matrix_view_engine() noexcept
:   mp_engine(nullptr)
,   m_row_start(0)
,   m_row_count(0)
,   m_col_start(0)
,   m_col_count(0)
{}

template<class ET> constexpr
matrix_view_engine<ET, writable_matrix_engine_tag, subset_view_tag>::matrix_view_engine
(engine_type& eng, index_type ri, index_type rn, index_type ci, index_type cn)
:   mp_engine(&eng)
,   m_row_start(ri)
,   m_row_count(rn)
,   m_col_start(ci)
,   m_col_count(cn)
{}

template<class ET>
template<class ET2> constexpr auto
matrix_view_engine<ET, writable_matrix_engine_tag, subset_view_tag>::operator =(ET2 const& rhs)
-> matrix_view_engine&
{
    detail::check_source_engine_size(rhs, rows(), columns());
    detail::matrix_assign_from(*this, rhs);
    return *this;
}

template<class ET>
template<class U> constexpr auto
matrix_view_engine<ET, writable_matrix_engine_tag, subset_view_tag>::operator =
(initializer_list<initializer_list<U>> rhs)
-> matrix_view_engine&
{
    detail::check_source_init_list(rhs, m_row_count, m_col_count);
    detail::matrix_assign_from(*this, rhs);
    return *this;
}

//----------
//- Status
//
template<class ET> constexpr auto
matrix_view_engine<ET, writable_matrix_engine_tag, subset_view_tag>::is_valid() const noexcept
-> bool
{
    return mp_engine != nullptr;
}

//----------
//- Capacity
//
template<class ET> constexpr auto
matrix_view_engine<ET, writable_matrix_engine_tag, subset_view_tag>::columns() const noexcept
-> index_type
{
    return m_col_count;
}

template<class ET> constexpr auto
matrix_view_engine<ET, writable_matrix_engine_tag, subset_view_tag>::rows() const noexcept
-> index_type
{
    return m_row_count;
}

template<class ET> constexpr auto
matrix_view_engine<ET, writable_matrix_engine_tag, subset_view_tag>::size() const noexcept
-> index_tuple_type
{
    return index_tuple_type(m_row_count, m_col_count);
}

template<class ET> constexpr auto
matrix_view_engine<ET, writable_matrix_engine_tag, subset_view_tag>::column_capacity() const noexcept
-> index_type
{
    return m_col_count;
}

template<class ET> constexpr auto
matrix_view_engine<ET, writable_matrix_engine_tag, subset_view_tag>::row_capacity() const noexcept
-> index_type
{
    return m_row_count;
}

template<class ET> constexpr auto
matrix_view_engine<ET, writable_matrix_engine_tag, subset_view_tag>::capacity() const noexcept
-> index_tuple_type
{
    return index_tuple_type(m_row_count, m_col_count);
}

//----------------
//- Element access
//
template<class ET> constexpr auto
matrix_view_engine<ET, writable_matrix_engine_tag, subset_view_tag>::operator ()(index_type i, index_type j) const
-> reference
{
    return (*mp_engine)(i + m_row_start, j + m_col_start);
}

//-------------
//- Data access
//
template<class ET> constexpr auto
matrix_view_engine<ET, writable_matrix_engine_tag, subset_view_tag>::owning_engine() const noexcept
-> owning_engine_type&
{
    return mp_engine->owning_engine();
}

template<class ET>
template<class ET2, detail::enable_if_spannable<ET, ET2>> constexpr auto
matrix_view_engine<ET, writable_matrix_engine_tag, subset_view_tag>::span() const noexcept
-> span_type
{
    return detail::noe_mdspan_submatrix(mp_engine->span(), m_row_start, m_row_count,
                                                           m_col_start, m_col_count);
}

//-----------
//- Modifiers
//
template<class ET> constexpr void
matrix_view_engine<ET, writable_matrix_engine_tag, subset_view_tag>::swap(matrix_view_engine& rhs) noexcept
{
    if (&rhs != this)
    {
        std::swap(mp_engine,   rhs.mp_engine);
        std::swap(m_row_start, rhs.m_row_start);
        std::swap(m_row_count, rhs.m_row_count);
        std::swap(m_col_start, rhs.m_col_start);
        std::swap(m_col_count, rhs.m_col_count);
    }
}


//--------------------------------------------------------------------------------------------------
//- This partial specialization is for second-level and higher readable submatrix views.
//
template<class NVET, class MCT, class VFT>
class matrix_view_engine<matrix_view_engine<NVET,MCT,VFT>, readable_matrix_engine_tag, subset_view_tag>
{
    static_assert(is_matrix_engine_tag_v<MCT>);
    using engine_const_span_type = typename matrix_view_engine<NVET,MCT,VFT>::const_span_type;

  public:
    //- Type aliases pertaining to the underlying engine type.
    //
    using engine_type        = matrix_view_engine<NVET,MCT,VFT>;
    using owning_engine_type = typename engine_type::owning_engine_type;

    //- Type aliases pertaining to this view engine type.
    //
    using engine_category  = readable_matrix_engine_tag;
    using element_type     = typename engine_type::element_type;
    using value_type       = typename engine_type::value_type;
    using pointer          = typename engine_type::const_pointer;
    using const_pointer    = typename engine_type::const_pointer;
    using reference        = typename engine_type::const_reference;
    using const_reference  = typename engine_type::const_reference;
    using difference_type  = typename engine_type::difference_type;
    using index_type       = typename engine_type::index_type;
    using index_tuple_type = typename engine_type::index_tuple_type;
    using span_type        = detail::noe_mdspan_submatrix_t<engine_const_span_type>;
    using const_span_type  = span_type;

    //- Construct/copy/destroy
    //
    ~matrix_view_engine() noexcept = default;

    constexpr matrix_view_engine() = default;
    constexpr matrix_view_engine(matrix_view_engine&&) noexcept = default;
    constexpr matrix_view_engine(matrix_view_engine const&) noexcept = default;
    constexpr matrix_view_engine(engine_type const& eng,
                                 index_type ri, index_type rn, index_type ci, index_type cn);
    constexpr matrix_view_engine(owning_engine_type const& eng,
                                 index_type ri, index_type rn, index_type ci, index_type cn);

    constexpr matrix_view_engine&     operator =(matrix_view_engine&&) noexcept = default;
    constexpr matrix_view_engine&     operator =(matrix_view_engine const&) noexcept = default;

    //- Status
    //
    constexpr bool              is_valid() const noexcept;

    //- Capacity
    //
    constexpr index_type        columns() const noexcept;
    constexpr index_type        rows() const noexcept;
    constexpr index_tuple_type  size() const noexcept;

    constexpr index_type        column_capacity() const noexcept;
    constexpr index_type        row_capacity() const noexcept;
    constexpr index_tuple_type  capacity() const noexcept;

    //- Element access
    //
    constexpr reference         operator ()(index_type i, index_type j) const;

    //- Data access
    //
    constexpr owning_engine_type const&     owning_engine() const noexcept;

    template<class ET2 = LA_MVET2, detail::enable_if_spannable<LA_MVET2, ET2> = true>
    constexpr span_type         span() const noexcept;

    //- Modifiers
    //
    constexpr void              swap(matrix_view_engine& rhs) noexcept;

  private:
    engine_type     m_engine;
    index_type      m_row_start;
    index_type      m_row_count;
    index_type      m_col_start;
    index_type      m_col_count;
};

//------------------------
//- Construct/copy/destroy
//
template<class NVET, class MCT, class VFT> constexpr
matrix_view_engine<LA_MVET2, readable_matrix_engine_tag, subset_view_tag>::matrix_view_engine
(engine_type const& eng, index_type ri, index_type rn, index_type ci, index_type cn)
:   m_engine(eng)
,   m_row_start(ri)
,   m_row_count(rn)
,   m_col_start(ci)
,   m_col_count(cn)
{}

template<class NVET, class MCT, class VFT> constexpr
matrix_view_engine<LA_MVET2, readable_matrix_engine_tag, subset_view_tag>::matrix_view_engine
(owning_engine_type const& eng, index_type ri, index_type rn, index_type ci, index_type cn)
:   m_engine(eng)
,   m_row_start(ri)
,   m_row_count(rn)
,   m_col_start(ci)
,   m_col_count(cn)
{}

//----------
//- Status
//
template<class NVET, class MCT, class VFT> constexpr auto
matrix_view_engine<LA_MVET2, readable_matrix_engine_tag, subset_view_tag>::is_valid() const noexcept
-> bool
{
    return m_engine.is_valid();
}

//----------
//- Capacity
//
template<class NVET, class MCT, class VFT> constexpr auto
matrix_view_engine<LA_MVET2, readable_matrix_engine_tag, subset_view_tag>::columns() const noexcept
-> index_type
{
    return m_col_count;
}

template<class NVET, class MCT, class VFT> constexpr auto
matrix_view_engine<LA_MVET2, readable_matrix_engine_tag, subset_view_tag>::rows() const noexcept
-> index_type
{
    return m_row_count;
}

template<class NVET, class MCT, class VFT> constexpr auto
matrix_view_engine<LA_MVET2, readable_matrix_engine_tag, subset_view_tag>::size() const noexcept
-> index_tuple_type
{
    return index_tuple_type(m_row_count, m_col_count);
}

template<class NVET, class MCT, class VFT> constexpr auto
matrix_view_engine<LA_MVET2, readable_matrix_engine_tag, subset_view_tag>::column_capacity() const noexcept
-> index_type
{
    return m_col_count;
}

template<class NVET, class MCT, class VFT> constexpr auto
matrix_view_engine<LA_MVET2, readable_matrix_engine_tag, subset_view_tag>::row_capacity() const noexcept
-> index_type
{
    return m_row_count;
}

template<class NVET, class MCT, class VFT> constexpr auto
matrix_view_engine<LA_MVET2, readable_matrix_engine_tag, subset_view_tag>::capacity() const noexcept
-> index_tuple_type
{
    return index_tuple_type(m_row_count, m_col_count);
}

//----------------
//- Element access
//
template<class NVET, class MCT, class VFT> constexpr auto
matrix_view_engine<LA_MVET2, readable_matrix_engine_tag, subset_view_tag>::operator ()(index_type i, index_type j) const
-> reference
{
    return m_engine(i + m_row_start, j + m_col_start);
}

//-------------
//- Data access
//
template<class NVET, class MCT, class VFT> constexpr auto
matrix_view_engine<LA_MVET2, readable_matrix_engine_tag, subset_view_tag>::owning_engine() const noexcept
-> owning_engine_type const&
{
    return m_engine.owning_engine();
}

template<class NVET, class MCT, class VFT>
template<class ET2, detail::enable_if_spannable<LA_MVET2, ET2>> constexpr auto
matrix_view_engine<LA_MVET2, readable_matrix_engine_tag, subset_view_tag>::span() const noexcept
-> span_type
{
    return detail::noe_mdspan_submatrix(m_engine.span(), m_row_start, m_row_count,
                                                         m_col_start, m_col_count);
}

//-----------
//- Modifiers
//
template<class NVET, class MCT, class VFT> constexpr void
matrix_view_engine<LA_MVET2, readable_matrix_engine_tag, subset_view_tag>::swap(matrix_view_engine& rhs) noexcept
{
    if (&rhs != this)
    {
        std::swap(m_engine,    rhs.m_engine);
        std::swap(m_row_start, rhs.m_row_start);
        std::swap(m_row_count, rhs.m_row_count);
        std::swap(m_col_start, rhs.m_col_start);
        std::swap(m_col_count, rhs.m_col_count);
    }
}

//--------------------------------------------------------------------------------------------------
//- This partial specialization is for second-level and higher writable submatrix views.
//
template<class NVET, class MCT, class VFT>
class matrix_view_engine<matrix_view_engine<NVET,MCT,VFT>, writable_matrix_engine_tag, subset_view_tag>
{
    static_assert(is_writable_engine_tag_v<MCT>);

    using engine_const_span_type = typename matrix_view_engine<NVET,MCT,VFT>::const_span_type;
    using engine_span_type       = typename matrix_view_engine<NVET,MCT,VFT>::span_type;

  public:
    //- Type aliases pertaining to the underlying engine type.
    //
    using engine_type        = matrix_view_engine<NVET,MCT,VFT>;
    using owning_engine_type = typename engine_type::owning_engine_type;

    //- Type aliases pertaining to this view engine type.
    //
    using engine_category  = writable_matrix_engine_tag;
    using element_type     = typename engine_type::element_type;
    using value_type       = typename engine_type::value_type;
    using pointer          = typename engine_type::pointer;
    using const_pointer    = typename engine_type::const_pointer;
    using reference        = typename engine_type::reference;
    using const_reference  = typename engine_type::const_reference;
    using difference_type  = typename engine_type::difference_type;
    using index_type       = typename engine_type::index_type;
    using index_tuple_type = typename engine_type::index_tuple_type;
    using span_type        = detail::noe_mdspan_submatrix_t<engine_span_type>;
    using const_span_type  = detail::noe_mdspan_submatrix_t<engine_const_span_type>;

    //- Construct/copy/destroy
    //
    ~matrix_view_engine() noexcept = default;

    constexpr matrix_view_engine() = default;
    constexpr matrix_view_engine(matrix_view_engine&&) noexcept = default;
    constexpr matrix_view_engine(matrix_view_engine const&) noexcept = default;
    constexpr matrix_view_engine(engine_type& eng,
                                 index_type ri, index_type rn, index_type ci, index_type cn);
    constexpr matrix_view_engine(owning_engine_type& eng,
                                 index_type ri, index_type rn, index_type ci, index_type cn);

    constexpr matrix_view_engine&     operator =(matrix_view_engine&&) noexcept = default;
    constexpr matrix_view_engine&     operator =(matrix_view_engine const&) noexcept = default;

    template<class ET2>
    constexpr matrix_view_engine&     operator =(ET2 const& rhs);
    template<class U>
    constexpr matrix_view_engine&     operator =(initializer_list<initializer_list<U>> rhs);


    //- Status
    //
    constexpr bool              is_valid() const noexcept;

    //- Capacity
    //
    constexpr index_type        columns() const noexcept;
    constexpr index_type        rows() const noexcept;
    constexpr index_tuple_type  size() const noexcept;

    constexpr index_type        column_capacity() const noexcept;
    constexpr index_type        row_capacity() const noexcept;
    constexpr index_tuple_type  capacity() const noexcept;

    //- Element access
    //
    constexpr reference         operator ()(index_type i, index_type j) const;

    //- Data access
    //
    constexpr owning_engine_type&   owning_engine() const noexcept;

    template<class ET2 = LA_MVET2, detail::enable_if_spannable<LA_MVET2, ET2> = true>
    constexpr span_type         span() const noexcept;

    //- Modifiers
    //
    constexpr void              swap(matrix_view_engine& rhs) noexcept;

  private:
    engine_type     m_engine;
    index_type      m_row_start;
    index_type      m_row_count;
    index_type      m_col_start;
    index_type      m_col_count;
};

//------------------------
//- Construct/copy/destroy
//
template<class NVET, class MCT, class VFT> constexpr
matrix_view_engine<LA_MVET2, writable_matrix_engine_tag, subset_view_tag>::matrix_view_engine
(engine_type& eng, index_type ri, index_type rn, index_type ci, index_type cn)
:   m_engine(eng)
,   m_row_start(ri)
,   m_row_count(rn)
,   m_col_start(ci)
,   m_col_count(cn)
{}

template<class NVET, class MCT, class VFT> constexpr
matrix_view_engine<LA_MVET2, writable_matrix_engine_tag, subset_view_tag>::matrix_view_engine
(owning_engine_type& eng, index_type ri, index_type rn, index_type ci, index_type cn)
:   m_engine(eng)
,   m_row_start(ri)
,   m_row_count(rn)
,   m_col_start(ci)
,   m_col_count(cn)
{}

template<class NVET, class MCT, class VFT>
template<class ET2> constexpr auto
matrix_view_engine<LA_MVET2, writable_matrix_engine_tag, subset_view_tag>::operator =(ET2 const& rhs)
-> matrix_view_engine&
{
    detail::check_source_engine_size(rhs, rows(), columns());
    detail::matrix_assign_from(*this, rhs);
    return *this;
}

template<class NVET, class MCT, class VFT>
template<class U> constexpr auto
matrix_view_engine<LA_MVET2, writable_matrix_engine_tag, subset_view_tag>::operator =
(initializer_list<initializer_list<U>> rhs)
-> matrix_view_engine&
{
    detail::check_source_init_list(rhs, m_row_count, m_col_count);
    detail::matrix_assign_from(*this, rhs);
    return *this;
}

//----------
//- Status
//
template<class NVET, class MCT, class VFT> constexpr auto
matrix_view_engine<LA_MVET2, writable_matrix_engine_tag, subset_view_tag>::is_valid() const noexcept
-> bool
{
    return m_engine.is_valid();
}

//----------
//- Capacity
//
template<class NVET, class MCT, class VFT> constexpr auto
matrix_view_engine<LA_MVET2, writable_matrix_engine_tag, subset_view_tag>::columns() const noexcept
-> index_type
{
    return m_col_count;
}

template<class NVET, class MCT, class VFT> constexpr auto
matrix_view_engine<LA_MVET2, writable_matrix_engine_tag, subset_view_tag>::rows() const noexcept
-> index_type
{
    return m_row_count;
}

template<class NVET, class MCT, class VFT> constexpr auto
matrix_view_engine<LA_MVET2, writable_matrix_engine_tag, subset_view_tag>::size() const noexcept
-> index_tuple_type
{
    return index_tuple_type(m_row_count, m_col_count);
}

template<class NVET, class MCT, class VFT> constexpr auto
matrix_view_engine<LA_MVET2, writable_matrix_engine_tag, subset_view_tag>::column_capacity() const noexcept
-> index_type
{
    return m_col_count;
}

template<class NVET, class MCT, class VFT> constexpr auto
matrix_view_engine<LA_MVET2, writable_matrix_engine_tag, subset_view_tag>::row_capacity() const noexcept
-> index_type
{
    return m_row_count;
}

template<class NVET, class MCT, class VFT> constexpr auto
matrix_view_engine<LA_MVET2, writable_matrix_engine_tag, subset_view_tag>::capacity() const noexcept
-> index_tuple_type
{
    return index_tuple_type(m_row_count, m_col_count);
}

//----------------
//- Element access
//
template<class NVET, class MCT, class VFT> constexpr auto
matrix_view_engine<LA_MVET2, writable_matrix_engine_tag, subset_view_tag>::operator ()(index_type i, index_type j) const
-> reference
{
    return m_engine(i + m_row_start, j + m_col_start);
}

//-------------
//- Data access
//
template<class NVET, class MCT, class VFT> constexpr auto
matrix_view_engine<LA_MVET2, writable_matrix_engine_tag, subset_view_tag>::owning_engine() const noexcept
-> owning_engine_type&
{
    return m_engine.owning_engine();
}

template<class NVET, class MCT, class VFT>
template<class ET2, detail::enable_if_spannable<LA_MVET2, ET2>> constexpr auto
matrix_view_engine<LA_MVET2, writable_matrix_engine_tag, subset_view_tag>::span() const noexcept
-> span_type
{
    return detail::noe_mdspan_submatrix(m_engine.span(), m_row_start, m_row_count,
                                                         m_col_start, m_col_count);
}

//-----------
//- Modifiers
//
template<class NVET, class MCT, class VFT> constexpr void
matrix_view_engine<LA_MVET2, writable_matrix_engine_tag, subset_view_tag>::swap(matrix_view_engine& rhs) noexcept
{
    if (&rhs != this)
    {
        std::swap(m_engine,    rhs.m_engine);
        std::swap(m_row_start, rhs.m_row_start);
        std::swap(m_row_count, rhs.m_row_count);
        std::swap(m_col_start, rhs.m_col_start);
        std::swap(m_col_count, rhs.m_col_count);
    }
}

#undef LA_MVET2
#endif
}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_MATRIX_VIEW_ENGINE_HPP_DEFINED
