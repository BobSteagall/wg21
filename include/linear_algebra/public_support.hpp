//==================================================================================================
//  File:       public_suport.hpp
//
//  Summary:    This header defines a small number of general-purpose facilities that are part
//              of the proposal's public interface.  These facilities are provide to assist
//              users wishing to create customized arithmetic operations.
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_PUBLIC_SUPPORT_HPP_DEFINED
#define LINEAR_ALGEBRA_PUBLIC_SUPPORT_HPP_DEFINED

namespace STD_LA {
//- This trivial class template is used to represent the presence of a scalar operand in an
//  expression.  It is part of the public interface so that users wishing to create customized
//  arithmetic operation traits can easily detect when an operand is a scalar.
//
template<class T>
struct scalar_engine
{
    using engine_category = scalar_engine_tag;
};


//- These variable templates are used to determine the kind of engine represented by a category
//  tag: scalar, vector, or matrix.
//
template<class TT> inline constexpr
bool    is_scalar_engine_tag_v = TT::value == scalar_engine_tag::value;

template<class TT> inline constexpr
bool    is_vector_engine_tag_v = TT::value >= readable_vector_engine_tag::value &&
                               TT::value <= resizable_vector_engine_tag::value;

template<class TT> inline constexpr
bool    is_matrix_engine_tag_v = TT::value >= readable_matrix_engine_tag::value &&
                               TT::value <= resizable_matrix_engine_tag::value;


//- These variable templates are used to determine the kind of engine based on the engine's
//  type: scalar, vector, or matrix.
//
template<class ET> inline constexpr
bool    is_scalar_engine_v = detail::is_scalar_engine_v<ET>;

template<class ET> inline constexpr
bool    is_vector_engine_v = detail::is_vector_engine_v<ET>;

template<typename ET> inline constexpr
bool    is_matrix_engine_v = detail::is_matrix_engine_v<ET>;


//- These variable templates are used to determine the attributes of an engine based on that
//  engine's category: readable, writable, initable, resizable.
//
template<class ET> inline constexpr
bool    is_readable_engine_v = detail::is_readable_engine_v<ET>;

template<class ET> inline constexpr
bool    is_writable_engine_v = detail::is_writable_engine_v<ET>;

template<class ET> inline constexpr
bool    is_initable_engine_v = detail::is_initable_engine_v<ET>;

template<class ET> inline constexpr
bool    is_resizable_engine_v = detail::is_resizable_engine_v<ET>;


//- This variable templates is used to determine whether or not two engine types are similar:
//  i.e., both types represent the same kind of engine
//
template<class ET1, class ET2> inline constexpr
bool    engines_are_similar_v = detail::engines_match_v<ET1, ET2>;


}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_PUBLIC_SUPPORT_HPP_DEFINED
