#ifndef TEST_NEW_NUMBER_HPP_DEFINED
#define TEST_NEW_NUMBER_HPP_DEFINED

class NewNum 
{
  public:
    double  m_value;

  public:
    NewNum();
    NewNum(NewNum&&) = default;
    NewNum(NewNum const&) = default;
    template<class U>   NewNum(U other);

    NewNum&     operator =(NewNum&&) = default;
    NewNum&     operator =(NewNum const&) = default;
    template<class U>   NewNum&     operator =(U rhs);

    NewNum     operator -() const;
    NewNum     operator +() const;

    NewNum&     operator +=(NewNum rhs);
    NewNum&     operator -=(NewNum rhs);
    NewNum&     operator *=(NewNum rhs);
    NewNum&     operator /=(NewNum rhs);

    template<class U>   NewNum&      operator +=(U rhs);
    template<class U>   NewNum&      operator -=(U rhs);
    template<class U>   NewNum&      operator *=(U rhs);
    template<class U>   NewNum&      operator /=(U rhs);
};

template<>
struct std::is_arithmetic<NewNum> : public std::true_type
{};

//------
//
inline
NewNum::NewNum()
:   m_value(0.)
{}

template<class U> inline
NewNum::NewNum(U other)
:   m_value(other)
{}

template<class U> inline 
NewNum& 
NewNum::operator =(U rhs)
{
    m_value = rhs;
    return *this;
}

//------
//
inline NewNum
NewNum::operator -() const
{
    return NewNum(-m_value);
}

inline NewNum
NewNum::operator +() const
{
    return *this;
}

//------
//
inline NewNum& 
NewNum::operator +=(NewNum rhs)
{
    m_value += rhs.m_value;
    return *this;
}

inline NewNum& 
NewNum::operator -=(NewNum rhs)
{
    m_value -= rhs.m_value;
    return *this;
}

inline NewNum& 
NewNum::operator *=(NewNum rhs)
{
    m_value *= rhs.m_value;
    return *this;
}

inline NewNum& 
NewNum::operator /=(NewNum rhs)
{
    m_value += rhs.m_value;
    return *this;
}

//------
//
template<class U>
inline NewNum& 
NewNum::operator +=(U rhs)
{
    m_value += (double) rhs;
    return *this;
}

template<class U>
inline NewNum& 
NewNum::operator -=(U rhs)
{
    m_value -= (double) rhs;
    return *this;
}

template<class U>
inline NewNum& 
NewNum::operator *=(U rhs)
{
    m_value *= (double) rhs;
    return *this;
}

template<class U>
inline NewNum& 
NewNum::operator /=(U rhs)
{
    m_value /= (double) rhs;
    return *this;
}

//------
//
inline bool
operator ==(NewNum lhs, NewNum rhs)
{
    return lhs.m_value == rhs.m_value;
}

template<class U>
inline bool
operator ==(NewNum lhs, U rhs)
{
    return lhs.m_value == (double) rhs;
}

template<class U>
inline bool
operator ==(U lhs, NewNum rhs)
{
    return (double) lhs == rhs.m_value;
}

//------
//
inline bool
operator !=(NewNum lhs, NewNum rhs)
{
    return lhs.m_value == rhs.m_value;
}

template<class U>
inline bool
operator !=(NewNum lhs, U rhs)
{
    return lhs.m_value != (double) rhs;
}

template<class U>
inline bool
operator !=(U lhs, NewNum rhs)
{
    return (double) lhs != rhs.m_value;
}

//------
//
inline bool
operator <(NewNum lhs, NewNum rhs)
{
    return lhs.m_value < rhs.m_value;
}

template<class U>
inline bool
operator <(NewNum lhs, U rhs)
{
    return lhs.m_value < (double) rhs;
}

template<class U>
inline bool
operator <(U lhs, NewNum rhs)
{
    return (double) lhs < rhs.m_value;
}

//------
//
inline bool
operator <=(NewNum lhs, NewNum rhs)
{
    return lhs.m_value <= rhs.m_value;
}

template<class U>
inline bool
operator <=(NewNum lhs, U rhs)
{
    return lhs.m_value <= (double) rhs;
}

template<class U>
inline bool
operator <=(U lhs, NewNum rhs)
{
    return (double) lhs <= rhs.m_value;
}

//------
//
inline bool
operator >(NewNum lhs, NewNum rhs)
{
    return lhs.m_value < rhs.m_value;
}

template<class U>
inline bool
operator >(NewNum lhs, U rhs)
{
    return lhs.m_value < (double) rhs;
}

template<class U>
inline bool
operator >(U lhs, NewNum rhs)
{
    return (double) lhs < rhs.m_value;
}

//------
//
inline bool
operator >=(NewNum lhs, NewNum rhs)
{
    return lhs.m_value <= rhs.m_value;
}

template<class U>
inline bool
operator >=(NewNum lhs, U rhs)
{
    return lhs.m_value <= (double) rhs;
}

template<class U>
inline bool
operator >=(U lhs, NewNum rhs)
{
    return (double) lhs <= rhs.m_value;
}

//------
//
inline NewNum
operator +(NewNum lhs, NewNum rhs)
{
    return NewNum(lhs.m_value + rhs.m_value);
}

template<class U>
inline NewNum
operator +(NewNum lhs, U rhs)
{
    return NewNum(lhs.m_value + (double) rhs);
}

template<class U>
inline NewNum
operator +(U lhs, NewNum rhs)
{
    return NewNum((double) lhs + rhs.m_value);
}

//------
//
inline NewNum
operator -(NewNum lhs, NewNum rhs)
{
    return NewNum(lhs.m_value - rhs.m_value);
}

template<class U>
inline NewNum
operator -(NewNum lhs, U rhs)
{
    return NewNum(lhs.m_value - (double) rhs);
}

template<class U>
inline NewNum
operator -(U lhs, NewNum rhs)
{
    return NewNum((double) lhs - rhs.m_value);
}

//------
//
inline NewNum
operator *(NewNum lhs, NewNum rhs)
{
    return NewNum(lhs.m_value * rhs.m_value);
}

template<class U>
inline NewNum
operator *(NewNum lhs, U rhs)
{
    return NewNum(lhs.m_value * (double) rhs);
}

template<class U>
inline NewNum
operator *(U lhs, NewNum rhs)
{
    return NewNum((double) lhs * rhs.m_value);
}

//------
//
inline NewNum
operator /(NewNum lhs, NewNum rhs)
{
    return NewNum(lhs.m_value / rhs.m_value);
}

template<class U>
inline NewNum
operator /(NewNum lhs, U rhs)
{
    return NewNum(lhs.m_value / (double) rhs);
}

template<class U>
inline NewNum
operator /(U lhs, NewNum rhs)
{
    return NewNum((double) lhs / rhs.m_value);
}


void fake_num_compile_test()
{
    NewNum p0;
    double  d0 = 1.0;
    float   f0 = 2.0f;

    NewNum p1(p0), p2(d0), p3(f0);

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

#endif  //- TEST_NEW_NUMBER_HPP_DEFINED
