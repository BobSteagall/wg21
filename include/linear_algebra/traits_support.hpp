//==================================================================================================
//  File:       traits_support.hpp
//
//  Summary:    This header defines library's default operation traits type and a customization
//              point that is specializable by users.
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_TRAITS_SUPPORT_HPP_DEFINED
#define LINEAR_ALGEBRA_TRAITS_SUPPORT_HPP_DEFINED

namespace STD_LA {
namespace detail {

template<class T, size_t R, size_t C, class A, class L>
struct engine_extents_helper<matrix_storage_engine<T, R, C, A, L>>
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


//--------------------------------------------------------------------------------------------------
//  Trait:      is_ttp_helper<T>
//
//  This private traits type is used by the has_nested_{*} concepts defined below to match a
//  template template parameter when that concept is evaluated.
//--------------------------------------------------------------------------------------------------
//
template<template<typename...> typename>
struct is_ttp_helper : public std::true_type
{};

//==================================================================================================
//  TRAITS ATTRIBUTE CONCEPT DEFINITIONS
//==================================================================================================
//  Concepts:   has_valid_{traits_name}<OT, U, V>
//              has_nested_{traits_name}<OT>
//              has_no_nested_{traits_name}<OT>
//              valid_{traits_name}<OT, U, V>
//
//  These private concepts determine whether an operation traits type OT contains a specifically-
//  named nested class template or alias template which itself takes a specific set of template
//  parameters and returns a nested type.
//
//  Because the library requires a number of these, and their names vary only by a small number
//  of tokens, the macro STD_LA_DEFINE_OP_TRAITS_CHECKER(NAME, RESULT_TYPE) is used to generate
//  them.
//
//  For example, invoking this macro with arguments "addition_engine_traits" and "engine_type"
//  would expand to the following:
//
//      template<typename OT, typename U, typename V>
//      concept has_valid_addition_engine_traits =
//          requires
//          {
//              typename OT::template addition_engine_traits<OT, U, V>::engine_type;
//          };
//
//      template<typename OT>
//      concept has_nested_addition_engine_traits =
//          requires
//          {
//              typename OT::addition_engine_traits;
//          }
//          or requires
//          {
//              typename is_ttp_helper<OT::template addition_engine_traits>::value_type;
//          };
//
//      template<typename OT>
//      concept has_no_nested_addition_engine_traits = not has_nested_addition_engine_traits<OT>;
//
//      template<typename OT, typename U, typename V>
//      concept valid_addition_engine_traits =
//          class_type<OT>
//          and
//          (has_valid_addition_engine_traits<OT,U,V> or has_no_nested_addition_engine_traits<OT>);
//
//  The idea here is simple: we need to ensure that a given operation traits type OT either:
//    a. supplies a nested template "addition_engine_traits" that fulfills certain requirements;
//       OR
//    b. has no nested type or type alias named "addition_engine_traits".
//
//  Additionally, in this example OT must:
//      1.  be a non-union class type;
//          AND
//      2a. have a nested class/alias template named "addition_engine_traits" that is
//          instantiable with the three provided template arguments, and when instantiated,
//          contains a public nested type alias called "engine_type";
//          OR
//      2b. have no nested type or alias called "addition_engine_traits".
//--------------------------------------------------------------------------------------------------
//
#define STD_LA_DEFINE_OP_TRAITS_CHECKER(NAME, RESULT_TYPE)                  \
                                                                            \
    template<typename OT, typename U, typename V>                           \
    concept has_valid_##NAME =                                              \
        requires                                                            \
        {                                                                   \
            typename OT::template NAME<OT, U, V>::RESULT_TYPE;              \
        };                                                                  \
                                                                            \
    template<typename OT>                                                   \
    concept has_nested_##NAME =                                             \
        requires                                                            \
        {                                                                   \
            typename OT::NAME;                                              \
        }                                                                   \
        or requires                                                         \
        {                                                                   \
            typename is_ttp_helper<OT::template NAME>::value_type;          \
        };                                                                  \
                                                                            \
    template<typename OT>                                                   \
    concept has_no_nested_##NAME = not has_nested_##NAME<OT>;               \
                                                                            \
    template<typename OT, typename U, typename V>                           \
    concept valid_##NAME =                                                  \
        class_type<OT>                                                      \
        and                                                                 \
        (has_valid_##NAME<OT, U, V> or has_no_nested_##NAME<OT>)


//--------------------------------------------------------------------------------------------------
//  Concepts:   valid_{operation}_traits<OT, U, V>
//
//  This private concept verifies that operation traits type OT has an acceptable traits
//  interface for a given arithmetic operation (addition, subtraction, multiplication,
//  and division).
//
//  Because the library requires a number of these, and their names vary only by a small number
//  of tokens, the macro STD_LA_DEFINE_VALID_OP_TRAITS_DETECTORS(OP) is used to generate them.
//  For example, invoking this macro with the argument "addition" would expand to the following:
//
//      STD_LA_DEFINE_OP_TRAITS_CHECKER(addition_element_traits, element_type);
//      STD_LA_DEFINE_OP_TRAITS_CHECKER(addition_engine_traits, engine_type);
//      STD_LA_DEFINE_OP_TRAITS_CHECKER(addition_arithmetic_traits, result_type);
//
//      template<typename OT, typename U, typename V>
//      concept valid_addition_traits =
//          valid_addition_element_traits<OT, U, V>
//          and
//          valid_addition_engine_traits<OT, U, V>
//          and
//          valid_addition_arithmetic_traits<OT, U, V>;
//
//  The idea is provide a way for an arithmetic operator to validate the set of element, engine,
//  and arithmetic traits that it will use to carry out its mission.  Assuming the validation
//  succeeds, the remaining template metaprogramming becomes much simpler.
//--------------------------------------------------------------------------------------------------
//
#define STD_LA_DEFINE_VALID_OP_TRAITS_DETECTORS(OP)                         \
                                                                            \
    STD_LA_DEFINE_OP_TRAITS_CHECKER(OP##_element_traits, element_type);     \
    STD_LA_DEFINE_OP_TRAITS_CHECKER(OP##_engine_traits, engine_type);       \
    STD_LA_DEFINE_OP_TRAITS_CHECKER(OP##_arithmetic_traits, result_type);   \
                                                                            \
    template<typename OT, typename U, typename V>                           \
    concept valid_##OP##_traits =                                           \
        valid_##OP##_element_traits<OT, U, V>                               \
        and                                                                 \
        valid_##OP##_engine_traits<OT, U, V>                                \
        and                                                                 \
        valid_##OP##_arithmetic_traits<OT, U, V>


STD_LA_DEFINE_VALID_OP_TRAITS_DETECTORS(addition);
STD_LA_DEFINE_VALID_OP_TRAITS_DETECTORS(subtraction);
STD_LA_DEFINE_VALID_OP_TRAITS_DETECTORS(multiplication);
STD_LA_DEFINE_VALID_OP_TRAITS_DETECTORS(division);

#undef STD_LA_DEFINE_VALID_OP_TRAITS_DETECTORS
#undef STD_LA_DEFINE_OP_TRAITS_CHECKER

//--------------------------------------------------------------------------------------------------
//- Some useful alias templates used to support the various traits types.
//
template<class T, size_t R, size_t C, class LT = matrix_layout::row_major>
using fixed_matrix_engine = matrix_storage_engine<T, R, C, void, LT>;

template<class T, size_t R, size_t C, class LT = matrix_layout::row_major>
using sized_matrix_engine = matrix_storage_engine<T, R, C, std::allocator<T>, LT>;

template<class T, class lt = matrix_layout::row_major>
using dynamic_matrix_engine =
                    matrix_storage_engine<T, dynamic_extent, dynamic_extent, std::allocator<T>, lt>;

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
//    c. If both allocator types are std::allocator, the result is allocator<T>.
//--------------------------------------------------------------------------------------------------
//
template<class AT1, class AT2, class T>
struct mse_allocation_traits
{
    using allocator_type = std::allocator<T>;
};

template<class T>
struct mse_allocation_traits<void, void, T>
{
    using allocator_type = void;
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

template<class AT1, class T>
requires
    (not detail::is_specialization_of_v<AT1, std::allocator>)
struct mse_allocation_traits<AT1, AT1, T>
{
    using traits_type    = std::allocator_traits<AT1>;
    using allocator_type = typename traits_type::template rebind_alloc<T>;
};

//------
//
template<class ET1, class ET2, bool DX, size_t RR, size_t CR, class T>
struct engine_allocation_traits
{
//    static constexpr bool   dyn_size = (DX  ||  (RR*CR) > 64);
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


//==================================================================================================
//                              **** LAYOUT TRAITS ****
//==================================================================================================
//- The standard layout addition traits type provides the default mechanism for determining the
//  resulting data layout when adding two matrices having different layouts..
//
template<class LT1, class LT2>
struct addsubdiv_layout_traits
{
    using layout_type = matrix_layout::row_major;
};

template<>
struct addsubdiv_layout_traits<matrix_layout::column_major, matrix_layout::column_major>
{
    using layout_type = matrix_layout::column_major;
};

//------
//
template<class LT1, class LT2>
struct multiplication_layout_traits
{
    using layout_type = matrix_layout::row_major;
};

template<>
struct multiplication_layout_traits<matrix_layout::row_major, matrix_layout::column_major>
{
    using layout_type = matrix_layout::column_major;
};

template<>
struct multiplication_layout_traits<matrix_layout::column_major, matrix_layout::column_major>
{
    using layout_type = matrix_layout::column_major;
};

//-------
//
template<class ET1, class ET2, bool DX>
struct engine_layout_traits
{
    using layout_type = matrix_layout::row_major;
};

template<class T1, size_t R1, size_t C1, class AT1, class LT1,
         class T2, size_t R2, size_t C2, class AT2, class LT2>
struct engine_layout_traits<matrix_storage_engine<T1, R1, C1, AT1, LT1>,
                            matrix_storage_engine<T2, R2, C2, AT2, LT2>,
                            true>
{
    using layout_type = typename multiplication_layout_traits<LT1, LT2>::layout_type;
};


template<class T1, size_t R1, size_t C1, class AT1, class LT1,
         class T2, size_t R2, size_t C2, class AT2, class LT2>
struct engine_layout_traits<matrix_storage_engine<T1, R1, C1, AT1, LT1>,
                            matrix_storage_engine<T2, R2, C2, AT2, LT2>,
                            false>
{
    using layout_type = typename addsubdiv_layout_traits<LT1, LT2>::layout_type;
};

}       //- detail namespace
}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_TRAITS_SUPPORT_HPP_DEFINED
