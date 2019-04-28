#include "linear_algebra.hpp"
#include <array>

using std::cout;
using std::endl;

using fsd_mat_engine_35 = STD_LA::fs_matrix_engine<double, 3, 5>;
using drd_mat_engine    = STD_LA::dr_matrix_engine<double, std::allocator<double>>;

using drm_double    = STD_LA::dyn_matrix<double>;
using fsm_double_35 = STD_LA::fs_matrix<double, 3, 5>;
using fsm_float_35  = STD_LA::fs_matrix<float, 3, 5>;
using fsm_double_36 = STD_LA::fs_matrix<double, 3, 6>;

template<class ET, class OT>
void
FillMatrix(STD_LA::matrix<ET, OT>& m)
{
    using index_type   = typename STD_LA::matrix<ET, OT>::index_type;
    using element_type = typename STD_LA::matrix<ET, OT>::element_type;

    element_type    x = 1;

    for (index_type i = 0;  i < m.rows();  ++i)
    {
        for (index_type j = 0;  j < m.columns();  ++j)
        {
            m(i, j) = x;  x = x + 1;
        }
    }
}

void t000()
{
    PRINT_FNAME();

    drm_double  m1;

    FillMatrix(m1);
    PRINT(m1);

    m1.resize(3, 3);
    FillMatrix(m1);
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
    FillMatrix(m3);
    m3.resize(6, 6);
    PRINT(m3);

    fsm_double_35  fm1, fm2, fm3;
    FillMatrix(fm1);
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
    FillMatrix(fm5);
    PRINT(fm5);
    fm4 = fm5;
    PRINT(fm4);

    //  fm4 = fm5.t();              fails on static_assert, as expected
    //  fsm_double_36   fm6(fm4);   fails on static_assert, as expected
}

void t001()
{
    PRINT_FNAME();

    drm_double  m1(4, 5);

    FillMatrix(m1);
    PRINT(m1);
    PRINT(m1.t());
    PRINT(m1.column(1));
    PRINT(m1.row(1));
    PRINT(m1.t().column(1));
    PRINT(m1.t().row(1));
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
}


void
TestGroup00()
{
    PRINT_FNAME();

    t000();
    t001();
}
