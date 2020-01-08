#include <array>
#include <linear_algebra.hpp>

void TestGroup00();
void TestGroup10();
void TestGroup20();
void TestGroup30();
void TestGroup40();
void TestGroup50();
void TestGroup60();
void TestGroup70();

int main()
{
    TestGroup00();
    TestGroup10();
    TestGroup20();
    TestGroup30();
	TestGroup40();
//	TestGroup50();
//	TestGroup60();
//	TestGroup70();

    std::array<int const, 2>   ai{3, 5};

    using T = int;
    using TC = int const;
    using TCC = TC const;

    PRINT_TYPE(T const*);
    PRINT_TYPE(TC const*);
    PRINT_TYPE(TCC const*);
//    PRINT_TYPE(TCC const const*);

    using MET = STD_LA::fs_matrix_engine<float,4,4>;
    using CET = STD_LA::fs_matrix_engine<float,4,4> const;

    PRINT_TYPE(STD_LA::detail::matrix_view_reference_t<MET>);
    PRINT_TYPE(STD_LA::detail::matrix_view_reference_t<CET>);

    return 0;
}