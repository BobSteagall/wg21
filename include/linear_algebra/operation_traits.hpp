//==================================================================================================
//  File:       operation_traits.hpp
//
//  Summary:    This header defines library's default operation traits type and a customization
//              point that is specializable by users.
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_OPERATION_TRAITS_HPP_DEFINED
#define LINEAR_ALGEBRA_OPERATION_TRAITS_HPP_DEFINED

namespace STD_LA {
namespace detail {
//--------------------------------------------------------------------------------------------------
//- Forward declarations of the default individual element/allocator/layout/engine/arithmetic
//  traits types referred to by matrix_operation_traits.  Forward declared in order to be
//  referenced in matrix_operation_traits below.
//--------------------------------------------------------------------------------------------------
//
template<class OT, class T1, class T2>      struct addition_element_traits;
template<class OT, class ET1, class ET2>    struct addition_engine_traits;
template<class OT, class OP1, class OP2>    struct addition_arithmetic_traits;

template<class OT, class T1, class T2>      struct subtraction_element_traits;
template<class OT, class ET1, class ET2>    struct subtraction_engine_traits;
template<class OT, class OP1, class OP2>    struct subtraction_arithmetic_traits;

template<class OT, class T1, class T2>      struct multiplication_element_traits;
template<class OT, class ET1, class ET2>    struct multiplication_engine_traits;
template<class OT, class OP1, class OP2>    struct multiplication_arithmetic_traits;

template<class OT, class T1, class T2>      struct division_element_traits;
template<class OT, class ET1, class ET2>    struct division_engine_traits;
template<class OT, class OP1, class OP2>    struct division_arithmetic_traits;

template<class OT, class AT1, class AT2, class T>   struct allocation_traits;
template<class OT, class LT1, class LT2>            struct addition_layout_traits;
template<class OT, class LT1, class LT2>            struct multiplication_layout_traits;


}       //- detail namespace
//==================================================================================================
//--------------------------------------------------------------------------------------------------
//  Class:  matrix_operation_traits
//
//  This class provides a set of nested type aliases which, in turn, provide for element
//  promotion, allocator promotion, layout promotion, engine promotion, and actual peform
//  computation for the four basic arithmetic operations (addition, subtraction, multiplication,
//  and scalar division).
//--------------------------------------------------------------------------------------------------
//
struct matrix_operation_traits
{
    //- Addition
    //
    template<class OTR, class T1, class T2>
    using addition_element_traits    = detail::addition_element_traits<OTR, T1, T2>;

    template<class OTR, class AT1, class AT2, class T>
    using addition_allocation_traits = detail::allocation_traits<OTR, AT1, AT2, T>;

    template<class OTR, class LT1, class LT2>
    using addition_layout_traits     = detail::addition_layout_traits<OTR, LT1, LT2>;

    template<class OTR, class ET1, class ET2>
    using addition_engine_traits     = detail::addition_engine_traits<OTR, ET1, ET2>;

    template<class OTR, class OP1, class OP2>
    using addition_arithmetic_traits = detail::addition_arithmetic_traits<OTR, OP1, OP2>;

    //- Subtraction
    //
    template<class OTR, class T1, class T2>
    using subtraction_element_traits    = detail::subtraction_element_traits<OTR, T1, T2>;

    template<class OTR, class AT1, class AT2, class T>
    using subtraction_allocation_traits = detail::allocation_traits<OTR, AT1, AT2, T>;

    template<class OTR, class LT1, class LT2>
    using subtraction_layout_traits     = detail::addition_layout_traits<OTR, LT1, LT2>;

    template<class OTR, class ET1, class ET2>
    using subtraction_engine_traits     = detail::subtraction_engine_traits<OTR, ET1, ET2>;

    template<class OTR, class OP1, class OP2>
    using subtraction_arithmetic_traits = detail::subtraction_arithmetic_traits<OTR, OP1, OP2>;

    //- Multiplication
    //
    template<class OTR, class T1, class T2>
    using multiplication_element_traits    = detail::multiplication_element_traits<OTR, T1, T2>;

    template<class OTR, class AT1, class AT2, class T>
    using multiplication_allocation_traits = detail::allocation_traits<OTR, AT1, AT2, T>;

    template<class OTR, class LT1, class LT2>
    using multiplication_layout_traits     = detail::multiplication_layout_traits<OTR, LT1, LT2>;

    template<class OTR, class ET1, class ET2>
    using multiplication_engine_traits     = detail::multiplication_engine_traits<OTR, ET1, ET2>;

    template<class OTR, class OP1, class OP2>
    using multiplication_arithmetic_traits = detail::multiplication_arithmetic_traits<OTR, OP1, OP2>;

    //- Division
    //
    template<class OTR, class T1, class T2>
    using division_element_traits    = detail::division_element_traits<OTR, T1, T2>;

    template<class OTR, class AT1, class AT2, class T>
    using division_allocation_traits = detail::allocation_traits<OTR, AT1, AT2, T>;

    template<class OTR, class LT1, class LT2>
    using division_layout_traits     = detail::addition_layout_traits<OTR, LT1, LT2>;

    template<class OTR, class T1, class T2>
    using division_engine_traits     = detail::division_engine_traits<OTR, T1, T2>;

    template<class OTR, class T1, class T2>
    using division_arithmetic_traits = detail::division_arithmetic_traits<OTR, T1, T2>;
};


//--------------------------------------------------------------------------------------------------
//  Class Template:     matrix_operation_traits_selector
//
//  This class template is a customization point, which can be specialized by users, that is
//  used by the arithmetic operators to select the operation traits type to be used in computing
//  that operator's result type.
//--------------------------------------------------------------------------------------------------
//
template<class COT1, class COT2>    struct matrix_operation_traits_selector;

template<>
struct matrix_operation_traits_selector<void, void>
{
    using traits_type = void;
};

template<>
struct matrix_operation_traits_selector<void, matrix_operation_traits>
{
    using traits_type = void;
};

template<class COT2>
struct matrix_operation_traits_selector<void, COT2>
{
    using traits_type = COT2;
};

//------
//
template<>
struct matrix_operation_traits_selector<matrix_operation_traits, void>
{
    using traits_type = void;
};

template<>
struct matrix_operation_traits_selector<matrix_operation_traits, matrix_operation_traits>
{
    using traits_type = void;
};

template<class COT2>
struct matrix_operation_traits_selector<matrix_operation_traits, COT2>
{
    using traits_type = COT2;
};

//------
//
template<class COT1>
struct matrix_operation_traits_selector<COT1, void>
{
    using traits_type = COT1;
};


template<class COT1>
struct matrix_operation_traits_selector<COT1, matrix_operation_traits>
{
    using traits_type = COT1;
};

//------
//
template<class COT1>
struct matrix_operation_traits_selector<COT1, COT1>
{
    using traits_type = COT1;
};


//- Alias template interface to trait.
//
template<class COT1, class COT2>
using select_matrix_operation_traits_t =
        typename matrix_operation_traits_selector<COT1,COT2>::traits_type;


//==================================================================================================
//  TRAITS DEFINITIONS
//==================================================================================================
//--------------------------------------------------------------------------------------------------
//  Traits:     {traits_name}_extractor<OT, U, V>
//  Aliases:    {traits_name}_t<OT, U, V>
//
//  These private traits types are used to extract element/engine/arithmetic traits types from
//  a containing operation traits type OT.  In each case, a straightforward application of the
//  type detection idiom is used to carry out the work.  A corresponding alias template is
//  defined for each extractor type.
//
//  Because the library requires a fairly large number of these, and their names vary only
//  by a small number of tokens, the macros STD_LA_DEFINE_OP_TRAITS_EXTRACTOR_2(OP, LVL),
//  STD_LA_DEFINE_OP_TRAITS_EXTRACTOR_3(OP, LVL), and STD_LA_DEFINE_OP_TRAITS_EXTRACTORS(OP)
//  are used to generate them.
//
//  For example, invoking STD_LA_DEFINE_OP_TRAITS_EXTRACTOR_2(addition, engine) will expand
//  to the following set of traits types:
//
//      template<typename OT, typename U, typename V, typename = void>
//      struct addition_engine_traits_extractor
//      {
//          using type = addition_engine_traits<OT, U, V>;
//      };
//
//      template<typename OT, typename U, typename V>
//      struct addition_engine_traits_extractor
//          <OT, U, V, std::void_t<typename OT::template addition_engine_traits<OT,U,V>>>
//      {
//          using type = typename OT::template addition_engine_traits<OT, U, V>;
//      };
//
//      template<typename OT, typename U, typename V>
//      using addition_engine_traits_t =
//          typename addition_engine_traits_extractor<OT, U, V>::type
//
//  Likewise, invoking STD_LA_DEFINE_OP_TRAITS_EXTRACTORS(addition) will expand to:
//
//      STD_LA_DEFINE_OP_TRAITS_EXTRACTOR_2(addition, element);
//      STD_LA_DEFINE_OP_TRAITS_EXTRACTOR_2(addition, layout);
//      STD_LA_DEFINE_OP_TRAITS_EXTRACTOR_3(addition, allocation);
//      STD_LA_DEFINE_OP_TRAITS_EXTRACTOR_2(addition, engine);
//      STD_LA_DEFINE_OP_TRAITS_EXTRACTOR_2(addition, arithmetic);
//
//  In turn, these macros generate the default addition_{element/engine/arithmetic}_traits
//  types, the addition_{element/engine/arithmetic}_traits_extractor traits types, and also
//  the corresponding get_addition_{element/engine/arithmetic}_traits_t alias templates.
//
//  The following set of default operation traits class templates is declared in the
//  std::math::detail namespace:
//
//      addition_element_traits<OT, U, V>
//      addition_allocation_traits<OT, U, V, W>
//      addition_layout_traits<OT, U, V>
//      addition_engine_traits<OT, U, V>
//      addition_arithmetic_traits<OT, U, V>
//
//      subtraction_element_traits<OT, U, V>
//      subtraction_allocation_traits<OT, U, V, W>
//      subtraction_layout_traits<OT, U, V>
//      subtraction_engine_traits<OT, U, V>
//      subtraction_arithmetic_traits<OT, U, V>
//
//      multiplication_element_traits<OT, U, V>
//      multiplication_allocation_traits<OT, U, V, W>
//      multiplication_layout_traits<OT, U, V>
//      multiplication_engine_traits<OT, U, V>
//      multiplication_arithmetic_traits<OT, U, V>
//
//      division_element_traits<OT, U, V>
//      division_allocation_traits<OT, U, V, W>
//      division_layout_traits<OT, U, V>
//      division_engine_traits<OT, U, V>
//      division_arithmetic_traits<OT, U, V>
//
//  The following set of extractor class templates is defined in the std::math::detail
//  namespace:
//
//      addition_element_traits_extractor<OT, U, V>
//      addition_allocation_traits_extractor<OT, U, V, W>
//      addition_layout_traits_extractor<OT, U, V>
//      addition_engine_traits_extractor<OT, U, V>
//      addition_arithmetic_traits_extractor<OT, U, V>
//
//      subtraction_element_traits_extractor<OT, U, V>
//      subtraction_allocation_traits_extractor<OT, U, V, W>
//      subtraction_layout_traits_extractor<OT, U, V>
//      subtraction_engine_traits_extractor<OT, U, V>
//      subtraction_arithmetic_traits_extractor<OT, U, V>
//
//      multiplication_element_traits_extractor<OT, U, V>
//      multiplication_allocation_traits_extractor<OT, U, V, W>
//      multiplication_layout_traits_extractor<OT, U, V>
//      multiplication_engine_traits_extractor<OT, U, V>
//      multiplication_arithmetic_traits_extractor<OT, U, V>
//
//      division_element_traits_extractor<OT, U, V>
//      division_allocation_traits_extractor<OT, U, V, W>
//      division_layout_traits_extractor<OT, U, V>
//      division_engine_traits_extractor<OT, U, V>
//      division_arithmetic_traits_extractor<OT, U, V>
//
//  The corresponding set of extractor alias templates is defined in the std::math
//  namespace:
//
//      get_addition_element_traits_t<OT, U, V>
//      get_addition_allocation_traits_t<OT, U, V, W>
//      get_addition_layout_traits_t<OT, U, V>
//      get_addition_engine_traits_t<OT, U, V>
//      get_addition_arithmetic_traits_t<OT, U, V>
//
//      get_subtraction_element_traits_t<OT, U, V>
//      get_subtraction_allocation_traits_t<OT, U, V, W>
//      get_subtraction_layout_traits_t<OT, U, V>
//      get_subtraction_engine_traits_t<OT, U, V>
//      get_subtraction_arithmetic_traits_t<OT, U, V>
//`
//      get_multiplication_element_traits_t<OT, U, V>
//      get_multiplication_allocation_traits_t<OT, U, V, W>
//      get_multiplication_layout_traits_t<OT, U, V>
//      get_multiplication_engine_traits_t<OT, U, V>
//      get_multiplication_arithmetic_traits_t<OT, U, V>
//
//      get_division_element_traits_t<OT, U, V>
//      get_division_allocation_traits_t<OT, U, V, W>
//      get_division_layout_traits_t<OT, U, V>
//      get_division_engine_traits_t<OT, U, V>
//      get_division_arithmetic_traits_t<OT, U, V>
//--------------------------------------------------------------------------------------------------
//
#define STD_LA_DEFINE_OP_TRAITS_EXTRACTOR_2(OP, LVL)                                \
    namespace detail {                                                              \
        template<typename OT, typename U, typename V> struct OP##_##LVL##_traits;   \
                                                                                    \
        template<typename OT, typename U, typename V, typename = void>              \
        struct OP##_##LVL##_traits_extractor                                        \
        {                                                                           \
            using type = matrix_operation_traits::OP##_##LVL##_traits<OT,U,V>;      \
        };                                                                          \
                                                                                    \
        template<typename OT, typename U, typename V>                               \
        struct OP##_##LVL##_traits_extractor                                        \
        <OT, U, V, std::void_t<typename OT::template OP##_##LVL##_traits<OT,U,V>>>  \
        {                                                                           \
            using type = typename OT::template OP##_##LVL##_traits<OT, U, V>;       \
        };                                                                          \
    }                                                                               \
                                                                                    \
    template<typename OT, typename U, typename V>                                   \
    using get_##OP##_##LVL##_traits_t =                                             \
        typename detail::OP##_##LVL##_traits_extractor<OT,U,V>::type


#define STD_LA_DEFINE_OP_TRAITS_EXTRACTOR_3(OP, LVL)                                \
    namespace detail {                                                              \
        template<typename OT, typename U, typename V, typename W>                   \
        class OP##_##LVL##_traits;                                                  \
                                                                                    \
        template<typename OT, typename U, typename V, typename W, typename = void>  \
        struct OP##_##LVL##_traits_extractor                                        \
        {                                                                           \
            using type = matrix_operation_traits::OP##_##LVL##_traits<OT,U,V,W>;    \
        };                                                                          \
                                                                                    \
        template<typename OT, typename U, typename V, typename W>                   \
        struct OP##_##LVL##_traits_extractor                                        \
        <OT,U,V,W,std::void_t<typename OT::template OP##_##LVL##_traits<OT,U,V,W>>> \
        {                                                                           \
            using type = typename OT::template OP##_##LVL##_traits<OT, U, V, W>;    \
        };                                                                          \
    }                                                                               \
                                                                                    \
    template<typename OT, typename U, typename V, typename W>                       \
    using get_##OP##_##LVL##_traits_t =                                             \
        typename detail::OP##_##LVL##_traits_extractor<OT, U, V, W>::type


#define STD_LA_DEFINE_OP_TRAITS_EXTRACTORS(OP)              \
    STD_LA_DEFINE_OP_TRAITS_EXTRACTOR_2(OP, element);       \
    STD_LA_DEFINE_OP_TRAITS_EXTRACTOR_2(OP, layout);        \
    STD_LA_DEFINE_OP_TRAITS_EXTRACTOR_3(OP, allocation);    \
    STD_LA_DEFINE_OP_TRAITS_EXTRACTOR_2(OP, engine);        \
    STD_LA_DEFINE_OP_TRAITS_EXTRACTOR_2(OP, arithmetic)

STD_LA_DEFINE_OP_TRAITS_EXTRACTORS(addition);
//STD_LA_DEFINE_OP_TRAITS_EXTRACTORS(subtraction);
//STD_LA_DEFINE_OP_TRAITS_EXTRACTORS(multiplication);
//STD_LA_DEFINE_OP_TRAITS_EXTRACTORS(division);

#undef STD_LA_DEFINE_OP_TRAITS_EXTRACTORS
#undef STD_LA_DEFINE_OP_TRAITS_EXTRACTOR_3
#undef STD_LA_DEFINE_OP_TRAITS_EXTRACTOR_2


namespace detail {
//--------------------------------------------------------------------------------------------------
//  Trait:      is_ttp_helper<T>
//
//  This private is used by the has_nested_{*} concepts defined below to match a template template
//  parameter when the concept is evaluated.
//--------------------------------------------------------------------------------------------------
//
template<template<typename...> typename>
struct is_ttp_helper : public std::true_type
{};


//==================================================================================================
//  CONCEPT DEFINITIONS
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
//  The idea here is simple: we need to ensure that operation traits type OT either: supplies
//  a nested template "addition_engine_traits" that fulfills certain requirements; or, has no
//  nested type or type alias named "addition_engine_traits".  Specifically, OT must
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

//template<class OT, class LT1, class LT2>    struct addition_layout_traits;


template<class OT, class LT1, class LT2>    struct multiplication_layout_traits;

template<class OT>
struct multiplication_layout_traits<OT, matrix_layout::row_major, matrix_layout::row_major>
{
    using layout_type = matrix_layout::row_major;
};

template<class OT>
struct multiplication_layout_traits<OT, matrix_layout::row_major, matrix_layout::column_major>
{
    using layout_type = matrix_layout::column_major;
};

template<class OT>
struct multiplication_layout_traits<OT, matrix_layout::column_major, matrix_layout::row_major>
{
    using layout_type = matrix_layout::row_major;
};

template<class OT>
struct multiplication_layout_traits<OT, matrix_layout::column_major, matrix_layout::column_major>
{
    using layout_type = matrix_layout::column_major;
};


template<class OT, class AT1, class AT2, class T>  struct allocation_traits;

template<class OT, class T>
struct allocation_traits<OT, void, void, T>
{
    using allocator_type = void;
};

template<class OT, class V, class T>
struct allocation_traits<OT, void, std::allocator<V>, T>
{
    using traits_type    = std::allocator_traits<std::allocator<V>>;
    using allocator_type = typename traits_type::template rebind_alloc<T>;
};

template<class OT, class U, class T>
struct allocation_traits<OT, std::allocator<U>, void, T>
{
    using traits_type    = std::allocator_traits<std::allocator<U>>;
    using allocator_type = typename traits_type::template rebind_alloc<T>;
};

template<class OT, class A1, class V, class T>
struct allocation_traits<OT, A1, std::allocator<V>, T>
{
    using traits_type    = std::allocator_traits<std::allocator<V>>;
    using allocator_type = typename traits_type::template rebind_alloc<T>;
};

template<class OT, class U, class A2, class T>
struct allocation_traits<OT, std::allocator<U>, A2, T>
{
    using traits_type    = std::allocator_traits<std::allocator<U>>;
    using allocator_type = typename traits_type::template rebind_alloc<T>;
};

template<class OT, class U, class V, class T>
struct allocation_traits<OT, std::allocator<U>, std::allocator<V>, T>
{
    using traits_type    = std::allocator_traits<std::allocator<U>>;
    using allocator_type = typename traits_type::template rebind_alloc<T>;
};


template<class ET>
struct engine_extents_helper
{
  private:
    template<class Lambda, int = (Lambda{}(), 0)>
    static constexpr ptrdiff_t
    get_value_helper(Lambda)
    {
        return Lambda{}();
    }

    static constexpr ptrdiff_t
    get_value_helper(...)
    {
        return dynamic_extent;
    }

  public:
    static constexpr ptrdiff_t
    columns()
    requires
        readable_matrix_engine<ET>
    {
        return get_value_helper([]{ return static_cast<ptrdiff_t>(ET().columns()); });
    }

    static constexpr ptrdiff_t
    rows()
    requires
        readable_matrix_engine<ET>
    {
        return get_value_helper([]{ return static_cast<ptrdiff_t>(ET().rows()); });
    }

    static constexpr ptrdiff_t
    size()
    requires
        readable_vector_engine<ET>
    {
        return get_value_helper([]{ return static_cast<ptrdiff_t>(ET().size()); });
    }
};

template<class T, ptrdiff_t N, class A, class L>
struct engine_extents_helper<matrix_storage_engine<T, extents<N>, A, L>>
{
    static constexpr ptrdiff_t
    size()
    {
        return N;
    }
};

template<class T, ptrdiff_t R, ptrdiff_t C, class A, class L>
struct engine_extents_helper<matrix_storage_engine<T, extents<R, C>, A, L>>
{
    static constexpr ptrdiff_t
    columns()
    {
        return C;
    }

    static constexpr ptrdiff_t
    rows()
    {
        return R;
    }
};

template<class ET>
struct engine_layout_helper
{
    using layout_type = matrix_layout::row_major;
};

template<class T, ptrdiff_t N, class A, class L>
struct engine_layout_helper<matrix_storage_engine<T, extents<N>, A, L>>
{
    using layout_type = void;
};

template<class T, ptrdiff_t R, ptrdiff_t C, class A, class L>
struct engine_layout_helper<matrix_storage_engine<T, extents<R, C>, A, L>>
{
    using layout_type = L;
};

//--------------------------------------------------------------------------------------------------
//- Some useful alias templates used to support the various traits types.
//
template<class T, ptrdiff_t N>
using fixed_vector_engine = matrix_storage_engine<T, extents<N>, void, void>;

template<class T, ptrdiff_t N>
using sized_vector_engine = matrix_storage_engine<T, extents<N>, std::allocator<T>, void>;

template<class T>
using dynamic_vector_engine =
        matrix_storage_engine<T, extents<dynamic_extent>, std::allocator<T>, void>;


template<class T, ptrdiff_t R, ptrdiff_t C, class LT = matrix_layout::row_major>
using fixed_matrix_engine = matrix_storage_engine<T, extents<R, C>, void, LT>;

template<class T, ptrdiff_t R, ptrdiff_t C, class LT = matrix_layout::row_major>
using sized_matrix_engine = matrix_storage_engine<T, extents<R, C>, std::allocator<T>, LT>;

template<class T, class LT = matrix_layout::row_major>
using dynamic_matrix_engine =
        matrix_storage_engine<T, extents<dynamic_extent, dynamic_extent>, std::allocator<T>, LT>;

}       //- detail namespace
}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_OPERATION_TRAITS_HPP_DEFINED
