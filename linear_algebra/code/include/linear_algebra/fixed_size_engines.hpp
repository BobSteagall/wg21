#ifndef LINEAR_ALGEBRA_FIXED_SIZE_ENGINES_HPP_DEFINED
#define LINEAR_ALGEBRA_FIXED_SIZE_ENGINES_HPP_DEFINED

#include "element_promotion_traits.hpp"

//- Experimental namespace for test implementation
//
namespace STD_LA {
//=================================================================================================
//  Fixed-size, fixed-capacity matrix engine.
//=================================================================================================
//
template<class T, int32_t N>
class fs_vector_engine
{
    static_assert(N >= 1);

  public:
    using element_type   = T;
    using is_dense       = std::true_type;
    using is_rectangular = std::true_type;
    using is_resizable   = std::false_type;
    using is_row_major   = std::true_type;
    using index_type     = int_fast32_t;
    using size_type      = int_fast32_t;

  public:
    fs_vector_engine() = default;
    fs_vector_engine(fs_vector_engine&&) = default;
    fs_vector_engine(fs_vector_engine const&) = default;

    fs_vector_engine&   operator =(fs_vector_engine&&) = default;
    fs_vector_engine&   operator =(fs_vector_engine const&) = default;

    T           operator ()(index_type i) const;
    T const*    data() const noexcept;

    size_type   capacity() const noexcept;
    size_type   elements() const noexcept;

    T&      operator ()(index_type i);
    T*      data() noexcept;

    void    swap_elements(index_type i, index_type j);

  private:
    T       ma_elems[N];      //- For exposition; data buffer
};

template<class T, int32_t N> inline 
T
fs_vector_engine<T,N>::operator ()(index_type i) const
{
    return ma_elems[i];
}

template<class T, int32_t N> inline 
T const*
fs_vector_engine<T,N>::data() const noexcept
{
    return &ma_elems[0];
}

template<class T, int32_t N> inline 
typename fs_vector_engine<T,N>::size_type
fs_vector_engine<T,N>::capacity() const noexcept
{
    return N;
}

template<class T, int32_t N> inline 
typename fs_vector_engine<T,N>::size_type
fs_vector_engine<T,N>::elements() const noexcept
{
    return N;
}

template<class T, int32_t N> inline 
T&
fs_vector_engine<T,N>::operator ()(index_type i)
{
    return ma_elems[i];
}

template<class T, int32_t N> inline 
T*
fs_vector_engine<T,N>::data() noexcept
{
    return &ma_elems[0];
}

template<class T, int32_t N> inline 
void
fs_vector_engine<T,N>::swap_elements(index_type, index_type)
{}


//=================================================================================================
//  Fixed-size, fixed-capacity matrix engine.
//=================================================================================================
//
template<class T, int32_t R, int32_t C>
class fs_matrix_engine
{
    static_assert(R >= 1);
    static_assert(C >= 1);

  public:
    using element_type   = T;
    using is_dense       = std::true_type;
    using is_rectangular = std::true_type;
    using is_resizable   = std::false_type;
    using is_row_major   = std::true_type;
    using index_type     = std::int_fast32_t;
    using size_type      = std::int_fast32_t;
    using size_tuple     = std::tuple<size_type, size_type>;

  public:
    fs_matrix_engine() = default;
    fs_matrix_engine(fs_matrix_engine&&) = default;
    fs_matrix_engine(fs_matrix_engine const&) = default;

    fs_matrix_engine&   operator =(fs_matrix_engine&&) = default;
    fs_matrix_engine&   operator =(fs_matrix_engine const&) = default;

    T           operator ()(index_type i, index_type j) const;
    T const*    data() const noexcept;

    size_type   columns() const noexcept;
    size_type   rows() const noexcept;
    size_tuple  size() const noexcept;

    size_type   column_capacity() const noexcept;
    size_type   row_capacity() const noexcept;
    size_tuple  capacity() const noexcept;

    T&      operator ()(index_type i, index_type j);
    T*      data() noexcept;

    void    swap_columns(index_type i, index_type j);
    void    swap_rows(index_type i, index_type j);

  private:
    T       ma_elems[R*C];      //- For exposition; data buffer
};

template<class T, int32_t R, int32_t C> inline 
T
fs_matrix_engine<T,R,C>::operator ()(index_type i, index_type j) const
{
    return ma_elems[i*C + j];
}

template<class T, int32_t R, int32_t C> inline 
T const*
fs_matrix_engine<T,R,C>::data() const noexcept
{
    return &ma_elems[0];
}

template<class T, int32_t R, int32_t C> inline 
typename fs_matrix_engine<T,R,C>::size_type
fs_matrix_engine<T,R,C>::columns() const noexcept
{
    return C;
}

template<class T, int32_t R, int32_t C> inline 
typename fs_matrix_engine<T,R,C>::size_type
fs_matrix_engine<T,R,C>::rows() const noexcept
{
    return R;
}

template<class T, int32_t R, int32_t C> inline 
typename fs_matrix_engine<T,R,C>::size_tuple
fs_matrix_engine<T,R,C>::size() const noexcept
{
    return size_tuple(R, C);
}

template<class T, int32_t R, int32_t C> inline 
typename fs_matrix_engine<T,R,C>::size_type
fs_matrix_engine<T,R,C>::column_capacity() const noexcept
{
    return C;
}

template<class T, int32_t R, int32_t C> inline 
typename fs_matrix_engine<T,R,C>::size_type
fs_matrix_engine<T,R,C>::row_capacity() const noexcept
{
    return R;
}

template<class T, int32_t R, int32_t C> inline 
typename fs_matrix_engine<T,R,C>::size_tuple
fs_matrix_engine<T,R,C>::capacity() const noexcept
{
    return size_tuple(R, C);
}

template<class T, int32_t R, int32_t C> inline 
T&
fs_matrix_engine<T,R,C>::operator ()(index_type i, index_type j)
{
    return ma_elems[i*C + j];
}

template<class T, int32_t R, int32_t C> inline 
T*
fs_matrix_engine<T,R,C>::data() noexcept
{
    return &ma_elems[0];
}

template<class T, int32_t R, int32_t C> inline 
void
fs_matrix_engine<T,R,C>::swap_columns(index_type, index_type)
{}

template<class T, int32_t R, int32_t C> inline 
void
fs_matrix_engine<T,R,C>::swap_rows(index_type, index_type)
{}

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_FIXED_SIZE_ENGINES_HPP_DEFINED
