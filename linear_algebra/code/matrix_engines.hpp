#ifndef MATRIX_ENGINES_HPP_DEFINED
#define MATRIX_ENGINES_HPP_DEFINED

#include <cstdint>
#include <complex>
#include <memory>
#include <tuple>
#include <type_traits>

//- Experimental namespace for test implementation
//
namespace std::la {

//=================================================================================================
//  Fixed-size matrix engine.
//=================================================================================================
//
template<class T, size_t R, size_t C>
class fs_matrix_engine
{
    static_assert(R >= 1);
    static_assert(C >= 1);

  public:
    using element_type      = T;
    using is_resizable_type = false_type;
    using size_tuple        = tuple<size_t, size_t>;

  public:
    fs_matrix_engine();
    fs_matrix_engine(fs_matrix_engine&&);
    fs_matrix_engine(fs_matrix_engine const&);

    fs_matrix_engine&   operator =(fs_matrix_engine&&);
    fs_matrix_engine&   operator =(fs_matrix_engine const&);

    T           operator ()(size_t i) const;
    T           operator ()(size_t i, size_t j) const;
    T const*    data() const;

    size_t      columns() const noexcept;
    size_t      rows() const noexcept;
    size_tuple  size() const noexcept;

    size_t      column_capacity() const noexcept;
    size_t      row_capacity() const noexcept;
    size_tuple  capacity() const noexcept;

    T&      operator ()(size_t i);
    T&      operator ()(size_t i, size_t j);
    T*      data();

    void    swap_columns(size_t i, size_t j);
    void    swap_rows(size_t i, size_t j);

  private:
    T       ma_elems[R*C];
    T*      mp_bias;
};


//=================================================================================================
//  Dynamically-resizable matrix engine.
//=================================================================================================
//
template<class T, class ALLOC = std::allocator<T>>
class dyn_matrix_engine
{
  public:
    using element_type      = T;
    using is_resizable_type = true_type;
    using size_tuple        = tuple<size_t, size_t>;

  public:
    dyn_matrix_engine();
    dyn_matrix_engine(dyn_matrix_engine&&);
    dyn_matrix_engine(dyn_matrix_engine const&);

    dyn_matrix_engine&  operator =(dyn_matrix_engine&&);
    dyn_matrix_engine&  operator =(dyn_matrix_engine const&);

    T           operator ()(size_t i) const;
    T           operator ()(size_t i, size_t j) const;
    T const*    data() const;

    size_t      columns() const noexcept;
    size_t      rows() const noexcept;
    size_tuple  size() const noexcept;

    size_t      column_capacity() const noexcept;
    size_t      row_capacity() const noexcept;
    size_tuple  capacity() const noexcept;

    T&      operator ()(size_t i);
    T&      operator ()(size_t i, size_t j);
    T*      data();

    void    reserve(size_tuple cap);
    void    reserve(size_t rowcap, size_t colcap);

    void    resize(size_tuple size);
    void    resize(size_t rows, size_t cols);
    void    resize(size_tuple size, size_tuple cap);
    void    resize(size_t rows, size_t cols, size_t rowcap, size_t colcap);

    void    swap_columns(size_t i, size_t j);
    void    swap_rows(size_t i, size_t j);

  private:
    using pointer = typename std::allocator_traits<ALLOC>::pointer;

    pointer     mp_elems;
    T*          mp_bias;
    size_t      m_rows;
    size_t      m_cols;
    size_t      m_rowcap;
    size_t      m_colcap;
};


//=================================================================================================
//  Matrix transpose engine, meant to act as an rvalue "view" in expressions, in order to prevent
//  unnecessary allocation/copying.
//=================================================================================================
//
template<class ENG>
class matrix_transpose_engine
{
  public:
    using engine_type       = ENG;
    using element_type      = typename engine_type::element_type;
    using is_resizable_type = false_type;
    using size_tuple        = typename engine_type::size_tuple;

  public:
    matrix_transpose_engine();
    matrix_transpose_engine(engine_type const& eng);
    matrix_transpose_engine(matrix_transpose_engine&&);
    matrix_transpose_engine(matrix_transpose_engine const&);

    matrix_transpose_engine&    operator =(matrix_transpose_engine&&);
    matrix_transpose_engine&    operator =(matrix_transpose_engine const&);

    element_type        operator ()(size_t i) const;
    element_type        operator ()(size_t i, size_t j) const;
    element_type const* data() const;

    size_t      columns() const noexcept;
    size_t      rows() const noexcept;
    size_tuple  size() const noexcept;

    size_t      column_capacity() const noexcept;
    size_t      row_capacity() const noexcept;
    size_tuple  capacity() const noexcept;

  private:
    engine_type*    mp_other;
};

}       //- std::la namespace
#endif  //- MATRIX_ENGINES_HPP_DEFINED
