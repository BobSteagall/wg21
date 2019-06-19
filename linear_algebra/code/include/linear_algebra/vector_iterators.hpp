//==================================================================================================
//  File:       row_column_views.hpp
//
//  Summary:    This header defines engines that act as views of rows and columns.
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_VECTOR_ITERATORS_HPP_DEFINED
#define LINEAR_ALGEBRA_VECTOR_ITERATORS_HPP_DEFINED

namespace STD_LA {
namespace detail {

template<class ET>  class vector_iterator;
template<class ET>  class vector_const_iterator;

//==================================================================================================
//  Basic mutable iterator for vector engines.
//==================================================================================================
//
template<class ET>
class vector_iterator
{
    static_assert(detail::is_vector_engine_v<ET>);

  public:
    using engine_type     = ET;
    using element_type    = typename engine_type::element_type;
    using value_type      = element_type;
    using reference       = typename engine_type::reference;
    using pointer         = typename engine_type::pointer;
    using const_reference = typename engine_type::const_reference;
    using const_pointer   = typename engine_type::const_pointer;
    using difference_type = typename engine_type::difference_type;
    using index_type      = typename engine_type::index_type;
    using size_type       = typename engine_type::size_type;

    using iterator_category = std::random_access_iterator_tag;

  public:
    ~vector_iterator() noexcept = default;

    constexpr vector_iterator() noexcept;
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
    friend ET;
    template<class ET2>             friend class vector_const_iterator;
    template<class ET2, class OT2>  friend class vector;

  private:
    engine_type*    mp_engine;
    index_type      m_curr;
    index_type      m_upper;

  private:
    constexpr vector_iterator(engine_type& eng);
    constexpr vector_iterator(engine_type* eng, index_type curr, index_type upper);
};

template<class ET> inline
constexpr
vector_iterator<ET>::vector_iterator() noexcept
:   mp_engine(nullptr)
,   m_curr(0)
,   m_upper(0)
{}

template<class ET> inline
constexpr
vector_iterator<ET>::vector_iterator(engine_type& eng)
:   mp_engine(&eng)
,   m_curr(0)
,   m_upper(static_cast<index_type>(eng.elements()))
{}

template<class ET> inline
constexpr
vector_iterator<ET>::vector_iterator(engine_type* p_eng, index_type curr, index_type upper)
:   mp_engine(p_eng)
,   m_curr(curr)
,   m_upper(upper)
{}

template<class ET> inline
constexpr typename vector_iterator<ET>::pointer
vector_iterator<ET>::operator ->() const
{
    return static_cast<pointer>(addressof((*mp_engine)(m_curr)));
}

template<class ET> inline
constexpr typename vector_iterator<ET>::reference
vector_iterator<ET>::operator *() const
{
    return (*mp_engine)(m_curr);
}

template<class ET> inline
constexpr typename vector_iterator<ET>::reference
vector_iterator<ET>::operator [](index_type n) const
{
    return (*mp_engine)(m_curr + n);
}

template<class ET> inline
constexpr typename vector_iterator<ET>::difference_type
vector_iterator<ET>::operator -(vector_iterator const& p) const
{
    return m_curr - p.m_curr;
}

template<class ET> inline
constexpr vector_iterator<ET>
vector_iterator<ET>::operator -(difference_type n) const
{
    return vector_iterator(mp_engine, m_curr - n, m_upper);
}

template<class ET> inline
constexpr vector_iterator<ET>
vector_iterator<ET>::operator +(difference_type n) const
{
    return vector_iterator(mp_engine, m_curr + n, m_upper);
}

template<class ET> inline
constexpr vector_iterator<ET>&
vector_iterator<ET>::operator ++()
{
    ++m_curr;
    return *this;
}

template<class ET> inline
constexpr vector_iterator<ET>
vector_iterator<ET>::operator ++(int)
{
    return vector_iterator(mp_engine, m_curr + 1, m_upper);
}

template<class ET> inline
constexpr vector_iterator<ET>&
vector_iterator<ET>::operator --()
{
    --m_curr;
    return *this;
}

template<class ET> inline
constexpr vector_iterator<ET>
vector_iterator<ET>::operator --(int)
{
    return vector_iterator(mp_engine, m_curr - 1, m_upper);
}

template<class ET> inline
constexpr vector_iterator<ET>&
vector_iterator<ET>::operator +=(difference_type n)
{
    m_curr += n;
    return *this;
}

template<class ET> inline
constexpr vector_iterator<ET>&
vector_iterator<ET>::operator -=(difference_type n)
{
    m_curr -= n;
    return *this;
}

template<class ET> inline
constexpr bool
vector_iterator<ET>::equals(vector_iterator const& p) const
{
    return mp_engine == p.mp_engine  &&  m_curr == p.m_curr;
}

template<class ET> inline
constexpr bool
vector_iterator<ET>::greater_than(vector_iterator const& p) const
{
    return mp_engine == p.mp_engine  &&  m_curr > p.m_curr;
}

template<class ET> inline
constexpr bool
vector_iterator<ET>::less_than(vector_iterator const& p) const
{
    return mp_engine == p.mp_engine  &&  m_curr < p.m_curr;
}

template<class ET> inline 
constexpr bool
operator ==(vector_iterator<ET> const& lhs, vector_iterator<ET> const& rhs)
{
    return lhs.equals(rhs);
}

template<class ET> inline 
constexpr bool
operator !=(vector_iterator<ET> const& lhs, vector_iterator<ET> const& rhs)
{
    return !lhs.equals(rhs);
}

template<class ET> inline 
constexpr bool
operator <(vector_iterator<ET> const& lhs, vector_iterator<ET> const& rhs)
{
    return lhs.less_than(rhs);
}

template<class ET> inline 
constexpr bool
operator <=(vector_iterator<ET> const& lhs, vector_iterator<ET> const& rhs)
{
    return !lhs.greater_than(rhs);
}

template<class ET> inline 
constexpr bool
operator >(vector_iterator<ET> const& lhs, vector_iterator<ET> const& rhs)
{
    return lhs.greater_than(rhs);
}

template<class ET> inline 
constexpr bool
operator >=(vector_iterator<ET> const& lhs, vector_iterator<ET> const& rhs)
{
    return !lhs.less_than(rhs);
}


//==================================================================================================
//  Basic mutable iterator for vector engines.
//==================================================================================================
//
template<class ET>
class vector_const_iterator
{
    static_assert(detail::is_vector_engine_v<ET>);

  public:
    using engine_type     = ET;
    using element_type    = typename engine_type::element_type;
    using value_type      = element_type;
    using reference       = typename engine_type::const_reference;
    using pointer         = typename engine_type::const_pointer;
    using const_reference = typename engine_type::const_reference;
    using const_pointer   = typename engine_type::const_pointer;
    using difference_type = typename engine_type::difference_type;
    using index_type      = typename engine_type::index_type;
    using size_type       = typename engine_type::size_type;

    using iterator_category = std::random_access_iterator_tag;

  public:
    ~vector_const_iterator() noexcept = default;

    constexpr vector_const_iterator() noexcept;
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
    friend ET;
    template<class ET2, class OT2>  friend class vector;

  private:
    engine_type const*  mp_engine;
    index_type          m_curr;
    index_type          m_upper;

  private:
    constexpr vector_const_iterator(engine_type const& eng);
    constexpr vector_const_iterator(engine_type const* eng, index_type curr, index_type upper);
    constexpr vector_const_iterator(vector_iterator<ET> const& p);
};

template<class ET> inline
constexpr
vector_const_iterator<ET>::vector_const_iterator() noexcept
:   mp_engine(nullptr)
,   m_curr(0)
,   m_upper(0)
{}

template<class ET> inline
constexpr
vector_const_iterator<ET>::vector_const_iterator(engine_type const& eng)
:   mp_engine(&eng)
,   m_curr(0)
,   m_upper(static_cast<index_type>(eng.elements()))
{}

template<class ET> inline
constexpr
vector_const_iterator<ET>::vector_const_iterator
(engine_type const* p_eng, index_type curr, index_type upper)
:   mp_engine(p_eng)
,   m_curr(curr)
,   m_upper(upper)
{}

template<class ET> inline
constexpr
vector_const_iterator<ET>::vector_const_iterator(vector_iterator<ET> const& p)
:   mp_engine(p.mp_engine)
,   m_curr(p.m_curr)
,   m_upper(p.m_upper)
{}

template<class ET> inline
constexpr typename vector_const_iterator<ET>::pointer
vector_const_iterator<ET>::operator ->() const
{
    return static_cast<pointer>(addressof((*mp_engine)(m_curr)));
}

template<class ET> inline
constexpr typename vector_const_iterator<ET>::reference
vector_const_iterator<ET>::operator *() const
{
    return (*mp_engine)(m_curr);
}

template<class ET> inline
constexpr typename vector_const_iterator<ET>::reference
vector_const_iterator<ET>::operator [](index_type n) const
{
    return (*mp_engine)(m_curr + n);
}

template<class ET> inline
constexpr typename vector_const_iterator<ET>::difference_type
vector_const_iterator<ET>::operator -(vector_const_iterator const& p) const
{
    return m_curr - p.m_curr;
}

template<class ET> inline
constexpr vector_const_iterator<ET>
vector_const_iterator<ET>::operator -(difference_type n) const
{
    return vector_const_iterator(mp_engine, m_curr - n, m_upper);
}

template<class ET> inline
constexpr vector_const_iterator<ET>
vector_const_iterator<ET>::operator +(difference_type n) const
{
    return vector_const_iterator(mp_engine, m_curr + n, m_upper);
}

template<class ET> inline
constexpr vector_const_iterator<ET>&
vector_const_iterator<ET>::operator ++()
{
    ++m_curr;
    return *this;
}

template<class ET> inline
constexpr vector_const_iterator<ET>
vector_const_iterator<ET>::operator ++(int)
{
    return vector_const_iterator(mp_engine, m_curr + 1, m_upper);
}

template<class ET> inline
constexpr vector_const_iterator<ET>&
vector_const_iterator<ET>::operator --()
{
    --m_curr;
    return *this;
}

template<class ET> inline
constexpr vector_const_iterator<ET>
vector_const_iterator<ET>::operator --(int)
{
    return vector_const_iterator(mp_engine, m_curr - 1, m_upper);
}

template<class ET> inline
constexpr vector_const_iterator<ET>&
vector_const_iterator<ET>::operator +=(difference_type n)
{
    m_curr += n;
    return *this;
}

template<class ET> inline
constexpr vector_const_iterator<ET>&
vector_const_iterator<ET>::operator -=(difference_type n)
{
    m_curr -= n;
    return *this;
}

template<class ET> inline
constexpr bool
vector_const_iterator<ET>::equals(vector_const_iterator const& p) const
{
    return mp_engine == p.mp_engine  &&  m_curr == p.m_curr;
}

template<class ET> inline
constexpr bool
vector_const_iterator<ET>::greater_than(vector_const_iterator const& p) const
{
    return mp_engine == p.mp_engine  &&  m_curr > p.m_curr;
}

template<class ET> inline
constexpr bool
vector_const_iterator<ET>::less_than(vector_const_iterator const& p) const
{
    return mp_engine == p.mp_engine  &&  m_curr < p.m_curr;
}

template<class ET> inline 
constexpr bool
operator ==(vector_const_iterator<ET> const& lhs, vector_const_iterator<ET> const& rhs)
{
    return lhs.equals(rhs);
}

template<class ET> inline 
constexpr bool
operator !=(vector_const_iterator<ET> const& lhs, vector_const_iterator<ET> const& rhs)
{
    return !lhs.equals(rhs);
}

template<class ET> inline 
constexpr bool
operator <(vector_const_iterator<ET> const& lhs, vector_const_iterator<ET> const& rhs)
{
    return lhs.less_than(rhs);
}

template<class ET> inline 
constexpr bool
operator <=(vector_const_iterator<ET> const& lhs, vector_const_iterator<ET> const& rhs)
{
    return !lhs.greater_than(rhs);
}

template<class ET> inline 
constexpr bool
operator >(vector_const_iterator<ET> const& lhs, vector_const_iterator<ET> const& rhs)
{
    return lhs.greater_than(rhs);
}

template<class ET> inline 
constexpr bool
operator >=(vector_const_iterator<ET> const& lhs, vector_const_iterator<ET> const& rhs)
{
    return !lhs.less_than(rhs);
}

}       //- detail namespace
}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_VECTOR_ITERATORS_HPP_DEFINED
