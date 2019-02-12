#ifndef MATRIX_ARITHMETIC_TRAITS_HPP_DEFINED
#define MATRIX_ARITHMETIC_TRAITS_HPP_DEFINED

#include "matrix.hpp"

namespace STD_LA {
//=================================================================================================
//                                      **** NEGATION ****
//=================================================================================================
//  Traits type that performs negation.
//=================================================================================================
//
template<class OP1, class OTR>
struct matrix_negation_traits;

template<class ET1, class OT1, class OTR>
struct matrix_negation_traits<column_vector<ET1, OT1>, OTR>
{
    using engine_type = matrix_negation_engine_t<ET1>;
    using op_traits   = OTR;
    using result_type = column_vector<engine_type, op_traits>;

    static result_type  negate(column_vector<ET1, OT1> const& cv1);
};

template<class ET1, class OT1, class OTR>
struct matrix_negation_traits<row_vector<ET1, OT1>, OTR>
{
    using engine_type = matrix_negation_engine_t<ET1>;
    using op_traits   = OTR;
    using result_type = row_vector<engine_type, op_traits>;

    static result_type  negate(row_vector<ET1, OT1> const& rv1);
};

template<class ET1, class OT1, class OTR>
struct matrix_negation_traits<matrix<ET1, OT1>, OTR>
{
    using engine_type = matrix_negation_engine_t<ET1>;
    using op_traits   = OTR;
    using result_type = matrix<engine_type, op_traits>;

    static result_type  negate(matrix<ET1, OT1> const& m1);
};


//=================================================================================================
//                                      **** ADDITION ****
//=================================================================================================
//  Traits type that performs addition.
//=================================================================================================
//
template<class OP1, class OP2, class OTR>
struct matrix_addition_traits;

template<class ET1, class OT1, class ET2, class OT2, class OTR>
struct matrix_addition_traits<column_vector<ET1, OT1>, column_vector<ET2, OT2>, OTR>
{
    using engine_type = matrix_addition_engine_t<ET1, ET2>;
    using op_traits   = OTR;
    using result_type = column_vector<engine_type, op_traits>;

    static result_type  add(column_vector<ET1, OT1> const& cv1, column_vector<ET2, OT2> const& cv2);
};

template<class ET1, class OT1, class ET2, class OT2, class OTR>
struct matrix_addition_traits<row_vector<ET1, OT1>, row_vector<ET2, OT2>, OTR>
{
    using engine_type = matrix_addition_engine_t<ET1, ET2>;
    using op_traits   = OTR;
    using result_type = row_vector<engine_type, op_traits>;

    static result_type  add(row_vector<ET1, OT1> const& rv1, row_vector<ET2, OT2> const& rv2);
};

template<class ET1, class OT1, class ET2, class OT2, class OTR>
struct matrix_addition_traits<matrix<ET1, OT1>, matrix<ET2, OT2>, OTR>
{
    using engine_type = matrix_addition_engine_t<ET1, ET2>;
    using op_traits   = OTR;
    using result_type = matrix<engine_type, op_traits>;

    static result_type  add(matrix<ET1, OT1> const& m1, matrix<ET2, OT2> const& m2);
};


//=================================================================================================
//                                     **** SUBTRACTION ****
//=================================================================================================
//  Traits type that performs subtraction.
//=================================================================================================
//
template<class OP1, class OP2, class OTR>
struct matrix_subtraction_traits;

template<class ET1, class OT1, class ET2, class OT2, class OTR>
struct matrix_subtraction_traits<column_vector<ET1, OT1>, column_vector<ET2, OT2>, OTR>
{
    using engine_type = matrix_subtraction_engine_t<ET1, ET2>;
    using op_traits   = OTR;
    using result_type = column_vector<engine_type, op_traits>;

    static result_type  subtract(column_vector<ET1, OT1> const& cv1, column_vector<ET2, OT2> const& cv2);
};

template<class ET1, class OT1, class ET2, class OT2, class OTR>
struct matrix_subtraction_traits<row_vector<ET1, OT1>, row_vector<ET2, OT2>, OTR>
{
    using engine_type = matrix_subtraction_engine_t<ET1, ET2>;
    using op_traits   = OTR;
    using result_type = row_vector<engine_type, op_traits>;

    static result_type  subtract(row_vector<ET1, OT1> const& rv1, row_vector<ET2, OT2> const& rv2);
};

template<class ET1, class OT1, class ET2, class OT2, class OTR>
struct matrix_subtraction_traits<matrix<ET1, OT1>, matrix<ET2, OT2>, OTR>
{
    using engine_type = matrix_subtraction_engine_t<ET1, ET2>;
    using op_traits   = OTR;
    using result_type = matrix<engine_type, op_traits>;

    static result_type  subtract(matrix<ET1, OT1> const& m1, matrix<ET2, OT2> const& m2);
};


//=================================================================================================
//                                   **** MULTIPLICATION ****
//=================================================================================================
//  Traits type that performs multiplication.
//=================================================================================================
//
template<class OP1, class OP2, class OTR>
struct matrix_multiplication_traits;

//- vector*scalar
//
template<class ET1, class OT1, class T2, class OTR>
struct matrix_multiplication_traits<column_vector<ET1, OT1>, T2, OTR>
{
    using engine_type = matrix_multiplication_engine_t<ET1, T2>;
    using op_traits   = OTR;
    using result_type = column_vector<engine_type, op_traits>;

    static result_type  multiply(column_vector<ET1, OT1> const& cv1, T2 s2);
};

template<class ET1, class OT1, class T2, class OTR>
struct matrix_multiplication_traits<row_vector<ET1, OT1>, T2, OTR>
{
    using engine_type = matrix_multiplication_engine_t<ET1, T2>;
    using op_traits   = OTR;
    using result_type = row_vector<engine_type, op_traits>;

    static result_type  multiply(row_vector<ET1, OT1> const& rv1, T2 s2);
};


//- scalar*vector
//
template<class T1, class ET2, class OT2, class OTR>
struct matrix_multiplication_traits<T1, column_vector<ET2, OT2>, OTR>
{
    using engine_type = matrix_multiplication_engine_t<ET2, T1>;
    using op_traits   = OTR;
    using result_type = column_vector<engine_type>;

    static result_type  multiply(T1 s1, column_vector<ET2, OT2> const& cv2);
};

template<class T1, class ET2, class OT2, class OTR>
struct matrix_multiplication_traits<T1, row_vector<ET2, OT2>, OTR>
{
    using engine_type = matrix_multiplication_engine_t<ET2, T1>;
    using op_traits   = OTR;
    using result_type = row_vector<engine_type, op_traits>;

    static result_type  multiply(T1 s1, row_vector<ET2, OT2> const& rv2);
};


//- matrix*scalar
//
template<class ET1, class OT1, class T2, class OTR>
struct matrix_multiplication_traits<matrix<ET1, OT1>, T2, OTR>
{
    using engine_type = matrix_multiplication_engine_t<ET1, T2>;
    using op_traits   = OTR;
    using result_type = matrix<engine_type, op_traits>;

    static result_type  multiply(matrix<ET1, OT1> const& m1, T2 s2);
};


//- scalar*matrix
//
template<class T1, class ET2, class OT2, class OTR>
struct matrix_multiplication_traits<T1, matrix<ET2, OT2>, OTR>
{
    using engine_type = matrix_multiplication_engine_t<T1, ET2>;
    using op_traits   = OTR;
    using result_type = matrix<engine_type, op_traits>;

    static result_type  multiply(T1 s1, matrix<ET2, OT2> const& m2);
};


//- vector*vector
//
template<class ET1, class OT1, class ET2, class OT2, class OTR>
struct matrix_multiplication_traits<row_vector<ET1, OT1>, column_vector<ET2, OT2>, OTR>
{
    using elem_type_1 = typename row_vector<ET1, OT1>::element_type;
    using elem_type_2 = typename column_vector<ET2, OT2>::element_type;
    using engine_type = void;
    using result_type = matrix_element_promotion_t<elem_type_1, elem_type_2>;

    static result_type  multiply(row_vector<ET1, OT1> const& rv1, column_vector<ET1, OT1> const& cv2);
};

template<class ET1, class OT1, class ET2, class OT2, class OTR>
struct matrix_multiplication_traits<column_vector<ET1, OT1>, row_vector<ET2, OT2>, OTR>
{
    using engine_type = matrix_multiplication_engine_t<ET1, ET2>;
    using op_traits   = OTR;
    using result_type = matrix<engine_type, op_traits>;

    static result_type  multiply(column_vector<ET1, OT1> const& cv1, row_vector<ET1, OT1> const& rv2);
};


//- matrix*vector
//
template<class ET1, class OT1, class ET2, class OT2, class OTR>
struct matrix_multiplication_traits<matrix<ET1, OT1>, column_vector<ET2, OT2>, OTR>
{
    using engine_type = matrix_multiplication_engine_t<ET1, ET2>;
    using op_traits   = OTR;
    using result_type = column_vector<engine_type, op_traits>;

    static result_type  multiply(matrix<ET1, OT1> const& m1, column_vector<ET2, OT2> const& cv2);
};

template<class ET1, class OT1, class ET2, class OT2, class OTR>
struct matrix_multiplication_traits<matrix<ET1, OT1>, row_vector<ET2, OT2>, OTR>
{
    using engine_type = matrix_multiplication_engine_t<ET1, ET2>;
    using op_traits   = OTR;
    using result_type = matrix<engine_type, op_traits>;

    static result_type  multiply(matrix<ET1, OT1> const& m1, row_vector<ET2, OT2> const& cv2);
};


//- vector*matrix
//
template<class ET1, class OT1, class ET2, class OT2, class OTR>
struct matrix_multiplication_traits<column_vector<ET1, OT1>, matrix<ET2, OT2>, OTR>
{
    using engine_type = matrix_multiplication_engine_t<ET1, ET2>;
    using op_traits   = OTR;
    using result_type = matrix<engine_type, op_traits>;

    static result_type  multiply(column_vector<ET1, OT1> const& rv1, matrix<ET2, OT2> const& m2);
};

template<class ET1, class OT1, class ET2, class OT2, class OTR>
struct matrix_multiplication_traits<row_vector<ET1, OT1>, matrix<ET2, OT2>, OTR>
{
    using engine_type = matrix_multiplication_engine_t<ET1, ET2>;
    using op_traits   = OTR;
    using result_type = row_vector<engine_type, op_traits>;

    static result_type  multiply(row_vector<ET1, OT1> const& rv1, matrix<ET2, OT2> const& m2);
};


//- matrix*matrix
//
template<class ET1, class OT1, class ET2, class OT2, class OTR>
struct matrix_multiplication_traits<matrix<ET1, OT1>, matrix<ET2, OT2>, OTR>
{
    using engine_type = matrix_multiplication_engine_t<ET1, ET2>;
    using op_traits   = OTR;
    using result_type = matrix<engine_type, op_traits>;

    static result_type  multiply(matrix<ET1, OT1> const& m1, matrix<ET2, OT2> const& m2);
};

}       //- STD_LA namespace
#endif  //- MATRIX_ARITHMETIC_TRAITS_HPP_DEFINED
