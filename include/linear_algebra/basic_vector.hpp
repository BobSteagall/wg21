//==================================================================================================
//  File:       basic_vector.hpp
//
//  Summary:    This header defines the basic_vector class template, one of the two primary
//              math objects provided by the library.
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_BASIC_VECTOR_HPP_DEFINED
#define LINEAR_ALGEBRA_BASIC_VECTOR_HPP_DEFINED

namespace STD_LA {

template<class ET, class OT>
requires
    detail::copyable<ET>
    and
    detail::default_initializable<ET>
    and
    detail::readable_matrix_engine<ET>
class basic_matrix;


template<class ET, class OT>
requires
    detail::copyable<ET>
    and
    detail::default_initializable<ET>
    and
    detail::readable_vector_engine<ET>
class basic_vector
{
    static constexpr bool   engine_has_mdspan = detail::spannable_vector_engine<ET>;

    using engine_support = detail::vector_engine_support;

  public:
    using engine_type          = ET;
    using owning_engine_type   = detail::get_owning_engine_type_t<ET>;
    using element_type         = typename engine_type::element_type;
    using value_type           = typename engine_type::value_type;
    using reference            = typename engine_type::reference;
    using const_reference      = typename engine_type::const_reference;
    using index_type           = typename engine_type::index_type;
    using span_type            = detail::get_mdspan_type_t<ET>;
    using const_span_type      = detail::get_const_mdspan_type_t<ET>;

  public:
    ~basic_vector() = default;

    constexpr basic_vector() = default;
    constexpr basic_vector(basic_vector&&) noexcept = default;
    constexpr basic_vector(basic_vector const&) = default;

    basic_vector&   operator =(basic_vector&&) noexcept = default;
    basic_vector&   operator =(basic_vector const&) = default;

    //----------------------------------------------------------
    //- Other constructors.
    //
    constexpr explicit
    basic_vector(index_type size)
    requires
        detail::reshapable_vector_engine<engine_type>
    :   m_engine(size, size)
    {}

    constexpr explicit
    basic_vector(index_type size, index_type cap)
    requires
        detail::reshapable_vector_engine<engine_type>
    :   m_engine(size, cap)
    {}

    //----------------------------------------------------------
    //- Construction from a vector of different engine type.
    //
    template<class ET2, class OT2>
    constexpr explicit
    basic_vector(basic_vector<ET2, OT2> const& rhs)
    requires
        detail::writable_vector_engine<engine_type>
        and
        detail::constructible_from<engine_type, ET2>
    :   m_engine(rhs)
    {}

    template<class ET2, class OT2>
    constexpr explicit
    basic_vector(basic_vector<ET2, OT2> const& rhs)
    requires
        detail::writable_vector_engine<engine_type>
        and
        detail::not_constructible_from<engine_type, ET2>
        and
        detail::convertible_from<element_type, typename ET2::element_type>
    :   m_engine()
    {
        engine_support::assign_from(m_engine, rhs.engine());
    }

    //----------------------------------------------------------
    //- Construction from a row/column basic_matrix.
    //
    template<class ET2, class OT2>
    constexpr explicit
    basic_vector(basic_matrix<ET2, OT2> const& rhs)
    requires
        detail::writable_vector_engine<engine_type>
        and
        detail::readable_and_1d_indexable_matrix_engine<ET2>
        and
        detail::constructible_from<engine_type, ET2>
    :   m_engine(rhs.engine())
    {}

    template<class ET2, class OT2>
    constexpr explicit
    basic_vector(basic_matrix<ET2, OT2> const& rhs)
    requires
        detail::writable_vector_engine<engine_type>
        and
        detail::readable_and_1d_indexable_matrix_engine<ET2>
        and
        detail::not_constructible_from<engine_type, ET2>
        and
        detail::convertible_from<element_type, typename ET2::element_type>
    :   m_engine()
    {
        engine_support::assign_from(m_engine, rhs.engine());
    }

    //----------------------------------------------------------
    //- Construction from a 1D standard sequence container.
    //
    template<class CT>
    constexpr explicit
    basic_vector(CT const& rhs)
    requires
        detail::writable_vector_engine<ET>
        and
        detail::random_access_standard_container<CT>
        and
        detail::constructible_from<engine_type, CT>
        and
        detail::convertible_from<typename ET::element_type, typename CT::value_type>
    :   m_engine(rhs)
    {}

    template<class CT>
    constexpr explicit
    basic_vector(CT const& rhs)
    requires
        detail::writable_vector_engine<ET>
        and
        detail::random_access_standard_container<CT>
        and
        detail::not_constructible_from<engine_type, CT>
        and
        detail::convertible_from<typename ET::element_type, typename CT::value_type>
    :   m_engine()
    {
        engine_support::assign_from(m_engine, rhs);
    }

    //----------------------------------------------------------
    //- Construction from a 1D mdspan.
    //
    template<class U, ptrdiff_t X0, class L, class A>
    constexpr explicit
    basic_vector(basic_mdspan<U, extents<X0>, L, A> const& rhs)
    requires
        detail::writable_vector_engine<engine_type>
        and
        detail::constructible_from<engine_type, decltype(rhs)>
    :   m_engine(rhs)
    {}

    template<class U, ptrdiff_t X0, class L, class A>
    constexpr explicit
    basic_vector(basic_mdspan<U, extents<X0>, L, A> const& rhs)
    requires
        detail::writable_vector_engine<engine_type>
        and
        detail::not_constructible_from<engine_type, decltype(rhs)>
        and
        detail::convertible_from<element_type, U>
    :   m_engine()
    {
        engine_support::assign_from(m_engine, rhs);
    }

    //----------------------------------------------------------
    //- Construction from a 1D initialization list.
    //
    template<class U>
    constexpr
    basic_vector(initializer_list<U> rhs)
    requires
        detail::writable_vector_engine<engine_type>
        and
        detail::constructible_from<engine_type, decltype(rhs)>
    :   m_engine(rhs)
    {}

    template<class U>
    constexpr
    basic_vector(initializer_list<U> rhs)
    requires
        detail::writable_vector_engine<engine_type>
        and
        detail::not_constructible_from<engine_type, decltype(rhs)>
        and
        detail::convertible_from<element_type, U>
    :   m_engine()
    {
        engine_support::assign_from(m_engine, rhs);
    }

    //----------------------------------------------------------
    //- Assignment from a vector of different engine type.
    //
    template<class ET2, class OT2>
    constexpr basic_vector&
    operator =(basic_vector<ET2, OT2> const& rhs)
    requires
        detail::writable_vector_engine<engine_type>
        and
        detail::assignable_from<engine_type, ET2>
    {
        m_engine = rhs.engine();
        return *this;
    }

    template<class ET2, class OT2>
    constexpr basic_vector&
    operator =(basic_vector<ET2, OT2> const& rhs)
    requires
        detail::writable_vector_engine<engine_type>
        and
        detail::not_assignable_from<engine_type, ET2>
        and
        detail::convertible_from<element_type, typename ET2::element_type>
    {
        engine_support::assign_from(m_engine, rhs.engine());
        return *this;
    }

    //----------------------------------------------------------
    //- Assignment from a row/column basic_matrix.
    //
    template<class ET2, class OT2>
    constexpr basic_vector&
    operator =(basic_matrix<ET2, OT2> const& rhs)
    requires
        detail::writable_vector_engine<engine_type>
        and
        detail::readable_and_1d_indexable_matrix_engine<ET2>
        and
        detail::assignable_from<engine_type, ET2>
    {
        m_engine = rhs.engine();
        return *this;
    }

    template<class ET2, class OT2>
    constexpr basic_vector&
    operator =(basic_matrix<ET2, OT2> const& rhs)
    requires
        detail::writable_vector_engine<engine_type>
        and
        detail::readable_and_1d_indexable_matrix_engine<ET2>
        and
        detail::not_assignable_from<engine_type, ET2>
        and
        detail::convertible_from<element_type, typename ET2::element_type>
    {
        engine_support::assign_from(m_engine, rhs.engine());
        return *this;
    }

    //----------------------------------------------------------
    //- Assignment from a 1D standard sequence container.
    //
    template<class CT>
    constexpr basic_vector&
    operator =(CT const& rhs)
    requires
        detail::writable_vector_engine<engine_type>
        and
        detail::random_access_standard_container<CT>
        and
        detail::assignable_from<engine_type, decltype(rhs)>
        and
        detail::convertible_from<element_type, typename CT::value_type>
    {
        m_engine = rhs;
        return *this;
    }

    template<class CT>
    constexpr basic_vector&
    operator =(CT const& rhs)
    requires
        detail::writable_vector_engine<ET>
        and
        detail::random_access_standard_container<CT>
        and
        detail::not_assignable_from<engine_type, decltype(rhs)>
        and
        detail::convertible_from<element_type, typename CT::value_type>
    {
        engine_support::assign_from(m_engine, rhs);
        return *this;
    }

    //----------------------------------------------------------
    //- Assignment from a 1D mdspan.
    //
    template<class U, ptrdiff_t X0, class L, class A>
    constexpr basic_vector&
    operator =(basic_mdspan<U, extents<X0>, L, A> const& rhs)
    requires
        detail::writable_vector_engine<engine_type>
        and
        detail::assignable_from<engine_type, decltype(rhs)>
    {
        m_engine = rhs;
        return *this;
    }

    template<class U, ptrdiff_t X0, class L, class A>
    constexpr basic_vector&
    operator =(basic_mdspan<U, extents<X0>, L, A> const& rhs)
    requires
        detail::writable_vector_engine<engine_type>
        and
        detail::not_assignable_from<engine_type, decltype(rhs)>
        and
        detail::convertible_from<element_type, U>
    {
        engine_support::assign_from(m_engine, rhs);
        return *this;
    }

    //----------------------------------------------------------
    //- Assignment from a 1D initializer list.
    //
    template<class U>
    constexpr basic_vector&
    operator =(initializer_list<U> rhs)
    requires
        detail::writable_vector_engine<engine_type>
        and
        detail::assignable_from<engine_type, decltype(rhs)>
    {
        m_engine = rhs;
        return *this;
    }

    template<class U>
    constexpr basic_vector&
    operator =(initializer_list<U> rhs)
    requires
        detail::writable_vector_engine<engine_type>
        and
        detail::not_assignable_from<engine_type, decltype(rhs)>
        and
        detail::convertible_from<element_type, U>
    {
        engine_support::assign_from(m_engine, rhs);
        return *this;
    }

    //----------------------------------------------------------
    //- Size and capacity reporting.
    //
    constexpr index_type
    elements() const noexcept
    {
        return m_engine.elements();
    }

    constexpr index_type
    columns() const noexcept
    {
        return m_engine.columns();
    }

    constexpr index_type
    rows() const noexcept
    {
        return m_engine.rows();
    }

    constexpr index_type
    size() const noexcept
    {
        return m_engine.size();
    }

    constexpr index_type
    column_capacity() const noexcept
    {
        return m_engine.column_capacity();
    }

    constexpr index_type
    row_capacity() const noexcept
    {
        return m_engine.row_capacity();
    }

    constexpr index_type
    capacity() const noexcept
    {
        return m_engine.capacity();
    }

    //----------------------------------------------------------
    //- Element access.
    //
    constexpr reference
    operator ()(index_type i)
    {
        return m_engine(i);
    }

    constexpr const_reference
    operator ()(index_type i) const
    {
        return m_engine(i);
    }

    //----------------------------------------------------------
    //- Data access.
    //
    constexpr engine_type&
    engine() noexcept
    {
        return m_engine;
    }

    constexpr engine_type const&
    engine() const noexcept
    {
        return m_engine;
    }

    constexpr span_type
    span() noexcept
    requires
        detail::spannable_vector_engine<engine_type>
    {
        return m_engine.span();
    }

    constexpr const_span_type
    span() const noexcept
    requires
        detail::spannable_vector_engine<engine_type>
    {
        return m_engine.span();
    }

    //----------------------------------------------------------
    //- Setting overall size and capacity.
    //
    constexpr void
    resize(index_type size)
    requires
        detail::reshapable_vector_engine<engine_type>
    {
        m_engine.reshape(size, m_engine.capacity());
    }

    constexpr void
    reserve(index_type cap)
    requires
        detail::reshapable_vector_engine<engine_type>
    {
        m_engine.reshape(m_engine.size(), cap);
    }

    constexpr void
    reshape(index_type size, index_type cap)
    requires
        detail::reshapable_vector_engine<engine_type>
    {
        m_engine.reshape(size, cap);
    }

    //----------------------------------------------------------
    //- Other modifiers.
    //
    constexpr void
    swap(basic_vector& rhs) noexcept
    {
        m_engine.swap(rhs.engine());
    }

  private:
    template<class ET2, class OT2>
    requires
        detail::copyable<ET2>
        and
        detail::default_initializable<ET2>
        and
        detail::readable_vector_engine<ET2>
    friend class basic_vector;

    engine_type     m_engine;
};

template<class T, class OT = matrix_operation_traits>
using dyn_vec = basic_vector<matrix_storage_engine<T, extents<dynamic_extent>, std::allocator<T>, unoriented>, OT>;

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_BASIC_VECTOR_HPP_DEFINED
