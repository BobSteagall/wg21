#ifndef LINEAR_ALGEBRA_ENGINE_PROMOTION_TRAITS_HPP_DEFINED
#define LINEAR_ALGEBRA_ENGINE_PROMOTION_TRAITS_HPP_DEFINED

namespace STD_LA {
namespace detail {
//- Testing to see if an engine has mutable element indexing.
//
template<class ET>
constexpr bool  has_mutable_tag_v = ET::engine_category::value >= mutable_matrix_engine_tag::value;

template<class ET1, class ET2>
constexpr bool  is_mutable_engine_v = is_same_v<ET1, ET2> && has_mutable_tag_v<ET1>;


template<class ET1, class ET2>
using enable_if_mutable = enable_if_t<is_mutable_engine_v<ET1, ET2>, bool>;


//- Testing to see if an engine is resizable.
//
template<class ET>
constexpr bool  has_resizable_tag_v = ET::engine_category::value >= resizable_matrix_engine_tag::value;

template<class ET1, class ET2>
constexpr bool  is_resizable_engine_v = is_same_v<ET1, ET2> && has_resizable_tag_v<ET1>;


template<class ET1, class ET2>
using enable_if_resizable = enable_if_t<is_resizable_engine_v<ET1, ET2>, bool>;

}   //- namespace detail
//=================================================================================================
//                                      **** NEGATION ****
//=================================================================================================
//  Traits type that performs engine promotion type computations for unary negation.
//=================================================================================================
//
template<class ET1>
struct matrix_engine_negation_traits
{
    using element_type = typename ET1::element_type;
    using engine_type  = dr_matrix_engine<element_type>;
};

//- Note that in the cases where allocators are rebound, there is an assumption that all
//  allocators are standard-conformant.
//
template<class T1, class A1>
struct matrix_engine_negation_traits<dr_matrix_engine<T1, A1>>
{
    using engine_type = dr_matrix_engine<T1, A1>;
};

template<class T1, int32_t R1, int32_t C1>
struct matrix_engine_negation_traits<fs_matrix_engine<T1, R1, C1>>
{
    using engine_type = fs_matrix_engine<T1, R1, C1>;
};

//- Transpose case.
//
template<class ET1>
struct matrix_engine_negation_traits<matrix_transpose_engine<ET1>>
{
    using engine_type = ET1;
};

//- Alias interface to trait.
//
template<class ET1>
using matrix_negation_engine_t = typename matrix_engine_negation_traits<ET1>::engine_type;


//=================================================================================================
//                                      **** ADDITION ****
//=================================================================================================
//  Traits type that performs engine promotion type computations for binary addition.
//=================================================================================================
//
/*
template<class ET1, class ET2>
struct matrix_engine_addition_traits
{
    using traits_category = matrix_engine_addition_traits_tag;
    using element_type_1  = typename ET1::element_type;
    using element_type_2  = typename ET2::element_type;
    using element_type    = matrix_element_promotion_t<element_type_1, element_type_2>;
    using engine_type     = dr_matrix_engine<element_type>;
};

//- Note that all cases where allocators are rebound assume standard-conformant allocator types.
//
template<class T1, class A1, class T2, class A2>
struct matrix_engine_addition_traits<dr_matrix_engine<T1, A1>, dr_matrix_engine<T2, A2>>
{
    using traits_category = matrix_engine_addition_traits_tag;
    using element_type    = matrix_element_promotion_t<T1, T2>;
    using alloc_type      = typename allocator_traits<A1>::template rebind_alloc<element_type>;
    using engine_type     = dr_matrix_engine<element_type, alloc_type>;
};

template<class T1, class A1, class T2, int32_t R2, int32_t C2>
struct matrix_engine_addition_traits<dr_matrix_engine<T1, A1>, fs_matrix_engine<T2, R2, C2>>
{
    using traits_category = matrix_engine_addition_traits_tag;
    using element_type    = matrix_element_promotion_t<T1, T2>;
    using alloc_type      = typename allocator_traits<A1>::template rebind_alloc<element_type>;
    using engine_type     = dr_matrix_engine<element_type, alloc_type>;
};

template<class T1, int32_t R1, int32_t C1, class T2, class A2>
struct matrix_engine_addition_traits<fs_matrix_engine<T1, R1, C1>, dr_matrix_engine<T2, A2>>
{
    using traits_category = matrix_engine_addition_traits_tag;
    using element_type    = matrix_element_promotion_t<T1, T2>;
    using alloc_type      = typename allocator_traits<A2>::template rebind_alloc<element_type>;
    using engine_type     = dr_matrix_engine<element_type, alloc_type>;
};

template<class T1, int32_t R1, int32_t C1, class T2, int32_t R2, int32_t C2>
struct matrix_engine_addition_traits<fs_matrix_engine<T1, R1, C1>, fs_matrix_engine<T2, R2, C2>>
{
    static_assert(R1 == R2);
    static_assert(C1 == C2);
    using traits_category = matrix_engine_addition_traits_tag;
    using element_type    = matrix_element_promotion_t<T1, T2>;
    using engine_type     = fs_matrix_engine<element_type, R1, C1>;
};

//- Transpose cases.
//
template<class ET1, class ET2>
struct matrix_engine_addition_traits<matrix_transpose_engine<ET1>, ET2>
{
    using traits_category = matrix_engine_addition_traits_tag;
    using engine_type     = typename matrix_engine_addition_traits<ET1, ET2>::engine_type;
};

template<class ET1, class ET2>
struct matrix_engine_addition_traits<ET1, matrix_transpose_engine<ET2>>
{
    using traits_category = matrix_engine_addition_traits_tag;
    using engine_type     = typename matrix_engine_addition_traits<ET1, ET2>::engine_type;
};

template<class ET1, class ET2>
struct matrix_engine_addition_traits<matrix_transpose_engine<ET1>, matrix_transpose_engine<ET2>>
{
    using traits_category = matrix_engine_addition_traits_tag;
    using engine_type     = typename matrix_engine_addition_traits<ET1, ET2>::engine_type;
};

template<class T1, int32_t R1, int32_t C1, class T2, int32_t R2, int32_t C2>
struct matrix_engine_addition_traits<matrix_transpose_engine<fs_matrix_engine<T1, R1, C1>>,
                                        fs_matrix_engine<T2, R2, C2>>
{
    static_assert(R1 == C2);
    static_assert(C1 == R2);
    using traits_category = matrix_engine_addition_traits_tag;
    using element_type    = matrix_element_promotion_t<T1, T2>;
    using engine_type     = fs_matrix_engine<element_type, R2, C2>;
};

template<class T1, int32_t R1, int32_t C1, class T2, int32_t R2, int32_t C2>
struct matrix_engine_addition_traits<fs_matrix_engine<T1, R1, C1>,
                                        matrix_transpose_engine<fs_matrix_engine<T2, R2, C2>>>
{
    static_assert(R1 == C2);
    static_assert(C1 == R2);
    using traits_category = matrix_engine_addition_traits_tag;
    using element_type    = matrix_element_promotion_t<T1, T2>;
    using engine_type     = fs_matrix_engine<element_type, R1, C1>;
};

template<class T1, int32_t R1, int32_t C1, class T2, int32_t R2, int32_t C2>
struct matrix_engine_addition_traits<matrix_transpose_engine<fs_matrix_engine<T1, R1, C1>>,
                                        matrix_transpose_engine<fs_matrix_engine<T2, R2, C2>>>
{
    static_assert(R1 == R2);
    static_assert(C1 == C2);
    using traits_category = matrix_engine_addition_traits_tag;
    using element_type    = matrix_element_promotion_t<T1, T2>;
    using engine_type     = fs_matrix_engine<element_type, C1, R1>;
};

//- Alias interface to trait.
//
template<class ET1, class ET2>
using matrix_addition_engine_t = typename matrix_engine_addition_traits<ET1, ET2>::engine_type;
*/

//=================================================================================================
//                                     **** SUBTRACTION ****
//=================================================================================================
//  Traits type that performs engine promotion type computations for binary addition.
//=================================================================================================
//
template<class ET1, class ET2>
struct matrix_engine_subtraction_traits
{
    using element_type_1 = typename ET1::element_type;
    using element_type_2 = typename ET2::element_type;
    using element_type   = matrix_element_promotion_t<element_type_1, element_type_2>;
    using engine_type    = dr_matrix_engine<element_type>;
};

//- Note that in the cases where allocators are rebound, there is an assumption that all
//  allocators are standard-conformant.
//
template<class T1, class A1, class T2, class A2>
struct matrix_engine_subtraction_traits<dr_matrix_engine<T1, A1>, dr_matrix_engine<T2, A2>>
{
    using element_type = matrix_element_promotion_t<T1, T2>;
    using alloc_type   = typename allocator_traits<A1>::template rebind_alloc<element_type>;
    using engine_type  = dr_matrix_engine<element_type, alloc_type>;
};

template<class T1, class A1, class T2, int32_t R2, int32_t C2>
struct matrix_engine_subtraction_traits<dr_matrix_engine<T1, A1>, fs_matrix_engine<T2, R2, C2>>
{
    using element_type = matrix_element_promotion_t<T1, T2>;
    using alloc_type   = typename allocator_traits<A1>::template rebind_alloc<element_type>;
    using engine_type  = dr_matrix_engine<element_type, alloc_type>;
};

template<class T1, int32_t R1, int32_t C1, class T2, class A2>
struct matrix_engine_subtraction_traits<fs_matrix_engine<T1, R1, C1>, dr_matrix_engine<T2, A2>>
{
    using element_type = matrix_element_promotion_t<T1, T2>;
    using alloc_type   = typename allocator_traits<A2>::template rebind_alloc<element_type>;
    using engine_type  = dr_matrix_engine<element_type, alloc_type>;
};

template<class T1, int32_t R1, int32_t C1, class T2, int32_t R2, int32_t C2>
struct matrix_engine_subtraction_traits<fs_matrix_engine<T1, R1, C1>, fs_matrix_engine<T2, R2, C2>>
{
    static_assert(R1 == R2);
    static_assert(C1 == C2);
    using element_type = matrix_element_promotion_t<T1, T2>;
    using engine_type  = fs_matrix_engine<element_type, R1, C2>;
};

//- Transpose cases.
//
template<class ET1, class ET2>
struct matrix_engine_subtraction_traits<matrix_transpose_engine<ET1>, ET2>
{
    using engine_type = typename matrix_engine_subtraction_traits<ET1, ET2>::engine_type;
};

template<class ET1, class ET2>
struct matrix_engine_subtraction_traits<ET1, matrix_transpose_engine<ET2>>
{
    using engine_type = typename matrix_engine_subtraction_traits<ET1, ET2>::engine_type;
};

template<class ET1, class ET2>
struct matrix_engine_subtraction_traits<matrix_transpose_engine<ET1>, matrix_transpose_engine<ET2>>
{
    using engine_type = typename matrix_engine_subtraction_traits<ET1, ET2>::engine_type;
};

template<class T1, int32_t R1, int32_t C1, class T2, int32_t R2, int32_t C2>
struct matrix_engine_subtraction_traits<matrix_transpose_engine<fs_matrix_engine<T1, R1, C1>>,
                                           fs_matrix_engine<T2, R2, C2>>
{
    static_assert(R1 == C2);
    static_assert(C1 == R2);
    using element_type = matrix_element_promotion_t<T1, T2>;
    using engine_type  = fs_matrix_engine<element_type, R2, C2>;
};

template<class T1, int32_t R1, int32_t C1, class T2, int32_t R2, int32_t C2>
struct matrix_engine_subtraction_traits<fs_matrix_engine<T1, R1, C1>,
                                           matrix_transpose_engine<fs_matrix_engine<T2, R2, C2>>>
{
    static_assert(R1 == C2);
    static_assert(C1 == R2);
    using element_type = matrix_element_promotion_t<T1, T2>;
    using engine_type  = fs_matrix_engine<element_type, R1, C1>;
};

template<class T1, int32_t R1, int32_t C1, class T2, int32_t R2, int32_t C2>
struct matrix_engine_subtraction_traits<matrix_transpose_engine<fs_matrix_engine<T1, R1, C1>>,
                                           matrix_transpose_engine<fs_matrix_engine<T2, R2, C2>>>
{
    static_assert(R1 == R2);
    static_assert(C1 == C2);
    using element_type = matrix_element_promotion_t<T1, T2>;
    using engine_type  = fs_matrix_engine<element_type, C1, R1>;
};

//- Alias interface to trait.
//
template<class ET1, class ET2>
using matrix_subtraction_engine_t = typename matrix_engine_subtraction_traits<ET1, ET2>::engine_type;


//=================================================================================================
//                                   **** MULTIPLICATION ****
//=================================================================================================
//  Traits type that performs engine promotion type computations for multiplication.
//=================================================================================================
//
template<class ET1, class ET2>
struct matrix_engine_multiplication_traits
{
    using element_type_1 = typename ET1::element_type;
    using element_type_2 = typename ET2::element_type;
    using element_type   = matrix_element_promotion_t<element_type_1, element_type_2>;
    using engine_type    = dr_matrix_engine<element_type>;
};

//- engine * scalar cases.
//
template<class T1, class A1, class T2>
struct matrix_engine_multiplication_traits<dr_matrix_engine<T1, A1>, T2>
{
    using element_type = matrix_element_promotion_t<T1, T2>;
    using alloc_type   = typename allocator_traits<A1>::template rebind_alloc<element_type>;
    using engine_type  = dr_matrix_engine<element_type, alloc_type>;
};

template<class T1, int32_t R1, int32_t C1, class T2>
struct matrix_engine_multiplication_traits<fs_matrix_engine<T1, R1, C1>, T2>
{
    using element_type = matrix_element_promotion_t<T1, T2>;
    using engine_type  = fs_matrix_engine<element_type, R1, C1>;
};


//- engine * engine cases.  Note that in the cases where allocators are rebound, there is an
//  assumption that all allocators are standard-conformant.
//
template<class T1, class A1, class T2, class A2>
struct matrix_engine_multiplication_traits<dr_matrix_engine<T1, A1>, dr_matrix_engine<T2, A2>>
{
    using element_type = matrix_element_promotion_t<T1, T2>;
    using alloc_type   = typename allocator_traits<A1>::template rebind_alloc<element_type>;
    using engine_type  = dr_matrix_engine<element_type, alloc_type>;
};

template<class T1, class A1, class T2, int32_t R2, int32_t C2>
struct matrix_engine_multiplication_traits<dr_matrix_engine<T1, A1>, fs_matrix_engine<T2, R2, C2>>
{
    using element_type = matrix_element_promotion_t<T1, T2>;
    using alloc_type   = typename allocator_traits<A1>::template rebind_alloc<element_type>;
    using engine_type  = dr_matrix_engine<element_type, alloc_type>;
};

template<class T1, int32_t R1, int32_t C1, class T2, class A2>
struct matrix_engine_multiplication_traits<fs_matrix_engine<T1, R1, C1>, dr_matrix_engine<T2, A2>>
{
    using element_type = matrix_element_promotion_t<T1, T2>;
    using alloc_type   = typename allocator_traits<A2>::template rebind_alloc<element_type>;
    using engine_type  = dr_matrix_engine<element_type, alloc_type>;
};

template<class T1, int32_t R1, int32_t C1, class T2, int32_t R2, int32_t C2>
struct matrix_engine_multiplication_traits<fs_matrix_engine<T1, R1, C1>, fs_matrix_engine<T2, R2, C2>>
{
    static_assert(C1 == R2);
    using element_type = matrix_element_promotion_t<T1, T2>;
    using engine_type  = fs_matrix_engine<element_type, R1, C2>;
};

//- Transpose cases.
//
template<class ET1, class ET2>
struct matrix_engine_multiplication_traits<matrix_transpose_engine<ET1>, ET2>
{
    using engine_type = typename matrix_engine_multiplication_traits<ET1, ET2>::engine_type;
};

template<class ET1, class ET2>
struct matrix_engine_multiplication_traits<ET1, matrix_transpose_engine<ET2>>
{
    using engine_type = typename matrix_engine_multiplication_traits<ET1, ET2>::engine_type;
};

template<class ET1, class ET2>
struct matrix_engine_multiplication_traits<matrix_transpose_engine<ET1>, matrix_transpose_engine<ET2>>
{
    using engine_type = typename matrix_engine_multiplication_traits<ET1, ET2>::engine_type;
};

template<class T1, int32_t R1, int32_t C1, class T2, int32_t R2, int32_t C2>
struct matrix_engine_multiplication_traits<matrix_transpose_engine<fs_matrix_engine<T1, R1, C1>>,
                                              fs_matrix_engine<T2, R2, C2>>
{
    static_assert(R1 == R2);
    using element_type = matrix_element_promotion_t<T1, T2>;
    using engine_type  = fs_matrix_engine<element_type, C1, C2>;
};

template<class T1, int32_t R1, int32_t C1, class T2, int32_t R2, int32_t C2>
struct matrix_engine_multiplication_traits<fs_matrix_engine<T1, R1, C1>,
                                              matrix_transpose_engine<fs_matrix_engine<T2, R2, C2>>>
{
    static_assert(C1 == C2);
    using element_type = matrix_element_promotion_t<T1, T2>;
    using engine_type  = fs_matrix_engine<element_type, R1, R2>;
};

template<class T1, int32_t R1, int32_t C1, class T2, int32_t R2, int32_t C2>
struct matrix_engine_multiplication_traits<matrix_transpose_engine<fs_matrix_engine<T1, R1, C1>>,
                                              matrix_transpose_engine<fs_matrix_engine<T2, R2, C2>>>
{
    static_assert(R1 == C2);
    using element_type = matrix_element_promotion_t<T1, T2>;
    using engine_type  = fs_matrix_engine<element_type, C1, R2>;
};

//- Alias interface to trait.
//
template<class ET1, class ET2>
using matrix_multiplication_engine_t = typename matrix_engine_multiplication_traits<ET1, ET2>::engine_type;

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_ENGINE_PROMOTION_TRAITS_HPP_DEFINED
