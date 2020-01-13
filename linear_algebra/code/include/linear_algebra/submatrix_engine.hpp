//==================================================================================================
//  File:       submatrix_engine.hpp
//
//  Summary:    This header defines an engine that acts as a "view" of a portion of a matrix.
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_SUBMATRIX_ENGINE_HPP_DEFINED
#define LINEAR_ALGEBRA_SUBMATRIX_ENGINE_HPP_DEFINED

namespace STD_LA {
//==================================================================================================
//  Sub-matrix engine, meant to act as a "view" of a portion of a matrix in expressions, in 
//  order to help avoid unnecessary allocation and element copying.
//==================================================================================================
//
template<class ET, class MCT>
class submatrix_engine
{
    static_assert(is_matrix_engine_v<ET>);
    static_assert(is_matrix_engine_tag<MCT>);

  public:
    //- Types
    //
    using engine_category = MCT;
    using element_type    = typename ET::element_type;
    using value_type      = typename ET::value_type;
    using pointer         = detail::noe_pointer_t<ET, MCT>;
    using const_pointer   = typename ET::const_pointer;
    using reference       = detail::noe_reference_t<ET, MCT>;
    using const_reference = typename ET::const_reference;
    using difference_type = typename ET::difference_type;
    using size_type       = typename ET::size_type;
    using size_tuple      = typename ET::size_tuple;

    //- Construct/copy/destroy
    //
    ~submatrix_engine() noexcept = default;

    constexpr submatrix_engine();
    constexpr submatrix_engine(submatrix_engine&&) noexcept = default;
    constexpr submatrix_engine(submatrix_engine const&) = default;

    constexpr submatrix_engine&     operator =(submatrix_engine&&) noexcept = default;
    constexpr submatrix_engine&     operator =(submatrix_engine const&) = default;

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
    constexpr void      swap(submatrix_engine& rhs);

  private:
    template<class ET2, class OT2>  friend class matrix;
    using referent_type = detail::noe_referent_t<ET, MCT>;

    referent_type*      mp_other;

    constexpr submatrix_engine(referent_type& eng);
};

//------------------------
//- Construct/copy/destroy
//
template<class ET, class MCT> constexpr 
submatrix_engine<ET, MCT>::submatrix_engine()
:   mp_other(nullptr)
{}

//----------
//- Capacity
//
template<class ET, class MCT> constexpr 
typename submatrix_engine<ET, MCT>::size_type
submatrix_engine<ET, MCT>::columns() const noexcept
{
    return mp_other->rows();
}

template<class ET, class MCT> constexpr 
typename submatrix_engine<ET, MCT>::size_type
submatrix_engine<ET, MCT>::rows() const noexcept
{
    return mp_other->columns();
}

template<class ET, class MCT> constexpr 
typename submatrix_engine<ET, MCT>::size_tuple
submatrix_engine<ET, MCT>::size() const noexcept
{
    return size_tuple(mp_other->columns(), mp_other->rows());
}

template<class ET, class MCT> constexpr 
typename submatrix_engine<ET, MCT>::size_type
submatrix_engine<ET, MCT>::column_capacity() const noexcept
{
    return mp_other->row_capacity();
}

template<class ET, class MCT> constexpr 
typename submatrix_engine<ET, MCT>::size_type
submatrix_engine<ET, MCT>::row_capacity() const noexcept
{
    return mp_other->column_capacity();
}

template<class ET, class MCT> constexpr 
typename submatrix_engine<ET, MCT>::size_tuple
submatrix_engine<ET, MCT>::capacity() const noexcept
{
    return size_tuple(mp_other->column_capacity(), mp_other->row_capacity());
}

//----------------
//- Element access
//
template<class ET, class MCT> constexpr 
typename submatrix_engine<ET, MCT>::reference
submatrix_engine<ET, MCT>::operator ()(size_type i, size_type j) const
{
    return (*mp_other)(i, j);
}

//-----------
//- Modifiers
//
template<class ET, class MCT> constexpr 
void
submatrix_engine<ET, MCT>::swap(submatrix_engine& rhs)
{
    std::swap(mp_other, rhs.mp_other);
}

template<class ET, class MCT> constexpr
submatrix_engine<ET, MCT>::submatrix_engine(referent_type& eng)
:   mp_other(&eng)
{}

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_SUBMATRIX_ENGINE_HPP_DEFINED
