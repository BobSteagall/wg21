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

TEST(Sub, Constexpr)
{
#if 0
    constexpr fmf_33    m1 = LST_33_1;
    constexpr fmf_33    m2 = LST_33_2;
    constexpr fmf_33    mr = m1 - m2;

    constexpr fmf_33    ms = mr.t();

    PRINT_TYPE(decltype(mr));
    PRINT_TYPE(decltype(ms));

    using ET1 = decltype(mr)::engine_type;
    using ET2 = decltype(mr.t())::engine_type;

    PRINT_TYPE(ET1);
    PRINT_TYPE(ET2);

    using ETT = addition_engine_traits<void, ET1, ET2>;
    PRINT_TYPE(ETT);
    PRINT_TYPE(ETT::owning_type_1);
    PRINT_TYPE(ETT::owning_type_2);
    PRINT_TYPE(ETT::allocation_traits);
    PRINT_TYPE(ETT::layout_traits);

    PRINT(ETT::R2);
    PRINT(ETT::C2);
    PRINT(has_constexpr_columns<ET1>());
    PRINT(has_constexpr_columns<ET2>());

    constexpr ET1   et1;
    constexpr auto  r1 = et1.rows();
    PRINT(r1);

    constexpr ET2   et2;
    constexpr auto  r2 = et2.rows();
    constexpr auto  s2 = et2.size();
    PRINT(r2);
    PRINT(s2);

    constexpr fmd_33    d2 = m2;

    auto  sm = d2.submatrix(0, 2, 0, 2);

    constexpr auto  m4 = mr - mr.h();
    constexpr auto  m5 = mr - mr.conj();
    constexpr auto  m6 = mr - mr.t();
    constexpr auto  m7 = mr - mr.h().conj().t();
    constexpr auto  m8 = mr - mr.adopt<void>();
    constexpr auto  m9 = mr.t() - mr.h() - mr.conj() - mr.adopt<void>();

    PRINT_TYPE(decltype(m4));
    PRINT_TYPE(decltype(m5));
    PRINT_TYPE(decltype(m6));
    PRINT_TYPE(decltype(m7));
    PRINT_TYPE(decltype(m8));
    PRINT_TYPE(decltype(m9));
    PRINT_TYPE(decltype(d2));

    constexpr fmf_33    mt = mr - mr.t().h().t();

//
//    constexpr fmd_33    d1 = LST_33_1;
//    constexpr fmd_33    d2 = m1;
//
//    PRINT(mr);
//    PRINT(ms);
//    PRINT(mt);

    static_assert(m1 == m1);
    static_assert(m1 != m2);

    constexpr auto  xc5 = engine_extents_helper<fmf_33::engine_type>::columns();
    constexpr auto  xc6 = engine_extents_helper<gmf_44::engine_type>::columns();
    constexpr auto  xc7 = engine_extents_helper<decltype(mr.t())::engine_type>::columns();
    constexpr auto  xc8 = engine_extents_helper<dmf::engine_type>::columns();

    PRINT(xc5);
    PRINT(xc6);
    PRINT(xc7);
    PRINT(xc8);

    //constexpr gmf_33    m10 = LST_33_1;
#endif
}
