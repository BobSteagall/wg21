#include "linear_algebra.hpp"
#include "test_new_number.hpp"

using cx_float  = std::complex<float>;
using cx_double = std::complex<double>;
using namespace STD_LA;

template<>
struct std::is_arithmetic<NewNum> : public std::true_type
{};

void t100()
{
    PRINT_FN_NAME(t100);

    fs_matrix<NewNum, 3, 4>     fmp1;
    fs_matrix<NewNum, 4, 5>     fmp2;
    dyn_matrix<NewNum>          dmp1(4, 5);
    fs_matrix<float, 4, 5>      fmf1;
    dyn_matrix<double>          dmd1(4, 5);

    auto    r0 = fmp1*fmp2;
    auto    r1 = fmp1*dmp1;
    auto    r2 = fmp1*fmf1;
    auto    r3 = fmp1*dmd1;
}

