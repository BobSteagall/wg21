#include "linear_algebra.hpp"
#include <array>

using std::cout;
using std::endl;

using fsd_mat_engine_35 = STD_LA::fs_matrix_engine<double, 3, 5>;
using drd_mat_engine    = STD_LA::dr_matrix_engine<double, std::allocator<double>>;

using drm_double    = STD_LA::dyn_matrix<double>;
using drv_double    = STD_LA::dyn_vector<double>;

using fsv_double_5  = STD_LA::fs_vector<double, 5>;
using fsm_double_35 = STD_LA::fs_matrix<double, 3, 5>;
using fsm_float_35  = STD_LA::fs_matrix<float, 3, 5>;
using fsm_double_36 = STD_LA::fs_matrix<double, 3, 6>;
using fsm_double_99 = STD_LA::fs_matrix<double, 9, 9>;

void t000()
{
    PRINT_FNAME();

    drm_double  m1; //(4, 4);

    Fill(m1);
    PRINT(m1);

    m1.resize(3, 3);
    Fill(m1);
    PRINT(m1);

    m1.reserve(6, 6);
    PRINT(m1);

    m1.resize(4, 4);
    PRINT(m1);

    drm_double  m2(m1);
    PRINT(m2);

    drm_double  m3(std::move(m1));
    PRINT(m1);
    PRINT(m3);
    Fill(m3);
    m3.resize(6, 6);
    PRINT(m3);

    fsm_double_35  fm1, fm2, fm3;
    Fill(fm1);
    PRINT(fm1);

    m1 = fm1;
    PRINT(m1);
    fm2 = fm1;
    PRINT(fm2);
    fm3 = m1;
    PRINT(fm3);

    PRINT(fm3.t());

    fsm_double_35   fm4;
    fsm_float_35    fm5;

    PRINT(fm4);
    Fill(fm5);
    PRINT(fm5);
    fm4 = fm5;
    PRINT(fm4);

    //  fm4 = fm5.t();              fails on static_assert, as expected
    //  fsm_double_36   fm6(fm4);   fails on static_assert, as expected
}

void t001()
{
    PRINT_FNAME();

    drm_double  m1(4, 5), m2;

    Fill(m1);
    PRINT(m1);
    PRINT(m1.t());
    PRINT(m1.column(1));

    PRINT(m1.row(1));
    PRINT(m1.t().column(1));

    auto    tc = m1.column(2);
    PRINT_TYPE(decltype(tc));
    PRINT(tc);
    PRINT_TYPE(decltype(tc(2)));
    tc(1) = 101;
    PRINT(tc);

    PRINT(m1.t().row(1));

    auto    tr = m1.row(2);
    PRINT_TYPE(decltype(tr));
    PRINT(tr);
    PRINT_TYPE(decltype(tr(2)));
    tr(3) = 101;
    PRINT(tr);

    PRINT(m1.t().t().column(1));
    PRINT(m1.t().t().row(1));

    std::cout << "\n================\n";

    PRINT(m1.column(0));
    PRINT(m1.column(1));
    PRINT(m1.column(2));
    PRINT(m1.column(3));
    PRINT(m1.column(4));

    PRINT(m1.row(0));
    PRINT(m1.row(1));
    PRINT(m1.row(2));
    PRINT(m1.row(3));

    PRINT(m1);
    m1.swap_columns(1, 3);
    PRINT(m1);
    m1.swap_rows(0, 2);
    PRINT(m1);

    m2 = m1;
    m2.swap_rows(0, 2);
    m2.swap_columns(1, 3);
    PRINT(m2);

    drv_double  v1;

    v1 = m2.row(1);
    PRINT(v1);
    v1 = m2.column(2);
    PRINT(v1);
}


constexpr double t002()
{
    fsm_double_35   fsm  = {{1, 2, 3, 4, 5}, {6, 7, 8, 9, 10}, {11, 12, 13, 14, 15}};

    fsv_double_5    fsv  = {11, 12, 13, 14, 15};
    fsv_double_5    fsv2 = {11, 12, 13, 0, 0};

    fsm.swap_columns(1, 3);
    fsm.swap_rows(0, 2);

    double    d1 = fsm(1,2);
    double    d2 = fsv(3);

    fsm.column(2) = {0, 0, 0};
    fsm.row(1)    = {17, 17, 17, 17, 17};

    STD_LA::detail::la_swap(d1, d2);

    fsm_double_35 const fsm2 = {{1, 2, 3, 4, 5}, {6, 7, 8, 9, 10}, {11, 12, 13, 14, 15}};

//    fsm2.column(2) = {0, 0, 0};
//    fsm2.row(1)    = {17, 17, 17, 17, 17};

    fsm_double_99   fsm3;
    fsm3.submatrix(1, 4, 3, 2) = {{1, 2}, {3, 4}, {5, 6}, {7, 8}};
    fsm3.submatrix(1, 4, 3, 2).row(2) = {11, 11};

    fsm.t().row(3) = {22, 23, 24};

    return fsm3(1, 2);
}

constexpr double t003()
{
    constexpr double d = t002();
    return d;
}

constexpr double cd = t003();

using namespace std::experimental;
using namespace std::experimental::math;

#include <array>

using std::array;

namespace {
void t002X(drm_double const& m1, fsm_double_36 const& m2)
{
#ifdef LA_USE_MDSPAN
    PRINT_FNAME();
    PRINT(m1);

    auto    spn1 = m1.span();
    PRINT(spn1);
    PRINT_TYPE(decltype(spn1));

    PRINT(m2);
    auto    spn2 = m2.span();
    PRINT(spn2);
    PRINT_TYPE(decltype(spn2));

    auto    tr3  = m1.t();
    PRINT(tr3);
    PRINT_TYPE(decltype(tr3));

    auto    spn3 = tr3.span();
    PRINT(spn3);
    PRINT_TYPE(decltype(spn3));

    auto    tr4  = m2.t();
    PRINT(tr4);
    PRINT_TYPE(decltype(tr4));

    auto    spn4 = tr4.span();
    PRINT(spn4);
    PRINT_TYPE(decltype(spn4));

    auto    col1 = m1.column(1);
    PRINT(col1);
    PRINT_TYPE(decltype(col1));

    auto    cspn1 = col1.span();
    PRINT(cspn1);
    PRINT_TYPE(decltype(cspn1));

    auto    row1 = m1.row(1);
    PRINT(row1);
    PRINT_TYPE(decltype(row1));

    auto    rspn1 = row1.span();
    PRINT(rspn1);
    PRINT_TYPE(decltype(rspn1));

    auto    sub1 = m1.submatrix(2, 4, 3, 6);
    PRINT(sub1);
    PRINT_TYPE(decltype(sub1));

    auto    sspn1 = sub1.span();
    PRINT(sspn1);
    PRINT_TYPE(decltype(sspn1));
#endif
}

void t001X()
{
#ifdef LA_USE_MDSPAN
    PRINT_FNAME();

    drm_double  m1(10, 13, 16, 19);
    Fill(m1);
    PRINT(m1);

    auto    spn1 = m1.span();
    PRINT(spn1);
    PRINT_TYPE(decltype(spn1));

    fsm_double_36  m2;
    Fill(m2);
    PRINT(m2);

    auto    spn2 = m2.span();
    PRINT(spn2);
    PRINT_TYPE(decltype(spn2));

    auto    tr3  = m1.t();
    PRINT(tr3);
    PRINT_TYPE(decltype(tr3));

    auto    spn3 = tr3.span();
    PRINT(spn3);
    PRINT_TYPE(decltype(spn3));

    auto    tr4  = m2.t();
    PRINT(tr4);
    PRINT_TYPE(decltype(tr4));

    auto    spn4 = tr4.span();
    PRINT(spn4);
    PRINT_TYPE(decltype(spn4));

    auto    col1 = m1.column(1);
    PRINT(col1);
    PRINT_TYPE(decltype(col1));

    auto    cspn1 = col1.span();
    PRINT(cspn1);
    PRINT_TYPE(decltype(cspn1));

    auto    row1 = m1.row(1);
    PRINT(row1);
    PRINT_TYPE(decltype(row1));

    auto    rspn1 = row1.span();
    PRINT(rspn1);
    PRINT_TYPE(decltype(rspn1));

    auto    sub1 = m1.submatrix(2, 4, 3, 6);
    PRINT(sub1);
    PRINT_TYPE(decltype(sub1));

    auto    sspn1 = sub1.span();
    PRINT(sspn1);
    PRINT_TYPE(decltype(sspn1));

    t002X(m1, m2);

//    static_assert(!std::is_same_v<double, double const>);
#endif
}
}

template<class T> constexpr void
tf(std::initializer_list<std::initializer_list<T>> list)
{
    STD_LA::detail::check_source_init_list(list);
}

constexpr int
t003X()
{
    tf({{0, 1, 2}, {1, 2, 3}, {2, 3, 4}, {3, 4, 5}});
    tf({{0, 1, 2}, {3, 4, 5}});

    STD_LA::detail::check_source_init_list({{0, 1, 2}, {3, 4, 5}});
    STD_LA::detail::check_source_init_list({{0, 1, 2}, {3, 4, 5}}, 2, 3);

    return 0;
}

int
t003Y()
{
    tf({{0, 1, 2}, {1, 2, 3}, {2, 3, 4}, {3, 4, 5}});
    tf({{0, 1, 2}, {3, 4, 5}});

    STD_LA::detail::check_source_init_list({{0, 1, 2}, {3, 4, 5}});
    STD_LA::detail::check_source_init_list({{0, 1, 2}, {3, 4, 5}}, 2, 3);
//    STD_LA::detail::check_source_init_list({{0, 1, 2}, {3, 4, 5}}, 2, 4);

    fsm_double_35   fsm  = {{1, 2, 3, 4, 5}, {6, 7, 8, 9, 10}, {11, 12, 13, 14, 15}};
    fsv_double_5    fsv  = {11, 12, 13, 14, 15};

    PRINT(fsm);
    PRINT(fsv);

    drm_double      drm  = {{1, 2, 3, 4, 5}, {6, 7, 8, 9, 10}, {11, 12, 13, 14, 15}};
    drv_double      drv  = {11, 12, 13, 14, 15};

    PRINT(drm);
    PRINT(drv);

    drm = {{11, 12, 13, 14, 15}, {6, 7, 8, 9, 10}, {1, 2, 3, 4, 5}};
    PRINT(drm);
    fsm = {{11, 12, 13, 14, 15}, {6, 7, 8, 9, 10}, {1, 2, 3, 4, 5}};
    PRINT(fsm);

    drv = {5, 4, 3, 2, 1};
    PRINT(drv);
    fsv = {5, 4, 3, 2, 1};
    PRINT(fsv);

    auto    fcv = fsm.column(2);
    PRINT(fcv);
    fcv = {6, 6, 6};
    PRINT(fcv);
    fsm.column(3) = {0, 0, 0};
    PRINT(fsm);
    fsm.row(2) = {8, 8, 8, 8, 8};
    PRINT(fsm);

    auto    dcv = drm.column(1);
    PRINT(dcv);
    dcv = {3, 3, 3};
    PRINT(dcv);
    PRINT(drm);
    drm.column(3) = {0, 0, 0};
    PRINT(drm);
    drm.row(2) = {18, 18, 18, 18, 18};
    PRINT(drm);

    drm_double  drm2(10, 10);
    PRINT(drm2);
    drm2.submatrix(1, 4, 3, 2) = {{1, 2}, {3, 4}, {5, 6}, {7, 8}};
    auto const& cdrm2 = drm2;
    PRINT(cdrm2);

    drm2.submatrix(1, 4, 3, 2).row(2) = {11, 11};
    PRINT(drm2);

    PRINT(fsm);
    drm2.submatrix(6, fsm.rows(), 4, fsm.columns()) = fsm;
    PRINT(drm2);

    PRINT(fsm.t());
    PRINT(fsm.t().row(1));

    fsm.t().row(1) = {86, 87, 88};
    PRINT(fsm.t());
    PRINT(fsm);

    fsm.column(0) = drm.column(1);
    PRINT(fsm);

    return 0;
}

void
TestGroup00()
{
    constexpr double x = t002();
    constexpr int    i = t003X();
    int j = t003Y();

//    PRINT_FNAME();

//    PRINT_TYPE(decltype(tf));

    t001X();

//    t000();
//    t001();
}
