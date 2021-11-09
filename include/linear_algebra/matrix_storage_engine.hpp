//==================================================================================================
//  File:       matrix_storage_engine.hpp
//
//  Summary:    This header defines an owning, dense, rectangular engine that owns and manages
//              the elements of a matrix.
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_MATRIX_STORAGE_ENGINE_HPP_DEFINED
#define LINEAR_ALGEBRA_MATRIX_STORAGE_ENGINE_HPP_DEFINED

namespace STD_LA {
namespace detail {
//--------------------------------------------------------------------------------------------------
//  Class Template:     mse_data<T, X, A, L>
//
//  Thie type contains and manages elements on behalf of matrix_storage_engine<T,X,A,L>.
//
//  Partial specializations of this class template are tailored to specific corresponding partial
//  specializations of matrix_storage_engine.  They provide the special member functions that make
//  sense for each valid set of template arguments.
//
//  This implementation assumes that all dynamically-allocated memory is default-constructed,
//  with the consequence that elements lying in unused capacity are also constructed with value
//  equal to that of a value-initialized element (i.e., "0").
//
//  This assumption makes implementation easy for now, but may be absent in the final version.
//--------------------------------------------------------------------------------------------------
//
template<class T, class X, class A, class L>    struct mse_data;


//--------------------------------------------------------------------------------------------------
//  Partial Specialization:     mse_data<T, extents<R, C>, void, L>
//
//  Manages elements representing a fixed-size matrix of R rows and C columns.  Its elements are
//  implemented as member data in a std::array.
//--------------------------------------------------------------------------------------------------
//
template<class T, size_t R, size_t C, class L>
struct mse_data<T, extents<R, C>, void, L>
{
    using array_type = std::array<T, R*C>;

    static constexpr bool   is_column_matrix     = (C == 1);
    static constexpr bool   is_row_matrix        = (R == 1);
    static constexpr bool   is_1d_indexable      = (is_column_matrix || is_row_matrix);
    static constexpr bool   is_column_reshapable = false;
    static constexpr bool   is_row_reshapable    = false;
    static constexpr bool   is_2d_reshapable     = false;
    static constexpr bool   is_column_major      = is_same_v<L, matrix_layout::column_major>;
    static constexpr bool   is_row_major         = is_same_v<L, matrix_layout::row_major>;

    static constexpr size_t     m_rows   = R;
    static constexpr size_t     m_cols   = C;
    static constexpr size_t     m_rowcap = R;
    static constexpr size_t     m_colcap = C;

    array_type  m_elems;

    ~mse_data() = default;

    constexpr mse_data()
    :   m_elems()
    {
        if constexpr (!std::is_class_v<T>)
        {
            for (auto& elem : m_elems) elem = T{};
        }
    }
    constexpr mse_data(mse_data&&) noexcept = default;
    constexpr mse_data(mse_data const&) = default;
    constexpr mse_data&     operator =(mse_data&&) noexcept = default;
    constexpr mse_data&     operator =(mse_data const&) = default;
};


//--------------------------------------------------------------------------------------------------
//  Partial Specialization:     mse_data<T, extents<R, C>, A, L>
//
//  Manages elements representing a fixed-size matrix of R rows and C columns.  Its elements are
//  implemented as member data in a std::vector.
//--------------------------------------------------------------------------------------------------
//
template<class T, size_t R, size_t C, class A, class L>
struct mse_data<T, extents<R, C>, A, L>
{
    using array_type = std::vector<T, A>;

    static constexpr bool   is_column_matrix     = (C == 1);
    static constexpr bool   is_row_matrix        = (R == 1);
    static constexpr bool   is_1d_indexable      = (is_column_matrix || is_row_matrix);
    static constexpr bool   is_column_reshapable = false;
    static constexpr bool   is_row_reshapable    = false;
    static constexpr bool   is_2d_reshapable     = false;
    static constexpr bool   is_column_major      = is_same_v<L, matrix_layout::column_major>;
    static constexpr bool   is_row_major         = is_same_v<L, matrix_layout::row_major>;

    static constexpr size_t     m_rows   = R;
    static constexpr size_t     m_cols   = C;
    static constexpr size_t     m_rowcap = R;
    static constexpr size_t     m_colcap = C;

    array_type  m_elems;

    //- Construct/copy/destroy.
    //
    ~mse_data() = default;

    constexpr mse_data()
    :   m_elems(static_cast<typename array_type::size_type>(R*C))
    {}
    constexpr mse_data(mse_data&&) noexcept = default;
    constexpr mse_data(mse_data const&) = default;
    constexpr mse_data&     operator =(mse_data&&) noexcept = default;
    constexpr mse_data&     operator =(mse_data const&) = default;
};


//--------------------------------------------------------------------------------------------------
//  Partial Specialization:     mse_data<T, extents<R, dynamic_extent>, A, L>
//
//  Manages elements representing a matrix having a fixed number of R rows and a dynamically-
//  resizable number of columns.  Its elements are implemented as member data in a std::vector.
//--------------------------------------------------------------------------------------------------
//
template<class T, size_t R, class A, class L>
struct mse_data<T, extents<R, dynamic_extent>, A, L>
{
    using array_type = std::vector<T, A>;

    static constexpr bool   is_column_matrix     = false;
    static constexpr bool   is_row_matrix        = (R == 1);
    static constexpr bool   is_1d_indexable      = is_row_matrix;
    static constexpr bool   is_column_reshapable = true;
    static constexpr bool   is_row_reshapable    = false;
    static constexpr bool   is_2d_reshapable     = false;
    static constexpr bool   is_column_major      = is_same_v<L, matrix_layout::column_major>;
    static constexpr bool   is_row_major         = is_same_v<L, matrix_layout::row_major>;

    static constexpr size_t     m_rows   = R;
    static constexpr size_t     m_rowcap = R;

    array_type  m_elems;
    size_t      m_cols;
    size_t      m_colcap;

    //- Construct/copy/destroy.
    //
    ~mse_data() = default;

    constexpr
    mse_data()
    :   m_elems(), m_cols(0), m_colcap(0)
    {}
    constexpr mse_data(mse_data&&) noexcept = default;
    constexpr mse_data(mse_data const&) = default;
    constexpr mse_data&     operator =(mse_data&&) noexcept = default;
    constexpr mse_data&     operator =(mse_data const&) = default;
};


//--------------------------------------------------------------------------------------------------
//  Partial Specialization:     mse_data<T, extents<dynamic_extent, C>, A, L>
//
//  Manages elements representing a matrix having a dynamically-resizable number of rows and a
//  fixed number of C columns.  Its elements are implemented as member data in a std::vector.
//--------------------------------------------------------------------------------------------------
//
template<class T, size_t C, class A, class L>
struct mse_data<T, extents<dynamic_extent, C>, A, L>
{
    using array_type = std::vector<T, A>;

    static constexpr bool   is_column_matrix     = (C == 1);
    static constexpr bool   is_row_matrix        = false;
    static constexpr bool   is_1d_indexable      = is_column_matrix;
    static constexpr bool   is_column_reshapable = false;
    static constexpr bool   is_row_reshapable    = true;
    static constexpr bool   is_2d_reshapable     = false;
    static constexpr bool   is_column_major      = is_same_v<L, matrix_layout::column_major>;
    static constexpr bool   is_row_major         = is_same_v<L, matrix_layout::row_major>;

    static constexpr size_t     m_cols   = C;
    static constexpr size_t     m_colcap = C;

    array_type  m_elems;
    size_t      m_rows;
    size_t      m_rowcap;

    //- Construct/copy/destroy.
    //
    ~mse_data() = default;

    constexpr
    mse_data()
    :   m_elems(), m_rows(0), m_rowcap(0)
    {}
    constexpr mse_data(mse_data&&) noexcept = default;
    constexpr mse_data(mse_data const&) = default;
    constexpr mse_data&     operator =(mse_data&&) noexcept = default;
    constexpr mse_data&     operator =(mse_data const&) = default;
};


//--------------------------------------------------------------------------------------------------
//  Partial Specialization:     mse_data<T, extents<dynamic_extent, dynamic_extent>, A, L>
//
//  Manages elements representing a matrix having a dynamically-resizable number of rows and a
//  dynamically-resizable number of columns.  Its elements are implemented as member data in a
//  std::vector.
//--------------------------------------------------------------------------------------------------
//
template<class T, class A, class L>
struct mse_data<T, extents<dynamic_extent, dynamic_extent>, A, L>
{
    using array_type = std::vector<T, A>;

    static constexpr bool   is_column_matrix     = false;
    static constexpr bool   is_row_matrix        = false;
    static constexpr bool   is_1d_indexable      = false;
    static constexpr bool   is_column_reshapable = false;
    static constexpr bool   is_row_reshapable    = false;
    static constexpr bool   is_2d_reshapable     = true;
    static constexpr bool   is_column_major      = is_same_v<L, matrix_layout::column_major>;
    static constexpr bool   is_row_major         = is_same_v<L, matrix_layout::row_major>;

    array_type  m_elems;
    size_t      m_rows;
    size_t      m_cols;
    size_t      m_rowcap;
    size_t      m_colcap;

    //- Construct/copy/destroy.
    //
    ~mse_data() = default;

    constexpr
        mse_data()
    :   m_elems(), m_rows(0), m_cols(0), m_rowcap(0), m_colcap(0)
    {}
    constexpr mse_data(mse_data&&) noexcept = default;
    constexpr mse_data(mse_data const&) = default;
    constexpr mse_data&     operator =(mse_data&&) noexcept = default;
    constexpr mse_data&     operator =(mse_data const&) = default;
};


//--------------------------------------------------------------------------------------------------
//  Traits Type:    mse_mdspan_support
//
//  Traits types (possibly temporary) that creates mdspan objects on behalf of traits type
//  mse_mdspan_support<MSED> (defined below).
//--------------------------------------------------------------------------------------------------
//
struct mse_mdspan_support
{
    using dyn_mat_extents = extents<dynamic_extent, dynamic_extent>;
    using dyn_mat_strides = array<typename dyn_mat_extents::size_type, 2>;
    using dyn_mat_layout  = layout_stride;
    using dyn_mat_mapping = typename dyn_mat_layout::mapping<dyn_mat_extents>;

    template<class ST, class MSE>
    static constexpr ST
    make_static_mdspan(MSE& mse)
    {
        return ST(mse.m_elems.data());
    }

    template<class ST, class MSE>
    static constexpr ST
    make_dynamic_mdspan(MSE& mse)
    {
        if constexpr (MSE::is_row_major)
        {
            dyn_mat_extents     extents(mse.m_rows, mse.m_cols);
            dyn_mat_strides     strides{mse.m_colcap, 1};
            dyn_mat_mapping     mapping(extents, strides);

            return ST(mse.m_elems.data(), mapping);
        }
        else
        {
            dyn_mat_extents     extents(mse.m_rows, mse.m_cols);
            dyn_mat_strides     strides{1, mse.m_rowcap};
            dyn_mat_mapping     mapping(extents, strides);

            return ST(mse.m_elems.data(), mapping);
        }
    }
};


//--------------------------------------------------------------------------------------------------
//  Traits Type:    mse_mdspan_traits<T, X, L>
//
//  Traits type that determines the types, and computes the values, of mdspan objects
//  on behalf of matrix_storage_engine<T,X,A,L>.
//
//  Partial specializations of this class template are tailored to specific sets of partial
//  specializations of its template parameter.  They provide static member functions to
//  create mdspan objects.
//
//  Note that only the fixed-size partial specialization creates a non-dynamic mdspan object;
//  if either the rows orcolumns size parameter is dynamic, then the corresponding mdspan type
//  is dynamic in both dimensions.
//
//  TODO: This may change later -- it should be possible to create two-dimensional mdspan objects
//  with one fixed and one dynamic dimension.
//--------------------------------------------------------------------------------------------------
//
template<class MSED>    struct mse_mdspan_traits;

//------ Partial specialization for extents<R, C>.
//
template<class T, size_t R, size_t C, class A, class L>
struct mse_mdspan_traits<mse_data<T, extents<R, C>, A, L>>
:   public mse_mdspan_support
{
    using mse_data_type   = mse_data<T, extents<R, C>, A, L>;
    using layout_type     = get_mdspan_layout_t<L>;
    using span_type       = mdspan<T, extents<R, C>, layout_type>;
    using const_span_type = mdspan<T const, extents<R, C>, layout_type>;

    static constexpr span_type
    make_mdspan(mse_data_type& mse)
    {
        return make_static_mdspan<span_type, mse_data_type>(mse);
    }

    static constexpr const_span_type
    make_const_mdspan(mse_data_type const& mse)
    {
        return make_static_mdspan<const_span_type, mse_data_type const>(mse);
    }
};


//------ Partial specialization for extents<R, dynamic_extent>.
//
template<class T, size_t R, class A, class L>
struct mse_mdspan_traits<mse_data<T, extents<R, dynamic_extent>, A, L>>
:   public mse_mdspan_support
{
    using mse_data_type   = mse_data<T, extents<R, dynamic_extent>, A, L>;
    using span_type       = mdspan<T, dyn_mat_extents, dyn_mat_layout>;
    using const_span_type = mdspan<T const, dyn_mat_extents, dyn_mat_layout>;

    static constexpr span_type
    make_mdspan(mse_data_type& mse)
    {
        return make_dynamic_mdspan<span_type, mse_data_type>(mse);
    }

    static constexpr const_span_type
    make_const_mdspan(mse_data_type const& mse)
    {
        return make_dynamic_mdspan<const_span_type, mse_data_type const>(mse);
    }
};


//------ Partial specialization for extents<dynamic_extent, C>.
//
template<class T, size_t C, class A, class L>
struct mse_mdspan_traits<mse_data<T, extents<dynamic_extent, C>, A, L>>
:   public mse_mdspan_support
{
    using mse_data_type   = mse_data<T, extents<dynamic_extent, C>, A, L>;
    using span_type       = mdspan<T, dyn_mat_extents, dyn_mat_layout>;
    using const_span_type = mdspan<T const, dyn_mat_extents, dyn_mat_layout>;

    static constexpr span_type
    make_mdspan(mse_data_type& mse)
    {
        return make_dynamic_mdspan<span_type, mse_data_type>(mse);
    }

    static constexpr const_span_type
    make_const_mdspan(mse_data_type const& mse)
    {
        return make_dynamic_mdspan<const_span_type, mse_data_type const>(mse);
    }
};


//------ Partial specialization for extents<dynamic_extent, dynamic_extent>.
//
template<class T, class A, class L>
struct mse_mdspan_traits<mse_data<T, extents<dynamic_extent, dynamic_extent>, A, L>>
:   public mse_mdspan_support
{
    using mse_data_type   = mse_data<T, extents<dynamic_extent, dynamic_extent>, A, L>;
    using span_type       = mdspan<T, dyn_mat_extents, dyn_mat_layout>;
    using const_span_type = mdspan<T const, dyn_mat_extents, dyn_mat_layout>;

    static constexpr span_type
    make_mdspan(mse_data_type& mse)
    {
        return make_dynamic_mdspan<span_type, mse_data_type>(mse);
    }

    static constexpr const_span_type
    make_const_mdspan(mse_data_type const& mse)
    {
        return make_dynamic_mdspan<const_span_type, mse_data_type const>(mse);
    }
};


}       //- detail namespace
//==================================================================================================
//==================================================================================================
//  Class Template:     matrix_storage_engine<T, X, A, L>
//
//  This class template implements an owning engine for use by the math object class templates
//  matrix<ET, OT> and basic_vector<ET, OT>.  Specifically, it provides storage suitable
//  for modeling a mathematical matrix or vector, having dimensions specified by X, employing
//  allocator A, and having element layout L.
//--------------------------------------------------------------------------------------------------
//
template<class T, class X, class A, class L>
class matrix_storage_engine
{
    static_assert(detail::valid_engine_extents<X>, "invalid extents parameter");
    static_assert(detail::valid_allocator_arg<T, A>, "invalid allocator parameter");
    static_assert(detail::valid_engine_extents_and_allocator<T, X, A>,
                  "invalid combination of element type, extents, and allocator");
};


//--------------------------------------------------------------------------------------------------
//  Partial Specialization:     matrix_storage_engine<T, extents<R, C>, A, L>
//
//  This partial specialization of matrix_storage_engine<T,X,A,L> implements an owning engine
//  for use by class template matrix<ET, OT>.  Specifically, it models a mathematical
//  matrix with R rows and C columns, employing allocator A, and having element layout L.
//
//  Sizes R and C may be positive integers, or have the value "dynamic_extent" (defined in the
//  <mdspan> header).  Allocator type A may be void, which indicates internal non-heap element
//  storage, or it may be an allocator type that fulfills all the requirements imposed by
//  std::allocator_traits.  Layout type L must be "row_major" or "column_major".
//
//  This specialization assumes that all dynamically-allocated memory is default-constructed,
//  with the consequence that elements lying in unused capacity are also constructed.  This
//  assumption makes implementation easy, but may be absent in the final version.
//--------------------------------------------------------------------------------------------------
//
template<class T, size_t R, size_t C, class A, class L>
requires
    detail::valid_engine_extents_and_allocator<T, extents<R, C>, A>
    and
    detail::valid_layout_for_2d_storage_engine<L>
class matrix_storage_engine<T, extents<R, C>, A, L>
{
    using this_type      = matrix_storage_engine;
    using support_traits = detail::matrix_engine_support;
    using storage_type   = detail::mse_data<T, extents<R, C>, A, L>;
    using mdspan_traits  = detail::mse_mdspan_traits<storage_type>;

    static constexpr bool   has_column_major_layout = storage_type::is_column_major;
    static constexpr bool   has_row_major_layout    = storage_type::is_row_major;
    static constexpr bool   is_1d_indexable         = storage_type::is_1d_indexable;
    static constexpr bool   is_2d_reshapable        = storage_type::is_2d_reshapable;
    static constexpr bool   is_column_reshapable    = storage_type::is_column_reshapable;
    static constexpr bool   is_row_reshapable       = storage_type::is_row_reshapable;

  private:
    storage_type    m_data;

  public:
    using allocator_type  = A;
    using element_type    = T;
    using reference       = element_type&;
    using const_reference = element_type const&;
    using size_type       = size_t;
    using span_type       = typename mdspan_traits::span_type;
    using const_span_type = typename mdspan_traits::const_span_type;

  public:
    ~matrix_storage_engine() = default;

    //- Construct / assign.
    //
    constexpr matrix_storage_engine() = default;
    constexpr matrix_storage_engine(matrix_storage_engine&&) noexcept = default;
    constexpr matrix_storage_engine(matrix_storage_engine const&) = default;

    constexpr matrix_storage_engine&    operator =(matrix_storage_engine&&) noexcept = default;
    constexpr matrix_storage_engine&    operator =(matrix_storage_engine const&) = default;

    //- Rehsaping constructors.
    //
    constexpr
    matrix_storage_engine(size_type rows, size_type cols)
    requires
        this_type::is_2d_reshapable
    :   m_data()
    {
        do_reshape(rows, cols, rows, cols);
    }

    constexpr
    matrix_storage_engine(size_type rows, size_type cols, size_type rowcap, size_type colcap)
    requires
        this_type::is_2d_reshapable
    :   m_data()
    {
        do_reshape(rows, cols, rowcap, colcap);
    }

    constexpr
    matrix_storage_engine(size_type cols)
    requires
        this_type::is_column_reshapable
    :   m_data()
    {
        do_reshape_columns(cols, cols);
    }

    constexpr
    matrix_storage_engine(size_type cols, size_type colcap)
    requires
        this_type::is_column_reshapable
    :   m_data()
    {
        do_reshape_columns(cols, colcap);
    }

    constexpr
    matrix_storage_engine(size_type rows)
    requires
        this_type::is_row_reshapable
    :   m_data()
    {
        do_reshape_rows(rows, rows);
    }

    constexpr
    matrix_storage_engine(size_type rows, size_type rowcap)
    requires
        this_type::is_row_reshapable
    :   m_data()
    {
        do_reshape_rows(rows, rowcap);
    }

    //- Heterogeneous construction from two-dimensional sources.
    //
    template<class ET2>
    constexpr
    matrix_storage_engine(ET2 const& rhs)
    requires
        detail::readable_matrix_engine<ET2>
        and
        detail::convertible_from<element_type, typename ET2::element_type>
    :   m_data()
    {
        support_traits::assign_from(*this, rhs);
    }

    template<class U, size_t X0, size_t X1, class SL, class SA>
    constexpr
    matrix_storage_engine(mdspan<U, extents<X0, X1>, SL, SA> const& rhs)
    requires
        detail::convertible_from<element_type, U>
    :   m_data()
    {
        support_traits::assign_from(*this, rhs);
    }

    template<class U>
    constexpr
    matrix_storage_engine(initializer_list<initializer_list<U>> rhs)
    requires
        detail::convertible_from<T, U>
    :   m_data()
    {
        support_traits::assign_from(*this, rhs);
    }

    //- Heterogeneous construction from one-dimensional sources.
    //
    template<class CT>
    constexpr
    matrix_storage_engine(CT const& rhs)
    requires
        this_type::is_1d_indexable
        and
        detail::random_access_standard_container<CT>
        and
        detail::convertible_from<element_type, typename CT::value_type>
    :   m_data()
    {
        support_traits::assign_from(*this, rhs);
    }

    template<class U, size_t X0, class SL, class SA>
    constexpr
    matrix_storage_engine(mdspan<U, extents<X0>, SL, SA> const& rhs)
    requires
        this_type::is_1d_indexable
        and
        detail::convertible_from<element_type, U>
    :   m_data()
    {
        support_traits::assign_from(*this, rhs);
    }

    template<class U>
    constexpr
    matrix_storage_engine(initializer_list<U> rhs)
    requires
        this_type::is_1d_indexable
        and
        detail::convertible_from<element_type, U>
    :   m_data()
    {
        support_traits::assign_from(*this, rhs);
    }

    //- Heterogeneous assignment from two-dimensional sources.
    //
    template<class ET2>
    constexpr matrix_storage_engine&
    operator =(ET2 const& rhs)
    requires
        detail::readable_matrix_engine<ET2>
        and
        detail::convertible_from<element_type, typename ET2::element_type>
    {
        support_traits::assign_from(*this, rhs);
        return *this;
    }

    template<class U, size_t X0, size_t X1, class SL, class SA>
    constexpr matrix_storage_engine&
    operator =(mdspan<U, extents<X0, X1>, SL, SA> const& rhs)
    requires
        detail::convertible_from<element_type, U>
    {
        support_traits::assign_from(*this, rhs);
        return *this;
    }

    template<class U>
    constexpr matrix_storage_engine&
    operator =(initializer_list<initializer_list<U>> rhs)
    requires
        detail::convertible_from<element_type, U>
    {
        support_traits::assign_from(*this, rhs);
        return *this;
    }

    //- Heterogeneous assignment from one-dimensional sources.
    //
    template<class CT>
    constexpr matrix_storage_engine&
    operator =(CT const& rhs)
    requires
        this_type::is_1d_indexable
        and
        detail::random_access_standard_container<CT>
        and
        detail::convertible_from<element_type, typename CT::value_type>
    {
        support_traits::assign_from(*this, rhs);
        return *this;
    }

    template<class U, size_t X0, class SL, class SA>
    constexpr matrix_storage_engine&
    operator =(mdspan<U, extents<X0>, SL, SA> const& rhs)
    requires
        this_type::is_1d_indexable
        and
        detail::convertible_from<element_type, U>
    {
        support_traits::assign_from(*this, rhs);
        return *this;
    }

    template<class U>
    constexpr matrix_storage_engine&
    operator =(initializer_list<U> rhs)
    requires
        this_type::is_1d_indexable
        and
        detail::convertible_from<element_type, U>
    {
        support_traits::assign_from(*this, rhs);
        return *this;
    }

    //- Size and capacity reporting.
    //
    constexpr size_type
    columns() const noexcept
    {
        return m_data.m_cols;
    }

    constexpr size_type
    rows() const noexcept
    {
        return m_data.m_rows;
    }

    constexpr size_type
    size() const noexcept
    {
        return m_data.m_rows * m_data.m_cols;
    }

    constexpr size_type
    column_capacity() const noexcept
    {
        return m_data.m_colcap;
    }

    constexpr size_type
    row_capacity() const noexcept
    {
        return m_data.m_rowcap;
    }

    constexpr size_type
    capacity() const noexcept
    {
        return m_data.m_rowcap * m_data.m_colcap;
    }

    //- Element access
    //
    constexpr reference
    operator ()(size_type i)
    requires
        this_type::is_1d_indexable
    {
        return m_data.m_elems[i];
    }

    constexpr const_reference
    operator ()(size_type i) const
    requires
        this_type::is_1d_indexable
    {
        return m_data.m_elems[i];
    }

    constexpr reference
    operator ()(size_type i, size_type j)
    requires
        this_type::has_row_major_layout
    {
        return m_data.m_elems[(i * m_data.m_colcap) + j];
    }

    constexpr reference
    operator ()(size_type i, size_type j)
    requires
        this_type::has_column_major_layout
    {
         return m_data.m_elems[i + (j * m_data.m_rowcap)];
    }

    constexpr const_reference
    operator ()(size_type i, size_type j) const
    requires
        this_type::has_row_major_layout
    {
        return m_data.m_elems[(i * m_data.m_colcap) + j];
    }

    constexpr const_reference
    operator ()(size_type i, size_type j) const
    requires
        this_type::has_column_major_layout
    {
         return m_data.m_elems[i + (j * m_data.m_rowcap)];
    }

    constexpr span_type
    span() noexcept
    {
        return mdspan_traits::make_mdspan(m_data);
    }

    constexpr const_span_type
    span() const noexcept
    {
        return mdspan_traits::make_const_mdspan(m_data);
    }

    //- Setting overall size and capacity.
    //
    constexpr void
    reshape(size_type rows, size_type cols, size_type rowcap, size_type colcap)
    requires
        this_type::is_2d_reshapable
    {
        do_reshape(rows, cols, rowcap, colcap);
    }

    //- Setting column size and capacity.
    //
    constexpr void
    reshape_columns(size_type cols, size_type colcap)
    requires
        this_type::is_column_reshapable
    {
        do_reshape_columns(cols, colcap);
    }

    constexpr void
    reshape_columns(size_type cols, size_type colcap)
    requires
        this_type::is_2d_reshapable
    {
        do_reshape(m_data.m_rows, cols, m_data.m_rowcap, colcap);
    }

    //- Setting row size and capacity.
    //
    constexpr void
    reshape_rows(size_type rows, size_type rowcap)
    requires
        this_type::is_row_reshapable
    {
        do_reshape_rows(rows, rowcap);
    }

    constexpr void
    reshape_rows(size_type rows, size_type rowcap)
    requires
        this_type::is_2d_reshapable
    {
        do_reshape(rows, m_data.m_cols, rowcap, m_data.m_colcap);
    }

    //- Other modifiers.
    //
    constexpr void
    swap(matrix_storage_engine& rhs) noexcept
    {
        support_traits::swap(m_data, rhs.m_data);
    }

  private:
    constexpr void
    do_reshape(size_type rows, size_type cols, size_type rowcap, size_type colcap)
    requires
        this_type::is_2d_reshapable
    {
        support_traits::verify_size(rows);
        support_traits::verify_size(cols);
        support_traits::verify_capacity(rowcap);
        support_traits::verify_capacity(colcap);

        //- Only allocate new storage if we need to.
        //
        if (rows > m_data.m_rowcap  ||  rowcap != m_data.m_rowcap  ||
            cols > m_data.m_colcap  ||  colcap != m_data.m_colcap)
        {
            //- Normalize requested new capacities.
            //
            rowcap = std::max(rows, rowcap);
            colcap = std::max(cols, colcap);

            //- Prepare a temporary object to receive elements from this one.
            //
            this_type   tmp;
            tmp.m_data.m_elems.resize(rowcap * colcap);
            tmp.m_data.m_rows   = rows;
            tmp.m_data.m_cols   = cols;
            tmp.m_data.m_rowcap = rowcap;
            tmp.m_data.m_colcap = colcap;

            //- Move the appropriate subset of elements into the temporary engine and swap.
            //
            size_type   dst_rows = std::min(rows, m_data.m_rows);
            size_type   dst_cols = std::min(cols, m_data.m_cols);
            support_traits::move_elements(tmp, *this, dst_rows, dst_cols);
            support_traits::swap(m_data, tmp.m_data);
        }
        else
        {
            if (rows < m_data.m_rows)
            {
                support_traits::fill_rows(*this, rows, m_data.m_rows, T{});
                m_data.m_rows = rows;
            }
            if (cols < m_data.m_cols)
            {
                support_traits::fill_columns(*this, cols, m_data.m_cols, T{});
                m_data.m_cols = cols;
            }
        }
    }

    constexpr void
    do_reshape_columns(size_type cols, size_type colcap)
    requires
        this_type::is_column_reshapable
    {
        support_traits::verify_size(cols);
        support_traits::verify_capacity(colcap);

        //- Only allocate new storage if we need to.
        //
        if (cols > m_data.m_colcap  ||  colcap != m_data.m_colcap)
        {
            //- Normalize requested new capacity.
            //
            colcap = std::max(cols, colcap);

            //- Prepare a temporary object to receive elements from this one.
            //
            this_type   tmp;
            tmp.m_data.m_elems.resize(m_data.m_rowcap * colcap);
            tmp.m_data.m_cols   = cols;
            tmp.m_data.m_colcap = colcap;

            //- Move the appropriate subset of elements into the temporary engine and swap.
            //
            size_type   dst_cols = std::min(cols, m_data.m_cols);
            support_traits::move_elements(tmp, *this, m_data.m_rows, dst_cols);
            support_traits::swap(*this, tmp);
        }
        else
        {
            if (cols < m_data.m_cols)
            {
                support_traits::fill_columns(*this, cols, m_data.m_cols, T{});
                m_data.m_cols = cols;
            }
        }
    }

    constexpr void
    do_reshape_rows(size_type rows, size_type rowcap)
    requires
        this_type::is_row_reshapable
    {
        support_traits::verify_size(rows);
        support_traits::verify_capacity(rowcap);

        //- Only allocate new storage if we need to.
        //
        if (rows > m_data.m_rowcap  ||  rowcap != m_data.m_rowcap)
        {
            //- Normalize requested new capacity.
            //
            rowcap = std::max(rows, rowcap);

            //- Prepare a temporary object to receive elements from this one.
            //
            this_type   tmp;
            tmp.m_data.m_elems.resize(rowcap * m_data.m_colcap);
            tmp.m_data.m_rows   = rows;
            tmp.m_data.m_rowcap = rowcap;

            //- Move the appropriate subset of elements into the temporary engine and swap.
            //
            size_type   dst_rows = std::min(rows, m_data.m_rows);
            support_traits::move_elements(tmp, *this, dst_rows, m_data.m_cols);
            support_traits::swap(*this, tmp);
        }
        else
        {
            if (rows < m_data.m_rows)
            {
                support_traits::fill_rows(*this, rows, m_data.m_rows, T{});
                m_data.m_rows = rows;
            }
        }
    }
};

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_MATRIX_STORAGE_ENGINE_HPP_DEFINED
