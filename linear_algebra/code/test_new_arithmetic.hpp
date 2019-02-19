#ifndef TEST_NEW_ARITHMETIC_HPP_DEFINED
#define TEST_NEW_ARITHMETIC_HPP_DEFINED

#include "linear_algebra.hpp"
#include "test_new_engine.hpp"

namespace STD_LA {

template<class T1, size_t R, size_t C, class T2, class OTR>
struct matrix_addition_traits<matrix<fs_matrix_engine_tst<T1, R, C>, OTR>, 
                              matrix<fs_matrix_engine_tst<T2, R, C>, OTR>, 
                              OTR>
{
    using engine_type = matrix_addition_engine_t<fs_matrix_engine_tst<T1, R, C>, 
                                                 fs_matrix_engine_tst<T2, R, C>>;
    using op_traits   = OTR;
    using result_type = matrix<engine_type, op_traits>;

    static result_type  add(matrix<fs_matrix_engine_tst<T1, R, C>, OTR> const& m1, 
                            matrix<fs_matrix_engine_tst<T2, R, C>, OTR> const& m2)
                        {
                            PrintOperandTypes<result_type>("addition_traits_TST", m1, m2);
                            return result_type();
                        }

};
}
#endif  //- TEST_NEW_ARITHMETIC_HPP_DEFINED