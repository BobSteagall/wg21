#ifndef LINEAR_ALGEBRA_VIEW_BASED_ENGINES_HPP_DEFINED
#define LINEAR_ALGEBRA_VIEW_BASED_ENGINES_HPP_DEFINED

#include "element_promotion_traits.hpp"

//- Experimental namespace for test implementation
//
namespace STD_LA
{
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
    using is_row_major   = std::conditional_t<ET::is_row_major::value, std::false_type, std::true_type>;
    using size_tuple     = typename engine_type::size_tuple;

  public:
    matrix_transpose_engine();
    matrix_transpose_engine(engine_type const& eng);
    matrix_transpose_engine(matrix_transpose_engine&&);
    matrix_transpose_engine(matrix_transpose_engine const&);

    matrix_transpose_engine& operator =(matrix_transpose_engine&&);
    matrix_transpose_engine& operator =(matrix_transpose_engine const&);

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
    engine_type* mp_other;   //- For exposition; pointer to actual engine
};

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_VIEW_BASED_ENGINES_HPP_DEFINED
