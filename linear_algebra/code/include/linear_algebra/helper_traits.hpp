#ifndef LINEAR_ALGEBRA_HELPER_TRAITS_HPP_DEFINED
#define LINEAR_ALGEBRA_HELPER_TRAITS_HPP_DEFINED

#include "engine_traits.hpp"

namespace STD_LA {

template<class ET1, class ET2>
using enable_if_resizable_t =
typename std::enable_if_t<std::is_same_v<ET1, ET2> && ET1::is_resizable::value, bool>;

template<typename T, T>
struct detection_helper {};

template<class CT, class RT, class... ARGS>
using has_resize_member = decltype(detection_helper<RT (CT::*)(ARGS...), &CT::resize>{});

template<class CT, class ST>
using has_rx_2 = has_resize_member<dr_matrix_engine<CT>, void, ST, ST>;

template<class CT, class ST>
using has_rx_4 = has_resize_member<dr_matrix_engine<CT>, void, ST, ST, ST, ST>;

/*
template<typename CT, typename ST, typename = void>
struct has_resize_x : std::false_type {};

template<typename CT, typename ST>
struct has_resize_x<CT, std::void_t<decltype(detection_helper<void (CT::*)(ST,ST), &CT::resize>())>>
:   std::true_type {};
*/

template<class ET1, class ET2>
using enable_if_engine_is_mutable =
typename std::enable_if_t<std::is_same_v<ET1, ET2> && (ET1::engine_category::value >= 2), bool>;

template<class ET1, class ET2>
using enable_if_mutable_test =
typename std::enable_if_t<std::is_same_v<ET1, ET2> && (ET1::engine_category::value >= 2), std::true_type>;


template<class RT, class O1>
void
PrintOperandTypes(std::string const& loc, O1 const& o1)
{
    cout << "in " << loc << endl
         << "  op1: " << get_type_name(o1) << endl
         << "  ret: " << get_type_name<RT>() << endl << endl;
}

template<class RT, class O1, class O2>
void
PrintOperandTypes(std::string const& loc, O1 const& o1, O2 const& o2)
{
    cout << "in " << loc << endl
         << "  op1: " << get_type_name(o1) << endl
         << "  op2: " << get_type_name(o2) << endl
         << "  ret: " << get_type_name<RT>() << endl << endl;
}

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_HELPER_TRAITS_HPP_DEFINED
