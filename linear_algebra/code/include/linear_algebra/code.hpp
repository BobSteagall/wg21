//==================================================================================================
//  File:       forward_declarations.hpp
//
//  Summary:    This header contains forward declarations of the library's public API.  These
//              declarations are made in order to support the nice linear order of header
//              inclusion found in the driver header (linear_algebra.hpp).
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_FORWARD_DECLARATIONS_HPP_DEFINED
#define LINEAR_ALGEBRA_FORWARD_DECLARATIONS_HPP_DEFINED

namespace STD_LA {
USING_STD

//- Some tags for specifying how engines should behave.
//
using scalar_engine_tag           = integral_constant<int, 0>;

using const_vector_engine_tag     = integral_constant<int, 1>;
using mutable_vector_engine_tag   = integral_constant<int, 2>;
using resizable_vector_engine_tag = integral_constant<int, 3>;

using const_matrix_engine_tag     = integral_constant<int, 4>;
using mutable_matrix_engine_tag   = integral_constant<int, 5>;
using resizable_matrix_engine_tag = integral_constant<int, 6>;

//- A traits type that supplies important information about a numerical type.  Note that
//  this traits class is a customization point.
//
template<class T>   struct number_traits;

//- Traits for verifying appropriate matrix element types.
//
template<class T>   struct is_complex;
template<class T>   struct is_field;
template<class T>   struct is_nc_ring;
template<class T>   struct is_ring;
template<class T>   struct is_semi_ring;

template<class T>   struct is_matrix_element;

struct default_matrix_operations {};

//- Basic linear algebra engine types.
//
template<class ET>                      class matrix_column_view;
template<class ET>                      class matrix_row_view;
template<class ET>                      class matrix_transpose_view;
template<class T, int32_t N>            class fs_vector_engine;
template<class T, int32_t R, int32_t C> class fs_matrix_engine;
template<class T, class AT>             class dr_vector_engine;
template<class T, class AT>             class dr_matrix_engine;

//- The default element promotion, engine promotion, and arithmetic operation traits for
//  the four basic arithmetic operations.
//
struct matrix_operation_traits;

//- Primary math object types.
//
template<class ET, class OT=matrix_operation_traits> class vector;
template<class ET, class OT=matrix_operation_traits> class matrix;

//- Math object element promotion traits, per arithmetical operation.
//
template<class T1>              struct matrix_negation_element_traits;
template<class T1, class T2>    struct matrix_addition_element_traits;
template<class T1, class T2>    struct matrix_subtraction_element_traits;
template<class T1, class T2>    struct matrix_multiplication_element_traits;

//- Math object engine promotion traits, per arithmetical operation.
//
template<class OT, class ET1>               struct matrix_negation_engine_traits;
template<class OT, class ET1, class ET2>    struct matrix_addition_engine_traits;
template<class OT, class ET1, class ET2>    struct matrix_subtraction_engine_traits;
template<class OT, class ET1, class ET2>    struct matrix_multiplication_engine_traits;

//- Math object arithmetic traits.
//
template<class OT, class OP1>               struct matrix_negation_traits;
template<class OT, class OP1, class OP2>    struct matrix_addition_traits;
template<class OT, class OP1, class OP2>    struct matrix_subtraction_traits;
template<class OT, class OP1, class OP2>    struct matrix_multiplication_traits;

//- The default element promotion, engine promotion, and arithmetic operation traits for
//  the four basic arithmetic operations.
//
struct matrix_operation_traits;

//- A traits type that chooses between two operation traits types in the binary arithmetic
//  operators and free functions that act like binary operators (e.g., outer_product()).
//  Note that this traits class is a customization point.
//
template<class T1, class T2>    struct matrix_operation_traits_selector;

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_FORWARD_DECLARATIONS_HPP_DEFINED
//==================================================================================================
//  File:       number_traits.hpp
//
//  Summary:    This header defines several public traits types for reporting the properties of
//              numeric types.  It also provides the primary template for the number_traits
//              customization point, intended to be specialize-able by users.
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_NUMBER_TRAITS_HPP_DEFINED
#define LINEAR_ALGEBRA_NUMBER_TRAITS_HPP_DEFINED

namespace STD_LA {
namespace detail {
//--------------------------------------------------------------------------------------------------
//- Some common implementation details for builtin types
//--------------------------------------------------------------------------------------------------
//
struct builtin_number_traits
{
    using is_field     = true_type;
    using is_nc_ring   = true_type;
    using is_ring      = true_type;
    using is_semi_ring = true_type;
};

struct non_number_traits
{
    using is_field     = false_type;
    using is_nc_ring   = false_type;
    using is_ring      = false_type;
    using is_semi_ring = false_type;
};

template<class T>
using scalar_number_traits_helper_t =
    conditional_t<is_arithmetic_v<T>, builtin_number_traits, non_number_traits>;

}   //- detail namespace

//--------------------------------------------------------------------------------------------------
//- New traits type number_traits<T> to specify the properties modeled by a numerical type.
//--------------------------------------------------------------------------------------------------
//
template<class T>
struct number_traits : public detail::scalar_number_traits_helper_t<T>
{};

template<class T>
struct number_traits<complex<T>> : public number_traits<T>
{};

//--------------------------------------------------------------------------------------------------
//- New trait "is_field<T>" to detect if a numerical type models a field.
//--------------------------------------------------------------------------------------------------
//
template<class T>
struct is_field : public bool_constant<number_traits<T>::is_field::value>
{};

template<class T>
constexpr bool  is_field_v = is_field<T>::value;

//--------------------------------------------------------------------------------------------------
//- New trait "is_nc_ring<T>" to detect if a numerical type models a non_commutative ring.
//--------------------------------------------------------------------------------------------------
//
template<class T>
struct is_nc_ring : public bool_constant<number_traits<T>::is_nc_ring::value>
{};

template<class T>
constexpr bool  is_nc_ring_v = is_ring<T>::value;

//--------------------------------------------------------------------------------------------------
//- New trait "is_ring<T>" to detect if a numerical type models a ring.
//--------------------------------------------------------------------------------------------------
//
template<class T>
struct is_ring : public bool_constant<number_traits<T>::is_ring::value>
{};

template<class T>
constexpr bool  is_ring_v = is_ring<T>::value;

//--------------------------------------------------------------------------------------------------
//- New trait "is_semi_ring<T>" to detect if a type models a semiring.
//--------------------------------------------------------------------------------------------------
//
template<class T>
struct is_semi_ring : public bool_constant<number_traits<T>::is_semi_ring::value>
{};

template<class T>
constexpr bool  is_semi_ring_v = is_semi_ring<T>::value;

//--------------------------------------------------------------------------------------------------
//- New trait "is_complex<T>" to detect if a type is complex<T>.
//--------------------------------------------------------------------------------------------------
//
template<class T>
struct is_complex : public false_type
{};

template<class T>
struct is_complex<complex<T>> : public true_type
{};

template<class T>
constexpr bool  is_complex_v = is_complex<T>::value;

//--------------------------------------------------------------------------------------------------
//- New trait "is_matrix_element<T>" to detect if a type is a valid matrix/vector element.
//--------------------------------------------------------------------------------------------------
//
template<class T>
struct is_matrix_element : public bool_constant<is_arithmetic_v<T> || is_field_v<T>>
{};

template<class T>
constexpr bool  is_matrix_element_v = is_matrix_element<T>::value;

#if 0
//--------------------------------------------------------------------------------------------------
//- Unfortunately, in C++17 complex<T> only permits arithmetical expressions between
//  homogeneous element types: for example, expressions like complex<float>*complex<float>
//  and float*complex<float> are permitted, but double*complex<float> is not.  This macro
//  enforces this homogeneity in the traits types below.  Hopefully complex<T> will
//  support heterogeneous expressions at some point in the near future.
//--------------------------------------------------------------------------------------------------
//
#define ENFORCE_COMPLEX_OPERAND_HOMOGENEITY

#ifdef ENFORCE_COMPLEX_OPERAND_HOMOGENEITY
    #define ASSERT_COMPLEX_OPERAND_HOMOGENEITY(T1,T2)   static_assert(is_same_v<T1, T2>)
#else
    #define ASSERT_COMPLEX_OPERAND_HOMOGENEITY(T1,T2)
#endif

//--------------------------------------------------------------------------------------------------
//- Traits type matrix_element_promotion<T1,T2> to perform element type promotion for arithmetical
//  expressions, using the helper from above.
//--------------------------------------------------------------------------------------------------
//
template<class T1, class T2>
struct matrix_element_promotion
{
    using type = detail::matrix_element_promotion_helper_t<T1, T2>;
};

template<class T1, class T2>
struct matrix_element_promotion<T1, complex<T2>>
{
    ASSERT_COMPLEX_OPERAND_HOMOGENEITY(T1, T2);
    using type = complex<detail::matrix_element_promotion_helper_t<T1, T2>>;
};

template<class T1, class T2>
struct matrix_element_promotion<complex<T1>, T2>
{
    ASSERT_COMPLEX_OPERAND_HOMOGENEITY(T1, T2);
    using type = complex<detail::matrix_element_promotion_helper_t<T1, T2>>;
};

template<class T1, class T2>
struct matrix_element_promotion<complex<T1>, complex<T2>>
{
    ASSERT_COMPLEX_OPERAND_HOMOGENEITY(T1, T2);
    using type = complex<detail::matrix_element_promotion_helper_t<T1, T2>>;
};

template<class T1, class T2>
using matrix_element_promotion_t = typename matrix_element_promotion<T1, T2>::type;

#undef ASSERT_COMPLEX_OPERAND_HOMOGENEITY
#endif

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_NUMBER_TRAITS_HPP_DEFINED
//==================================================================================================
//  File:       engine_traits.hpp
//
//  Summary:    This header defines several private traits types, used by the vector and matrix
//              class templates, that report properties of an engine.
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_ENGINE_TRAITS_HPP_DEFINED
#define LINEAR_ALGEBRA_ENGINE_TRAITS_HPP_DEFINED

namespace STD_LA {
namespace detail {

struct row_column_tag {};

//--------------------------------------------------------------------------------------------------
//- Internally-used tag type to facilitate distinguishing elements from vectors/matrices.
//
template<class T>
struct element_tag
{
    using engine_category = scalar_engine_tag;
};

//--------------------------------------------------------------------------------------------------
//- Internally-used constexpr variables for describe an engine's category.
//
template<class ET>
constexpr bool  is_scalar_engine_v = (ET::engine_category::value == scalar_engine_tag::value);

template<class ET>
constexpr bool  is_vector_engine_v =  (ET::engine_category::value >= const_vector_engine_tag::value)
                                   && (ET::engine_category::value <  const_matrix_engine_tag::value);

template<typename ET>
constexpr bool  is_matrix_engine_v = (ET::engine_category::value >= const_matrix_engine_tag::value);


template<class ET1, class ET2>
constexpr bool  engines_match_v =  (is_vector_engine_v<ET1> && is_vector_engine_v<ET2>)
                                || (is_matrix_engine_v<ET1> && is_matrix_engine_v<ET2>);


//--------------------------------------------------------------------------------------------------
//- Tests to determine if an engine has mutable element indexing.
//
template<class ET>
constexpr bool  has_mutable_tag_v = ET::engine_category::value >= mutable_matrix_engine_tag::value;

template<class ET1, class ET2>
constexpr bool  is_mutable_engine_v = is_same_v<ET1, ET2> && has_mutable_tag_v<ET1>;

//------
//
template<class ET1, class ET2>
using enable_if_mutable = enable_if_t<is_mutable_engine_v<ET1, ET2>, bool>;

//--------------------------------------------------------------------------------------------------
//- Tests to determine if an engine is resizable.
//
template<class ET>
constexpr bool  has_resizable_tag_v = ET::engine_category::value >= resizable_matrix_engine_tag::value;

template<class ET>
constexpr bool  is_resizable_engine_v = ET::is_resizable::value;

//------
//
template<class ET1, class ET2>
using enable_if_resizable = enable_if_t<is_same_v<ET1, ET2> && is_resizable_engine_v<ET1>, bool>;


//--------------------------------------------------------------------------------------------------
//- Tests/facilites for fixed-size engines.
//
template<class ET>
constexpr bool  is_fixed_size_engine_v = ET::is_fixed_size::value;

template<class ET>
constexpr auto  engine_size_v = ET().size();

}       //- detail namespace

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_ENGINE_TRAITS_HPP_DEFINED
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
//==================================================================================================
//  File:       row_column_views.hpp
//
//  Summary:    This header defines engines that act as views of rows and columns.
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_ROW_COLUMN_VIEWS_HPP_DEFINED
#define LINEAR_ALGEBRA_ROW_COLUMN_VIEWS_HPP_DEFINED

namespace STD_LA {
//==================================================================================================
//  Matrix column view engine, meant to act as an rvalue-ish "view" of a matrix column in 
//  expressions, in order to prevent unnecessary allocation and element copying.
//==================================================================================================
//
template<class ET>
class matrix_column_view
{
    static_assert(detail::is_matrix_engine_v<ET>);

  public:
    using engine_type     = ET;
    using engine_category = const_vector_engine_tag;
    using element_type    = typename engine_type::element_type;
    using value_type      = typename engine_type::value_type;
    using reference       = typename engine_type::const_reference;
    using pointer         = typename engine_type::const_pointer;
    using const_reference = typename engine_type::const_reference;
    using const_pointer   = typename engine_type::const_pointer;
    using iterator        = detail::vector_const_iterator<matrix_column_view>;
    using const_iterator  = detail::vector_const_iterator<matrix_column_view>;
    using difference_type = typename engine_type::difference_type;
    using index_type      = typename engine_type::index_type;
    using size_type       = typename engine_type::size_type;

    using is_fixed_size   = typename engine_type::is_fixed_size;
    using is_resizable    = false_type;

    using is_column_major = typename engine_type::is_row_major;
    using is_dense        = typename engine_type::is_dense;
    using is_rectangular  = typename engine_type::is_rectangular;
    using is_row_major    = typename engine_type::is_column_major;

  public:
    constexpr matrix_column_view();
    constexpr matrix_column_view(engine_type const& eng, index_type col);
    constexpr matrix_column_view(matrix_column_view&&) noexcept = default;
    constexpr matrix_column_view(matrix_column_view const&) = default;

    constexpr matrix_column_view&   operator =(matrix_column_view&&) noexcept = default;
    constexpr matrix_column_view&   operator =(matrix_column_view const&) = default;

    constexpr const_reference   operator ()(index_type i) const;
    constexpr const_iterator    begin() const noexcept;
    constexpr const_iterator    end() const noexcept;
    constexpr size_type         capacity() const noexcept;
    constexpr size_type         elements() const noexcept;

    constexpr void      assign(matrix_column_view const& rhs);

  private:
    engine_type const*  mp_other;
    index_type          m_column;
};

template<class ET> inline 
constexpr
matrix_column_view<ET>::matrix_column_view()
:   mp_other(nullptr)
,   m_column(0)
{}

template<class ET> inline 
constexpr
matrix_column_view<ET>::matrix_column_view(engine_type const& eng, index_type col)
:   mp_other(&eng)
,   m_column(col)
{}

template<class ET> inline 
constexpr typename matrix_column_view<ET>::const_reference
matrix_column_view<ET>::operator ()(index_type i) const
{
    return (*mp_other)(i, m_column);
}

template<class ET> inline 
constexpr typename matrix_column_view<ET>::size_type
matrix_column_view<ET>::capacity() const noexcept
{
    return mp_other->rows();
}

template<class ET> inline 
constexpr typename matrix_column_view<ET>::size_type
matrix_column_view<ET>::elements() const noexcept
{
    return mp_other->rows();
}

template<class ET> inline 
constexpr typename matrix_column_view<ET>::const_iterator
matrix_column_view<ET>::begin() const noexcept
{
    return const_iterator(this, 0, mp_other->rows());
}

template<class ET> inline 
constexpr typename matrix_column_view<ET>::const_iterator
matrix_column_view<ET>::end() const noexcept
{
    return const_iterator(this, mp_other->rows(), mp_other->rows());
}

template<class ET> inline 
constexpr void
matrix_column_view<ET>::assign(matrix_column_view const& rhs)
{
    mp_other = rhs.mp_other;
    m_column = rhs.m_column;
}


//==================================================================================================
//  Matrix row view engine, meant to act as an rvalue-ish "view" of a matrix row in expressions, 
//  in order to prevent unnecessary allocation and element copying.
//==================================================================================================
//
template<class ET>
class matrix_row_view
{
    static_assert(detail::is_matrix_engine_v<ET>);

  public:
    using engine_type     = ET;
    using engine_category = const_vector_engine_tag;
    using element_type    = typename engine_type::element_type;
    using value_type      = typename engine_type::value_type;
    using reference       = typename engine_type::const_reference;
    using pointer         = typename engine_type::const_pointer;
    using const_reference = typename engine_type::const_reference;
    using const_pointer   = typename engine_type::const_pointer;
    using iterator        = detail::vector_const_iterator<matrix_row_view>;
    using const_iterator  = detail::vector_const_iterator<matrix_row_view>;
    using difference_type = typename engine_type::difference_type;
    using index_type      = typename engine_type::index_type;
    using size_type       = typename engine_type::size_type;

    using is_fixed_size   = typename engine_type::is_fixed_size;
    using is_resizable    = false_type;

    using is_column_major = typename engine_type::is_row_major;
    using is_dense        = typename engine_type::is_dense;
    using is_rectangular  = typename engine_type::is_rectangular;
    using is_row_major    = typename engine_type::is_column_major;

  public:
    constexpr matrix_row_view();
    constexpr matrix_row_view(engine_type const& eng, index_type row);
    constexpr matrix_row_view(matrix_row_view&&) noexcept = default;
    constexpr matrix_row_view(matrix_row_view const&) = default;

    constexpr matrix_row_view&  operator =(matrix_row_view&&) noexcept = default;
    constexpr matrix_row_view&  operator =(matrix_row_view const&) = default;

    constexpr const_reference   operator ()(index_type j) const;
    constexpr const_iterator    begin() const noexcept;
    constexpr const_iterator    end() const noexcept;
    constexpr size_type         capacity() const noexcept;
    constexpr size_type         elements() const noexcept;

    constexpr void      assign(matrix_row_view const& rhs);

  private:
    engine_type const*  mp_other;
    index_type          m_row;
};

template<class ET> inline 
constexpr
matrix_row_view<ET>::matrix_row_view()
:   mp_other(nullptr)
,   m_row(0)
{}

template<class ET> inline 
constexpr
matrix_row_view<ET>::matrix_row_view(engine_type const& eng, index_type row)
:   mp_other(&eng)
,   m_row(row)
{}

template<class ET> inline 
constexpr typename matrix_row_view<ET>::const_reference
matrix_row_view<ET>::operator ()(index_type j) const
{
    return (*mp_other)(m_row, j);
}

template<class ET> inline 
constexpr typename matrix_row_view<ET>::const_iterator
matrix_row_view<ET>::begin() const noexcept
{
    return const_iterator(this, 0, mp_other->columns());
}

template<class ET> inline 
constexpr typename matrix_row_view<ET>::const_iterator
matrix_row_view<ET>::end() const noexcept
{
    return const_iterator(this, mp_other->columns(), mp_other->columns());
}

template<class ET> inline 
constexpr typename matrix_row_view<ET>::size_type
matrix_row_view<ET>::capacity() const noexcept
{
    return mp_other->columns();
}

template<class ET> inline 
constexpr typename matrix_row_view<ET>::size_type
matrix_row_view<ET>::elements() const noexcept
{
    return mp_other->columns();
}

template<class ET> inline 
constexpr void
matrix_row_view<ET>::assign(matrix_row_view const& rhs)
{
    mp_other = rhs.mp_other;
    m_row    = rhs.m_row;
}

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_ROW_COLUMN_VIEWS_HPP_DEFINED
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
template<class ET>
class matrix_transpose_view
{
    static_assert(detail::is_matrix_engine_v<ET>);

  public:
    using engine_type     = ET;
    using engine_category = const_matrix_engine_tag;
    using element_type    = typename engine_type::element_type;
    using value_type      = typename engine_type::value_type;
    using reference       = typename engine_type::const_reference;
    using pointer         = typename engine_type::const_pointer;
    using const_reference = typename engine_type::const_reference;
    using const_pointer   = typename engine_type::const_pointer;
    using difference_type = typename engine_type::difference_type;
    using index_type      = typename engine_type::index_type;
    using size_type       = typename engine_type::size_type;
    using size_tuple      = typename engine_type::size_tuple;

    using is_fixed_size   = typename engine_type::is_fixed_size;
    using is_resizable    = false_type;

    using is_column_major = typename engine_type::is_row_major;
    using is_dense        = typename engine_type::is_dense;
    using is_rectangular  = typename engine_type::is_rectangular;
    using is_row_major    = typename engine_type::is_column_major;

    using column_view_type    = matrix_column_view<matrix_transpose_view>;
    using row_view_type       = matrix_row_view<matrix_transpose_view>;
    using transpose_view_type = matrix_transpose_view<matrix_transpose_view>;

  public:
    constexpr matrix_transpose_view();
    constexpr matrix_transpose_view(engine_type const& eng);
    constexpr matrix_transpose_view(matrix_transpose_view&&) noexcept = default;
    constexpr matrix_transpose_view(matrix_transpose_view const&) = default;

    constexpr matrix_transpose_view&    operator =(matrix_transpose_view&&) noexcept = default;
    constexpr matrix_transpose_view&    operator =(matrix_transpose_view const&) = default;

    constexpr const_reference   operator ()(index_type i, index_type j) const;
    constexpr const_pointer     data() const noexcept;

    constexpr size_type     columns() const noexcept;
    constexpr size_type     rows() const noexcept;
    constexpr size_tuple    size() const noexcept;

    constexpr size_type     column_capacity() const noexcept;
    constexpr size_type     row_capacity() const noexcept;
    constexpr size_tuple    capacity() const noexcept;

    constexpr void  assign(matrix_transpose_view const& rhs);

  private:
    engine_type const*  mp_other;
};

template<class ET> inline 
constexpr matrix_transpose_view<ET>::matrix_transpose_view()
:   mp_other(nullptr)
{}

template<class ET> inline 
constexpr
matrix_transpose_view<ET>::matrix_transpose_view(engine_type const& eng)
:   mp_other(&eng)
{}

template<class ET> inline 
constexpr typename matrix_transpose_view<ET>::const_reference
matrix_transpose_view<ET>::operator ()(index_type i, index_type j) const
{
    return (*mp_other)(j, i);
}

template<class ET> inline
constexpr typename matrix_transpose_view<ET>::const_pointer
matrix_transpose_view<ET>::data() const noexcept
{
    return mp_other->data();
}

template<class ET> inline
constexpr typename matrix_transpose_view<ET>::size_type
matrix_transpose_view<ET>::columns() const noexcept
{
    return mp_other->rows();
}

template<class ET> inline
constexpr typename matrix_transpose_view<ET>::size_type
matrix_transpose_view<ET>::rows() const noexcept
{
    return mp_other->columns();
}

template<class ET> inline
constexpr typename matrix_transpose_view<ET>::size_tuple
matrix_transpose_view<ET>::size() const noexcept
{
    return size_tuple(mp_other->columns(), mp_other->rows());
}

template<class ET> inline
constexpr typename matrix_transpose_view<ET>::size_type
matrix_transpose_view<ET>::column_capacity() const noexcept
{
    return mp_other->row_capacity();
}

template<class ET> inline
constexpr typename matrix_transpose_view<ET>::size_type
matrix_transpose_view<ET>::row_capacity() const noexcept
{
    return mp_other->column_capacity();
}

template<class ET> inline
constexpr typename matrix_transpose_view<ET>::size_tuple
matrix_transpose_view<ET>::capacity() const noexcept
{
    return size_tuple(mp_other->column_capacity(), mp_other->row_capacity());
}

template<class ET> inline
constexpr void
matrix_transpose_view<ET>::assign(matrix_transpose_view const& rhs)
{
    mp_other == rhs.mp_other;
}

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_TRANSPOSE_VIEWS_HPP_DEFINED
//==================================================================================================
//  File:       dynamic_engines.hpp
//
//  Summary:    This header defines dynamically-resizable vector and matrix engines.  In this
//              context, dynamically-resizable means that the row and column extents of such
//              objects are set at run-time, and thus unknown at compile-time.
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_DYNAMIC_ENGINES_HPP_DEFINED
#define LINEAR_ALGEBRA_DYNAMIC_ENGINES_HPP_DEFINED

namespace STD_LA {
namespace detail {
//--------------------------------------------------------------------------------------------------
//  Some private helper functions for allocating/deallocating the memory used by the dynamic
//  vector and matrix engines defined below.  Note that all memory thus allocated is default-
//  constructed.  This means that elements lying in (currently) unused capacity are also
//  initialized.
//--------------------------------------------------------------------------------------------------
//
template<class AT>
typename allocator_traits<AT>::pointer
allocate(AT& alloc, size_t n)
{
    auto    p_dst = allocator_traits<AT>::allocate(alloc, n);

    try
    {
        uninitialized_value_construct_n(p_dst, n);
    }
    catch (...)
    {
        allocator_traits<AT>::deallocate(alloc, p_dst, n);
        throw;
    }
    return p_dst;
}

//------
//
template<class AT>
typename allocator_traits<AT>::pointer
allocate(AT& alloc, size_t n, typename allocator_traits<AT>::const_pointer p_src)
{
    auto    p_dst = allocator_traits<AT>::allocate(alloc, n);

    try
    {
        uninitialized_copy_n(p_src, n, p_dst);
    }
    catch (...)
    {
        allocator_traits<AT>::deallocate(alloc, p_dst, n);
        throw;
    }
    return p_dst;
}

//------
//
template<class AT>
void
deallocate(AT& alloc, typename allocator_traits<AT>::pointer p_dst, size_t n) noexcept
{
    if (p_dst != nullptr)
    {
        destroy_n(p_dst, n);
        allocator_traits<AT>::deallocate(alloc, p_dst, n);
    }
}

}   //- detail namespace
//==================================================================================================
//  Dynamically-resizable matrix engine.
//==================================================================================================
//
template<class T, class AT>
class dr_vector_engine
{
    static_assert(is_matrix_element_v<T>);

  public:
    using engine_category = resizable_vector_engine_tag;
    using element_type    = T;
    using value_type      = T;
    using alloc_type      = AT;
    using reference       = T&;
    using pointer         = typename allocator_traits<AT>::pointer;
    using const_reference = T const&;
    using const_pointer   = typename allocator_traits<AT>::const_pointer;
    using iterator        = detail::vector_iterator<dr_vector_engine>;
    using const_iterator  = detail::vector_const_iterator<dr_vector_engine>;
    using difference_type = ptrdiff_t;
    using index_type      = ptrdiff_t;
    using size_type       = ptrdiff_t;

    using is_fixed_size   = false_type;
    using is_resizable    = true_type;

    using is_column_major = true_type;
    using is_dense        = true_type;
    using is_rectangular  = true_type;
    using is_row_major    = true_type;

  public:
    ~dr_vector_engine() noexcept;
    dr_vector_engine();
    dr_vector_engine(dr_vector_engine&&) noexcept;
    dr_vector_engine(dr_vector_engine const&);
    dr_vector_engine(size_type elems);
    dr_vector_engine(size_type elems, size_type elem_cap);

    dr_vector_engine& operator =(dr_vector_engine&&) noexcept;
    dr_vector_engine& operator =(dr_vector_engine const&);

    const_reference operator ()(index_type i) const;
    const_iterator  begin() const noexcept;
    const_iterator  end() const noexcept;

    size_type       elements() const noexcept;
    size_type       capacity() const noexcept;

    reference   operator ()(index_type i);
    iterator    begin() noexcept;
    iterator    end() noexcept;

    void    assign(dr_vector_engine const& rhs);
    template<class ET2>
    void    assign(ET2 const& rhs);

    void    reserve(size_type cap);
    void    resize(size_type elems);
    void    resize(size_type elems, size_type cap);
    void    swap(dr_vector_engine& rhs) noexcept;
    void    swap_elements(index_type i, index_type j);

  private:
    pointer     mp_elems;
    size_type   m_elems;
    size_type   m_elemcap;
    alloc_type  m_alloc;

    void    alloc_new(size_type elems, size_type cap);
    void    check_capacity(size_type cap);
    void    check_size(size_type elems);
    void    reshape(size_type elems, size_type cap);
};

template<class T, class AT> inline
dr_vector_engine<T,AT>::~dr_vector_engine() noexcept
{
    detail::deallocate(m_alloc, mp_elems, m_elemcap);
}

template<class T, class AT> inline
dr_vector_engine<T,AT>::dr_vector_engine()
:   mp_elems(nullptr)
,   m_elems(0)
,   m_elemcap(0)
,   m_alloc()
{}

template<class T, class AT> inline
dr_vector_engine<T,AT>::dr_vector_engine(dr_vector_engine&& rhs) noexcept
:   mp_elems(nullptr)
,   m_elems(0)
,   m_elemcap(0)
{
    rhs.swap(*this);
}

template<class T, class AT> inline
dr_vector_engine<T,AT>::dr_vector_engine(dr_vector_engine const& rhs)
:   mp_elems(nullptr)
,   m_elems(0)
,   m_elemcap(0)
,   m_alloc()
{
    assign(rhs);
}

template<class T, class AT> inline
dr_vector_engine<T,AT>::dr_vector_engine(size_type elems)
:   mp_elems(nullptr)
,   m_elems(0)
,   m_elemcap(0)
,   m_alloc()
{
    alloc_new(elems, elems);
}

template<class T, class AT> inline
dr_vector_engine<T,AT>::dr_vector_engine(size_type elems, size_type cap)
:   mp_elems(nullptr)
,   m_elems(0)
,   m_elemcap(0)
,   m_alloc()
{
    alloc_new(elems, cap);
}

template<class T, class AT> inline
dr_vector_engine<T,AT>&
dr_vector_engine<T,AT>::operator =(dr_vector_engine&& rhs) noexcept
{
    dr_vector_engine    tmp;
    tmp.swap(rhs);
    tmp.swap(*this);
    return *this;
}

template<class T, class AT> inline
dr_vector_engine<T,AT>&
dr_vector_engine<T,AT>::operator =(dr_vector_engine const& rhs)
{
    assign(rhs);
    return *this;
}

template<class T, class AT> inline
typename dr_vector_engine<T,AT>::const_reference
dr_vector_engine<T,AT>::operator ()(index_type i) const
{
    return mp_elems[i];
}

template<class T, class AT> inline
typename dr_vector_engine<T,AT>::const_iterator
dr_vector_engine<T,AT>::begin() const noexcept
{
    return const_iterator(this, 0, m_elemcap);
}

template<class T, class AT> inline
typename dr_vector_engine<T,AT>::const_iterator
dr_vector_engine<T,AT>::end() const noexcept
{
    return const_iterator(this, m_elemcap, m_elemcap);
}

template<class T, class AT> inline
typename dr_vector_engine<T,AT>::size_type
dr_vector_engine<T,AT>::elements() const noexcept
{
    return m_elems;
}

template<class T, class AT> inline
typename dr_vector_engine<T,AT>::size_type
dr_vector_engine<T,AT>::capacity() const noexcept
{
    return m_elemcap;
}

template<class T, class AT> inline
typename dr_vector_engine<T,AT>::reference
dr_vector_engine<T,AT>::operator ()(index_type i)
{
    return mp_elems[i];
}

template<class T, class AT> inline
typename dr_vector_engine<T,AT>::iterator
dr_vector_engine<T,AT>::begin() noexcept
{
    return iterator(this, 0, m_elemcap);
}

template<class T, class AT> inline
typename dr_vector_engine<T,AT>::iterator
dr_vector_engine<T,AT>::end() noexcept
{
    return iterator(this, m_elemcap, m_elemcap);
}

template<class T, class AT>
void
dr_vector_engine<T,AT>::assign(dr_vector_engine const& rhs)
{
    if (&rhs == this) return;

    size_t      old_n = (size_t)(m_elemcap);
    size_t      new_n = (size_t)(rhs.m_elemcap);
    pointer     p_tmp = detail::allocate(m_alloc, new_n, rhs.mp_elems);

    detail::deallocate(m_alloc, mp_elems, old_n);
    mp_elems  = p_tmp;
    m_elems   = rhs.m_elems;
    m_elemcap = rhs.m_elemcap;
}

template<class T, class AT>
template<class ET2>
void
dr_vector_engine<T,AT>::assign(ET2 const& rhs)
{
    static_assert(detail::is_vector_engine_v<ET2>);
    using src_index_type = typename ET2::index_type;

    index_type          elems = (index_type) rhs.elements();
    dr_vector_engine    tmp(elems);

    if constexpr(is_same_v<index_type, src_index_type>)
    {
        for (index_type i = 0;  i < elems;  ++i)
        {
            tmp(i) = rhs(i);
        }
    }
    else
    {
        src_index_type  si;
        index_type      di;

        for (di = 0, si = 0;  di < elems;  ++di, ++si)
        {
            tmp(di) = rhs(si);
        }
    }

    tmp.swap(*this);
}

template<class T, class AT> inline
void
dr_vector_engine<T,AT>::reserve(size_type cap)
{
    reshape(m_elems, cap);
}

template<class T, class AT> inline
void
dr_vector_engine<T,AT>::resize(size_type elems)
{
    reshape(elems, m_elemcap);
}

template<class T, class AT> inline
void
dr_vector_engine<T,AT>::resize(size_type elems, size_type cap)
{
    reshape(elems, cap);
}

template<class T, class AT> inline
void
dr_vector_engine<T,AT>::swap(dr_vector_engine& other) noexcept
{
    if (&other != this)
    {
        std::swap(mp_elems,  other.mp_elems);
        std::swap(m_elems,   other.m_elems);
        std::swap(m_elemcap, other.m_elemcap);
    }
}

template<class T, class AT> inline
void
dr_vector_engine<T,AT>::swap_elements(index_type i, index_type j)
{
    std::swap(mp_elems[i], mp_elems[j]);
}

template<class T, class AT>
void
dr_vector_engine<T,AT>::alloc_new(size_type new_size, size_type new_cap)
{
    check_size(new_size);
    check_capacity(new_cap);

    new_cap   = max(new_size, new_cap);
    mp_elems  = detail::allocate(m_alloc, new_cap);
    m_elems   = new_size;
    m_elemcap = new_cap;
}

template<class T, class AT>
void
dr_vector_engine<T,AT>::check_capacity(size_type cap)
{
    if (cap < 0)
    {
        throw runtime_error("invalid capacity");
    }
}

template<class T, class AT>
void
dr_vector_engine<T,AT>::check_size(size_type elems)
{
    if (elems < 1)
    {
        throw runtime_error("invalid size");
    }
}

template<class T, class AT>
void
dr_vector_engine<T,AT>::reshape(size_type elems, size_type cap)
{
    if (elems > m_elemcap  ||  cap > m_elemcap)
    {
        dr_vector_engine    tmp(elems, cap);
        index_type const    dst_elems = min(elems, m_elems);

        for (index_type i = 0;  i < dst_elems;  ++i)
        {
            tmp.mp_elems[i] = mp_elems[i];
        }
        tmp.swap(*this);
    }
    else
    {
        check_size(elems);
        m_elems = elems;
    }
}


//==================================================================================================
//  Dynamically-resizable matrix engine.
//==================================================================================================
//
template<class T, class AT>
class dr_matrix_engine
{
    static_assert(is_matrix_element_v<T>);

  public:
    using engine_category = resizable_matrix_engine_tag;
    using element_type    = T;
    using value_type      = T;
    using allocator_type  = AT;
    using reference       = T&;
    using pointer         = typename allocator_traits<AT>::pointer;
    using const_reference = T const&;
    using const_pointer   = typename allocator_traits<AT>::const_pointer;
    using difference_type = ptrdiff_t;
    using index_type      = ptrdiff_t;
    using size_type       = ptrdiff_t;
    using size_tuple      = tuple<size_type, size_type>;

    using is_fixed_size   = false_type;
    using is_resizable    = true_type;

    using is_column_major = false_type;
    using is_dense        = true_type;
    using is_rectangular  = true_type;
    using is_row_major    = true_type;

    using column_view_type    = matrix_column_view<dr_matrix_engine>;
    using row_view_type       = matrix_row_view<dr_matrix_engine>;
    using transpose_view_type = matrix_transpose_view<dr_matrix_engine>;

  public:
    ~dr_matrix_engine();
    dr_matrix_engine();
    dr_matrix_engine(dr_matrix_engine&& rhs) noexcept;
    dr_matrix_engine(dr_matrix_engine const& rhs);
    dr_matrix_engine(size_type rows, size_type cols);
    dr_matrix_engine(size_type rows, size_type cols, size_type rowcap, size_type colcap);

    dr_matrix_engine& operator =(dr_matrix_engine&&) noexcept;
    dr_matrix_engine& operator =(dr_matrix_engine const&);

    const_reference     operator ()(index_type i, index_type j) const;

    size_type   columns() const noexcept;
    size_type   rows() const noexcept;
    size_tuple  size() const noexcept;

    size_type   column_capacity() const noexcept;
    size_type   row_capacity() const noexcept;
    size_tuple  capacity() const noexcept;

    reference   operator ()(index_type i, index_type j);

    void    assign(dr_matrix_engine const& rhs);
    template<class ET2>
    void    assign(ET2 const& rhs);

    void    reserve(size_type rowcap, size_type colcap);
    void    resize(size_type rows, size_type cols);
    void    resize(size_type rows, size_type cols, size_type rowcap, size_type colcap);
    void    swap(dr_matrix_engine& other) noexcept;
    void    swap_columns(index_type c1, index_type c2);
    void    swap_rows(index_type r1, index_type r2);

  private:
    pointer         mp_elems;       //- For exposition; data buffer
    size_type       m_rows;
    size_type       m_cols;
    size_type       m_rowcap;
    size_type       m_colcap;
    allocator_type  m_alloc;

    void    alloc_new(size_type rows, size_type cols, size_type rowcap, size_type colcap);
    void    check_capacities(size_type rowcap, size_type colcap);
    void    check_sizes(size_type rows, size_type cols);
    void    reshape(size_type rows, size_type cols, size_type rowcap, size_type colcap);
};

template<class T, class AT>
dr_matrix_engine<T,AT>::~dr_matrix_engine()
{
    delete [] mp_elems;
}

template<class T, class AT>
dr_matrix_engine<T,AT>::dr_matrix_engine()
:   mp_elems(nullptr)
,   m_rows(0)
,   m_cols(0)
,   m_rowcap(0)
,   m_colcap(0)
,   m_alloc()
{}

template<class T, class AT>
dr_matrix_engine<T,AT>::dr_matrix_engine(dr_matrix_engine&& rhs) noexcept
:   mp_elems(nullptr)
,   m_rows(0)
,   m_cols(0)
,   m_rowcap(0)
,   m_colcap(0)
,   m_alloc()
{
    rhs.swap(*this);
}

template<class T, class AT>
dr_matrix_engine<T,AT>::dr_matrix_engine(dr_matrix_engine const& rhs)
:   mp_elems(nullptr)
,   m_rows(0)
,   m_cols(0)
,   m_rowcap(0)
,   m_colcap(0)
,   m_alloc()
{
    assign(rhs);
}

template<class T, class AT>
dr_matrix_engine<T,AT>::dr_matrix_engine(size_type rows, size_type cols)
:   mp_elems(nullptr)
,   m_rows(0)
,   m_cols(0)
,   m_rowcap(0)
,   m_colcap(0)
,   m_alloc()
{
    alloc_new(rows, cols, rows, cols);
}

template<class T, class AT>
dr_matrix_engine<T,AT>::dr_matrix_engine
(size_type rows, size_type cols, size_type rowcap, size_type colcap)
:   mp_elems(nullptr)
,   m_rows(0)
,   m_cols(0)
,   m_rowcap(0)
,   m_colcap(0)
,   m_alloc()
{
    alloc_new(rows, cols, rowcap, colcap);
}

template<class T, class AT>
dr_matrix_engine<T,AT>&
dr_matrix_engine<T,AT>::operator =(dr_matrix_engine&& rhs) noexcept
{
    dr_matrix_engine    tmp;
    tmp.swap(rhs);
    tmp.swap(*this);
    return *this;
}

template<class T, class AT>
dr_matrix_engine<T,AT>&
dr_matrix_engine<T,AT>::operator =(dr_matrix_engine const& rhs)
{
    assign(rhs);
    return *this;
}

template<class T, class AT> inline
typename dr_matrix_engine<T,AT>::const_reference
dr_matrix_engine<T,AT>::operator ()(index_type i, index_type j) const
{
    return mp_elems[i*m_colcap + j];
}

template<class T, class AT> inline
typename dr_matrix_engine<T,AT>::size_type
dr_matrix_engine<T,AT>::columns() const noexcept
{
    return m_cols;
}

template<class T, class AT> inline
typename dr_matrix_engine<T,AT>::size_type
dr_matrix_engine<T,AT>::rows() const noexcept
{
    return m_rows;
}

template<class T, class AT> inline
typename dr_matrix_engine<T,AT>::size_tuple
dr_matrix_engine<T,AT>::size() const noexcept
{
    return size_tuple(m_rows, m_cols);
}

template<class T, class AT> inline
typename dr_matrix_engine<T,AT>::size_type
dr_matrix_engine<T,AT>::column_capacity() const noexcept
{
    return m_colcap;
}

template<class T, class AT> inline
typename dr_matrix_engine<T,AT>::size_type
dr_matrix_engine<T,AT>::row_capacity() const noexcept
{
    return m_rowcap;
}

template<class T, class AT> inline
typename dr_matrix_engine<T,AT>::size_tuple
dr_matrix_engine<T,AT>::capacity() const noexcept
{
    return size_tuple(m_rowcap, m_colcap);
}

template<class T, class AT> inline
typename dr_matrix_engine<T,AT>::reference
dr_matrix_engine<T,AT>::operator ()(index_type i, index_type j)
{
    return mp_elems[i*m_colcap + j];
}

template<class T, class AT>
void
dr_matrix_engine<T,AT>::assign(dr_matrix_engine const& rhs)
{
    if (&rhs == this) return;

    size_t      old_n = (size_t)(m_rowcap*m_colcap);
    size_t      new_n = (size_t)(rhs.m_rowcap*rhs.m_colcap);
    pointer     p_tmp = detail::allocate(m_alloc, new_n, rhs.mp_elems);

    detail::deallocate(m_alloc, mp_elems, old_n);
    mp_elems = p_tmp;
    m_rows   = rhs.m_rows;
    m_cols   = rhs.m_cols;
    m_rowcap = rhs.m_rowcap;
    m_colcap = rhs.m_colcap;
}

template<class T, class AT>
template<class ET2>
void
dr_matrix_engine<T,AT>::assign(ET2 const& rhs)
{
    static_assert(detail::is_matrix_engine_v<ET2>);
    using src_index_type = typename ET2::index_type;

    index_type          rows = (index_type) rhs.rows();
    index_type          cols = (index_type) rhs.columns();
    dr_matrix_engine    tmp(rows, cols);

    if constexpr(is_same_v<index_type, src_index_type>)
    {
        for (index_type i = 0;  i < rows;  ++i)
        {
            for (index_type j = 0;  j < cols;  ++j)
            {
                tmp(i, j) = rhs(i, j);
            }
        }
    }
    else
    {
        src_index_type  si, sj;
        index_type      di, dj;

        for (di = 0, si = 0;  di < rows;  ++di, ++si)
        {
            for (dj = 0, sj = 0;  dj < cols;  ++dj, ++sj)
            {
                tmp(di, dj) = rhs(si, sj);
            }
        }
    }
    tmp.swap(*this);
}

template<class T, class AT>
void
dr_matrix_engine<T,AT>::reserve(size_type rowcap, size_type colcap)
{
    reshape(m_rows, m_cols, rowcap, colcap);
}

template<class T, class AT>
void
dr_matrix_engine<T,AT>::resize(size_type rows, size_type cols)
{
    reshape(rows, cols, m_rowcap, m_colcap);
}

template<class T, class AT>
void
dr_matrix_engine<T,AT>::resize(size_type rows, size_type cols, size_type rowcap, size_type colcap)
{
    reshape(rows, cols, rowcap, colcap);
}

template<class T, class AT>
void
dr_matrix_engine<T,AT>::swap(dr_matrix_engine& other) noexcept
{
    if (&other != this)
    {
        std::swap(mp_elems, other.mp_elems);
        std::swap(m_rows,   other.m_rows);
        std::swap(m_cols,   other.m_cols);
        std::swap(m_rowcap, other.m_rowcap);
        std::swap(m_colcap, other.m_colcap);
    }
}

template<class T, class AT>
void
dr_matrix_engine<T,AT>::swap_columns(index_type c1, index_type c2)
{
    if (c1 != c2)
    {
        for (index_type i = 0;  i < m_rows;  ++i)
        {
            std::swap(mp_elems[i*m_colcap + c1], mp_elems[i*m_colcap + c2]);
        }
    }
}

template<class T, class AT>
void
dr_matrix_engine<T,AT>::swap_rows(index_type r1, index_type r2)
{
    if (r1 != r2)
    {
        for (index_type j = 0;  j < m_cols;  ++j)
        {
            std::swap(mp_elems[r1*m_colcap + j], mp_elems[r2*m_colcap + j]);
        }
    }
}

template<class T, class AT>
void
dr_matrix_engine<T,AT>::alloc_new(size_type rows, size_type cols, size_type rowcap, size_type colcap)
{
    check_sizes(rows, cols);
    check_capacities(rowcap, colcap);
    rowcap = max(rows, rowcap);
    colcap = max(cols, colcap);

    mp_elems = detail::allocate(m_alloc, (size_t)(rowcap*colcap));
    m_rows   = rows;
    m_cols   = cols;
    m_rowcap = rowcap;
    m_colcap = colcap;
}

template<class T, class AT>
void
dr_matrix_engine<T,AT>::check_capacities(size_type rowcap, size_type colcap)
{
    if (rowcap < 0  || colcap < 0)
    {
        throw runtime_error("invalid capacity");
    }
}

template<class T, class AT>
void
dr_matrix_engine<T,AT>::check_sizes(size_type rows, size_type cols)
{
    if (rows < 1  || cols < 1)
    {
        throw runtime_error("invalid size");
    }
}

template<class T, class AT>
void
dr_matrix_engine<T,AT>::reshape(size_type rows, size_type cols, size_type rowcap, size_type colcap)
{
    if (rows > m_rowcap  ||  cols > m_colcap   ||  rowcap > m_rowcap  ||  colcap > m_colcap)
    {
        dr_matrix_engine    tmp(rows, cols, rowcap, colcap);
        index_type const    dst_rows = min(rows, m_rows);
        index_type const    dst_cols = min(cols, m_cols);

        for (index_type i = 0;  i < dst_rows;  ++i)
        {
            for (index_type j = 0;  j < dst_cols;  ++j)
            {
                tmp.mp_elems[i*m_colcap + j] = mp_elems[i*m_colcap + j];
            }
        }
        tmp.swap(*this);
    }
    else
    {
        check_sizes(rows, cols);
        check_capacities(rowcap, colcap);
        m_rows = rows;
        m_cols = cols;
    }
}

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_DYNAMIC_ENGINES_HPP_DEFINED
//==================================================================================================
//  File:       fixed_size_engines.hpp
//
//  Summary:    This header defines fixed-size vector and matrix engines.  In this context,
//              fixed-size means that the row and column extents of such objects are known at
//              compile-time, and thus the engines can be made constexpr.
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_FIXED_SIZE_ENGINES_HPP_DEFINED
#define LINEAR_ALGEBRA_FIXED_SIZE_ENGINES_HPP_DEFINED

namespace STD_LA {
//==================================================================================================
//  Fixed-size, fixed-capacity matrix engine.
//==================================================================================================
//
template<class T, int32_t N>
class fs_vector_engine
{
    static_assert(is_matrix_element_v<T>);
    static_assert(N >= 1);

  public:
    using engine_category = mutable_vector_engine_tag;
    using element_type    = T;
    using value_type      = T;
    using reference       = T&;
    using pointer         = T*;
    using const_reference = T const&;
    using const_pointer   = T const*;
    using iterator        = detail::vector_iterator<fs_vector_engine>;
    using const_iterator  = detail::vector_const_iterator<fs_vector_engine>;
    using difference_type = ptrdiff_t;
    using index_type      = int_fast32_t;
    using size_type       = int_fast32_t;

    using is_fixed_size   = true_type;
    using is_resizable    = false_type;

    using is_column_major = true_type;
    using is_dense        = true_type;
    using is_rectangular  = true_type;
    using is_row_major    = true_type;

  public:
    constexpr fs_vector_engine();
    constexpr fs_vector_engine(fs_vector_engine&&) noexcept = default;
    constexpr fs_vector_engine(fs_vector_engine const&) = default;

    constexpr fs_vector_engine&     operator =(fs_vector_engine&&) noexcept = default;
    constexpr fs_vector_engine&     operator =(fs_vector_engine const&) = default;

    constexpr const_reference   operator ()(index_type i) const;
    constexpr const_iterator    begin() const noexcept;
    constexpr const_iterator    end() const noexcept;

    constexpr size_type     capacity() const noexcept;
    constexpr index_type    elements() const noexcept;
    constexpr size_type     size() const noexcept;

    constexpr reference     operator ()(index_type i);
    constexpr iterator      begin() noexcept;
    constexpr iterator      end() noexcept;

    constexpr void  swap(fs_vector_engine& rhs) noexcept;
    constexpr void  swap_elements(index_type i, index_type j) noexcept;

  private:
    T   ma_elems[N];
};

template<class T, int32_t N>
constexpr
fs_vector_engine<T,N>::fs_vector_engine()
{
    if constexpr (is_arithmetic_v<T>)
    {
        for (auto& elem : ma_elems) 
        {
            elem = static_cast<T>(0);
        }
    }
}

template<class T, int32_t N> inline
constexpr typename fs_vector_engine<T,N>::const_reference
fs_vector_engine<T,N>::operator ()(index_type i) const
{
    return ma_elems[i];
}

template<class T, int32_t N> inline
constexpr typename fs_vector_engine<T,N>::const_iterator
fs_vector_engine<T,N>::begin() const noexcept
{
    return const_iterator(this, 0, N);
}

template<class T, int32_t N> inline
constexpr typename fs_vector_engine<T,N>::const_iterator
fs_vector_engine<T,N>::end() const noexcept
{
    return const_iterator(this, N, N);
}

template<class T, int32_t N> inline
constexpr typename fs_vector_engine<T,N>::size_type
fs_vector_engine<T,N>::capacity() const noexcept
{
    return N;
}

template<class T, int32_t N> inline
constexpr typename fs_vector_engine<T,N>::index_type
fs_vector_engine<T,N>::elements() const noexcept
{
    return N;
}

template<class T, int32_t N> inline
constexpr typename fs_vector_engine<T,N>::size_type
fs_vector_engine<T,N>::size() const noexcept
{
    return N;
}

template<class T, int32_t N> inline
constexpr typename fs_vector_engine<T,N>::reference
fs_vector_engine<T,N>::operator ()(index_type i)
{
    return ma_elems[i];
}

template<class T, int32_t N> inline
constexpr typename fs_vector_engine<T,N>::iterator
fs_vector_engine<T,N>::begin() noexcept
{
    return iterator(this, 0, N);
}

template<class T, int32_t N> inline
constexpr typename fs_vector_engine<T,N>::iterator
fs_vector_engine<T,N>::end() noexcept
{
    return iterator(this, N, N);
}

template<class T, int32_t N> inline
constexpr void
fs_vector_engine<T,N>::swap(fs_vector_engine& rhs) noexcept
{
    if (&rhs != this)
    {
        for (index_type i = 0;  i < N;  ++i)
        {
            std::swap(ma_elems[i], rhs.ma_elems[i]);
        }
    }
}

template<class T, int32_t N> inline
constexpr void
fs_vector_engine<T,N>::swap_elements(index_type i, index_type j) noexcept
{
    std::swap(ma_elems[i], ma_elems[j]);
}


//==================================================================================================
//  Fixed-size, fixed-capacity matrix engine.
//==================================================================================================
//
template<class T, int32_t R, int32_t C>
class fs_matrix_engine
{
    static_assert(is_matrix_element_v<T>);
    static_assert(R >= 1);
    static_assert(C >= 1);

  public:
    using engine_category = mutable_matrix_engine_tag;
    using element_type    = T;
    using value_type      = T;
    using reference       = T&;
    using pointer         = T*;
    using const_reference = T const&;
    using const_pointer   = T const*;
    using difference_type = ptrdiff_t;
    using index_type      = int_fast32_t;
    using size_type       = int_fast32_t;
    using size_tuple      = tuple<size_type, size_type>;

    using is_fixed_size   = true_type;
    using is_resizable    = false_type;

    using is_column_major = false_type;
    using is_dense        = true_type;
    using is_rectangular  = true_type;
    using is_row_major    = true_type;

    using column_view_type    = matrix_column_view<fs_matrix_engine>;
    using row_view_type       = matrix_row_view<fs_matrix_engine>;
    using transpose_view_type = matrix_transpose_view<fs_matrix_engine>;

  public:
    constexpr fs_matrix_engine();
    constexpr fs_matrix_engine(fs_matrix_engine&&) noexcept = default;
    constexpr fs_matrix_engine(fs_matrix_engine const&) = default;

    constexpr fs_matrix_engine&     operator =(fs_matrix_engine&&) noexcept = default;
    constexpr fs_matrix_engine&     operator =(fs_matrix_engine const&) = default;

    constexpr const_reference   operator ()(index_type i, index_type j) const;

    constexpr index_type    columns() const noexcept;
    constexpr index_type    rows() const noexcept;
    constexpr size_tuple    size() const noexcept;

    constexpr size_type     column_capacity() const noexcept;
    constexpr size_type     row_capacity() const noexcept;
    constexpr size_tuple    capacity() const noexcept;

    constexpr reference     operator ()(index_type i, index_type j);

    constexpr void      assign(fs_matrix_engine const& rhs);
    template<class ET2>
    constexpr void      assign(ET2 const& rhs);

    constexpr void      swap(fs_matrix_engine& rhs) noexcept;
    constexpr void      swap_columns(index_type j1, index_type j2) noexcept;
    constexpr void      swap_rows(index_type i1, index_type i2) noexcept;

  private:
    T   ma_elems[R*C];
};

template<class T, int32_t R, int32_t C> inline
constexpr
fs_matrix_engine<T,R,C>::fs_matrix_engine()
:   ma_elems()
{
    if constexpr (is_arithmetic_v<T>)
    {
        for (auto& elem : ma_elems) 
        {
            elem = static_cast<T>(0);
        }
    }
}

template<class T, int32_t R, int32_t C> inline
constexpr typename fs_matrix_engine<T,R,C>::const_reference
fs_matrix_engine<T,R,C>::operator ()(index_type i, index_type j) const
{
    return ma_elems[i*C + j];
}

template<class T, int32_t R, int32_t C> inline
constexpr typename fs_matrix_engine<T,R,C>::index_type
fs_matrix_engine<T,R,C>::columns() const noexcept
{
    return C;
}

template<class T, int32_t R, int32_t C> inline
constexpr typename fs_matrix_engine<T,R,C>::index_type
fs_matrix_engine<T,R,C>::rows() const noexcept
{
    return R;
}

template<class T, int32_t R, int32_t C> inline
constexpr typename fs_matrix_engine<T,R,C>::size_tuple
fs_matrix_engine<T,R,C>::size() const noexcept
{
    return size_tuple(R, C);
}

template<class T, int32_t R, int32_t C> inline
constexpr typename fs_matrix_engine<T,R,C>::size_type
fs_matrix_engine<T,R,C>::column_capacity() const noexcept
{
    return C;
}

template<class T, int32_t R, int32_t C> inline
constexpr typename fs_matrix_engine<T,R,C>::size_type
fs_matrix_engine<T,R,C>::row_capacity() const noexcept
{
    return R;
}

template<class T, int32_t R, int32_t C> inline
constexpr typename fs_matrix_engine<T,R,C>::size_tuple
fs_matrix_engine<T,R,C>::capacity() const noexcept
{
    return size_tuple(R, C);
}

template<class T, int32_t R, int32_t C> inline
constexpr typename fs_matrix_engine<T,R,C>::reference
fs_matrix_engine<T,R,C>::operator ()(index_type i, index_type j)
{
    return ma_elems[i*C + j];
}

template<class T, int32_t R, int32_t C> inline
constexpr void
fs_matrix_engine<T,R,C>::assign(fs_matrix_engine const& rhs)
{
    if (&rhs != this) return;

    for (index_type i = 0;  i < R*C;  ++i)
    {
        ma_elems[i] = rhs.ma_elems[i];
    }
}

template<class T, int32_t R, int32_t C> 
template<class ET2> inline
constexpr void
fs_matrix_engine<T,R,C>::assign(ET2 const& rhs)
{
    using src_index_type = typename ET2::index_type;

    if (rhs.size() != size()) 
    {
        throw runtime_error("invalid size");
    }

    if constexpr(is_same_v<index_type, src_index_type>)
    {
        for (index_type i = 0;  i < rows();  ++i)
        {
            for (index_type j = 0;  j < columns();  ++j)
            {
                (*this)(i, j) = rhs(i, j);
            }
        }
    }
    else
    {
        src_index_type  si, sj;
        index_type      di, dj;

        for (di = 0, si = 0;  di < rows();  ++di, ++si)
        {
            for (dj = 0, sj = 0;  dj < columns();  ++dj, ++sj)
            {
                (*this)(di, dj) = rhs(si, sj);
            }
        }
    }
}

template<class T, int32_t R, int32_t C> inline
constexpr void
fs_matrix_engine<T,R,C>::swap(fs_matrix_engine& rhs) noexcept
{
    if (&rhs != this)
    {
        for (index_type i = 0;  i < R*C;  ++i)
        {
            std::swap(ma_elems[i], rhs.ma_elems[i]);
        }
    }
}

template<class T, int32_t R, int32_t C> inline
constexpr void
fs_matrix_engine<T,R,C>::swap_columns(index_type j1, index_type j2) noexcept
{
    if (j1 != j2)
    {
        for (index_type i = 0;  i < R;  ++i)
        {
            std::swap(ma_elems[i*C + j1], ma_elems[i*C + j2]);
        }
    }
}

template<class T, int32_t R, int32_t C> inline
constexpr void
fs_matrix_engine<T,R,C>::swap_rows(index_type i1, index_type i2) noexcept
{
    if (i1 != i2)
    {
        for (index_type j = 0;  j < C;  ++j)
        {
            std::swap(ma_elems[i1*C + j], ma_elems[i2*C + j]);
        }
    }
}

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_FIXED_SIZE_ENGINES_HPP_DEFINED
//==================================================================================================
//  File:       vector.hpp
//
//  Summary:    This header defines the vector class template, one of the two primary math
//              objects provided by the library.
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_VECTOR_HPP_DEFINED
#define LINEAR_ALGEBRA_VECTOR_HPP_DEFINED

namespace STD_LA {
//==================================================================================================
//  A vector type parametrized by an engine type and operator traits.
//==================================================================================================
//
template<class ET, class OT>
class vector
{
    static_assert(detail::is_vector_engine_v<ET>);
    static_assert(is_matrix_element_v<typename ET::element_type>);

  public:
    using engine_type     = ET;
    using element_type    = typename engine_type::element_type;
    using reference       = typename engine_type::reference;
    using pointer         = typename engine_type::pointer;
    using const_reference = typename engine_type::const_reference;
    using const_pointer   = typename engine_type::const_pointer;
    using iterator        = typename engine_type::iterator;
    using const_iterator  = typename engine_type::const_iterator;
    using index_type      = typename engine_type::index_type;
    using size_type       = typename engine_type::size_type;

    using transpose_type  = vector const&;
    using hermitian_type  = conditional_t<is_complex_v<element_type>, vector, transpose_type>;

    using is_fixed_size   = typename engine_type::is_fixed_size;
    using is_resizable    = typename engine_type::is_resizable;

    using is_column_major = typename engine_type::is_column_major;
    using is_dense        = typename engine_type::is_dense;
    using is_rectangular  = typename engine_type::is_rectangular;
    using is_row_major    = typename engine_type::is_row_major;

  public:
    ~vector() = default;

    constexpr vector();
    constexpr vector(vector&&) noexcept = default;
    constexpr vector(vector const&) = default;
    template<class ET2, class OT2>
    constexpr vector(vector<ET2, OT2> const& src);

    template<class ET2 = ET, detail::enable_if_resizable<ET, ET2> = true>
    constexpr vector(size_type elems);
    template<class ET2 = ET, detail::enable_if_resizable<ET, ET2> = true>
    constexpr vector(size_type elems, size_type elemcap);

    constexpr vector& operator =(vector&&) noexcept = default;
    constexpr vector& operator =(vector const&) = default;
    template<class ET2, class OT2>
    constexpr vector& operator =(vector<ET2, OT2> const& rhs);

    //- Const element access.
    //
    constexpr const_reference   operator ()(index_type i) const;
    constexpr const_iterator    begin() const noexcept;
    constexpr const_iterator    end() const noexcept;

    //- Accessors.
    //
    constexpr size_type     capacity() const noexcept;
    constexpr index_type    elements() const noexcept;
    constexpr size_type     size() const noexcept;

    //- Transpose and Hermitian.
    //
    constexpr transpose_type    t() const;
    constexpr hermitian_type    h() const;

    //- Mutable element access.
    //
    constexpr reference     operator ()(index_type i);
    constexpr iterator      begin() noexcept;
    constexpr iterator      end() noexcept;

    //- Assignment.
    //
    constexpr void      assign(vector const& rhs);
    template<class ET2, class OT2>
    constexpr void      assign(vector<ET2, OT2> const& rhs);

    //- Change capacity.
    //
    template<class ET2 = ET, detail::enable_if_resizable<ET, ET2> = true>
    constexpr void      reserve(size_type elemcap);

    //- Change size.
    //
    template<class ET2 = ET, detail::enable_if_resizable<ET, ET2> = true>
    constexpr void      resize(size_type elems);

    //- Change size and capacity in one shot.
    //
    template<class ET2 = ET, detail::enable_if_resizable<ET, ET2> = true>
    constexpr void      resize(size_type elems, size_type elemcap);

    //- Element operations.
    //
    constexpr void      swap(vector& rhs) noexcept;
    constexpr void      swap_elements(index_type i, index_type j) noexcept;

  private:
    template<class ET2, class OT2> friend class vector;
    template<class ET2, class OT2> friend class matrix;

  private:
    engine_type     m_engine;

  private:
    template<class ET2>
    constexpr vector(ET2 const& eng, index_type idx, detail::row_column_tag);
};

template<class ET, class OT> inline
constexpr
vector<ET,OT>::vector()
:   m_engine()
{}

template<class ET, class OT>
template<class ET2, class OT2> inline
constexpr
vector<ET,OT>::vector(vector<ET2, OT2> const&)
{}

template<class ET, class OT>
template<class ET2, detail::enable_if_resizable<ET, ET2>> inline
constexpr
vector<ET,OT>::vector(size_type elems)
:   m_engine(elems)
{}

template<class ET, class OT>
template<class ET2, detail::enable_if_resizable<ET, ET2>> inline
constexpr
vector<ET,OT>::vector(size_type elems, size_type cap)
:   m_engine(elems, cap)
{}

template<class ET, class OT>
template<class ET2> inline
constexpr
vector<ET,OT>::vector(ET2 const& eng, index_type idx, detail::row_column_tag)
:   m_engine(eng, idx)
{}

template<class ET, class OT>
template<class ET2, class OT2> inline
constexpr
vector<ET,OT>&
vector<ET,OT>::operator =(vector<ET2, OT2> const& rhs)
{
    m_engine.assign(rhs.m_engine);
    return *this;
}

template<class ET, class OT> inline
constexpr typename vector<ET,OT>::const_reference
vector<ET,OT>::operator ()(index_type i) const
{
    return m_engine(i);
}

template<class ET, class OT> inline
constexpr typename vector<ET,OT>::const_iterator
vector<ET,OT>::begin() const noexcept
{
    return m_engine.begin();
}

template<class ET, class OT> inline
constexpr typename vector<ET,OT>::const_iterator
vector<ET,OT>::end() const noexcept
{
    return m_engine.end();
}

template<class ET, class OT> inline
constexpr typename vector<ET,OT>::size_type
vector<ET,OT>::capacity() const noexcept
{
    return m_engine.capacity();
}

template<class ET, class OT> inline
constexpr typename vector<ET,OT>::index_type
vector<ET,OT>::elements() const noexcept
{
    return m_engine.elements();
}

template<class ET, class OT> inline
constexpr typename vector<ET,OT>::size_type
vector<ET,OT>::size() const noexcept
{
    return m_engine.elements();
}

template<class ET, class OT> inline
constexpr typename vector<ET,OT>::transpose_type
vector<ET,OT>::t() const
{
    return transpose_type(m_engine);
}

template<class ET, class OT> inline
constexpr typename vector<ET,OT>::hermitian_type
vector<ET,OT>::h() const
{
    if constexpr (is_complex_v<element_type>)
    {
        return hermitian_type();
    }
    else
    {
        return hermitian_type(m_engine);
    }
}

template<class ET, class OT> inline
constexpr typename vector<ET,OT>::reference
vector<ET,OT>::operator ()(index_type i)
{
    return m_engine(i);
}

template<class ET, class OT> inline
constexpr typename vector<ET,OT>::iterator
vector<ET,OT>::begin() noexcept
{
    return m_engine.begin();
}

template<class ET, class OT> inline
constexpr typename vector<ET,OT>::iterator
vector<ET,OT>::end() noexcept
{
    return m_engine.end();
}

template<class ET, class OT>
constexpr void
vector<ET,OT>::assign(vector const& rhs)
{
    m_engine.assign(rhs.m_engine);
}

template<class ET, class OT>
template<class ET2, class OT2>
constexpr void
vector<ET,OT>::assign(vector<ET2, OT2> const& rhs)
{
    m_engine.assign(rhs);
}

template<class ET, class OT>
template<class ET2, detail::enable_if_resizable<ET, ET2>> inline
constexpr void
vector<ET,OT>::reserve(size_type cap)
{
    m_engine.reserve(cap);
}

template<class ET, class OT>
template<class ET2, detail::enable_if_resizable<ET, ET2>> inline
constexpr void
vector<ET,OT>::resize(size_type elems)
{
    m_engine.resize(elems);
}

template<class ET, class OT>
template<class ET2, detail::enable_if_resizable<ET, ET2>> inline
constexpr void
vector<ET,OT>::resize(size_type elems, size_type cap)
{
    m_engine.resize(elems, cap);
}

template<class ET, class OT> inline
constexpr void
vector<ET,OT>::swap(vector& rhs) noexcept
{
    m_engine.swap(rhs);
}

template<class ET, class OT> inline
constexpr void
vector<ET,OT>::swap_elements(index_type i, index_type j) noexcept
{
    m_engine.swap_elements(i, j);
}


template<class ET1, class OT1, class ET2, class OT2>
constexpr bool
operator ==(vector<ET1, OT1> const& v1, vector<ET2, OT2> const& v2)
{
    if (v1.size() != v2.size()) return false;

    for (int i = 0;  i < v1.elements();  ++i)
    {
        if (v1(i) != v2(i)) return false;
    }
    return true;
}

template<class ET1, class OT1, class ET2, class OT2>
constexpr bool
operator !=(vector<ET1, OT1> const& v1, vector<ET2, OT2> const& v2)
{
    return !(v1 == v2);
}


}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_VECTOR_HPP_DEFINED
//==================================================================================================
//  File:       matrix.hpp
//
//  Summary:    This header defines the matrix class template, one of the two primary math
//              objects provided by the library.
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_MATRIX_HPP_DEFINED
#define LINEAR_ALGEBRA_MATRIX_HPP_DEFINED

#include <linear_algebra.hpp>

namespace STD_LA {
//==================================================================================================
//  A matrix parametrized by an engine type and operator traits.
//==================================================================================================
//
template<class ET, class OT>
class matrix
{
    static_assert(detail::is_matrix_engine_v<ET>);

  public:
    using engine_type     = ET;
    using element_type    = typename engine_type::element_type;
    using reference       = typename engine_type::reference;
    using const_reference = typename engine_type::const_reference;
    using index_type      = typename engine_type::index_type;
    using size_type       = typename engine_type::size_type;
    using size_tuple      = typename engine_type::size_tuple;

    using column_type     = vector<matrix_column_view<engine_type>, OT>;
    using row_type        = vector<matrix_row_view<engine_type>, OT>;
    using transpose_type  = matrix<matrix_transpose_view<engine_type>, OT>;
    using hermitian_type  = conditional_t<is_complex_v<element_type>, matrix, transpose_type>;

    using is_fixed_size   = typename engine_type::is_fixed_size;
    using is_resizable    = typename engine_type::is_resizable;

    using is_column_major = typename engine_type::is_column_major;
    using is_dense        = typename engine_type::is_dense;
    using is_rectangular  = typename engine_type::is_rectangular;
    using is_row_major    = typename engine_type::is_row_major;

  public:
    ~matrix() = default;
    constexpr matrix() = default;
    constexpr matrix(matrix&&) noexcept = default;
    constexpr matrix(matrix const&) = default;

    template<class ET2, class OT2>
    matrix(matrix<ET2, OT2> const& src);
    template<class ET2 = ET, detail::enable_if_resizable<ET, ET2> = true>
    constexpr matrix(size_tuple size);
    template<class ET2 = ET, detail::enable_if_resizable<ET, ET2> = true>
    constexpr matrix(size_type rows, size_type cols);

    template<class ET2 = ET, detail::enable_if_resizable<ET, ET2> = true>
    constexpr matrix(size_tuple size, size_tuple cap);
    template<class ET2 = ET, detail::enable_if_resizable<ET, ET2> = true>
    constexpr matrix(size_type rows, size_type cols, size_type rowcap, size_type colcap);

    constexpr matrix&       operator =(matrix&&) noexcept = default;
    constexpr matrix&       operator =(matrix const&) = default;
    template<class ET2, class OT2>
    constexpr matrix&       operator =(matrix<ET2, OT2> const& rhs);

    //- Const element access.
    //
    constexpr const_reference   operator ()(index_type i, index_type j) const;

    //- Accessors.
    //
    constexpr index_type    columns() const noexcept;
    constexpr index_type    rows() const noexcept;
    constexpr size_tuple    size() const noexcept;

    constexpr size_type     column_capacity() const noexcept;
    constexpr size_type     row_capacity() const noexcept;
    constexpr size_tuple    capacity() const noexcept;

    //- Column view, row view, transpose view, and Hermitian.
    //
    constexpr column_type       column(index_type j) const noexcept;
    constexpr row_type          row(index_type i) const noexcept;
    constexpr transpose_type    t() const;
    constexpr hermitian_type    h() const;

    //- Mutable element access.
    //
    constexpr reference     operator ()(index_type i, index_type j);

    //- Assignment.
    //
    constexpr void      assign(matrix const& rhs);
    template<class ET2, class OT2>
    constexpr void      assign(matrix<ET2, OT2> const& rhs);

    //- Change capacity.
    //
    template<class ET2 = ET, detail::enable_if_resizable<ET, ET2> = true>
    constexpr void      reserve(size_tuple cap);
    template<class ET2 = ET, detail::enable_if_resizable<ET, ET2> = true>
    constexpr void      reserve(size_type rowcap, size_type colcap);

    //- Change size.
    //
    template<class ET2 = ET, detail::enable_if_resizable<ET, ET2> = true>
    constexpr void      resize(size_tuple size);
    template<class ET2 = ET, detail::enable_if_resizable<ET, ET2> = true>
    constexpr void      resize(size_type rows, size_type cols);

    //- Change size and capacity in one shot.
    //
    template<class ET2 = ET, detail::enable_if_resizable<ET, ET2> = true>
    constexpr void      resize(size_tuple size, size_tuple cap);
    template<class ET2 = ET, detail::enable_if_resizable<ET, ET2> = true>
    constexpr void      resize(size_type rows, size_type cols, size_type rowcap, size_type colcap);

    //- Swapping operations.
    //
    template<class ET2 = ET, detail::enable_if_mutable<ET, ET2> = true>
    constexpr void      swap(matrix& rhs) noexcept;
    template<class ET2 = ET, detail::enable_if_mutable<ET, ET2> = true>
    constexpr void      swap_columns(index_type i, index_type j) noexcept;
    template<class ET2 = ET, detail::enable_if_mutable<ET, ET2> = true>
    constexpr void      swap_rows(index_type i, index_type j) noexcept;

  private:
    template<class ET2, class OT2> friend class matrix;
    template<class ET2, class OT2> friend class vector;

  private:
    engine_type     m_engine;

  private:
    constexpr matrix(engine_type const& eng);
};

template<class ET, class OT>
template<class ET2, class OT2>
matrix<ET,OT>::matrix(matrix<ET2, OT2> const& rhs)
:   m_engine()
{
    assign(rhs);
}

template<class ET, class OT>
template<class ET2, detail::enable_if_resizable<ET, ET2>>
constexpr 
matrix<ET,OT>::matrix(size_tuple size)
:   m_engine(get<0>(size), get<1>(size))
{}

template<class ET, class OT>
template<class ET2, detail::enable_if_resizable<ET, ET2>>
constexpr 
matrix<ET,OT>::matrix(size_type rows, size_type cols)
:   m_engine(rows, cols)
{}

template<class ET, class OT>
template<class ET2, detail::enable_if_resizable<ET, ET2>>
constexpr 
matrix<ET,OT>::matrix(size_tuple size, size_tuple cap)
:   m_engine(get<0>(size), get<1>(size), get<0>(cap), get<1>(cap))
{}

template<class ET, class OT>
template<class ET2, detail::enable_if_resizable<ET, ET2>>
constexpr 
matrix<ET,OT>::matrix(size_type rows, size_type cols, size_type rowcap, size_type colcap)
:   m_engine(rows, cols, rowcap, colcap)
{}

template<class ET, class OT>
constexpr 
matrix<ET,OT>::matrix(engine_type const& eng)
:   m_engine(eng)
{}

template<class ET, class OT>
template<class ET2, class OT2>
constexpr matrix<ET,OT>&
matrix<ET,OT>::operator =(matrix<ET2, OT2> const& rhs)
{
    m_engine.assign(rhs.m_engine);
    return *this;
}

template<class ET, class OT> inline
constexpr typename matrix<ET,OT>::const_reference
matrix<ET,OT>::operator ()(index_type i, index_type j) const
{
    return m_engine(i, j);
}

template<class ET, class OT> inline
constexpr typename matrix<ET,OT>::index_type
matrix<ET,OT>::columns() const noexcept
{
    return m_engine.columns();
}

template<class ET, class OT> inline
constexpr typename matrix<ET,OT>::index_type
matrix<ET,OT>::rows() const noexcept
{
    return m_engine.rows();
}

template<class ET, class OT> inline
constexpr typename matrix<ET,OT>::size_tuple
matrix<ET,OT>::size() const noexcept
{
    return size_tuple(m_engine.rows(), m_engine.columns());
}

template<class ET, class OT> inline
constexpr typename matrix<ET,OT>::size_type
matrix<ET,OT>::column_capacity() const noexcept
{
    return m_engine.column_capacity();
}

template<class ET, class OT> inline
constexpr typename matrix<ET,OT>::size_type
matrix<ET,OT>::row_capacity() const noexcept
{
    return m_engine.row_capacity();
}

template<class ET, class OT> inline
constexpr typename matrix<ET,OT>::size_tuple
matrix<ET,OT>::capacity() const noexcept
{
    return size_tuple(m_engine.row_capacity(), m_engine.column_capacity());
}

template<class ET, class OT> inline
constexpr typename matrix<ET,OT>::column_type
matrix<ET,OT>::column(index_type j) const noexcept
{
    return column_type(m_engine, j, detail::row_column_tag());
}

template<class ET, class OT> inline
constexpr typename matrix<ET,OT>::row_type
matrix<ET,OT>::row(index_type i) const noexcept
{
    return row_type(m_engine, i, detail::row_column_tag());
}

template<class ET, class OT> inline
constexpr typename matrix<ET,OT>::transpose_type
matrix<ET,OT>::t() const
{
    return transpose_type(m_engine);
}

template<class ET, class OT> inline
constexpr typename matrix<ET,OT>::hermitian_type
matrix<ET,OT>::h() const
{
    if constexpr (is_complex_v<element_type>)
    {
        return hermitian_type();
    }
    else
    {
        return hermitian_type(m_engine);
    }
}

template<class ET, class OT> inline
constexpr typename matrix<ET,OT>::reference
matrix<ET,OT>::operator ()(index_type i, index_type j)
{
    return m_engine(i, j);
}

template<class ET, class OT> inline
constexpr void
matrix<ET,OT>::assign(matrix const& rhs)
{
    m_engine.assign(rhs.m_engine);
}

template<class ET, class OT>
template<class ET2, class OT2> inline
constexpr void
matrix<ET,OT>::assign(matrix<ET2, OT2> const& rhs)
{
    m_engine.assign(rhs.m_engine);
}

template<class ET, class OT>
template<class ET2, detail::enable_if_resizable<ET, ET2>> inline
constexpr void
matrix<ET,OT>::reserve(size_tuple cap)
{
    m_engine.resize(get<0>(cap), get<1>(cap));
}

template<class ET, class OT>
template<class ET2, detail::enable_if_resizable<ET, ET2>> inline
constexpr void
matrix<ET,OT>::reserve(size_type rowcap, size_type colcap)
{
    m_engine.reserve(rowcap, colcap);
}

template<class ET, class OT>
template<class ET2, detail::enable_if_resizable<ET, ET2>> inline
constexpr void
matrix<ET,OT>::resize(size_tuple size)
{
    m_engine.resize(get<0>(size), get<1>(size));
}

template<class ET, class OT>
template<class ET2, detail::enable_if_resizable<ET, ET2>> inline
constexpr void
matrix<ET,OT>::resize(size_type rows, size_type cols)
{
    m_engine.resize(rows, cols);
}

template<class ET, class OT>
template<class ET2, detail::enable_if_resizable<ET, ET2>> inline
constexpr void
matrix<ET,OT>::resize(size_tuple size, size_tuple cap)
{
    m_engine.resize(get<0>(size), get<1>(size), get<0>(cap), get<1>(cap));
}

template<class ET, class OT>
template<class ET2, detail::enable_if_resizable<ET, ET2>> inline
constexpr void
matrix<ET,OT>::resize(size_type rows, size_type cols, size_type rowcap, size_type colcap)
{
    m_engine.resize(rows, cols, rowcap, colcap);
}

template<class ET, class OT>
template<class ET2, detail::enable_if_mutable<ET, ET2>> inline
constexpr void
matrix<ET,OT>::swap(matrix& rhs) noexcept
{
    m_engine.swap(rhs.m_engine);
}

template<class ET, class OT>
template<class ET2, detail::enable_if_mutable<ET, ET2>> inline
constexpr void
matrix<ET,OT>::swap_columns(index_type c1, index_type c2) noexcept
{
    m_engine.swap_columns(c1, c2);
}

template<class ET, class OT>
template<class ET2, detail::enable_if_mutable<ET, ET2>> inline
constexpr void
matrix<ET,OT>::swap_rows(index_type r1, index_type r2) noexcept
{
    m_engine.swap_rows(r1, r2);
}

//--------
//
template<class ET1, class OT1, class ET2, class OT2> 
constexpr bool
operator ==(matrix<ET1, OT1> const& m1, matrix<ET2, OT2> const& m2)
{
    if (m1.size() != m2.size()) return false;

    for (int i = 0;  i < m1.rows();  ++i)
    {
        for (int j = 0;  j < m2.columns();  ++j)
        {
            if (m1(i, j) != m2(i, j)) return false;
        }
    }
    return true;
}

template<class ET1, class OT1, class ET2, class OT2> 
constexpr bool
operator !=(matrix<ET1, OT1> const& m1, matrix<ET2, OT2> const& m2)
{
    return !(m1 == m2);
}


}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_MATRIX_HPP_DEFINED
//==================================================================================================
//  File:       library_aliases.hpp
//
//  Summary:    This header defines several public type aliases, intended as a convenience for
//              the user.
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_LIBRARY_ALIASES_HPP_DEFINED
#define LINEAR_ALGEBRA_LIBRARY_ALIASES_HPP_DEFINED

//- Convenience aliases.
//
namespace STD_LA {

//- Aliases for column_vector/row_vector/matrix objects based on dynamic engines.
//
template<class T, class A = allocator<T>>
using dyn_vector = vector<dr_vector_engine<T, A>>;

template<class T, class A = allocator<T>>
using dyn_matrix = matrix<dr_matrix_engine<T, A>>;


//- Aliases for column_vector/row_vector/matrix objects based on fixed-size engines.
//
template<class T, int32_t N>
using fs_vector = vector<fs_vector_engine<T, N>>;

template<class T, int32_t R, int32_t C>
using fs_matrix = matrix<fs_matrix_engine<T, R, C>>;

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_LIBRARY_ALIASES_HPP_DEFINED
//==================================================================================================
//  File:       arithmetic_traits.hpp
//
//  Summary:    This header defines several private traits types that are used by the various
//              arithmetic traits types, public and private, to implement arithmetic operators.
//==================================================================================================
//
#ifndef MATRIX_ARITHMETIC_TRAITS_HPP_DEFINED
#define MATRIX_ARITHMETIC_TRAITS_HPP_DEFINED

namespace STD_LA {
namespace detail {
//--------------------------------------------------------------------------------------------------
//- Helper traits for choosing between three traits-type parameters.
//
template<class T1, class T2, class DEF>
struct non_void_traits_chooser;

template<class T1, class DEF>
struct non_void_traits_chooser<T1, void, DEF>
{
    using traits_type = T1;
};

template<class T2, class DEF>
struct non_void_traits_chooser<void, T2, DEF>
{
    using traits_type = T2;
};

template<class DEF>
struct non_void_traits_chooser<void, void, DEF>
{
    using traits_type = DEF;
};


//--------------------------------------------------------------------------------------------------
//- Helper traits for rebinding standard-conformant allocators.
//
template<class A1, class T1>
using rebind_alloc_t = typename allocator_traits<A1>::template rebind_alloc<T1>;


}       //- detail namespace

template<class ET, class OT> inline
constexpr bool
result_requires_resize(vector<ET, OT> const&)
{
	return vector<ET, OT>::engine_type::is_resizable::value == true &&
           vector<ET, OT>::engine_type::is_fixed_size::value == false;
}

template<class ET, class OT> inline
constexpr bool
result_requires_resize(matrix<ET, OT> const&)
{
	return matrix<ET, OT>::engine_type::is_resizable::value == true &&
           matrix<ET, OT>::engine_type::is_fixed_size::value == false;
}

template<class ET, class OT> inline
constexpr bool
is_fixed_size(vector<ET,OT> const&)
{
    return vector<ET,OT>::engine_type::is_fixed_size::value;
}

template<class ET, class OT> inline
constexpr bool
is_fixed_size(matrix<ET,OT> const&)
{
    return matrix<ET,OT>::engine_type::is_fixed_size::value;
}

template<class ET, class OT> inline
constexpr bool
is_resizable(vector<ET,OT> const&)
{
    return vector<ET,OT>::engine_type::is_resizable::value;
}

template<class ET, class OT> inline
constexpr bool
is_resizable(matrix<ET,OT> const&)
{
    return matrix<ET,OT>::engine_type::is_resizable::value;
}

}       //- STD_LA namespace
#endif  //- MATRIX_ARITHMETIC_TRAITS_HPP_DEFINED
//==================================================================================================
//  File:       addition_traits.hpp
//
//  Summary:    This header defines several private and public traits types that are used to
//              implement the binary addition operator.  This is a somewhat long file, divided
//              into two main sections, each with three sub-sections:
//                  1. Private traits definitions that:
//                      A. determine the element promotion traits to be used for addition
//                      B. determine the engine promotion traits to be used for addition
//                      C. determine the arithmetic traits to be used to perform addition
//                  2. Public traits definitions that:
//                      A. perform element promotion for addition
//                      B. perform engine promotion for addition
//                      C. perform the actual act of addition
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_ADDITION_TRAITS_HPP_DEFINED
#define LINEAR_ALGEBRA_ADDITION_TRAITS_HPP_DEFINED

namespace STD_LA {
namespace detail {
//==================================================================================================
//                         **** ELEMENT ADDITION TRAITS DETECTORS ****
//==================================================================================================
//
//- Form 1 type detection of nested element addition traits.
//
template<typename OT>
using element_add_traits_f1_t = typename OT::element_addition_traits;

template<typename OT>
using element_add_type_f1_t = typename element_add_traits_f1_t<OT>::element_type;

//- Define the form 1 detectors.
//
template<typename OT, typename = void>
struct detect_element_add_traits_f1
:   public false_type
{
    using traits_type = void;
};

template<typename OT>
struct detect_element_add_traits_f1<OT, void_t<element_add_type_f1_t<OT>>>
:   public true_type
{
    using traits_type = element_add_traits_f1_t<OT>;
};

//----------------------------------------------------------
//- Form 2 type detection of nested element addition traits.
//
template<typename OT, typename T1, typename T2>
using element_add_traits_f2_t = typename OT::template element_addition_traits<T1, T2>;

template<typename OT, typename T1, typename T2>
using element_add_type_f2_t = typename element_add_traits_f2_t<OT, T1, T2>::element_type;

//- Define the form 2 detectors.
//
template<typename OT, typename T1, typename T2, typename = void>
struct detect_element_add_traits_f2
:   public false_type
{
    using traits_type = void;
};

template<typename OT, typename T1, typename T2>
struct detect_element_add_traits_f2<OT, T1, T2, void_t<element_add_type_f2_t<OT, T1, T2>>>
:   public true_type
{
    using traits_type = element_add_traits_f2_t<OT, T1, T2>;
};

//---------------------------------------------
//- Element addition traits type determination.
//
template<typename OT, typename T1, typename T2>
struct element_add_traits_chooser
{
    using CT1 = typename detect_element_add_traits_f1<OT>::traits_type;
    using CT2 = typename detect_element_add_traits_f2<OT, T1, T2>::traits_type;
    using DEF = matrix_addition_element_traits<T1, T2>;

    using traits_type = typename non_void_traits_chooser<CT1, CT2, DEF>::traits_type;
};

template<typename OT, typename T1, typename T2>
using element_add_traits_t = typename element_add_traits_chooser<OT, T1, T2>::traits_type;

template<class OT, class T1, class T2>
constexpr bool  has_element_add_traits_v = detect_element_add_traits_f2<OT, T1, T2>::value ||
                                           detect_element_add_traits_f1<OT>::value;

//---------------------------------------------
//- Element addition result type determination.
//
template<typename OT, typename T1, typename T2>
struct element_add_type
{
    using traits_type  = typename element_add_traits_chooser<OT, T1, T2>::traits_type;
    using element_type = typename traits_type::element_type;
};

template<typename OT, typename T1, typename T2>
using element_add_type_t = typename element_add_type<OT, T1, T2>::element_type;


//==================================================================================================
//                          **** ENGINE ADDITION TRAITS DETECTORS ****
//==================================================================================================
//
//- Form 1 type detection of nested engine addition traits.
//
template<typename OT>
using engine_add_traits_f1_t = typename OT::engine_addition_traits;

template<typename OT>
using engine_add_type_f1_t = typename engine_add_traits_f1_t<OT>::engine_type;

//- Define the form 1 detectors.
//
template<typename OT, typename = void>
struct detect_engine_add_traits_f1
:   public false_type
{
    using traits_type = void;
};

template<typename OT>
struct detect_engine_add_traits_f1<OT, void_t<engine_add_type_f1_t<OT>>>
:   public true_type
{
    using traits_type = engine_add_traits_f1_t<OT>;
};

//---------------------------------------------------------
//- Form 2 type detection of nested engine addition traits.
//
template<typename OT, typename T1, typename T2>
using engine_add_traits_f2_t = typename OT::template engine_addition_traits<OT, T1, T2>;

template<typename OT, typename T1, typename T2>
using engine_add_type_f2_t = typename engine_add_traits_f2_t<OT, T1, T2>::engine_type;

//- Define the form 2 detectors.
//
template<typename OT, typename ET1, typename ET2, typename = void>
struct detect_engine_add_traits_f2
:   public false_type
{
    using traits_type = void;
};

template<typename OT, typename ET1, typename ET2>
struct detect_engine_add_traits_f2<OT, ET1, ET2, void_t<engine_add_type_f2_t<OT, ET1, ET2>>>
:   public true_type
{
    using traits_type = engine_add_traits_f2_t<OT, ET1, ET2>;
};

//--------------------------------------------
//- Engine addition traits type determination.
//
template<typename OT, typename ET1, typename ET2>
struct engine_add_traits_chooser
{
    using CT1 = typename detect_engine_add_traits_f1<OT>::traits_type;
    using CT2 = typename detect_engine_add_traits_f2<OT, ET1, ET2>::traits_type;
    using DEF = matrix_addition_engine_traits<OT, ET1, ET2>;

    using traits_type = typename non_void_traits_chooser<CT1, CT2, DEF>::traits_type;
};

template<typename OT, typename ET1, typename ET2>
using engine_add_traits_t = typename engine_add_traits_chooser<OT, ET1, ET2>::traits_type;

template<class OT, class ET1, class ET2>
constexpr bool  has_engine_add_traits_v = detect_engine_add_traits_f2<OT, ET1, ET2>::value ||
                                          detect_engine_add_traits_f1<OT>::value;

//--------------------------------------------
//- Engine addition result type determination.
//
template<typename OT, typename ET1, typename ET2>
struct engine_add_type
{
    using traits_type = typename engine_add_traits_chooser<OT, ET1, ET2>::traits_type;
    using engine_type = typename traits_type::engine_type;
};

template<typename OT, typename ET1, typename ET2>
using engine_add_type_t = typename engine_add_type<OT, ET1, ET2>::engine_type;


//==================================================================================================
//                         **** ADDITION ARITHMETIC TRAITS DETECTORS ****
//==================================================================================================
//
//- Form 1 type detection of nested addition arithmetic traits.
//
template<typename OT>
using add_traits_f1_t = typename OT::addition_traits;

template<typename OT>
using add_type_f1_t = typename add_traits_f1_t<OT>::result_type;

//- Define the form 1 detectors.
//
template<typename OT, typename = void>
struct detect_add_traits_f1
:   public false_type
{
    using traits_type = void;
};

template<typename OT>
struct detect_add_traits_f1<OT, void_t<add_type_f1_t<OT>>>
:   public true_type
{
    using traits_type = add_traits_f1_t<OT>;
};

//-------------------------------------------------------------
//- Form 2 type detection of nested addition arithmetic traits.
//
template<typename OT, typename T1, typename T2>
using add_traits_f2_t = typename OT::template addition_traits<OT, T1, T2>;

template<typename OT, typename T1, typename T2>
using add_type_f2_t = typename add_traits_f2_t<OT, T1, T2>::result_type;

//- Define the form 2 detectors.
//
template<typename OT, typename OP1, typename OP2, typename = void>
struct detect_add_traits_f2
:   public false_type
{
    using traits_type = void;
};

template<typename OT, typename OP1, typename OP2>
struct detect_add_traits_f2<OT, OP1, OP2, void_t<add_type_f2_t<OT, OP1, OP2>>>
:   public true_type
{
    using traits_type = typename OT::template addition_traits<OT, OP1, OP2>;
};

//------------------------------------------------
//- Addition arithmetic traits type determination.
//
template<typename OT, typename OP1, typename OP2>
struct add_traits_chooser
{
    using CT1 = typename detect_add_traits_f1<OT>::traits_type;
    using CT2 = typename detect_add_traits_f2<OT, OP1, OP2>::traits_type;
    using DEF = matrix_addition_traits<OT, OP1, OP2>;

    using traits_type = typename non_void_traits_chooser<CT1, CT2, DEF>::traits_type;
};

template<typename OT, typename OP1, typename OP2>
using addition_traits_t = typename add_traits_chooser<OT, OP1, OP2>::traits_type;

template<class OT, class OP1, class OP2>
constexpr bool  has_add_traits_v = detect_add_traits_f2<OT, OP1, OP2>::value ||
                                   detect_add_traits_f1<OT>::value;


}       //- detail namespace
//==================================================================================================
//                              **** ELEMENT ADDITION TRAITS ****
//==================================================================================================
//
//- Alias interface to detection meta-function that extracts the element addition traits type.
//
template<class OT, class T1, class T2>
using matrix_addition_element_t = detail::element_add_type_t<OT, T1, T2>;


//- The standard element addition traits type provides the default mechanism for determining the
//  result of adding two elements of (possibly) different types.
//
template<class T1, class T2>
struct matrix_addition_element_traits
{
    using element_type = decltype(declval<T1>() + declval<T2>());
};


//==================================================================================================
//                               **** ENGINE ADDITION TRAITS ****
//==================================================================================================
//
//- Alias interface to detection meta-function that extracts the engine addition traits type.
//
template<class OT, class ET1, class ET2>
using matrix_addition_engine_t = detail::engine_add_type_t<OT, ET1, ET2>;


//- The standard engine addition traits type provides the default mechanism for determining the
//  correct engine type for a matrix/matrix or vector/vector addition.
//
template<class OT, class ET1, class ET2>
struct matrix_addition_engine_traits
{
    static_assert(detail::engines_match_v<ET1, ET2>);

    using element_type_1 = typename ET1::element_type;
    using element_type_2 = typename ET2::element_type;
    using element_type   = matrix_addition_element_t<OT, element_type_1, element_type_2>;
    using engine_type    = conditional_t<detail::is_matrix_engine_v<ET1>,
                                         dr_matrix_engine<element_type, allocator<element_type>>,
                                         dr_vector_engine<element_type, allocator<element_type>>>;
};

//- General transpose cases for matrices.
//
template<class OT, class ET1, class ET2>
struct matrix_addition_engine_traits<OT, matrix_transpose_view<ET1>, ET2>
{
    using engine_type = typename matrix_addition_engine_traits<OT, ET1, ET2>::engine_type;
};

template<class OT, class ET1, class ET2>
struct matrix_addition_engine_traits<OT, ET1, matrix_transpose_view<ET2>>
{
    using engine_type = typename matrix_addition_engine_traits<OT, ET1, ET2>::engine_type;
};

template<class OT, class ET1, class ET2>
struct matrix_addition_engine_traits<OT, matrix_transpose_view<ET1>, matrix_transpose_view<ET2>>
{
    using engine_type = typename matrix_addition_engine_traits<OT, ET1, ET2>::engine_type;
};


//--------------------------------------------------------------------------------------------------
//- ENGINE + ENGINE cases for vector/vector.  Note that all partial specializations below in which
//  allocators are rebound assume standard-conformant allocator types.
//
//- dr_vector_engine + dr_vector_engine.
//
template<class OT, class T1, class A1, class T2, class A2>
struct matrix_addition_engine_traits<OT, dr_vector_engine<T1, A1>, dr_vector_engine<T2, A2>>
{
    using element_type = matrix_addition_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<A1, element_type>;
    using engine_type  = dr_vector_engine<element_type, alloc_type>;
};

//--------------------------------------
//- dr_vector_engine + fs_vector_engine.
//
template<class OT, class T1, class A1, class T2, int32_t N2>
struct matrix_addition_engine_traits<OT, dr_vector_engine<T1, A1>, fs_vector_engine<T2, N2>>
{
    using element_type = matrix_addition_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<A1, element_type>;
    using engine_type  = dr_vector_engine<element_type, alloc_type>;
};

//--------------------------------------
//- fs_vector_engine + dr_vector_engine.
//
template<class OT, class T1, int32_t N1, class T2, class A2>
struct matrix_addition_engine_traits<OT, fs_vector_engine<T1, N1>, dr_vector_engine<T2, A2>>
{
    using element_type = matrix_addition_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<A2, element_type>;
    using engine_type  = dr_vector_engine<element_type, alloc_type>;
};

//--------------------------------------
//- fs_vector_engine + fs_vector_engine.
//
template<class OT, class T1, int32_t N1, class T2, int32_t N2>
struct matrix_addition_engine_traits<OT, fs_vector_engine<T1, N1>, fs_vector_engine<T2, N2>>
{
    static_assert(N1 == N2);
    using element_type = matrix_addition_element_t<OT, T1, T2>;
    using engine_type  = fs_vector_engine<element_type, N1>;
};

//--------------------------------------------------------------------------------------------------
//- ENGINE + ENGINE cases for matrix/matrix.  Note that all partial specializations below in which
//  allocators are rebound assume standard-conformant allocator types.
//
//- dr_matrix_engine + dr_matrix_engine.
//
template<class OT, class T1, class A1, class T2, class A2>
struct matrix_addition_engine_traits<OT,
                                     dr_matrix_engine<T1, A1>,
                                     dr_matrix_engine<T2, A2>>
{
    using element_type = matrix_addition_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<A1, element_type>;
    using engine_type  = dr_matrix_engine<element_type, alloc_type>;
};

template<class OT, class T1, class A1, class T2, class A2>
struct matrix_addition_engine_traits<OT,
                                     dr_matrix_engine<T1, A1>,
                                     matrix_transpose_view<dr_matrix_engine<T2, A2>>>
{
    using element_type = matrix_addition_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<A1, element_type>;
    using engine_type  = dr_matrix_engine<element_type, alloc_type>;
};

template<class OT, class T1, class A1, class T2, class A2>
struct matrix_addition_engine_traits<OT,
                                     matrix_transpose_view<dr_matrix_engine<T1, A1>>,
                                     dr_matrix_engine<T2, A2>>
{
    using element_type = matrix_addition_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<A1, element_type>;
    using engine_type  = dr_matrix_engine<element_type, alloc_type>;
};

template<class OT, class T1, class A1, class T2, class A2>
struct matrix_addition_engine_traits<OT,
                                     matrix_transpose_view<dr_matrix_engine<T1, A1>>,
                                     matrix_transpose_view<dr_matrix_engine<T2, A2>>>
{
    using element_type = matrix_addition_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<A1, element_type>;
    using engine_type  = dr_matrix_engine<element_type, alloc_type>;
};

//--------------------------------------
//- dr_matrix_engine + fs_matrix_engine.
//
template<class OT, class T1, class A1, class T2, int32_t R2, int32_t C2>
struct matrix_addition_engine_traits<OT,
                                     dr_matrix_engine<T1, A1>,
                                     fs_matrix_engine<T2, R2, C2>>
{
    using element_type = matrix_addition_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<A1, element_type>;
    using engine_type  = dr_matrix_engine<element_type, alloc_type>;
};

template<class OT, class T1, class A1, class T2, int32_t R2, int32_t C2>
struct matrix_addition_engine_traits<OT,
                                     dr_matrix_engine<T1, A1>,
                                     matrix_transpose_view<fs_matrix_engine<T2, R2, C2>>>
{
    using element_type = matrix_addition_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<A1, element_type>;
    using engine_type  = dr_matrix_engine<element_type, alloc_type>;
};

template<class OT, class T1, class A1, class T2, int32_t R2, int32_t C2>
struct matrix_addition_engine_traits<OT,
                                     matrix_transpose_view<dr_matrix_engine<T1, A1>>,
                                     fs_matrix_engine<T2, R2, C2>>
{
    using element_type = matrix_addition_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<A1, element_type>;
    using engine_type  = dr_matrix_engine<element_type, alloc_type>;
};

template<class OT, class T1, class A1, class T2, int32_t R2, int32_t C2>
struct matrix_addition_engine_traits<OT,
                                     matrix_transpose_view<dr_matrix_engine<T1, A1>>,
                                     matrix_transpose_view<fs_matrix_engine<T2, R2, C2>>>
{
    using element_type = matrix_addition_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<A1, element_type>;
    using engine_type  = dr_matrix_engine<element_type, alloc_type>;
};

//--------------------------------------
//- fs_matrix_engine + dr_matrix_engine.
//
template<class OT, class T1, int32_t R1, int32_t C1, class T2, class A2>
struct matrix_addition_engine_traits<OT,
                                     fs_matrix_engine<T1, R1, C1>,
                                     dr_matrix_engine<T2, A2>>
{
    using element_type = matrix_addition_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<A2, element_type>;
    using engine_type  = dr_matrix_engine<element_type, alloc_type>;
};

template<class OT, class T1, int32_t R1, int32_t C1, class T2, class A2>
struct matrix_addition_engine_traits<OT,
                                     fs_matrix_engine<T1, R1, C1>,
                                     matrix_transpose_view<dr_matrix_engine<T2, A2>>>
{
    using element_type = matrix_addition_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<A2, element_type>;
    using engine_type  = dr_matrix_engine<element_type, alloc_type>;
};

template<class OT, class T1, int32_t R1, int32_t C1, class T2, class A2>
struct matrix_addition_engine_traits<OT,
                                     matrix_transpose_view<fs_matrix_engine<T1, R1, C1>>,
                                     dr_matrix_engine<T2, A2>>
{
    using element_type = matrix_addition_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<A2, element_type>;
    using engine_type  = dr_matrix_engine<element_type, alloc_type>;
};

template<class OT, class T1, int32_t R1, int32_t C1, class T2, class A2>
struct matrix_addition_engine_traits<OT,
                                     matrix_transpose_view<fs_matrix_engine<T1, R1, C1>>,
                                     matrix_transpose_view<dr_matrix_engine<T2, A2>>>
{
    using element_type = matrix_addition_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<A2, element_type>;
    using engine_type  = dr_matrix_engine<element_type, alloc_type>;
};

//--------------------------------------
//- fs_matrix_engine + fs_matrix_engine.
//
template<class OT, class T1, int32_t R1, int32_t C1, class T2, int32_t R2, int32_t C2>
struct matrix_addition_engine_traits<OT,
                                     fs_matrix_engine<T1, R1, C1>,
                                     fs_matrix_engine<T2, R2, C2>>
{
    static_assert(R1 == R2);
    static_assert(C1 == C2);
    using element_type = matrix_addition_element_t<OT, T1, T2>;
    using engine_type  = fs_matrix_engine<element_type, R1, C1>;
};

template<class OT, class T1, int32_t R1, int32_t C1, class T2, int32_t R2, int32_t C2>
struct matrix_addition_engine_traits<OT,
                                     matrix_transpose_view<fs_matrix_engine<T1, R1, C1>>,
                                     fs_matrix_engine<T2, R2, C2>>
{
    static_assert(R1 == C2);
    static_assert(C1 == R2);
    using element_type = matrix_addition_element_t<OT, T1, T2>;
    using engine_type  = fs_matrix_engine<element_type, R2, C2>;
};

template<class OT, class T1, int32_t R1, int32_t C1, class T2, int32_t R2, int32_t C2>
struct matrix_addition_engine_traits<OT,
                                     fs_matrix_engine<T1, R1, C1>,
                                     matrix_transpose_view<fs_matrix_engine<T2, R2, C2>>>
{
    static_assert(R1 == C2);
    static_assert(C1 == R2);
    using element_type = matrix_addition_element_t<OT, T1, T2>;
    using engine_type  = fs_matrix_engine<element_type, R1, C1>;
};

template<class OT, class T1, int32_t R1, int32_t C1, class T2, int32_t R2, int32_t C2>
struct matrix_addition_engine_traits<OT,
                                     matrix_transpose_view<fs_matrix_engine<T1, R1, C1>>,
                                     matrix_transpose_view<fs_matrix_engine<T2, R2, C2>>>
{
    static_assert(R1 == R2);
    static_assert(C1 == C2);
    using element_type = matrix_addition_element_t<OT, T1, T2>;
    using engine_type  = fs_matrix_engine<element_type, C1, R1>;
};


//==================================================================================================
//                                 **** ADDITION TRAITS ****
//==================================================================================================
//
//- Alias interface to detection meta-function that extracts the addition traits type.
//
template<class OT, class OP1, class OP2>
using matrix_addition_traits_t = detail::addition_traits_t<OT, OP1, OP2>;


//- The standard addition traits type provides the default mechanism for computing the result
//  of a matrix/matrix or vector/vector addition.
//
template<class OT, class ET1, class OT1, class ET2, class OT2>
struct matrix_addition_traits<OT, vector<ET1, OT1>, vector<ET2, OT2>>
{
    using engine_type = matrix_addition_engine_t<OT, ET1, ET2>;
    using op_traits   = OT;
    using result_type = vector<engine_type, op_traits>;

    static result_type  add(vector<ET1, OT1> const& v1, vector<ET2, OT2> const& v2);
};

//------
//
template<class OT, class ET1, class OT1, class ET2, class OT2>
struct matrix_addition_traits<OT, matrix<ET1, OT1>, matrix<ET2, OT2>>
{
    using engine_type = matrix_addition_engine_t<OT, ET1, ET2>;
    using op_traits   = OT;
    using result_type = matrix<engine_type, op_traits>;

    static result_type  add(matrix<ET1, OT1> const& m1, matrix<ET2, OT2> const& m2);
};

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_ADDITION_TRAITS_HPP_DEFINED
//==================================================================================================
//  File:       addition_traits_impl.hpp
//
//  Summary:    This header defines the static member functions of matrix_addition_traits that
//              perform the actual arithmetic.
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_ADDITION_TRAITS_IMPL_HPP_DEFINED
#define LINEAR_ALGEBRA_ADDITION_TRAITS_IMPL_HPP_DEFINED

namespace STD_LA {
//=================================================================================================
//                        **** ADDITION TRAITS FUNCTION IMPLEMENTATION ****
//==================================================================================================
//
template<class OT, class ET1, class OT1, class ET2, class OT2>
inline auto
matrix_addition_traits<OT, vector<ET1, OT1>, vector<ET2, OT2>>::add
(vector<ET1, OT1> const& v1, vector<ET2, OT2> const& v2) -> result_type
{
    PrintOperandTypes<result_type>("addition_traits", v1, v2);

    result_type     vr;

    if constexpr (result_requires_resize(vr))
    {
        vr.resize(v1.elements());
    }

    transform(v1.begin(), v1.end(), v2.begin(), vr.begin(), [](auto lhs, auto rhs) { return lhs + rhs; });

    return vr;
}

//------
//
template<class OT, class ET1, class OT1, class ET2, class OT2>
inline auto
matrix_addition_traits<OT, matrix<ET1, OT1>, matrix<ET2, OT2>>::add
(matrix<ET1, OT1> const& m1, matrix<ET2, OT2> const& m2) -> result_type
{
    PrintOperandTypes<result_type>("addition_traits", m1, m2);

	result_type		mr{};                           //- Braces here to avoid C4701 from MSVC
	auto const      rows = m1.rows();
	auto const      columns = m1.columns();

	if constexpr (result_requires_resize(mr))
	{
		mr.resize(rows, columns);
    }

	for (auto i = 0;  i < rows;  ++i)
    {
		for (auto j = 0;  j < columns;  ++j)
        {
			mr(i, j) = m1(i, j) + m2(i, j);
        }
    }

/*
	if constexpr (result_requires_resize(mr))
	{
		mr.resize(rows, columns);
		auto data = mr.data();
		for (auto i = 0; i < columns; ++i)
			for (auto j = 0; j < rows; ++j)
				*data++ = m1(i, j) + m2(i, j);		// Safe because the resize means that mr capacity = size for rows and columns.
	}
	else
	{
		transform(m1.data(), m1.data() + (rows * columns), m2.data(), mr.data(),
			[](auto lhs, auto rhs) {return lhs + rhs; });
	}
*/

	return mr;
}

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_ADDITION_TRAITS_IMPL_HPP_DEFINED
//==================================================================================================
//  File:       subtraction_traits.hpp
//
//  Summary:    This header defines several private and public traits types that are used to
//              implement the binary subtraction operator.  This is a somewhat long file, divided
//              into two main sections, each with three sub-sections:
//                  1. Private traits definitions that:
//                      A. determine the element promotion traits to be used for subtraction
//                      B. determine the engine promotion traits to be used for subtraction
//                      C. determine the arithmetic traits to be used to perform subtraction
//                  2. Public traits definitions that:
//                      A. perform element promotion for subtraction
//                      B. perform engine promotion for subtraction
//                      C. perform the actual act of subtraction
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_SUBTRACTION_TRAITS_HPP_DEFINED
#define LINEAR_ALGEBRA_SUBTRACTION_TRAITS_HPP_DEFINED

namespace STD_LA {
namespace detail {
//==================================================================================================
//                          **** ELEMENT SUBTRACTION TRAITS DETECTORS ****
//==================================================================================================
//
//- Form 1 type detection of nested element subtraction traits.
//
template<typename OT>
using element_sub_traits_f1_t = typename OT::element_subtraction_traits;

template<typename OT>
using element_sub_type_f1_t = typename element_sub_traits_f1_t<OT>::element_type;

//- Define the form 1 detectors.
//
template<typename OT, typename = void>
struct detect_element_sub_traits_f1
:   public false_type
{
    using traits_type = void;
};

template<typename OT>
struct detect_element_sub_traits_f1<OT, void_t<element_sub_type_f1_t<OT>>>
:   public true_type
{
    using traits_type = element_sub_traits_f1_t<OT>;
};

//-------------------------------------------------------------
//- Form 2 type detection of nested element subtraction traits.
//
template<typename OT, typename T1, typename T2>
using element_sub_traits_f2_t = typename OT::template element_subtraction_traits<T1, T2>;

template<typename OT, typename T1, typename T2>
using element_sub_type_f2_t = typename element_sub_traits_f2_t<OT, T1, T2>::element_type;

//- Define the form 2 detectors.
//
template<typename OT, typename T1, typename T2, typename = void>
struct detect_element_sub_traits_f2
:   public false_type
{
    using traits_type = void;
};

template<typename OT, typename T1, typename T2>
struct detect_element_sub_traits_f2<OT, T1, T2, void_t<element_sub_type_f2_t<OT, T1, T2>>>
:   public true_type
{
    using traits_type = element_sub_traits_f2_t<OT, T1, T2>;
};

//------------------------------------------------
//- Element subtraction traits type determination.
//
template<typename OT, typename T1, typename T2>
struct element_sub_traits_chooser
{
    using CT1 = typename detect_element_sub_traits_f1<OT>::traits_type;
    using CT2 = typename detect_element_sub_traits_f2<OT, T1, T2>::traits_type;
    using DEF = matrix_subtraction_element_traits<T1, T2>;

    using traits_type = typename non_void_traits_chooser<CT1, CT2, DEF>::traits_type;
};

template<typename OT, typename T1, typename T2>
using element_sub_traits_t = typename element_sub_traits_chooser<OT, T1, T2>::traits_type;

template<class OT, class T1, class T2>
constexpr bool  has_element_sub_traits_v = detect_element_sub_traits_f2<OT, T1, T2>::value ||
                                           detect_element_sub_traits_f1<OT>::value;

//------------------------------------------------
//- Element subtraction result type determination.
//
template<typename OT, typename T1, typename T2>
struct element_sub_type
{
    using traits_type  = typename element_sub_traits_chooser<OT, T1, T2>::traits_type;
    using element_type = typename traits_type::element_type;
};

template<typename OT, typename T1, typename T2>
using element_sub_type_t = typename element_sub_type<OT, T1, T2>::element_type;


//==================================================================================================
//                           **** ENGINE SUBTRACTION TRAITS DETECTORS ****
//==================================================================================================
//
//- Form 1 type detection of nested engine subtraction traits.
//
template<typename OT>
using engine_sub_traits_f1_t = typename OT::engine_subtraction_traits;

template<typename OT>
using engine_sub_type_f1_t = typename engine_sub_traits_f1_t<OT>::engine_type;

//- Define the form 1 detectors.
//
template<typename OT, typename = void>
struct detect_engine_sub_traits_f1
:   public false_type
{
    using traits_type = void;
};

template<typename OT>
struct detect_engine_sub_traits_f1<OT, void_t<engine_sub_type_f1_t<OT>>>
:   public true_type
{
    using traits_type = engine_sub_traits_f1_t<OT>;
};

//------------------------------------------------------------
//- Form 2 type detection of nested engine subtraction traits.
//
template<typename OT, typename T1, typename T2>
using engine_sub_traits_f2_t = typename OT::template engine_subtraction_traits<OT, T1, T2>;

template<typename OT, typename T1, typename T2>
using engine_sub_type_f2_t = typename engine_sub_traits_f2_t<OT, T1, T2>::engine_type;

//- Define the form 2 detectors.
//
template<typename OT, typename ET1, typename ET2, typename = void>
struct detect_engine_sub_traits_f2
:   public false_type
{
    using traits_type = void;
};

template<typename OT, typename ET1, typename ET2>
struct detect_engine_sub_traits_f2<OT, ET1, ET2, void_t<engine_sub_type_f2_t<OT, ET1, ET2>>>
:   public true_type
{
    using traits_type = engine_sub_traits_f2_t<OT, ET1, ET2>;
};

//-----------------------------------------------
//- Engine subtraction traits type determination.
//
template<typename OT, typename ET1, typename ET2>
struct engine_sub_traits_chooser
{
    using CT1 = typename detect_engine_sub_traits_f1<OT>::traits_type;
    using CT2 = typename detect_engine_sub_traits_f2<OT, ET1, ET2>::traits_type;
    using DEF = matrix_subtraction_engine_traits<OT, ET1, ET2>;

    using traits_type = typename non_void_traits_chooser<CT1, CT2, DEF>::traits_type;
};

template<typename OT, typename ET1, typename ET2>
using engine_sub_traits_t = typename engine_sub_traits_chooser<OT, ET1, ET2>::traits_type;

template<class OT, class ET1, class ET2>
constexpr bool  has_engine_sub_traits_v = detect_engine_sub_traits_f2<OT, ET1, ET2>::value ||
                                          detect_engine_sub_traits_f1<OT>::value;

//-----------------------------------------------
//- Engine subtraction result type determination.
//
template<typename OT, typename ET1, typename ET2>
struct engine_sub_type
{
    using traits_type = typename engine_sub_traits_chooser<OT, ET1, ET2>::traits_type;
    using engine_type = typename traits_type::engine_type;
};

template<typename OT, typename ET1, typename ET2>
using engine_sub_type_t = typename engine_sub_type<OT, ET1, ET2>::engine_type;


//==================================================================================================
//                        **** SUBTRACTION ARITHMETIC TRAITS DETECTORS ****
//==================================================================================================
//
//- Form 1 type detection of nested subtraction arithmetic traits.
//
template<typename OT>
using sub_traits_f1_t = typename OT::subtraction_traits;

template<typename OT>
using sub_type_f1_t = typename sub_traits_f1_t<OT>::result_type;

//- Define the form 1 detectors.
//
template<typename OT, typename = void>
struct detect_sub_traits_f1
:   public false_type
{
    using traits_type = void;
};

template<typename OT>
struct detect_sub_traits_f1<OT, void_t<sub_type_f1_t<OT>>>
:   public true_type
{
    using traits_type = sub_traits_f1_t<OT>;
};

//----------------------------------------------------------------
//- Form 2 type detection of nested subtraction arithmetic traits.
//
template<typename OT, typename T1, typename T2>
using sub_traits_f2_t = typename OT::template subtraction_traits<OT, T1, T2>;

template<typename OT, typename T1, typename T2>
using sub_type_f2_t = typename sub_traits_f2_t<OT, T1, T2>::result_type;

//- Define the form 2 detectors.
//
template<typename OT, typename OP1, typename OP2, typename = void>
struct detect_sub_traits_f2
:   public false_type
{
    using traits_type = void;
};

template<typename OT, typename OP1, typename OP2>
struct detect_sub_traits_f2<OT, OP1, OP2, void_t<sub_type_f2_t<OT, OP1, OP2>>>
:   public true_type
{
    using traits_type = typename OT::template subtraction_traits<OT, OP1, OP2>;
};

//---------------------------------------------------
//- Subtraction arithmetic traits type determination.
//
template<typename OT, typename OP1, typename OP2>
struct sub_traits_chooser
{
    using CT1 = typename detect_sub_traits_f1<OT>::traits_type;
    using CT2 = typename detect_sub_traits_f2<OT, OP1, OP2>::traits_type;
    using DEF = matrix_subtraction_traits<OT, OP1, OP2>;

    using traits_type = typename non_void_traits_chooser<CT1, CT2, DEF>::traits_type;
};

template<typename OT, typename OP1, typename OP2>
using subtraction_traits_t = typename sub_traits_chooser<OT, OP1, OP2>::traits_type;

template<class OT, class OP1, class OP2>
constexpr bool  has_sub_traits_v = detect_sub_traits_f2<OT, OP1, OP2>::value ||
                                   detect_sub_traits_f1<OT>::value;


}       //- detail namespace
//==================================================================================================
//                              **** ELEMENT SUBTRACTION TRAITS ****
//==================================================================================================
//
//- Alias interface to detection meta-function that extracts the element subtraction traits type.
//
template<class OT, class T1, class T2>
using matrix_subtraction_element_t = detail::element_sub_type_t<OT, T1, T2>;


//- The standard element subtraction traits type provides the default mechanism for determining the
//  result of subtracting two elements of (possibly) different types.
//
template<class T1, class T2>
struct matrix_subtraction_element_traits
{
    using element_type = decltype(declval<T1>() - declval<T2>());
};


//==================================================================================================
//                                **** ENGINE SUBTRACTION TRAITS ****
//==================================================================================================
//
//- Alias interface to detection meta-function that extracts the engine subtraction traits type.
//
template<class OT, class ET1, class ET2>
using matrix_subtraction_engine_t = detail::engine_sub_type_t<OT, ET1, ET2>;


//- The standard engine subtraction traits type provides the default mechanism for determining the
//  correct engine type for a matrix/matrix or vector/vector subtraction.
//
template<class OT, class ET1, class ET2>
struct matrix_subtraction_engine_traits
{
    static_assert(detail::engines_match_v<ET1, ET2>);

    using element_type_1 = typename ET1::element_type;
    using element_type_2 = typename ET2::element_type;
    using element_type   = matrix_subtraction_element_t<OT, element_type_1, element_type_2>;
    using engine_type    = conditional_t<detail::is_matrix_engine_v<ET1>,
                                         dr_matrix_engine<element_type, allocator<element_type>>,
                                         dr_vector_engine<element_type, allocator<element_type>>>;
};

//- General transpose cases for matrices.
//
template<class OT, class ET1, class ET2>
struct matrix_subtraction_engine_traits<OT, matrix_transpose_view<ET1>, ET2>
{
    using engine_type = typename matrix_subtraction_engine_traits<OT, ET1, ET2>::engine_type;
};

template<class OT, class ET1, class ET2>
struct matrix_subtraction_engine_traits<OT, ET1, matrix_transpose_view<ET2>>
{
    using engine_type = typename matrix_subtraction_engine_traits<OT, ET1, ET2>::engine_type;
};

template<class OT, class ET1, class ET2>
struct matrix_subtraction_engine_traits<OT, matrix_transpose_view<ET1>, matrix_transpose_view<ET2>>
{
    using engine_type = typename matrix_subtraction_engine_traits<OT, ET1, ET2>::engine_type;
};

//--------------------------------------------------------------------------------------------------
//- ENGINE - ENGINE cases for vector/vector.  Note that all partial specializations below in which
//  allocators are rebound assume standard-conformant allocator types.
//
//- dr_vector_engine - dr_vector_engine.
//
template<class OT, class T1, class A1, class T2, class A2>
struct matrix_subtraction_engine_traits<OT, dr_vector_engine<T1, A1>, dr_vector_engine<T2, A2>>
{
    using element_type = matrix_subtraction_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<A1, element_type>;
    using engine_type  = dr_vector_engine<element_type, alloc_type>;
};

//--------------------------------------
//- dr_vector_engine - fs_vector_engine.
//
template<class OT, class T1, class A1, class T2, int32_t N2>
struct matrix_subtraction_engine_traits<OT, dr_vector_engine<T1, A1>, fs_vector_engine<T2, N2>>
{
    using element_type = matrix_subtraction_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<A1, element_type>;
    using engine_type  = dr_vector_engine<element_type, alloc_type>;
};

//--------------------------------------
//- fs_vector_engine - dr_vector_engine.
//
template<class OT, class T1, int32_t N1, class T2, class A2>
struct matrix_subtraction_engine_traits<OT, fs_vector_engine<T1, N1>, dr_vector_engine<T2, A2>>
{
    using element_type = matrix_subtraction_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<A2, element_type>;
    using engine_type  = dr_vector_engine<element_type, alloc_type>;
};

//--------------------------------------
//- fs_vector_engine - fs_vector_engine.
//
template<class OT, class T1, int32_t N1, class T2, int32_t N2>
struct matrix_subtraction_engine_traits<OT, fs_vector_engine<T1, N1>, fs_vector_engine<T2, N2>>
{
    static_assert(N1 == N2);
    using element_type = matrix_subtraction_element_t<OT, T1, T2>;
    using engine_type  = fs_vector_engine<element_type, N1>;
};

//--------------------------------------------------------------------------------------------------
//- ENGINE - ENGINE cases for matrix/matrix.  Note that all partial specializations below in which
//  allocators are rebound assume standard-conformant allocator types.
//
//- dr_matrix_engine - dr_matrix_engine.
//
template<class OT, class T1, class A1, class T2, class A2>
struct matrix_subtraction_engine_traits<OT,
                                        dr_matrix_engine<T1, A1>,
                                        dr_matrix_engine<T2, A2>>
{
    using element_type = matrix_subtraction_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<A1, element_type>;
    using engine_type  = dr_matrix_engine<element_type, alloc_type>;
};

template<class OT, class T1, class A1, class T2, class A2>
struct matrix_subtraction_engine_traits<OT,
                                        dr_matrix_engine<T1, A1>,
                                        matrix_transpose_view<dr_matrix_engine<T2, A2>>>
{
    using element_type = matrix_subtraction_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<A1, element_type>;
    using engine_type  = dr_matrix_engine<element_type, alloc_type>;
};

template<class OT, class T1, class A1, class T2, class A2>
struct matrix_subtraction_engine_traits<OT,
                                        matrix_transpose_view<dr_matrix_engine<T1, A1>>,
                                        dr_matrix_engine<T2, A2>>
{
    using element_type = matrix_subtraction_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<A1, element_type>;
    using engine_type  = dr_matrix_engine<element_type, alloc_type>;
};

template<class OT, class T1, class A1, class T2, class A2>
struct matrix_subtraction_engine_traits<OT,
                                        matrix_transpose_view<dr_matrix_engine<T1, A1>>,
                                        matrix_transpose_view<dr_matrix_engine<T2, A2>>>
{
    using element_type = matrix_subtraction_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<A1, element_type>;
    using engine_type  = dr_matrix_engine<element_type, alloc_type>;
};

//--------------------------------------
//- dr_matrix_engine - fs_matrix_engine.
//
template<class OT, class T1, class A1, class T2, int32_t R2, int32_t C2>
struct matrix_subtraction_engine_traits<OT,
                                        dr_matrix_engine<T1, A1>,
                                        fs_matrix_engine<T2, R2, C2>>
{
    using element_type = matrix_subtraction_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<A1, element_type>;
    using engine_type  = dr_matrix_engine<element_type, alloc_type>;
};

template<class OT, class T1, class A1, class T2, int32_t R2, int32_t C2>
struct matrix_subtraction_engine_traits<OT,
                                        dr_matrix_engine<T1, A1>,
                                        matrix_transpose_view<fs_matrix_engine<T2, R2, C2>>>
{
    using element_type = matrix_subtraction_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<A1, element_type>;
    using engine_type  = dr_matrix_engine<element_type, alloc_type>;
};

template<class OT, class T1, class A1, class T2, int32_t R2, int32_t C2>
struct matrix_subtraction_engine_traits<OT,
                                        matrix_transpose_view<dr_matrix_engine<T1, A1>>,
                                        fs_matrix_engine<T2, R2, C2>>
{
    using element_type = matrix_subtraction_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<A1, element_type>;
    using engine_type  = dr_matrix_engine<element_type, alloc_type>;
};

template<class OT, class T1, class A1, class T2, int32_t R2, int32_t C2>
struct matrix_subtraction_engine_traits<OT,
                                        matrix_transpose_view<dr_matrix_engine<T1, A1>>,
                                        matrix_transpose_view<fs_matrix_engine<T2, R2, C2>>>
{
    using element_type = matrix_subtraction_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<A1, element_type>;
    using engine_type  = dr_matrix_engine<element_type, alloc_type>;
};

//--------------------------------------
//- fs_matrix_engine - dr_matrix_engine.
//
template<class OT, class T1, int32_t R1, int32_t C1, class T2, class A2>
struct matrix_subtraction_engine_traits<OT,
                                        fs_matrix_engine<T1, R1, C1>,
                                        dr_matrix_engine<T2, A2>>
{
    using element_type = matrix_subtraction_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<A2, element_type>;
    using engine_type  = dr_matrix_engine<element_type, alloc_type>;
};

template<class OT, class T1, int32_t R1, int32_t C1, class T2, class A2>
struct matrix_subtraction_engine_traits<OT,
                                        fs_matrix_engine<T1, R1, C1>,
                                        matrix_transpose_view<dr_matrix_engine<T2, A2>>>
{
    using element_type = matrix_subtraction_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<A2, element_type>;
    using engine_type  = dr_matrix_engine<element_type, alloc_type>;
};

template<class OT, class T1, int32_t R1, int32_t C1, class T2, class A2>
struct matrix_subtraction_engine_traits<OT,
                                        matrix_transpose_view<fs_matrix_engine<T1, R1, C1>>,
                                        dr_matrix_engine<T2, A2>>
{
    using element_type = matrix_subtraction_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<A2, element_type>;
    using engine_type  = dr_matrix_engine<element_type, alloc_type>;
};

template<class OT, class T1, int32_t R1, int32_t C1, class T2, class A2>
struct matrix_subtraction_engine_traits<OT,
                                        matrix_transpose_view<fs_matrix_engine<T1, R1, C1>>,
                                        matrix_transpose_view<dr_matrix_engine<T2, A2>>>
{
    using element_type = matrix_subtraction_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<A2, element_type>;
    using engine_type  = dr_matrix_engine<element_type, alloc_type>;
};

//--------------------------------------
//- fs_matrix_engine - fs_matrix_engine.
//
template<class OT, class T1, int32_t R1, int32_t C1, class T2, int32_t R2, int32_t C2>
struct matrix_subtraction_engine_traits<OT,
                                        fs_matrix_engine<T1, R1, C1>,
                                        fs_matrix_engine<T2, R2, C2>>
{
    static_assert(R1 == R2);
    static_assert(C1 == C2);
    using element_type = matrix_subtraction_element_t<OT, T1, T2>;
    using engine_type  = fs_matrix_engine<element_type, R1, C1>;
};

template<class OT, class T1, int32_t R1, int32_t C1, class T2, int32_t R2, int32_t C2>
struct matrix_subtraction_engine_traits<OT,
                                        matrix_transpose_view<fs_matrix_engine<T1, R1, C1>>,
                                        fs_matrix_engine<T2, R2, C2>>
{
    static_assert(R1 == C2);
    static_assert(C1 == R2);
    using element_type = matrix_subtraction_element_t<OT, T1, T2>;
    using engine_type  = fs_matrix_engine<element_type, R2, C2>;
};

template<class OT, class T1, int32_t R1, int32_t C1, class T2, int32_t R2, int32_t C2>
struct matrix_subtraction_engine_traits<OT,
                                        fs_matrix_engine<T1, R1, C1>,
                                        matrix_transpose_view<fs_matrix_engine<T2, R2, C2>>>
{
    static_assert(R1 == C2);
    static_assert(C1 == R2);
    using element_type = matrix_subtraction_element_t<OT, T1, T2>;
    using engine_type  = fs_matrix_engine<element_type, R1, C1>;
};

template<class OT, class T1, int32_t R1, int32_t C1, class T2, int32_t R2, int32_t C2>
struct matrix_subtraction_engine_traits<OT,
                                        matrix_transpose_view<fs_matrix_engine<T1, R1, C1>>,
                                        matrix_transpose_view<fs_matrix_engine<T2, R2, C2>>>
{
    static_assert(R1 == R2);
    static_assert(C1 == C2);
    using element_type = matrix_subtraction_element_t<OT, T1, T2>;
    using engine_type  = fs_matrix_engine<element_type, C1, R1>;
};


//==================================================================================================
//                                   **** SUBTRACTION TRAITS ****
//==================================================================================================
//
//- Alias interface to detection meta-function that extracts the subtraction traits type.
//
template<class OT, class OP1, class OP2>
using matrix_subtraction_traits_t = detail::subtraction_traits_t<OT, OP1, OP2>;


//- The standard subtraction traits type provides the default mechanism for computing the result
//  of a matrix/matrix or vector/vector subtraction.
//
template<class OT, class ET1, class OT1, class ET2, class OT2>
struct matrix_subtraction_traits<OT, vector<ET1, OT1>, vector<ET2, OT2>>
{
    using engine_type = matrix_subtraction_engine_t<OT, ET1, ET2>;
    using op_traits   = OT;
    using result_type = vector<engine_type, op_traits>;

    static result_type  subtract(vector<ET1, OT1> const& v1, vector<ET2, OT2> const& v2);
};

//------
//
template<class OT, class ET1, class OT1, class ET2, class OT2>
struct matrix_subtraction_traits<OT, matrix<ET1, OT1>, matrix<ET2, OT2>>
{
    using engine_type = matrix_subtraction_engine_t<OT, ET1, ET2>;
    using op_traits   = OT;
    using result_type = matrix<engine_type, op_traits>;

    static result_type  subtract(matrix<ET1, OT1> const& m1, matrix<ET2, OT2> const& m2);
};

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_SUBTRACTION_TRAITS_HPP_DEFINED
//==================================================================================================
//  File:       subtraction_traits_impl.hpp
//
//  Summary:    This header defines the static member functions of matrix_subtraction_traits that
//              perform the actual arithmetic.
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_SUBTRACTION_TRAITS_IMPL_HPP_DEFINED
#define LINEAR_ALGEBRA_SUBTRACTION_TRAITS_IMPL_HPP_DEFINED

namespace STD_LA {
//==================================================================================================
//                       **** SUBTRACTION TRAITS FUNCTION IMPLEMENTATION ****
//==================================================================================================
//
template<class OT, class ET1, class OT1, class ET2, class OT2>
inline auto
matrix_subtraction_traits<OT, vector<ET1, OT1>, vector<ET2, OT2>>::subtract
(vector<ET1, OT1> const& v1, vector<ET2, OT2> const& v2) -> result_type
{
    PrintOperandTypes<result_type>("subtraction_traits", v1, v2);

	result_type     vr;

	if constexpr (result_requires_resize(vr))
	{
		vr.resize(v1.elements());
	}

	transform(v1.begin(), v1.end(), v2.begin(), vr.begin(), [](auto lhs, auto rhs){return lhs - rhs;});

	return vr;
}

//------
//
template<class OT, class ET1, class OT1, class ET2, class OT2>
inline auto
matrix_subtraction_traits<OT, matrix<ET1, OT1>, matrix<ET2, OT2>>::subtract
(matrix<ET1, OT1> const& m1, matrix<ET2, OT2> const& m2) -> result_type
{
    PrintOperandTypes<result_type>("subtraction_traits", m1, m2);

	result_type		mr{};                           //- Braces here to avoid C4701 from MSVC
	auto const      rows = m1.rows();
	auto const      columns = m1.columns();

	if constexpr (result_requires_resize(mr))
	{
		mr.resize(rows, columns);
    }

	for (auto i = 0;  i < rows;  ++i)
    {
		for (auto j = 0;  j < columns;  ++j)
        {
			mr(i, j) = m1(i, j) - m2(i, j);
        }
    }
/*
	if constexpr (result_requires_resize(mr))
	{
		mr.resize(rows, columns);
		auto data = mr.data();
		for (auto i = 0; i < columns; ++i)
			for (auto j = 0; j < rows; ++j)
				*data++ = m1(i, j) - m2(i, j);		// Safe because the resize means that mr capacity = size for rows and columns.
	}
	else
	{
//		transform(m1.begin(), m1.data() + (rows * columns), m2.data(), mr.data(),
//			[](auto lhs, auto rhs) {return lhs - rhs; });
	}
*/
	return mr;
}

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_SUBTRACTION_TRAITS_IMPL_HPP_DEFINED
//==================================================================================================
//  File:       negation_traits.hpp
//
//  Summary:    This header defines several private and public traits types that are used to
//              implement the unary negation operator.  This is a somewhat long file, divided
//              into two main sections, each with three sub-sections:
//                  1. Private traits definitions that:
//                      A. determine the element promotion traits to be used for negation
//                      B. determine the engine promotion traits to be used for negation
//                      C. determine the arithmetic traits to be used to perform negation
//                  2. Public traits definitions that:
//                      A. perform element promotion for negation
//                      B. perform engine promotion for negation
//                      C. perform the actual act of negation
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_NEGATION_TRAITS_HPP_DEFINED
#define LINEAR_ALGEBRA_NEGATION_TRAITS_HPP_DEFINED

namespace STD_LA {
namespace detail {
//==================================================================================================
//                          **** ELEMENT NEGATION TRAITS DETECTORS ****
//==================================================================================================
//
//- Form 1 type detection of nested element negation traits.
//
template<typename OT>
using element_neg_traits_f1_t = typename OT::element_negation_traits;

template<typename OT>
using element_neg_type_f1_t = typename element_neg_traits_f1_t<OT>::element_type;

//- Define the form 1 detectors.
//
template<typename OT, typename = void>
struct detect_element_neg_traits_f1
:   public false_type
{
    using traits_type = void;
};

template<typename OT>
struct detect_element_neg_traits_f1<OT, void_t<element_neg_type_f1_t<OT>>>
:   public true_type
{
    using traits_type = element_neg_traits_f1_t<OT>;
};

//----------------------------------------------------------
//- Form 2 type detection of nested element negation traits.
//  First, define two helper aliases.
//
template<typename OT, typename T1>
using element_neg_traits_f2_t = typename OT::template element_negation_traits<T1>;

template<typename OT, typename T1>
using element_neg_type_f2_t = typename element_neg_traits_f2_t<OT, T1>::element_type;


//- Define the form 2 detectors.
//
template<typename OT, typename T1, typename = void>
struct detect_element_neg_traits_f2
:   public false_type
{
    using traits_type = void;
};

template<typename OT, typename T1>
struct detect_element_neg_traits_f2<OT, T1, void_t<element_neg_type_f2_t<OT, T1>>>
:   public true_type
{
    using traits_type = element_neg_traits_f2_t<OT, T1>;
};

//---------------------------------------------
//- Element negation traits type determination.
//
template<typename OT, typename T1>
struct element_neg_traits_chooser
{
    using CT1 = typename detect_element_neg_traits_f1<OT>::traits_type;
    using CT2 = typename detect_element_neg_traits_f2<OT, T1>::traits_type;
    using DEF = matrix_negation_element_traits<T1>;

    using traits_type = typename non_void_traits_chooser<CT1, CT2, DEF>::traits_type;
};

template<typename OT, typename T1>
using element_neg_traits_t = typename element_neg_traits_chooser<OT, T1>::traits_type;

template<class OT, class T1>
constexpr bool  has_element_neg_traits_v = detect_element_neg_traits_f2<OT, T1>::value ||
                                           detect_element_neg_traits_f1<OT>::value;

//---------------------------------------------
//- Element negation result type determination.
//
template<typename OT, typename T1>
struct element_neg_type
{
    using traits_type  = typename element_neg_traits_chooser<OT, T1>::traits_type;
    using element_type = typename traits_type::element_type;
};

template<typename OT, typename T1>
using element_neg_type_t = typename element_neg_type<OT, T1>::element_type;


//==================================================================================================
//                           **** ENGINE NEGATION TRAITS DETECTORS ****
//==================================================================================================
//
//- Form 1 type detection of nested engine negation traits.
//
template<typename OT>
using engine_neg_traits_f1_t = typename OT::engine_negation_traits;

template<typename OT>
using engine_neg_type_f1_t = typename engine_neg_traits_f1_t<OT>::engine_type;

//- Define the form 1 detectors.
//
template<typename OT, typename = void>
struct detect_engine_neg_traits_f1
:   public false_type
{
    using traits_type = void;
};

template<typename OT>
struct detect_engine_neg_traits_f1<OT, void_t<engine_neg_type_f1_t<OT>>>
:   public true_type
{
    using traits_type = engine_neg_traits_f1_t<OT>;
};

//---------------------------------------------------------
//- Form 2 type detection of nested engine negation traits.
//
template<typename OT, typename T1>
using engine_neg_traits_f2_t = typename OT::template engine_negation_traits<OT, T1>;

template<typename OT, typename T1>
using engine_neg_type_f2_t = typename engine_neg_traits_f2_t<OT, T1>::engine_type;

//- Define the form 2 detectors.
//
template<typename OT, typename ET1, typename = void>
struct detect_engine_neg_traits_f2
:   public false_type
{
    using traits_type = void;
};

template<typename OT, typename ET1>
struct detect_engine_neg_traits_f2<OT, ET1, void_t<engine_neg_type_f2_t<OT, ET1>>>
:   public true_type
{
    using traits_type = engine_neg_traits_f2_t<OT, ET1>;
};

//--------------------------------------------
//- Engine negation traits type determination.
//
template<typename OT, typename ET1>
struct engine_neg_traits_chooser
{
    using CT1 = typename detect_engine_neg_traits_f1<OT>::traits_type;
    using CT2 = typename detect_engine_neg_traits_f2<OT, ET1>::traits_type;
    using DEF = matrix_negation_engine_traits<OT, ET1>;

    using traits_type = typename non_void_traits_chooser<CT1, CT2, DEF>::traits_type;
};

template<typename OT, typename ET1>
using engine_neg_traits_t = typename engine_neg_traits_chooser<OT, ET1>::traits_type;

template<class OT, class ET1>
constexpr bool  has_engine_neg_traits_v = detect_engine_neg_traits_f2<OT, ET1>::value ||
                                          detect_engine_neg_traits_f1<OT>::value;

//--------------------------------------------
//- Engine negation result type determination.
//
template<typename OT, typename ET1>
struct engine_neg_type
{
    using traits_type = typename engine_neg_traits_chooser<OT, ET1>::traits_type;
    using engine_type = typename traits_type::engine_type;
};

template<typename OT, typename ET1>
using engine_neg_type_t = typename engine_neg_type<OT, ET1>::engine_type;


//==================================================================================================
//                      **** NEGATION ARITHMETIC TRAITS AND DETECTORS ****
//==================================================================================================
//
//- Form 1 type detection of nested negation arithmetic traits.
//
template<typename OT>
using neg_traits_f1_t = typename OT::negation_traits;

template<typename OT>
using neg_type_f1_t = typename neg_traits_f1_t<OT>::result_type;


//- Define the form 1 detectors.
//
template<typename OT, typename = void>
struct detect_neg_traits_f1
:   public false_type
{
    using traits_type = void;
};

template<typename OT>
struct detect_neg_traits_f1<OT, void_t<neg_type_f1_t<OT>>>
:   public true_type
{
    using traits_type = neg_traits_f1_t<OT>;
};

//-------------------------------------------------------------
//- Form 2 type detection of nested negation arithmetic traits.
//
template<typename OT, typename T1>
using neg_traits_f2_t = typename OT::template negation_traits<OT, T1>;

template<typename OT, typename T1>
using neg_type_f2_t = typename neg_traits_f2_t<OT, T1>::result_type;

//- Define the form 2 detectors.
//
template<typename OT, typename OP1, typename = void>
struct detect_neg_traits_f2
:   public false_type
{
    using traits_type = void;
};

template<typename OT, typename OP1>
struct detect_neg_traits_f2<OT, OP1, void_t<neg_type_f2_t<OT, OP1>>>
:   public true_type
{
    using traits_type = typename OT::template negation_traits<OT, OP1>;
};

//------------------------------------------------
//- Addition arithmetic traits type determination.
//
template<typename OT, typename OP1>
struct neg_traits_chooser
{
    using CT1 = typename detect_neg_traits_f1<OT>::traits_type;
    using CT2 = typename detect_neg_traits_f2<OT, OP1>::traits_type;
    using DEF = matrix_negation_traits<OT, OP1>;

    using traits_type = typename non_void_traits_chooser<CT1, CT2, DEF>::traits_type;
};

template<typename OT, typename OP1>
using negation_traits_t = typename neg_traits_chooser<OT, OP1>::traits_type;

template<class OT, class OP1>
constexpr bool  has_neg_traits_v = detect_neg_traits_f2<OT, OP1>::value ||
                                   detect_neg_traits_f1<OT>::value;


}       //- detail namespace
//==================================================================================================
//                                 **** ELEMENT NEGATION TRAITS ****
//==================================================================================================
//
//- Alias interface to detection meta-function that extracts the element negation traits type.
//
template<class OT, class T1>
using matrix_negation_element_t = detail::element_neg_type_t<OT, T1>;


//- The standard element negation traits type provides the default mechanism for determining the
//  result of negating a vector or matrix element.
//
template<class T1>
struct matrix_negation_element_traits
{
    using element_type = decltype(-declval<T1>());
};


//==================================================================================================
//                                   **** ENGINE NEGATION TRAITS ****
//==================================================================================================
//
//- Alias interface to detection meta-function that extracts the engine negation traits type.
//- Alias interface to trait.
//
template<class OT, class ET1>
using matrix_negation_engine_t = detail::engine_neg_type_t<OT, ET1>;


//- The standard engine negation traits type provides the default mechanism for determining the
//  correct engine type for a matrix or vector negation.
//
template<class OT, class ET1>
struct matrix_negation_engine_traits
{
    using element_type_1 = typename ET1::element_type;
    using element_type   = matrix_negation_element_t<OT, element_type_1>;
    using engine_type    = conditional_t<detail::is_matrix_engine_v<ET1>,
                                         dr_matrix_engine<element_type, allocator<element_type>>,
                                         dr_vector_engine<element_type, allocator<element_type>>>;
};

//- General transpose cases for matrices.
//
template<class OT, class ET1>
struct matrix_negation_engine_traits<OT, matrix_transpose_view<ET1>>
{
    using element_type = matrix_negation_element_t<OT, typename ET1::element_type>;
    using engine_type  = typename matrix_negation_engine_traits<OT, ET1>::engine_type;
};

//--------------------------------------------------------------------------------------------------
//  -ENGINE case for vector.  Note that all partial specializations below in which allocators
//  are rebound assume standard-conformant allocator types.
//
//- dr_vector_engine.
//
template<class OT, class T1, class A1>
struct matrix_negation_engine_traits<OT, dr_vector_engine<T1, A1>>
{
    using element_type = matrix_negation_element_t<OT, T1>;
    using engine_type  = dr_vector_engine<T1, A1>;
};

//-------------------
//- fs_vector_engine.
//
template<class OT, class T1, int32_t N1>
struct matrix_negation_engine_traits<OT, fs_vector_engine<T1, N1>>
{
    using element_type = matrix_negation_element_t<OT, T1>;
    using engine_type  = fs_vector_engine<T1, N1>;
};

//--------------------------------------------------------------------------------------------------
//  -ENGINE case for matrix.  Note that all partial specializations below in which allocators
//  are rebound assume standard-conformant allocator types.
//
//- dr_matrix_engine.
//
template<class OT, class T1, int32_t R1, int32_t C1>
struct matrix_negation_engine_traits<OT, fs_matrix_engine<T1, R1, C1>>
{
    using element_type = matrix_negation_element_t<OT, T1>;
    using engine_type  = fs_matrix_engine<T1, R1, C1>;
};

template<class OT, class T1, int32_t R1, int32_t C1>
struct matrix_negation_engine_traits<OT, matrix_transpose_view<fs_matrix_engine<T1, R1, C1>>>
{
    using element_type = matrix_negation_element_t<OT, T1>;
    using engine_type  = fs_matrix_engine<element_type, C1, R1>;
};

//-------------------
//- fs_matrix_engine.
//
template<class OT, class T1, class A1>
struct matrix_negation_engine_traits<OT, dr_matrix_engine<T1, A1>>
{
    using element_type = matrix_negation_element_t<OT, T1>;
    using engine_type  = dr_matrix_engine<T1, A1>;
};

template<class OT, class T1, class A1>
struct matrix_negation_engine_traits<OT, matrix_transpose_view<dr_matrix_engine<T1, A1>>>
{
    using element_type = matrix_negation_element_t<OT, T1>;
    using engine_type  = dr_matrix_engine<T1, A1>;
};


//==================================================================================================
//                               **** NEGATION ARITHMETIC TRAITS ****
//==================================================================================================
//
//- Alias interface to detection meta-function that extracts the negation traits type.
//
template<class OT, class OP1>
using matrix_negation_traits_t = detail::negation_traits_t<OT, OP1>;


//- The standard addition traits type provides the default mechanism for computing the result
//  of a matrix or vector negation.
//
template<class OT, class ET1, class OT1>
struct matrix_negation_traits<OT, vector<ET1, OT1>>
{
    using engine_type = matrix_negation_engine_t<OT, ET1>;
    using op_traits   = OT;
    using result_type = vector<engine_type, op_traits>;

    static result_type  negate(vector<ET1, OT1> const& v1);
};

//------
//
template<class OT, class ET1, class OT1>
struct matrix_negation_traits<OT, matrix<ET1, OT1>>
{
    using engine_type = matrix_negation_engine_t<OT, ET1>;
    using op_traits   = OT;
    using result_type = matrix<engine_type, op_traits>;

    static result_type  negate(matrix<ET1, OT1> const& m1);
};

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_NEGATION_TRAITS_HPP_DEFINED
//==================================================================================================
//  File:       negation_traits_impl.hpp
//
//  Summary:    This header defines the static member functions of matrix_negation_traits that
//              perform the actual arithmetic.
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_NEGATION_TRAITS_IMPL_HPP_DEFINED
#define LINEAR_ALGEBRA_NEGATION_TRAITS_IMPL_HPP_DEFINED

namespace STD_LA {
//==================================================================================================
//                         **** NEGATION TRAITS FUNCTION IMPLEMENTATION ****
//==================================================================================================
//
template<class OT, class ET1, class OT1>
inline auto
matrix_negation_traits<OT, vector<ET1, OT1>>::negate(vector<ET1, OT1> const& v1) -> result_type
{
    PrintOperandTypes<result_type>("negation_traits", v1);

	result_type     vr;

	if constexpr (result_requires_resize(vr))
	{
		vr.resize(v1.elements());
	}

	transform(v1.begin(), v1.end(), vr.begin(), [](auto val){ return -val; });

	return vr;
}

//------
//
template<class OT, class ET1, class OT1>
inline auto
matrix_negation_traits<OT, matrix<ET1, OT1>>::negate(matrix<ET1, OT1> const& m1) -> result_type
{
    PrintOperandTypes<result_type>("negation_traits", m1);

	result_type		mr{};                           //- Braces here to avoid C4701 from MSVC
	auto const      rows = m1.rows();
	auto const      columns = m1.columns();

	if constexpr (result_requires_resize(mr))
	{
		mr.resize(rows, columns);
	}

	for (auto i = 0;  i < rows;  ++i)
    {
		for (auto j = 0;  j <  columns;  ++j)
        {
			mr(i, j) = -m1(i, j);
        }
    }
/*
	if constexpr (result_requires_resize(mr))
	{
		mr.resize(rows, columns);
		auto data = mr.data();
		for (auto i = 0; i < columns; ++i)
			for (auto j = 0; j < rows; ++j)
				*data++ = -m1(i, j);		// Safe because the resize means that mr capacity = size for rows and columns.
	}
	else
	{
		transform(m1.data(), m1.data() + (rows * columns), mr.data(),
			[](auto val) {return -val; });
	}
*/
	return mr;
}

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_NEGATION_TRAITS_IMPL_HPP_DEFINED
//==================================================================================================
//  File:       multiplication_traits.hpp
//
//  Summary:    This header defines several private and public traits types that are used to
//              implement the binary multiplication operator.  This is a somewhat long file,
//              divided into two main sections, each with three sub-sections:
//                  1. Private traits definitions that:
//                      A. determine the element promotion traits to be used for multiplication
//                      B. determine the engine promotion traits to be used for multiplication
//                      C. determine the arithmetic traits to be used to perform multiplication
//                  2. Public traits definitions that:
//                      A. perform element promotion for multiplication
//                      B. perform engine promotion for multiplication
//                      C. perform the actual act of multiplication
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_MULTIPLICATION_TRAITS_HPP_DEFINED
#define LINEAR_ALGEBRA_MULTIPLICATION_TRAITS_HPP_DEFINED

namespace STD_LA {
namespace detail {
//==================================================================================================
//                         **** ELEMENT MULTIPLICATION TRAITS DETECTORS ****
//==================================================================================================
//
//- Form 1 type detection of nested element multiplication traits.
//
template<typename OT>
using element_mul_traits_f1_t = typename OT::element_multiplication_traits;

template<typename OT>
using element_mul_type_f1_t = typename element_mul_traits_f1_t<OT>::element_type;

//- Define the form 1 detectors.
//
template<typename OT, typename = void>
struct detect_element_mul_traits_f1
:   public false_type
{
    using traits_type = void;
};

template<typename OT>
struct detect_element_mul_traits_f1<OT, void_t<element_mul_type_f1_t<OT>>>
:   public true_type
{
    using traits_type = element_mul_traits_f1_t<OT>;
};

//----------------------------------------------------------------
//- Form 2 type detection of nested element multiplication traits.
//
template<typename OT, typename T1, typename T2>
using element_mul_traits_f2_t = typename OT::template element_multiplication_traits<T1, T2>;

template<typename OT, typename T1, typename T2>
using element_mul_type_f2_t = typename element_mul_traits_f2_t<OT, T1, T2>::element_type;

//- Define the form 2 detectors.
//
template<typename OT, typename T1, typename T2, typename = void>
struct detect_element_mul_traits_f2
:   public false_type
{
    using traits_type = void;
};

template<typename OT, typename T1, typename T2>
struct detect_element_mul_traits_f2<OT, T1, T2, void_t<element_mul_type_f2_t<OT, T1, T2>>>
:   public true_type
{
    using traits_type = element_mul_traits_f2_t<OT, T1, T2>;
};

//---------------------------------------------------
//- Element multiplication traits type determination.
//
template<typename OT, typename T1, typename T2>
struct element_mul_traits_chooser
{
    using CT1 = typename detect_element_mul_traits_f1<OT>::traits_type;
    using CT2 = typename detect_element_mul_traits_f2<OT, T1, T2>::traits_type;
    using DEF = matrix_multiplication_element_traits<T1, T2>;

    using traits_type = typename non_void_traits_chooser<CT1, CT2, DEF>::traits_type;
};

template<typename OT, typename T1, typename T2>
using element_mul_traits_t = typename element_mul_traits_chooser<OT, T1, T2>::traits_type;

template<class OT, class T1, class T2>
constexpr bool  has_element_mul_traits_v = detect_element_mul_traits_f2<OT, T1, T2>::value ||
                                           detect_element_mul_traits_f1<OT>::value;

//---------------------------------------------------
//- Element multiplication result type determination.
//
template<typename OT, typename T1, typename T2>
struct element_mul_type
{
    using traits_type  = typename element_mul_traits_chooser<OT, T1, T2>::traits_type;
    using element_type = typename traits_type::element_type;
};

template<typename OT, typename T1, typename T2>
using element_mul_type_t = typename element_mul_type<OT, T1, T2>::element_type;


//==================================================================================================
//                         **** ENGINE MULTIPLICATION TRAITS DETECTORS ****
//==================================================================================================
//
//- Form 1 type detection of nested engine multiplication traits.
//
template<typename OT>
using engine_mul_traits_f1_t = typename OT::engine_multiplication_traits;

template<typename OT>
using engine_mul_type_f1_t = typename engine_mul_traits_f1_t<OT>::engine_type;

//- Define the form 1 detectors.
//
template<typename OT, typename = void>
struct detect_engine_mul_traits_f1
:   public false_type
{
    using traits_type = void;
};

template<typename OT>
struct detect_engine_mul_traits_f1<OT, void_t<engine_mul_type_f1_t<OT>>>
:   public true_type
{
    using traits_type = engine_mul_traits_f1_t<OT>;
};

//---------------------------------------------------------------
//- Form 2 type detection of nested engine multiplication traits.
//
template<typename OT, typename T1, typename T2>
using engine_mul_traits_f2_t = typename OT::template engine_multiplication_traits<OT, T1, T2>;

template<typename OT, typename T1, typename T2>
using engine_mul_type_f2_t = typename engine_mul_traits_f2_t<OT, T1, T2>::engine_type;

//- Define the form 2 detectors.
//
template<typename OT, typename ET1, typename ET2, typename = void>
struct detect_engine_mul_traits_f2
:   public false_type
{
    using traits_type = void;
};

template<typename OT, typename ET1, typename ET2>
struct detect_engine_mul_traits_f2<OT, ET1, ET2, void_t<engine_mul_type_f2_t<OT, ET1, ET2>>>
:   public true_type
{
    using traits_type = engine_mul_traits_f2_t<OT, ET1, ET2>;
};

//--------------------------------------------------
//- Engine multiplication traits type determination.
//
template<typename OT, typename ET1, typename ET2>
struct engine_mul_traits_chooser
{
    using CT1 = typename detect_engine_mul_traits_f1<OT>::traits_type;
    using CT2 = typename detect_engine_mul_traits_f2<OT, ET1, ET2>::traits_type;
    using DEF = matrix_multiplication_engine_traits<OT, ET1, ET2>;

    using traits_type = typename non_void_traits_chooser<CT1, CT2, DEF>::traits_type;
};

template<typename OT, typename ET1, typename ET2>
using engine_mul_traits_t = typename engine_mul_traits_chooser<OT, ET1, ET2>::traits_type;

template<class OT, class ET1, class ET2>
constexpr bool  has_engine_mul_traits_v = detect_engine_mul_traits_f2<OT, ET1, ET2>::value ||
                                          detect_engine_mul_traits_f1<OT>::value;

//--------------------------------------------------
//- Engine multiplication result type determination.
//
template<typename OT, typename ET1, typename ET2>
struct engine_mul_type
{
    using traits_type = typename engine_mul_traits_chooser<OT, ET1, ET2>::traits_type;
    using engine_type = typename traits_type::engine_type;
};

template<typename OT, typename ET1, typename ET2>
using engine_mul_type_t = typename engine_mul_type<OT, ET1, ET2>::engine_type;


//==================================================================================================
//                        **** MULTIPLICATION ARITHMETIC TRAITS DETECTORS ****
//==================================================================================================
//
//- Form 1 type detection of nested multiplication arithmetic traits.
//
template<typename OT>
using mul_traits_f1_t = typename OT::multiplication_traits;

template<typename OT>
using mul_type_f1_t = typename mul_traits_f1_t<OT>::result_type;

//- Define the form 1 detectors.
//
template<typename OT, typename = void>
struct detect_mul_traits_f1
:   public false_type
{
    using traits_type = void;
};

template<typename OT>
struct detect_mul_traits_f1<OT, void_t<mul_type_f1_t<OT>>>
:   public true_type
{
    using traits_type = mul_traits_f1_t<OT>;
};

//-------------------------------------------------------------------
//- Form 2 type detection of nested multiplication arithmetic traits.
//
template<typename OT, typename T1, typename T2>
using mul_traits_f2_t = typename OT::template multiplication_traits<OT, T1, T2>;

template<typename OT, typename T1, typename T2>
using mul_type_f2_t = typename mul_traits_f2_t<OT, T1, T2>::result_type;


//- Define the form 2 detectors.
//
template<typename OT, typename OP1, typename OP2, typename = void>
struct detect_mul_traits_f2
:   public false_type
{
    using traits_type = void;
};

template<typename OT, typename OP1, typename OP2>
struct detect_mul_traits_f2<OT, OP1, OP2, void_t<mul_type_f2_t<OT, OP1, OP2>>>
:   public true_type
{
    using traits_type = typename OT::template multiplication_traits<OT, OP1, OP2>;
};

//------------------------------------------------------
//- Multiplication arithmetic traits type determination.
//
template<typename OT, typename OP1, typename OP2>
struct mul_traits_chooser
{
    using CT1 = typename detect_mul_traits_f1<OT>::traits_type;
    using CT2 = typename detect_mul_traits_f2<OT, OP1, OP2>::traits_type;
    using DEF = matrix_multiplication_traits<OT, OP1, OP2>;

    using traits_type = typename non_void_traits_chooser<CT1, CT2, DEF>::traits_type;
};

template<typename OT, typename OP1, typename OP2>
using multiplication_traits_t = typename mul_traits_chooser<OT, OP1, OP2>::traits_type;

template<class OT, class OP1, class OP2>
constexpr bool  has_mul_traits_v = detect_mul_traits_f2<OT, OP1, OP2>::value ||
                                   detect_mul_traits_f1<OT>::value;


}       //- detail namespace
//==================================================================================================
//                           **** ELEMENT MULTIPLICATION TRAITS ****
//==================================================================================================
//
//- Alias interface to detection meta-function that extracts the element multiplication traits type.
//
template<class OT, class T1, class T2>
using matrix_multiplication_element_t = detail::element_mul_type_t<OT, T1, T2>;


//- The standard element multiplication traits type provides the default mechanism for determining
//  the result of multiplying two elements of (possibly) different types.
//
template<class T1, class T2>
struct matrix_multiplication_element_traits
{
    using element_type = decltype(declval<T1>() * declval<T2>());
};


//==================================================================================================
//                            **** ENGINE MULTIPLICATION TRAITS ****
//==================================================================================================
//
//- Alias interface to detection meta-function that extracts the engine multiplication traits type.
//
template<class OT, class ET1, class ET2>
using matrix_multiplication_engine_t = detail::engine_mul_type_t<OT, ET1, ET2>;

//- The standard engine multiplication traits type provides the default mechanism for determining
//  the correct engine type for a matrix/matrix, matrix/vector, matrix/scalar, vector/vector, or
//  vector/scalar multiplication.
//
template<class OT, class ET1, class ET2>
struct matrix_multiplication_engine_traits
{
    static constexpr bool
    use_matrix_engine =  (detail::is_matrix_engine_v<ET1> && detail::is_matrix_engine_v<ET2>)
                      || (detail::is_matrix_engine_v<ET1> && detail::is_scalar_engine_v<ET2>)
                      || (detail::is_scalar_engine_v<ET1> && detail::is_matrix_engine_v<ET2>);

    using element_type_1 = typename ET1::element_type;
    using element_type_2 = typename ET2::element_type;
    using element_type   = matrix_multiplication_element_t<OT, element_type_1, element_type_2>;
    using engine_type    = conditional_t<use_matrix_engine,
                                         dr_matrix_engine<element_type, allocator<element_type>>,
                                         dr_vector_engine<element_type, allocator<element_type>>>;
};

//- General transpose cases for matrices.
//
template<class OT, class ET1, class ET2>
struct matrix_multiplication_engine_traits<OT, ET1, matrix_transpose_view<ET2>>
{
    using engine_type = typename matrix_multiplication_engine_traits<OT, ET1, ET2>::engine_type;
};

template<class OT, class ET1, class ET2>
struct matrix_multiplication_engine_traits<OT, matrix_transpose_view<ET1>, ET2>
{
    using engine_type = typename matrix_multiplication_engine_traits<OT, ET1, ET2>::engine_type;
};

template<class OT, class ET1, class ET2>
struct matrix_multiplication_engine_traits<OT, matrix_transpose_view<ET1>, matrix_transpose_view<ET2>>
{
    using engine_type = typename matrix_multiplication_engine_traits<OT, ET1, ET2>::engine_type;
};

//--------------------------------------------------------------------------------------------------
//- ENGINE * SCALAR cases for vectors.
//
template<class OT, class T1, class A1, class T2>
struct matrix_multiplication_engine_traits<OT, dr_vector_engine<T1, A1>, detail::element_tag<T2>>
{
    using element_type = matrix_multiplication_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<A1, element_type>;
    using engine_type  = dr_vector_engine<element_type, alloc_type>;
};

template<class OT, class T1, int32_t N1, class T2>
struct matrix_multiplication_engine_traits<OT, fs_vector_engine<T1, N1>, detail::element_tag<T2>>
{
    using element_type = matrix_multiplication_element_t<OT, T1, T2>;
    using engine_type  = fs_vector_engine<element_type, N1>;
};

//--------------------------------------------------------------------------------------------------
//- SCALAR * ENGINE cases for vectors.
//
template<class OT, class T1, class T2, class A2>
struct matrix_multiplication_engine_traits<OT, detail::element_tag<T1>, dr_vector_engine<T2, A2>>
{
    using element_type = matrix_multiplication_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<A2, element_type>;
    using engine_type  = dr_vector_engine<element_type, alloc_type>;
};

template<class OT, class T1, class T2, int32_t N2>
struct matrix_multiplication_engine_traits<OT, detail::element_tag<T1>, fs_vector_engine<T2, N2>>
{
    using element_type = matrix_multiplication_element_t<OT, T1, T2>;
    using engine_type  = fs_vector_engine<element_type, N2>;
};

//--------------------------------------------------------------------------------------------------
//- ENGINE * SCALAR cases for matrices.
//
//- dr_matrix_engine * scalar.
//
template<class OT, class T1, class A1, class T2>
struct matrix_multiplication_engine_traits<OT,
                                           dr_matrix_engine<T1, A1>,
                                           detail::element_tag<T2>>
{
    using element_type = matrix_multiplication_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<A1, element_type>;
    using engine_type  = dr_matrix_engine<element_type, alloc_type>;
};

template<class OT, class T1, class A1, class T2>
struct matrix_multiplication_engine_traits<OT,
                                           matrix_transpose_view<dr_matrix_engine<T1, A1>>,
                                           detail::element_tag<T2>>
{
    using element_type = matrix_multiplication_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<A1, element_type>;
    using engine_type  = dr_matrix_engine<element_type, alloc_type>;
};

//----------------------------
//- fs_matrix_engine * scalar.
//
template<class OT, class T1, int32_t R1, int32_t C1, class T2>
struct matrix_multiplication_engine_traits<OT,
                                           fs_matrix_engine<T1, R1, C1>,
                                           detail::element_tag<T2>>
{
    using element_type = matrix_multiplication_element_t<OT, T1, T2>;
    using engine_type  = fs_matrix_engine<element_type, R1, C1>;
};

template<class OT, class T1, int32_t R1, int32_t C1, class T2>
struct matrix_multiplication_engine_traits<OT,
                                           matrix_transpose_view<fs_matrix_engine<T1, R1, C1>>,
                                           detail::element_tag<T2>>
{
    using element_type = matrix_multiplication_element_t<OT, T1, T2>;
    using engine_type  = fs_matrix_engine<element_type, C1, R1>;
};

//--------------------------------------------------------------------------------------------------
//- SCALAR * ENGINE cases for matrices.
//
//- scalar * dr_matrix_engine.
//
template<class OT, class T1, class T2, class A2>
struct matrix_multiplication_engine_traits<OT,
                                           detail::element_tag<T1>,
                                           dr_matrix_engine<T2, A2>>
{
    using element_type = matrix_multiplication_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<A2, element_type>;
    using engine_type  = dr_matrix_engine<element_type, alloc_type>;
};

template<class OT, class T1, class T2, class A2>
struct matrix_multiplication_engine_traits<OT,
                                           detail::element_tag<T1>,
                                           matrix_transpose_view<dr_matrix_engine<T2, A2>>>
{
    using element_type = matrix_multiplication_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<A2, element_type>;
    using engine_type  = dr_matrix_engine<element_type, alloc_type>;
};

//----------------------------
//- scalar * fs_matrix_engine.
//
template<class OT, class T1, class T2, int32_t R2, int32_t C2>
struct matrix_multiplication_engine_traits<OT,
                                           detail::element_tag<T1>,
                                           fs_matrix_engine<T2, R2, C2>>
{
    using element_type = matrix_multiplication_element_t<OT, T1, T2>;
    using engine_type  = fs_matrix_engine<element_type, R2, C2>;
};

template<class OT, class T1, class T2, int32_t R2, int32_t C2>
struct matrix_multiplication_engine_traits<OT,
                                           detail::element_tag<T1>,
                                           matrix_transpose_view<fs_matrix_engine<T2, R2, C2>>>
{
    using element_type = matrix_multiplication_element_t<OT, T1, T2>;
    using engine_type  = fs_matrix_engine<element_type, C2, R2>;
};

//--------------------------------------------------------------------------------------------------
//- ENGINE * ENGINE cases for matrix/vector.  Note that all partial specializations below in which
//  allocators are rebound assume standard-conformant allocator types.
//
//- dr_matrix_engine * dr_vector_engine.
//
template<class OT, class T1, class A1, class T2, class A2>
struct matrix_multiplication_engine_traits<OT,
                                           dr_matrix_engine<T1, A1>,
                                           dr_vector_engine<T2, A2>>
{
    using element_type = matrix_multiplication_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<A1, element_type>;
    using engine_type  = dr_vector_engine<element_type, alloc_type>;
};

template<class OT, class T1, class A1, class T2, class A2>
struct matrix_multiplication_engine_traits<OT,
                                           matrix_transpose_view<dr_matrix_engine<T1, A1>>,
                                           dr_vector_engine<T2, A2>>
{
    using element_type = matrix_multiplication_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<A1, element_type>;
    using engine_type  = dr_vector_engine<element_type, alloc_type>;
};

//--------------------------------------
//- dr_matrix_engine * fs_vector_engine.
//
template<class OT, class T1, class A1, class T2, int32_t N2>
struct matrix_multiplication_engine_traits<OT,
                                           dr_matrix_engine<T1, A1>,
                                           fs_vector_engine<T2, N2>>
{
    using element_type = matrix_multiplication_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<A1, element_type>;
    using engine_type  = dr_vector_engine<element_type, alloc_type>;
};

template<class OT, class T1, class A1, class T2, int32_t N2>
struct matrix_multiplication_engine_traits<OT,
                                           matrix_transpose_view<dr_matrix_engine<T1, A1>>,
                                           fs_vector_engine<T2, N2>>
{
    using element_type = matrix_multiplication_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<A1, element_type>;
    using engine_type  = dr_vector_engine<element_type, alloc_type>;
};

//--------------------------------------
//- fs_matrix_engine * dr_vector_engine.
//
template<class OT, class T1, int32_t R1, int32_t C1, class T2, class A2>
struct matrix_multiplication_engine_traits<OT,
                                           fs_matrix_engine<T1, R1, C1>,
                                           dr_vector_engine<T2, A2>>
{
    using element_type = matrix_multiplication_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<A2, element_type>;
    using engine_type  = dr_vector_engine<element_type, alloc_type>;
};

template<class OT, class T1, int32_t R1, int32_t C1, class T2, class A2>
struct matrix_multiplication_engine_traits<OT,
                                           matrix_transpose_view<fs_matrix_engine<T1, R1, C1>>,
                                           dr_vector_engine<T2, A2>>
{
    using element_type = matrix_multiplication_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<A2, element_type>;
    using engine_type  = dr_vector_engine<element_type, alloc_type>;
};

//--------------------------------------
//- fs_matrix_engine * fs_vector_engine.
//
template<class OT, class T1, int32_t R1, int32_t C1, class T2, int32_t N2>
struct matrix_multiplication_engine_traits<OT,
                                           fs_matrix_engine<T1, R1, C1>,
                                           fs_vector_engine<T2, N2>>
{
    static_assert(C1 == N2);
    using element_type = matrix_multiplication_element_t<OT, T1, T2>;
    using engine_type  = fs_vector_engine<element_type, N2>;
};

template<class OT, class T1, int32_t R1, int32_t C1, class T2, int32_t N2>
struct matrix_multiplication_engine_traits<OT,
                                           matrix_transpose_view<fs_matrix_engine<T1, R1, C1>>,
                                           fs_vector_engine<T2, N2>>
{
    static_assert(R1 == N2);
    using element_type = matrix_multiplication_element_t<OT, T1, T2>;
    using engine_type  = fs_vector_engine<element_type, N2>;
};


//--------------------------------------------------------------------------------------------------
//- ENGINE * ENGINE cases for vector/matrix.  Note that all partial specializations below in which
//  allocators are rebound assume standard-conformant allocator types.
//
//- dr_vector_engine * dr_matrix_engine.
//
template<class OT, class T1, class A1, class T2, class A2>
struct matrix_multiplication_engine_traits<OT,
                                           dr_vector_engine<T1, A1>,
                                           dr_matrix_engine<T2, A2>>
{
    using element_type = matrix_multiplication_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<A1, element_type>;
    using engine_type  = dr_vector_engine<element_type, alloc_type>;
};

template<class OT, class T1, class A1, class T2, class A2>
struct matrix_multiplication_engine_traits<OT,
                                           dr_vector_engine<T1, A1>,
                                           matrix_transpose_view<dr_matrix_engine<T2, A2>>>
{
    using element_type = matrix_multiplication_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<A1, element_type>;
    using engine_type  = dr_vector_engine<element_type, alloc_type>;
};

//--------------------------------------
//- dr_vector_engine * fs_matrix_engine.
//
template<class OT, class T1, class A1, class T2, int32_t R2, int32_t C2>
struct matrix_multiplication_engine_traits<OT,
                                           dr_vector_engine<T1, A1>,
                                           fs_matrix_engine<T2, R2, C2>>
{
    using element_type = matrix_multiplication_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<A1, element_type>;
    using engine_type  = dr_vector_engine<element_type, alloc_type>;
};

template<class OT, class T1, class A1, class T2, int32_t R2, int32_t C2>
struct matrix_multiplication_engine_traits<OT,
                                           dr_vector_engine<T1, A1>,
                                           matrix_transpose_view<fs_matrix_engine<T2, R2, C2>>>
{
    using element_type = matrix_multiplication_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<A1, element_type>;
    using engine_type  = dr_vector_engine<element_type, alloc_type>;
};

//--------------------------------------
//- fs_vector_engine * dr_matrix_engine.
//
template<class OT, class T1, int32_t N1, class T2, class A2>
struct matrix_multiplication_engine_traits<OT,
                                           fs_vector_engine<T1, N1>,
                                           dr_matrix_engine<T2, A2>>
{
    using element_type = matrix_multiplication_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<A2, element_type>;
    using engine_type  = dr_vector_engine<element_type, alloc_type>;
};

template<class OT, class T1, int32_t N1, class T2, class A2>
struct matrix_multiplication_engine_traits<OT,
                                           fs_vector_engine<T1, N1>,
                                           matrix_transpose_view<dr_matrix_engine<T2, A2>>>
{
    using element_type = matrix_multiplication_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<A2, element_type>;
    using engine_type  = dr_vector_engine<element_type, alloc_type>;
};

//--------------------------------------
//- fs_vector_engine * fs_matrix_engine.
//
template<class OT, class T1, int32_t N1, class T2, int32_t R2, int32_t C2>
struct matrix_multiplication_engine_traits<OT,
                                           fs_vector_engine<T1, N1>,
                                           fs_matrix_engine<T2, R2, C2>>
{
    static_assert(N1 == R2);
    using element_type = matrix_multiplication_element_t<OT, T1, T2>;
    using engine_type  = fs_vector_engine<element_type, N1>;
};

template<class OT, class T1, int32_t N1, class T2, int32_t R2, int32_t C2>
struct matrix_multiplication_engine_traits<OT,
                                           fs_vector_engine<T1, N1>,
                                           matrix_transpose_view<fs_matrix_engine<T2, R2, C2>>>
{
    static_assert(N1 == C2);
    using element_type = matrix_multiplication_element_t<OT, T1, T2>;
    using engine_type  = fs_vector_engine<element_type, N1>;
};


//--------------------------------------------------------------------------------------------------
//- ENGINE * ENGINE cases for matrix/matrix.  Note that all partial specializations below in which
//  allocators are rebound assume standard-conformant allocator types.
//
//- dr_matrix_engine * dr_matrix_engine.
//
template<class OT, class T1, class A1, class T2, class A2>
struct matrix_multiplication_engine_traits<OT,
                                           dr_matrix_engine<T1, A1>,
                                           dr_matrix_engine<T2, A2>>
{
    using element_type = matrix_multiplication_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<A1, element_type>;
    using engine_type  = dr_matrix_engine<element_type, alloc_type>;
};

template<class OT, class T1, class A1, class T2, class A2>
struct matrix_multiplication_engine_traits<OT,
                                           dr_matrix_engine<T1, A1>,
                                           matrix_transpose_view<dr_matrix_engine<T2, A2>>>
{
    using element_type = matrix_multiplication_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<A1, element_type>;
    using engine_type  = dr_matrix_engine<element_type, alloc_type>;
};

template<class OT, class T1, class A1, class T2, class A2>
struct matrix_multiplication_engine_traits<OT,
                                           matrix_transpose_view<dr_matrix_engine<T1, A1>>,
                                           dr_matrix_engine<T2, A2>>
{
    using element_type = matrix_multiplication_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<A1, element_type>;
    using engine_type  = dr_matrix_engine<element_type, alloc_type>;
};

template<class OT, class T1, class A1, class T2, class A2>
struct matrix_multiplication_engine_traits<OT,
                                           matrix_transpose_view<dr_matrix_engine<T1, A1>>,
                                           matrix_transpose_view<dr_matrix_engine<T2, A2>>>
{
    using element_type = matrix_multiplication_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<A1, element_type>;
    using engine_type  = dr_matrix_engine<element_type, alloc_type>;
};

//--------------------------------------
//- dr_matrix_engine * fs_matrix_engine.
//
template<class OT, class T1, class A1, class T2, int32_t R2, int32_t C2>
struct matrix_multiplication_engine_traits<OT,
                                           dr_matrix_engine<T1, A1>,
                                           fs_matrix_engine<T2, R2, C2>>
{
    using element_type = matrix_multiplication_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<A1, element_type>;
    using engine_type  = dr_matrix_engine<element_type, alloc_type>;
};

template<class OT, class T1, class A1, class T2, int32_t R2, int32_t C2>
struct matrix_multiplication_engine_traits<OT,
                                           dr_matrix_engine<T1, A1>,
                                           matrix_transpose_view<fs_matrix_engine<T2, R2, C2>>>
{
    using element_type = matrix_multiplication_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<A1, element_type>;
    using engine_type  = dr_matrix_engine<element_type, alloc_type>;
};

template<class OT, class T1, class A1, class T2, int32_t R2, int32_t C2>
struct matrix_multiplication_engine_traits<OT,
                                           matrix_transpose_view<dr_matrix_engine<T1, A1>>,
                                           fs_matrix_engine<T2, R2, C2>>
{
    using element_type = matrix_multiplication_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<A1, element_type>;
    using engine_type  = dr_matrix_engine<element_type, alloc_type>;
};

template<class OT, class T1, class A1, class T2, int32_t R2, int32_t C2>
struct matrix_multiplication_engine_traits<OT,
                                           matrix_transpose_view<dr_matrix_engine<T1, A1>>,
                                           matrix_transpose_view<fs_matrix_engine<T2, R2, C2>>>
{
    using element_type = matrix_multiplication_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<A1, element_type>;
    using engine_type  = dr_matrix_engine<element_type, alloc_type>;
};

//--------------------------------------
//- fs_matrix_engine * dr_matrix_engine.
//
template<class OT, class T1, int32_t R1, int32_t C1, class T2, class A2>
struct matrix_multiplication_engine_traits<OT,
                                           fs_matrix_engine<T1, R1, C1>,
                                           dr_matrix_engine<T2, A2>>
{
    using element_type = matrix_multiplication_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<A2, element_type>;
    using engine_type  = dr_matrix_engine<element_type, alloc_type>;
};

template<class OT, class T1, int32_t R1, int32_t C1, class T2, class A2>
struct matrix_multiplication_engine_traits<OT,
                                           fs_matrix_engine<T1, R1, C1>,
                                           matrix_transpose_view<dr_matrix_engine<T2, A2>>>
{
    using element_type = matrix_multiplication_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<A2, element_type>;
    using engine_type  = dr_matrix_engine<element_type, alloc_type>;
};

template<class OT, class T1, int32_t R1, int32_t C1, class T2, class A2>
struct matrix_multiplication_engine_traits<OT,
                                           matrix_transpose_view<fs_matrix_engine<T1, R1, C1>>,
                                           dr_matrix_engine<T2, A2>>
{
    using element_type = matrix_multiplication_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<A2, element_type>;
    using engine_type  = dr_matrix_engine<element_type, alloc_type>;
};

template<class OT, class T1, int32_t R1, int32_t C1, class T2, class A2>
struct matrix_multiplication_engine_traits<OT,
                                           matrix_transpose_view<fs_matrix_engine<T1, R1, C1>>,
                                           matrix_transpose_view<dr_matrix_engine<T2, A2>>>
{
    using element_type = matrix_multiplication_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<A2, element_type>;
    using engine_type  = dr_matrix_engine<element_type, alloc_type>;
};

//--------------------------------------
//- fs_matrix_engine * fs_matrix_engine.
//
template<class OT, class T1, int32_t R1, int32_t C1, class T2, int32_t R2, int32_t C2>
struct matrix_multiplication_engine_traits<OT,
                                           fs_matrix_engine<T1, R1, C1>,
                                           fs_matrix_engine<T2, R2, C2>>
{
    static_assert(C1 == R2);
    using element_type = matrix_multiplication_element_t<OT, T1, T2>;
    using engine_type  = fs_matrix_engine<element_type, R1, C2>;
};

template<class OT, class T1, int32_t R1, int32_t C1, class T2, int32_t R2, int32_t C2>
struct matrix_multiplication_engine_traits<OT,
                                           fs_matrix_engine<T1, R1, C1>,
                                           matrix_transpose_view<fs_matrix_engine<T2, R2, C2>>>
{
    static_assert(C1 == C2);
    using element_type = matrix_multiplication_element_t<OT, T1, T2>;
    using engine_type  = fs_matrix_engine<element_type, R1, R2>;
};

template<class OT, class T1, int32_t R1, int32_t C1, class T2, int32_t R2, int32_t C2>
struct matrix_multiplication_engine_traits<OT,
                                           matrix_transpose_view<fs_matrix_engine<T1, R1, C1>>,
                                           fs_matrix_engine<T2, R2, C2>>
{
    static_assert(R1 == R2);
    using element_type = matrix_multiplication_element_t<OT, T1, T2>;
    using engine_type  = fs_matrix_engine<element_type, C1, C2>;
};

template<class OT, class T1, int32_t R1, int32_t C1, class T2, int32_t R2, int32_t C2>
struct matrix_multiplication_engine_traits<OT,
                                           matrix_transpose_view<fs_matrix_engine<T1, R1, C1>>,
                                           matrix_transpose_view<fs_matrix_engine<T2, R2, C2>>>
{
    static_assert(R1 == C2);
    using element_type = matrix_multiplication_element_t<OT, T1, T2>;
    using engine_type  = fs_matrix_engine<element_type, C1, R2>;
};


//==================================================================================================
//                                **** MULTIPLICATION TRAITS ****
//==================================================================================================
//
//- Alias interface to detection meta-function that extracts the multiplication traits type.
//
template<class OT, class OP1, class OP2>
using matrix_multiplication_traits_t = detail::multiplication_traits_t<OT, OP1, OP2>;


//- The standard multiplication traits type provides the default mechanism for computing the
//  correct result type of a multiplication.
//
//---------------
//- vector*scalar
//
template<class OTR, class ET1, class OT1, class T2>
struct matrix_multiplication_traits<OTR, vector<ET1, OT1>, T2>
{
    using scalar_type = detail::element_tag<T2>;
    using engine_type = matrix_multiplication_engine_t<OTR, ET1, scalar_type>;
    using op_traits   = OTR;
    using result_type = vector<engine_type, op_traits>;

    static result_type  multiply(vector<ET1, OT1> const& v1, T2 const& s2);
};

//---------------
//- scalar*vector
//
template<class OTR, class T1, class ET2, class OT2>
struct matrix_multiplication_traits<OTR, T1, vector<ET2, OT2>>
{
    using scalar_type = detail::element_tag<T1>;
    using engine_type = matrix_multiplication_engine_t<OTR, scalar_type, ET2>;
    using op_traits   = OTR;
    using result_type = vector<engine_type, op_traits>;

    static result_type  multiply(T1 const& s1, vector<ET2, OT2> const& v2);
};

//---------------
//- matrix*scalar
//
template<class OTR, class ET1, class OT1, class T2>
struct matrix_multiplication_traits<OTR, matrix<ET1, OT1>, T2>
{
    using scalar_type = detail::element_tag<T2>;
    using engine_type = matrix_multiplication_engine_t<OTR, ET1, scalar_type>;
    using op_traits   = OTR;
    using result_type = matrix<engine_type, op_traits>;

    static result_type  multiply(matrix<ET1, OT1> const& m1, T2 const& s2);
};

//---------------
//- scalar*matrix
//
template<class OTR, class T1, class ET2, class OT2>
struct matrix_multiplication_traits<OTR, T1, matrix<ET2, OT2>>
{
    using scalar_type = detail::element_tag<T1>;
    using engine_type = matrix_multiplication_engine_t<OTR, scalar_type, ET2>;
    using op_traits   = OTR;
    using result_type = matrix<engine_type, op_traits>;

    static result_type  multiply(T1 const& s1, matrix<ET2, OT2> const& m2);
};

//---------------
//- vector*vector
//
template<class OTR, class ET1, class OT1, class ET2, class OT2>
struct matrix_multiplication_traits<OTR, vector<ET1, OT1>, vector<ET2, OT2>>
{
    //- Note that this specialization returns a scalar, and therefore does not compute an
    //  engine type.
    //
    using op_traits   = OTR;
    using elem_type_1 = typename vector<ET1, OT1>::element_type;
    using elem_type_2 = typename vector<ET2, OT2>::element_type;
    using result_type = matrix_multiplication_element_t<op_traits, elem_type_1, elem_type_2>;

    static result_type  multiply(vector<ET1, OT1> const& v1, vector<ET2, OT2> const& v2);
};

//---------------
//- matrix*vector
//
template<class OTR, class ET1, class OT1, class ET2, class OT2>
struct matrix_multiplication_traits<OTR, matrix<ET1, OT1>, vector<ET2, OT2>>
{
    using engine_type = matrix_multiplication_engine_t<OTR, ET1, ET2>;
    using op_traits   = OTR;
    using result_type = vector<engine_type, op_traits>;

    static result_type  multiply(matrix<ET1, OT1> const& m1, vector<ET2, OT2> const& m2);
};

//---------------
//- vector*matrix
//
template<class OTR, class ET1, class OT1, class ET2, class OT2>
struct matrix_multiplication_traits<OTR, vector<ET1, OT1>, matrix<ET2, OT2>>
{
    using engine_type = matrix_multiplication_engine_t<OTR, ET1, ET2>;
    using op_traits   = OTR;
    using result_type = vector<engine_type, op_traits>;

    static result_type  multiply(vector<ET1, OT1> const& m1, matrix<ET2, OT2> const& m2);
};

//---------------
//- matrix*matrix
//
template<class OTR, class ET1, class OT1, class ET2, class OT2>
struct matrix_multiplication_traits<OTR, matrix<ET1, OT1>, matrix<ET2, OT2>>
{
    using engine_type = matrix_multiplication_engine_t<OTR, ET1, ET2>;
    using op_traits   = OTR;
    using result_type = matrix<engine_type, op_traits>;

    static result_type  multiply(matrix<ET1, OT1> const& m1, matrix<ET2, OT2> const& m2);
};

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_MULTIPLICATION_TRAITS_HPP_DEFINED
//==================================================================================================
//  File:       multiplication_traits_impl.hpp
//
//  Summary:    This header defines the static member functions of matrix_multiplication_traits
//              that perform the actual arithmetic.
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_MULTIPLICATION_TRAITS_IMPL_HPP_DEFINED
#define LINEAR_ALGEBRA_MULTIPLICATION_TRAITS_IMPL_HPP_DEFINED

namespace STD_LA {
//==================================================================================================
//                   **** MULTIPLICATION TRAITS FUNCTION IMPLEMENTATION ****
//==================================================================================================
//
//- vector*scalar
//
template<class OTR, class ET1, class OT1, class T2>
inline auto
matrix_multiplication_traits<OTR, vector<ET1, OT1>, T2>::multiply
(vector<ET1, OT1> const& v1, T2 const& s2) -> result_type
{
    PrintOperandTypes<result_type>("multiplication_traits (v*s)", v1, s2);

	result_type     vr{};

	if constexpr (result_requires_resize(vr))
	{
		vr.resize(v1.elements());
	}

	transform(v1.begin(), v1.end(), vr.begin(), [&](auto val) { return val * s2; });

	return vr;
}

//---------------
//- scalar*vector
//
template<class OTR, class T1, class ET2, class OT2>
inline auto
matrix_multiplication_traits<OTR, T1, vector<ET2, OT2>>::multiply
(T1 const& s1, vector<ET2, OT2> const& v2) -> result_type
{
    PrintOperandTypes<result_type>("multiplication_traits (s*v)", s1, v2);

	result_type     vr{};

	if constexpr (result_requires_resize(vr))
	{
		vr.resize(v2.elements());
	}

	transform(v2.begin(), v2.end(), vr.begin(), [&](auto val) { return val * s1; });

	return vr;
}

//---------------
//- matrix*scalar
//
template<class OTR, class ET1, class OT1, class T2>
inline auto
matrix_multiplication_traits<OTR, matrix<ET1, OT1>, T2>::multiply
(matrix<ET1, OT1> const& m1, T2 const& s2) -> result_type
{
    PrintOperandTypes<result_type>("multiplication_traits (m*s)", m1, s2);

	result_type		mr{};
	auto const      rows = m1.rows();
	auto const      columns = m1.columns();

	if constexpr (result_requires_resize(mr))
	{
		mr.resize(rows, columns);
    }

	for (auto i = 0;  i < rows;  ++i)
    {
		for (auto j = 0;  j < columns;  ++j)
        {
			mr(i, j) = m1(i, j) * s2;
        }
    }

/*
	if constexpr (result_requires_resize(mr))
	{
		mr.resize(rows, columns);
		auto data = mr.data();
		for (auto i = 0; i < columns; ++i)
			for (auto j = 0; j < rows; ++j)
				* data++ = m1(i, j) * s2;		// Safe because the resize means that mr capacity = size for rows and columns.
	}
	else
	{
		transform(m1.data(), m1.data() + (rows * columns), mr.data(),
			[&](auto val) {return val * s2; });
	}
*/
	return mr;
}

//---------------
//- scalar*matrix
//
template<class OTR, class T1, class ET2, class OT2>
inline auto
matrix_multiplication_traits<OTR, T1, matrix<ET2, OT2>>::multiply
(T1 const& s1, matrix<ET2, OT2> const& m2) -> result_type
{
    PrintOperandTypes<result_type>("multiplication_traits (s*m)", s1, m2);

	result_type		mr{};
	const auto		rows = m2.rows();
	const auto		columns = m2.columns();

	if constexpr (result_requires_resize(mr))
	{
		mr.resize(rows, columns);
	}

	for (auto i = 0;  i < rows;  ++i)
    {
		for (auto j = 0;  j < columns;  ++j)
        {
			mr(i, j) = m2(i, j) * s1;		// Safe because the resize means that mr capacity = size for rows and columns.
        }
    }
/*
	if constexpr (result_requires_resize(mr))
	{
		mr.resize(rows, columns);
		auto data = mr.data();
		for (auto i = 0; i < columns; ++i)
			for (auto j = 0; j < rows; ++j)
				* data++ = m2(i, j) * s1;		// Safe because the resize means that mr capacity = size for rows and columns.
	}
	else
	{
		transform(m2.data(), m2.data() + (rows * columns), mr.data(),
			[&](auto val) {return val * s1; });
	}
*/
	return mr;
}

//- vector*vector
//
template<class OTR, class ET1, class OT1, class ET2, class OT2>
inline auto
matrix_multiplication_traits<OTR, vector<ET1, OT1>, vector<ET2, OT2>>::multiply
(vector<ET1, OT1> const& v1, vector<ET2, OT2> const& v2) -> result_type
{
    PrintOperandTypes<result_type>("multiplication_traits (v*v)", v1, v2);
    return std::inner_product(v1.begin(), v1.end(), v2.begin(), result_type(0));
}

//---------------
//- matrix*vector
//
template<class OTR, class ET1, class OT1, class ET2, class OT2>
inline auto
matrix_multiplication_traits<OTR, matrix<ET1, OT1>, vector<ET2, OT2>>::multiply
(matrix<ET1, OT1> const& m1, vector<ET2, OT2> const& v2) -> result_type
{
    PrintOperandTypes<result_type>("multiplication_traits (m*v) ", m1, v2);

	result_type     vr{};

	if constexpr (result_requires_resize(vr))
	{
		vr.resize(v2.elements());
	}

/*
    vr(e) = std::inner_product(&m1(e, 0), &m1(e, m1.columns()), v2.data(), typename result_type::element_type(0));
*/
	return vr;
}

//---------------
//- vector*matrix
//
template<class OTR, class ET1, class OT1, class ET2, class OT2>
inline auto
matrix_multiplication_traits<OTR, vector<ET1, OT1>, matrix<ET2, OT2>>::multiply
(vector<ET1, OT1> const& m1, matrix<ET2, OT2> const& m2) -> result_type
{
    PrintOperandTypes<result_type>("multiplication_traits (v*m)", m1, m2);
    return result_type();
}

//---------------
//- matrix*matrix
//
template<class OTR, class ET1, class OT1, class ET2, class OT2>
inline auto
matrix_multiplication_traits<OTR, matrix<ET1, OT1>, matrix<ET2, OT2>>::multiply
(matrix<ET1, OT1> const& m1, matrix<ET2, OT2> const& m2) -> result_type
{
    PrintOperandTypes<result_type>("multiplication_traits (m*m)", m1, m2);
    return result_type();
}

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_MULTIPLICATION_TRAITS_IMPL_HPP_DEFINED
//==================================================================================================
//  File:       operation_traits.hpp
//
//  Summary:    This header defines library's default operation traits type and a customization
//              point that is specialize-able by users.
//
//              Class matrix_operation_traits provides a set of nested type aliases which, in
//              turn, provide for element promotion, engine promotion, and computation for the
//              four basic arithmetic operations (addition, subtraction, negation, multiplication).
//
//              Class template matrix_operation_traits_selector is a customization point,
//              which can be specialized by users, that is used by the arithmetic operators
//              to select the operation traits to be used in computing that operation's result
//              type.
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_OPERATION_TRAITS_HPP_DEFINED
#define LINEAR_ALGEBRA_OPERATION_TRAITS_HPP_DEFINED

namespace STD_LA {
//==================================================================================================
//                                  **** OPERATION TRAITS ****
//==================================================================================================
//  Traits type that refers to the four basic arithmetic traits types.
//==================================================================================================
//
struct matrix_operation_traits
{
    //- Default element promotion traits.
    //
    template<class T1>
    using element_negation_traits = matrix_negation_element_traits<T1>;

    template<class T1, class T2>
    using element_addition_traits = matrix_addition_element_traits<T1, T2>;

    template<class T1, class T2>
    using element_subtraction_traits = matrix_subtraction_element_traits<T1, T2>;

    template<class T1, class T2>
    using element_multiplication_traits = matrix_multiplication_element_traits<T1, T2>;

    //- Default engine promotion traits.
    //
    template<class OTR, class ET1>
    using engine_negation_traits = matrix_negation_engine_traits<OTR, ET1>;

    template<class OTR, class ET1, class ET2>
    using engine_addition_traits = matrix_addition_engine_traits<OTR, ET1, ET2>;

    template<class OTR, class ET1, class ET2>
    using engine_subtraction_traits = matrix_subtraction_engine_traits<OTR, ET1, ET2>;

    template<class OTR, class ET1, class ET2>
    using engine_multiplication_traits = matrix_multiplication_engine_traits<OTR, ET1, ET2>;

    //- Default arithmetic operation traits.
    //
    template<class OP1, class OTR>
    using negation_traits = matrix_negation_traits<OP1, OTR>;

    template<class OTR, class OP1, class OP2>
    using addition_traits = matrix_addition_traits<OTR, OP1, OP2>;

    template<class OTR, class OP1, class OP2>
    using subtraction_traits = matrix_subtraction_traits<OTR, OP1, OP2>;

    template<class OTR, class OP1, class OP2>
    using multiplication_traits = matrix_multiplication_traits<OTR, OP1, OP2>;
};

//==================================================================================================
//                             **** OPERATION TRAITS SELECTION ****
//==================================================================================================
//  Traits type that selects the set of operator traits that is used in an arithmetic expression.
//==================================================================================================
//
template<class T1, class T2>
struct matrix_operation_traits_selector;

template<class T1>
struct matrix_operation_traits_selector<T1, T1>
{
    using traits_type = T1;
};

template<class T1>
struct matrix_operation_traits_selector<T1, matrix_operation_traits>
{
    using traits_type = T1;
};

template<class T1>
struct matrix_operation_traits_selector<matrix_operation_traits, T1>
{
    using traits_type = T1;
};

template<>
struct matrix_operation_traits_selector<matrix_operation_traits, matrix_operation_traits>
{
    using traits_type = matrix_operation_traits;
};

template<class T1>
struct matrix_operation_traits_selector<T1, default_matrix_operations>
{
    using traits_type = T1;
};

template<class T1>
struct matrix_operation_traits_selector<default_matrix_operations, T1>
{
    using traits_type = T1;
};

template<>
struct matrix_operation_traits_selector<default_matrix_operations, default_matrix_operations>
{
    using traits_type = default_matrix_operations;
};

//- Alias interface to trait.
//
template<class T1, class T2>
using matrix_operation_traits_selector_t = typename matrix_operation_traits_selector<T1,T2>::traits_type;


}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_OPERATION_TRAITS_HPP_DEFINED
//==================================================================================================
//  File:       arithmetic_operators.hpp
//
//  Summary:    This header defines the overloaded operators that implement basic arithmetic
//              operations on vectors and matrices.
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_ARITHMETIC_OPERATORS_HPP_DEFINED
#define LINEAR_ALGEBRA_ARITHMETIC_OPERATORS_HPP_DEFINED

namespace STD_LA {
//=================================================================================================
//  Binary addition operators, which forward to the addition traits to do the work.
//=================================================================================================
//
template<class ET1, class OT1, class ET2, class OT2>
inline auto
operator +(vector<ET1, OT1> const& v1, vector<ET2, OT2> const& v2)
{
    using op_traits  = matrix_operation_traits_selector_t<OT1, OT2>;
    using op1_type   = vector<ET1, OT1>;
    using op2_type   = vector<ET2, OT2>;
    using add_traits = matrix_addition_traits_t<op_traits, op1_type, op2_type>;

    return add_traits::add(v1, v2);
}

template<class ET1, class OT1, class ET2, class OT2>
inline auto
operator +(matrix<ET1, OT1> const& m1, matrix<ET2, OT2> const& m2)
{
    using op_traits  = matrix_operation_traits_selector_t<OT1, OT2>;
    using op1_type   = matrix<ET1, OT1>;
    using op2_type   = matrix<ET2, OT2>;
    using add_traits = matrix_addition_traits_t<op_traits, op1_type, op2_type>;

    return add_traits::add(m1, m2);
}


//=================================================================================================
//  Binary subtraction operators, which forward to the subtraction traits to do the work.
//=================================================================================================
//
template<class ET1, class OT1, class ET2, class OT2>
inline auto
operator -(vector<ET1, OT1> const& v1, vector<ET2, OT2> const& v2)
{
    using op_traits  = matrix_operation_traits_selector_t<OT1, OT2>;
    using op1_type   = vector<ET1, OT1>;
    using op2_type   = vector<ET2, OT2>;
    using sub_traits = matrix_subtraction_traits_t<op_traits, op1_type, op2_type>;

    return sub_traits::subtract(v1, v2);
}

template<class ET1, class OT1, class ET2, class OT2>
inline auto
operator -(matrix<ET1, OT1> const& m1, matrix<ET2, OT2> const& m2)
{
    using op_traits  = matrix_operation_traits_selector_t<OT1, OT2>;
    using op1_type   = matrix<ET1, OT1>;
    using op2_type   = matrix<ET2, OT2>;
    using sub_traits = matrix_subtraction_traits_t<op_traits, op1_type, op2_type>;

    return sub_traits::subtract(m1, m2);
}


//=================================================================================================
//  Unary negation operators, which forward to the negation traits to do the work.
//=================================================================================================
//
template<class ET1, class OT1>
inline auto
operator -(vector<ET1, OT1> const& v1)
{
    using op1_type   = vector<ET1, OT1>;
    using op_traits  = OT1;
    using neg_traits = matrix_negation_traits_t<op_traits, op1_type>;

    return neg_traits::negate(v1);
}

template<class ET1, class OT1>
inline auto
operator -(matrix<ET1, OT1> const& m1)
{
    using op1_type   = matrix<ET1, OT1>;
    using op_traits  = OT1;
    using neg_traits = matrix_negation_traits_t<op_traits, op1_type>;

    return neg_traits::negate(m1);
}


//=================================================================================================
//  Multiplication operators, which forward to the multiplication traits to do the work.
//=================================================================================================
//- vector*scalar
//
template<class ET1, class OT1, class S2>
inline auto
operator *(vector<ET1, OT1> const& v1, S2 const& s2)
{
    static_assert(is_matrix_element_v<S2>);

    using op_traits  = OT1;
    using op1_type   = vector<ET1, OT1>;
    using op2_type   = S2;
    using mul_traits = matrix_multiplication_traits_t<op_traits, op1_type, op2_type>;

    return mul_traits::multiply(v1, s2);
}

template<class S1, class ET2, class OT2>
inline auto
operator *(S1 const& s1, vector<ET2, OT2> const& v2)
{
    static_assert(is_matrix_element_v<S1>);

    using op_traits  = OT2;
    using op1_type   = S1;
    using op2_type   = vector<ET2, OT2>;
    using mul_traits = matrix_multiplication_traits_t<op_traits, op1_type, op2_type>;

    return mul_traits::multiply(s1, v2);
}

//---------------
//- matrix*scalar
//
template<class ET1, class OT1, class S2>
inline auto
operator *(matrix<ET1, OT1> const& m1, S2 const& s2)
{
    static_assert(is_matrix_element_v<S2>);

    using op_traits  = OT1;
    using op1_type   = matrix<ET1, OT1>;
    using op2_type   = S2;
    using mul_traits = matrix_multiplication_traits_t<op_traits, op1_type, op2_type>;

    return mul_traits::multiply(m1, s2);
}

template<class S1, class ET2, class OT2>
inline auto
operator *(S1 const& s1, matrix<ET2, OT2> const& m2)
{
    static_assert(is_matrix_element_v<S1>);

    using op_traits  = OT2;
    using op1_type   = S1;
    using op2_type   = matrix<ET2, OT2>;
    using mul_traits = matrix_multiplication_traits_t<op_traits, op1_type, op2_type>;

    return mul_traits::multiply(s1, m2);
}

//---------------
//- vector*vector
//
template<class ET1, class OT1, class ET2, class OT2>
inline auto
operator *(vector<ET1, OT1> const& v1, vector<ET2, OT2> const& v2)
{
    using op_traits  = matrix_operation_traits_selector_t<OT1, OT2>;
    using op1_type   = vector<ET1, OT1>;
    using op2_type   = vector<ET2, OT2>;
    using mul_traits = matrix_multiplication_traits_t<op_traits, op1_type, op2_type>;

    return mul_traits::multiply(v1, v2);
}

//---------------
//- matrix*vector
//
template<class ET1, class OT1, class ET2, class OT2>
inline auto
operator *(matrix<ET1, OT1> const& m1, vector<ET2, OT2> const& v2)
{
    using op_traits  = matrix_operation_traits_selector_t<OT1, OT2>;
    using op1_type   = matrix<ET1, OT1>;
    using op2_type   = vector<ET2, OT2>;
    using mul_traits = matrix_multiplication_traits_t<op_traits, op1_type, op2_type>;

    return mul_traits::multiply(m1, v2);
}

//---------------
//- vector*matrix
//
template<class ET1, class OT1, class ET2, class OT2>
inline auto
operator *(vector<ET1, OT1> const& v1, matrix<ET2, OT2> const& m2)
{
    using op_traits  = matrix_operation_traits_selector_t<OT1, OT2>;
    using op1_type   = vector<ET1, OT1>;
    using op2_type   = matrix<ET2, OT2>;
    using mul_traits = matrix_multiplication_traits_t<op_traits, op1_type, op2_type>;

    return mul_traits::multiply(v1, m2);
}

//---------------
//- matrix*matrix
//
template<class ET1, class OT1, class ET2, class OT2>
inline auto
operator *(matrix<ET1, OT1> const& m1, matrix<ET2, OT2> const& m2)
{
    using op_traits  = matrix_operation_traits_selector_t<OT1, OT2>;
    using op1_type   = matrix<ET1, OT1>;
    using op2_type   = matrix<ET2, OT2>;
    using mul_traits = matrix_multiplication_traits_t<op_traits, op1_type, op2_type>;

    return mul_traits::multiply(m1, m2);
}

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_ARITHMETIC_OPERATORS_HPP_DEFINED
