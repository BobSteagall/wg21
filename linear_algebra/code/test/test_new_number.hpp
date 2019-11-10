#ifndef TEST_NEW_NUMBER_HPP_DEFINED
#define TEST_NEW_NUMBER_HPP_DEFINED

#ifdef __clang__
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wunused-comparison"
#endif

class new_num
{
  public:
    double  m_value;

  public:
    new_num();
    new_num(new_num&&) = default;
    new_num(new_num const&) = default;
    template<class U>   new_num(U other);

    new_num&    operator =(new_num&&) = default;
    new_num&    operator =(new_num const&) = default;
    template<class U>   new_num&    operator =(U rhs);

    new_num     operator -() const;
    new_num     operator +() const;

    new_num&    operator +=(new_num rhs);
    new_num&    operator -=(new_num rhs);
    new_num&    operator *=(new_num rhs);
    new_num&    operator /=(new_num rhs);

    template<class U>   new_num&    operator +=(U rhs);
    template<class U>   new_num&    operator -=(U rhs);
    template<class U>   new_num&    operator *=(U rhs);
    template<class U>   new_num&    operator /=(U rhs);
};

template<>
struct std::is_arithmetic<new_num> : public std::true_type
{};

//------
//
inline
new_num::new_num()
:   m_value(0.0)
{}

template<class U> inline
new_num::new_num(U other)
:   m_value(other)
{}

template<class U> inline
new_num&
new_num::operator =(U rhs)
{
    m_value = rhs;
    return *this;
}

//------
//
inline new_num
new_num::operator -() const
{
    return new_num(-m_value);
}

inline new_num
new_num::operator +() const
{
    return *this;
}

//------
//
inline new_num&
new_num::operator +=(new_num rhs)
{
    m_value += rhs.m_value;
    return *this;
}

inline new_num&
new_num::operator -=(new_num rhs)
{
    m_value -= rhs.m_value;
    return *this;
}

inline new_num&
new_num::operator *=(new_num rhs)
{
    m_value *= rhs.m_value;
    return *this;
}

inline new_num&
new_num::operator /=(new_num rhs)
{
    m_value += rhs.m_value;
    return *this;
}

//------
//
template<class U>
inline new_num&
new_num::operator +=(U rhs)
{
    m_value += (double) rhs;
    return *this;
}

template<class U>
inline new_num&
new_num::operator -=(U rhs)
{
    m_value -= (double) rhs;
    return *this;
}

template<class U>
inline new_num&
new_num::operator *=(U rhs)
{
    m_value *= (double) rhs;
    return *this;
}

template<class U>
inline new_num&
new_num::operator /=(U rhs)
{
    m_value /= (double) rhs;
    return *this;
}

//------
//
inline bool
operator ==(new_num lhs, new_num rhs)
{
    return lhs.m_value == rhs.m_value;
}

template<class U>
inline bool
operator ==(new_num lhs, U rhs)
{
    return lhs.m_value == (double) rhs;
}

template<class U>
inline bool
operator ==(U lhs, new_num rhs)
{
    return (double) lhs == rhs.m_value;
}

//------
//
inline bool
operator !=(new_num lhs, new_num rhs)
{
    return lhs.m_value == rhs.m_value;
}

template<class U>
inline bool
operator !=(new_num lhs, U rhs)
{
    return lhs.m_value != (double) rhs;
}

template<class U>
inline bool
operator !=(U lhs, new_num rhs)
{
    return (double) lhs != rhs.m_value;
}

//------
//
inline bool
operator <(new_num lhs, new_num rhs)
{
    return lhs.m_value < rhs.m_value;
}

template<class U>
inline bool
operator <(new_num lhs, U rhs)
{
    return lhs.m_value < (double) rhs;
}

template<class U>
inline bool
operator <(U lhs, new_num rhs)
{
    return (double) lhs < rhs.m_value;
}

//------
//
inline bool
operator <=(new_num lhs, new_num rhs)
{
    return lhs.m_value <= rhs.m_value;
}

template<class U>
inline bool
operator <=(new_num lhs, U rhs)
{
    return lhs.m_value <= (double) rhs;
}

template<class U>
inline bool
operator <=(U lhs, new_num rhs)
{
    return (double) lhs <= rhs.m_value;
}

//------
//
inline bool
operator >(new_num lhs, new_num rhs)
{
    return lhs.m_value < rhs.m_value;
}

template<class U>
inline bool
operator >(new_num lhs, U rhs)
{
    return lhs.m_value < (double) rhs;
}

template<class U>
inline bool
operator >(U lhs, new_num rhs)
{
    return (double) lhs < rhs.m_value;
}

//------
//
inline bool
operator >=(new_num lhs, new_num rhs)
{
    return lhs.m_value <= rhs.m_value;
}

template<class U>
inline bool
operator >=(new_num lhs, U rhs)
{
    return lhs.m_value <= (double) rhs;
}

template<class U>
inline bool
operator >=(U lhs, new_num rhs)
{
    return (double) lhs <= rhs.m_value;
}

//------
//
inline new_num
operator +(new_num lhs, new_num rhs)
{
    return new_num(lhs.m_value + rhs.m_value);
}

template<class U>
inline new_num
operator +(new_num lhs, U rhs)
{
    return new_num(lhs.m_value + (double) rhs);
}

template<class U>
inline new_num
operator +(U lhs, new_num rhs)
{
    return new_num((double) lhs + rhs.m_value);
}

//------
//
inline new_num
operator -(new_num lhs, new_num rhs)
{
    return new_num(lhs.m_value - rhs.m_value);
}

template<class U>
inline new_num
operator -(new_num lhs, U rhs)
{
    return new_num(lhs.m_value - (double) rhs);
}

template<class U>
inline new_num
operator -(U lhs, new_num rhs)
{
    return new_num((double) lhs - rhs.m_value);
}

//------
//
inline new_num
operator *(new_num lhs, new_num rhs)
{
    return new_num(lhs.m_value * rhs.m_value);
}

template<class U>
inline std::enable_if_t<std::is_arithmetic_v<U>, new_num>
operator *(new_num lhs, U rhs)
{
    return new_num(lhs.m_value * (double) rhs);
}

template<class U>
inline std::enable_if_t<std::is_arithmetic_v<U>, new_num>
operator *(U lhs, new_num rhs)
{
    return new_num((double) lhs * rhs.m_value);
}

//------
//
inline new_num
operator /(new_num lhs, new_num rhs)
{
    return new_num(lhs.m_value / rhs.m_value);
}

template<class U>
inline new_num
operator /(new_num lhs, U rhs)
{
    return new_num(lhs.m_value / (double) rhs);
}

template<class U>
inline new_num
operator /(U lhs, new_num rhs)
{
    return new_num((double) lhs / rhs.m_value);
}

inline void
new_num_compile_test()
{
    new_num p0;
    double  d0 = 1.0;
    float   f0 = 2.0f;

    new_num p1(p0), p2(d0), p3(f0);

    p1 = p2;
    p1 = d0;
    p1 = f0;

    p1 += p2;
    p1 += d0;
    p1 += f0;
    p1 -= p3;
    p1 -= d0;
    p1 -= f0;
    p1 *= p2;
    p1 *= d0;
    p1 *= f0;
    p1 /= p2;
    p1 /= d0;
    p1 /= f0;

    p1 == p2;
    p1 != p2;
    p1 <  p2;
    p1 <= p2;
    p1 >  p2;
    p1 >= p2;

    p1 == d0;
    p1 != d0;
    p1 <  d0;
    p1 <= d0;
    p1 >  d0;
    p1 >= d0;

    d0 == p2;
    d0 != p2;
    d0 <  p2;
    d0 <= p2;
    d0 >  p2;
    d0 >= p2;

    p1 + p2;
    p1 - p2;
    p1 * p2;
    p1 / p2;

    p1 + f0;
    p1 - f0;
    p1 * f0;
    p1 / f0;

    f0 + p2;
    f0 - p2;
    f0 * p2;
    f0 / p2;
}

template<>
struct STD_LA::number_traits<new_num>
{
    using is_field    = true_type;
    using is_nc_ring  = true_type;
    using is_ring     = true_type;
    using is_semiring = true_type;
};

#ifdef __clang__
    #pragma clang diagnostic pop
#endif
#endif  //- TEST_NEW_NUMBER_HPP_DEFINED
