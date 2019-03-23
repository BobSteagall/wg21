#ifndef LINEAR_ALGEBRA_HELPER_TRAITS_HPP_DEFINED
#define LINEAR_ALGEBRA_HELPER_TRAITS_HPP_DEFINED

#include "engine_promotion_traits.hpp"

namespace STD_LA {

template<class ET1, class ET2>
using enable_if_resizable_t =
typename std::enable_if_t<std::is_same_v<ET1, ET2>&& ET1::is_resizable::value, bool>;

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
