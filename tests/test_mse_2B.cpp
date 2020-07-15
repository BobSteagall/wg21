#include "test_common.hpp"

using namespace STD_LA;
using namespace STD_LA::detail;

//==================================================================================================
//  Unit tests for fixed-size statically allocated matrix engine, Case 2B
//==================================================================================================
//
using mse_f_ffa_cm = matrix_storage_engine<float, extents<1, 4>, allocator<float>, column_major>;
using mse_f_ffa_rm = matrix_storage_engine<float, extents<1, 4>, allocator<float>, row_major>;
using mse_i_ffa_cm = matrix_storage_engine<int, extents<1, 4>, allocator<int>, column_major>;
using mse_d_ffa_rm = matrix_storage_engine<double, extents<1, 4>, allocator<double>, column_major>;

TEST(MSE_Matrix_2B, DefaultCtor)
{
    mse_f_ffa_rm    e1;

    EXPECT_EQ(e1.rows(), 1);
    EXPECT_EQ(e1.columns(), 4);
    EXPECT_EQ(e1.row_capacity(), 1);
    EXPECT_EQ(e1.column_capacity(), 4);

    //- Verify elements are value initialized.
    //
    EXPECT_EQ(e1(0, 0), 0.0f);
    EXPECT_EQ(e1(0, 1), 0.0f);
    EXPECT_EQ(e1(0, 2), 0.0f);
    EXPECT_EQ(e1(0, 3), 0.0f);
    EXPECT_EQ(e1(0), 0.0f);
    EXPECT_EQ(e1(1), 0.0f);
    EXPECT_EQ(e1(2), 0.0f);
    EXPECT_EQ(e1(3), 0.0f);
}


TEST(MSE_Matrix_2B, CmpEq)
{
    mse_f_ffa_rm    e1, e2, e3;

    //- Verify size, capacity, initial values.
    //
    EXPECT_EQ(e1.rows(), 1);
    EXPECT_EQ(e1.columns(), 4);
    EXPECT_EQ(e1.row_capacity(), 1);
    EXPECT_EQ(e1.column_capacity(), 4);

    EXPECT_EQ(e1(0, 0), 0.0f);
    EXPECT_EQ(e1(0, 1), 0.0f);
    EXPECT_EQ(e1(0, 2), 0.0f);
    EXPECT_EQ(e1(0, 3), 0.0f);
    EXPECT_EQ(e1(0), 0.0f);
    EXPECT_EQ(e1(1), 0.0f);
    EXPECT_EQ(e1(2), 0.0f);
    EXPECT_EQ(e1(3), 0.0f);

    //- Verify expected equality of default-constructed engines.
    //
    EXPECT_TRUE(m_cmp_eq(e1, e1));
    EXPECT_TRUE(m_cmp_eq(e1, e2));
    EXPECT_TRUE(m_cmp_eq(e1, e3));

    //- Verify equality against an init-list of equivalent values.
    //
    EXPECT_TRUE(m_cmp_eq(e1, LST_14_0));
    EXPECT_TRUE(m_cmp_eq(e1, il_14_0));
    EXPECT_TRUE(m_cmp_eq(e1, fl_14_0));

    //- Verify inequality against an init-list of different values.
    //
    EXPECT_FALSE(m_cmp_eq(e1, LST_14_1));
    EXPECT_FALSE(m_cmp_eq(e1, il_14_1));
    EXPECT_FALSE(m_cmp_eq(e1, fl_14_1));

    //- Assign new element values via mutable indexing and verify them.
    //
    e2(0, 0) = 11.0f;
    e2(0, 1) = 12.0f;
    e2(0, 2) = 13.0f;
    e2(0, 3) = 14.0f;
    EXPECT_EQ(e2(0), 11.0f);
    EXPECT_EQ(e2(1), 12.0f);
    EXPECT_EQ(e2(2), 13.0f);
    EXPECT_EQ(e2(3), 14.0f);
    EXPECT_FALSE(m_cmp_eq(e1, e2));

    e3(0) = 11.0f;
    e3(1) = 12.0f;
    e3(2) = 13.0f;
    e3(3) = 14.0f;
    EXPECT_EQ(e3(0, 0), 11.0f);
    EXPECT_EQ(e3(0, 1), 12.0f);
    EXPECT_EQ(e3(0, 2), 13.0f);
    EXPECT_EQ(e3(0, 3), 14.0f);
    EXPECT_FALSE(m_cmp_eq(e1, e3));

    //- Verify expected equality of various argument combinations.
    //
    EXPECT_TRUE(m_cmp_eq(e2, e3));

    EXPECT_TRUE(m_cmp_eq(e2, LST_14_2));
    EXPECT_TRUE(m_cmp_eq(e2, il_14_2));
    EXPECT_TRUE(m_cmp_eq(e2, fl_14_2));

    EXPECT_TRUE(m_cmp_eq(e3, LST_14_2));
    EXPECT_TRUE(m_cmp_eq(e3, il_14_2));
    EXPECT_TRUE(m_cmp_eq(e3, fl_14_2));

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
    EXPECT_FALSE(m_cmp_eq(e3, fl_14_0));
    EXPECT_FALSE(m_cmp_eq(e3, il_14_1));
    EXPECT_FALSE(m_cmp_eq(e3, mse_f_ffa_rm(LST_14_1)));
}


TEST(MSE_Matrix_2B, MoveCtor)
{
    //- Default construct and verify initial state.
    //
    mse_f_ffa_rm    e1;

    EXPECT_EQ(e1.rows(), 1);
    EXPECT_EQ(e1.columns(), 4);
    EXPECT_EQ(e1.row_capacity(), 1);
    EXPECT_EQ(e1.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e1, LST_14_0));

    //- Assign new element values and verify them.
    //
    e1(0, 0) = 11.0f;
    e1(0, 1) = 12.0f;
    e1(0, 2) = 13.0f;
    e1(0, 3) = 14.0f;
    EXPECT_TRUE(m_cmp_eq(e1, fl_14_2));

    //- Construct a new engine via move ctor and verify that its initial state is as expected.
    //
    mse_f_ffa_rm    e2(std::move(e1));

    EXPECT_EQ(e1.rows(), 1);
    EXPECT_EQ(e1.columns(), 4);
    EXPECT_EQ(e2.row_capacity(), 1);
    EXPECT_EQ(e2.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e2, fl_14_2));
}


TEST(MSE_Matrix_2B, CopyCtor)
{
    //- Default construct and verify initial state.
    //
    mse_f_ffa_rm    e1;

    EXPECT_EQ(e1.rows(), 1);
    EXPECT_EQ(e1.columns(), 4);
    EXPECT_EQ(e1.row_capacity(), 1);
    EXPECT_EQ(e1.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e1, LST_14_0));

    //- Assign new element values and verify them.
    //
    e1(0, 0) = 11.0f;
    e1(0, 1) = 12.0f;
    e1(0, 2) = 13.0f;
    e1(0, 3) = 14.0f;
    EXPECT_TRUE(m_cmp_eq(e1, fl_14_2));

    //- Construct new engines via copy ctors and verify their initial states.
    //
    mse_f_ffa_rm    e2(e1);

    EXPECT_EQ(e1.rows(), 1);
    EXPECT_EQ(e1.columns(), 4);
    EXPECT_EQ(e2.row_capacity(), 1);
    EXPECT_EQ(e2.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e2, e1));

    mse_f_ffa_rm    e3 = e1;

    EXPECT_EQ(e1.rows(), 1);
    EXPECT_EQ(e1.columns(), 4);
    EXPECT_EQ(e3.row_capacity(), 1);
    EXPECT_EQ(e3.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e3, e1));

    mse_f_ffa_rm    e4{e1};

    EXPECT_EQ(e1.rows(), 1);
    EXPECT_EQ(e1.columns(), 4);
    EXPECT_EQ(e4.row_capacity(), 1);
    EXPECT_EQ(e4.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e4, e1));
}


TEST(MSE_Matrix_2B, ListCtor)
{
    //- Construct new engines via list ctor and verify that their initial states.
    //
    mse_f_ffa_rm    e1 = LST_14_1;
    mse_f_ffa_rm    e2 LST_14_1 ;
    mse_f_ffa_rm    e3(LST_14_1);

    EXPECT_EQ(e1.rows(), 1);
    EXPECT_EQ(e1.columns(), 4);
    EXPECT_EQ(e1.row_capacity(), 1);
    EXPECT_EQ(e1.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e1, LST_14_1));

    EXPECT_EQ(e2.rows(), 1);
    EXPECT_EQ(e2.columns(), 4);
    EXPECT_EQ(e2.row_capacity(), 1);
    EXPECT_EQ(e2.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e2, e1));

    EXPECT_EQ(e3.rows(), 1);
    EXPECT_EQ(e3.columns(), 4);
    EXPECT_EQ(e3.row_capacity(), 1);
    EXPECT_EQ(e3.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e3, e1));

    mse_f_ffa_rm    e4 = LST_4_1;
    mse_f_ffa_rm    e5 LST_4_1 ;
    mse_f_ffa_rm    e6(LST_4_1);

    EXPECT_EQ(e4.rows(), 1);
    EXPECT_EQ(e4.columns(), 4);
    EXPECT_EQ(e4.row_capacity(), 1);
    EXPECT_EQ(e4.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e4, LST_14_1));

    EXPECT_EQ(e5.rows(), 1);
    EXPECT_EQ(e5.columns(), 4);
    EXPECT_EQ(e5.row_capacity(), 1);
    EXPECT_EQ(e5.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e2, e1));

    EXPECT_EQ(e6.rows(), 1);
    EXPECT_EQ(e6.columns(), 4);
    EXPECT_EQ(e6.row_capacity(), 1);
    EXPECT_EQ(e6.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e3, e1));
}


TEST(MSE_Matrix_2B, EngineCtor)
{
    //- Construct new engines via direct engine ctor and verify their initial states.
    //
    mse_i_ffa_cm    e1 = LST_4_2;
    mse_f_ffa_rm    e2(e1);
    mse_d_ffa_rm    e3(e2);
    mse_f_ffa_cm    e4 = LST_4_2;
    mse_f_ffa_rm    e5{e4};

    EXPECT_EQ(e1.rows(), 1);
    EXPECT_EQ(e1.columns(), 4);
    EXPECT_EQ(e1.row_capacity(), 1);
    EXPECT_EQ(e1.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e1, il_14_2));

    EXPECT_EQ(e2.rows(), 1);
    EXPECT_EQ(e2.columns(), 4);
    EXPECT_EQ(e2.row_capacity(), 1);
    EXPECT_EQ(e2.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e2, e1));

    EXPECT_EQ(e3.rows(), 1);
    EXPECT_EQ(e3.columns(), 4);
    EXPECT_EQ(e3.row_capacity(), 1);
    EXPECT_EQ(e3.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e3, e1));

    EXPECT_EQ(e4.rows(), 1);
    EXPECT_EQ(e4.columns(), 4);
    EXPECT_EQ(e4.row_capacity(), 1);
    EXPECT_EQ(e4.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e4, e1));

    EXPECT_EQ(e5.rows(), 1);
    EXPECT_EQ(e5.columns(), 4);
    EXPECT_EQ(e5.row_capacity(), 1);
    EXPECT_EQ(e5.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e5, e1));
}


TEST(MSE_Matrix_2B, MoveAssign)
{
    //- Default construct and verify initial state.
    //
    mse_f_ffa_rm    e1;

    EXPECT_EQ(e1.rows(), 1);
    EXPECT_EQ(e1.columns(), 4);
    EXPECT_EQ(e1.row_capacity(), 1);
    EXPECT_EQ(e1.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e1, il_14_0));

    //- List construct and verify initial state.
    //
    mse_f_ffa_rm    e2(il_4_1);

    EXPECT_EQ(e2.rows(), 1);
    EXPECT_EQ(e2.columns(), 4);
    EXPECT_EQ(e2.row_capacity(), 1);
    EXPECT_EQ(e2.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e2, fl_14_1));

    //- Move-assign and verify.
    //
    e1 = std::move(e2);
    EXPECT_TRUE(m_cmp_eq(e1, fl_14_1));
}


TEST(MSE_Matrix_2B, CopyAssign)
{
    //- Default construct and verify initial state.
    //
    mse_f_ffa_rm    e1;

    EXPECT_EQ(e1.rows(), 1);
    EXPECT_EQ(e1.columns(), 4);
    EXPECT_EQ(e1.row_capacity(), 1);
    EXPECT_EQ(e1.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e1, il_14_0));

    //- List construct and verify initial state.
    //
    mse_f_ffa_rm    e2(il_14_1);

    EXPECT_EQ(e2.rows(), 1);
    EXPECT_EQ(e2.columns(), 4);
    EXPECT_EQ(e2.row_capacity(), 1);
    EXPECT_EQ(e2.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e2, fl_14_1));

    //- Copy-assign and verify.
    //
    e1 = e2;
    EXPECT_TRUE(m_cmp_eq(e1, fl_14_1));
}


TEST(MSE_Matrix_2B, EngineAssign)
{
    //- Default construct and verify initial state.
    //
    mse_f_ffa_rm    e1, e2;

    EXPECT_EQ(e1.rows(), 1);
    EXPECT_EQ(e1.columns(), 4);
    EXPECT_EQ(e1.row_capacity(), 1);
    EXPECT_EQ(e1.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e1, LST_14_0));

    EXPECT_EQ(e2.rows(), 1);
    EXPECT_EQ(e2.columns(), 4);
    EXPECT_EQ(e2.row_capacity(), 1);
    EXPECT_EQ(e2.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e2, fl_14_0));

    //- List construct and verify initial state.
    //
    mse_i_ffa_cm    e3 LST_14_1 ;

    EXPECT_EQ(e3.rows(), 1);
    EXPECT_EQ(e3.columns(), 4);
    EXPECT_EQ(e3.row_capacity(), 1);
    EXPECT_EQ(e3.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e3, il_14_1));

    mse_i_ffa_cm    e4 LST_4_2 ;

    EXPECT_EQ(e4.rows(), 1);
    EXPECT_EQ(e4.columns(), 4);
    EXPECT_EQ(e4.row_capacity(), 1);
    EXPECT_EQ(e4.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e4, il_14_2));

    EXPECT_FALSE(m_cmp_eq(e3, e4));

    //- Assign and verify.
    //
    e1 = e3;
    EXPECT_TRUE(m_cmp_eq(e1, fl_14_1));
    EXPECT_TRUE(m_cmp_eq(e1, e3));

    e2 = e4;
    EXPECT_TRUE(m_cmp_eq(e2, fl_14_2));
    EXPECT_TRUE(m_cmp_eq(e2, e4));
}


TEST(MSE_Matrix_2B, ListAssign)
{
    //- Default construct and verify initial state.
    //
    mse_f_ffa_rm     e1;
    mse_f_ffa_cm     e2{il_4_1};

    EXPECT_EQ(e1.rows(), 1);
    EXPECT_EQ(e1.columns(), 4);
    EXPECT_EQ(e1.row_capacity(), 1);
    EXPECT_EQ(e1.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e1, LST_14_0));

    EXPECT_EQ(e2.rows(), 1);
    EXPECT_EQ(e2.columns(), 4);
    EXPECT_EQ(e2.row_capacity(), 1);
    EXPECT_EQ(e2.column_capacity(), 4);
    EXPECT_TRUE(m_cmp_eq(e2, LST_14_1));

    //- Assign and verify.
    //
    e1 = LST_14_1;
    EXPECT_TRUE(m_cmp_eq(e1, LST_14_1));
    EXPECT_TRUE(m_cmp_eq(e1, il_14_1));
    EXPECT_TRUE(m_cmp_eq(e1, fl_14_1));
    EXPECT_TRUE(m_cmp_eq(e1, e2));

    e1 = fl_14_0;
    EXPECT_TRUE(m_cmp_eq(e1, LST_14_0));
    EXPECT_TRUE(m_cmp_eq(e1, il_14_0));
    EXPECT_TRUE(m_cmp_eq(e1, fl_14_0));
    EXPECT_TRUE(m_cmp_eq(e1, mse_f_ffa_rm()));

    e1 = LST_4_2;
    EXPECT_TRUE(m_cmp_eq(e1, LST_14_2));
    EXPECT_TRUE(m_cmp_eq(e1, il_14_2));
    EXPECT_TRUE(m_cmp_eq(e1, fl_14_2));
    EXPECT_FALSE(m_cmp_eq(e1, e2));

    e1 = fl_4_0;
    EXPECT_TRUE(m_cmp_eq(e1, LST_14_0));
    EXPECT_TRUE(m_cmp_eq(e1, il_14_0));
    EXPECT_TRUE(m_cmp_eq(e1, fl_14_0));
    EXPECT_TRUE(m_cmp_eq(e1, mse_f_ffa_rm()));
}


TEST(MSE_Matrix_2B, Swap)
{
    //- Default construct and verify initial state.
    //
    mse_f_ffa_rm    e1;

    EXPECT_EQ(e1.rows(), 1);
    EXPECT_EQ(e1.columns(), 4);
    EXPECT_TRUE(m_cmp_eq(e1, fl_14_0));

    //- List construct and verify initial state.
    //
    mse_f_ffa_rm    e2(fl_4_2);

    EXPECT_EQ(e2.rows(), 1);
    EXPECT_EQ(e2.columns(), 4);
    EXPECT_TRUE(m_cmp_eq(e2, fl_14_2));

    //- Swap contents and verify.
    //
    e1.swap(e2);
    EXPECT_TRUE(m_cmp_eq(e1, fl_14_2));
    EXPECT_TRUE(m_cmp_eq(e2, fl_14_0));

    e2.swap(e1);
    EXPECT_TRUE(m_cmp_eq(e1, il_14_0));
    EXPECT_TRUE(m_cmp_eq(e2, il_14_2));
}


TEST(MSE_Matrix_2B, Span)
{
    mse_f_ffa_rm        e1(il_14_2), e2, e3{e1};
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

    EXPECT_EQ(csp1(0, 0), 11.0f);
    EXPECT_EQ(csp1(0, 1), 12.0f);
    EXPECT_EQ(csp1(0, 2), 13.0f);
    EXPECT_EQ(csp1(0, 3), 14.0f);

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
    sp1(0, 1) = 222.0f;
    sp1(0, 2) = 333.0f;
    sp1(0, 3) = 444.0f;
    EXPECT_EQ(e1(0), 111.0f);
    EXPECT_EQ(e1(1), 222.0f);
    EXPECT_EQ(e1(2), 333.0f);
    EXPECT_EQ(e1(3), 444.0f);
}
