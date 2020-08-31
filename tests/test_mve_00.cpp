#define ENABLE_TEST_PRINTING
#include "test_common.hpp"

using namespace STD_LA;
using namespace STD_LA::detail;
using namespace MDSPAN_NS;


TEST(Matrix, NegationView)
{
}

TEST(Matrix, ConjugateView)
{
}

TEST(Matrix, HermitianView)
{
}

TEST(AddTraits, MatViews)
{
    using fxd_float   = fixed_size_matrix<float, 2, 3>;
    using gen_float   = general_matrix<float, 2, 3>;
    using dyn_float   = dynamic_matrix<float>;

    fxd_float   m1  = {{ 1, 2, 3}, { 4, 5, 6}};
    auto        m1n = -m1;
    auto        m1t = m1.t();
    auto        m1h = m1.h();

    PRINT(m1);
    PRINT(-m1);
    PRINT(m1n);
    PRINT(m1.t());
    PRINT(m1t);
    PRINT(m1.h());
    PRINT(m1h);

    m1t(0, 0) = 38.0f;
    PRINT(m1t);
    m1.t()(0, 1) = 48.0f;
    PRINT(m1.t());

    dyn_float   m2 = {{10, 20}, {30, 40}, {50, 60}};
    m1.t() = m2;
    PRINT(m1.t());

    matrix_view_engine<fxd_float::engine_type, matrix_view::submatrix>  ve1;
    matrix_view_engine<fxd_float::engine_type, matrix_view::transpose>  ve2;

    PRINT(m1);
    PRINT(m1.submatrix(0, 2, 1, 2));
    PRINT(m1.submatrix(0, 2, 1, 2).t());

    m1.submatrix(0, 2, 1, 2)(0, 1) = 70;
    PRINT(m1);
    PRINT(m1.submatrix(0, 2, 1, 2));
    PRINT(m1.submatrix(0, 2, 1, 2).t());

    m1.submatrix(0, 2, 1, 2).t()(0, 1) = 80;
    PRINT(m1);
    PRINT(m1.submatrix(0, 2, 1, 2));
    PRINT(m1.submatrix(0, 2, 1, 2).t());

    PRINT(m1.row(1));
    PRINT(m1.column(1));
    PRINT(m1.row(1).t());
    PRINT(m1.column(1).t());

    PRINT_TYPE(decltype(m1.column(1).t()));
    PRINT_TYPE(decltype(m1.column(1).t()(0)));
    PRINT(m1);
    PRINT(m1.column(1).t());

    PRINT(m1);
    PRINT(m1.span());
    PRINT(-m1);
    PRINT((-m1).span());

    PRINT(m1.t());
    PRINT(m1.t().span());

    auto    s1 = m1.t().span();
    PRINT(s1);
    s1(0, 0) = 43;
    PRINT(s1);
    PRINT(m1);
    PRINT(m1.t());
    PRINT(m1.t().span());

    PRINT_TYPE(decltype(m1.span()));
    PRINT(m1.span());
    PRINT_TYPE(decltype(m1.submatrix(0, 2, 1, 2)));
    PRINT(m1.submatrix(0, 2, 1, 2));
    PRINT_TYPE(decltype(m1.submatrix(0, 2, 1, 2).span()));
    PRINT(m1.submatrix(0, 2, 1, 2).span());

    PRINT_TYPE(decltype(m2.span()));
    PRINT(m2.span());
    PRINT_TYPE(decltype(m2.submatrix(0, 2, 1, 2)));
    PRINT(m2.submatrix(0, 2, 1, 2));
    PRINT_TYPE(decltype(m2.submatrix(0, 2, 1, 2).span()));
    PRINT(m2.submatrix(0, 2, 1, 2).span());

    PRINT(m1.row(1));
    PRINT(m1.row(1).span());

    PRINT(m1.row(1).t());
    PRINT(m1.row(1).t().span());

    PRINT(m1.column(0));
    PRINT(m1.column(0).span());

    PRINT(m1.column(0).t());
    PRINT(m1.column(0).t().span());

    m1.column(0).t()(0) = 55;
    m1.column(0).t().span()(0, 1) = 66;
    PRINT(m1);
    PRINT(m1.column(0).t().span());
    PRINT_TYPE(decltype(m1.column(0).t().span()));

    PRINT(m1);
    PRINT(m1.conj());
    PRINT_TYPE(decltype(m1.conj()));
}

TEST(AddTraits, VecViews)
{
    using fxd_float   = fixed_size_vector<float, 3>;
    using gen_float   = general_vector<float, 3>;
    using dyn_float   = dynamic_vector<float>;

    fxd_float   v1  = { 1, 2, 3};

    PRINT_TYPE(decltype(v1));
    PRINT(v1);
    PRINT_TYPE(decltype(-v1));
    PRINT(-v1);

    PRINT_TYPE(decltype(v1.span()));
    PRINT(v1.span());
    PRINT_TYPE(decltype((-v1).span()));
    PRINT((-v1).span());

    dyn_float   v2 = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    PRINT_TYPE(decltype(v2));
    PRINT(v2);
    PRINT_TYPE(decltype(v2.subvector(2, 5)));
    PRINT(v2.subvector(2, 5));
    PRINT_TYPE(decltype(v2.subvector(2, 5).span()));
    PRINT(v2.subvector(2, 5).span());
    PRINT_TYPE(decltype((-v2).subvector(2, 5)));
    PRINT((-v2).subvector(2, 5));
    PRINT_TYPE(decltype((-v2).subvector(2, 5).span()));
    PRINT((-v2).subvector(2, 5).span());
    PRINT_TYPE(decltype((-v2.subvector(2, 5))));
    PRINT((-v2.subvector(2, 5)));
    PRINT_TYPE(decltype((-v2.subvector(2, 5)).span()));
    PRINT((-v2.subvector(2, 5)).span());

    PRINT_TYPE(decltype(v2.conj()));
    PRINT(v2.conj());
}

