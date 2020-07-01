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

