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
//  Type that contains and manages elements on behalf of matrix_storage_engine<T,X,A,L>.
//
//  Partial specializations of this class template are tailored to specific corresponding partial
//  specializations of matrix_storage_engine.  They provide the special member functions that make
//  sense for each valid set of template arguments.
//
//  This implementation assumes that all dynamically-allocated memory is default-constructed,
//  with the consequence that elements lying in unused capacity are also constructed with value
//  equal to that of a value-initialized element (i.e., "0").
//
//  This assumption makes implementation easy, but may be absent in the final version.
//--------------------------------------------------------------------------------------------------
//
template<class T, class X, class A, class L>    struct mse_data;


//--------------------------------------------------------------------------------------------------
//  Specialization:     mse_data<T, extents<N>, void, L>
//
//  Manages elements representing a fixed-size unoriented vector of N elements.  Its elements are
//  implemented as member data in a std::array.
//--------------------------------------------------------------------------------------------------
//
template<class T, ptrdiff_t N, class L>
struct mse_data<T, extents<N>, void, L>
{
    using array_type = std::array<T, N>;

    static constexpr bool   is_reshapable = false;

    static constexpr ptrdiff_t  m_size = N;
    static constexpr ptrdiff_t  m_cap  = N;

    array_type  m_elems;

    ~mse_data() = default;

    constexpr mse_data()
    :   m_elems()
    {
        if constexpr (!is_class_v<T>)
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
//  Specialization:     mse_data<T, extents<N>, A, L>
//
//  Manages elements representing a fixed-size unoriented vector of N elements.  Its elements are
//  implemented as member data in a std::vector.
//--------------------------------------------------------------------------------------------------
//
template<class T, ptrdiff_t N, class A, class L>
struct mse_data<T, extents<N>, A, L>
{
    using array_type = std::vector<T, A>;

    static constexpr bool   is_reshapable = false;

    static constexpr ptrdiff_t  m_size = N;
    static constexpr ptrdiff_t  m_cap  = N;

    array_type  m_elems;

    ~mse_data() = default;

    mse_data()
    :   m_elems(N)
    {}
    mse_data(mse_data&&) noexcept = default;
    mse_data(mse_data const&) = default;
    mse_data&   operator =(mse_data&&) noexcept = default;
    mse_data&   operator =(mse_data const&) = default;
};


//--------------------------------------------------------------------------------------------------
//  Partial Specialization:     mse_data<T, extents<dynamic_extent>, A, L>
//
//  Manages elements representing a dynamically-resizable unoriented vector.  Its elements are
//  implemented as member data in a std::vector.
//--------------------------------------------------------------------------------------------------
//
template<class T, class A, class L>
struct mse_data<T, extents<dynamic_extent>, A, L>
{
    using array_type = std::vector<T, A>;

    static constexpr bool   is_reshapable = true;

    array_type  m_elems;
    ptrdiff_t   m_size;
    ptrdiff_t   m_cap;

    ~mse_data() = default;

    constexpr mse_data()
    :   m_elems(), m_size(0), m_cap(0)
    {}
    constexpr mse_data(mse_data&&) noexcept = default;
    constexpr mse_data(mse_data const&) = default;
    constexpr mse_data&     operator =(mse_data&&) noexcept = default;
    constexpr mse_data&     operator =(mse_data const&) = default;
};


//--------------------------------------------------------------------------------------------------
//  Partial Specialization:     mse_data<T, extents<R, C>, void, L>
//
//  Manages elements representing a fixed-size matrix of R rows and C columns.  Its elements are
//  implemented as member data in a std::array.
//--------------------------------------------------------------------------------------------------
//
template<class T, ptrdiff_t R, ptrdiff_t C, class L>
struct mse_data<T, extents<R, C>, void, L>
{
    using array_type = std::array<T, R*C>;

    static constexpr bool   is_column_matrix     = (C == 1);
    static constexpr bool   is_row_matrix        = (R == 1);
    static constexpr bool   is_1d_indexable      = (is_column_matrix || is_row_matrix);
    static constexpr bool   is_column_reshapable = false;
    static constexpr bool   is_row_reshapable    = false;
    static constexpr bool   is_2d_reshapable     = false;
    static constexpr bool   is_column_major      = is_same_v<L, column_major>;
    static constexpr bool   is_row_major         = is_same_v<L, row_major>;

    static constexpr ptrdiff_t  m_rows   = R;
    static constexpr ptrdiff_t  m_cols   = C;
    static constexpr ptrdiff_t  m_rowcap = R;
    static constexpr ptrdiff_t  m_colcap = C;

    array_type  m_elems;

    ~mse_data() = default;

    constexpr mse_data()
    :   m_elems()
    {
        if constexpr (!is_class_v<T>)
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
template<class T, ptrdiff_t R, ptrdiff_t C, class A, class L>
struct mse_data<T, extents<R, C>, A, L>
{
    using array_type = std::vector<T, A>;

    static constexpr bool   is_column_matrix     = (C == 1);
    static constexpr bool   is_row_matrix        = (R == 1);
    static constexpr bool   is_1d_indexable      = (is_column_matrix || is_row_matrix);
    static constexpr bool   is_column_reshapable = false;
    static constexpr bool   is_row_reshapable    = false;
    static constexpr bool   is_2d_reshapable     = false;
    static constexpr bool   is_column_major      = is_same_v<L, column_major>;
    static constexpr bool   is_row_major         = is_same_v<L, row_major>;

    static constexpr ptrdiff_t  m_rows   = R;
    static constexpr ptrdiff_t  m_cols   = C;
    static constexpr ptrdiff_t  m_rowcap = R;
    static constexpr ptrdiff_t  m_colcap = C;

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
template<class T, ptrdiff_t R, class A, class L>
struct mse_data<T, extents<R, dynamic_extent>, A, L>
{
    using array_type = std::vector<T, A>;

    static constexpr bool   is_column_matrix     = false;
    static constexpr bool   is_row_matrix        = (R == 1);
    static constexpr bool   is_1d_indexable      = is_row_matrix;
    static constexpr bool   is_column_reshapable = true;
    static constexpr bool   is_row_reshapable    = false;
    static constexpr bool   is_2d_reshapable     = false;
    static constexpr bool   is_column_major      = is_same_v<L, column_major>;
    static constexpr bool   is_row_major         = is_same_v<L, row_major>;

    static constexpr ptrdiff_t  m_rows   = R;
    static constexpr ptrdiff_t  m_rowcap = R;

    array_type  m_elems;
    ptrdiff_t   m_cols;
    ptrdiff_t   m_colcap;

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
template<class T, ptrdiff_t C, class A, class L>
struct mse_data<T, extents<dynamic_extent, C>, A, L>
{
    using array_type = std::vector<T, A>;

    static constexpr bool   is_column_matrix     = (C == 1);
    static constexpr bool   is_row_matrix        = false;
    static constexpr bool   is_1d_indexable      = is_column_matrix;
    static constexpr bool   is_column_reshapable = false;
    static constexpr bool   is_row_reshapable    = true;
    static constexpr bool   is_2d_reshapable     = false;
    static constexpr bool   is_column_major      = is_same_v<L, column_major>;
    static constexpr bool   is_row_major         = is_same_v<L, row_major>;

    static constexpr ptrdiff_t  m_cols   = C;
    static constexpr ptrdiff_t  m_colcap = C;

    array_type  m_elems;
    ptrdiff_t   m_rows;
    ptrdiff_t   m_rowcap;

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
    static constexpr bool   is_column_major      = is_same_v<L, column_major>;
    static constexpr bool   is_row_major         = is_same_v<L, row_major>;

    array_type  m_elems;
    ptrdiff_t   m_rows;
    ptrdiff_t   m_cols;
    ptrdiff_t   m_rowcap;
    ptrdiff_t   m_colcap;

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
//  Traits Type:    mse_1d_mdspan_support
//                  mse_2d_mdspan_support
//
//  Traits types (possibly temporary) that creates basic_mdspan objects on behalf of traits type
//  mse_2d_mdspan_support<MSED> (defined below).
//--------------------------------------------------------------------------------------------------
//
struct mse_1d_mdspan_support
{
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
        return ST(mse.m_elems.data(), mse.m_size);
    }
};

//------
//
struct mse_2d_mdspan_support
{
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
//  Traits type that determines the types, and computes the values, of basic_mdspan objects
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


//------ Partial specialization for extents<N>.
//
template<class T, ptrdiff_t N, class A, class L>
struct mse_mdspan_traits<mse_data<T, extents<N>, A, L>>
:   mse_1d_mdspan_support
{
    using mse_data_type   = mse_data<T, extents<N>, A, L>;
    using span_type       = mdspan<T, N>;
    using const_span_type = mdspan<T const, N>;

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


//------ Partial specialization for extents<dynamic_extent>.
//
template<class T, class A, class L>
struct mse_mdspan_traits<mse_data<T, extents<dynamic_extent>, A, L>>
:   mse_1d_mdspan_support
{
    using mse_data_type   = mse_data<T, extents<dynamic_extent>, A, L>;
    using span_type       = mdspan<T, dynamic_extent>;
    using const_span_type = mdspan<T const, dynamic_extent>;

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


//------ Partial specialization for extents<R, C>.
//
template<class T, ptrdiff_t R, ptrdiff_t C, class A, class L>
struct mse_mdspan_traits<mse_data<T, extents<R, C>, A, L>>
:   mse_2d_mdspan_support
{
    using mse_data_type   = mse_data<T, extents<R, C>, A, L>;
    using layout_type     = get_mdspan_layout_t<L>;
    using span_type       = basic_mdspan<T, extents<R, C>, layout_type>;
    using const_span_type = basic_mdspan<T const, extents<R, C>, layout_type>;

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
template<class T, ptrdiff_t R, class A, class L>
struct mse_mdspan_traits<mse_data<T, extents<R, dynamic_extent>, A, L>>
:   mse_2d_mdspan_support
{
    using mse_data_type   = mse_data<T, extents<R, dynamic_extent>, A, L>;
    using span_type       = basic_mdspan<T, dyn_mat_extents, dyn_mat_layout>;
    using const_span_type = basic_mdspan<T const, dyn_mat_extents, dyn_mat_layout>;

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
template<class T, ptrdiff_t C, class A, class L>
struct mse_mdspan_traits<mse_data<T, extents<dynamic_extent, C>, A, L>>
:   mse_2d_mdspan_support
{
    using mse_data_type   = mse_data<T, extents<dynamic_extent, C>, A, L>;
    using span_type       = basic_mdspan<T, dyn_mat_extents, dyn_mat_layout>;
    using const_span_type = basic_mdspan<T const, dyn_mat_extents, dyn_mat_layout>;

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
:   mse_2d_mdspan_support
{
    using mse_data_type   = mse_data<T, extents<dynamic_extent, dynamic_extent>, A, L>;
    using span_type       = basic_mdspan<T, dyn_mat_extents, dyn_mat_layout>;
    using const_span_type = basic_mdspan<T const, dyn_mat_extents, dyn_mat_layout>;

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
//--------------------------------------------------------------------------------------------------
//  Class Template:     matrix_storage_engine<T, extents<N>, A, L>
//
//  This partial specialization of matrix_storage_engine<T,X,A,L> implements an owning engine
//  for use by class template basic_vector<ET, OT>.  Specifically, it models a mathematical
//  vector having N elements, employing allocator A, and having element layout L.
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
//  Partial Specialization:     matrix_storage_engine<T, extents<N>, A, L>
//
//  This partial specialization of matrix_storage_engine<T,X,A,L> implements an owning engine
//  for use by class template basic_vector<ET, OT>.  Specifically, it models a mathematical
//  vector having N elements, employing allocator A, and having element layout L.
//
//  Size N may be a positive integer, or have the value "dynamic_extent" (defined by the
//  mdspan facilities).  Allocator type A may be void, which indicates internal non-heap
//  element storage, or it may be an allocator type that fulfills all the requirements imposed
//  by std::allocator_traits.  Layout type L must be "unspecified".
//
//  This specialization assumes that all dynamically-allocated memory is default-constructed,
//  with the consequence that elements lying in unused capacity are also constructed.  This
//  assumption makes implementation easy, but may be absent in the final version.
//--------------------------------------------------------------------------------------------------
//
template<class T, ptrdiff_t N, class A, class L>
requires
    detail::valid_engine_extents_and_allocator<T, extents<N>, A>
    and
    detail::valid_layout_for_1d_storage_engine<L>
class matrix_storage_engine<T, extents<N>, A, L>
{
    using this_type      = matrix_storage_engine;
    using storage_type   = detail::mse_data<T, extents<N>, A, L>;
    using mdspan_traits  = detail::mse_mdspan_traits<storage_type>;
    using support_traits = detail::vector_engine_support;

    static constexpr bool   is_reshapable = storage_type::is_reshapable;

  private:
    storage_type    m_data;

  public:
    using value_type      = T;
    using allocator_type  = A;
    using element_type    = value_type;
    using reference       = element_type&;
    using const_reference = element_type const&;
    using index_type      = ptrdiff_t;
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

    //- Heterogeneous constructors.
    //
    constexpr
    matrix_storage_engine(index_type size)
    requires
        this_type::is_reshapable
    :   m_data()
    {
        do_reshape(size, size);
    }

    constexpr
    matrix_storage_engine(index_type size, index_type cap)
    requires
        this_type::is_reshapable
    :   m_data()
    {
        do_reshape(size, cap);
    }

    template<class ET2>
    constexpr
    matrix_storage_engine(ET2 const& rhs)
    requires
        detail::readable_vector_engine<ET2>
        and
        detail::convertible_from<element_type, typename ET2::element_type>
    :   m_data()
    {
        support_traits::assign_from(*this, rhs);
    }

    template<class CT>
    constexpr
    matrix_storage_engine(CT const& rhs)
    requires
        detail::random_access_standard_container<CT>
        and
        detail::convertible_from<element_type, typename CT::value_type>
    :   m_data()
    {
        support_traits::assign_from(*this, rhs);
    }

    template<class U, ptrdiff_t X0, class SL, class SA>
    constexpr
    matrix_storage_engine(basic_mdspan<U, extents<X0>, SL, SA> const& rhs)
    requires
        detail::convertible_from<element_type, U>
    :   m_data()
    {
        support_traits::assign_from(*this, rhs);
    }

    template<class U>
    constexpr
    matrix_storage_engine(initializer_list<U> rhs)
    requires
        detail::convertible_from<element_type, U>
    :   m_data()
    {
        support_traits::assign_from(*this, rhs);
    }

    //- Heterogeneous assignment operators.
    //
    template<class ET2>
    constexpr matrix_storage_engine&
    operator =(ET2 const& rhs)
    requires
        detail::readable_vector_engine<ET2>
        and
        detail::convertible_from<element_type, typename ET2::element_type>
    {
        support_traits::assign_from(*this, rhs);
        return *this;
    }

    template<class CT>
    constexpr matrix_storage_engine&
    operator =(CT const& rhs)
    requires
        detail::random_access_standard_container<CT>
        and
        detail::convertible_from<element_type, typename CT::value_type>
    {
        support_traits::assign_from(*this, rhs);
        return *this;
    }

    template<class U, ptrdiff_t X0, class SL, class SA>
    constexpr matrix_storage_engine&
    operator =(basic_mdspan<U, extents<X0>, SL, SA> const& rhs)
    requires
        detail::convertible_from<element_type, U>
    {
        support_traits::assign_from(*this, rhs);
        return *this;
    }

    template<class U>
    constexpr matrix_storage_engine&
    operator =(initializer_list<U> rhs)
    requires
        detail::convertible_from<element_type, U>
    {
        support_traits::assign_from(*this, rhs);
        return *this;
    }

    //- Size and capacity reporting.
    //
    constexpr index_type
    size() const noexcept
    {
        return m_data.m_size;
    }

    constexpr index_type
    capacity() const noexcept
    {
        return m_data.m_cap;
    }

    //- Setting overall size and capacity.
    //
    void
    reshape(index_type newsize, index_type newcap)
    requires
        this_type::is_reshapable
    {
        do_reshape(newsize, newcap);
    }

    //- Element access
    //
    constexpr reference
    operator ()(index_type i)
    {
        return m_data.m_elems[i];
    }

    constexpr const_reference
    operator ()(index_type i) const
    {
        return m_data.m_elems[i];
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

    //- Modifiers
    //
    constexpr void
    swap(matrix_storage_engine& rhs) noexcept
    {
        support_traits::swap(m_data, rhs.m_data);
    }

  private:
    constexpr void
    do_reshape(ptrdiff_t newsize, ptrdiff_t newcap)
    requires
        this_type::is_reshapable
    {
        if (newsize == m_data.m_size) return;

        support_traits::verify_size(newsize);
        support_traits::verify_capacity(newcap);

        //- Only allocate new storage if it is needed.
        //
        if (newsize > m_data.m_cap  ||  newcap != m_data.m_cap)
        {
            //- Normalize requested new capacity.
            //
            newcap = max(newsize, newcap);

            //- Prepare a temporary engine to receive elements from this one.
            //
            this_type   tmp;
            tmp.m_data.m_elems.resize(newcap);
            tmp.m_data.m_size = newsize;
            tmp.m_data.m_cap  = newcap;

            //- Move the appropriate subset of elements into the temporary engine, then swap.
            //
            ptrdiff_t   dst_size = min(newsize, m_data.m_size);
            support_traits::move_elements(tmp, *this, dst_size);
            support_traits::swap(m_data, tmp.m_data);
        }
        else
        {
            if (newsize < m_data.m_size)
            {
                support_traits::fill(*this, newsize, m_data.m_size, T{});
                m_data.m_size = newsize;
            }
        }
    }
};


//--------------------------------------------------------------------------------------------------
//  Partial Specialization:     matrix_storage_engine<T, extents<R, C>, A, L>
//
//  This partial specialization of matrix_storage_engine<T,X,A,L> implements an owning engine
//  for use by class template basic_matrix<ET, OT>.  Specifically, it models a mathematical
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
template<class T, ptrdiff_t R, ptrdiff_t C, class A, class L>
requires
    detail::valid_engine_extents_and_allocator<T, extents<R, C>, A>
    and
    detail::valid_layout_for_2d_storage_engine<L>
class matrix_storage_engine<T, extents<R, C>, A, L>
{
    using this_type      = matrix_storage_engine;
    using storage_type   = detail::mse_data<T, extents<R, C>, A, L>;
    using mdspan_traits  = detail::mse_mdspan_traits<storage_type>;
    using support_traits = detail::matrix_engine_support;

    static constexpr bool   is_1d_indexable      = storage_type::is_1d_indexable;
    static constexpr bool   is_column_major      = storage_type::is_column_major;
    static constexpr bool   is_row_major         = storage_type::is_row_major;
    static constexpr bool   is_2d_reshapable     = storage_type::is_2d_reshapable   ;
    static constexpr bool   is_column_reshapable = storage_type::is_column_reshapable;
    static constexpr bool   is_row_reshapable    = storage_type::is_row_reshapable;

  private:
    storage_type    m_data;

  public:
    using allocator_type  = A;
    using element_type    = T;
    using reference       = element_type&;
    using const_reference = element_type const&;
    using index_type      = ptrdiff_t;
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
    matrix_storage_engine(index_type rows, index_type cols)
    requires
        this_type::is_2d_reshapable
    :   m_data()
    {
        do_reshape(rows, cols, rows, cols);
    }

    constexpr
    matrix_storage_engine(index_type rows, index_type cols, index_type rowcap, index_type colcap)
    requires
        this_type::is_2d_reshapable
    :   m_data()
    {
        do_reshape(rows, cols, rowcap, colcap);
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

    template<class U, ptrdiff_t X0, ptrdiff_t X1, class SL, class SA>
    constexpr
    matrix_storage_engine(basic_mdspan<U, extents<X0, X1>, SL, SA> const& rhs)
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
    template<class ET2>
    constexpr
    matrix_storage_engine(ET2 const& rhs)
    requires
        this_type::is_1d_indexable
        and
        detail::readable_1d_vector_engine<ET2>
        and
        detail::convertible_from<element_type, typename ET2::element_type>
    :   m_data()
    {
        support_traits::assign_from(*this, rhs);
    }

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

    template<class U, ptrdiff_t X0, class SL, class SA>
    constexpr
    matrix_storage_engine(basic_mdspan<U, extents<X0>, SL, SA> const& rhs)
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

    template<class U, ptrdiff_t X0, ptrdiff_t X1, class SL, class SA>
    constexpr matrix_storage_engine&
    operator =(basic_mdspan<U, extents<X0, X1>, SL, SA> const& rhs)
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
    template<class ET2>
    constexpr matrix_storage_engine&
    operator =(ET2 const& rhs)
    requires
        this_type::is_1d_indexable
        and
        detail::readable_1d_vector_engine<ET2>
        and
        detail::convertible_from<element_type, typename ET2::element_type>
    {
        support_traits::assign_from(*this, rhs);
        return *this;
    }

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

    template<class U, ptrdiff_t X0, class SL, class SA>
    constexpr matrix_storage_engine&
    operator =(basic_mdspan<U, extents<X0>, SL, SA> const& rhs)
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
    constexpr index_type
    columns() const noexcept
    {
        return m_data.m_cols;
    }

    constexpr index_type
    rows() const noexcept
    {
        return m_data.m_rows;
    }

    constexpr index_type
    size() const noexcept
    {
        return m_data.m_rows * m_data.m_cols;
    }

    constexpr index_type
    column_capacity() const noexcept
    {
        return m_data.m_colcap;
    }

    constexpr index_type
    row_capacity() const noexcept
    {
        return m_data.m_rowcap;
    }

    constexpr index_type
    capacity() const noexcept
    {
        return m_data.m_rowcap * m_data.m_colcap;
    }

    //- Element access
    //
    constexpr reference
    operator ()(index_type i)
    requires
        this_type::is_1d_indexable
    {
        return m_data.m_elems[i];
    }

    constexpr const_reference
    operator ()(index_type i) const
    requires
        this_type::is_1d_indexable
    {
        return m_data.m_elems[i];
    }

    constexpr reference
    operator ()(index_type i, index_type j)
    requires
        this_type::is_row_major
    {
        return m_data.m_elems[(i * m_data.m_colcap) + j];
    }

    constexpr reference
    operator ()(index_type i, index_type j)
    requires
        this_type::is_column_major
    {
         return m_data.m_elems[i + (j * m_data.m_rowcap)];
    }

    constexpr const_reference
    operator ()(index_type i, index_type j) const
    requires
        this_type::is_row_major
    {
        return m_data.m_elems[(i * m_data.m_colcap) + j];
    }

    constexpr const_reference
    operator ()(index_type i, index_type j) const
    requires
        this_type::is_column_major
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
    reshape(index_type rows, index_type cols, index_type rowcap, index_type colcap)
    requires
        this_type::is_2d_reshapable
    {
        do_reshape(rows, cols, rowcap, colcap);
    }

    //- Setting column size and capacity.
    //
    constexpr void
    reshape_columns(index_type cols, index_type colcap)
    requires
        this_type::is_column_reshapable
    {
        do_reshape_columns(cols, colcap);
    }

    constexpr void
    reshape_columns(index_type cols, index_type colcap)
    requires
        this_type::is_2d_reshapable
    {
        do_reshape(m_data.m_rows, cols, m_data.m_rowcap, colcap);
    }

    //- Setting row size and capacity.
    //
    constexpr void
    reshape_rows(index_type rows, index_type rowcap)
    requires
        this_type::is_row_reshapable
    {
        do_reshape_rows(rows, rowcap);
    }

    constexpr void
    reshape_rows(index_type rows, index_type rowcap)
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
    do_reshape(ptrdiff_t rows, ptrdiff_t cols, ptrdiff_t rowcap, ptrdiff_t colcap)
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
            rowcap = max(rows, rowcap);
            colcap = max(cols, colcap);

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
            ptrdiff_t   dst_rows = min(rows, m_data.m_rows);
            ptrdiff_t   dst_cols = min(cols, m_data.m_cols);
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
    do_reshape_columns(ptrdiff_t cols, ptrdiff_t colcap)
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
            colcap = max(cols, colcap);

            //- Prepare a temporary object to receive elements from this one.
            //
            this_type   tmp;
            tmp.m_data.m_elems.resize(m_data.m_rowcap * colcap);
            tmp.m_data.m_cols   = cols;
            tmp.m_data.m_colcap = colcap;

            //- Move the appropriate subset of elements into the temporary engine and swap.
            //
            ptrdiff_t   dst_cols = min(cols, m_data.m_cols);
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
    do_reshape_rows(ptrdiff_t rows, ptrdiff_t rowcap)
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
            rowcap = max(rows, rowcap);

            //- Prepare a temporary object to receive elements from this one.
            //
            this_type   tmp;
            tmp.m_data.m_elems.resize(rowcap * m_data.m_colcap);
            tmp.m_data.m_rows   = rows;
            tmp.m_data.m_rowcap = rowcap;

            //- Move the appropriate subset of elements into the temporary engine and swap.
            //
            ptrdiff_t   dst_rows = min(rows, m_data.m_rows);
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
