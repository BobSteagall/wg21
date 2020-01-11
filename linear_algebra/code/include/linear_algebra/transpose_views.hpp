//==================================================================================================
//  File:       transpose_views.hpp
//
//  Summary:    This header defines engines that act as const views of other engines.
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_TRANSPOSE_VIEWS_HPP_DEFINED
#define LINEAR_ALGEBRA_TRANSPOSE_VIEWS_HPP_DEFINED

namespace STD_LA {
//==================================================================================================
//  Matrix transpose engine, meant to act as an rvalue-ish "view" in expressions, in order to
//  prevent unnecessary allocation and element copying.
//==================================================================================================
//
template<class ET, class MCT>
class matrix_transpose_engine
{
    static_assert(detail::is_matrix_engine_v<ET>);
    static_assert(detail::is_matrix_engine_tag<MCT>);

  public:
    //- Types
    //
    using engine_category = MCT;
    using element_type    = typename ET::element_type;
    using value_type      = typename ET::value_type;
    using pointer         = detail::view_pointer_t<ET, MCT>;
    using const_pointer   = typename ET::const_pointer;
    using reference       = detail::view_reference_t<ET, MCT>;
    using const_reference = typename ET::const_reference;
    using difference_type = typename ET::difference_type;
    using size_type       = typename ET::size_type;
    using size_tuple      = typename ET::size_tuple;

    //- Construct/copy/destroy
    //
    ~matrix_transpose_engine() noexcept = default;

    constexpr matrix_transpose_engine();
    constexpr matrix_transpose_engine(matrix_transpose_engine&&) noexcept = default;
    constexpr matrix_transpose_engine(matrix_transpose_engine const&) = default;

    constexpr matrix_transpose_engine&  operator =(matrix_transpose_engine&&) noexcept = default;
    constexpr matrix_transpose_engine&  operator =(matrix_transpose_engine const&) = default;

    //- Capacity
    //
    constexpr size_type     columns() const noexcept;
    constexpr size_type     rows() const noexcept;
    constexpr size_tuple    size() const noexcept;

    constexpr size_type     column_capacity() const noexcept;
    constexpr size_type     row_capacity() const noexcept;
    constexpr size_tuple    capacity() const noexcept;

    //- Element access
    //
    constexpr reference     operator ()(size_type i, size_type j) const;

    //- Modifiers
    //
    constexpr void      swap(matrix_transpose_engine& rhs);

  private:
    template<class ET2, class OT2>  friend class matrix;
    using referent_type = detail::view_referent_t<ET, MCT>;

    referent_type*      mp_other;

    constexpr matrix_transpose_engine(referent_type& eng);
};

//------------------------
//- Construct/copy/destroy
//
template<class ET, class MCT> constexpr 
matrix_transpose_engine<ET, MCT>::matrix_transpose_engine()
:   mp_other(nullptr)
{}

//----------
//- Capacity
//
template<class ET, class MCT> constexpr 
typename matrix_transpose_engine<ET, MCT>::size_type
matrix_transpose_engine<ET, MCT>::columns() const noexcept
{
    return mp_other->rows();
}

template<class ET, class MCT> constexpr 
typename matrix_transpose_engine<ET, MCT>::size_type
matrix_transpose_engine<ET, MCT>::rows() const noexcept
{
    return mp_other->columns();
}

template<class ET, class MCT> constexpr 
typename matrix_transpose_engine<ET, MCT>::size_tuple
matrix_transpose_engine<ET, MCT>::size() const noexcept
{
    return size_tuple(mp_other->columns(), mp_other->rows());
}

template<class ET, class MCT> constexpr 
typename matrix_transpose_engine<ET, MCT>::size_type
matrix_transpose_engine<ET, MCT>::column_capacity() const noexcept
{
    return mp_other->row_capacity();
}

template<class ET, class MCT> constexpr 
typename matrix_transpose_engine<ET, MCT>::size_type
matrix_transpose_engine<ET, MCT>::row_capacity() const noexcept
{
    return mp_other->column_capacity();
}

template<class ET, class MCT> constexpr 
typename matrix_transpose_engine<ET, MCT>::size_tuple
matrix_transpose_engine<ET, MCT>::capacity() const noexcept
{
    return size_tuple(mp_other->column_capacity(), mp_other->row_capacity());
}

//----------------
//- Element access
//
template<class ET, class MCT> constexpr 
typename matrix_transpose_engine<ET, MCT>::reference
matrix_transpose_engine<ET, MCT>::operator ()(size_type i, size_type j) const
{
    return (*mp_other)(j, i);
}

//-----------
//- Modifiers
//
template<class ET, class MCT> constexpr 
void
matrix_transpose_engine<ET, MCT>::swap(matrix_transpose_engine& rhs)
{
    std::swap(mp_other, rhs.mp_other);
}

template<class ET, class MCT> constexpr
matrix_transpose_engine<ET, MCT>::matrix_transpose_engine(referent_type& eng)
:   mp_other(&eng)
{}

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_TRANSPOSE_VIEWS_HPP_DEFINED
