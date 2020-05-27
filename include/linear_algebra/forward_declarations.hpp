//==================================================================================================
//  File:       forward_declarations.hpp
//
//  Summary:    This header contains forward declarations of the library's public API.  These
//              declarations are made to support the nice linear order of header inclusion
//              found in the driver header (linear_algebra.hpp).
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_FORWARD_DECLARATIONS_HPP_DEFINED
#define LINEAR_ALGEBRA_FORWARD_DECLARATIONS_HPP_DEFINED

namespace STD_LA {
USING_STD

//- Engine category tag types, used to specify the interfaces expressed by engine types.
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

//- A trivial engine to represent a scalar operand.
//
template<class T>   struct scalar_engine;

//- Owning engines with dynamically-allocated external storage.
//
template<class T, class AT = allocator<T>>  class dr_vector_engine;
template<class T, class AT = allocator<T>>  class dr_matrix_engine;

//- Owning engines with fixed-size internal storage.
//
template<class T, ptrdiff_t N>              class fs_vector_engine;
template<class T, ptrdiff_t R, ptrdiff_t C> class fs_matrix_engine;

//- Non-owning, view-style engines (NOEs).
//
template<class ET, class VCT, class VFT>   class vector_view_engine;
template<class ET, class MCT, class VFT>   class matrix_view_engine;

//- Non-owning view engine functionality tag types, used to specify the functionality of a view.
//
struct column_view_tag {};
struct row_view_tag {};
struct subset_view_tag {};
struct negation_view_tag {};
struct transpose_view_tag {};
struct hermitian_view_tag {};

//- These are some convenience aliases, to make it easier for user-created operation traits.
//
template<class ET>
using vector_negation_engine = vector_view_engine<ET, readable_vector_engine_tag, negation_view_tag>;

template<class ET, class VCT>
using vector_subset_engine = vector_view_engine<ET, VCT, subset_view_tag>;


template<class ET, class VCT>
using matrix_column_engine = vector_view_engine<ET, VCT, column_view_tag>;

template<class ET, class VCT>
using matrix_row_engine = vector_view_engine<ET, VCT, row_view_tag>;

template<class ET>
using matrix_negation_engine = matrix_view_engine<ET, readable_matrix_engine_tag, negation_view_tag>;

template<class ET, class MCT>
using matrix_hermitian_engine = matrix_view_engine<ET, readable_matrix_engine_tag, hermitian_view_tag>;

template<class ET, class MCT>
using matrix_subset_engine = matrix_view_engine<ET, MCT, subset_view_tag>;

template<class ET, class MCT>
using matrix_transpose_engine = matrix_view_engine<ET, MCT, transpose_view_tag>;

//- The default element promotion, engine promotion, and arithmetic operation traits for
//  the four basic arithmetic operations.
//
struct matrix_operation_traits;

//- Primary math object types.
//
template<class ET, class OT=matrix_operation_traits> class vector;
template<class ET, class OT=matrix_operation_traits> class matrix;

//- Aliases for vector and matrix objects based on dynamic engines.
//
template<class T, class A = allocator<T>>
using dyn_vector = vector<dr_vector_engine<T, A>>;

template<class T, class A = allocator<T>>
using dyn_matrix = matrix<dr_matrix_engine<T, A>>;

//- Aliases for vector and matrix objects based on fixed-size engines.
//
template<class T, ptrdiff_t N>
using fs_vector = vector<fs_vector_engine<T, N>>;

template<class T, ptrdiff_t R, ptrdiff_t C>
using fs_matrix = matrix<fs_matrix_engine<T, R, C>>;

#ifndef LA_NEGATION_AS_VIEW
//- Traits pertaining to negation.
//
template<class T1>              struct matrix_negation_element_traits;
template<class OT, class ET1>   struct matrix_negation_engine_traits;
template<class OT, class OP1>   struct matrix_negation_arithmetic_traits;
#endif

//- Traits pertaining to addition.
//
template<class T1, class T2>                struct matrix_addition_element_traits;
template<class OT, class ET1, class ET2>    struct matrix_addition_engine_traits;
template<class OT, class OP1, class OP2>    struct matrix_addition_arithmetic_traits;

//- Traits pertaining to subtraction.
//
template<class T1, class T2>                struct matrix_subtraction_element_traits;
template<class OT, class ET1, class ET2>    struct matrix_subtraction_engine_traits;
template<class OT, class OP1, class OP2>    struct matrix_subtraction_arithmetic_traits;

//- Traits pertaining to multiplication.
//
template<class T1, class T2>                struct matrix_multiplication_element_traits;
template<class OT, class ET1, class ET2>    struct matrix_multiplication_engine_traits;
template<class OT, class OP1, class OP2>    struct matrix_multiplication_arithmetic_traits;

//- Traits pertaining to scalar division.
//
template<class T1, class T2>                struct matrix_division_element_traits;
template<class OT, class ET1, class ET2>    struct matrix_division_engine_traits;
template<class OT, class OP1, class OP2>    struct matrix_division_arithmetic_traits;

//- A traits type that chooses between two operation traits types in the binary arithmetic
//  operators and free functions that act like binary operators (e.g., outer_product()).
//  Note that this traits class is a customization point.
//
template<class T1, class T2>    struct matrix_operation_traits_selector;

#ifndef LA_NEGATION_AS_VIEW
//- Negation operators
//
template<class ET1, class OT1>
constexpr auto  operator -(vector<ET1, OT1> const& v1);

template<class ET1, class OT1, class ET2, class OT2>
constexpr auto  operator -(matrix<ET1, OT1> const& m1);

#endif
//- Addition operators
//
template<class ET1, class OT1, class ET2, class OT2>
constexpr auto  operator +(vector<ET1, OT1> const& v1, vector<ET2, OT2> const& v2);

template<class ET1, class OT1, class ET2, class OT2>
constexpr auto  operator +(matrix<ET1, OT1> const& m1, matrix<ET2, OT2> const& m2);

//- Subtraction operators
//
template<class ET1, class OT1, class ET2, class OT2>
constexpr auto  operator -(vector<ET1, OT1> const& v1, vector<ET2, OT2> const& v2);

template<class ET1, class OT1, class ET2, class OT2>
constexpr auto  operator -(matrix<ET1, OT1> const& m1, matrix<ET2, OT2> const& m2);

//- Multiplication operators
//
template<class ET1, class OT1, class S2>
constexpr auto  operator *(vector<ET1, OT1> const& v1, S2 const& s2);

template<class S1, class ET2, class OT2>
constexpr auto    operator *(S1 const& s1, vector<ET2, OT2> const& v2);

template<class ET1, class OT1, class S2>
constexpr auto  operator *(matrix<ET1, OT1> const& m1, S2 const& s2);

template<class S1, class ET2, class OT2>
constexpr auto  operator *(S1 const& s1, matrix<ET2, OT2> const& m2);

template<class ET1, class OT1, class ET2, class OT2>
constexpr auto  operator *(vector<ET1, OT1> const& v1, matrix<ET2, OT2> const& m2);

template<class ET1, class OT1, class ET2, class OT2>
constexpr auto  operator *(matrix<ET1, OT1> const& m1, vector<ET2, OT2> const& v2);

template<class ET1, class OT1, class ET2, class OT2>
constexpr auto  operator *(vector<ET1, OT1> const& v1, vector<ET2, OT2> const& v2);

template<class ET1, class OT1, class ET2, class OT2>
constexpr auto  operator *(matrix<ET1, OT1> const& m1, matrix<ET2, OT2> const& m2);

//- Scalar division operators
//
template<class ET1, class OT1, class S2>
constexpr auto  operator /(vector<ET1, OT1> const& v1, S2 const& s2);

template<class ET1, class OT1, class S2>
constexpr auto  operator /(matrix<ET1, OT1> const& m1, S2 const& s2);

template<class ET1, class OT1, class ET2, class OT2>
auto  operator /(vector<ET1, OT1> const& v1, vector<ET2, OT2> const& v2) = delete;

template<class ET1, class OT1, class ET2, class OT2>
auto  operator /(vector<ET1, OT1> const& v1, matrix<ET2, OT2> const& v2) = delete;

template<class ET1, class OT1, class ET2, class OT2>
auto  operator /(matrix<ET1, OT1> const& v1, vector<ET2, OT2> const& v2) = delete;

template<class ET1, class OT1, class ET2, class OT2>
auto  operator /(matrix<ET1, OT1> const& v1, matrix<ET2, OT2> const& v2) = delete;

//- Other functions.
//
template<class ET1, class OT1, class ET2, class OT2>
constexpr auto  inner_product(vector<ET1, OT1> const& v1, vector<ET2, OT2> const& v2);

template<class ET1, class OT1, class ET2, class OT2>
constexpr auto  outer_product(vector<ET1, OT1> const& v1, vector<ET2, OT2> const& v2);

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_FORWARD_DECLARATIONS_HPP_DEFINED
