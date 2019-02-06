#ifndef MATRIX_OPERATION_TRAITS_HPP_DEFINED
#define MATRIX_OPERATION_TRAITS_HPP_DEFINED

#include "matrix.hpp"

namespace std::la {
//=================================================================================================
//                                      **** NEGATION ****
//=================================================================================================
//  Traits type that performs unary negations.
//=================================================================================================
//
template<class OP1>
struct matrix_negation_traits;

template<class E1>
struct matrix_negation_traits<column_vector<E1>>
{
    using engine_type = matrix_engine_negate_t<E1>;
    using result_type = column_vector<engine_type>;

    static result_type  negate(column_vector<E1> const& cv1);
};

template<class E1>
struct matrix_negation_traits<row_vector<E1>>
{
    using engine_type = matrix_engine_negate_t<E1>;
    using result_type = row_vector<engine_type>;

    static result_type  negate(row_vector<E1> const& rv1);
};

template<class E1>
struct matrix_negation_traits<matrix<E1>>
{
    using engine_type = matrix_engine_negate_t<E1>;
    using result_type = matrix<engine_type>;

    static result_type  negate(matrix<E1> const& m1);
};


//=================================================================================================
//                                      **** ADDITION ****
//=================================================================================================
//  Traits type that performs the additions.
//=================================================================================================
//
template<class OP1, class OP2>
struct matrix_addition_traits;

template<class E1, class E2>
struct matrix_addition_traits<column_vector<E1>, column_vector<E2>>
{
    using engine_type = matrix_engine_add_t<E1, E2>;
    using result_type = column_vector<engine_type>;

    static result_type  add(column_vector<E1> const& cv1, column_vector<E2> const& cv2);
};

template<class E1, class E2>
struct matrix_addition_traits<row_vector<E1>, row_vector<E2>>
{
    using engine_type = matrix_engine_add_t<E1, E2>;
    using result_type = row_vector<engine_type>;

    static result_type  add(row_vector<E1> const& rv1, row_vector<E2> const& rv2);
};

template<class E1, class E2>
struct matrix_addition_traits<matrix<E1>, matrix<E2>>
{
    using engine_type = matrix_engine_add_t<E1, E2>;
    using result_type = matrix<engine_type>;

    static result_type  add(matrix<E1> const& m1, matrix<E2> const& m2);
};


//=================================================================================================
//                                     **** SUBTRACTION ****
//=================================================================================================
//  Traits type that performs the subtractions.
//=================================================================================================
//
template<class OP1, class OP2>
struct matrix_subtraction_traits;

template<class E1, class E2>
struct matrix_subtraction_traits<column_vector<E1>, column_vector<E2>>
{
    using engine_type = matrix_engine_subtract_t<E1, E2>;
    using result_type = column_vector<engine_type>;

    static result_type  subtract(column_vector<E1> const& cv1, column_vector<E2> const& cv2);
};

template<class E1, class E2>
struct matrix_subtraction_traits<row_vector<E1>, row_vector<E2>>
{
    using engine_type = matrix_engine_subtract_t<E1, E2>;
    using result_type = row_vector<engine_type>;

    static result_type  subtract(row_vector<E1> const& rv1, row_vector<E2> const& rv2);
};

template<class E1, class E2>
struct matrix_subtraction_traits<matrix<E1>, matrix<E2>>
{
    using engine_type = matrix_engine_subtract_t<E1, E2>;
    using result_type = matrix<engine_type>;

    static result_type  subtract(matrix<E1> const& m1, matrix<E2> const& m2);
};


//=================================================================================================
//                                   **** MULTIPLICATION ****
//=================================================================================================
//  Traits type that performs the multiplications.
//=================================================================================================
//
template<class OP1, class OP2>
struct matrix_multiplication_traits;

//- vector*scalar
//
template<class E1, class T2>
struct matrix_multiplication_traits<column_vector<E1>, T2>
{
    using engine_type = matrix_engine_multiply_t<E1, T2>;
    using result_type = column_vector<engine_type>;

    static result_type  multiply(column_vector<E1> const& m, T2 s);
};

template<class E1, class T2>
struct matrix_multiplication_traits<row_vector<E1>, T2>
{
    using engine_type = matrix_engine_multiply_t<E1, T2>;
    using result_type = column_vector<engine_type>;

    static result_type  multiply(row_vector<E1> const& m, T2 s);
};


//- matrix*scalar
//
template<class E1, class T2>
struct matrix_multiplication_traits<matrix<E1>, T2>
{
    using engine_type = matrix_engine_multiply_t<E1, T2>;
    using result_type = column_vector<engine_type>;

    static result_type  multiply(matrix<E1> const& m, T2 s);
};


//- vector*vector
//
template<class E1, class E2>
struct matrix_multiplication_traits<row_vector<E1>, column_vector<E2>>
{
    using elem_type_1 = typename row_vector<E1>::element_type;
    using elem_type_2 = typename column_vector<E2>::element_type;
    using result_type = element_promotion_t<elem_type_1, elem_type_2>;

    static result_type  multiply(row_vector<E1> const& rv, column_vector<E1> const& cv);
};

template<class E1, class E2>
struct matrix_multiplication_traits<column_vector<E1>, row_vector<E2>>
{
    using engine_type = matrix_engine_multiply_t<E1, E2>;
    using result_type = matrix<engine_type>;

    static result_type  multiply(column_vector<E1> const& cv, row_vector<E1> const& rv);
};


//- matrix*vector
//
template<class E1, class E2>
struct matrix_multiplication_traits<matrix<E1>, column_vector<E2>>
{
    using engine_type = matrix_engine_multiply_t<E1, E2>;
    using result_type = column_vector<engine_type>;

    static result_type  multiply(matrix<E1> const& m, column_vector<E2> const& cv);
};

template<class E1, class E2>
struct matrix_multiplication_traits<matrix<E1>, row_vector<E2>>
{
    using engine_type = matrix_engine_multiply_t<E1, E2>;
    using result_type = matrix<engine_type>;

    static result_type  multiply(matrix<E1> const& m, row_vector<E2> const& cv);
};


//- vector*matrix
//
template<class E1, class E2>
struct matrix_multiplication_traits<column_vector<E1>, matrix<E2>>
{
    using engine_type = matrix_engine_multiply_t<E1, E2>;
    using result_type = matrix<engine_type>;

    static result_type  multiply(column_vector<E1> const& rv, matrix<E2> const& m);
};

template<class E1, class E2>
struct matrix_multiplication_traits<row_vector<E1>, matrix<E2>>
{
    using engine_type = matrix_engine_multiply_t<E1, E2>;
    using result_type = row_vector<engine_type>;

    static result_type  multiply(row_vector<E1> const& rv, matrix<E2> const& m);
};


//- matrix*matrix
//
template<class E1, class E2>
struct matrix_multiplication_traits<matrix<E1>, matrix<E2>>
{
    using engine_type = matrix_engine_multiply_t<E1, E2>;
    using result_type = matrix<engine_type>;

    static result_type  multiply(matrix<E1> const& m1, matrix<E2> const& m2);
};

}       //- std::la namespace
#endif  //- MATRIX_OPERATION_TRAITS_HPP_DEFINED
