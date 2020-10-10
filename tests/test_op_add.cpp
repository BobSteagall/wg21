#define ENABLE_TEST_PRINTING
#include "test_common.hpp"

using namespace STD_LA;
using namespace STD_LA::detail;
using namespace MDSPAN_NS;

using fmf_33 = fixed_size_matrix<float, 3, 3>;
using fmf_44 = fixed_size_matrix<float, 4, 4>;

using gmf_33 = general_matrix<float, 3, 3>;
using gmf_44 = general_matrix<float, 4, 4>;

using dmf    = dynamic_matrix<float>;

template<class Lambda, int = (Lambda{}(), 0)> inline constexpr
ptrdiff_t
get_extent_helper(Lambda)
{
    return Lambda{}();
}

inline constexpr
ptrdiff_t
get_extent_helper(...)
{
    return dynamic_extent;
}

template<class ET> inline constexpr
ptrdiff_t
get_column_extent()
{
    return get_extent_helper([]{ return static_cast<ptrdiff_t>(ET().columns()); });
}

template<class ET> inline constexpr
ptrdiff_t
get_row_extent()
{
    return get_extent_helper([]{ return static_cast<ptrdiff_t>(ET().rows()); });
}



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

    constexpr auto  xc1 = get_column_extent<fmf_33::engine_type>();
    constexpr auto  xc2 = get_column_extent<gmf_44::engine_type>();
    constexpr auto  xc3 = get_column_extent<decltype(mr.t())::engine_type>();
    constexpr auto  xc4 = get_column_extent<dmf::engine_type>();

    constexpr auto  xc5 = extent_helper<fmf_33::engine_type>::columns();
    constexpr auto  xc6 = extent_helper<gmf_44::engine_type>::columns();
    constexpr auto  xc7 = extent_helper<decltype(mr.t())::engine_type>::columns();
    constexpr auto  xc8 = extent_helper<dmf::engine_type>::columns();

    //constexpr gmf_33    m10 = LST_33_1;
}
