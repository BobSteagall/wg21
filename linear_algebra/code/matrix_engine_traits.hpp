#ifndef MATRIX_ENGINE_TRAITS_HPP_DEFINED
#define MATRIX_ENGINE_TRAITS_HPP_DEFINED

#include "matrix_engines.hpp"

namespace std::la {
//=================================================================================================
//                                      **** NEGATION ****
//=================================================================================================
//  Traits type that performs engine promotion type computations for unary negation.
//=================================================================================================
//
template<class E1>
struct matrix_engine_negate_promotion;

//- Note that in the cases where allocators are rebound, there is an assumption that all
//  allocators are standard-conformant.
//
template<class T1, class A1>
struct matrix_engine_negate_promotion<dyn_matrix_engine<T1, A1>>
{
    using engine_type = dyn_matrix_engine<T1, A1>;
};

template<class T1, size_t R1, size_t C1>
struct matrix_engine_negate_promotion<fs_matrix_engine<T1, R1, C1>>
{
    using engine_type = fs_matrix_engine<T1, R1, C1>;
};

//- Transpose case.
//
template<class E1>
struct matrix_engine_negate_promotion<matrix_transpose_engine<E1>>
{
    using engine_type = E1;
};


//- Alias interface to trait.
//
template<class E1>
using matrix_engine_negate_t = typename matrix_engine_negate_promotion<E1>::engine_type;


//=================================================================================================
//                                      **** ADDITION ****
//=================================================================================================
//  Traits type that performs engine promotion type computations for binary addition.
//=================================================================================================
//
template<class E1, class E2>
struct matrix_engine_add_promotion;

//- Note that in the cases where allocators are rebound, there is an assumption that all
//  allocators are standard-conformant.
//
template<class T1, class A1, class T2, class A2>
struct matrix_engine_add_promotion<dyn_matrix_engine<T1, A1>, dyn_matrix_engine<T2, A2>>
{
    using element_type = matrix_element_promotion_t<T1, T2>;
    using alloc_type   = typename std::allocator_traits<A1>::template rebind_alloc<element_type>;
    using engine_type  = dyn_matrix_engine<element_type, alloc_type>;
};

template<class T1, class A1, class T2, size_t R2, size_t C2>
struct matrix_engine_add_promotion<dyn_matrix_engine<T1, A1>, fs_matrix_engine<T2, R2, C2>>
{
    using element_type = matrix_element_promotion_t<T1, T2>;
    using alloc_type   = typename std::allocator_traits<A1>::template rebind_alloc<element_type>;
    using engine_type  = dyn_matrix_engine<element_type, alloc_type>;
};

template<class T1, size_t R1, size_t C1, class T2, class A2>
struct matrix_engine_add_promotion<fs_matrix_engine<T1, R1, C1>, dyn_matrix_engine<T2, A2>>
{
    using element_type = matrix_element_promotion_t<T1, T2>;
    using alloc_type   = typename std::allocator_traits<A2>::template rebind_alloc<element_type>;
    using engine_type  = dyn_matrix_engine<element_type, alloc_type>;
};

template<class T1, size_t R1, size_t C1, class T2, size_t R2, size_t C2>
struct matrix_engine_add_promotion<fs_matrix_engine<T1, R1, C1>, fs_matrix_engine<T2, R2, C2>>
{
    static_assert(R1 == R2);
    static_assert(C1 == C2);
    using element_type = matrix_element_promotion_t<T1, T2>;
    using engine_type  = fs_matrix_engine<element_type, R1, C2>;
};

//- Transpose cases.
//
template<class E1, class E2>
struct matrix_engine_add_promotion<matrix_transpose_engine<E1>, E2>
{
    using engine_type = typename matrix_engine_add_promotion<E1, E2>::engine_type;
};

template<class E1, class E2>
struct matrix_engine_add_promotion<E1, matrix_transpose_engine<E2>>
{
    using engine_type = typename matrix_engine_add_promotion<E1, E2>::engine_type;
};

template<class E1, class E2>
struct matrix_engine_add_promotion<matrix_transpose_engine<E1>, matrix_transpose_engine<E2>>
{
    using engine_type = typename matrix_engine_add_promotion<E1, E2>::engine_type;
};

template<class T1, size_t R1, size_t C1, class T2, size_t R2, size_t C2>
struct matrix_engine_add_promotion<matrix_transpose_engine<fs_matrix_engine<T1, R1, C1>>,
                                   fs_matrix_engine<T2, R2, C2>>
{
    static_assert(R1 == C2);
    static_assert(C1 == R2);
    using element_type = matrix_element_promotion_t<T1, T2>;
    using engine_type  = fs_matrix_engine<element_type, R1, C2>;
};

template<class T1, size_t R1, size_t C1, class T2, size_t R2, size_t C2>
struct matrix_engine_add_promotion<fs_matrix_engine<T1, R1, C1>,
                                   matrix_transpose_engine<fs_matrix_engine<T2, R2, C2>>>
{
    static_assert(R1 == C2);
    static_assert(C1 == R2);
    using element_type = matrix_element_promotion_t<T1, T2>;
    using engine_type  = fs_matrix_engine<element_type, R1, C2>;
};


//- Alias interface to trait.
//
template<class E1, class E2>
using matrix_engine_add_t = typename matrix_engine_add_promotion<E1, E2>::engine_type;


//=================================================================================================
//                                     **** SUBTRACTION ****
//=================================================================================================
//  Traits type that performs engine promotion type computations for binary addition.
//=================================================================================================
//
template<class E1, class E2>
struct matrix_engine_subtract_promotion;

//- Note that in the cases where allocators are rebound, there is an assumption that all
//  allocators are standard-conformant.
//
template<class T1, class A1, class T2, class A2>
struct matrix_engine_subtract_promotion<dyn_matrix_engine<T1, A1>, dyn_matrix_engine<T2, A2>>
{
    using element_type = matrix_element_promotion_t<T1, T2>;
    using alloc_type   = typename std::allocator_traits<A1>::template rebind_alloc<element_type>;
    using engine_type  = dyn_matrix_engine<element_type, alloc_type>;
};

template<class T1, class A1, class T2, size_t R2, size_t C2>
struct matrix_engine_subtract_promotion<dyn_matrix_engine<T1, A1>, fs_matrix_engine<T2, R2, C2>>
{
    using element_type = matrix_element_promotion_t<T1, T2>;
    using alloc_type   = typename std::allocator_traits<A1>::template rebind_alloc<element_type>;
    using engine_type  = dyn_matrix_engine<element_type, alloc_type>;
};

template<class T1, size_t R1, size_t C1, class T2, class A2>
struct matrix_engine_subtract_promotion<fs_matrix_engine<T1, R1, C1>, dyn_matrix_engine<T2, A2>>
{
    using element_type = matrix_element_promotion_t<T1, T2>;
    using alloc_type   = typename std::allocator_traits<A2>::template rebind_alloc<element_type>;
    using engine_type  = dyn_matrix_engine<element_type, alloc_type>;
};

template<class T1, size_t R1, size_t C1, class T2, size_t R2, size_t C2>
struct matrix_engine_subtract_promotion<fs_matrix_engine<T1, R1, C1>, fs_matrix_engine<T2, R2, C2>>
{
    static_assert(R1 == R2);
    static_assert(C1 == C2);
    using element_type = matrix_element_promotion_t<T1, T2>;
    using engine_type  = fs_matrix_engine<element_type, R1, C2>;
};

//- Transpose cases.
//
template<class E1, class E2>
struct matrix_engine_subtract_promotion<matrix_transpose_engine<E1>, E2>
{
    using engine_type = typename matrix_engine_subtract_promotion<E1, E2>::engine_type;
};

template<class E1, class E2>
struct matrix_engine_subtract_promotion<E1, matrix_transpose_engine<E2>>
{
    using engine_type = typename matrix_engine_subtract_promotion<E1, E2>::engine_type;
};

template<class E1, class E2>
struct matrix_engine_subtract_promotion<matrix_transpose_engine<E1>, matrix_transpose_engine<E2>>
{
    using engine_type = typename matrix_engine_subtract_promotion<E1, E2>::engine_type;
};

template<class T1, size_t R1, size_t C1, class T2, size_t R2, size_t C2>
struct matrix_engine_subtract_promotion<matrix_transpose_engine<fs_matrix_engine<T1, R1, C1>>,
                                        fs_matrix_engine<T2, R2, C2>>
{
    static_assert(R1 == C2);
    static_assert(C1 == R2);
    using element_type = matrix_element_promotion_t<T1, T2>;
    using engine_type  = fs_matrix_engine<element_type, R1, C2>;
};

template<class T1, size_t R1, size_t C1, class T2, size_t R2, size_t C2>
struct matrix_engine_subtract_promotion<fs_matrix_engine<T1, R1, C1>,
                                        matrix_transpose_engine<fs_matrix_engine<T2, R2, C2>>>
{
    static_assert(R1 == C2);
    static_assert(C1 == R2);
    using element_type = matrix_element_promotion_t<T1, T2>;
    using engine_type  = fs_matrix_engine<element_type, R1, C2>;
};


//- Alias interface to trait.
//
template<class E1, class E2>
using matrix_engine_subtract_t = typename matrix_engine_subtract_promotion<E1, E2>::engine_type;


//=================================================================================================
//                                   **** MULTIPLICATION ****
//=================================================================================================
//  Traits type that performs engine promotion type computations for multiplication.
//=================================================================================================
//
template<class E1, class E2>
struct matrix_engine_multiply_promotion;

//- engine * scalar cases.
//
template<class T1, class A1, class T2>
struct matrix_engine_multiply_promotion<dyn_matrix_engine<T1, A1>, T2>
{
    static_assert(is_matrix_element_v<T2>);

    using element_type = matrix_element_promotion_t<T1, T2>;
    using alloc_type   = typename std::allocator_traits<A1>::template rebind_alloc<element_type>;
    using engine_type  = dyn_matrix_engine<element_type, alloc_type>;
};

template<class T1, size_t R1, size_t C1, class T2>
struct matrix_engine_multiply_promotion<fs_matrix_engine<T1, R1, C1>, T2>
{
    static_assert(is_matrix_element_v<T2>);

    using element_type = matrix_element_promotion_t<T1, T2>;
    using engine_type  = fs_matrix_engine<element_type, R1, C1>;
};


//- engine * engine cases.  Note that in the cases where allocators are rebound, there is an
//  assumption that all allocators are standard-conformant.
//
template<class T1, class A1, class T2, class A2>
struct matrix_engine_multiply_promotion<dyn_matrix_engine<T1, A1>, dyn_matrix_engine<T2, A2>>
{
    using element_type = matrix_element_promotion_t<T1, T2>;
    using alloc_type   = typename std::allocator_traits<A1>::template rebind_alloc<element_type>;
    using engine_type  = dyn_matrix_engine<element_type, alloc_type>;
};

template<class T1, class A1, class T2, size_t R2, size_t C2>
struct matrix_engine_multiply_promotion<dyn_matrix_engine<T1, A1>, fs_matrix_engine<T2, R2, C2>>
{
    using element_type = matrix_element_promotion_t<T1, T2>;
    using alloc_type   = typename std::allocator_traits<A1>::template rebind_alloc<element_type>;
    using engine_type  = dyn_matrix_engine<element_type, alloc_type>;
};

template<class T1, size_t R1, size_t C1, class T2, class A2>
struct matrix_engine_multiply_promotion<fs_matrix_engine<T1, R1, C1>, dyn_matrix_engine<T2, A2>>
{
    using element_type = matrix_element_promotion_t<T1, T2>;
    using alloc_type   = typename std::allocator_traits<A2>::template rebind_alloc<element_type>;
    using engine_type  = dyn_matrix_engine<element_type, alloc_type>;
};

template<class T1, size_t R1, size_t C1, class T2, size_t R2, size_t C2>
struct matrix_engine_multiply_promotion<fs_matrix_engine<T1, R1, C1>, fs_matrix_engine<T2, R2, C2>>
{
    static_assert(C1 == R2);
    using element_type = matrix_element_promotion_t<T1, T2>;
    using engine_type  = fs_matrix_engine<element_type, R1, C2>;
};

//- Transpose cases.
//
template<class E1, class E2>
struct matrix_engine_multiply_promotion<matrix_transpose_engine<E1>, E2>
{
    using engine_type = typename matrix_engine_multiply_promotion<E1, E2>::engine_type;
};

template<class E1, class E2>
struct matrix_engine_multiply_promotion<E1, matrix_transpose_engine<E2>>
{
    using engine_type = typename matrix_engine_multiply_promotion<E1, E2>::engine_type;
};

template<class E1, class E2>
struct matrix_engine_multiply_promotion<matrix_transpose_engine<E1>, matrix_transpose_engine<E2>>
{
    using engine_type = typename matrix_engine_multiply_promotion<E1, E2>::engine_type;
};

template<class T1, size_t R1, size_t C1, class T2, size_t R2, size_t C2>
struct matrix_engine_multiply_promotion<matrix_transpose_engine<fs_matrix_engine<T1, R1, C1>>,
                                        fs_matrix_engine<T2, R2, C2>>
{
    static_assert(R1 == R2);
    using element_type = matrix_element_promotion_t<T1, T2>;
    using engine_type  = fs_matrix_engine<element_type, C1, C2>;
};

template<class T1, size_t R1, size_t C1, class T2, size_t R2, size_t C2>
struct matrix_engine_multiply_promotion<fs_matrix_engine<T1, R1, C1>,
                                        matrix_transpose_engine<fs_matrix_engine<T2, R2, C2>>>
{
    static_assert(C1 == C2);
    using element_type = matrix_element_promotion_t<T1, T2>;
    using engine_type  = fs_matrix_engine<element_type, R1, R2>;
};

template<class T1, size_t R1, size_t C1, class T2, size_t R2, size_t C2>
struct matrix_engine_multiply_promotion<matrix_transpose_engine<fs_matrix_engine<T1, R1, C1>>,
                                        matrix_transpose_engine<fs_matrix_engine<T2, R2, C2>>>
{
    static_assert(R1 == C2);
    using element_type = matrix_element_promotion_t<T1, T2>;
    using engine_type  = fs_matrix_engine<element_type, C1, R2>;
};

//- Alias interface to trait.
//
template<class E1, class E2>
using matrix_engine_multiply_t = typename matrix_engine_multiply_promotion<E1, E2>::engine_type;

}       //- std::la namespace
#endif  //- MATRIX_ENGINE_TRAITS_HPP_DEFINED
