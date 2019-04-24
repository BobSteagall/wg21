#include "linear_algebra.hpp"

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
    drm_double  m1;

    FillMatrix(m1);
    PRINT_MATRIX(m1);

    m1.resize(3, 3);
    FillMatrix(m1);
    PRINT_MATRIX(m1);

    m1.reserve(6, 6);
    PRINT_MATRIX(m1);

    m1.resize(4, 4);
    PRINT_MATRIX(m1);

    drm_double  m2(m1);
    PRINT_MATRIX(m2);

    drm_double  m3(std::move(m1));
    PRINT_MATRIX(m1);
    PRINT_MATRIX(m3);
    FillMatrix(m3);
    m3.resize(6, 6);
    PRINT_MATRIX(m3);

    fsm_double_35  fm1, fm2, fm3;
    FillMatrix(fm1);
    PRINT_MATRIX(fm1);

    m1 = fm1;
    PRINT_MATRIX(m1);
    fm2 = fm1;
    PRINT_MATRIX(fm2);
    fm3 = m1;
    PRINT_MATRIX(fm3);

    fsm_float_35    fm4;
    fm3 = fm4;
    PRINT_MATRIX(fm3);
}

void
TestGroup00()
{
    PRINT_FNAME();

    t000();
}
