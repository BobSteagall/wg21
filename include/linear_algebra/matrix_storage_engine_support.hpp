//==================================================================================================
//  File:       matrix_storage_engine_support.hpp
//
//  Summary:    This header defines an owning, dense, rectangular engine that owns and manages
//              the elements of a matrix.
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_MATRIX_STORAGE_ENGINE_SUPPORT_HPP_DEFINED
#define LINEAR_ALGEBRA_MATRIX_STORAGE_ENGINE_SUPPORT_HPP_DEFINED

namespace STD_LA {
namespace detail {
//--------------------------------------------------------------------------------------------------
//  Concept:    valid_mse_extents<X>
//
//  This private concept is used to validate the second template argument of a specialization of
//  matrix_storage_engine, the engine's extents type.  It must be a one- or two-dimensional extents
//  type, and each dimension's template argument may have only a certain set of values.
//--------------------------------------------------------------------------------------------------
//
template<class X>
struct is_valid_storage_extents : public false_type {};

//- Valid for 1-D / vector
//
template<ptrdiff_t N>
struct is_valid_storage_extents<extents<N>>
{
    static constexpr bool   value = (N == dynamic_extent || N > 0);
};

//- Valid for 2-D / matrix
//
template<ptrdiff_t R, ptrdiff_t C>
struct is_valid_storage_extents<extents<R,C>>
{
    static constexpr bool   value = (R == dynamic_extent || R > 0) && (C == dynamic_extent || C > 0);
};

//- Concept definition.
//
template<typename X>
concept valid_mse_extents = is_valid_storage_extents<X>::value;


//--------------------------------------------------------------------------------------------------
//  Concept:    valid_mse_allocator<A, T>
//
//  This private concept is used to validate the third template argument of matrix_storage_engine,
//  the allocator type.  It must be void, or it must be possible to instantiate a specialization
//  of allocator_traits<A> that meets certain requirements relative to element type T, such as
//  allocator_traits<A>::value_type is the same type as T.
//--------------------------------------------------------------------------------------------------
//
template<typename T>
concept no_allocator = requires { requires is_same_v<T, void>; };

#if defined(LA_COMPILER_GCC) && ((LA_GCC_VERSION == 9) || (LA_GCC_VERSION == 10))
    //- Neither GCC 9 nor GCC 10 can parse the type requirement 'AT::template rebind_alloc<T>',
    //  so we add a level of indirection and hoist it up into its own alias template.
    //
    template<class AT, class T>
    using at_rebind_alloc_t = typename AT::template rebind_alloc<T>;

    template<typename AT, typename T>
    concept valid_allocator_traits =
        requires
        {
            typename AT::allocator_type;
            typename AT::value_type;
            typename AT::size_type;
            typename AT::pointer;
            typename at_rebind_alloc_t<AT, T>;
            requires is_same_v<T, typename AT::value_type>;
        }
        and
        requires (typename AT::allocator_type a, typename AT::pointer p, typename AT::size_type n)
        {
            { AT::deallocate(a, p, n) };
            { AT::allocate(a, n) } -> returns<typename AT::pointer>;
            { static_cast<T*>(p) };
        #ifdef LA_COMPOUND_REQUIREMENT_SYNTAX_SUPPORTED
            { *p   } -> returns<T&>;
            { p[n] } -> returns<T&>;
        #else
            requires is_same_v<decltype(*p), T&>;
            requires is_same_v<decltype(p[n]), T&>;
        #endif
        };
#else
    //- Clang 10 and VC++ 16.5 accept the following without any problems.
    //
    template<typename AT, typename T>
    concept valid_allocator_traits =
        requires
        {
            typename AT::allocator_type;
            typename AT::value_type;
            typename AT::size_type;
            typename AT::pointer;
            typename AT::template rebind_alloc<T>;
            requires is_same_v<T, typename AT::value_type>;
        }
        and
        requires (typename AT::allocator_type a, typename AT::pointer p, typename AT::size_type n)
        {
            { AT::deallocate(a, p, n) };
            { AT::allocate(a, n) } -> returns<typename AT::pointer>;
            { static_cast<T*>(p) };
            { *p   } -> returns<T&>;
            { p[n] } -> returns<T&>;
        };
#endif

//- Concept definition.
//
template<typename A, typename T>
concept valid_mse_allocator = no_allocator<A> or valid_allocator_traits<allocator_traits<A>, T>;


//--------------------------------------------------------------------------------------------------
//  Concept:    valid_mse_matrix_layout<L>, valid_mse_vector_layout<L>
//
//  This private concept is used to validate the fourth template argument of a specialization of
//  matrix_storage_engine, the engine's layout type.  It must be either row_major or column_major.
//--------------------------------------------------------------------------------------------------
//
template<typename EL>
concept valid_mse_layout = (is_same_v<EL, row_major> || is_same_v<EL, column_major>);

template<typename EL>
concept valid_mse_matrix_layout = (is_same_v<EL, row_major> || is_same_v<EL, column_major>);

template<typename EL>
concept valid_mse_vector_layout = is_same_v<EL, unoriented>;

template<class MSE>                             struct mse_support;
template<class T, class X, class A, class L>    struct mse_data;

//--------------------------------------------------------------------------------------------------
//  Class Template:     mse_support<T, X, A, L>
//
//  Policy/traits type that performs element maniuplation on behalof of mse_data<T, X, A, L>.
//
//  Partial specializations of this class template are tailored to specific corresponding partial
//  specializations of matrix_storage_engine.  They provide only the special member functions,
//  and all other manipulation of their internal state is performed by matrix_storage_engine.
//
//  The implementation assumes that all dynamically-allocated memory is default-constructed,
//  with the consequence that elements lying in unused capacity are also constructed.  This
//  assumption makes implementation easy, but may be absent in the final version.
//--------------------------------------------------------------------------------------------------
//
struct mse_support_base
{
    static inline constexpr void
    verify_capacity(ptrdiff_t c)
    {
        if (c < 0)
        {
            throw runtime_error("invalid capacity parameter");
        }
    }

    static inline constexpr void
    verify_size(ptrdiff_t s)
    {
        if (s < 1)
        {
            throw runtime_error("invalid size parameter");
        }
    }

    static inline constexpr void
    verify_size(ptrdiff_t s1, ptrdiff_t s2)
    {
        if (s1 != s2)
        {
            throw runtime_error("invalid size parameter");
        }
    }

    template<class U>
    static constexpr tuple<ptrdiff_t, ptrdiff_t>
    verify_list(initializer_list<initializer_list<U>> list)
    {
        size_t  rows = list.size();
        size_t  cols = list.begin()->size();

        for (auto&& row : list)
        {
            if (row.size() != cols)
            {
                throw runtime_error("matrix initializer_list has invalid shape");
            }
        }
        return {static_cast<ptrdiff_t>(rows), static_cast<ptrdiff_t>(cols)};
    }
};

template<class T, ptrdiff_t N, class A, class L>
struct mse_support<mse_data<T, extents<N>, A, L>> : public mse_support_base
{
    //- Engine representation type.
    //
    using mse_type = mse_data<T, extents<N>, A, L>;

    template<class FN>
    static constexpr void
    apply(mse_type& dst, ptrdiff_t i0, ptrdiff_t i1, FN fn)
    {
        for (ptrdiff_t i = i0;  i < i1;  ++i)
        {
            dst.m_elems[i] = fn(i);
        }
    }

    template<class ET2>
    static inline constexpr void
    copy_engine(mse_type& dst, ET2 const& src)
    {
        apply(dst, 0, dst.m_size, [&src](ptrdiff_t i){ return src(i); });
    }

    template<class U>
    static constexpr void
    copy_list(mse_type& dst, initializer_list<U> src)
    {
        transform(src.begin(), src.end(), dst.m_elems.begin(),
                    [](U const& u){  return static_cast<T>(u);  });
    }

    static inline constexpr void
    fill(mse_type& dst, ptrdiff_t i0, ptrdiff_t i1, T const& t)
    {
        apply(dst, i0, i1, [&t](ptrdiff_t){ return t; });
    }

    static inline constexpr void
    move_elements(mse_type& dst, mse_type& src, ptrdiff_t size)
    {
        apply(dst, 0, size, [&src](ptrdiff_t i){ return std::move(src.m_elems[i]); });
    }

    static inline constexpr void
    swap(mse_type& m1, mse_type& m2) noexcept
    {
        if (&m1 != &m2)
        {
            mse_type    m0(std::move(m1));
            m1 = std::move(m2);
            m2 = std::move(m0);
        }
    }
};

template<class T, ptrdiff_t R, ptrdiff_t C, class A, class L>
struct mse_support<mse_data<T, extents<R, C>, A, L>>  : public mse_support_base
{
    //- Engine representation type.
    //
    using mse_type = mse_data<T, extents<R, C>, A, L>;

    template<class FN>
    static constexpr void
    apply(mse_type& dst, ptrdiff_t i0, ptrdiff_t j0, ptrdiff_t i1, ptrdiff_t j1, FN fn)
    {
        if constexpr (is_same_v<L, row_major>)
        {
            for (ptrdiff_t i = i0;  i < i1;  ++i)
            {
                for (ptrdiff_t j = j0;  j < j1;  ++j)
                {
                    element(dst, i, j) = fn(i, j);
                }
            }
        }
        else
        {
            for (ptrdiff_t j = j0;  j < j1;  ++j)
            {
                for (ptrdiff_t i = i0;  i < i1;  ++i)
                {
                    element(dst, i, j) = fn(i, j);
                }
            }
        }
    }

    static inline constexpr T&
    element(mse_type& dst, ptrdiff_t i, ptrdiff_t j)
    {
        if constexpr (mse_type::is_row_major)
            return dst.m_elems[i*dst.m_colcap + j];
        else
            return dst.m_elems[i + j*dst.m_rowcap];
    }

    template<class ET>
    static inline constexpr void
    copy_engine(mse_type& dst, ET const& src)
    {
        apply(dst, 0, 0, dst.m_rows, dst.m_cols,
                [&src](ptrdiff_t i, ptrdiff_t j){  return static_cast<T>(src(i, j));  });
    }

    template<class U>
    static constexpr void
    copy_list(mse_type& dst, initializer_list<U> src)
    {
        transform(src.begin(), src.end(), dst.m_elems.begin(),
                    [](U const& u){  return static_cast<T>(u);  });
    }

    template<class T2>
    static constexpr void
    copy_list(mse_type& dst, initializer_list<initializer_list<T2>> src)
    {
        using row_iter = decltype(src.begin());
        using col_iter = decltype(src.begin()->begin());

        ptrdiff_t   di = 0;
        row_iter    rp = src.begin();

        for (;  di < dst.m_rows;  ++di, ++rp)
        {
            ptrdiff_t   dj = 0;
            col_iter    cp = rp->begin();

            for (;  dj < dst.m_cols;  ++dj, ++cp)
            {
                element(dst, di, dj) = static_cast<T>(*cp);
            }
        }
    }

    static inline constexpr void
    fill_columns(mse_type& dst, ptrdiff_t c0, ptrdiff_t c1, T const& t)
    {
        apply(dst, 0, c0, dst.m_rows, c1,
                [&t](ptrdiff_t, ptrdiff_t){  return t;  });
    }

    static inline constexpr void
    fill_rows(mse_type& dst, ptrdiff_t r0, ptrdiff_t r1, T const& t)
    {
        apply(dst, r0, 0, r1, dst.m_cols,
                [&t](ptrdiff_t, ptrdiff_t){  return t;  });
    }

    static inline constexpr void
    move_elements(mse_type& dst, mse_type& src, ptrdiff_t rows, ptrdiff_t cols)
    {
        apply(dst, 0, 0, rows, cols,
                [&src](ptrdiff_t i, ptrdiff_t j){  return std::move(element(src, i, j));  });
    }

    static inline constexpr void
    swap(mse_type& m1, mse_type& m2) noexcept
    {
        if (&m1 != &m2)
        {
            mse_type    m0(std::move(m1));
            m1 = std::move(m2);
            m2 = std::move(m0);
        }
    }

    //- Support for mdspan -- types and functions.
    //
    static constexpr bool   use_dyn_span = (R == dynamic_extent || C == dynamic_extent);

    using fixed_layout  = conditional_t<is_same_v<L, row_major>, layout_right, layout_left>;
    using fixed_mdspan  = basic_mdspan<T, extents<R, C>, fixed_layout>;
    using fixed_cmdspan = basic_mdspan<T const, extents<R, C>, fixed_layout>;

    using dyn_extents = extents<dynamic_extent, dynamic_extent>;
    using dyn_layout  = layout_stride<dynamic_extent, dynamic_extent>;
    using dyn_mapping = typename dyn_layout::template mapping<dyn_extents>;
    using dyn_strides = array<typename dyn_extents::index_type, 2>;
    using dyn_mdspan  = basic_mdspan<T, dyn_extents, dyn_layout>;
    using dyn_cmdspan = basic_mdspan<T const, dyn_extents, dyn_layout>;

    using span_type       = conditional_t<use_dyn_span, dyn_mdspan, fixed_mdspan>;
    using const_span_type = conditional_t<use_dyn_span, dyn_cmdspan, fixed_cmdspan>;

    static inline constexpr span_type
    make_span(mse_type& rep)
    {
        if constexpr (use_dyn_span)
        {
            if constexpr (is_same_v<L, row_major>)
            {
                dyn_extents     extents(rep.m_rows, rep.m_cols);
                dyn_strides     strides{rep.m_colcap, 1};
                dyn_mapping     mapping(extents, strides);

                return span_type(rep.m_elems.data(), mapping);
            }
            else
            {
                dyn_extents     extents(rep.m_rows, rep.m_cols);
                dyn_strides     strides{1, rep.m_rowcap};
                dyn_mapping     mapping(extents, strides);

                return span_type(rep.m_elems.data(), mapping);
            }
        }
        else
        {
            return span_type(rep.m_elems.data());
        }
    }

    static inline constexpr const_span_type
    make_const_span(mse_type const& rep)
    {
        if constexpr (use_dyn_span)
        {
            if constexpr (is_same_v<L, row_major>)
            {
                dyn_extents     extents(rep.m_rows, rep.m_cols);
                dyn_strides     strides{rep.m_colcap, 1};
                dyn_mapping     mapping(extents, strides);

                return const_span_type(rep.m_elems.data(), mapping);
            }
            else
            {
                dyn_extents     extents(rep.m_rows, rep.m_cols);
                dyn_strides     strides{1, rep.m_rowcap};
                dyn_mapping     mapping(extents, strides);

                return const_span_type(rep.m_elems.data(), mapping);
            }
        }
        else
        {
            return const_span_type(rep.m_elems.data());
        }
    }
};


//--------------------------------------------------------------------------------------------------
//  Class Template:     mse_data<T, X, A, L>
//
//  Type that contains and manages elements on behalf of matrix_storage_engine<T,X,A,L>.
//
//  Partial specializations of this class template are tailored to specific corresponding partial
//  specializations of matrix_storage_engine.  They provide only the special member functions,
//  and all other manipulation of their internal state is performed by matrix_storage_engine.
//
//  The implementation assumes that all dynamically-allocated memory is default-constructed,
//  with the consequence that elements lying in unused capacity are also constructed.  This
//  assumption makes implementation easy, but may be absent in the final version.
//--------------------------------------------------------------------------------------------------
//
//-----------------------------------------------------------------
//- Fixed size.  Elements contained as member data in a std::array.
//
template<class T, ptrdiff_t N, class L>
struct mse_data<T, extents<N>, void, L>
{
    using this_type       = mse_data;
    using array_type      = std::array<T, N>;
    using support         = mse_support<this_type>;
    using span_type       = mdspan<T, N>;
    using const_span_type = mdspan<T const, N>;

    static constexpr bool   is_resizable = false;

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

    inline constexpr span_type
    span() noexcept
    {
        return span_type(m_elems.data());
    }

    inline constexpr const_span_type
    span() const noexcept
    {
        return const_span_type(m_elems.data());
    }

    template<class U>
    constexpr void
    assign(initializer_list<U> src)
    {
        ptrdiff_t   size = static_cast<ptrdiff_t>(src.size());
        support::verify_size(size, m_size);
        support::copy_list(*this, src);
    }

    template<class ET>
    constexpr void
    assign(ET const& eng)
    {
        support::verify_size(static_cast<ptrdiff_t>(eng.size()), m_size);
        support::copy_engine(*this, eng);
    }

    inline constexpr void
    swap(mse_data& rhs) noexcept
    {
        support::swap(*this, rhs);
    }
};

//------------------------------------------------------------------
//- Fixed size.  Elements contained as member data in a std::vector.
//
template<class T, ptrdiff_t N, class A, class L>
struct mse_data<T, extents<N>, A, L>
{
    using this_type       = mse_data;
    using array_type      = std::vector<T, A>;
    using support         = mse_support<this_type>;
    using span_type       = mdspan<T, N>;
    using const_span_type = mdspan<T const, N>;

    static constexpr bool   is_resizable = false;

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

    inline span_type
    span() noexcept
    {
        return span_type(m_elems.data());
    }

    inline const_span_type
    span() const noexcept
    {
        return const_span_type(m_elems.data());
    }

    template<class U>
    constexpr void
    assign(initializer_list<U> src)
    {
        ptrdiff_t   size = static_cast<ptrdiff_t>(src.size());
        support::verify_size(size, m_size);
        support::copy_list(*this, src);
    }

    template<class ET>
    constexpr void
    assign(ET const& eng)
    {
        support::verify_size(static_cast<ptrdiff_t>(eng.size()), m_size);
        support::copy_engine(*this, eng);
    }

    inline void
    swap(mse_data& rhs) noexcept
    {
        support::swap(*this, rhs);
    }
};

//--------------------------------------------------------------------
//- Dynamic size.  Elements contained as member data in a std::vector.
//
template<class T, class A, class L>
struct mse_data<T, extents<dynamic_extent>, A, L>
{
    using this_type       = mse_data;
    using array_type      = std::vector<T, A>;
    using support         = mse_support<this_type>;
    using span_type       = mdspan<T, dynamic_extent>;
    using const_span_type = mdspan<T const, dynamic_extent>;

    static constexpr bool   is_resizable = true;

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

    inline constexpr span_type
    span() noexcept
    {
        return span_type(m_elems.data(), m_size);
    }

    inline constexpr const_span_type
    span() const noexcept
    {
        return const_span_type(m_elems.data(), m_size);
    }

    template<class U>
    constexpr void
    assign(initializer_list<U> src)
    {
        reshape(static_cast<ptrdiff_t>(src.size()), m_cap);
        support::copy_list(*this, src);
    }

    template<class ET>
    constexpr void
    assign(ET const& eng)
    {
        reshape(static_cast<ptrdiff_t>(eng.size()), m_cap);
        support::copy_engine(*this, eng);
    }

    void
    reshape(ptrdiff_t newsize, ptrdiff_t newcap)
    {
        if (newsize == m_size) return;

        support::verify_size(newsize);
        support::verify_capacity(newcap);

        //- Only reallocate new storage if we have to.
        //
        if (newsize > m_cap  ||  newcap != m_cap)
        {
            //- Normalize requested new capacity.
            //
            newcap = max(newsize, newcap);

            //- Prepare a temporary engine to receive elements from this one.
            //
            this_type   tmp;
            tmp.m_elems.resize(newcap);
            tmp.m_size = newsize;
            tmp.m_cap  = newcap;

            //- Move the appropriate subset of elements into the temporary engine, then swap.
            //
            ptrdiff_t   dst_size = min(newsize, m_size);
            support::move_elements(tmp, *this, dst_size);
            support::swap(*this, tmp);
        }
        else
        {
            if (newsize < m_size)
            {
                support::fill(*this, newsize, m_size, T{});
            }
            m_size = newsize;
        }
    }

    inline void
    swap(mse_data& rhs) noexcept
    {
        support::swap(*this, rhs);
    }
};

//---------------------------------------------------------------------------------
//- Fixed rows / fixed columns.  Elements contained as member data in a std::array.
//
template<class T, ptrdiff_t R, ptrdiff_t C, class L>
struct mse_data<T, extents<R, C>, void, L>
{
    using this_type       = mse_data;
    using array_type      = std::array<T, R*C>;
    using support         = mse_support<this_type>;
    using fixed_layout    = conditional_t<is_same_v<L, row_major>, layout_right, layout_left>;
    using span_type       = basic_mdspan<T, extents<R, C>, fixed_layout>;
    using const_span_type = basic_mdspan<T const, extents<R, C>, fixed_layout>;

    static constexpr bool   is_column_matrix    = (C == 1);
    static constexpr bool   is_row_matrix       = (R == 1);
    static constexpr bool   is_linear_matrix    = (is_column_matrix || is_row_matrix);
    static constexpr bool   is_column_resizable = false;
    static constexpr bool   is_row_resizable    = false;
    static constexpr bool   is_resizable        = false;
    static constexpr bool   is_column_major     = is_same_v<L, column_major>;
    static constexpr bool   is_row_major        = is_same_v<L, row_major>;

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

    inline span_type
    span() noexcept
    {
        return span_type(m_elems.data());
    }

    inline const_span_type
    span() const noexcept
    {
        return const_span_type(m_elems.data());
    }

    template<class U>
    constexpr void
    assign(initializer_list<U> src)
        requires this_type::is_column_matrix
    {
        ptrdiff_t   rows = static_cast<ptrdiff_t>(src.size());
        support::verify_size(rows, m_rows);
        support::copy_list(*this, src);
    }

    template<class U>
    constexpr void
    assign(initializer_list<U> src)
        requires this_type::is_row_matrix
    {
        ptrdiff_t   cols = static_cast<ptrdiff_t>(src.size());
        support::verify_size(cols, m_cols);
        support::copy_list(*this, src);
    }

    template<class U>
    constexpr void
    assign(initializer_list<initializer_list<U>> src)
    {
        auto    [rows, cols] = support::verify_list(src);
        support::verify_size(cols, m_cols);
        support::verify_size(rows, m_rows);
        support::copy_list(*this, src);
    }

    template<class ET>
    constexpr void
    assign(ET const& eng)
    {
        support::verify_size(static_cast<ptrdiff_t>(eng.rows()), m_rows);
        support::verify_size(static_cast<ptrdiff_t>(eng.columns()), m_cols);
        support::copy_engine(*this, eng);
    }

    inline void
    swap(mse_data& rhs) noexcept
    {
        support::swap(*this, rhs);
    }
};

//-------------------------------------------------------------------------------
//- Fixed rows / fixed columns.  Elements dynamically allocated in a std::vector.
//
template<class T, ptrdiff_t R, ptrdiff_t C, class A, class L>
struct mse_data<T, extents<R, C>, A, L>
{
    using this_type       = mse_data;
    using array_type      = std::vector<T, A>;
    using support         = mse_support<this_type>;
    using fixed_layout    = conditional_t<is_same_v<L, row_major>, layout_right, layout_left>;
    using span_type       = basic_mdspan<T, extents<R, C>, fixed_layout>;
    using const_span_type = basic_mdspan<T const, extents<R, C>, fixed_layout>;

    static constexpr bool   is_column_matrix    = (C == 1);
    static constexpr bool   is_row_matrix       = (R == 1);
    static constexpr bool   is_linear_matrix    = (is_column_matrix || is_row_matrix);
    static constexpr bool   is_column_resizable = false;
    static constexpr bool   is_row_resizable    = false;
    static constexpr bool   is_resizable        = false;
    static constexpr bool   is_column_major     = is_same_v<L, column_major>;
    static constexpr bool   is_row_major        = is_same_v<L, row_major>;

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

    inline span_type
    span() noexcept
    {
        return span_type(m_elems.data());
    }

    inline const_span_type
    span() const noexcept
    {
        return const_span_type(m_elems.data());
    }

    template<class U>
    constexpr void
    assign(initializer_list<U> src)
        requires this_type::is_column_matrix
    {
        ptrdiff_t   rows = static_cast<ptrdiff_t>(src.size());
        support::verify_size(rows, m_rows);
        support::copy_list(*this, src);
    }

    template<class U>
    constexpr void
    assign(initializer_list<U> src)
        requires this_type::is_row_matrix
    {
        ptrdiff_t   cols = static_cast<ptrdiff_t>(src.size());
        support::verify_size(cols, m_cols);
        support::copy_list(*this, src);
    }

    template<class U>
    void
    assign(initializer_list<initializer_list<U>> src)
    {
        auto    [rows, cols] = support::verify_list(src);
        support::verify_size(cols, m_cols);
        support::verify_size(rows, m_rows);
        support::copy_list(*this, src);
    }

    template<class ET>
    void
    assign(ET const& eng)
    {
        support::verify_size(static_cast<ptrdiff_t>(eng.rows()), m_rows);
        support::verify_size(static_cast<ptrdiff_t>(eng.columns()), m_cols);
        support::copy_engine(*this, eng);
    }

    inline void
    swap(mse_data& rhs) noexcept
    {
        support::swap(*this, rhs);
    }
};

//---------------------------------------------------------------------------------
//- Fixed rows / dynamic columns.  Elements dynamically allocated in a std::vector.
//
template<class T, ptrdiff_t R, class A, class L>
struct mse_data<T, extents<R, dynamic_extent>, A, L>
{
    using this_type       = mse_data;
    using array_type      = std::vector<T, A>;
    using support         = mse_support<this_type>;
    using span_type       = typename support::span_type;
    using const_span_type = typename support::const_span_type;

    static constexpr bool   is_column_matrix    = false;
    static constexpr bool   is_row_matrix       = (R == 1);
    static constexpr bool   is_linear_matrix    = is_row_matrix;
    static constexpr bool   is_column_resizable = true;
    static constexpr bool   is_row_resizable    = false;
    static constexpr bool   is_resizable        = false;
    static constexpr bool   is_column_major     = is_same_v<L, column_major>;
    static constexpr bool   is_row_major        = is_same_v<L, row_major>;

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

    inline span_type
    span() noexcept
    {
        return support::make_span(*this);
    }

    inline const_span_type
    span() const noexcept
    {
        return support::make_const_span(*this);
    }

    template<class U>
    constexpr void
    assign(initializer_list<U> src)
        requires this_type::is_row_matrix
    {
        ptrdiff_t   cols = static_cast<ptrdiff_t>(src.size());
        reshape_columns(cols, m_colcap);
        support::copy_list(*this, src);
    }

    template<class U>
    void
    assign(initializer_list<initializer_list<U>> src)
    {
        auto    [rows, cols] = support::verify_list(src);
        support::verify_size(rows, m_rows);
        reshape_columns(cols, m_colcap);
        support::copy_list(*this, src);
    }

    template<class ET>
    void
    assign(ET const& eng)
    {
        support::verify_size(static_cast<ptrdiff_t>(eng.rows()), m_rows);
        reshape_columns(static_cast<ptrdiff_t>(eng.columns()), m_colcap);
        support::copy_engine(*this, eng);
    }

    void
    reshape_columns(ptrdiff_t cols, ptrdiff_t colcap)
    {
        support::verify_size(cols);
        support::verify_capacity(colcap);

        //- Only reallocate new storage if we have to.
        //
        if (cols > m_colcap  ||  colcap != m_colcap)
        {
            //- Normalize requested new capacity.
            //
            colcap = max(cols, colcap);

            //- Prepare a temporary object to receive elements from this one.
            //
            this_type   tmp;
            tmp.m_elems.resize(m_rowcap*colcap);
            tmp.m_cols   = cols;
            tmp.m_colcap = colcap;

            //- Move the appropriate subset of elements into the temporary engine and swap.
            //
            ptrdiff_t   dst_cols = min(cols, m_cols);
            support::move_elements(tmp, *this, m_rows, dst_cols);
            support::swap(*this, tmp);
        }
        else
        {
            if (cols < m_cols)
            {
                support::fill_columns(*this, cols, m_cols, T{});
            }
            m_cols = cols;
        }
    }

    inline void
    swap(mse_data& rhs) noexcept
    {
        support::swap(*this, rhs);
    }
};

//---------------------------------------------------------------------------------
//- Dynamic rows / fixed columns.  Elements dynamically allocated in a std::vector.
//
template<class T, ptrdiff_t C, class A, class L>
struct mse_data<T, extents<dynamic_extent, C>, A, L>
{
    using this_type       = mse_data;
    using array_type      = std::vector<T, A>;
    using support         = mse_support<this_type>;
    using span_type       = typename support::span_type;
    using const_span_type = typename support::const_span_type;

    static constexpr bool   is_column_matrix    = (C == 1);
    static constexpr bool   is_row_matrix       = false;
    static constexpr bool   is_linear_matrix    = is_column_matrix;
    static constexpr bool   is_column_resizable = false;
    static constexpr bool   is_row_resizable    = true;
    static constexpr bool   is_resizable        = false;
    static constexpr bool   is_column_major     = is_same_v<L, column_major>;
    static constexpr bool   is_row_major        = is_same_v<L, row_major>;

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

    inline span_type
    span() noexcept
    {
        return support::make_span(*this);
    }

    inline const_span_type
    span() const noexcept
    {
        return support::make_const_span(*this);
    }

    template<class U>
    constexpr void
    assign(initializer_list<U> src)
        requires this_type::is_column_matrix
    {
        ptrdiff_t   rows = static_cast<ptrdiff_t>(src.size());
        reshape_rows(rows, m_rowcap);
        support::copy_list(*this, src);
    }

    template<class U>
    void
    assign(initializer_list<initializer_list<U>> src)
    {
        auto    [rows, cols] = support::verify_list(src);
        support::verify_size(cols, m_cols);
        reshape_rows(rows, m_rowcap);
        support::copy_list(*this, src);
    }

    template<class ET>
    void
    assign(ET const& eng)
    {
        support::verify_size(static_cast<ptrdiff_t>(eng.columns()), m_cols);
        reshape_rows(static_cast<ptrdiff_t>(eng.rows()), m_rowcap);
        support::copy_engine(*this, eng);
    }

    void
    reshape_rows(ptrdiff_t rows, ptrdiff_t rowcap)
    {
        support::verify_size(rows);
        support::verify_capacity(rowcap);

        //- Only reallocate new storage if we have to.
        //
        if (rows > m_rowcap  ||  rowcap != m_rowcap)
        {
            //- Normalize requested new capacity.
            //
            rowcap = max(rows, rowcap);

            //- Prepare a temporary object to receive elements from this one.
            //
            this_type   tmp;
            tmp.m_elems.resize(rowcap*m_colcap);
            tmp.m_rows   = rows;
            tmp.m_rowcap = rowcap;

            //- Move the appropriate subset of elements into the temporary engine and swap.
            //
            ptrdiff_t   dst_rows = min(rows, m_rows);
            support::move_elements(tmp, *this, dst_rows, m_cols);
            support::swap(*this, tmp);
        }
        else
        {
            if (rows < m_rows)
            {
                support::fill_rows(*this, rows, m_rows, T{});
            }
            m_rows = rows;
        }
    }

    inline void
    swap(mse_data& rhs) noexcept
    {
        support::swap(*this, rhs);
    }
};

//-----------------------------------------------------------------------------------
//- Dynamic rows / dynamic columns.  Elements dynamically allocated in a std::vector.
//
template<class T, class A, class L>
struct mse_data<T, extents<dynamic_extent, dynamic_extent>, A, L>
{
    using this_type       = mse_data;
    using array_type      = std::vector<T, A>;
    using support         = mse_support<this_type>;
    using span_type       = typename support::span_type;
    using const_span_type = typename support::const_span_type;

    static constexpr bool   is_linear_matrix    = false;
    static constexpr bool   is_column_matrix    = false;
    static constexpr bool   is_row_matrix       = false;
    static constexpr bool   is_column_resizable = true;
    static constexpr bool   is_row_resizable    = true;
    static constexpr bool   is_resizable        = true;
    static constexpr bool   is_column_major     = is_same_v<L, column_major>;
    static constexpr bool   is_row_major        = is_same_v<L, row_major>;

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

    inline span_type
    span() noexcept
    {
        return support::make_span(*this);
    }

    inline const_span_type
    span() const noexcept
    {
        return support::make_const_span(*this);
    }

    template<class U>
    void
    assign(initializer_list<initializer_list<U>> src)
    {
        auto    [rows, cols] = support::verify_list(src);
        reshape(rows, cols, m_rowcap, m_colcap);
        support::copy_list(*this, src);
    }

    template<class ET>
    void
    assign(ET const& eng)
    {
        reshape(eng.rows(), eng.columns(), m_rowcap, m_colcap);
        support::copy_engine(*this, eng);
    }

    inline void
    reshape_columns(ptrdiff_t cols, ptrdiff_t colcap)
    {
        reshape(m_rows, cols, m_rowcap, colcap);
    }

    inline void
    reshape_rows(ptrdiff_t rows, ptrdiff_t rowcap)
    {
        reshape(rows, m_cols, rowcap, m_colcap);
    }

    void
    reshape(ptrdiff_t rows, ptrdiff_t cols, ptrdiff_t rowcap, ptrdiff_t colcap)
    {
        support::verify_size(rows);
        support::verify_size(cols);
        support::verify_capacity(rowcap);
        support::verify_capacity(colcap);

        //- Only reallocate new storage if we have to.
        //
        if (rows > m_rowcap  ||  cols > m_colcap  ||  rowcap != m_rowcap  ||  colcap != m_colcap)
        {
            //- Normalize requested new capacities.
            //
            rowcap = max(rows, rowcap);
            colcap = max(cols, colcap);

            //- Prepare a temporary object to receive elements from this one.
            //
            this_type   tmp;
            tmp.m_elems.resize(rowcap*colcap);
            tmp.m_rows   = rows;
            tmp.m_cols   = cols;
            tmp.m_rowcap = rowcap;
            tmp.m_colcap = colcap;

            //- Move the appropriate subset of elements into the temporary engine and swap.
            //
            ptrdiff_t   dst_rows = min(rows, this->m_rows);
            ptrdiff_t   dst_cols = min(cols, this->m_cols);
            support::move_elements(tmp, *this, dst_rows, dst_cols);
            support::swap(*this, tmp);
        }
        else
        {
            if (rows < m_rows)
            {
                support::fill_rows(*this, rows, m_rows, T{});
            }
            if (cols < m_cols)
            {
                support::fill_columns(*this, cols, m_cols, T{});
            }
            m_rows = rows;
            m_cols = cols;
        }
    }

    inline void
    swap(mse_data& rhs) noexcept
    {
        support::swap(*this, rhs);
    }
};

}       //- detail namespace
}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_MATRIX_STORAGE_ENGINE_SUPPORT_HPP_DEFINED
