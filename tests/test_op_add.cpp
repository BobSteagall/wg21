#define ENABLE_TEST_PRINTING
#include "test_common.hpp"

using namespace STD_LA;
using namespace STD_LA::detail;
using namespace MDSPAN_NS;

using fmf_33 = fixed_size_matrix<float, 3, 3>;
using fmf_44 = fixed_size_matrix<float, 4, 4>;

using gmf_33 = general_matrix<float, 3, 3>;
using gmf_44 = general_matrix<float, 4, 4>;

TEST(Add, Constexpr)
{
    constexpr fmf_33    m1 = LST_33_1;
    constexpr fmf_33    m2 = LST_33_2;
    constexpr fmf_33    mr = m1 + m2;

    constexpr fmf_33    ms = mr + mr.t();

    PRINT(mr);
    PRINT(ms);

    static_assert(m1 == m1);
    static_assert(m1 != m2);

    //constexpr gmf_33    m10 = LST_33_1;
}
