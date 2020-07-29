//==================================================================================================
//  File:       matrix_storage_engine_data.hpp
//
//  Summary:    This header defines several partial specializations of mse_data<T, X, A, L>.
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_MATRIX_STORAGE_ENGINE_DATA_HPP_DEFINED
#define LINEAR_ALGEBRA_MATRIX_STORAGE_ENGINE_DATA_HPP_DEFINED

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

    static constexpr bool   is_fixed_size = true;
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
            m_elems.fill(T{});
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

    static constexpr bool   is_fixed_size = true;
    static constexpr bool   is_reshapable = false;

    static constexpr ptrdiff_t  m_size = N;
    static constexpr ptrdiff_t  m_cap  = N;

    array_type  m_elems;

    ~mse_data() = default;
    inline mse_data()
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

    static constexpr bool   is_fixed_size = false;
    static constexpr bool   is_reshapable = true;

    array_type  m_elems;
    ptrdiff_t   m_size;
    ptrdiff_t   m_cap;

    ~mse_data() = default;
    inline constexpr mse_data()
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

    static constexpr bool   is_column_matrix      = (C == 1);
    static constexpr bool   is_row_matrix         = (R == 1);
    static constexpr bool   is_linearly_indexable = (is_column_matrix || is_row_matrix);
    static constexpr bool   is_fixed_size         = true;
    static constexpr bool   is_column_reshapable  = false;
    static constexpr bool   is_row_reshapable     = false;
    static constexpr bool   is_reshapable         = false;
    static constexpr bool   is_column_major       = is_same_v<L, column_major>;
    static constexpr bool   is_row_major          = is_same_v<L, row_major>;

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
            m_elems.fill(T{});
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

    static constexpr bool   is_column_matrix      = (C == 1);
    static constexpr bool   is_row_matrix         = (R == 1);
    static constexpr bool   is_linearly_indexable = (is_column_matrix || is_row_matrix);
    static constexpr bool   is_fixed_size         = true;
    static constexpr bool   is_column_reshapable  = false;
    static constexpr bool   is_row_reshapable     = false;
    static constexpr bool   is_reshapable         = false;
    static constexpr bool   is_column_major       = is_same_v<L, column_major>;
    static constexpr bool   is_row_major          = is_same_v<L, row_major>;

    static constexpr ptrdiff_t  m_rows   = R;
    static constexpr ptrdiff_t  m_cols   = C;
    static constexpr ptrdiff_t  m_rowcap = R;
    static constexpr ptrdiff_t  m_colcap = C;

    array_type  m_elems;

    //- Construct/copy/destroy.
    //
    ~mse_data() = default;

    inline constexpr mse_data()
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

    static constexpr bool   is_column_matrix      = false;
    static constexpr bool   is_row_matrix         = (R == 1);
    static constexpr bool   is_linearly_indexable = is_row_matrix;
    static constexpr bool   is_fixed_size         = false;
    static constexpr bool   is_column_reshapable  = true;
    static constexpr bool   is_row_reshapable     = false;
    static constexpr bool   is_reshapable         = false;
    static constexpr bool   is_column_major       = is_same_v<L, column_major>;
    static constexpr bool   is_row_major          = is_same_v<L, row_major>;

    static constexpr ptrdiff_t  m_rows   = R;
    static constexpr ptrdiff_t  m_rowcap = R;

    array_type  m_elems;
    ptrdiff_t   m_cols;
    ptrdiff_t   m_colcap;

    //- Construct/copy/destroy.
    //
    ~mse_data() = default;

    inline constexpr
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

    static constexpr bool   is_column_matrix      = (C == 1);
    static constexpr bool   is_row_matrix         = false;
    static constexpr bool   is_linearly_indexable = is_column_matrix;
    static constexpr bool   is_fixed_size         = false;
    static constexpr bool   is_column_reshapable  = false;
    static constexpr bool   is_row_reshapable     = true;
    static constexpr bool   is_reshapable         = false;
    static constexpr bool   is_column_major       = is_same_v<L, column_major>;
    static constexpr bool   is_row_major          = is_same_v<L, row_major>;

    static constexpr ptrdiff_t  m_cols   = C;
    static constexpr ptrdiff_t  m_colcap = C;

    array_type  m_elems;
    ptrdiff_t   m_rows;
    ptrdiff_t   m_rowcap;

    //- Construct/copy/destroy.
    //
    ~mse_data() = default;

    inline constexpr
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

    static constexpr bool   is_column_matrix      = false;
    static constexpr bool   is_row_matrix         = false;
    static constexpr bool   is_linearly_indexable = false;
    static constexpr bool   is_fixed_size         = false;
    static constexpr bool   is_column_reshapable  = true;
    static constexpr bool   is_row_reshapable     = true;
    static constexpr bool   is_reshapable         = true;
    static constexpr bool   is_column_major       = is_same_v<L, column_major>;
    static constexpr bool   is_row_major          = is_same_v<L, row_major>;

    array_type  m_elems;
    ptrdiff_t   m_rows;
    ptrdiff_t   m_cols;
    ptrdiff_t   m_rowcap;
    ptrdiff_t   m_colcap;

    //- Construct/copy/destroy.
    //
    ~mse_data() = default;

    inline
    mse_data()
    :   m_elems(), m_rows(0), m_cols(0), m_rowcap(0), m_colcap(0)
    {}
    mse_data(mse_data&&) noexcept = default;
    mse_data(mse_data const&) = default;
    mse_data&   operator =(mse_data&&) noexcept = default;
    mse_data&   operator =(mse_data const&) = default;
};


//--------------------------------------------------------------------------------------------------
//  Concepts:   linearly_indexable<MSD>
//              reshapable<MSD>
//              column_reshapable<MSD>
//              row_reshapable<MSD>
//
//  These private concepts are used to probe various properties of the mse_data specializations
//  defined above.
//--------------------------------------------------------------------------------------------------
//
template<typename MSD>
concept linearly_indexable_msd = MSD::is_linearly_indexable;

template<typename MSD>
concept reshapable_msd = MSD::is_reshapable;

template<typename MSD>
concept column_reshapable_msd = MSD::is_column_reshapable;

template<typename MSD>
concept row_reshapable_msd = MSD::is_row_reshapable;


}       //- detail namespace
}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_MATRIX_STORAGE_ENGINE_DATA_HPP_DEFINED
