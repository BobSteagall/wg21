#include "linear_algebra.hpp"
#include "test_new_number.hpp"
#include "test_new_engine.hpp"
#include "test_new_arithmetic.hpp"

using cx_float  = std::complex<float>;
using cx_double = std::complex<double>;
using cx_newnum = std::complex<NewNum>;
using namespace STD_LA;

void t101()
{
    PRINT_FN_NAME(t101);

    fs_matrix<NewNum, 3, 4>     fmp1;
    fs_matrix<NewNum, 3, 4>     fmp2;
    dyn_matrix<NewNum>          dmp1(3, 4);
    fs_matrix<float, 3, 4>      fmf1;
    fs_matrix<double, 3, 4>     fmd1;
    dyn_matrix<double>          dmd1(3, 4);

    auto    r0 = fmp1 + fmp2;
    auto    r1 = fmp1 + dmp1;
    auto    r2 = fmp1 + fmf1;
    auto    r3 = fmp1 + dmd1;

    fs_matrix<cx_newnum, 3, 4>  fmcp1;

    auto    r4 = fmcp1 + fmp2;
}

void t102()
{
    PRINT_FN_NAME(t102);

    fs_matrix<NewNum, 3, 4>     fmp1;
    fs_matrix<NewNum, 4, 5>     fmp2;
    dyn_matrix<NewNum>          dmp1(4, 5);
    fs_matrix<float, 4, 5>      fmf1;
    fs_matrix<double, 4, 5>     fmd1;
    dyn_matrix<double>          dmd1(4, 5);

    auto    r0 = fmp1 * fmp2;
    auto    r1 = fmp1 * dmp1;
    auto    r2 = fmp1 * fmf1;
    auto    r3 = fmp1 * dmd1;

    fs_matrix<cx_newnum, 3, 4>  fmcp1;

    auto    r4 = fmcp1 * fmp2;
}

void t201()
{
    PRINT_FN_NAME(t201);

    fs_matrix_tst<NewNum, 3, 4>     fmnn1;
    fs_matrix_tst<NewNum, 3, 4>     fmnn2;
    dyn_matrix<NewNum>              dmnn1(3, 4);
    fs_matrix_tst<float, 3, 4>      fmf1;
    fs_matrix_tst<double, 3, 4>     fmd1;
    dyn_matrix<double>              dmd1(3, 4);

    auto    r0 = fmnn1 + fmnn2;
    auto    r1 = fmnn1 + dmnn1;
    auto    r2 = fmnn1 + fmf1;
    auto    r3 = fmnn1 + dmd1;

    fs_matrix<cx_newnum, 3, 4>      fmcp1;

    auto    r4 = fmcp1 + fmnn2;

    fs_matrix<float, 3, 4>          fmf2;
    fs_matrix<double, 3, 4>         fmd2;

    auto    r5 = fmf1 + fmf2;
    auto    r6 = fmf2 + fmf1;
}

void t301()
{
    PRINT_FN_NAME(t301);

    fs_matrix<float, 4, 4>  m1;
    fs_vector<float, 4>     v1, vr;

    vr = m1 * v1;
}

void t100()
{
    static_assert(is_matrix_element_v<NewNum>);
    static_assert(is_matrix_element_v<cx_newnum>);

    t101();
    t102();

    t201();

    t301();
}

