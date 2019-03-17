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
};

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_FIXED_SIZE_ENGINES_HPP_DEFINED
