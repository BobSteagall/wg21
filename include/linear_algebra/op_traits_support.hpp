//==================================================================================================
//  File:       op_traits_support.hpp
//
//  Summary:    This header defines library's default operation traits type and a customization
//              point that may be specialized by users.
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_OP_TRAITS_SUPPORT_HPP_DEFINED
#define LINEAR_ALGEBRA_OP_TRAITS_SUPPORT_HPP_DEFINED

namespace STD_LA {
namespace detail {
//--------------------------------------------------------------------------------------------------
//  Class:      engine_extents_helper<matrix_storage_engine<T, R, C, AT, LT>>
//
//  This partial specialization determines the number of rows and columns associated with
//  a specific specialization of `matrix_storage_engine`.
//--------------------------------------------------------------------------------------------------
//
template<class T, size_t R, size_t C, class AT, class LT>
struct engine_extents_helper<matrix_storage_engine<T, R, C, AT, LT>>
{
    static constexpr size_t
    columns()
    {
        return C;
    }

    static constexpr size_t
    rows()
    {
        return R;
    }

    static constexpr size_t
    size()
    {
        return R*C;
    }
};


//==================================================================================================
//                              **** ALLOCATION TRAITS ****
//==================================================================================================
//--------------------------------------------------------------------------------------------------
//  Class:  mse_allocation_traits
//
//  This type performs allocator promotion when both engine types are matrix_storage_engine.  The
//  idea is straightforward:
//    a. If both allocator types are void, then the resulting allocator type is void;
//    b. If one allocator is std::allocator, and the other is custom, rebind from the custom
//       allocator type;
//    c. If both allocator types are std::allocator, the result is std::allocator.
//--------------------------------------------------------------------------------------------------
//
template<class AT1, class AT2, class T>
struct mse_allocation_traits
{
    using allocator_type = std::allocator<T>;
};

template<class AT1, class T>
struct mse_allocation_traits<AT1, AT1, T>
{
    using traits_type    = std::allocator_traits<AT1>;
    using allocator_type = typename traits_type::template rebind_alloc<T>;
};

template<class AT1, class V, class T>
struct mse_allocation_traits<AT1, std::allocator<V>, T>
{
    using traits_type    = std::allocator_traits<AT1>;
    using allocator_type = typename traits_type::template rebind_alloc<T>;
};

template<class U, class AT2, class T>
struct mse_allocation_traits<std::allocator<U>, AT2, T>
{
    using traits_type    = std::allocator_traits<AT2>;
    using allocator_type = typename traits_type::template rebind_alloc<T>;
};

template<class U, class V, class T>
struct mse_allocation_traits<std::allocator<U>, std::allocator<V>, T>
{
    using allocator_type = std::allocator<T>;
};

template<class U, class T>
struct mse_allocation_traits<std::allocator<U>, std::allocator<U>, T>
{
    using allocator_type = std::allocator<T>;
};

template<class AT2, class T>
struct mse_allocation_traits<void, AT2, T>
{
    using traits_type    = std::allocator_traits<AT2>;
    using allocator_type = typename traits_type::template rebind_alloc<T>;
};

template<class AT1, class T>
struct mse_allocation_traits<AT1, void, T>
{
    using traits_type    = std::allocator_traits<AT1>;
    using allocator_type = typename traits_type::template rebind_alloc<T>;
};

template<class V, class T>
struct mse_allocation_traits<void, std::allocator<V>, T>
{
    using allocator_type = std::allocator<T>;
};

template<class U, class T>
struct mse_allocation_traits<std::allocator<U>, void, T>
{
    using allocator_type = std::allocator<T>;
};

template<class T>
struct mse_allocation_traits<void, void, T>
{
    using allocator_type = void;
};

//------
//
template<class ET1, class ET2, bool DX, size_t RR, size_t CR, class T>
struct engine_allocation_traits
{
    static constexpr bool   dyn_size = DX;
    using allocator_type = std::conditional_t<dyn_size, std::allocator<T>, void>;
};

template<class T1, size_t R1, size_t C1, class AT1, class LT1,
         class T2, size_t R2, size_t C2, class AT2, class LT2,
         bool DX, size_t RR, size_t CR, class T>
struct engine_allocation_traits<matrix_storage_engine<T1, R1, C1, AT1, LT1>,
                                matrix_storage_engine<T2, R2, C2, AT2, LT2>,
                                DX, RR, CR, T>
{
    using allocator_type = typename mse_allocation_traits<AT1, AT2, T>::allocator_type;
};

}       //- detail namespace
}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_OP_TRAITS_SUPPORT_HPP_DEFINED
