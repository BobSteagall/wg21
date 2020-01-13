//==================================================================================================
//  File:       public_suport.hpp
//
//  Summary:    This header defines a small number of types and functions for 
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_PUBLIC_SUPPORT_HPP_DEFINED
#define LINEAR_ALGEBRA_PUBLIC_SUPPORT_HPP_DEFINED

namespace STD_LA {

template<class VTT> inline constexpr
bool    is_scalar_engine_tag = VTT::value == scalar_engine_tag::value;

template<class VTT> inline constexpr
bool    is_vector_engine_tag = VTT::value >= readable_vector_engine_tag::value &&
                               VTT::value <= resizable_vector_engine_tag::value;

template<class VTT> inline constexpr
bool    is_matrix_engine_tag = VTT::value >= readable_matrix_engine_tag::value &&
                               VTT::value <= resizable_matrix_engine_tag::value;


template<class ET> inline constexpr 
bool    is_scalar_engine_v = detail::is_scalar_v<ET>;

template<class ET> inline constexpr 
bool    is_vector_engine_v = detail::is_vector_v<ET>;

template<typename ET> inline constexpr 
bool    is_matrix_engine_v = detail::is_matrix_v<ET>;

template<class ET> inline constexpr 
bool    is_readable_engine_v = detail::is_readable_v<ET>;

template<class ET> inline constexpr 
bool    is_writable_engine_v = detail::is_writable_v<ET>;

template<class ET> inline constexpr 
bool    is_resizable_engine_v = detail::is_resizable_v<ET>;


template<class ET, class OT> constexpr 
bool
result_requires_resize(vector<ET, OT> const&)
{
	return is_resizable_engine_v<ET>;
}

template<class ET, class OT> constexpr 
bool
result_requires_resize(matrix<ET, OT> const&)
{
	return is_resizable_engine_v<ET>;
}


template<class T>
struct scalar_engine
{
    using engine_category = scalar_engine_tag;
};

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_PUBLIC_SUPPORT_HPP_DEFINED
