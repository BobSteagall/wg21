#ifndef MATRIX_ENGINES_HPP_DEFINED
#define MATRIX_ENGINES_HPP_DEFINED

#include "matrix_element_traits.hpp"

//- Experimental namespace for test implementation
//
namespace STD_LA {
//=================================================================================================
//  Fixed-size, fixed-capacity matrix engine.
//=================================================================================================
//
template<class T, size_t R, size_t C>
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
    using size_tuple     = std::tuple<size_t, size_t>;

  public:
    fs_matrix_engine() = default;
    fs_matrix_engine(fs_matrix_engine&&) = default;
    fs_matrix_engine(fs_matrix_engine const&) = default;

    fs_matrix_engine&   operator =(fs_matrix_engine&&) = default;
    fs_matrix_engine&   operator =(fs_matrix_engine const&) = default;

    T           operator ()(size_t i) const;
    T           operator ()(size_t i, size_t j) const;
    T const*    data() const noexcept;

    size_t      columns() const noexcept;
    size_t      rows() const noexcept;
    size_tuple  size() const noexcept;

    size_t      column_capacity() const noexcept;
    size_t      row_capacity() const noexcept;
    size_tuple  capacity() const noexcept;

    T&      operator ()(size_t i);
    T&      operator ()(size_t i, size_t j);
    T*      data() noexcept;

    void    swap_columns(size_t i, size_t j);
    void    swap_rows(size_t i, size_t j);

  private:
    T       ma_elems[R*C];      //- For exposition; data buffer
    T*      mp_bias;            //- For exposition; bias pointer for 1-based indexing
};


//=================================================================================================
//  Dynamically-resizable matrix engine.
//=================================================================================================
//
template<class T, class AT>
class dr_matrix_engine
{
  public:
    using element_type   = T;
    using is_dense       = std::true_type;
    using is_rectangular = std::true_type;
    using is_resizable   = std::true_type;
    using is_row_major   = std::true_type;
    using size_tuple     = std::tuple<size_t, size_t>;

  public:
    dr_matrix_engine();
    dr_matrix_engine(dr_matrix_engine&&);
    dr_matrix_engine(dr_matrix_engine const&);
    dr_matrix_engine(size_tuple size);
    dr_matrix_engine(size_t rows, size_t cols);
    dr_matrix_engine(size_tuple size, size_tuple cap);
    dr_matrix_engine(size_t rows, size_t cols, size_t rowcap, size_t colcap);

    dr_matrix_engine&   operator =(dr_matrix_engine&&);
    dr_matrix_engine&   operator =(dr_matrix_engine const&);

    T           operator ()(size_t i) const;
    T           operator ()(size_t i, size_t j) const;
    T const*    data() const noexcept;

    size_t      columns() const noexcept;
    size_t      rows() const noexcept;
    size_tuple  size() const noexcept;

    size_t      column_capacity() const noexcept;
    size_t      row_capacity() const noexcept;
    size_tuple  capacity() const noexcept;

    T&      operator ()(size_t i);
    T&      operator ()(size_t i, size_t j);
    T*      data() noexcept;

    void    reserve(size_tuple cap);
    void    reserve(size_t rowcap, size_t colcap);

    void    resize(size_tuple size);
    void    resize(size_t rows, size_t cols);
    void    resize(size_tuple size, size_tuple cap);
    void    resize(size_t rows, size_t cols, size_t rowcap, size_t colcap);

    void    swap_columns(size_t i, size_t j);
    void    swap_rows(size_t i, size_t j);

  private:
    using pointer = typename std::allocator_traits<AT>::pointer;

    pointer     mp_elems;       //- For exposition; data buffer
    T*          mp_bias;        //- For exposition; bias pointer for 1-based indexing
    size_t      m_rows;
    size_t      m_cols;
    size_t      m_rowcap;
    size_t      m_colcap;
};


//=================================================================================================
//  Matrix transpose engine, meant to act as an rvalue-ish "view" in expressions, in order to
//  prevent unnecessary allocation and element copying.
//=================================================================================================
//
template<class ET>
class matrix_transpose_engine
{
  public:
    using engine_type    = ET;
    using element_type   = typename engine_type::element_type;
    using is_dense       = typename engine_type::is_dense;
    using is_rectangular = typename engine_type::is_rectangular;
    using is_resizable   = std::false_type;
    using is_row_major   = std::conditional_t<std::is_same_v<typename ET::is_row_major, std::true_type>,
                                                  std::false_type,
                                                  std::true_type>;
    using size_tuple     = typename engine_type::size_tuple;

  public:
    matrix_transpose_engine();
    matrix_transpose_engine(engine_type const& eng);
    matrix_transpose_engine(matrix_transpose_engine&&);
    matrix_transpose_engine(matrix_transpose_engine const&);

    matrix_transpose_engine&    operator =(matrix_transpose_engine&&);
    matrix_transpose_engine&    operator =(matrix_transpose_engine const&);

    element_type        operator ()(size_t i) const;
    element_type        operator ()(size_t i, size_t j) const;
    element_type const* data() const noexcept;

    size_t      columns() const noexcept;
    size_t      rows() const noexcept;
    size_tuple  size() const noexcept;

    size_t      column_capacity() const noexcept;
    size_t      row_capacity() const noexcept;
    size_tuple  capacity() const noexcept;

  private:
    engine_type*    mp_other;   //- For exposition; pointer to actual engine
};

}       //- STD_LA namespace
#endif  //- MATRIX_ENGINES_HPP_DEFINED
