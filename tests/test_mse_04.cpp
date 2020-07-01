#include "test_common.hpp"

using namespace STD_LA;
using namespace STD_LA::detail;

//==================================================================================================
//  Unit tests for fixed-size statically allocated matrix engine, Case 4A
//==================================================================================================
//
using mse_f_ffv_cm = matrix_storage_engine<float, extents<4, 4>, void, column_major>;
using mse_f_ffv_rm = matrix_storage_engine<float, extents<4, 4>, void, row_major>;
using mse_i_ffv_cm = matrix_storage_engine<int, extents<4, 4>, void, column_major>;
using mse_d_ffv_rm = matrix_storage_engine<double, extents<4, 4>, void, column_major>;

TEST(MSE_Matrix_4A, DirectCtor)
{
    mse_f_ffv_rm    e1;

    EXPECT_EQ(e1.rows(), 4);
    EXPECT_EQ(e1.columns(), 4);
    EXPECT_EQ(e1.size(), itup_t(4, 4));
    EXPECT_EQ(e1.row_capacity(), 4);
    EXPECT_EQ(e1.column_capacity(), 4);
    EXPECT_EQ(e1.capacity(), itup_t(4, 4));

    //- Verify elements are value initialized.
    //
    EXPECT_EQ(e1(0, 0), 0.0f);
    EXPECT_EQ(e1(0, 1), 0.0f);
    EXPECT_EQ(e1(0, 2), 0.0f);
    EXPECT_EQ(e1(0, 3), 0.0f);
    EXPECT_EQ(e1(1, 0), 0.0f);
    EXPECT_EQ(e1(1, 1), 0.0f);
    EXPECT_EQ(e1(1, 2), 0.0f);
    EXPECT_EQ(e1(1, 3), 0.0f);
    EXPECT_EQ(e1(2, 0), 0.0f);
    EXPECT_EQ(e1(2, 1), 0.0f);
    EXPECT_EQ(e1(2, 2), 0.0f);
    EXPECT_EQ(e1(2, 3), 0.0f);
    EXPECT_EQ(e1(3, 0), 0.0f);
    EXPECT_EQ(e1(3, 1), 0.0f);
    EXPECT_EQ(e1(3, 2), 0.0f);
    EXPECT_EQ(e1(3, 3), 0.0f);
}


TEST(MSE_Matrix_4A, CmpEq)
{
    mse_f_ffv_rm    e1, e2, e3;

    //- Verify size, capacity, initial values.
    //
    EXPECT_EQ(e1.rows(), 4);
    EXPECT_EQ(e1.columns(), 4);
    EXPECT_EQ(e1.size(), itup_t(4, 4));
    EXPECT_EQ(e1.row_capacity(), 4);
    EXPECT_EQ(e1.column_capacity(), 4);
    EXPECT_EQ(e1.capacity(), itup_t(4, 4));

    EXPECT_EQ(e1(0, 0), 0.0f);
    EXPECT_EQ(e1(0, 1), 0.0f);
    EXPECT_EQ(e1(0, 2), 0.0f);
    EXPECT_EQ(e1(0, 3), 0.0f);
    EXPECT_EQ(e1(1, 0), 0.0f);
    EXPECT_EQ(e1(1, 1), 0.0f);
    EXPECT_EQ(e1(1, 2), 0.0f);
    EXPECT_EQ(e1(1, 3), 0.0f);
    EXPECT_EQ(e1(2, 0), 0.0f);
    EXPECT_EQ(e1(2, 1), 0.0f);
    EXPECT_EQ(e1(2, 2), 0.0f);
    EXPECT_EQ(e1(2, 3), 0.0f);
    EXPECT_EQ(e1(3, 0), 0.0f);
    EXPECT_EQ(e1(3, 1), 0.0f);
    EXPECT_EQ(e1(3, 2), 0.0f);
    EXPECT_EQ(e1(3, 3), 0.0f);

    //- Verify expected equality of default-constructed engines.
    //
    EXPECT_TRUE(m_cmp_eq(e1, e1));
    EXPECT_TRUE(m_cmp_eq(e1, e2));
    EXPECT_TRUE(m_cmp_eq(e1, e3));

    //- Verify equality against an init-list of equivalent values.
    //
    EXPECT_TRUE(m_cmp_eq(e1, LST_44_0));
    EXPECT_TRUE(m_cmp_eq(e1, il_44_0));
    EXPECT_TRUE(m_cmp_eq(e1, fl_44_0));

    //- Verify inequality against an init-list of different values.
    //
    EXPECT_FALSE(m_cmp_eq(e1, LST_44_1));
    EXPECT_FALSE(m_cmp_eq(e1, il_44_1));
    EXPECT_FALSE(m_cmp_eq(e1, fl_44_1));

    //- Assign new element values via mutable indexing and verify them.
    //
    e2(0, 0) = 11.0f;
    e2(0, 1) = 12.0f;
    e2(0, 2) = 13.0f;
    e2(0, 3) = 14.0f;
    e2(1, 0) = 21.0f;
    e2(1, 1) = 22.0f;
    e2(1, 2) = 23.0f;
    e2(1, 3) = 24.0f;
    e2(2, 0) = 31.0f;
    e2(2, 1) = 32.0f;
    e2(2, 2) = 33.0f;
    e2(2, 3) = 34.0f;
    e2(3, 0) = 41.0f;
    e2(3, 1) = 42.0f;
    e2(3, 2) = 43.0f;
    e2(3, 3) = 44.0f;
    EXPECT_EQ(e2(0, 0), 11.0f);
    EXPECT_EQ(e2(0, 1), 12.0f);
    EXPECT_EQ(e2(0, 2), 13.0f);
    EXPECT_EQ(e2(0, 3), 14.0f);
    EXPECT_EQ(e2(1, 0), 21.0f);
    EXPECT_EQ(e2(1, 1), 22.0f);
    EXPECT_EQ(e2(1, 2), 23.0f);
    EXPECT_EQ(e2(1, 3), 24.0f);
    EXPECT_EQ(e2(2, 0), 31.0f);
    EXPECT_EQ(e2(2, 1), 32.0f);
    EXPECT_EQ(e2(2, 2), 33.0f);
    EXPECT_EQ(e2(2, 3), 34.0f);
    EXPECT_EQ(e2(3, 0), 41.0f);
    EXPECT_EQ(e2(3, 1), 42.0f);
    EXPECT_EQ(e2(3, 2), 43.0f);
    EXPECT_EQ(e2(3, 3), 44.0f);
    EXPECT_FALSE(m_cmp_eq(e1, e2));

    e3(0, 0) = 11.0f;
    e3(0, 1) = 12.0f;
    e3(0, 2) = 13.0f;
    e3(0, 3) = 14.0f;
    e3(1, 0) = 21.0f;
    e3(1, 1) = 22.0f;
    e3(1, 2) = 23.0f;
    e3(1, 3) = 24.0f;
    e3(2, 0) = 31.0f;
    e3(2, 1) = 32.0f;
    e3(2, 2) = 33.0f;
    e3(2, 3) = 34.0f;
    e3(3, 0) = 41.0f;
    e3(3, 1) = 42.0f;
    e3(3, 2) = 43.0f;
    e3(3, 3) = 44.0f;
    EXPECT_EQ(e3(0, 0), 11.0f);
    EXPECT_EQ(e3(0, 1), 12.0f);
    EXPECT_EQ(e3(0, 2), 13.0f);
    EXPECT_EQ(e3(0, 3), 14.0f);
    EXPECT_EQ(e3(1, 0), 21.0f);
    EXPECT_EQ(e3(1, 1), 22.0f);
    EXPECT_EQ(e3(1, 2), 23.0f);
    EXPECT_EQ(e3(1, 3), 24.0f);
    EXPECT_EQ(e3(2, 0), 31.0f);
    EXPECT_EQ(e3(2, 1), 32.0f);
    EXPECT_EQ(e3(2, 2), 33.0f);
    EXPECT_EQ(e3(2, 3), 34.0f);
    EXPECT_EQ(e3(3, 0), 41.0f);
    EXPECT_EQ(e3(3, 1), 42.0f);
    EXPECT_EQ(e3(3, 2), 43.0f);
    EXPECT_EQ(e3(3, 3), 44.0f);
    EXPECT_FALSE(m_cmp_eq(e1, e3));

    //- Verify expected equality of various argument combinations.
    //
    EXPECT_TRUE(m_cmp_eq(e2, e3));

    EXPECT_TRUE(m_cmp_eq(e2, LST_44_2));
    EXPECT_TRUE(m_cmp_eq(e2, il_44_2));
    EXPECT_TRUE(m_cmp_eq(e2, fl_44_2));

    EXPECT_TRUE(m_cmp_eq(e3, LST_44_2));
    EXPECT_TRUE(m_cmp_eq(e3, il_44_2));
    EXPECT_TRUE(m_cmp_eq(e3, fl_44_2));

    EXPECT_TRUE(m_cmp_eq(e2, e3));
    EXPECT_FALSE(m_cmp_eq(e2, e1));
    EXPECT_FALSE(m_cmp_eq(e1, e3));

    //- Verify expected inequality when compared to init-lists of incorrect size.
    //
    EXPECT_FALSE(m_cmp_eq(e3, LST_33_0));
    EXPECT_FALSE(m_cmp_eq(e3, il_33_1));
    EXPECT_FALSE(m_cmp_eq(e3, fl_33_2));

    //- Verify expected inequality against init-lists and engines having different contents.
    //
    EXPECT_FALSE(m_cmp_eq(e3, fl_44_0));
    EXPECT_FALSE(m_cmp_eq(e3, il_44_1));
    EXPECT_FALSE(m_cmp_eq(e3, mse_f_ffv_rm(LST_44_1)));
}


TEST(MSE_Matrix_4A, MoveCtor)
{
    //- Default construct and verify initial state.
    //
    mse_f_ffv_rm    e1;

    EXPECT_EQ(e1.rows(), 4);
    EXPECT_EQ(e1.columns(), 4);
    EXPECT_EQ(e1.row_capacity(), 4);
    EXPECT_EQ(e1.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e1, LST_44_0));

    //- Assign new element values and verify them.
    //
    e1(0, 0) = 11.0f;
    e1(0, 1) = 12.0f;
    e1(0, 2) = 13.0f;
    e1(0, 3) = 14.0f;
    e1(1, 0) = 21.0f;
    e1(1, 1) = 22.0f;
    e1(1, 2) = 23.0f;
    e1(1, 3) = 24.0f;
    e1(2, 0) = 31.0f;
    e1(2, 1) = 32.0f;
    e1(2, 2) = 33.0f;
    e1(2, 3) = 34.0f;
    e1(3, 0) = 41.0f;
    e1(3, 1) = 42.0f;
    e1(3, 2) = 43.0f;
    e1(3, 3) = 44.0f;
    EXPECT_TRUE(m_cmp_eq(e1, fl_44_2));

    //- Construct a new engine via move ctor and verify that its initial state is as expected.
    //
    mse_f_ffv_rm    e2(std::move(e1));

    EXPECT_EQ(e1.rows(), 4);
    EXPECT_EQ(e1.columns(), 4);
    EXPECT_EQ(e2.row_capacity(), 4);
    EXPECT_EQ(e2.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e2, fl_44_2));
}


TEST(MSE_Matrix_4A, CopyCtor)
{
    //- Default construct and verify initial state.
    //
    mse_f_ffv_rm    e1;
    EXPECT_EQ(e1.rows(), 4);
    EXPECT_EQ(e1.columns(), 4);
    EXPECT_EQ(e1.row_capacity(), 4);
    EXPECT_EQ(e1.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e1, LST_44_0));

    //- Assign new element values and verify them.
    //
    e1(0, 0) = 11.0f;
    e1(0, 1) = 12.0f;
    e1(0, 2) = 13.0f;
    e1(0, 3) = 14.0f;
    e1(1, 0) = 21.0f;
    e1(1, 1) = 22.0f;
    e1(1, 2) = 23.0f;
    e1(1, 3) = 24.0f;
    e1(2, 0) = 31.0f;
    e1(2, 1) = 32.0f;
    e1(2, 2) = 33.0f;
    e1(2, 3) = 34.0f;
    e1(3, 0) = 41.0f;
    e1(3, 1) = 42.0f;
    e1(3, 2) = 43.0f;
    e1(3, 3) = 44.0f;
    EXPECT_TRUE(m_cmp_eq(e1, fl_44_2));

    //- Construct new engines via copy ctors and verify their initial states.
    //
    mse_f_ffv_rm    e2(e1);

    EXPECT_EQ(e1.rows(), 4);
    EXPECT_EQ(e1.columns(), 4);
    EXPECT_EQ(e2.row_capacity(), 4);
    EXPECT_EQ(e2.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e2, e1));

    mse_f_ffv_rm    e3 = e1;

    EXPECT_EQ(e1.rows(), 4);
    EXPECT_EQ(e1.columns(), 4);
    EXPECT_EQ(e3.row_capacity(), 4);
    EXPECT_EQ(e3.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e3, e1));

    mse_f_ffv_rm    e4{e1};

    EXPECT_EQ(e1.rows(), 4);
    EXPECT_EQ(e1.columns(), 4);
    EXPECT_EQ(e4.row_capacity(), 4);
    EXPECT_EQ(e4.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e4, e1));
}


TEST(MSE_Matrix_4A, ListCtor)
{
    //- Construct new engines via list ctor and verify that their initial states.
    //
    mse_f_ffv_rm    e1 = LST_44_1;
    mse_f_ffv_rm    e2 LST_44_1 ;
    mse_f_ffv_rm    e3(LST_44_1);

    EXPECT_EQ(e1.rows(), 4);
    EXPECT_EQ(e1.columns(), 4);
    EXPECT_EQ(e1.row_capacity(), 4);
    EXPECT_EQ(e1.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e1, LST_44_1));

    EXPECT_EQ(e2.rows(), 4);
    EXPECT_EQ(e2.columns(), 4);
    EXPECT_EQ(e2.row_capacity(), 4);
    EXPECT_EQ(e2.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e2, e1));

    EXPECT_EQ(e3.rows(), 4);
    EXPECT_EQ(e3.columns(), 4);
    EXPECT_EQ(e3.row_capacity(), 4);
    EXPECT_EQ(e3.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e3, e1));
}


TEST(MSE_Matrix_4A, EngineCtor)
{
    //- Construct new engines via direct engine ctor and verify their initial states.
    //
    mse_i_ffv_cm    e1 = LST_44_2;
    mse_f_ffv_rm    e2(e1);
    mse_d_ffv_rm    e3(e2);
    mse_f_ffv_cm    e4 = LST_44_2;
    mse_f_ffv_rm    e5{e4};

    EXPECT_EQ(e1.rows(), 4);
    EXPECT_EQ(e1.columns(), 4);
    EXPECT_EQ(e1.row_capacity(), 4);
    EXPECT_EQ(e1.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e1, il_44_2));

    EXPECT_EQ(e2.rows(), 4);
    EXPECT_EQ(e2.columns(), 4);
    EXPECT_EQ(e2.row_capacity(), 4);
    EXPECT_EQ(e2.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e2, e1));

    EXPECT_EQ(e3.rows(), 4);
    EXPECT_EQ(e3.columns(), 4);
    EXPECT_EQ(e3.row_capacity(), 4);
    EXPECT_EQ(e3.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e3, e1));

    EXPECT_EQ(e4.rows(), 4);
    EXPECT_EQ(e4.columns(), 4);
    EXPECT_EQ(e4.row_capacity(), 4);
    EXPECT_EQ(e4.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e4, e1));

    EXPECT_EQ(e5.rows(), 4);
    EXPECT_EQ(e5.columns(), 4);
    EXPECT_EQ(e5.row_capacity(), 4);
    EXPECT_EQ(e5.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e5, e1));
}


TEST(MSE_Matrix_4A, MoveAssign)
{
    //- Default construct and verify initial state.
    //
    mse_f_ffv_rm    e1;

    EXPECT_EQ(e1.rows(), 4);
    EXPECT_EQ(e1.columns(), 4);
    EXPECT_EQ(e1.row_capacity(), 4);
    EXPECT_EQ(e1.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e1, il_44_0));

    //- List construct and verify initial state.
    //
    mse_f_ffv_rm    e2(il_44_1);

    EXPECT_EQ(e2.rows(), 4);
    EXPECT_EQ(e2.columns(), 4);
    EXPECT_EQ(e2.row_capacity(), 4);
    EXPECT_EQ(e2.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e2, fl_44_1));

    //- Move-assign and verify.
    //
    e1 = std::move(e2);
    EXPECT_TRUE(m_cmp_eq(e1, fl_44_1));
}


TEST(MSE_Matrix_4A, CopyAssign)
{
    //- Default construct and verify initial state.
    //
    mse_f_ffv_rm    e1;

    EXPECT_EQ(e1.rows(), 4);
    EXPECT_EQ(e1.columns(), 4);
    EXPECT_EQ(e1.row_capacity(), 4);
    EXPECT_EQ(e1.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e1, il_44_0));

    //- List construct and verify initial state.
    //
    mse_f_ffv_rm    e2(il_44_1);

    EXPECT_EQ(e2.rows(), 4);
    EXPECT_EQ(e2.columns(), 4);
    EXPECT_EQ(e2.row_capacity(), 4);
    EXPECT_EQ(e2.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e2, fl_44_1));

    //- Copy-assign and verify.
    //
    e1 = e2;
    EXPECT_TRUE(m_cmp_eq(e1, fl_44_1));
}


TEST(MSE_Matrix_4A, EngineAssign)
{
    //- Default construct and verify initial state.
    //
    mse_f_ffv_rm    e1, e2;

    EXPECT_EQ(e1.rows(), 4);
    EXPECT_EQ(e1.columns(), 4);
    EXPECT_EQ(e1.row_capacity(), 4);
    EXPECT_EQ(e1.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e1, LST_44_0));

    EXPECT_EQ(e2.rows(), 4);
    EXPECT_EQ(e2.columns(), 4);
    EXPECT_EQ(e2.row_capacity(), 4);
    EXPECT_EQ(e2.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e2, fl_44_0));

    //- List construct and verify initial state.
    //
    mse_i_ffv_cm    e3 LST_44_1 ;

    EXPECT_EQ(e3.rows(), 4);
    EXPECT_EQ(e3.columns(), 4);
    EXPECT_EQ(e3.row_capacity(), 4);
    EXPECT_EQ(e3.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e3, il_44_1));

    mse_i_ffv_cm    e4 LST_44_2 ;

    EXPECT_EQ(e4.rows(), 4);
    EXPECT_EQ(e4.columns(), 4);
    EXPECT_EQ(e4.row_capacity(), 4);
    EXPECT_EQ(e4.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e4, il_44_2));

    EXPECT_FALSE(m_cmp_eq(e3, e4));

    //- Assign and verify.
    //
    e1 = e3;
    EXPECT_TRUE(m_cmp_eq(e1, fl_44_1));
    EXPECT_TRUE(m_cmp_eq(e1, e3));

    e2 = e4;
    EXPECT_TRUE(m_cmp_eq(e2, fl_44_2));
    EXPECT_TRUE(m_cmp_eq(e2, e4));
}


TEST(MSE_Matrix_4A, ListAssign)
{
    //- Default construct and verify initial state.
    //
    mse_f_ffv_rm     e1;
    mse_f_ffv_cm     e2{il_44_1};

    EXPECT_EQ(e1.rows(), 4);
    EXPECT_EQ(e1.columns(), 4);
    EXPECT_EQ(e1.row_capacity(), 4);
    EXPECT_EQ(e1.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e1, LST_44_0));

    EXPECT_EQ(e2.rows(), 4);
    EXPECT_EQ(e2.columns(), 4);
    EXPECT_EQ(e2.row_capacity(), 4);
    EXPECT_EQ(e2.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e2, LST_44_1));

    //- Assign and verify.
    //
    e1 = LST_44_1;
    EXPECT_TRUE(m_cmp_eq(e1, LST_44_1));
    EXPECT_TRUE(m_cmp_eq(e1, il_44_1));
    EXPECT_TRUE(m_cmp_eq(e1, fl_44_1));
    EXPECT_TRUE(m_cmp_eq(e1, e2));

    e1 = fl_44_0;
    EXPECT_TRUE(m_cmp_eq(e1, LST_44_0));
    EXPECT_TRUE(m_cmp_eq(e1, il_44_0));
    EXPECT_TRUE(m_cmp_eq(e1, fl_44_0));
    EXPECT_TRUE(m_cmp_eq(e1, mse_f_ffv_rm()));
}


TEST(MSE_Matrix_4A, Swap)
{
    //- Default construct and verify initial state.
    //
    mse_f_ffv_rm    e1;

    EXPECT_EQ(e1.size(), st_44);
    EXPECT_TRUE(m_cmp_eq(e1, fl_44_0));

    //- List construct and verify initial state.
    //
    mse_f_ffv_rm    e2(fl_44_2);

    EXPECT_EQ(e2.size(), st_44);
    EXPECT_TRUE(m_cmp_eq(e2, fl_44_2));

    //- Swap contents and verify.
    //
    e1.swap(e2);
    EXPECT_TRUE(m_cmp_eq(e1, fl_44_2));
    EXPECT_TRUE(m_cmp_eq(e2, fl_44_0));

    e2.swap(e1);
    EXPECT_TRUE(m_cmp_eq(e1, il_44_0));
    EXPECT_TRUE(m_cmp_eq(e2, il_44_2));

    //- Swap some elements and verify.
    //
    e2.swap_columns(0, 2);
    EXPECT_TRUE(m_cmp_eq(e2, LST_44_2CS));

    e2 = LST_44_2;
    EXPECT_TRUE(m_cmp_eq(e2, LST_44_2));

    e2.swap_rows(1, 2);
    EXPECT_TRUE(m_cmp_eq(e2, LST_44_2RS));
}


TEST(MSE_Matrix_4A, Span)
{
    mse_f_ffv_rm        e1(il_44_2), e2, e3{e1};
    mse_f_ffv_rm const& ce1 = e1;

    auto    sp1  = e1.span();
    auto    csp1 = ce1.span();

    //- Const and mutable spans should have different types.
    //
    EXPECT_FALSE((std::is_same_v<decltype(sp1), decltype(csp1)>));

    //- The elements of the const and mutable spans should have the expected values.
    //
    EXPECT_EQ(sp1(0, 0), 11.0f);
    EXPECT_EQ(sp1(0, 1), 12.0f);
    EXPECT_EQ(sp1(0, 2), 13.0f);
    EXPECT_EQ(sp1(0, 3), 14.0f);
    EXPECT_EQ(sp1(1, 0), 21.0f);
    EXPECT_EQ(sp1(1, 1), 22.0f);
    EXPECT_EQ(sp1(1, 2), 23.0f);
    EXPECT_EQ(sp1(1, 3), 24.0f);
    EXPECT_EQ(sp1(2, 0), 31.0f);
    EXPECT_EQ(sp1(2, 1), 32.0f);
    EXPECT_EQ(sp1(2, 2), 33.0f);
    EXPECT_EQ(sp1(2, 3), 34.0f);
    EXPECT_EQ(sp1(3, 0), 41.0f);
    EXPECT_EQ(sp1(3, 1), 42.0f);
    EXPECT_EQ(sp1(3, 2), 43.0f);
    EXPECT_EQ(sp1(3, 3), 44.0f);

    EXPECT_EQ(csp1(0, 0), 11.0f);
    EXPECT_EQ(csp1(0, 1), 12.0f);
    EXPECT_EQ(csp1(0, 2), 13.0f);
    EXPECT_EQ(csp1(0, 3), 14.0f);
    EXPECT_EQ(csp1(1, 0), 21.0f);
    EXPECT_EQ(csp1(1, 1), 22.0f);
    EXPECT_EQ(csp1(1, 2), 23.0f);
    EXPECT_EQ(csp1(1, 3), 24.0f);
    EXPECT_EQ(csp1(2, 0), 31.0f);
    EXPECT_EQ(csp1(2, 1), 32.0f);
    EXPECT_EQ(csp1(2, 2), 33.0f);
    EXPECT_EQ(csp1(2, 3), 34.0f);
    EXPECT_EQ(csp1(3, 0), 41.0f);
    EXPECT_EQ(csp1(3, 1), 42.0f);
    EXPECT_EQ(csp1(3, 2), 43.0f);
    EXPECT_EQ(csp1(3, 3), 44.0f);

    //- Whole-object comparison between engines and spans should work as expected.
    //
    EXPECT_TRUE(m_cmp_eq(e1, sp1));
    EXPECT_TRUE(m_cmp_eq(e1, csp1));
    EXPECT_TRUE(m_cmp_eq(e3, sp1));
    EXPECT_TRUE(m_cmp_eq(e3, csp1));
    EXPECT_FALSE(m_cmp_eq(e2, sp1));
    EXPECT_FALSE(m_cmp_eq(e2, csp1));

    //- Setting values of individual span elements should be reflected in the owning engine.
    //
    sp1(0, 0) = 111.0f;
    sp1(1, 1) = 222.0f;
    sp1(2, 2) = 333.0f;
    sp1(3, 3) = 444.0f;
    EXPECT_EQ(e1(0, 0), 111.0f);
    EXPECT_EQ(e1(1, 1), 222.0f);
    EXPECT_EQ(e1(2, 2), 333.0f);
    EXPECT_EQ(e1(3, 3), 444.0f);
}


//==================================================================================================
//  Unit tests for fixed-size dynamically-allocated matrix engine, Case 4B
//==================================================================================================
//
using mse_f_ffa_cm = matrix_storage_engine<float, extents<4, 4>, allocator<float>, column_major>;
using mse_f_ffa_rm = matrix_storage_engine<float, extents<4, 4>, allocator<float>, row_major>;
using mse_i_ffa_cm = matrix_storage_engine<int, extents<4, 4>, allocator<int>, column_major>;
using mse_d_ffa_rm = matrix_storage_engine<double, extents<4, 4>, allocator<double>, column_major>;

TEST(MSE_Matrix_4B, DirectCtor)
{
    mse_f_ffa_rm    e1;

    EXPECT_EQ(e1.rows(), 4);
    EXPECT_EQ(e1.columns(), 4);
    EXPECT_EQ(e1.size(), itup_t(4, 4));
    EXPECT_EQ(e1.row_capacity(), 4);
    EXPECT_EQ(e1.column_capacity(), 4);
    EXPECT_EQ(e1.capacity(), itup_t(4, 4));

    //- Verify elements are value initialized.
    //
    EXPECT_EQ(e1(0, 0), 0.0f);
    EXPECT_EQ(e1(0, 1), 0.0f);
    EXPECT_EQ(e1(0, 2), 0.0f);
    EXPECT_EQ(e1(0, 3), 0.0f);
    EXPECT_EQ(e1(1, 0), 0.0f);
    EXPECT_EQ(e1(1, 1), 0.0f);
    EXPECT_EQ(e1(1, 2), 0.0f);
    EXPECT_EQ(e1(1, 3), 0.0f);
    EXPECT_EQ(e1(2, 0), 0.0f);
    EXPECT_EQ(e1(2, 1), 0.0f);
    EXPECT_EQ(e1(2, 2), 0.0f);
    EXPECT_EQ(e1(2, 3), 0.0f);
    EXPECT_EQ(e1(3, 0), 0.0f);
    EXPECT_EQ(e1(3, 1), 0.0f);
    EXPECT_EQ(e1(3, 2), 0.0f);
    EXPECT_EQ(e1(3, 3), 0.0f);
}


TEST(MSE_Matrix_4B, CmpEq)
{
    mse_f_ffa_rm    e1, e2, e3;

    //- Verify size, capacity, initial values.
    //
    EXPECT_EQ(e1.rows(), 4);
    EXPECT_EQ(e1.columns(), 4);
    EXPECT_EQ(e1.size(), itup_t(4, 4));
    EXPECT_EQ(e1.row_capacity(), 4);
    EXPECT_EQ(e1.column_capacity(), 4);
    EXPECT_EQ(e1.capacity(), itup_t(4, 4));


    EXPECT_EQ(e1(0, 0), 0.0f);
    EXPECT_EQ(e1(0, 1), 0.0f);
    EXPECT_EQ(e1(0, 2), 0.0f);
    EXPECT_EQ(e1(0, 3), 0.0f);
    EXPECT_EQ(e1(1, 0), 0.0f);
    EXPECT_EQ(e1(1, 1), 0.0f);
    EXPECT_EQ(e1(1, 2), 0.0f);
    EXPECT_EQ(e1(1, 3), 0.0f);
    EXPECT_EQ(e1(2, 0), 0.0f);
    EXPECT_EQ(e1(2, 1), 0.0f);
    EXPECT_EQ(e1(2, 2), 0.0f);
    EXPECT_EQ(e1(2, 3), 0.0f);
    EXPECT_EQ(e1(3, 0), 0.0f);
    EXPECT_EQ(e1(3, 1), 0.0f);
    EXPECT_EQ(e1(3, 2), 0.0f);
    EXPECT_EQ(e1(3, 3), 0.0f);

    //- Verify expected equality of default-constructed engines.
    //
    EXPECT_TRUE(m_cmp_eq(e1, e1));
    EXPECT_TRUE(m_cmp_eq(e1, e2));
    EXPECT_TRUE(m_cmp_eq(e1, e3));

    //- Verify equality against an init-list of equivalent values.
    //
    EXPECT_TRUE(m_cmp_eq(e1, LST_44_0));
    EXPECT_TRUE(m_cmp_eq(e1, il_44_0));
    EXPECT_TRUE(m_cmp_eq(e1, fl_44_0));

    //- Verify inequality against an init-list of different values.
    //
    EXPECT_FALSE(m_cmp_eq(e1, LST_44_1));
    EXPECT_FALSE(m_cmp_eq(e1, il_44_1));
    EXPECT_FALSE(m_cmp_eq(e1, fl_44_1));

    //- Assign new element values via mutable indexing and verify them.
    //
    e2(0, 0) = 11.0f;
    e2(0, 1) = 12.0f;
    e2(0, 2) = 13.0f;
    e2(0, 3) = 14.0f;
    e2(1, 0) = 21.0f;
    e2(1, 1) = 22.0f;
    e2(1, 2) = 23.0f;
    e2(1, 3) = 24.0f;
    e2(2, 0) = 31.0f;
    e2(2, 1) = 32.0f;
    e2(2, 2) = 33.0f;
    e2(2, 3) = 34.0f;
    e2(3, 0) = 41.0f;
    e2(3, 1) = 42.0f;
    e2(3, 2) = 43.0f;
    e2(3, 3) = 44.0f;
    EXPECT_EQ(e2(0, 0), 11.0f);
    EXPECT_EQ(e2(0, 1), 12.0f);
    EXPECT_EQ(e2(0, 2), 13.0f);
    EXPECT_EQ(e2(0, 3), 14.0f);
    EXPECT_EQ(e2(1, 0), 21.0f);
    EXPECT_EQ(e2(1, 1), 22.0f);
    EXPECT_EQ(e2(1, 2), 23.0f);
    EXPECT_EQ(e2(1, 3), 24.0f);
    EXPECT_EQ(e2(2, 0), 31.0f);
    EXPECT_EQ(e2(2, 1), 32.0f);
    EXPECT_EQ(e2(2, 2), 33.0f);
    EXPECT_EQ(e2(2, 3), 34.0f);
    EXPECT_EQ(e2(3, 0), 41.0f);
    EXPECT_EQ(e2(3, 1), 42.0f);
    EXPECT_EQ(e2(3, 2), 43.0f);
    EXPECT_EQ(e2(3, 3), 44.0f);
    EXPECT_FALSE(m_cmp_eq(e1, e2));

    e3(0, 0) = 11.0f;
    e3(0, 1) = 12.0f;
    e3(0, 2) = 13.0f;
    e3(0, 3) = 14.0f;
    e3(1, 0) = 21.0f;
    e3(1, 1) = 22.0f;
    e3(1, 2) = 23.0f;
    e3(1, 3) = 24.0f;
    e3(2, 0) = 31.0f;
    e3(2, 1) = 32.0f;
    e3(2, 2) = 33.0f;
    e3(2, 3) = 34.0f;
    e3(3, 0) = 41.0f;
    e3(3, 1) = 42.0f;
    e3(3, 2) = 43.0f;
    e3(3, 3) = 44.0f;
    EXPECT_EQ(e3(0, 0), 11.0f);
    EXPECT_EQ(e3(0, 1), 12.0f);
    EXPECT_EQ(e3(0, 2), 13.0f);
    EXPECT_EQ(e3(0, 3), 14.0f);
    EXPECT_EQ(e3(1, 0), 21.0f);
    EXPECT_EQ(e3(1, 1), 22.0f);
    EXPECT_EQ(e3(1, 2), 23.0f);
    EXPECT_EQ(e3(1, 3), 24.0f);
    EXPECT_EQ(e3(2, 0), 31.0f);
    EXPECT_EQ(e3(2, 1), 32.0f);
    EXPECT_EQ(e3(2, 2), 33.0f);
    EXPECT_EQ(e3(2, 3), 34.0f);
    EXPECT_EQ(e3(3, 0), 41.0f);
    EXPECT_EQ(e3(3, 1), 42.0f);
    EXPECT_EQ(e3(3, 2), 43.0f);
    EXPECT_EQ(e3(3, 3), 44.0f);
    EXPECT_FALSE(m_cmp_eq(e1, e3));

    //- Verify expected equality of various argument combinations.
    //
    EXPECT_TRUE(m_cmp_eq(e2, e3));

    EXPECT_TRUE(m_cmp_eq(e2, LST_44_2));
    EXPECT_TRUE(m_cmp_eq(e2, il_44_2));
    EXPECT_TRUE(m_cmp_eq(e2, fl_44_2));

    EXPECT_TRUE(m_cmp_eq(e3, LST_44_2));
    EXPECT_TRUE(m_cmp_eq(e3, il_44_2));
    EXPECT_TRUE(m_cmp_eq(e3, fl_44_2));

    EXPECT_TRUE(m_cmp_eq(e2, e3));
    EXPECT_FALSE(m_cmp_eq(e2, e1));
    EXPECT_FALSE(m_cmp_eq(e1, e3));

    //- Verify expected inequality when compared to init-lists of incorrect size.
    //
    EXPECT_FALSE(m_cmp_eq(e3, LST_33_0));
    EXPECT_FALSE(m_cmp_eq(e3, il_33_1));
    EXPECT_FALSE(m_cmp_eq(e3, fl_33_2));

    //- Verify expected inequality against init-lists and engines having different contents.
    //
    EXPECT_FALSE(m_cmp_eq(e3, fl_44_0));
    EXPECT_FALSE(m_cmp_eq(e3, il_44_1));
    EXPECT_FALSE(m_cmp_eq(e3, mse_f_ffa_rm(LST_44_1)));
}


TEST(MSE_Matrix_4B, MoveCtor)
{
    //- Default construct and verify initial state.
    //
    mse_f_ffa_rm    e1;

    EXPECT_EQ(e1.rows(), 4);
    EXPECT_EQ(e1.columns(), 4);
    EXPECT_EQ(e1.row_capacity(), 4);
    EXPECT_EQ(e1.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e1, LST_44_0));

    //- Assign new element values and verify them.
    //
    e1(0, 0) = 11.0f;
    e1(0, 1) = 12.0f;
    e1(0, 2) = 13.0f;
    e1(0, 3) = 14.0f;
    e1(1, 0) = 21.0f;
    e1(1, 1) = 22.0f;
    e1(1, 2) = 23.0f;
    e1(1, 3) = 24.0f;
    e1(2, 0) = 31.0f;
    e1(2, 1) = 32.0f;
    e1(2, 2) = 33.0f;
    e1(2, 3) = 34.0f;
    e1(3, 0) = 41.0f;
    e1(3, 1) = 42.0f;
    e1(3, 2) = 43.0f;
    e1(3, 3) = 44.0f;
    EXPECT_TRUE(m_cmp_eq(e1, fl_44_2));

    //- Construct a new engine via move ctor and verify that its initial state is as expected.
    //
    mse_f_ffa_rm    e2(std::move(e1));

    EXPECT_EQ(e1.rows(), 4);
    EXPECT_EQ(e1.columns(), 4);
    EXPECT_EQ(e2.row_capacity(), 4);
    EXPECT_EQ(e2.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e2, fl_44_2));
}


TEST(MSE_Matrix_4B, CopyCtor)
{
    //- Default construct and verify initial state.
    //
    mse_f_ffa_rm    e1;
    EXPECT_EQ(e1.rows(), 4);
    EXPECT_EQ(e1.columns(), 4);
    EXPECT_EQ(e1.row_capacity(), 4);
    EXPECT_EQ(e1.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e1, LST_44_0));

    //- Assign new element values and verify them.
    //
    e1(0, 0) = 11.0f;
    e1(0, 1) = 12.0f;
    e1(0, 2) = 13.0f;
    e1(0, 3) = 14.0f;
    e1(1, 0) = 21.0f;
    e1(1, 1) = 22.0f;
    e1(1, 2) = 23.0f;
    e1(1, 3) = 24.0f;
    e1(2, 0) = 31.0f;
    e1(2, 1) = 32.0f;
    e1(2, 2) = 33.0f;
    e1(2, 3) = 34.0f;
    e1(3, 0) = 41.0f;
    e1(3, 1) = 42.0f;
    e1(3, 2) = 43.0f;
    e1(3, 3) = 44.0f;
    EXPECT_TRUE(m_cmp_eq(e1, fl_44_2));

    //- Construct new engines via copy ctors and verify their initial states.
    //
    mse_f_ffa_rm    e2(e1);

    EXPECT_EQ(e1.rows(), 4);
    EXPECT_EQ(e1.columns(), 4);
    EXPECT_EQ(e2.row_capacity(), 4);
    EXPECT_EQ(e2.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e2, e1));

    mse_f_ffa_rm    e3 = e1;

    EXPECT_EQ(e1.rows(), 4);
    EXPECT_EQ(e1.columns(), 4);
    EXPECT_EQ(e3.row_capacity(), 4);
    EXPECT_EQ(e3.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e3, e1));

    mse_f_ffa_rm    e4{e1};

    EXPECT_EQ(e1.rows(), 4);
    EXPECT_EQ(e1.columns(), 4);
    EXPECT_EQ(e4.row_capacity(), 4);
    EXPECT_EQ(e4.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e4, e1));
}


TEST(MSE_Matrix_4B, ListCtor)
{
    //- Construct new engines via list ctor and verify that their initial states.
    //
    mse_f_ffa_rm    e1 = LST_44_1;
    mse_f_ffa_rm    e2 LST_44_1 ;
    mse_f_ffa_rm    e3(LST_44_1);

    EXPECT_EQ(e1.rows(), 4);
    EXPECT_EQ(e1.columns(), 4);
    EXPECT_EQ(e1.row_capacity(), 4);
    EXPECT_EQ(e1.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e1, LST_44_1));

    EXPECT_EQ(e2.rows(), 4);
    EXPECT_EQ(e2.columns(), 4);
    EXPECT_EQ(e2.row_capacity(), 4);
    EXPECT_EQ(e2.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e2, e1));

    EXPECT_EQ(e3.rows(), 4);
    EXPECT_EQ(e3.columns(), 4);
    EXPECT_EQ(e3.row_capacity(), 4);
    EXPECT_EQ(e3.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e3, e1));
}


TEST(MSE_Matrix_4B, EngineCtor)
{
    //- Construct new engines via direct engine ctor and verify their initial states.
    //
    mse_i_ffa_cm    e1 = LST_44_2;
    mse_f_ffa_rm    e2(e1);
    mse_d_ffa_rm    e3(e2);
    mse_f_ffa_cm    e4 = LST_44_2;
    mse_f_ffa_rm    e5{e4};

    EXPECT_EQ(e1.rows(), 4);
    EXPECT_EQ(e1.columns(), 4);
    EXPECT_EQ(e1.row_capacity(), 4);
    EXPECT_EQ(e1.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e1, il_44_2));

    EXPECT_EQ(e2.rows(), 4);
    EXPECT_EQ(e2.columns(), 4);
    EXPECT_EQ(e2.row_capacity(), 4);
    EXPECT_EQ(e2.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e2, e1));

    EXPECT_EQ(e3.rows(), 4);
    EXPECT_EQ(e3.columns(), 4);
    EXPECT_EQ(e3.row_capacity(), 4);
    EXPECT_EQ(e3.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e3, e1));

    EXPECT_EQ(e4.rows(), 4);
    EXPECT_EQ(e4.columns(), 4);
    EXPECT_EQ(e4.row_capacity(), 4);
    EXPECT_EQ(e4.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e4, e1));

    EXPECT_EQ(e5.rows(), 4);
    EXPECT_EQ(e5.columns(), 4);
    EXPECT_EQ(e5.row_capacity(), 4);
    EXPECT_EQ(e5.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e5, e1));
}


TEST(MSE_Matrix_4B, MoveAssign)
{
    //- Default construct and verify initial state.
    //
    mse_f_ffa_rm    e1;

    EXPECT_EQ(e1.rows(), 4);
    EXPECT_EQ(e1.columns(), 4);
    EXPECT_EQ(e1.row_capacity(), 4);
    EXPECT_EQ(e1.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e1, il_44_0));

    //- List construct and verify initial state.
    //
    mse_f_ffa_rm    e2(il_44_1);

    EXPECT_EQ(e2.rows(), 4);
    EXPECT_EQ(e2.columns(), 4);
    EXPECT_EQ(e2.row_capacity(), 4);
    EXPECT_EQ(e2.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e2, fl_44_1));

    //- Move-assign and verify.
    //
    e1 = std::move(e2);
    EXPECT_TRUE(m_cmp_eq(e1, fl_44_1));
}


TEST(MSE_Matrix_4B, CopyAssign)
{
    //- Default construct and verify initial state.
    //
    mse_f_ffa_rm    e1;

    EXPECT_EQ(e1.rows(), 4);
    EXPECT_EQ(e1.columns(), 4);
    EXPECT_EQ(e1.row_capacity(), 4);
    EXPECT_EQ(e1.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e1, il_44_0));

    //- List construct and verify initial state.
    //
    mse_f_ffa_rm    e2(il_44_1);

    EXPECT_EQ(e2.rows(), 4);
    EXPECT_EQ(e2.columns(), 4);
    EXPECT_EQ(e2.row_capacity(), 4);
    EXPECT_EQ(e2.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e2, fl_44_1));

    //- Copy-assign and verify.
    //
    e1 = e2;
    EXPECT_TRUE(m_cmp_eq(e1, fl_44_1));
}


TEST(MSE_Matrix_4B, EngineAssign)
{
    //- Default construct and verify initial state.
    //
    mse_f_ffa_rm    e1, e2;

    EXPECT_EQ(e1.rows(), 4);
    EXPECT_EQ(e1.columns(), 4);
    EXPECT_EQ(e1.row_capacity(), 4);
    EXPECT_EQ(e1.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e1, LST_44_0));

    EXPECT_EQ(e2.rows(), 4);
    EXPECT_EQ(e2.columns(), 4);
    EXPECT_EQ(e2.row_capacity(), 4);
    EXPECT_EQ(e2.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e2, fl_44_0));

    //- List construct and verify initial state.
    //
    mse_i_ffa_cm    e3 LST_44_1 ;

    EXPECT_EQ(e3.rows(), 4);
    EXPECT_EQ(e3.columns(), 4);
    EXPECT_EQ(e3.row_capacity(), 4);
    EXPECT_EQ(e3.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e3, il_44_1));

    mse_i_ffa_cm    e4 LST_44_2 ;

    EXPECT_EQ(e4.rows(), 4);
    EXPECT_EQ(e4.columns(), 4);
    EXPECT_EQ(e4.row_capacity(), 4);
    EXPECT_EQ(e4.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e4, il_44_2));

    EXPECT_FALSE(m_cmp_eq(e3, e4));

    //- Assign and verify.
    //
    e1 = e3;
    EXPECT_TRUE(m_cmp_eq(e1, fl_44_1));
    EXPECT_TRUE(m_cmp_eq(e1, e3));

    e2 = e4;
    EXPECT_TRUE(m_cmp_eq(e2, fl_44_2));
    EXPECT_TRUE(m_cmp_eq(e2, e4));
}


TEST(MSE_Matrix_4B, ListAssign)
{
    //- Default construct and verify initial state.
    //
    mse_f_ffa_rm     e1;
    mse_f_ffa_cm     e2{il_44_1};

    EXPECT_EQ(e1.rows(), 4);
    EXPECT_EQ(e1.columns(), 4);
    EXPECT_EQ(e1.row_capacity(), 4);
    EXPECT_EQ(e1.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e1, LST_44_0));

    EXPECT_EQ(e2.rows(), 4);
    EXPECT_EQ(e2.columns(), 4);
    EXPECT_EQ(e2.row_capacity(), 4);
    EXPECT_EQ(e2.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e2, LST_44_1));

    //- Assign and verify.
    //
    e1 = LST_44_1;
    EXPECT_TRUE(m_cmp_eq(e1, LST_44_1));
    EXPECT_TRUE(m_cmp_eq(e1, il_44_1));
    EXPECT_TRUE(m_cmp_eq(e1, fl_44_1));
    EXPECT_TRUE(m_cmp_eq(e1, e2));

    e1 = fl_44_0;
    EXPECT_TRUE(m_cmp_eq(e1, LST_44_0));
    EXPECT_TRUE(m_cmp_eq(e1, il_44_0));
    EXPECT_TRUE(m_cmp_eq(e1, fl_44_0));
    EXPECT_TRUE(m_cmp_eq(e1, mse_f_ffa_rm()));
}


TEST(MSE_Matrix_4B, Swap)
{
    //- Default construct and verify initial state.
    //
    mse_f_ffa_rm    e1;

    EXPECT_EQ(e1.size(), st_44);
    EXPECT_TRUE(m_cmp_eq(e1, fl_44_0));

    //- List construct and verify initial state.
    //
    mse_f_ffa_rm    e2(fl_44_2);

    EXPECT_EQ(e2.size(), st_44);
    EXPECT_TRUE(m_cmp_eq(e2, fl_44_2));

    //- Swap contents and verify.
    //
    e1.swap(e2);
    EXPECT_TRUE(m_cmp_eq(e1, fl_44_2));
    EXPECT_TRUE(m_cmp_eq(e2, fl_44_0));

    e2.swap(e1);
    EXPECT_TRUE(m_cmp_eq(e1, il_44_0));
    EXPECT_TRUE(m_cmp_eq(e2, il_44_2));

    //- Swap some elements and verify.
    //
    e2.swap_columns(0, 2);
    EXPECT_TRUE(m_cmp_eq(e2, LST_44_2CS));

    e2 = LST_44_2;
    EXPECT_TRUE(m_cmp_eq(e2, LST_44_2));

    e2.swap_rows(1, 2);
    EXPECT_TRUE(m_cmp_eq(e2, LST_44_2RS));
}


TEST(MSE_Matrix_4B, Span)
{
    mse_f_ffa_rm        e1(il_44_2), e2, e3{e1};
    mse_f_ffa_rm const& ce1 = e1;

    auto    sp1  = e1.span();
    auto    csp1 = ce1.span();

    //- Const and mutable spans should have different types.
    //
    EXPECT_FALSE((std::is_same_v<decltype(sp1), decltype(csp1)>));

    //- The elements of the const and mutable spans should have the expected values.
    //
    EXPECT_EQ(sp1(0, 0), 11.0f);
    EXPECT_EQ(sp1(0, 1), 12.0f);
    EXPECT_EQ(sp1(0, 2), 13.0f);
    EXPECT_EQ(sp1(0, 3), 14.0f);
    EXPECT_EQ(sp1(1, 0), 21.0f);
    EXPECT_EQ(sp1(1, 1), 22.0f);
    EXPECT_EQ(sp1(1, 2), 23.0f);
    EXPECT_EQ(sp1(1, 3), 24.0f);
    EXPECT_EQ(sp1(2, 0), 31.0f);
    EXPECT_EQ(sp1(2, 1), 32.0f);
    EXPECT_EQ(sp1(2, 2), 33.0f);
    EXPECT_EQ(sp1(2, 3), 34.0f);
    EXPECT_EQ(sp1(3, 0), 41.0f);
    EXPECT_EQ(sp1(3, 1), 42.0f);
    EXPECT_EQ(sp1(3, 2), 43.0f);
    EXPECT_EQ(sp1(3, 3), 44.0f);

    EXPECT_EQ(csp1(0, 0), 11.0f);
    EXPECT_EQ(csp1(0, 1), 12.0f);
    EXPECT_EQ(csp1(0, 2), 13.0f);
    EXPECT_EQ(csp1(0, 3), 14.0f);
    EXPECT_EQ(csp1(1, 0), 21.0f);
    EXPECT_EQ(csp1(1, 1), 22.0f);
    EXPECT_EQ(csp1(1, 2), 23.0f);
    EXPECT_EQ(csp1(1, 3), 24.0f);
    EXPECT_EQ(csp1(2, 0), 31.0f);
    EXPECT_EQ(csp1(2, 1), 32.0f);
    EXPECT_EQ(csp1(2, 2), 33.0f);
    EXPECT_EQ(csp1(2, 3), 34.0f);
    EXPECT_EQ(csp1(3, 0), 41.0f);
    EXPECT_EQ(csp1(3, 1), 42.0f);
    EXPECT_EQ(csp1(3, 2), 43.0f);
    EXPECT_EQ(csp1(3, 3), 44.0f);

    //- Whole-object comparison between engines and spans should work as expected.
    //
    EXPECT_TRUE(m_cmp_eq(e1, sp1));
    EXPECT_TRUE(m_cmp_eq(e1, csp1));
    EXPECT_TRUE(m_cmp_eq(e3, sp1));
    EXPECT_TRUE(m_cmp_eq(e3, csp1));
    EXPECT_FALSE(m_cmp_eq(e2, sp1));
    EXPECT_FALSE(m_cmp_eq(e2, csp1));

    //- Setting values of individual span elements should be reflected in the owning engine.
    //
    sp1(0, 0) = 111.0f;
    sp1(1, 1) = 222.0f;
    sp1(2, 2) = 333.0f;
    sp1(3, 3) = 444.0f;
    EXPECT_EQ(e1(0, 0), 111.0f);
    EXPECT_EQ(e1(1, 1), 222.0f);
    EXPECT_EQ(e1(2, 2), 333.0f);
    EXPECT_EQ(e1(3, 3), 444.0f);
}


//==================================================================================================
//  Unit tests for dynamically-resizeable matrix engine, Case 4C
//==================================================================================================
//
using mse_f_df_cm = matrix_storage_engine<float, extents<dynamic_extent, 4>, allocator<float>, column_major>;
using mse_f_df_rm = matrix_storage_engine<float, extents<dynamic_extent, 4>, allocator<float>, row_major>;
using mse_i_df_cm = matrix_storage_engine<int, extents<dynamic_extent, 4>, allocator<int>, column_major>;
using mse_d_df_rm = matrix_storage_engine<double, extents<dynamic_extent, 4>, allocator<double>, column_major>;

TEST(MSE_Matrix_4C, DefaultCtor)
{
    mse_f_df_rm     e1;

    EXPECT_EQ(e1.rows(), 0);
    EXPECT_EQ(e1.columns(), 4);
    EXPECT_EQ(e1.row_capacity(), 0);
    EXPECT_EQ(e1.column_capacity(), 4);
}


TEST(MSE_Matrix_4C, DirectCtor)
{
    mse_f_df_rm     e1;

    e1.reshape_rows(4, 0);
    EXPECT_EQ(e1.rows(), 4);
    EXPECT_EQ(e1.columns(), 4);
    EXPECT_EQ(e1.size(), st_44);
    EXPECT_EQ(e1.row_capacity(), 4);
    EXPECT_EQ(e1.column_capacity(), 4);

    //- Verify elements are value initialized.
    //
    EXPECT_EQ(e1(0, 0), 0.0f);
    EXPECT_EQ(e1(0, 1), 0.0f);
    EXPECT_EQ(e1(0, 2), 0.0f);
    EXPECT_EQ(e1(0, 3), 0.0f);
    EXPECT_EQ(e1(1, 0), 0.0f);
    EXPECT_EQ(e1(1, 1), 0.0f);
    EXPECT_EQ(e1(1, 2), 0.0f);
    EXPECT_EQ(e1(1, 3), 0.0f);
    EXPECT_EQ(e1(2, 0), 0.0f);
    EXPECT_EQ(e1(2, 1), 0.0f);
    EXPECT_EQ(e1(2, 2), 0.0f);
    EXPECT_EQ(e1(2, 3), 0.0f);
    EXPECT_EQ(e1(3, 0), 0.0f);
    EXPECT_EQ(e1(3, 1), 0.0f);
    EXPECT_EQ(e1(3, 2), 0.0f);
    EXPECT_EQ(e1(3, 3), 0.0f);
}


TEST(MSE_Matrix_4C, CmpEq)
{
    mse_f_df_rm     e1, e2, e3;

    e1.reshape_rows(4, 0);
    e2.reshape_rows(4, 0);
    e3.reshape_rows(4, 0);

    //- Verify size, capacity, initial values.
    //
    EXPECT_EQ(e1.rows(), 4);
    EXPECT_EQ(e1.columns(), 4);
    EXPECT_EQ(e1.row_capacity(), 4);
    EXPECT_EQ(e1.column_capacity(), 4);

    EXPECT_EQ(e1(0, 0), 0.0f);
    EXPECT_EQ(e1(0, 1), 0.0f);
    EXPECT_EQ(e1(0, 2), 0.0f);
    EXPECT_EQ(e1(0, 3), 0.0f);
    EXPECT_EQ(e1(1, 0), 0.0f);
    EXPECT_EQ(e1(1, 1), 0.0f);
    EXPECT_EQ(e1(1, 2), 0.0f);
    EXPECT_EQ(e1(1, 3), 0.0f);
    EXPECT_EQ(e1(2, 0), 0.0f);
    EXPECT_EQ(e1(2, 1), 0.0f);
    EXPECT_EQ(e1(2, 2), 0.0f);
    EXPECT_EQ(e1(2, 3), 0.0f);
    EXPECT_EQ(e1(3, 0), 0.0f);
    EXPECT_EQ(e1(3, 1), 0.0f);
    EXPECT_EQ(e1(3, 2), 0.0f);
    EXPECT_EQ(e1(3, 3), 0.0f);

    //- Verify expected equality of default-constructed engines.
    //
    EXPECT_TRUE(m_cmp_eq(e1, e1));
    EXPECT_TRUE(m_cmp_eq(e1, e2));
    EXPECT_TRUE(m_cmp_eq(e1, e3));

    //- Verify equality against an init-list of equivalent values.
    //
    EXPECT_TRUE(m_cmp_eq(e1, LST_44_0));
    EXPECT_TRUE(m_cmp_eq(e1, il_44_0));
    EXPECT_TRUE(m_cmp_eq(e1, fl_44_0));

    //- Verify inequality against an init-list of different values.
    //
    EXPECT_FALSE(m_cmp_eq(e1, LST_44_1));
    EXPECT_FALSE(m_cmp_eq(e1, il_44_1));
    EXPECT_FALSE(m_cmp_eq(e1, fl_44_1));

    //- Assign new element values via mutable indexing and verify them.
    //
    e2(0, 0) = 11.0f;
    e2(0, 1) = 12.0f;
    e2(0, 2) = 13.0f;
    e2(0, 3) = 14.0f;
    e2(1, 0) = 21.0f;
    e2(1, 1) = 22.0f;
    e2(1, 2) = 23.0f;
    e2(1, 3) = 24.0f;
    e2(2, 0) = 31.0f;
    e2(2, 1) = 32.0f;
    e2(2, 2) = 33.0f;
    e2(2, 3) = 34.0f;
    e2(3, 0) = 41.0f;
    e2(3, 1) = 42.0f;
    e2(3, 2) = 43.0f;
    e2(3, 3) = 44.0f;
    EXPECT_EQ(e2(0, 0), 11.0f);
    EXPECT_EQ(e2(0, 1), 12.0f);
    EXPECT_EQ(e2(0, 2), 13.0f);
    EXPECT_EQ(e2(0, 3), 14.0f);
    EXPECT_EQ(e2(1, 0), 21.0f);
    EXPECT_EQ(e2(1, 1), 22.0f);
    EXPECT_EQ(e2(1, 2), 23.0f);
    EXPECT_EQ(e2(1, 3), 24.0f);
    EXPECT_EQ(e2(2, 0), 31.0f);
    EXPECT_EQ(e2(2, 1), 32.0f);
    EXPECT_EQ(e2(2, 2), 33.0f);
    EXPECT_EQ(e2(2, 3), 34.0f);
    EXPECT_EQ(e2(3, 0), 41.0f);
    EXPECT_EQ(e2(3, 1), 42.0f);
    EXPECT_EQ(e2(3, 2), 43.0f);
    EXPECT_EQ(e2(3, 3), 44.0f);
    EXPECT_FALSE(m_cmp_eq(e1, e2));

    e3(0, 0) = 11.0f;
    e3(0, 1) = 12.0f;
    e3(0, 2) = 13.0f;
    e3(0, 3) = 14.0f;
    e3(1, 0) = 21.0f;
    e3(1, 1) = 22.0f;
    e3(1, 2) = 23.0f;
    e3(1, 3) = 24.0f;
    e3(2, 0) = 31.0f;
    e3(2, 1) = 32.0f;
    e3(2, 2) = 33.0f;
    e3(2, 3) = 34.0f;
    e3(3, 0) = 41.0f;
    e3(3, 1) = 42.0f;
    e3(3, 2) = 43.0f;
    e3(3, 3) = 44.0f;
    EXPECT_EQ(e3(0, 0), 11.0f);
    EXPECT_EQ(e3(0, 1), 12.0f);
    EXPECT_EQ(e3(0, 2), 13.0f);
    EXPECT_EQ(e3(0, 3), 14.0f);
    EXPECT_EQ(e3(1, 0), 21.0f);
    EXPECT_EQ(e3(1, 1), 22.0f);
    EXPECT_EQ(e3(1, 2), 23.0f);
    EXPECT_EQ(e3(1, 3), 24.0f);
    EXPECT_EQ(e3(2, 0), 31.0f);
    EXPECT_EQ(e3(2, 1), 32.0f);
    EXPECT_EQ(e3(2, 2), 33.0f);
    EXPECT_EQ(e3(2, 3), 34.0f);
    EXPECT_EQ(e3(3, 0), 41.0f);
    EXPECT_EQ(e3(3, 1), 42.0f);
    EXPECT_EQ(e3(3, 2), 43.0f);
    EXPECT_EQ(e3(3, 3), 44.0f);
    EXPECT_FALSE(m_cmp_eq(e1, e3));

    //- Verify expected equality of various argument combinations.
    //
    EXPECT_TRUE(m_cmp_eq(e2, e3));

    EXPECT_TRUE(m_cmp_eq(e2, LST_44_2));
    EXPECT_TRUE(m_cmp_eq(e2, il_44_2));
    EXPECT_TRUE(m_cmp_eq(e2, fl_44_2));

    EXPECT_TRUE(m_cmp_eq(e3, LST_44_2));
    EXPECT_TRUE(m_cmp_eq(e3, il_44_2));
    EXPECT_TRUE(m_cmp_eq(e3, fl_44_2));

    EXPECT_TRUE(m_cmp_eq(e2, e3));
    EXPECT_FALSE(m_cmp_eq(e2, e1));
    EXPECT_FALSE(m_cmp_eq(e1, e3));

    //- Verify expected inequality when compared to init-lists of incorrect size.
    //
    EXPECT_FALSE(m_cmp_eq(e3, LST_33_0));
    EXPECT_FALSE(m_cmp_eq(e3, il_33_1));
    EXPECT_FALSE(m_cmp_eq(e3, fl_33_2));

    //- Verify expected inequality against init-lists and engines having different contents.
    //
    EXPECT_FALSE(m_cmp_eq(e3, fl_44_0));
    EXPECT_FALSE(m_cmp_eq(e3, il_44_1));
}


TEST(MSE_Matrix_4C, MoveCtor)
{
    //- Default construct and verify initial state.
    //
    mse_f_df_rm     e1;

    e1.reshape_rows(4, 0);
    EXPECT_EQ(e1.rows(), 4);
    EXPECT_EQ(e1.columns(), 4);
    EXPECT_EQ(e1.row_capacity(), 4);
    EXPECT_EQ(e1.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e1, LST_44_0));

    //- Assign new element values and verify them.
    //
    e1(0, 0) = 11.0f;
    e1(0, 1) = 12.0f;
    e1(0, 2) = 13.0f;
    e1(0, 3) = 14.0f;
    e1(1, 0) = 21.0f;
    e1(1, 1) = 22.0f;
    e1(1, 2) = 23.0f;
    e1(1, 3) = 24.0f;
    e1(2, 0) = 31.0f;
    e1(2, 1) = 32.0f;
    e1(2, 2) = 33.0f;
    e1(2, 3) = 34.0f;
    e1(3, 0) = 41.0f;
    e1(3, 1) = 42.0f;
    e1(3, 2) = 43.0f;
    e1(3, 3) = 44.0f;
    EXPECT_TRUE(m_cmp_eq(e1, fl_44_2));

    //- Construct a new engine via move ctor and verify that its initial state is as expected.
    //
    mse_f_df_rm     e2(std::move(e1));

    EXPECT_EQ(e2.rows(), 4);
    EXPECT_EQ(e2.columns(), 4);
    EXPECT_EQ(e2.row_capacity(), 4);
    EXPECT_EQ(e2.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e2, fl_44_2));
}


TEST(MSE_Matrix_4C, CopyCtor)
{
    //- Default construct and verify initial state.
    //
    mse_f_df_rm     e1;

    e1.reshape_rows(4, 0);
    EXPECT_EQ(e1.rows(), 4);
    EXPECT_EQ(e1.columns(), 4);
    EXPECT_EQ(e1.row_capacity(), 4);
    EXPECT_EQ(e1.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e1, LST_44_0));

    //- Assign new element values and verify them.
    //
    e1(0, 0) = 11.0f;
    e1(0, 1) = 12.0f;
    e1(0, 2) = 13.0f;
    e1(0, 3) = 14.0f;
    e1(1, 0) = 21.0f;
    e1(1, 1) = 22.0f;
    e1(1, 2) = 23.0f;
    e1(1, 3) = 24.0f;
    e1(2, 0) = 31.0f;
    e1(2, 1) = 32.0f;
    e1(2, 2) = 33.0f;
    e1(2, 3) = 34.0f;
    e1(3, 0) = 41.0f;
    e1(3, 1) = 42.0f;
    e1(3, 2) = 43.0f;
    e1(3, 3) = 44.0f;
    EXPECT_TRUE(m_cmp_eq(e1, fl_44_2));

    //- Construct new engines via copy ctors and verify their initial states.
    //
    mse_f_df_rm     e2(e1);

    EXPECT_EQ(e2.rows(), 4);
    EXPECT_EQ(e2.columns(), 4);
    EXPECT_EQ(e2.row_capacity(), 4);
    EXPECT_EQ(e2.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e2, e1));

    mse_f_df_rm     e3 = e1;

    EXPECT_EQ(e3.rows(), 4);
    EXPECT_EQ(e3.columns(), 4);
    EXPECT_EQ(e3.row_capacity(), 4);
    EXPECT_EQ(e3.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e3, e1));

    mse_f_df_rm     e4{e1};

    EXPECT_EQ(e4.rows(), 4);
    EXPECT_EQ(e4.columns(), 4);
    EXPECT_EQ(e4.size(), st_44);
    EXPECT_EQ(e4.row_capacity(), 4);
    EXPECT_EQ(e4.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e4, e1));
}


TEST(MSE_Matrix_4C, ListCtor)
{
    //- Construct new engines via list ctor and verify that their initial states.
    //
    mse_f_df_rm     e1 = LST_44_1;
    mse_f_df_rm     e2 LST_44_1 ;
    mse_f_df_rm     e3(LST_44_1);

    EXPECT_EQ(e1.rows(), 4);
    EXPECT_EQ(e1.columns(), 4);
    EXPECT_EQ(e1.row_capacity(), 4);
    EXPECT_EQ(e1.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e1, LST_44_1));

    EXPECT_EQ(e2.rows(), 4);
    EXPECT_EQ(e2.columns(), 4);
    EXPECT_EQ(e2.row_capacity(), 4);
    EXPECT_EQ(e2.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e2, e1));

    EXPECT_EQ(e3.rows(), 4);
    EXPECT_EQ(e3.columns(), 4);
    EXPECT_EQ(e3.row_capacity(), 4);
    EXPECT_EQ(e3.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e3, e1));
}


TEST(MSE_Matrix_4C, EngineCtor)
{
    //- Construct new engines via direct engine ctor and verify their initial states.
    //
    mse_i_df_cm     e1 = LST_44_2;
    mse_f_df_rm     e2(e1);
    mse_d_df_rm     e3(e2);
    mse_f_df_cm     e4 = LST_44_2;
    mse_f_df_rm     e5{e4};

    EXPECT_EQ(e1.rows(), 4);
    EXPECT_EQ(e1.columns(), 4);
    EXPECT_EQ(e1.row_capacity(), 4);
    EXPECT_EQ(e1.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e1, il_44_2));

    EXPECT_EQ(e2.rows(), 4);
    EXPECT_EQ(e2.columns(), 4);
    EXPECT_EQ(e2.row_capacity(), 4);
    EXPECT_EQ(e2.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e2, e1));

    EXPECT_EQ(e3.rows(), 4);
    EXPECT_EQ(e3.columns(), 4);
    EXPECT_EQ(e3.row_capacity(), 4);
    EXPECT_EQ(e3.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e3, e1));

    EXPECT_EQ(e4.rows(), 4);
    EXPECT_EQ(e4.columns(), 4);
    EXPECT_EQ(e4.row_capacity(), 4);
    EXPECT_EQ(e4.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e4, e1));

    EXPECT_EQ(e5.rows(), 4);
    EXPECT_EQ(e5.columns(), 4);
    EXPECT_EQ(e5.row_capacity(), 4);
    EXPECT_EQ(e5.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e5, e1));
}


TEST(MSE_Matrix_4C, MoveAssign)
{
    //- Default construct and verify initial state.
    //
    mse_f_df_rm     e1;

    e1.reshape_rows(4, 0);
    EXPECT_EQ(e1.rows(), 4);
    EXPECT_EQ(e1.columns(), 4);
    EXPECT_EQ(e1.row_capacity(), 4);
    EXPECT_EQ(e1.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e1, il_44_0));

    //- List construct and verify initial state.
    //
    mse_f_df_rm     e2(il_44_1);

    EXPECT_EQ(e2.rows(), 4);
    EXPECT_EQ(e2.columns(), 4);
    EXPECT_EQ(e2.row_capacity(), 4);
    EXPECT_EQ(e2.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e2, fl_44_1));

    //- Move-assign and verify.
    //
    e1 = std::move(e2);
    EXPECT_TRUE(m_cmp_eq(e1, fl_44_1));
}


TEST(MSE_Matrix_4C, CopyAssign)
{
    //- Default construct and verify initial state.
    //
    mse_f_df_rm     e1;

    e1.reshape_rows(4, 0);
    EXPECT_EQ(e1.rows(), 4);
    EXPECT_EQ(e1.columns(), 4);
    EXPECT_EQ(e1.row_capacity(), 4);
    EXPECT_EQ(e1.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e1, il_44_0));

    //- List construct and verify initial state.
    //
    mse_f_df_rm     e2(il_44_1);

    EXPECT_EQ(e2.rows(), 4);
    EXPECT_EQ(e2.columns(), 4);
    EXPECT_EQ(e2.row_capacity(), 4);
    EXPECT_EQ(e2.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e2, fl_44_1));

    //- Copy-assign and verify.
    //
    e1 = e2;
    EXPECT_TRUE(m_cmp_eq(e1, fl_44_1));
}


TEST(MSE_Matrix_4C, EngineAssign)
{
    //- Default construct and verify initial state.
    //
    mse_f_df_rm     e1, e2;

    e1.reshape_rows(4, 0);
    EXPECT_EQ(e1.rows(), 4);
    EXPECT_EQ(e1.columns(), 4);
    EXPECT_EQ(e1.row_capacity(), 4);
    EXPECT_EQ(e1.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e1, LST_44_0));

    e2.reshape_rows(4, 0);
    EXPECT_EQ(e2.rows(), 4);
    EXPECT_EQ(e2.columns(), 4);
    EXPECT_EQ(e2.row_capacity(), 4);
    EXPECT_EQ(e2.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e2, fl_44_0));

    //- List construct and verify initial state.
    //
    mse_i_df_cm     e3 LST_44_1 ;

    EXPECT_EQ(e3.rows(), 4);
    EXPECT_EQ(e3.columns(), 4);
    EXPECT_EQ(e3.row_capacity(), 4);
    EXPECT_EQ(e3.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e3, il_44_1));

    fs_matrix_engine<int, 4, 4> e4 LST_44_2 ;

    EXPECT_EQ(e4.rows(), 4);
    EXPECT_EQ(e4.columns(), 4);
    EXPECT_EQ(e4.capacity(), st_44);
    EXPECT_TRUE(m_cmp_eq(e4, il_44_2));

    EXPECT_FALSE(m_cmp_eq(e3, e4));

    //- Assign and verify.
    //
    e1 = e3;
    EXPECT_TRUE(m_cmp_eq(e1, fl_44_1));
    EXPECT_TRUE(m_cmp_eq(e1, e3));

    e2 = e4;
    EXPECT_TRUE(m_cmp_eq(e2, fl_44_2));
    EXPECT_TRUE(m_cmp_eq(e2, e4));
}


TEST(MSE_Matrix_4C, ListAssign)
{
    //- Default construct and verify initial state.
    //
    mse_f_df_rm     e1;
    mse_f_df_rm     e2{il_44_1};

    e1.reshape_rows(4, 0);
    EXPECT_EQ(e1.rows(), 4);
    EXPECT_EQ(e1.columns(), 4);
    EXPECT_EQ(e1.row_capacity(), 4);
    EXPECT_EQ(e1.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e1, LST_44_0));

    EXPECT_EQ(e2.rows(), 4);
    EXPECT_EQ(e2.columns(), 4);
    EXPECT_EQ(e2.row_capacity(), 4);
    EXPECT_EQ(e2.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e2, LST_44_1));

    //- Assign and verify.
    //
    e1 = LST_44_1;
    EXPECT_TRUE(m_cmp_eq(e1, LST_44_1));
    EXPECT_TRUE(m_cmp_eq(e1, il_44_1));
    EXPECT_TRUE(m_cmp_eq(e1, fl_44_1));
    EXPECT_TRUE(m_cmp_eq(e1, e2));

    e1 = fl_44_0;
    EXPECT_TRUE(m_cmp_eq(e1, LST_44_0));
    EXPECT_TRUE(m_cmp_eq(e1, il_44_0));
    EXPECT_TRUE(m_cmp_eq(e1, fl_44_0));
}


TEST(MSE_Matrix_4C, ReshapeRows)
{
    //- List construct and verify initial state.
    //
    mse_f_df_rm      e1 LST_44_2 ;

    EXPECT_EQ(e1.rows(), 4);
    EXPECT_EQ(e1.columns(), 4);
    EXPECT_EQ(e1.row_capacity(), 4);
    EXPECT_EQ(e1.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e1, LST_44_2));

    //- Resize upward and verify.
    //
    e1.reshape_rows(6, e1.row_capacity());
    EXPECT_EQ(e1.rows(), 6);
    EXPECT_EQ(e1.columns(), 4);
    EXPECT_EQ(e1.row_capacity(), 6);
    EXPECT_EQ(e1.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e1, LST_44_2_RR3));

    //- Resize downward and verify.
    //
    e1.reshape_rows(2, e1.row_capacity());
    EXPECT_EQ(e1.rows(), 2);
    EXPECT_EQ(e1.columns(), 4);
    EXPECT_EQ(e1.row_capacity(), 6);
    EXPECT_EQ(e1.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e1, LST_44_2_RR1));

    //- List construct and verify initial state.
    //
    mse_f_df_rm      e2(il_44_2);

    EXPECT_EQ(e2.rows(), 4);
    EXPECT_EQ(e2.columns(), 4);
    EXPECT_EQ(e2.row_capacity(), 4);
    EXPECT_EQ(e2.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e2, il_44_2));

    //- Reserve extra space and verify.
    //
    e2.reshape_rows(e2.rows(), 8);
    EXPECT_EQ(e2.rows(), 4);
    EXPECT_EQ(e2.columns(), 4);
    EXPECT_EQ(e2.row_capacity(), 8);
    EXPECT_EQ(e2.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e2, il_44_2));

    //- Release extra space and verify.
    //
    e2.reshape_rows(e2.rows(), 0);
    EXPECT_EQ(e2.rows(), 4);
    EXPECT_EQ(e2.columns(), 4);
    EXPECT_EQ(e2.row_capacity(), 4);
    EXPECT_EQ(e2.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e2, il_44_2));

    //- List construct and verify initial state.
    //
    mse_f_df_rm      e3 LST_44_2 ;

    EXPECT_EQ(e3.rows(), 4);
    EXPECT_EQ(e3.columns(), 4);
    EXPECT_EQ(e3.row_capacity(), 4);
    EXPECT_EQ(e3.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e3, LST_44_2));

    //- Resize upward and verify.
    //
    e3.reshape_rows(6, 8);
    EXPECT_EQ(e3.rows(), 6);
    EXPECT_EQ(e3.columns(), 4);
    EXPECT_EQ(e3.row_capacity(), 8);
    EXPECT_EQ(e3.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e3, LST_44_2_RR3));

    //- Resize downward and verify.
    //
    e3.reshape_rows(2, 8);
    EXPECT_EQ(e3.rows(), 2);
    EXPECT_EQ(e3.columns(), 4);
    EXPECT_EQ(e3.row_capacity(), 8);
    EXPECT_EQ(e3.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e3, LST_44_2_RR1));
}


TEST(MSE_Matrix_4C, Swap)
{
    //- Default construct and verify initial state.
    //
    mse_f_df_rm     e1;

    e1.reshape_rows(4, 0);
    EXPECT_EQ(e1.rows(), 4);
    EXPECT_EQ(e1.columns(), 4);
    EXPECT_TRUE(m_cmp_eq(e1, fl_44_0));

    //- List construct and verify initial state.
    //
    mse_f_df_rm     e2(fl_44_2);

    EXPECT_EQ(e2.rows(), 4);
    EXPECT_EQ(e2.columns(), 4);
    EXPECT_TRUE(m_cmp_eq(e2, fl_44_2));

    //- Swap contents and verify.
    //
    e1.swap(e2);
    EXPECT_TRUE(m_cmp_eq(e1, fl_44_2));
    EXPECT_TRUE(m_cmp_eq(e2, fl_44_0));

    e2.swap(e1);
    EXPECT_TRUE(m_cmp_eq(e1, il_44_0));
    EXPECT_TRUE(m_cmp_eq(e2, il_44_2));

    //- Swap some elements and verify.
    //
    e2.swap_columns(0, 2);
    EXPECT_TRUE(m_cmp_eq(e2, LST_44_2CS));

    e2 = LST_44_2;
    EXPECT_TRUE(m_cmp_eq(e2, LST_44_2));

    e2.swap_rows(1, 2);
    EXPECT_TRUE(m_cmp_eq(e2, LST_44_2RS));
}


TEST(MSE_Matrix_4C, Span)
{
    mse_f_df_rm         e1(il_44_2), e2, e3{e1};
    mse_f_df_rm const&  ce1 = e1;

    auto    sp1  = e1.span();
    auto    csp1 = ce1.span();

    e2.reshape_rows(4, 0);
    EXPECT_EQ(e2.rows(), 4);
    EXPECT_EQ(e2.columns(), 4);
    EXPECT_TRUE(m_cmp_eq(e2, fl_44_0));

    //- Const and mutable spans should have different types.
    //
    EXPECT_FALSE((std::is_same_v<decltype(sp1), decltype(csp1)>));

    //- The elements of the const and mutable spans should have the expected values.
    //
    EXPECT_EQ(sp1(0, 0), 11.0f);
    EXPECT_EQ(sp1(0, 1), 12.0f);
    EXPECT_EQ(sp1(0, 2), 13.0f);
    EXPECT_EQ(sp1(0, 3), 14.0f);
    EXPECT_EQ(sp1(1, 0), 21.0f);
    EXPECT_EQ(sp1(1, 1), 22.0f);
    EXPECT_EQ(sp1(1, 2), 23.0f);
    EXPECT_EQ(sp1(1, 3), 24.0f);
    EXPECT_EQ(sp1(2, 0), 31.0f);
    EXPECT_EQ(sp1(2, 1), 32.0f);
    EXPECT_EQ(sp1(2, 2), 33.0f);
    EXPECT_EQ(sp1(2, 3), 34.0f);
    EXPECT_EQ(sp1(3, 0), 41.0f);
    EXPECT_EQ(sp1(3, 1), 42.0f);
    EXPECT_EQ(sp1(3, 2), 43.0f);
    EXPECT_EQ(sp1(3, 3), 44.0f);

    EXPECT_EQ(csp1(0, 0), 11.0f);
    EXPECT_EQ(csp1(0, 1), 12.0f);
    EXPECT_EQ(csp1(0, 2), 13.0f);
    EXPECT_EQ(csp1(0, 3), 14.0f);
    EXPECT_EQ(csp1(1, 0), 21.0f);
    EXPECT_EQ(csp1(1, 1), 22.0f);
    EXPECT_EQ(csp1(1, 2), 23.0f);
    EXPECT_EQ(csp1(1, 3), 24.0f);
    EXPECT_EQ(csp1(2, 0), 31.0f);
    EXPECT_EQ(csp1(2, 1), 32.0f);
    EXPECT_EQ(csp1(2, 2), 33.0f);
    EXPECT_EQ(csp1(2, 3), 34.0f);
    EXPECT_EQ(csp1(3, 0), 41.0f);
    EXPECT_EQ(csp1(3, 1), 42.0f);
    EXPECT_EQ(csp1(3, 2), 43.0f);
    EXPECT_EQ(csp1(3, 3), 44.0f);

    //- Whole-object comparison between engines and spans should work as expected.
    //
    EXPECT_TRUE(m_cmp_eq(e1, sp1));
    EXPECT_TRUE(m_cmp_eq(e1, csp1));
    EXPECT_TRUE(m_cmp_eq(e3, sp1));
    EXPECT_TRUE(m_cmp_eq(e3, csp1));
    EXPECT_FALSE(m_cmp_eq(e2, sp1));
    EXPECT_FALSE(m_cmp_eq(e2, csp1));

    //- Setting values of individual span elements should be reflected in the owning engine.
    //
    sp1(0, 0) = 111.0f;
    sp1(1, 1) = 222.0f;
    sp1(2, 2) = 333.0f;
    sp1(3, 3) = 444.0f;
    EXPECT_EQ(e1(0, 0), 111.0f);
    EXPECT_EQ(e1(1, 1), 222.0f);
    EXPECT_EQ(e1(2, 2), 333.0f);
    EXPECT_EQ(e1(3, 3), 444.0f);
}


//==================================================================================================
//  Unit tests for dynamically-resizeable matrix engine, Case 4E
//==================================================================================================
//
using mse_f_dd_cm = matrix_storage_engine<float, extents<dynamic_extent, dynamic_extent>, allocator<float>, column_major>;
using mse_f_dd_rm = matrix_storage_engine<float, extents<dynamic_extent, dynamic_extent>, allocator<float>, row_major>;
using mse_i_dd_cm = matrix_storage_engine<int, extents<dynamic_extent, dynamic_extent>, allocator<int>, column_major>;
using mse_d_dd_rm = matrix_storage_engine<double, extents<dynamic_extent, dynamic_extent>, allocator<double>, column_major>;

TEST(MSE_Matrix_4E, DefaultCtor)
{
    mse_f_dd_rm     e1;

    EXPECT_EQ(e1.rows(), 0);
    EXPECT_EQ(e1.columns(), 0);
    EXPECT_EQ(e1.row_capacity(), 0);
    EXPECT_EQ(e1.column_capacity(), 0);
}


TEST(MSE_Matrix_4E, DirectCtor)
{
    mse_f_dd_rm     e1(4, 4);

    EXPECT_EQ(e1.rows(), 4);
    EXPECT_EQ(e1.columns(), 4);
    EXPECT_EQ(e1.size(), st_44);
    EXPECT_EQ(e1.row_capacity(), 4);
    EXPECT_EQ(e1.column_capacity(), 4);

    //- Verify elements are value initialized.
    //
    EXPECT_EQ(e1(0, 0), 0.0f);
    EXPECT_EQ(e1(0, 1), 0.0f);
    EXPECT_EQ(e1(0, 2), 0.0f);
    EXPECT_EQ(e1(0, 3), 0.0f);
    EXPECT_EQ(e1(1, 0), 0.0f);
    EXPECT_EQ(e1(1, 1), 0.0f);
    EXPECT_EQ(e1(1, 2), 0.0f);
    EXPECT_EQ(e1(1, 3), 0.0f);
    EXPECT_EQ(e1(2, 0), 0.0f);
    EXPECT_EQ(e1(2, 1), 0.0f);
    EXPECT_EQ(e1(2, 2), 0.0f);
    EXPECT_EQ(e1(2, 3), 0.0f);
    EXPECT_EQ(e1(3, 0), 0.0f);
    EXPECT_EQ(e1(3, 1), 0.0f);
    EXPECT_EQ(e1(3, 2), 0.0f);
    EXPECT_EQ(e1(3, 3), 0.0f);
}


TEST(MSE_Matrix_4E, CmpEq)
{
    mse_f_dd_rm     e1(4, 4), e2(4, 4), e3{4, 4};

    //- Verify size, capacity, initial values.
    //
    EXPECT_EQ(e1.rows(), 4);
    EXPECT_EQ(e1.columns(), 4);
    EXPECT_EQ(e1.size(), st_44);
    EXPECT_EQ(e1.row_capacity(), 4);
    EXPECT_EQ(e1.column_capacity(), 4);

    EXPECT_EQ(e1(0, 0), 0.0f);
    EXPECT_EQ(e1(0, 1), 0.0f);
    EXPECT_EQ(e1(0, 2), 0.0f);
    EXPECT_EQ(e1(0, 3), 0.0f);
    EXPECT_EQ(e1(1, 0), 0.0f);
    EXPECT_EQ(e1(1, 1), 0.0f);
    EXPECT_EQ(e1(1, 2), 0.0f);
    EXPECT_EQ(e1(1, 3), 0.0f);
    EXPECT_EQ(e1(2, 0), 0.0f);
    EXPECT_EQ(e1(2, 1), 0.0f);
    EXPECT_EQ(e1(2, 2), 0.0f);
    EXPECT_EQ(e1(2, 3), 0.0f);
    EXPECT_EQ(e1(3, 0), 0.0f);
    EXPECT_EQ(e1(3, 1), 0.0f);
    EXPECT_EQ(e1(3, 2), 0.0f);
    EXPECT_EQ(e1(3, 3), 0.0f);

    //- Verify expected equality of default-constructed engines.
    //
    EXPECT_TRUE(m_cmp_eq(e1, e1));
    EXPECT_TRUE(m_cmp_eq(e1, e2));
    EXPECT_TRUE(m_cmp_eq(e1, e3));

    //- Verify equality against an init-list of equivalent values.
    //
    EXPECT_TRUE(m_cmp_eq(e1, LST_44_0));
    EXPECT_TRUE(m_cmp_eq(e1, il_44_0));
    EXPECT_TRUE(m_cmp_eq(e1, fl_44_0));

    //- Verify inequality against an init-list of different values.
    //
    EXPECT_FALSE(m_cmp_eq(e1, LST_44_1));
    EXPECT_FALSE(m_cmp_eq(e1, il_44_1));
    EXPECT_FALSE(m_cmp_eq(e1, fl_44_1));

    //- Assign new element values via mutable indexing and verify them.
    //
    e2(0, 0) = 11.0f;
    e2(0, 1) = 12.0f;
    e2(0, 2) = 13.0f;
    e2(0, 3) = 14.0f;
    e2(1, 0) = 21.0f;
    e2(1, 1) = 22.0f;
    e2(1, 2) = 23.0f;
    e2(1, 3) = 24.0f;
    e2(2, 0) = 31.0f;
    e2(2, 1) = 32.0f;
    e2(2, 2) = 33.0f;
    e2(2, 3) = 34.0f;
    e2(3, 0) = 41.0f;
    e2(3, 1) = 42.0f;
    e2(3, 2) = 43.0f;
    e2(3, 3) = 44.0f;
    EXPECT_EQ(e2(0, 0), 11.0f);
    EXPECT_EQ(e2(0, 1), 12.0f);
    EXPECT_EQ(e2(0, 2), 13.0f);
    EXPECT_EQ(e2(0, 3), 14.0f);
    EXPECT_EQ(e2(1, 0), 21.0f);
    EXPECT_EQ(e2(1, 1), 22.0f);
    EXPECT_EQ(e2(1, 2), 23.0f);
    EXPECT_EQ(e2(1, 3), 24.0f);
    EXPECT_EQ(e2(2, 0), 31.0f);
    EXPECT_EQ(e2(2, 1), 32.0f);
    EXPECT_EQ(e2(2, 2), 33.0f);
    EXPECT_EQ(e2(2, 3), 34.0f);
    EXPECT_EQ(e2(3, 0), 41.0f);
    EXPECT_EQ(e2(3, 1), 42.0f);
    EXPECT_EQ(e2(3, 2), 43.0f);
    EXPECT_EQ(e2(3, 3), 44.0f);
    EXPECT_FALSE(m_cmp_eq(e1, e2));

    e3(0, 0) = 11.0f;
    e3(0, 1) = 12.0f;
    e3(0, 2) = 13.0f;
    e3(0, 3) = 14.0f;
    e3(1, 0) = 21.0f;
    e3(1, 1) = 22.0f;
    e3(1, 2) = 23.0f;
    e3(1, 3) = 24.0f;
    e3(2, 0) = 31.0f;
    e3(2, 1) = 32.0f;
    e3(2, 2) = 33.0f;
    e3(2, 3) = 34.0f;
    e3(3, 0) = 41.0f;
    e3(3, 1) = 42.0f;
    e3(3, 2) = 43.0f;
    e3(3, 3) = 44.0f;
    EXPECT_EQ(e3(0, 0), 11.0f);
    EXPECT_EQ(e3(0, 1), 12.0f);
    EXPECT_EQ(e3(0, 2), 13.0f);
    EXPECT_EQ(e3(0, 3), 14.0f);
    EXPECT_EQ(e3(1, 0), 21.0f);
    EXPECT_EQ(e3(1, 1), 22.0f);
    EXPECT_EQ(e3(1, 2), 23.0f);
    EXPECT_EQ(e3(1, 3), 24.0f);
    EXPECT_EQ(e3(2, 0), 31.0f);
    EXPECT_EQ(e3(2, 1), 32.0f);
    EXPECT_EQ(e3(2, 2), 33.0f);
    EXPECT_EQ(e3(2, 3), 34.0f);
    EXPECT_EQ(e3(3, 0), 41.0f);
    EXPECT_EQ(e3(3, 1), 42.0f);
    EXPECT_EQ(e3(3, 2), 43.0f);
    EXPECT_EQ(e3(3, 3), 44.0f);
    EXPECT_FALSE(m_cmp_eq(e1, e3));

    //- Verify expected equality of various argument combinations.
    //
    EXPECT_TRUE(m_cmp_eq(e2, e3));

    EXPECT_TRUE(m_cmp_eq(e2, LST_44_2));
    EXPECT_TRUE(m_cmp_eq(e2, il_44_2));
    EXPECT_TRUE(m_cmp_eq(e2, fl_44_2));

    EXPECT_TRUE(m_cmp_eq(e3, LST_44_2));
    EXPECT_TRUE(m_cmp_eq(e3, il_44_2));
    EXPECT_TRUE(m_cmp_eq(e3, fl_44_2));

    EXPECT_TRUE(m_cmp_eq(e2, e3));
    EXPECT_FALSE(m_cmp_eq(e2, e1));
    EXPECT_FALSE(m_cmp_eq(e1, e3));

    //- Verify expected inequality when compared to init-lists of incorrect size.
    //
    EXPECT_FALSE(m_cmp_eq(e3, LST_33_0));
    EXPECT_FALSE(m_cmp_eq(e3, il_33_1));
    EXPECT_FALSE(m_cmp_eq(e3, fl_33_2));

    //- Verify expected inequality against init-lists and engines having different contents.
    //
    EXPECT_FALSE(m_cmp_eq(e3, fl_44_0));
    EXPECT_FALSE(m_cmp_eq(e3, il_44_1));
    EXPECT_FALSE(m_cmp_eq(e3, mse_f_dd_rm(LST_44_1)));

    //- Verify size, capacity, initial values.
    //
    mse_f_dd_rm     e4(3, 3);

    EXPECT_EQ(e4.rows(), 3);
    EXPECT_EQ(e4.columns(), 3);
    EXPECT_EQ(e4.size(), st_33);
    EXPECT_EQ(e4.row_capacity(), 3);
    EXPECT_EQ(e4.column_capacity(), 3);

    //- Verify expected default values.
    //
    EXPECT_TRUE(m_cmp_eq(e4, LST_33_0));
    EXPECT_TRUE(m_cmp_eq(e4, il_33_0));
    EXPECT_TRUE(m_cmp_eq(e4, fl_33_0));

    //- Verify expected results comparing engines of different sizes.
    //
    EXPECT_FALSE(m_cmp_eq(e4, e3));
    EXPECT_FALSE(m_cmp_eq(e3, e4));
}


TEST(MSE_Matrix_4E, MoveCtor)
{
    //- Default construct and verify initial state.
    //
    mse_f_dd_rm     e1(4, 4);

    EXPECT_EQ(e1.rows(), 4);
    EXPECT_EQ(e1.columns(), 4);
    EXPECT_EQ(e1.row_capacity(), 4);
    EXPECT_EQ(e1.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e1, LST_44_0));

    //- Assign new element values and verify them.
    //
    e1(0, 0) = 11.0f;
    e1(0, 1) = 12.0f;
    e1(0, 2) = 13.0f;
    e1(0, 3) = 14.0f;
    e1(1, 0) = 21.0f;
    e1(1, 1) = 22.0f;
    e1(1, 2) = 23.0f;
    e1(1, 3) = 24.0f;
    e1(2, 0) = 31.0f;
    e1(2, 1) = 32.0f;
    e1(2, 2) = 33.0f;
    e1(2, 3) = 34.0f;
    e1(3, 0) = 41.0f;
    e1(3, 1) = 42.0f;
    e1(3, 2) = 43.0f;
    e1(3, 3) = 44.0f;
    EXPECT_TRUE(m_cmp_eq(e1, fl_44_2));

    //- Construct a new engine via move ctor and verify that its initial state is as expected.
    //
    mse_f_dd_rm     e2(std::move(e1));

    EXPECT_EQ(e2.rows(), 4);
    EXPECT_EQ(e2.columns(), 4);
    EXPECT_EQ(e2.row_capacity(), 4);
    EXPECT_EQ(e2.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e2, fl_44_2));
}


TEST(MSE_Matrix_4E, CopyCtor)
{
    //- Default construct and verify initial state.
    //
    mse_f_dd_rm     e1(4, 4);

    EXPECT_EQ(e1.rows(), 4);
    EXPECT_EQ(e1.columns(), 4);
    EXPECT_EQ(e1.row_capacity(), 4);
    EXPECT_EQ(e1.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e1, LST_44_0));

    //- Assign new element values and verify them.
    //
    e1(0, 0) = 11.0f;
    e1(0, 1) = 12.0f;
    e1(0, 2) = 13.0f;
    e1(0, 3) = 14.0f;
    e1(1, 0) = 21.0f;
    e1(1, 1) = 22.0f;
    e1(1, 2) = 23.0f;
    e1(1, 3) = 24.0f;
    e1(2, 0) = 31.0f;
    e1(2, 1) = 32.0f;
    e1(2, 2) = 33.0f;
    e1(2, 3) = 34.0f;
    e1(3, 0) = 41.0f;
    e1(3, 1) = 42.0f;
    e1(3, 2) = 43.0f;
    e1(3, 3) = 44.0f;
    EXPECT_TRUE(m_cmp_eq(e1, fl_44_2));

    //- Construct new engines via copy ctors and verify their initial states.
    //
    mse_f_dd_rm     e2(e1);

    EXPECT_EQ(e2.rows(), 4);
    EXPECT_EQ(e2.columns(), 4);
    EXPECT_EQ(e2.row_capacity(), 4);
    EXPECT_EQ(e2.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e2, e1));

    mse_f_dd_rm     e3 = e1;

    EXPECT_EQ(e3.rows(), 4);
    EXPECT_EQ(e3.columns(), 4);
    EXPECT_EQ(e3.row_capacity(), 4);
    EXPECT_EQ(e3.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e3, e1));

    mse_f_dd_rm     e4{e1};

    EXPECT_EQ(e4.rows(), 4);
    EXPECT_EQ(e4.columns(), 4);
    EXPECT_EQ(e4.size(), st_44);
    EXPECT_EQ(e4.row_capacity(), 4);
    EXPECT_EQ(e4.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e4, e1));
}


TEST(MSE_Matrix_4E, ListCtor)
{
    //- Construct new engines via list ctor and verify that their initial states.
    //
    mse_f_dd_rm     e1 = LST_44_1;
    mse_f_dd_rm     e2 LST_44_1 ;
    mse_f_dd_rm     e3(LST_44_1);

    EXPECT_EQ(e1.rows(), 4);
    EXPECT_EQ(e1.columns(), 4);
    EXPECT_EQ(e1.row_capacity(), 4);
    EXPECT_EQ(e1.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e1, LST_44_1));

    EXPECT_EQ(e2.rows(), 4);
    EXPECT_EQ(e2.columns(), 4);
    EXPECT_EQ(e2.row_capacity(), 4);
    EXPECT_EQ(e2.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e2, e1));

    EXPECT_EQ(e3.rows(), 4);
    EXPECT_EQ(e3.columns(), 4);
    EXPECT_EQ(e3.row_capacity(), 4);
    EXPECT_EQ(e3.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e3, e1));
}


TEST(MSE_Matrix_4E, EngineCtor)
{
    //- Construct new engines via direct engine ctor and verify their initial states.
    //
    mse_i_dd_cm     e1 = LST_44_2;
    mse_f_dd_rm     e2(e1);
    mse_d_dd_rm     e3(e2);
    mse_f_dd_cm     e4 = LST_44_2;
    mse_f_dd_rm     e5{e4};

    EXPECT_EQ(e1.rows(), 4);
    EXPECT_EQ(e1.columns(), 4);
    EXPECT_EQ(e1.row_capacity(), 4);
    EXPECT_EQ(e1.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e1, il_44_2));

    EXPECT_EQ(e2.rows(), 4);
    EXPECT_EQ(e2.columns(), 4);
    EXPECT_EQ(e2.row_capacity(), 4);
    EXPECT_EQ(e2.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e2, e1));

    EXPECT_EQ(e3.rows(), 4);
    EXPECT_EQ(e3.columns(), 4);
    EXPECT_EQ(e3.row_capacity(), 4);
    EXPECT_EQ(e3.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e3, e1));

    EXPECT_EQ(e4.rows(), 4);
    EXPECT_EQ(e4.columns(), 4);
    EXPECT_EQ(e4.row_capacity(), 4);
    EXPECT_EQ(e4.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e4, e1));

    EXPECT_EQ(e5.rows(), 4);
    EXPECT_EQ(e5.columns(), 4);
    EXPECT_EQ(e5.row_capacity(), 4);
    EXPECT_EQ(e5.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e5, e1));
}


TEST(MSE_Matrix_4E, MoveAssign)
{
    //- Default construct and verify initial state.
    //
    mse_f_dd_rm     e1(4, 4);

    EXPECT_EQ(e1.rows(), 4);
    EXPECT_EQ(e1.columns(), 4);
    EXPECT_EQ(e1.row_capacity(), 4);
    EXPECT_EQ(e1.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e1, il_44_0));

    //- List construct and verify initial state.
    //
    mse_f_dd_rm     e2(il_44_1);

    EXPECT_EQ(e2.rows(), 4);
    EXPECT_EQ(e2.columns(), 4);
    EXPECT_EQ(e2.row_capacity(), 4);
    EXPECT_EQ(e2.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e2, fl_44_1));

    //- Move-assign and verify.
    //
    e1 = std::move(e2);
    EXPECT_TRUE(m_cmp_eq(e1, fl_44_1));
}


TEST(MSE_Matrix_4E, CopyAssign)
{
    //- Default construct and verify initial state.
    //
    mse_f_dd_rm     e1(4, 4);

    EXPECT_EQ(e1.rows(), 4);
    EXPECT_EQ(e1.columns(), 4);
    EXPECT_EQ(e1.row_capacity(), 4);
    EXPECT_EQ(e1.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e1, il_44_0));

    //- List construct and verify initial state.
    //
    mse_f_dd_rm     e2(il_44_1);

    EXPECT_EQ(e2.rows(), 4);
    EXPECT_EQ(e2.columns(), 4);
    EXPECT_EQ(e2.row_capacity(), 4);
    EXPECT_EQ(e2.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e2, fl_44_1));

    //- Copy-assign and verify.
    //
    e1 = e2;
    EXPECT_TRUE(m_cmp_eq(e1, fl_44_1));
}


TEST(MSE_Matrix_4E, EngineAssign)
{
    //- Default construct and verify initial state.
    //
    mse_f_dd_rm     e1(4, 4), e2{4, 4};

    EXPECT_EQ(e1.rows(), 4);
    EXPECT_EQ(e1.columns(), 4);
    EXPECT_EQ(e1.row_capacity(), 4);
    EXPECT_EQ(e1.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e1, LST_44_0));

    EXPECT_EQ(e2.rows(), 4);
    EXPECT_EQ(e2.columns(), 4);
    EXPECT_EQ(e2.row_capacity(), 4);
    EXPECT_EQ(e2.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e2, fl_44_0));

    //- List construct and verify initial state.
    //
    mse_i_dd_cm     e3 LST_44_1 ;

    EXPECT_EQ(e3.rows(), 4);
    EXPECT_EQ(e3.columns(), 4);
    EXPECT_EQ(e3.row_capacity(), 4);
    EXPECT_EQ(e3.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e3, il_44_1));

    fs_matrix_engine<int, 4, 4> e4 LST_44_2 ;

    EXPECT_EQ(e4.rows(), 4);
    EXPECT_EQ(e4.columns(), 4);
    EXPECT_EQ(e4.capacity(), st_44);
    EXPECT_TRUE(m_cmp_eq(e4, il_44_2));

    EXPECT_FALSE(m_cmp_eq(e3, e4));

    //- Assign and verify.
    //
    e1 = e3;
    EXPECT_TRUE(m_cmp_eq(e1, fl_44_1));
    EXPECT_TRUE(m_cmp_eq(e1, e3));

    e2 = e4;
    EXPECT_TRUE(m_cmp_eq(e2, fl_44_2));
    EXPECT_TRUE(m_cmp_eq(e2, e4));
}


TEST(MSE_Matrix_4E, ListAssign)
{
    //- Default construct and verify initial state.
    //
    mse_f_dd_rm     e1(4, 4);
    mse_f_dd_rm     e2{il_44_1};

    EXPECT_EQ(e1.rows(), 4);
    EXPECT_EQ(e1.columns(), 4);
    EXPECT_EQ(e1.row_capacity(), 4);
    EXPECT_EQ(e1.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e1, LST_44_0));

    EXPECT_EQ(e2.rows(), 4);
    EXPECT_EQ(e2.columns(), 4);
    EXPECT_EQ(e2.row_capacity(), 4);
    EXPECT_EQ(e2.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e2, LST_44_1));

    //- Assign and verify.
    //
    e1 = LST_44_1;
    EXPECT_TRUE(m_cmp_eq(e1, LST_44_1));
    EXPECT_TRUE(m_cmp_eq(e1, il_44_1));
    EXPECT_TRUE(m_cmp_eq(e1, fl_44_1));
    EXPECT_TRUE(m_cmp_eq(e1, e2));

    e1 = fl_44_0;
    EXPECT_TRUE(m_cmp_eq(e1, LST_44_0));
    EXPECT_TRUE(m_cmp_eq(e1, il_44_0));
    EXPECT_TRUE(m_cmp_eq(e1, fl_44_0));
    EXPECT_TRUE(m_cmp_eq(e1, mse_f_dd_rm(4, 4)));

    e1 = fl_33_1;
    EXPECT_EQ(e1.size(), st_33);
    EXPECT_EQ(e1.row_capacity(), 3);
    EXPECT_EQ(e1.column_capacity(), 3);
    EXPECT_TRUE(m_cmp_eq(e1, fl_33_1));
}


TEST(MSE_Matrix_4E, ReshapeColumns)
{
    //- List construct and verify initial state.
    //
    mse_f_dd_rm      e1 LST_44_2 ;

    EXPECT_EQ(e1.rows(), 4);
    EXPECT_EQ(e1.columns(), 4);
    EXPECT_EQ(e1.row_capacity(), 4);
    EXPECT_EQ(e1.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e1, LST_44_2));

    //- Resize upward and verify.
    //
    e1.reshape_columns(6, e1.column_capacity());
    EXPECT_EQ(e1.rows(), 4);
    EXPECT_EQ(e1.columns(), 6);
    EXPECT_EQ(e1.row_capacity(), 4);
    EXPECT_EQ(e1.column_capacity(), 6);
    EXPECT_TRUE(m_cmp_eq(e1, LST_44_2_CR3));

    //- Resize downward and verify.
    //
    e1.reshape_columns(2, e1.column_capacity());
    EXPECT_EQ(e1.rows(), 4);
    EXPECT_EQ(e1.columns(), 2);
    EXPECT_EQ(e1.row_capacity(), 4);
    EXPECT_EQ(e1.column_capacity(), 6);
    EXPECT_TRUE(m_cmp_eq(e1, LST_44_2_CR1));

    //- List construct and verify initial state.
    //
    mse_f_dd_rm      e2(il_44_2);

    EXPECT_EQ(e2.rows(), 4);
    EXPECT_EQ(e2.columns(), 4);
    EXPECT_EQ(e2.row_capacity(), 4);
    EXPECT_EQ(e2.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e2, il_44_2));

    //- Reserve extra space and verify.
    //
    e2.reshape_columns(e2.columns(), 8);
    EXPECT_EQ(e2.rows(), 4);
    EXPECT_EQ(e2.columns(), 4);
    EXPECT_EQ(e2.row_capacity(), 4);
    EXPECT_EQ(e2.column_capacity(), 8);
    EXPECT_TRUE(m_cmp_eq(e2, il_44_2));

    //- Release extra space and verify.
    //
    e2.reshape_columns(4, 0);
    EXPECT_EQ(e2.rows(), 4);
    EXPECT_EQ(e2.columns(), 4);
    EXPECT_EQ(e2.row_capacity(), 4);
    EXPECT_EQ(e2.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e2, il_44_2));

    //- List construct and verify initial state.
    //
    mse_f_dd_rm      e3 LST_44_2 ;

    EXPECT_EQ(e3.rows(), 4);
    EXPECT_EQ(e3.columns(), 4);
    EXPECT_EQ(e3.row_capacity(), 4);
    EXPECT_EQ(e3.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e3, LST_44_2));

    //- Resize upward and verify.
    //
    e3.reshape_columns(6, 8);
    EXPECT_EQ(e3.rows(), 4);
    EXPECT_EQ(e3.columns(), 6);
    EXPECT_EQ(e3.row_capacity(), 4);
    EXPECT_EQ(e3.column_capacity(), 8);
    EXPECT_TRUE(m_cmp_eq(e3, LST_44_2_CR3));

    //- Resize downward and verify.
    //
    e3.reshape_columns(2, 8);
    EXPECT_EQ(e3.rows(), 4);
    EXPECT_EQ(e3.columns(), 2);
    EXPECT_EQ(e3.row_capacity(), 4);
    EXPECT_EQ(e3.column_capacity(), 8);
    EXPECT_TRUE(m_cmp_eq(e3, LST_44_2_CR1));
}


TEST(MSE_Matrix_4E, ReshapeRows)
{
    //- List construct and verify initial state.
    //
    mse_f_dd_rm      e1 LST_44_2 ;

    EXPECT_EQ(e1.rows(), 4);
    EXPECT_EQ(e1.columns(), 4);
    EXPECT_EQ(e1.row_capacity(), 4);
    EXPECT_EQ(e1.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e1, LST_44_2));

    //- Resize upward and verify.
    //
    e1.reshape_rows(6, e1.row_capacity());
    EXPECT_EQ(e1.rows(), 6);
    EXPECT_EQ(e1.columns(), 4);
    EXPECT_EQ(e1.row_capacity(), 6);
    EXPECT_EQ(e1.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e1, LST_44_2_RR3));

    //- Resize downward and verify.
    //
    e1.reshape_rows(2, e1.row_capacity());
    EXPECT_EQ(e1.rows(), 2);
    EXPECT_EQ(e1.columns(), 4);
    EXPECT_EQ(e1.row_capacity(), 6);
    EXPECT_EQ(e1.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e1, LST_44_2_RR1));

    //- List construct and verify initial state.
    //
    mse_f_dd_rm      e2(il_44_2);

    EXPECT_EQ(e2.rows(), 4);
    EXPECT_EQ(e2.columns(), 4);
    EXPECT_EQ(e2.row_capacity(), 4);
    EXPECT_EQ(e2.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e2, il_44_2));

    //- Reserve extra space and verify.
    //
    e2.reshape_rows(e2.rows(), 8);
    EXPECT_EQ(e2.rows(), 4);
    EXPECT_EQ(e2.columns(), 4);
    EXPECT_EQ(e2.row_capacity(), 8);
    EXPECT_EQ(e2.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e2, il_44_2));

    //- Release extra space and verify.
    //
    e2.reshape_rows(e2.rows(), 0);
    EXPECT_EQ(e2.rows(), 4);
    EXPECT_EQ(e2.columns(), 4);
    EXPECT_EQ(e2.row_capacity(), 4);
    EXPECT_EQ(e2.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e2, il_44_2));

    //- List construct and verify initial state.
    //
    mse_f_dd_rm      e3 LST_44_2 ;

    EXPECT_EQ(e3.rows(), 4);
    EXPECT_EQ(e3.columns(), 4);
    EXPECT_EQ(e3.row_capacity(), 4);
    EXPECT_EQ(e3.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e3, LST_44_2));

    //- Resize upward and verify.
    //
    e3.reshape_rows(6, 8);
    EXPECT_EQ(e3.rows(), 6);
    EXPECT_EQ(e3.columns(), 4);
    EXPECT_EQ(e3.row_capacity(), 8);
    EXPECT_EQ(e3.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e3, LST_44_2_RR3));

    //- Resize downward and verify.
    //
    e3.reshape_rows(2, 8);
    EXPECT_EQ(e3.rows(), 2);
    EXPECT_EQ(e3.columns(), 4);
    EXPECT_EQ(e3.row_capacity(), 8);
    EXPECT_EQ(e3.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e3, LST_44_2_RR1));
}


TEST(MSE_Matrix_4E, Reshape)
{
    //- List construct and verify initial state.
    //
    mse_f_dd_rm      e1 LST_44_2 ;

    EXPECT_EQ(e1.rows(), 4);
    EXPECT_EQ(e1.columns(), 4);
    EXPECT_EQ(e1.row_capacity(), 4);
    EXPECT_EQ(e1.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e1, LST_44_2));

    //- Resize downward and verify.
    //
    e1.reshape(3, 3, e1.row_capacity(), e1.column_capacity());
    EXPECT_EQ(e1.rows(), 3);
    EXPECT_EQ(e1.columns(), 3);
    EXPECT_EQ(e1.row_capacity(), 4);
    EXPECT_EQ(e1.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e1, LST_33_3));

    //- Resize upward and verify.
    //
    e1.reshape(5, 5, e1.row_capacity(), e1.column_capacity());
    EXPECT_EQ(e1.rows(), 5);
    EXPECT_EQ(e1.columns(), 5);
    EXPECT_EQ(e1.row_capacity(), 5);
    EXPECT_EQ(e1.column_capacity(), 5);
    EXPECT_TRUE(m_cmp_eq(e1, LST_55_3));

    //- List construct and verify initial state.
    //
    mse_f_dd_rm      e2(il_44_2);

    EXPECT_EQ(e2.size(), st_44);
    EXPECT_EQ(e2.rows(), 4);
    EXPECT_EQ(e2.columns(), 4);
    EXPECT_EQ(e2.row_capacity(), 4);
    EXPECT_EQ(e2.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e2, il_44_2));

    //- Reserve extra space and verify.
    //
    e2.reshape(e2.rows(), e2.columns(), 32, 32);
    EXPECT_EQ(e2.rows(), 4);
    EXPECT_EQ(e2.columns(), 4);
    EXPECT_EQ(e2.row_capacity(), 32);
    EXPECT_EQ(e2.column_capacity(), 32);
    EXPECT_TRUE(m_cmp_eq(e2, il_44_2));

    //- Reduce extra space and verify.
    //
    e2.reshape(e2.rows(), e2.columns(), 0, 0);
    EXPECT_EQ(e2.rows(), 4);
    EXPECT_EQ(e2.columns(), 4);
    EXPECT_EQ(e2.row_capacity(), 4);
    EXPECT_EQ(e2.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e2, il_44_2));

    //- List construct and verify initial state.
    //
    mse_f_dd_rm      e3 LST_44_2 ;

    EXPECT_EQ(e3.rows(), 4);
    EXPECT_EQ(e3.columns(), 4);
    EXPECT_EQ(e3.row_capacity(), 4);
    EXPECT_EQ(e3.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e3, LST_44_2));

    //- Reshape downward and verify.
    //
    e3.reshape(3, 3, 6, 6);
    EXPECT_EQ(e3.rows(), 3);
    EXPECT_EQ(e3.columns(), 3);
    EXPECT_EQ(e3.row_capacity(), 6);
    EXPECT_EQ(e3.column_capacity(), 6);
    EXPECT_TRUE(m_cmp_eq(e3, LST_33_3));

    //- Reshape upward and verify.
    //
    e3.reshape(5, 5, 8, 8);
    EXPECT_EQ(e3.rows(), 5);
    EXPECT_EQ(e3.columns(), 5);
    EXPECT_EQ(e3.row_capacity(), 8);
    EXPECT_EQ(e3.column_capacity(), 8);
    EXPECT_TRUE(m_cmp_eq(e3, LST_55_3));
}


TEST(MSE_Matrix_4E, Swap)
{
    //- Default construct and verify initial state.
    //
    mse_f_dd_rm     e1(4, 4);

    EXPECT_EQ(e1.size(), st_44);
    EXPECT_TRUE(m_cmp_eq(e1, fl_44_0));

    //- List construct and verify initial state.
    //
    mse_f_dd_rm     e2(fl_44_2);

    EXPECT_EQ(e2.size(), st_44);
    EXPECT_TRUE(m_cmp_eq(e2, fl_44_2));

    //- Swap contents and verify.
    //
    e1.swap(e2);
    EXPECT_TRUE(m_cmp_eq(e1, fl_44_2));
    EXPECT_TRUE(m_cmp_eq(e2, fl_44_0));

    e2.swap(e1);
    EXPECT_TRUE(m_cmp_eq(e1, il_44_0));
    EXPECT_TRUE(m_cmp_eq(e2, il_44_2));

    //- Swap some elements and verify.
    //
    e2.swap_columns(0, 2);
    EXPECT_TRUE(m_cmp_eq(e2, LST_44_2CS));

    e2 = LST_44_2;
    EXPECT_TRUE(m_cmp_eq(e2, LST_44_2));

    e2.swap_rows(1, 2);
    EXPECT_TRUE(m_cmp_eq(e2, LST_44_2RS));
}


TEST(MSE_Matrix_4E, Span)
{
    mse_f_dd_rm         e1(il_44_2), e2{4, 4}, e3{e1};
    mse_f_dd_rm const&  ce1 = e1;

    auto    sp1  = e1.span();
    auto    csp1 = ce1.span();

    //- Const and mutable spans should have different types.
    //
    EXPECT_FALSE((std::is_same_v<decltype(sp1), decltype(csp1)>));

    //- The elements of the const and mutable spans should have the expected values.
    //
    EXPECT_EQ(sp1(0, 0), 11.0f);
    EXPECT_EQ(sp1(0, 1), 12.0f);
    EXPECT_EQ(sp1(0, 2), 13.0f);
    EXPECT_EQ(sp1(0, 3), 14.0f);
    EXPECT_EQ(sp1(1, 0), 21.0f);
    EXPECT_EQ(sp1(1, 1), 22.0f);
    EXPECT_EQ(sp1(1, 2), 23.0f);
    EXPECT_EQ(sp1(1, 3), 24.0f);
    EXPECT_EQ(sp1(2, 0), 31.0f);
    EXPECT_EQ(sp1(2, 1), 32.0f);
    EXPECT_EQ(sp1(2, 2), 33.0f);
    EXPECT_EQ(sp1(2, 3), 34.0f);
    EXPECT_EQ(sp1(3, 0), 41.0f);
    EXPECT_EQ(sp1(3, 1), 42.0f);
    EXPECT_EQ(sp1(3, 2), 43.0f);
    EXPECT_EQ(sp1(3, 3), 44.0f);

    EXPECT_EQ(csp1(0, 0), 11.0f);
    EXPECT_EQ(csp1(0, 1), 12.0f);
    EXPECT_EQ(csp1(0, 2), 13.0f);
    EXPECT_EQ(csp1(0, 3), 14.0f);
    EXPECT_EQ(csp1(1, 0), 21.0f);
    EXPECT_EQ(csp1(1, 1), 22.0f);
    EXPECT_EQ(csp1(1, 2), 23.0f);
    EXPECT_EQ(csp1(1, 3), 24.0f);
    EXPECT_EQ(csp1(2, 0), 31.0f);
    EXPECT_EQ(csp1(2, 1), 32.0f);
    EXPECT_EQ(csp1(2, 2), 33.0f);
    EXPECT_EQ(csp1(2, 3), 34.0f);
    EXPECT_EQ(csp1(3, 0), 41.0f);
    EXPECT_EQ(csp1(3, 1), 42.0f);
    EXPECT_EQ(csp1(3, 2), 43.0f);
    EXPECT_EQ(csp1(3, 3), 44.0f);

    //- Whole-object comparison between engines and spans should work as expected.
    //
    EXPECT_TRUE(m_cmp_eq(e1, sp1));
    EXPECT_TRUE(m_cmp_eq(e1, csp1));
    EXPECT_TRUE(m_cmp_eq(e3, sp1));
    EXPECT_TRUE(m_cmp_eq(e3, csp1));
    EXPECT_FALSE(m_cmp_eq(e2, sp1));
    EXPECT_FALSE(m_cmp_eq(e2, csp1));

    //- Setting values of individual span elements should be reflected in the owning engine.
    //
    sp1(0, 0) = 111.0f;
    sp1(1, 1) = 222.0f;
    sp1(2, 2) = 333.0f;
    sp1(3, 3) = 444.0f;
    EXPECT_EQ(e1(0, 0), 111.0f);
    EXPECT_EQ(e1(1, 1), 222.0f);
    EXPECT_EQ(e1(2, 2), 333.0f);
    EXPECT_EQ(e1(3, 3), 444.0f);
}
