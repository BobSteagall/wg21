//==================================================================================================
//  File:       vector_iterators.hpp
//
//  Summary:    This header defines external iterator types for vectors.
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_VECTOR_ITERATORS_HPP_DEFINED
#define LINEAR_ALGEBRA_VECTOR_ITERATORS_HPP_DEFINED

namespace STD_LA {
namespace detail {

template<class VT>  class vector_iterator;
template<class VT>  class vector_const_iterator;

//==================================================================================================
//  Very simple mutable iterator for vectors.
//==================================================================================================
//
template<class VT>
class vector_iterator
{
    using engine_type       = typename VT::engine_type;

  public:
    using iterator_category = random_access_iterator_tag;
    using vector_type       = VT;
    using element_type      = typename vector_type::element_type;
    using value_type        = remove_cv_t<element_type>;
    using pointer           = typename vector_type::pointer;
    using const_pointer     = typename vector_type::const_pointer;
    using reference         = typename vector_type::reference;
    using const_reference   = typename vector_type::const_reference;
    using difference_type   = typename vector_type::difference_type;
    using index_type         = typename vector_type::index_type;

  public:
    ~vector_iterator() noexcept = default;

    constexpr vector_iterator() noexcept;
    constexpr vector_iterator(vector_type& vec, index_type curr, index_type upper);
    constexpr vector_iterator(vector_iterator&&) noexcept = default;
    constexpr vector_iterator(vector_iterator const&) noexcept = default;

    constexpr vector_iterator&  operator =(vector_iterator&&) noexcept = default;
    constexpr vector_iterator&  operator =(vector_iterator const&) noexcept = default;

    constexpr pointer           operator ->() const;
    constexpr reference         operator  *() const;
    constexpr reference         operator [](index_type n) const;

    constexpr difference_type   operator -(vector_iterator const& p) const;
    constexpr vector_iterator   operator -(difference_type n) const;
    constexpr vector_iterator   operator +(difference_type n) const;

    constexpr vector_iterator&  operator ++();
    constexpr vector_iterator   operator ++(int);
    constexpr vector_iterator&  operator --();
    constexpr vector_iterator   operator --(int);
    constexpr vector_iterator&  operator +=(difference_type n);
    constexpr vector_iterator&  operator -=(difference_type n);

    //- Additional helper functions used to implement the comparison operators.
    //
    constexpr bool  equals(vector_iterator const& p) const;
    constexpr bool  greater_than(vector_iterator const& p) const;
    constexpr bool  less_than(vector_iterator const& p) const;

  private:
    friend VT;
    template<class VT2>             friend class vector_const_iterator;
    template<class VT2>             friend class vector_iterator;
    template<class ET2, class OT2>  friend class STD_LA::vector;

  private:
    vector_type*    mp_vector;
    index_type       m_curr;
    index_type       m_upper;

  private:
    constexpr vector_iterator(vector_type& vec);
    constexpr vector_iterator(vector_type* vec, index_type curr, index_type upper);
};

template<class VT> constexpr
vector_iterator<VT>::vector_iterator() noexcept
:   mp_vector(nullptr)
,   m_curr(0)
,   m_upper(0)
{}

template<class VT> constexpr
vector_iterator<VT>::vector_iterator(vector_type& vec)
:   mp_vector(&vec)
,   m_curr(0)
,   m_upper(static_cast<index_type>(vec.size()))
{}

template<class VT> constexpr
vector_iterator<VT>::vector_iterator(vector_type& vec, index_type curr, index_type upper)
:   mp_vector(&vec)
,   m_curr(curr)
,   m_upper(upper)
{}

template<class VT> constexpr
vector_iterator<VT>::vector_iterator(vector_type* p_eng, index_type curr, index_type upper)
:   mp_vector(p_eng)
,   m_curr(curr)
,   m_upper(upper)
{}

template<class VT> constexpr
typename vector_iterator<VT>::pointer
vector_iterator<VT>::operator ->() const
{
    return static_cast<pointer>(addressof((*mp_vector)(m_curr)));
}

template<class VT> constexpr
typename vector_iterator<VT>::reference
vector_iterator<VT>::operator *() const
{
    return (*mp_vector)(m_curr);
}

template<class VT> constexpr
typename vector_iterator<VT>::reference
vector_iterator<VT>::operator [](index_type n) const
{
    return (*mp_vector)(m_curr + n);
}

template<class VT> constexpr
typename vector_iterator<VT>::difference_type
vector_iterator<VT>::operator -(vector_iterator const& p) const
{
    return m_curr - p.m_curr;
}

template<class VT> constexpr
vector_iterator<VT>
vector_iterator<VT>::operator -(difference_type n) const
{
    return vector_iterator(mp_vector, m_curr - n, m_upper);
}

template<class VT> constexpr
vector_iterator<VT>
vector_iterator<VT>::operator +(difference_type n) const
{
    return vector_iterator(mp_vector, m_curr + n, m_upper);
}

template<class VT> constexpr
vector_iterator<VT>&
vector_iterator<VT>::operator ++()
{
    ++m_curr;
    return *this;
}

template<class VT> constexpr
vector_iterator<VT>
vector_iterator<VT>::operator ++(int)
{
    return vector_iterator(mp_vector, m_curr + 1, m_upper);
}

template<class VT> constexpr
vector_iterator<VT>&
vector_iterator<VT>::operator --()
{
    --m_curr;
    return *this;
}

template<class VT> constexpr
vector_iterator<VT>
vector_iterator<VT>::operator --(int)
{
    return vector_iterator(mp_vector, m_curr - 1, m_upper);
}

template<class VT> constexpr
vector_iterator<VT>&
vector_iterator<VT>::operator +=(difference_type n)
{
    m_curr += n;
    return *this;
}

template<class VT> constexpr
vector_iterator<VT>&
vector_iterator<VT>::operator -=(difference_type n)
{
    m_curr -= n;
    return *this;
}

template<class VT> constexpr
bool
vector_iterator<VT>::equals(vector_iterator const& p) const
{
    return mp_vector == p.mp_vector  &&  m_curr == p.m_curr;
}

template<class VT> constexpr
bool
vector_iterator<VT>::greater_than(vector_iterator const& p) const
{
    return mp_vector == p.mp_vector  &&  m_curr > p.m_curr;
}

template<class VT> constexpr
bool
vector_iterator<VT>::less_than(vector_iterator const& p) const
{
    return mp_vector == p.mp_vector  &&  m_curr < p.m_curr;
}

template<class VT> constexpr
bool
operator ==(vector_iterator<VT> const& lhs, vector_iterator<VT> const& rhs)
{
    return lhs.equals(rhs);
}

template<class VT> constexpr
bool
operator !=(vector_iterator<VT> const& lhs, vector_iterator<VT> const& rhs)
{
    return !lhs.equals(rhs);
}

template<class VT> constexpr
bool
operator <(vector_iterator<VT> const& lhs, vector_iterator<VT> const& rhs)
{
    return lhs.less_than(rhs);
}

template<class VT> constexpr
bool
operator <=(vector_iterator<VT> const& lhs, vector_iterator<VT> const& rhs)
{
    return !lhs.greater_than(rhs);
}

template<class VT> constexpr
bool
operator >(vector_iterator<VT> const& lhs, vector_iterator<VT> const& rhs)
{
    return lhs.greater_than(rhs);
}

template<class VT> constexpr
bool
operator >=(vector_iterator<VT> const& lhs, vector_iterator<VT> const& rhs)
{
    return !lhs.less_than(rhs);
}


//==================================================================================================
// Very simple const iterator for vector engines.
//==================================================================================================
//
template<class VT>
class vector_const_iterator
{
    using engine_type       = typename VT::engine_type;

  public:
    using iterator_category = random_access_iterator_tag;
    using vector_type       = VT;
    using element_type      = typename vector_type::element_type;
    using value_type        = remove_cv_t<element_type>;
    using pointer           = typename vector_type::const_pointer;
    using const_pointer     = typename vector_type::const_pointer;
    using reference         = typename vector_type::const_reference;
    using const_reference   = typename vector_type::const_reference;
    using difference_type   = typename vector_type::difference_type;
    using index_type         = typename vector_type::index_type;

  public:
    ~vector_const_iterator() noexcept = default;

    constexpr vector_const_iterator() noexcept;
    constexpr vector_const_iterator(vector_type const& vec, index_type curr, index_type upper);
    constexpr vector_const_iterator(vector_const_iterator&&) noexcept = default;
    constexpr vector_const_iterator(vector_const_iterator const&) noexcept = default;

    constexpr vector_const_iterator&    operator =(vector_const_iterator&&) noexcept = default;
    constexpr vector_const_iterator&    operator =(vector_const_iterator const&) noexcept = default;

    constexpr const_pointer             operator ->() const;
    constexpr const_reference           operator  *() const;
    constexpr const_reference           operator [](index_type n) const;

    constexpr difference_type           operator -(vector_const_iterator const& p) const;
    constexpr vector_const_iterator     operator -(difference_type n) const;
    constexpr vector_const_iterator     operator +(difference_type n) const;

    constexpr vector_const_iterator&    operator ++();
    constexpr vector_const_iterator     operator ++(int);
    constexpr vector_const_iterator&    operator --();
    constexpr vector_const_iterator     operator --(int);
    constexpr vector_const_iterator&    operator +=(difference_type n);
    constexpr vector_const_iterator&    operator -=(difference_type n);

    //- Additional helper functions used to implement the comparison operators.
    //
    constexpr bool  equals(vector_const_iterator const& p) const;
    constexpr bool  greater_than(vector_const_iterator const& p) const;
    constexpr bool  less_than(vector_const_iterator const& p) const;

  private:
    friend VT;
    template<class VT2>             friend class vector_const_iterator;
    template<class VT2>             friend class vector_iterator;
    template<class ET2, class OT2>  friend class STD_LA::vector;

  private:
    vector_type const*  mp_vector;
    index_type           m_curr;
    index_type           m_upper;

  private:
    constexpr vector_const_iterator(vector_type const& vec);
    constexpr vector_const_iterator(vector_type const* vec, index_type curr, index_type upper);
    constexpr vector_const_iterator(vector_iterator<VT> const& p);
};

template<class VT> constexpr
vector_const_iterator<VT>::vector_const_iterator() noexcept
:   mp_vector(nullptr)
,   m_curr(0)
,   m_upper(0)
{}

template<class VT> constexpr
vector_const_iterator<VT>::vector_const_iterator(vector_type const& vec)
:   mp_vector(&vec)
,   m_curr(0)
,   m_upper(static_cast<index_type>(vec.size()))
{}

template<class VT> constexpr
vector_const_iterator<VT>::vector_const_iterator
(vector_type const& vec, index_type curr, index_type upper)
:   mp_vector(&vec)
,   m_curr(curr)
,   m_upper(upper)
{}

template<class VT> constexpr
vector_const_iterator<VT>::vector_const_iterator
(vector_type const* p_eng, index_type curr, index_type upper)
:   mp_vector(p_eng)
,   m_curr(curr)
,   m_upper(upper)
{}

template<class VT> constexpr
vector_const_iterator<VT>::vector_const_iterator(vector_iterator<VT> const& p)
:   mp_vector(p.mp_vector)
,   m_curr(p.m_curr)
,   m_upper(p.m_upper)
{}

template<class VT> constexpr
typename vector_const_iterator<VT>::pointer
vector_const_iterator<VT>::operator ->() const
{
    return static_cast<pointer>(addressof((*mp_vector)(m_curr)));
}

template<class VT> constexpr
typename vector_const_iterator<VT>::reference
vector_const_iterator<VT>::operator *() const
{
    return (*mp_vector)(m_curr);
}

template<class VT> constexpr
typename vector_const_iterator<VT>::reference
vector_const_iterator<VT>::operator [](index_type n) const
{
    return (*mp_vector)(m_curr + n);
}

template<class VT> constexpr
typename vector_const_iterator<VT>::difference_type
vector_const_iterator<VT>::operator -(vector_const_iterator const& p) const
{
    return m_curr - p.m_curr;
}

template<class VT> constexpr
vector_const_iterator<VT>
vector_const_iterator<VT>::operator -(difference_type n) const
{
    return vector_const_iterator(mp_vector, m_curr - n, m_upper);
}

template<class VT> constexpr
vector_const_iterator<VT>
vector_const_iterator<VT>::operator +(difference_type n) const
{
    return vector_const_iterator(mp_vector, m_curr + n, m_upper);
}

template<class VT> constexpr
vector_const_iterator<VT>&
vector_const_iterator<VT>::operator ++()
{
    ++m_curr;
    return *this;
}

template<class VT> constexpr
vector_const_iterator<VT>
vector_const_iterator<VT>::operator ++(int)
{
    return vector_const_iterator(mp_vector, m_curr + 1, m_upper);
}

template<class VT> constexpr
vector_const_iterator<VT>&
vector_const_iterator<VT>::operator --()
{
    --m_curr;
    return *this;
}

template<class VT> constexpr
vector_const_iterator<VT>
vector_const_iterator<VT>::operator --(int)
{
    return vector_const_iterator(mp_vector, m_curr - 1, m_upper);
}

template<class VT> constexpr
vector_const_iterator<VT>&
vector_const_iterator<VT>::operator +=(difference_type n)
{
    m_curr += n;
    return *this;
}

template<class VT> constexpr
vector_const_iterator<VT>&
vector_const_iterator<VT>::operator -=(difference_type n)
{
    m_curr -= n;
    return *this;
}

template<class VT> constexpr
bool
vector_const_iterator<VT>::equals(vector_const_iterator const& p) const
{
    return mp_vector == p.mp_vector  &&  m_curr == p.m_curr;
}

template<class VT> constexpr
bool
vector_const_iterator<VT>::greater_than(vector_const_iterator const& p) const
{
    return mp_vector == p.mp_vector  &&  m_curr > p.m_curr;
}

template<class VT> constexpr
bool
vector_const_iterator<VT>::less_than(vector_const_iterator const& p) const
{
    return mp_vector == p.mp_vector  &&  m_curr < p.m_curr;
}

template<class VT> constexpr
bool
operator ==(vector_const_iterator<VT> const& lhs, vector_const_iterator<VT> const& rhs)
{
    return lhs.equals(rhs);
}

template<class VT> constexpr
bool
operator !=(vector_const_iterator<VT> const& lhs, vector_const_iterator<VT> const& rhs)
{
    return !lhs.equals(rhs);
}

template<class VT> constexpr
bool
operator <(vector_const_iterator<VT> const& lhs, vector_const_iterator<VT> const& rhs)
{
    return lhs.less_than(rhs);
}

template<class VT> constexpr
bool
operator <=(vector_const_iterator<VT> const& lhs, vector_const_iterator<VT> const& rhs)
{
    return !lhs.greater_than(rhs);
}

template<class VT> constexpr
bool
operator >(vector_const_iterator<VT> const& lhs, vector_const_iterator<VT> const& rhs)
{
    return lhs.greater_than(rhs);
}

template<class VT> constexpr
bool
operator >=(vector_const_iterator<VT> const& lhs, vector_const_iterator<VT> const& rhs)
{
    return !lhs.less_than(rhs);
}

}       //- detail namespace
}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_VECTOR_ITERATORS_HPP_DEFINED
