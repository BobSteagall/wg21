#define ENABLE_TEST_PRINTING
#include "test_common.hpp"

using namespace STD_LA;
using namespace STD_LA::detail;
using namespace MDSPAN_NS;

using fmf_33 = fixed_size_matrix<float, 3, 3>;
using fmf_34 = fixed_size_matrix<float, 3, 4>;
using fmf_43 = fixed_size_matrix<float, 4, 3>;
using fmf_44 = fixed_size_matrix<float, 4, 4>;

using gmf_33 = general_matrix<float, 3, 3>;
using gmf_44 = general_matrix<float, 4, 4>;

using fmd_33 = fixed_size_matrix<double, 3, 3>;

using dmf    = dynamic_matrix<float>;

TEST(Mul, Constexpr)
{
    constexpr fmf_33    m0 = LST_33_0;
    PRINT(m0);
    constexpr fmf_33    m1 = LST_33_1;
    PRINT(m1);
    constexpr fmf_33    m2 = LST_33_2;
    PRINT(m2);
    constexpr fmf_33    m3 = m1 * 2.0f;
    PRINT(m3);
    constexpr fmf_33    m4 = m1.t() * 2.0f;
    PRINT(m4);
    constexpr fmf_33    m5 = 3.0 * m1;
    PRINT(m5);
    constexpr fmf_33    m6 = 3.0 * m1.t();
    PRINT(m6);


    constexpr fmf_33    m7 = LST_33_4;
    PRINT(m7);
    constexpr fmf_33    m8 = m7*m0;
    PRINT(m8);
    constexpr fmf_33    m9 = m7*m7;
    PRINT(m9);

    constexpr fmf_33    m10 = LST_33_5;
    PRINT(m10);
    constexpr fmf_33    m11 = m7*m10;
    PRINT(m11);

    constexpr fmf_33    m12 = m7.t() * m7;
    PRINT(m12);

    constexpr fmf_34    m13 = LST_34_1;
    PRINT(m13);
    constexpr fmf_43    m14 = LST_43_1;
    PRINT(m14);
    constexpr auto      m15 = m13*m14;
    PRINT(m15);
    PRINT_TYPE(decltype(m15));
    constexpr auto      m16 = m14*m13;
    PRINT(m16);
    PRINT_TYPE(decltype(m16));
}
