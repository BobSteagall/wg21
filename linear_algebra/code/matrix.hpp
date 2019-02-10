#ifndef MATRIX_HPP_DEFINED
#define MATRIX_HPP_DEFINED

#include "matrix_engine_traits.hpp"

namespace std::la {

//- New trait to use with SFINAE to determine if an engine is resizable; if it is, then
//  something can be enabled.
//
template<class ET1, class ET2>
using enable_if_resizable_t =
    typename std::enable_if_t<is_same_v<ET1, ET2> && ET1::is_resizable::value, bool>;

//  template<class ET1, class ET2>
//  using enable_if_complex_t =
//      typename std::enable_if_t<is_same_v<ET1, ET2> && is_complex_v<typename ET1::element_type>, bool>;

//=================================================================================================
//  A column vector parametrized by an engine type.
//=================================================================================================
//
template<class ET, class OT>
class column_vector
{
  public:
    using engine_type    = ET;
    using element_type   = typename ET::element_type;
    using is_resizable   = typename ET::is_resizable;
    using size_tuple     = typename ET::size_tuple;
    using transpose_type = row_vector<matrix_transpose_engine<ET>>;
    using hermitian_type = std::conditional_t<is_complex_v<element_type>,
                                                  row_vector<ET, OT>,
                                                  transpose_type>;
    static_assert(is_matrix_element_v<element_type>);

  public:
    ~column_vector() = default;
    column_vector();
    column_vector(column_vector&&) = default;
    column_vector(column_vector const&) = default;
    template<class ET2>
    column_vector(column_vector<ET2> const& src);

    template<class ET2 = ET, enable_if_resizable_t<ET, ET2> = true>
    column_vector(size_t rows);
    template<class ET2 = ET, enable_if_resizable_t<ET, ET2> = true>
    column_vector(size_t rows, size_t rowcap);

    column_vector&  operator =(column_vector&&) = default;
    column_vector&  operator =(column_vector const&) = default;
    template<class ET2>
    column_vector&  operator =(column_vector<ET2> const& rhs);

    //- Const element access.
    //
    element_type        operator ()(size_t i) const;
    element_type const* data() const;

    //- Accessors.
    //
    size_t      columns() const noexcept;
    size_t      rows() const noexcept;
    size_tuple  size() const noexcept;

    size_t      column_capacity() const noexcept;
    size_t      row_capacity() const noexcept;
    size_tuple  capacity() const noexcept;

    //- Transpose and Hermitian.
    //
    transpose_type  t() const;
    hermitian_type  h() const;

    //- Mutable element access.
    //
    element_type    operator ()(size_t i);
    element_type*   data();

    //- Change capacity.
    //
    template<class ET2 = ET, enable_if_resizable_t<ET, ET2> = true>
    void    reserve(size_t rowcap);

    //- Change size.
    //
    template<class ET2 = ET, enable_if_resizable_t<ET, ET2> = true>
    void    resize(size_t rows);

    //- Change size and capacity in one shot.
    //
    template<class ET2 = ET, enable_if_resizable_t<ET, ET2> = true>
    void    resize(size_t rows, size_t rowcap);

    //- Row operations.
    //
    void    swap_rows(size_t i, size_t j);

  private:
    engine_type     m_engine;
};


//=================================================================================================
//  A row vector parametrized by an engine type.
//=================================================================================================
//
template<class ET, class OT>
class row_vector
{
  public:
    using engine_type    = ET;
    using element_type   = typename ET::element_type;
    using is_resizable   = typename ET::is_resizable;
    using size_tuple     = typename ET::size_tuple;
    using transpose_type = column_vector<matrix_transpose_engine<ET>>;
    using hermitian_type = std::conditional_t<is_complex_v<element_type>,
                                                  column_vector<ET, OT>,
                                                  transpose_type>;
    static_assert(is_matrix_element_v<element_type>);

  public:
    ~row_vector() = default;
    row_vector();
    row_vector(row_vector&&) = default;
    row_vector(row_vector const&) = default;
    template<class ET2>
    row_vector(row_vector<ET2> const& src);

    template<class ET2 = ET, enable_if_resizable_t<ET, ET2> = true>
    row_vector(size_t cols);
    template<class ET2 = ET, enable_if_resizable_t<ET, ET2> = true>
    row_vector(size_t cols, size_t colcap);

    row_vector&     operator =(row_vector&&) = default;
    row_vector&     operator =(row_vector const&) = default;
    template<class ET2>
    row_vector&     operator =(row_vector<ET2> const& rhs);

    //- Const element access.
    //
    element_type        operator ()(size_t i) const;
    element_type const* data() const;

    //- Accessors.
    //
    size_t      columns() const noexcept;
    size_t      rows() const noexcept;
    size_tuple  size() const noexcept;

    size_t      column_capacity() const noexcept;
    size_t      row_capacity() const noexcept;
    size_tuple  capacity() const noexcept;

    //- Transpose and Hermitian.
    //
    transpose_type  t() const;
    hermitian_type  h() const;

    //- Mutable element access.
    //
    element_type    operator ()(size_t i);
    element_type*   data();

    //- Change capacity.
    //
    template<class ET2 = ET, enable_if_resizable_t<ET, ET2> = true>
    void    reserve(size_t colcap);

    //- Change size.
    //
    template<class ET2 = ET, enable_if_resizable_t<ET, ET2> = true>
    void    resize(size_t cols);

    //- Change size and capacity in one shot.
    //
    template<class ET2 = ET, enable_if_resizable_t<ET, ET2> = true>
    void    resize(size_t cols, size_t colcap);

    //- column operations.
    //
    void    swap_columns(size_t i, size_t j);

  private:
    engine_type     m_engine;
};


//=================================================================================================
//  A matrix parametrized by an engine type.
//=================================================================================================
//
template<class ET, class OT>
class matrix
{
  public:
    using engine_type    = ET;
    using element_type   = typename ET::element_type;
    using is_resizable   = typename ET::is_resizable;
    using size_tuple     = typename ET::size_tuple;
    using transpose_type = matrix<matrix_transpose_engine<ET>>;
    using hermitian_type = std::conditional_t<is_complex_v<element_type>,
                                                  matrix<ET, OT>,
                                                  transpose_type>;
    static_assert(is_matrix_element_v<element_type>);

  public:
    ~matrix() = default;
    matrix();
    matrix(matrix&&) = default;
    matrix(matrix const&) = default;
    template<class ET2>
    matrix(matrix<ET2> const& src);

    template<class ET2 = ET, enable_if_resizable_t<ET, ET2> = true>
    matrix(size_tuple size);
    template<class ET2 = ET, enable_if_resizable_t<ET, ET2> = true>
    matrix(size_t rows, size_t cols);

    template<class ET2 = ET, enable_if_resizable_t<ET, ET2> = true>
    matrix(size_tuple size, size_tuple cap);
    template<class ET2 = ET, enable_if_resizable_t<ET, ET2> = true>
    matrix(size_t rows, size_t cols, size_t rowcap, size_t colcap);

    matrix&     operator =(matrix&&) = default;
    matrix&     operator =(matrix const&) = default;
    template<class ET2>
    matrix&     operator =(matrix<ET2> const& rhs);

    //- Const element access.
    //
    element_type        operator ()(size_t i, size_t j) const;
    element_type const* data() const;

    //- Accessors.
    //
    size_t      columns() const noexcept;
    size_t      rows() const noexcept;
    size_tuple  size() const noexcept;

    size_t      column_capacity() const noexcept;
    size_t      row_capacity() const noexcept;
    size_tuple  capacity() const noexcept;

    //- Transpose and Hermitian.
    //
    transpose_type  t() const;
    hermitian_type  h() const;

    //- Mutable element access.
    //
    element_type    operator ()(size_t i, size_t j);
    element_type*   data();

    //- Change capacity.
    //
    template<class ET2 = ET, enable_if_resizable_t<ET, ET2> = true>
    void    reserve(size_tuple cap);
    template<class ET2 = ET, enable_if_resizable_t<ET, ET2> = true>
    void    reserve(size_t rowcap, size_t colcap);

    //- Change size.
    //
    template<class ET2 = ET, enable_if_resizable_t<ET, ET2> = true>
    void    resize(size_tuple size);
    template<class ET2 = ET, enable_if_resizable_t<ET, ET2> = true>
    void    resize(size_t rows, size_t cols);

    //- Change size and capacity in one shot.
    //
    template<class ET2 = ET, enable_if_resizable_t<ET, ET2> = true>
    void    resize(size_tuple size, size_tuple cap);
    template<class ET2 = ET, enable_if_resizable_t<ET, ET2> = true>
    void    resize(size_t rows, size_t cols, size_t rowcap, size_t colcap);

    //- Row and column operations.
    //
    void    swap_columns(size_t i, size_t j);
    void    swap_rows(size_t i, size_t j);

  private:
    engine_type     m_engine;
};

}       //- std::la namespace
#endif  //- MATRIX_HPP_DEFINED
