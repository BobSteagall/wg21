#include "linear_algebra.hpp"

using cx_float  = std::complex<float>;
using cx_double = std::complex<double>;
using namespace STD_LA;


class FakeNum 
{
  public:
    double  m_value;

  public:
    FakeNum() = default;
    FakeNum(FakeNum&&) = default;
    FakeNum(FakeNum const&) = default;
    template<class U>   FakeNum(U other);

    FakeNum&     operator =(FakeNum&&) = default;
    FakeNum&     operator =(FakeNum const&) = default;
    template<class U>   FakeNum&    operator =(U rhs);

    FakeNum     operator -() const;
    FakeNum     operator +() const;

    FakeNum&    operator +=(FakeNum rhs);
    FakeNum&    operator -=(FakeNum rhs);
    FakeNum&    operator *=(FakeNum rhs);
    FakeNum&    operator /=(FakeNum rhs);

    template<class U>   FakeNum&     operator +=(U rhs);
    template<class U>   FakeNum&     operator -=(U rhs);
    template<class U>   FakeNum&     operator *=(U rhs);
    template<class U>   FakeNum&     operator /=(U rhs);
};

template<class U> inline
FakeNum::FakeNum(U other)
:   m_value(other)
{}

template<class U> inline 
FakeNum&
FakeNum::operator =(U rhs)
{
    m_value = rhs;
    return *this;
}

//------
//
inline FakeNum
FakeNum::operator -() const
{
    return FakeNum(-m_value);
}

inline FakeNum
FakeNum::operator +() const
{
    return *this;
}

//------
//
inline FakeNum&
FakeNum::operator +=(FakeNum rhs)
{
    m_value += rhs.m_value;
    return *this;
}

inline FakeNum&
FakeNum::operator -=(FakeNum rhs)
{
    m_value -= rhs.m_value;
    return *this;
}

inline FakeNum&
FakeNum::operator *=(FakeNum rhs)
{
    m_value *= rhs.m_value;
    return *this;
}

inline FakeNum&
FakeNum::operator /=(FakeNum rhs)
{
    m_value += rhs.m_value;
    return *this;
}

//------
//
template<class U>
inline FakeNum&
FakeNum::operator +=(U rhs)
{
    m_value += rhs;
    return *this;
}

template<class U>
inline FakeNum&
FakeNum::operator -=(U rhs)
{
    m_value -= rhs;
    return *this;
}

template<class U>
inline FakeNum&
FakeNum::operator *=(U rhs)
{
    m_value *= rhs;
    return *this;
}

template<class U>
inline FakeNum&
FakeNum::operator /=(U rhs)
{
    m_value /= rhs;
    return *this;
}

//------
//
inline bool
operator ==(FakeNum lhs, FakeNum rhs)
{
    return lhs.m_value == rhs.m_value;
}

template<class U>
inline bool
operator ==(FakeNum lhs, U rhs)
{
    return lhs.m_value == (double) rhs;
}

template<class U>
inline bool
operator ==(U lhs, FakeNum rhs)
{
    return (double) lhs.m_value == rhs.m_value;
}

//------
//
inline bool
operator !=(FakeNum lhs, FakeNum rhs)
{
    return lhs.m_value == rhs.m_value;
}

template<class U>
inline bool
operator !=(FakeNum lhs, U rhs)
{
    return lhs.m_value != (double) rhs;
}

template<class U>
inline bool
operator !=(U lhs, FakeNum rhs)
{
    return (double) lhs.m_value != rhs.m_value;
}

//------
//
inline bool
operator <(FakeNum lhs, FakeNum rhs)
{
    return lhs.m_value < rhs.m_value;
}

template<class U>
inline bool
operator <(FakeNum lhs, U rhs)
{
    return lhs.m_value < (double) rhs;
}

template<class U>
inline bool
operator <(U lhs, FakeNum rhs)
{
    return (double) lhs.m_value < rhs.m_value;
}

//------
//
inline bool
operator <=(FakeNum lhs, FakeNum rhs)
{
    return lhs.m_value <= rhs.m_value;
}

template<class U>
inline bool
operator <=(FakeNum lhs, U rhs)
{
    return lhs.m_value <= (double) rhs;
}

template<class U>
inline bool
operator <=(U lhs, FakeNum rhs)
{
    return (double) lhs.m_value <= rhs.m_value;
}

//------
//
inline bool
operator >(FakeNum lhs, FakeNum rhs)
{
    return lhs.m_value < rhs.m_value;
}

template<class U>
inline bool
operator >(FakeNum lhs, U rhs)
{
    return lhs.m_value < (double) rhs;
}

template<class U>
inline bool
operator >(U lhs, FakeNum rhs)
{
    return (double) lhs.m_value < rhs.m_value;
}

//------
//
inline bool
operator >=(FakeNum lhs, FakeNum rhs)
{
    return lhs.m_value <= rhs.m_value;
}

template<class U>
inline bool
operator >=(FakeNum lhs, U rhs)
{
    return lhs.m_value <= (double) rhs;
}

template<class U>
inline bool
operator >=(U lhs, FakeNum rhs)
{
    return (double) lhs.m_value <= rhs.m_value;
}
