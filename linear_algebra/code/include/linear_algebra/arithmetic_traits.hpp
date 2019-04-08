#ifndef MATRIX_ARITHMETIC_TRAITS_HPP_DEFINED
#define MATRIX_ARITHMETIC_TRAITS_HPP_DEFINED

namespace STD_LA {
//=================================================================================================
//                                      **** NEGATION ****
//=================================================================================================
//  Traits type that performs negation.
//=================================================================================================
//
template<class OP1, class OTR>
struct matrix_negation_traits;

//------
//
template<class ET1, class OT1, class OTR>
struct matrix_negation_traits<vector<ET1, OT1>, OTR>
{
    using engine_type = matrix_negation_engine_t<ET1>;
    using op_traits   = OTR;
    using result_type = vector<engine_type, op_traits>;

    static result_type  negate(vector<ET1, OT1> const& v1);
};

template<class ET1, class OT1, class OTR> inline
typename matrix_negation_traits<vector<ET1, OT1>, OTR>::result_type
matrix_negation_traits<vector<ET1, OT1>, OTR>::negate(vector<ET1,OT1> const& v1)
{
    PrintOperandTypes<result_type>("negation_traits", v1);
    return result_type();
}

//------
//
template<class ET1, class OT1, class OTR>
struct matrix_negation_traits<matrix<ET1, OT1>, OTR>
{
    using engine_type = matrix_negation_engine_t<ET1>;
    using op_traits   = OTR;
    using result_type = matrix<engine_type, op_traits>;

    static result_type  negate(matrix<ET1, OT1> const& m1);
};

template<class ET1, class OT1, class OTR> inline
typename matrix_negation_traits<matrix<ET1, OT1>, OTR>::result_type
matrix_negation_traits<matrix<ET1, OT1>, OTR>::negate(matrix<ET1,OT1> const& m1)
{
    PrintOperandTypes<result_type>("negation_traits", m1);
    return result_type();
}

//=================================================================================================
//                                      **** ADDITION ****
//=================================================================================================
//  Traits type that performs addition.
//=================================================================================================
//
template<class OP1, class OP2>
struct matrix_addition_traits;

//------
//
template<class ET1, class OT1, class ET2, class OT2>
struct matrix_addition_traits<vector<ET1, OT1>, vector<ET2, OT2>>
{
    using engine_type = matrix_addition_engine_t<ET1, ET2>;
    using op_traits   = OT1;
    using result_type = vector<engine_type, op_traits>;

    static result_type  add(vector<ET1, OT1> const& v1, vector<ET2, OT2> const& v2);
};

template<class ET1, class OT1, class ET2, class OT2>
auto
matrix_addition_traits<vector<ET1, OT1>, vector<ET2, OT2>>::add
(vector<ET1, OT1> const& v1, vector<ET2, OT2> const& v2) -> result_type
{
    PrintOperandTypes<result_type>("addition_traits", v1, v2);
    return result_type();
}

//------
//
template<class ET1, class OT1, class ET2, class OT2>
struct matrix_addition_traits<matrix<ET1, OT1>, matrix<ET2, OT2>>
{
    using engine_type = matrix_addition_engine_t<ET1, ET2>;
    using op_traits   = OT1;
    using result_type = matrix<engine_type, op_traits>;

    static result_type  add(matrix<ET1, OT1> const& m1, matrix<ET2, OT2> const& m2);
};

template<class ET1, class OT1, class ET2, class OT2>
typename matrix_addition_traits<matrix<ET1, OT1>, matrix<ET2, OT2>>::result_type
matrix_addition_traits<matrix<ET1, OT1>, matrix<ET2, OT2>>::add
(matrix<ET1, OT1> const& m1, matrix<ET2, OT2> const& m2)
{
    PrintOperandTypes<result_type>("addition_traits", m1, m2);
    return result_type();
}

//=================================================================================================
//                                     **** SUBTRACTION ****
//=================================================================================================
//  Traits type that performs subtraction.
//=================================================================================================
//
template<class OP1, class OP2, class OTR>
struct matrix_subtraction_traits;

//------
//
template<class ET1, class OT1, class ET2, class OT2, class OTR>
struct matrix_subtraction_traits<vector<ET1, OT1>, vector<ET2, OT2>, OTR>
{
    using engine_type = matrix_subtraction_engine_t<ET1, ET2>;
    using op_traits   = OTR;
    using result_type = vector<engine_type, op_traits>;

    static result_type  subtract(vector<ET1, OT1> const& v1, vector<ET2, OT2> const& v2);
};

template<class ET1, class OT1, class ET2, class OT2, class OTR>
typename matrix_subtraction_traits<vector<ET1, OT1>, vector<ET2, OT2>, OTR>::result_type
matrix_subtraction_traits<vector<ET1, OT1>, vector<ET2, OT2>, OTR>::subtract
(vector<ET1, OT1> const& v1, vector<ET2, OT2> const& v2)
{
    PrintOperandTypes<result_type>("subtraction_traits", v1, v2);
    return result_type();
}

//------
//
template<class ET1, class OT1, class ET2, class OT2, class OTR>
struct matrix_subtraction_traits<matrix<ET1, OT1>, matrix<ET2, OT2>, OTR>
{
    using engine_type = matrix_subtraction_engine_t<ET1, ET2>;
    using op_traits   = OTR;
    using result_type = matrix<engine_type, op_traits>;

    static result_type  subtract(matrix<ET1, OT1> const& m1, matrix<ET2, OT2> const& m2);
};

template<class ET1, class OT1, class ET2, class OT2, class OTR>
typename matrix_subtraction_traits<matrix<ET1, OT1>, matrix<ET2, OT2>, OTR>::result_type
matrix_subtraction_traits<matrix<ET1, OT1>, matrix<ET2, OT2>, OTR>::subtract
(matrix<ET1, OT1> const& m1, matrix<ET2, OT2> const& m2)
{
    PrintOperandTypes<result_type>("subtraction_traits", m1, m2);
    return result_type();
}


//=================================================================================================
//                                   **** MULTIPLICATION ****
//=================================================================================================
//  Traits type that performs multiplication.
//=================================================================================================
//
template<class OP1, class OP2, class OTR>
struct matrix_multiplication_traits;

//---------------
//- vector*scalar
//
template<class ET1, class OT1, class T2, class OTR>
struct matrix_multiplication_traits<vector<ET1, OT1>, T2, OTR>
{
    using engine_type = matrix_multiplication_engine_t<ET1, T2>;
    using op_traits   = OTR;
    using result_type = vector<engine_type, op_traits>;

    static result_type  multiply(vector<ET1, OT1> const& v1, T2 s2);
};

template<class ET1, class OT1, class T2, class OTR> inline
typename matrix_multiplication_traits<vector<ET1, OT1>, T2, OTR>::result_type
matrix_multiplication_traits<vector<ET1, OT1>, T2, OTR>::multiply
(vector<ET1, OT1> const& v1, T2 s2)
{
    PrintOperandTypes<result_type>("multiplication_traits", v1, s2);
    return result_type();
}

//---------------
//- scalar*vector
//
template<class T1, class ET2, class OT2, class OTR>
struct matrix_multiplication_traits<T1, vector<ET2, OT2>, OTR>
{
    using engine_type = matrix_multiplication_engine_t<ET2, T1>;
    using op_traits   = OTR;
    using result_type = vector<engine_type, op_traits>;

    static result_type  multiply(T1 s1, vector<ET2, OT2> const& v2);
};

template<class T1, class ET2, class OT2, class OTR> inline
typename matrix_multiplication_traits<T1, vector<ET2, OT2>, OTR>::result_type
matrix_multiplication_traits<T1, vector<ET2, OT2>, OTR>::multiply
(T1 s1, vector<ET2, OT2> const& v2)
{
    PrintOperandTypes<result_type>("multiplication_traits", s1, v2);
    return result_type();
}

//---------------
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

template<class ET1, class OT1, class T2, class OTR> inline
typename matrix_multiplication_traits<matrix<ET1, OT1>, T2, OTR>::result_type
matrix_multiplication_traits<matrix<ET1, OT1>, T2, OTR>::multiply(matrix<ET1, OT1> const& m1, T2 s2)
{
    PrintOperandTypes<result_type>("multiplication_traits", m1, s2);
    return result_type();
}

//---------------
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

template<class T1, class ET2, class OT2, class OTR> inline
typename matrix_multiplication_traits<T1, matrix<ET2, OT2>, OTR>::result_type
matrix_multiplication_traits<T1, matrix<ET2, OT2>, OTR>::multiply(T1 s1, matrix<ET2, OT2> const& m2)
{
    PrintOperandTypes<result_type>("multiplication_traits", s1, m2);
    return result_type();
}

//---------------
//- vector*vector
//
template<class ET1, class OT1, class ET2, class OT2, class OTR>
struct matrix_multiplication_traits<vector<ET1, OT1>, vector<ET2, OT2>, OTR>
{
    using elem_type_1 = typename vector<ET1, OT1>::element_type;
    using elem_type_2 = typename vector<ET2, OT2>::element_type;
    using result_type = matrix_element_promotion_t<elem_type_1, elem_type_2>;

    static result_type  multiply(vector<ET1, OT1> const& v1, vector<ET2, OT2> const& v2);
};

template<class ET1, class OT1, class ET2, class OT2, class OTR> inline
typename matrix_multiplication_traits<vector<ET1, OT1>, vector<ET2, OT2>, OTR>::result_type
matrix_multiplication_traits<vector<ET1, OT1>, vector<ET2, OT2>, OTR>::multiply
(vector<ET1, OT1> const& v1, vector<ET2, OT2> const& v2)
{
    PrintOperandTypes<result_type>("multiplication_traits", v1, v2);
    return result_type();
}

//---------------
//- matrix*vector
//
template<class ET1, class OT1, class ET2, class OT2, class OTR>
struct matrix_multiplication_traits<matrix<ET1, OT1>, vector<ET2, OT2>, OTR>
{
    using engine_type = matrix_multiplication_engine_t<ET1, ET2>;
    using op_traits   = OTR;
    using result_type = vector<engine_type, op_traits>;

    static result_type  multiply(matrix<ET1, OT1> const& m1, vector<ET2, OT2> const& v2);
};

template<class ET1, class OT1, class ET2, class OT2, class OTR> inline
typename matrix_multiplication_traits<matrix<ET1, OT1>, vector<ET2, OT2>, OTR>::result_type
matrix_multiplication_traits<matrix<ET1, OT1>, vector<ET2, OT2>, OTR>::multiply
(matrix<ET1, OT1> const& m1, vector<ET2, OT2> const& v2)
{
    PrintOperandTypes<result_type>("multiplication_traits", m1, v2);
    return result_type();
}

//---------------
//- vector*matrix
//
template<class ET1, class OT1, class ET2, class OT2, class OTR>
struct matrix_multiplication_traits<vector<ET1, OT1>, matrix<ET2, OT2>, OTR>
{
    using engine_type = matrix_multiplication_engine_t<ET1, ET2>;
    using op_traits   = OTR;
    using result_type = vector<engine_type, op_traits>;

    static result_type  multiply(vector<ET1, OT1> const& v1, matrix<ET2, OT2> const& m2);
};

template<class ET1, class OT1, class ET2, class OT2, class OTR> inline
typename matrix_multiplication_traits<vector<ET1, OT1>, matrix<ET2, OT2>, OTR>::result_type
matrix_multiplication_traits<vector<ET1, OT1>, matrix<ET2, OT2>, OTR>::multiply
(vector<ET1, OT1> const& v1, matrix<ET2, OT2> const& m2)
{
    PrintOperandTypes<result_type>("multiplication_traits", v1, m2);
    return result_type();
}

//---------------
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


template<class ET1, class OT1, class ET2, class OT2, class OTR> inline auto
//typename matrix_multiplication_traits<matrix<ET1, OT1>, matrix<ET2, OT2>, OTR>::result_type
matrix_multiplication_traits<matrix<ET1, OT1>, matrix<ET2, OT2>, OTR>::multiply
(matrix<ET1, OT1> const& m1, matrix<ET2, OT2> const& m2) -> result_type
{
/*    if constexpr (!ET1::is_resizable::value && ET2::is_resizable::value)
    {
        cout << "hi\n";
    }
*/    PrintOperandTypes<result_type>("multiplication_traits", m1, m2);
    return result_type();
}

}       //- STD_LA namespace
#endif  //- MATRIX_ARITHMETIC_TRAITS_HPP_DEFINED
