//==================================================================================================
//  File:       operation_traits.hpp
//
//  Summary:    This header defines library's default operation traits type and a customization
//              point that is specializable by users.
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_OP_TRAITS_EXTRACTORS_HPP_DEFINED
#define LINEAR_ALGEBRA_OP_TRAITS_EXTRACTORS_HPP_DEFINED

namespace STD_LA {
namespace detail {

//==================================================================================================
//  EXTRACTION TRAITS DEFINITIONS
//==================================================================================================
//--------------------------------------------------------------------------------------------------
//  Traits:     {traits_name}_extractor<OT, U, V>
//  Aliases:    get_{traits_name}_t<OT, U, V>
//
//  These private traits types are used to extract element/engine/arithmetic traits types from
//  a containing operation traits type OT.  In each case, a straightforward application of the
//  type detection idiom is used to carry out the work.  A corresponding alias template is
//  defined for each extractor type.
//
//  Because the library requires a fairly large number of these, and their names vary only
//  by a small number of tokens, the macros STD_LA_DEFINE_OP_TRAITS_EXTRACTOR(OP, LVL)
//  and STD_LA_DEFINE_OP_TRAITS_EXTRACTORS(OP) are used to generate them.
//
//  For example, invoking STD_LA_DEFINE_OP_TRAITS_EXTRACTOR(addition, engine) will expand
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
//      STD_LA_DEFINE_OP_TRAITS_EXTRACTOR(addition, element);
//      STD_LA_DEFINE_OP_TRAITS_EXTRACTOR(addition, engine);
//      STD_LA_DEFINE_OP_TRAITS_EXTRACTOR(addition, arithmetic);
//
//  In turn, these macros generate the default addition_{element/engine/arithmetic}_traits
//  types, the addition_{element/engine/arithmetic}_traits_extractor traits types, and also
//  the corresponding get_addition_{element/engine/arithmetic}_traits_t alias templates.
//
//  The following set of extractor class templates is defined in the std::math::detail
//  namespace:
//
//      addition_element_traits_extractor<OT, U, V>
//      addition_engine_traits_extractor<OT, U, V>
//      addition_arithmetic_traits_extractor<OT, U, V>
//
//      subtraction_element_traits_extractor<OT, U, V>
//      subtraction_engine_traits_extractor<OT, U, V>
//      subtraction_arithmetic_traits_extractor<OT, U, V>
//
//      multiplication_element_traits_extractor<OT, U, V>
//      multiplication_engine_traits_extractor<OT, U, V>
//      multiplication_arithmetic_traits_extractor<OT, U, V>
//
//      division_element_traits_extractor<OT, U, V>
//      division_engine_traits_extractor<OT, U, V>
//      division_arithmetic_traits_extractor<OT, U, V>
//
//  The corresponding set of extractor alias templates is defined in the std::math
//  namespace:
//
//      get_addition_element_traits_t<OT, U, V>
//      get_addition_engine_traits_t<OT, U, V>
//      get_addition_arithmetic_traits_t<OT, U, V>
//
//      get_subtraction_element_traits_t<OT, U, V>
//      get_subtraction_engine_traits_t<OT, U, V>
//      get_subtraction_arithmetic_traits_t<OT, U, V>
//`
//      get_multiplication_element_traits_t<OT, U, V>
//      get_multiplication_engine_traits_t<OT, U, V>
//      get_multiplication_arithmetic_traits_t<OT, U, V>
//
//      get_division_element_traits_t<OT, U, V>
//      get_division_engine_traits_t<OT, U, V>
//      get_division_arithmetic_traits_t<OT, U, V>
//--------------------------------------------------------------------------------------------------
//
template<typename OT, typename U, typename V, typename = void>
struct addition_element_traits_extractor
{
    using type = matrix_operation_traits::addition_element_traits<OT,U,V>;
};

template<typename OT, typename U, typename V>
struct addition_element_traits_extractor<OT, U, V, void_t<typename OT::template addition_element_traits<OT,U,V>::element_type>>
{
    using type = typename OT::template addition_element_traits<OT, U, V>;
};

//------
//
template<typename OT, typename U, typename V, typename = void>
struct addition_layout_traits_extractor
{
    using type = matrix_operation_traits::addition_layout_traits<OT,U,V>;
};

template<typename OT, typename U, typename V>
struct addition_layout_traits_extractor<OT, U, V, void_t<typename OT::template addition_layout_traits<OT,U,V>::layout_type>>
{
    using type = typename OT::template addition_layout_traits<OT, U, V>;
};

//-----
//
template<typename OT, typename U, typename V, typename = void>
struct addition_engine_traits_extractor
{
    using type = matrix_operation_traits::addition_engine_traits<OT,U,V>;
};

template<typename OT, typename U, typename V>
struct addition_engine_traits_extractor<OT, U, V, void_t<typename OT::template addition_engine_traits<OT,U,V>::engine_type>>
{
    using type = typename OT::template addition_engine_traits<OT, U, V>;
};

//-----
//
template<typename OT, typename U, typename V, typename = void>
struct addition_arithmetic_traits_extractor
{
    using type = matrix_operation_traits::addition_arithmetic_traits<OT,U,V>;
};

template<typename OT, typename U, typename V>
struct addition_arithmetic_traits_extractor<OT, U, V, void_t<typename OT::template addition_arithmetic_traits<OT,U,V>::result_type>>
{
    using type = typename OT::template addition_arithmetic_traits<OT, U, V>;
};


#if 1
#define STD_LA_DEFINE_OP_TRAITS_EXTRACTOR(OP, LVL, RT)                                  \
    namespace detail {                                                                  \
        template<typename OT, typename U, typename V, typename = void>                  \
        struct OP##_##LVL##_traits_extractor                                            \
        {                                                                               \
            using type = matrix_operation_traits::OP##_##LVL##_traits<OT,U,V>;          \
        };                                                                              \
                                                                                        \
        template<typename OT, typename U, typename V>                                   \
        struct OP##_##LVL##_traits_extractor                                            \
        <OT, U, V, std::void_t<typename OT::template OP##_##LVL##_traits<OT,U,V>::RT>>  \
        {                                                                               \
            using type = typename OT::template OP##_##LVL##_traits<OT, U, V>;           \
        };                                                                              \
    }                                                                                   \
                                                                                        \
    template<typename OT, typename U, typename V>                                       \
    using get_##OP##_##LVL##_traits_t =                                                 \
        typename detail::OP##_##LVL##_traits_extractor<OT,U,V>::type;


#define STD_LA_DEFINE_OP_TRAITS_EXTRACTORS(OP)                      \
    STD_LA_DEFINE_OP_TRAITS_EXTRACTOR(OP, element, element_type)    \
    STD_LA_DEFINE_OP_TRAITS_EXTRACTOR(OP, engine, engine_type)      \
    STD_LA_DEFINE_OP_TRAITS_EXTRACTOR(OP, arithmetic, result_type)

#else

#define STD_LA_DEFINE_OP_TRAITS_EXTRACTORS(OP)                                                      \
    namespace detail {                                                                              \
        template<typename OT, typename U, typename V, typename = void>                              \
        struct OP##_element_traits_extractor                                                        \
        {                                                                                           \
            using type = matrix_operation_traits::OP##_element_traits<OT,U,V>;                      \
        };                                                                                          \
                                                                                                    \
        template<typename OT, typename U, typename V>                                               \
        struct OP##_element_traits_extractor                                                        \
        <OT, U, V, std::void_t<typename OT::template OP##_element_traits<OT,U,V>::element_type>>    \
        {                                                                                           \
            using type = typename OT::template OP##_element_traits<OT, U, V>;                       \
        };                                                                                          \
                                                                                                    \
        template<typename OT, typename U, typename V, typename = void>                              \
        struct OP##_engine_traits_extractor                                                         \
        {                                                                                           \
            using type = matrix_operation_traits::OP##_engine_traits<OT,U,V>;                       \
        };                                                                                          \
                                                                                                    \
        template<typename OT, typename U, typename V>                                               \
        struct OP##_engine_traits_extractor                                                         \
        <OT, U, V, std::void_t<typename OT::template OP##_engine_traits<OT,U,V>::engine_type>>      \
        {                                                                                           \
            using type = typename OT::template OP##_engine_traits<OT, U, V>;                        \
        };                                                                                          \
                                                                                                    \
        template<typename OT, typename U, typename V, typename = void>                              \
        struct OP##_arithmetic_traits_extractor                                                     \
        {                                                                                           \
            using type = matrix_operation_traits::OP##_arithmetic_traits<OT,U,V>;                   \
        };                                                                                          \
                                                                                                    \
        template<typename OT, typename U, typename V>                                               \
        struct OP##_arithmetic_traits_extractor                                                     \
        <OT, U, V, std::void_t<typename OT::template OP##_arithmetic_traits<OT,U,V>::result_type>>  \
        {                                                                                           \
            using type = typename OT::template OP##_arithmetic_traits<OT, U, V>;                    \
        };                                                                                          \
    }                                                                                               \
                                                                                                    \
    template<typename OT, typename U, typename V>                                                   \
    using get_##OP##_element_traits_t =                                                             \
        typename detail::OP##_element_traits_extractor<OT,U,V>::type;                               \
                                                                                                    \
    template<typename OT, typename U, typename V>                                                   \
    using get_##OP##_engine_traits_t =                                                              \
        typename detail::OP##_engine_traits_extractor<OT,U,V>::type;                                \
                                                                                                    \
    template<typename OT, typename U, typename V>                                                   \
    using get_##OP##_arithmetic_traits_t =                                                          \
        typename detail::OP##_arithmetic_traits_extractor<OT,U,V>::type;

#endif

STD_LA_DEFINE_OP_TRAITS_EXTRACTORS(addition)
STD_LA_DEFINE_OP_TRAITS_EXTRACTORS(subtraction)
STD_LA_DEFINE_OP_TRAITS_EXTRACTORS(multiplication)
STD_LA_DEFINE_OP_TRAITS_EXTRACTORS(division)

#undef STD_LA_DEFINE_OP_TRAITS_EXTRACTORS
#undef STD_LA_DEFINE_OP_TRAITS_EXTRACTOR

}       //- detail namespace
}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_OP_TRAITS_EXTRACTORS_HPP_DEFINED
