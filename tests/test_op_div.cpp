#define ENABLE_TEST_PRINTING
#include "test_common.hpp"

using namespace STD_LA;
using namespace STD_LA::detail;
using namespace MDSPAN_NS;

using fmf_33 = fixed_size_matrix<float, 3, 3>;
using fmf_44 = fixed_size_matrix<float, 4, 4>;

using gmf_33 = general_matrix<float, 3, 3>;
using gmf_44 = general_matrix<float, 4, 4>;

using fmd_33 = fixed_size_matrix<double, 3, 3>;

using dmf    = dynamic_matrix<float>;

TEST(Div, Constexpr)
{
    constexpr fmf_33    m1 = LST_33_1;
    PRINT(m1);
    constexpr fmf_33    m2 = LST_33_2;
    PRINT(m2);
    constexpr fmf_33    mr = m1 / 2.0f;
    PRINT(mr);
    constexpr fmf_33    ms = mr.t() / 2.0;
    PRINT(ms);

}
