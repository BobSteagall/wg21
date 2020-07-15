//==================================================================================================
//  File:       matrix_storage_engine_support.hpp
//
//  Summary:    This header defines several concepts and types whose purpose is to support the
//              mse_data and matrix_storage_engine types.
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_MATRIX_STORAGE_ENGINE_SUPPORT_HPP_DEFINED
#define LINEAR_ALGEBRA_MATRIX_STORAGE_ENGINE_SUPPORT_HPP_DEFINED

namespace STD_LA {
namespace detail {
//- Some forward declarations.
//
template<class MSE>                             struct mse_support;
template<class T, class X, class A, class L>    struct mse_data;

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
concept no_allocator = same_as<T, void>;

#if defined(LA_COMPILER_GCC) && ((LA_GCC_VERSION == 9) || (LA_GCC_VERSION == 10))
    //- Neither GCC 9 nor GCC 10 can parse the type requirement 'AT::template rebind_alloc<T>',
    //  so we add a level of indirection and hoist it up into its own alias template.
    //
    template<class AT, class T>
    using at_rebind_alloc_t = typename AT::template rebind_alloc<T>;

    template<typename AT, typename T>
    concept valid_allocator_traits =
        requires (typename AT::allocator_type a, typename AT::pointer p, typename AT::size_type n)
        {
            typename AT::allocator_type;
            typename AT::value_type;
            typename AT::size_type;
            typename AT::pointer;
            typename at_rebind_alloc_t<AT, T>;
            requires is_same_v<T, typename AT::value_type>;
            { AT::deallocate(a, p, n) };
            { AT::allocate(a, n) } -> same_as<typename AT::pointer>;
            { static_cast<T*>(p) };
        #ifdef LA_COMPOUND_REQUIREMENT_SYNTAX_SUPPORTED
            { *p   } -> same_as<T&>;
            { p[n] } -> same_as<T&>;
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
        requires (typename AT::allocator_type a, typename AT::pointer p, typename AT::size_type n)
        {
            typename AT::allocator_type;
            typename AT::value_type;
            typename AT::size_type;
            typename AT::pointer;
            typename AT::template rebind_alloc<T>;
            requires is_same_v<T, typename AT::value_type>;
            { AT::deallocate(a, p, n) };
            { AT::allocate(a, n) } -> same_as<typename AT::pointer>;
            { static_cast<T*>(p) };
            { *p   } -> same_as<T&>;
            { p[n] } -> same_as<T&>;
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
template<typename L>
concept valid_mse_layout = (is_same_v<L, row_major> || is_same_v<L, column_major>);

template<typename L>
concept valid_mse_matrix_layout = (is_same_v<L, row_major> || is_same_v<L, column_major>);

template<typename L>
concept valid_mse_vector_layout = is_same_v<L, unoriented>;

template<typename MSD>
concept resizable = MSD::is_resizable;

template<typename MSD>
concept column_resizable = MSD::is_column_resizable;

template<typename MSD>
concept row_resizable = MSD::is_row_resizable;

template<typename MSD>
concept linear_matrix = MSD::is_linear_matrix;

//--------------------------------------------------------------------------------------------------
//  Partial Specialization:     mse_support<T, extents<N>, A, L>
//
//  Traits type that performs certain element manipulation on behalf of mse_data<T, X, A, L>.
//
//  This partial specialization of mse_support provides services to the various partial
//  specializations of mse_data having the form mse_data<T, extents<N>, A, L>.  These
//  services are common to all of those partial specializations, and so are consolidated
//  into one place (here).
//--------------------------------------------------------------------------------------------------
//
template<class T, ptrdiff_t N, class A, class L>
struct mse_support<mse_data<T, extents<N>, A, L>> : public engine_support_base
{
    //- Engine representation type.
    //
    using mse_type = mse_data<T, extents<N>, A, L>;

    //- Apply some transformation to all elements.
    //
    template<class FN>
    static constexpr void
    apply(mse_type& dst, ptrdiff_t i0, ptrdiff_t i1, FN fn)
    {
        for (ptrdiff_t i = i0;  i < i1;  ++i)
        {
            dst.m_elems[i] = fn(i);
        }
    }

    //- Copy elements from an unrelated engine type into a destination MSE.
    //
    template<class ET2>
    static inline constexpr void
    copy_engine(mse_type& dst, ET2 const& src)
    {
        apply(dst, 0, dst.m_size, [&src](ptrdiff_t i){ return src(i); });
    }

    //- Copy elements from a linear (1-D) initializer list into a destination MSE.
    //
    template<class U>
    static constexpr void
    copy_list(mse_type& dst, initializer_list<U> src)
    {
        transform(src.begin(), src.end(), dst.m_elems.begin(),
                    [](U const& u){  return static_cast<T>(u);  });
    }

    //- Fill the specified range of elements of an MSE with some value.
    //
    static inline constexpr void
    fill(mse_type& dst, ptrdiff_t i0, ptrdiff_t i1, T const& t)
    {
        apply(dst, i0, i1, [&t](ptrdiff_t){ return t; });
    }

    //- Move elements from a source MSE into a destination MSE.
    //
    static inline constexpr void
    move_elements(mse_type& dst, mse_type& src, ptrdiff_t size)
    {
        apply(dst, 0, size, [&src](ptrdiff_t i){ return std::move(src.m_elems[i]); });
    }

    //- Swap the internals of two MSE objects.
    //
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


//--------------------------------------------------------------------------------------------------
//  Partial Specialization:     mse_support<T, extents<R, C>, A, L>
//
//  Policy/traits type that performs element manipulation on behalof of mse_data<T, X, A, L>.
//
//  This partial specialization of mse_support provides services to the various partial
//  specializations of mse_data having the form mse_data<T, extents<R, C>, A, L>.  These
//  services are common to all of those partial specializations, so are consolidated into
//  one place (here).
//--------------------------------------------------------------------------------------------------
//
template<class T, ptrdiff_t R, ptrdiff_t C, class A, class L>
struct mse_support<mse_data<T, extents<R, C>, A, L>>  : public engine_support_base
{
    //- Important types.
    //
    using mse_type            = mse_data<T, extents<R, C>, A, L>;
    using dyn_extents         = extents<dynamic_extent, dynamic_extent>;
    using dyn_layout          = layout_stride<dynamic_extent, dynamic_extent>;
    using dyn_strides         = array<typename dyn_extents::index_type, 2>;
    using dyn_mapping         = typename dyn_layout::template mapping<dyn_extents>;
    using dyn_span_type       = basic_mdspan<T, dyn_extents, dyn_layout>;
    using const_dyn_span_type = basic_mdspan<T const, dyn_extents, dyn_layout>;

    //- Support for dynamic mdspan.
    //
    static inline constexpr dyn_span_type
    make_dyn_mdspan(mse_type& rep)
    {
        if constexpr (is_same_v<L, row_major>)
        {
            dyn_extents     extents(rep.m_rows, rep.m_cols);
            dyn_strides     strides{rep.m_colcap, 1};
            dyn_mapping     mapping(extents, strides);

            return dyn_span_type(rep.m_elems.data(), mapping);
        }
        else
        {
            dyn_extents     extents(rep.m_rows, rep.m_cols);
            dyn_strides     strides{1, rep.m_rowcap};
            dyn_mapping     mapping(extents, strides);

            return dyn_span_type(rep.m_elems.data(), mapping);
        }
    }

    static inline constexpr const_dyn_span_type
    make_const_dyn_mdspan(mse_type const& rep)
    {
        if constexpr (is_same_v<L, row_major>)
        {
            dyn_extents     extents(rep.m_rows, rep.m_cols);
            dyn_strides     strides{rep.m_colcap, 1};
            dyn_mapping     mapping(extents, strides);

            return const_dyn_span_type(rep.m_elems.data(), mapping);
        }
        else
        {
            dyn_extents     extents(rep.m_rows, rep.m_cols);
            dyn_strides     strides{1, rep.m_rowcap};
            dyn_mapping     mapping(extents, strides);

            return const_dyn_span_type(rep.m_elems.data(), mapping);
        }
    }

    //- Look up an element via indices.
    //
    static inline constexpr T&
    element(mse_type& dst, ptrdiff_t i, ptrdiff_t j)
    {
        if constexpr (mse_type::is_row_major)
            return dst.m_elems[i*dst.m_colcap + j];
        else
            return dst.m_elems[i + j*dst.m_rowcap];
    }

    //- Apply a transformation to all elements, with the order governed by the elements' layout.
    //
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

    //- Copy elements from an unrelated engine type into a destination MSE.
    //
    template<class ET>
    static inline constexpr void
    copy_engine(mse_type& dst, ET const& src)
    {
        apply(dst, 0, 0, dst.m_rows, dst.m_cols,
                [&src](ptrdiff_t i, ptrdiff_t j){  return static_cast<T>(src(i, j));  });
    }

    //- Copy elements from a linear (1-D) initializer list into a destination MSE.
    //
    template<class U>
    static constexpr void
    copy_list(mse_type& dst, initializer_list<U> src)
    {
        transform(src.begin(), src.end(), dst.m_elems.begin(),
                    [](U const& u){  return static_cast<T>(u);  });
    }

    //- Copy elements from a 2-D initializer list into a destination MSE.
    //
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

    //- Fill the specified range of columns of an MSE with some value.
    //
    static inline constexpr void
    fill_columns(mse_type& dst, ptrdiff_t c0, ptrdiff_t c1, T const& t)
    {
        apply(dst, 0, c0, dst.m_rows, c1,
                [&t](ptrdiff_t, ptrdiff_t){  return t;  });
    }

    //- Fill the specified range of rows of an MSE with some value.
    //
    static inline constexpr void
    fill_rows(mse_type& dst, ptrdiff_t r0, ptrdiff_t r1, T const& t)
    {
        apply(dst, r0, 0, r1, dst.m_cols,
                [&t](ptrdiff_t, ptrdiff_t){  return t;  });
    }

    //- Move elements from a source MSE into a destination MSE.
    //
    static inline constexpr void
    move_elements(mse_type& dst, mse_type& src, ptrdiff_t rows, ptrdiff_t cols)
    {
        apply(dst, 0, 0, rows, cols,
                [&src](ptrdiff_t i, ptrdiff_t j){  return std::move(element(src, i, j));  });
    }

    //- Swap the internals of two MSE objects.
    //
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

}       //- detail namespace
}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_MATRIX_STORAGE_ENGINE_SUPPORT_HPP_DEFINED
