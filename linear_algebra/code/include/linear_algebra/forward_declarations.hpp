//==================================================================================================
//  File:       forward_declarations.hpp
//
//  Summary:    This header contains forward declarations of the library's public API.  These
//              declarations are made in order to support the nice linear order of header
//              inclusion found in the driver header (linear_algebra.hpp).
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_FORWARD_DECLARATIONS_HPP_DEFINED
#define LINEAR_ALGEBRA_FORWARD_DECLARATIONS_HPP_DEFINED

namespace STD_LA {
USING_STD

//- Tags that specify how engines behave.
//
struct scalar_engine_tag           : public integral_constant<int, 0> {};

struct readable_vector_engine_tag  : public integral_constant<int, 1> {};
struct writable_vector_engine_tag  : public integral_constant<int, 2> {};
struct initable_vector_engine_tag  : public integral_constant<int, 3> {};
struct resizable_vector_engine_tag : public integral_constant<int, 5> {};

struct readable_matrix_engine_tag  : public integral_constant<int, 7> {};
struct writable_matrix_engine_tag  : public integral_constant<int, 11> {};
struct initable_matrix_engine_tag  : public integral_constant<int, 13> {};
struct resizable_matrix_engine_tag : public integral_constant<int, 17> {};


//- Owning engines with dynamically-allocated external storage.
//
template<class T, class AT>     class dr_vector_engine;
template<class T, class AT>     class dr_matrix_engine;

//- Owning engines with fixed-size internal storage.
//
template<class T, size_t N>             class fs_vector_engine;
template<class T, size_t R, size_t C>   class fs_matrix_engine;

//- Non-owning, view-style engines (NOEs).
//
template<class ET, class VCT>   class column_engine;
template<class ET, class VCT>   class row_engine;
template<class ET, class MCT>   class transpose_engine;
template<class ET, class MCT>   class submatrix_engine;

template<class T>   struct scalar_engine;

//- The default element promotion, engine promotion, and arithmetic operation traits for
//  the four basic arithmetic operations.
//
struct matrix_operation_traits;

//- TODO: remove this
//
struct default_matrix_operations {};

//- Primary math object types.
//
template<class ET, class OT=matrix_operation_traits> class vector;
template<class ET, class OT=matrix_operation_traits> class matrix;

//- Math object element promotion traits, per arithmetical operation.
//
template<class T1>              struct matrix_negation_element_traits;
template<class T1, class T2>    struct matrix_addition_element_traits;
template<class T1, class T2>    struct matrix_subtraction_element_traits;
template<class T1, class T2>    struct matrix_multiplication_element_traits;

//- Math object engine promotion traits, per arithmetical operation.
//
template<class OT, class ET1>               struct matrix_negation_engine_traits;
template<class OT, class ET1, class ET2>    struct matrix_addition_engine_traits;
template<class OT, class ET1, class ET2>    struct matrix_subtraction_engine_traits;
template<class OT, class ET1, class ET2>    struct matrix_multiplication_engine_traits;

//- Math object arithmetic traits.
//
template<class OT, class OP1>               struct matrix_negation_traits;
template<class OT, class OP1, class OP2>    struct matrix_addition_traits;
template<class OT, class OP1, class OP2>    struct matrix_subtraction_traits;
template<class OT, class OP1, class OP2>    struct matrix_multiplication_traits;

//- A traits type that chooses between two operation traits types in the binary arithmetic
//  operators and free functions that act like binary operators (e.g., outer_product()).
//  Note that this traits class is a customization point.
//
template<class T1, class T2>    struct matrix_operation_traits_selector;

//- Addition operators
//
template<class ET1, class OT1, class ET2, class OT2>
auto  operator +(vector<ET1, OT1> const& v1, vector<ET2, OT2> const& v2);

template<class ET1, class OT1, class ET2, class OT2>
auto  operator +(matrix<ET1, OT1> const& m1, matrix<ET2, OT2> const& m2);

//- Subtraction operators
//
template<class ET1, class OT1, class ET2, class OT2>
auto  operator -(vector<ET1, OT1> const& v1, vector<ET2, OT2> const& v2);

template<class ET1, class OT1, class ET2, class OT2>
auto  operator -(matrix<ET1, OT1> const& m1, matrix<ET2, OT2> const& m2);

//- Negation operators
//
template<class ET1, class OT1>
auto  operator -(vector<ET1, OT1> const& v1);

template<class ET1, class OT1, class ET2, class OT2>
auto  operator -(matrix<ET1, OT1> const& m1);

//- Vector*Scalar multiplication operators
//
template<class ET1, class OT1, class S2>
auto  operator *(vector<ET1, OT1> const& v1, S2 const& s2);

template<class S1, class ET2, class OT2>
auto    operator *(S1 const& s1, vector<ET2, OT2> const& v2);

//- Matrix*Scalar multiplication operators
//
template<class ET1, class OT1, class S2>
auto  operator *(matrix<ET1, OT1> const& m1, S2 const& s2);

template<class S1, class ET2, class OT2>
auto  operator *(S1 const& s1, matrix<ET2, OT2> const& m2);

//- Vector*Matrix multiplication operators
//
template<class ET1, class OT1, class ET2, class OT2>
auto  operator *(vector<ET1, OT1> const& v1, matrix<ET2, OT2> const& m2);

//- Matrix*Vector multiplication operators
//
template<class ET1, class OT1, class ET2, class OT2>
auto  operator *(matrix<ET1, OT1> const& m1, vector<ET2, OT2> const& v2);

//- Vector*Vector multiplication operators
//
template<class ET1, class OT1, class ET2, class OT2>
auto  operator *(vector<ET1, OT1> const& v1, vector<ET2, OT2> const& v2);

//- Matrix*Matrix multiplication operators
//
template<class ET1, class OT1, class ET2, class OT2>
auto  operator *(matrix<ET1, OT1> const& m1, matrix<ET2, OT2> const& m2);

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_FORWARD_DECLARATIONS_HPP_DEFINED
