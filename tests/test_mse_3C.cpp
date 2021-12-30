#include "test_common.hpp"

using namespace STD_LA;
using namespace STD_LA::detail;
using namespace MDSPAN_NS;

//==================================================================================================
//  Unit tests for resizable, dynamically allocated matrix engine, Case 3C (column matrix)
//==================================================================================================
//
using mse_f_df_cm = matrix_storage_engine<float, dynamic_extent, 1, std::allocator<float>, matrix_layout::column_major>;
using mse_f_df_rm = matrix_storage_engine<float, dynamic_extent, 1, std::allocator<float>, matrix_layout::row_major>;
using mse_i_df_cm = matrix_storage_engine<int, dynamic_extent, 1, std::allocator<int>, matrix_layout::column_major>;
using mse_d_df_rm = matrix_storage_engine<double, dynamic_extent, 1, std::allocator<double>, matrix_layout::row_major>;

using msupport = matrix_engine_support;

TEST(MSE_Matrix_3C, DefaultCtor)
{
    mse_f_df_rm    e1;

    EXPECT_EQ(e1.rows(), 0);
    EXPECT_EQ(e1.columns(), 1);
    EXPECT_EQ(e1.row_capacity(), 0);
    EXPECT_EQ(e1.column_capacity(), 1);

    e1.reshape_rows(4, 0);
    EXPECT_EQ(e1.rows(), 4);
    EXPECT_EQ(e1.columns(), 1);
    EXPECT_EQ(e1.row_capacity(), 4);
    EXPECT_EQ(e1.column_capacity(), 1);

    //- Verify elements are value initialized.
    //
    EXPECT_EQ(e1(0, 0), 0.0f);
    EXPECT_EQ(e1(1, 0), 0.0f);
    EXPECT_EQ(e1(2, 0), 0.0f);
    EXPECT_EQ(e1(3, 0), 0.0f);
    EXPECT_EQ(e1(0), 0.0f);
    EXPECT_EQ(e1(1), 0.0f);
    EXPECT_EQ(e1(2), 0.0f);
    EXPECT_EQ(e1(3), 0.0f);
}


TEST(MSE_Matrix_3C, CmpEq)
{
    mse_f_df_rm    e1, e2, e3;

    std::array<float, 4>        a1 = LST_4_0;
    mdspan<float, extents<4>>   s1(a1.data());

    std::array<float, 4>        a2 = LST_4_2;
    mdspan<float, extents<4>>   s2(a2.data());

    std::array<float, 5>        a3 = LST_5_2;
    mdspan<float, extents<5>>   s3(a2.data());

    std::array<float, 4>        a4 = LST_4_3;
    mdspan<float, extents<4>>   s4(a4.data());

    e1.reshape_rows(4, 0);
    e2.reshape_rows(4, 0);
    e3.reshape_rows(4, 0);

    //- Verify size, capacity, initial values.
    //
    EXPECT_EQ(e1.rows(), 4);
    EXPECT_EQ(e1.columns(), 1);
    EXPECT_EQ(e1.row_capacity(), 4);
    EXPECT_EQ(e1.column_capacity(), 1);

    EXPECT_EQ(e1(0, 0), 0.0f);
    EXPECT_EQ(e1(1, 0), 0.0f);
    EXPECT_EQ(e1(2, 0), 0.0f);
    EXPECT_EQ(e1(3, 0), 0.0f);
    EXPECT_EQ(e1(0), 0.0f);
    EXPECT_EQ(e1(1), 0.0f);
    EXPECT_EQ(e1(2), 0.0f);
    EXPECT_EQ(e1(3), 0.0f);

    //- Verify expected equality of default-constructed engines.
    //
    EXPECT_TRUE(msupport::compare(e1, e1));
    EXPECT_TRUE(msupport::compare(e1, e2));
    EXPECT_TRUE(msupport::compare(e1, e3));

    //- Verify equality against an init-list of equivalent values.
    //
    EXPECT_TRUE(msupport::compare(e1, LST_41_0));
    EXPECT_TRUE(msupport::compare(e1, il_41_0));
    EXPECT_TRUE(msupport::compare(e1, fl_41_0));
    EXPECT_TRUE(msupport::compare(e1, a1));
    EXPECT_TRUE(msupport::compare(e1, s1));

    //- Verify inequality against an init-list of different values.
    //
    EXPECT_FALSE(msupport::compare(e1, LST_41_1));
    EXPECT_FALSE(msupport::compare(e1, il_41_1));
    EXPECT_FALSE(msupport::compare(e1, fl_41_1));
    EXPECT_FALSE(msupport::compare(e1, a2));
    EXPECT_FALSE(msupport::compare(e1, s2));

    //- Assign new element values via mutable indexing and verify them.
    //
    e2(0, 0) = 11.0f;
    e2(1, 0) = 12.0f;
    e2(2, 0) = 13.0f;
    e2(3, 0) = 14.0f;
    EXPECT_EQ(e2(0), 11.0f);
    EXPECT_EQ(e2(1), 12.0f);
    EXPECT_EQ(e2(2), 13.0f);
    EXPECT_EQ(e2(3), 14.0f);
    EXPECT_FALSE(msupport::compare(e1, e2));

    e3(0) = 11.0f;
    e3(1) = 12.0f;
    e3(2) = 13.0f;
    e3(3) = 14.0f;
    EXPECT_EQ(e3(0, 0), 11.0f);
    EXPECT_EQ(e3(1, 0), 12.0f);
    EXPECT_EQ(e3(2, 0), 13.0f);
    EXPECT_EQ(e3(3, 0), 14.0f);
    EXPECT_FALSE(msupport::compare(e1, e3));

    //- Verify expected equality of various argument combinations.
    //
    EXPECT_TRUE(msupport::compare(e2, e3));

    EXPECT_TRUE(msupport::compare(e2, LST_41_2));
    EXPECT_TRUE(msupport::compare(e2, il_41_2));
    EXPECT_TRUE(msupport::compare(e2, fl_41_2));
    EXPECT_TRUE(msupport::compare(e2, a2));
    EXPECT_TRUE(msupport::compare(e2, s2));

    EXPECT_TRUE(msupport::compare(e3, LST_41_2));
    EXPECT_TRUE(msupport::compare(e3, il_41_2));
    EXPECT_TRUE(msupport::compare(e3, fl_41_2));
    EXPECT_TRUE(msupport::compare(e3, a2));
    EXPECT_TRUE(msupport::compare(e3, s2));

    EXPECT_TRUE(msupport::compare(e2, e3));
    EXPECT_FALSE(msupport::compare(e2, e1));
    EXPECT_FALSE(msupport::compare(e1, e3));

    //- Verify expected inequality when compared to init-lists of incorrect size.
    //
    EXPECT_FALSE(msupport::compare(e3, LST_33_0));
    EXPECT_FALSE(msupport::compare(e3, il_33_1));
    EXPECT_FALSE(msupport::compare(e3, fl_33_2));
    EXPECT_FALSE(msupport::compare(e3, a3));
    EXPECT_FALSE(msupport::compare(e3, s3));

    //- Verify expected inequality against init-lists and engines having different contents.
    //
    EXPECT_FALSE(msupport::compare(e3, fl_41_0));
    EXPECT_FALSE(msupport::compare(e3, il_41_1));
    EXPECT_FALSE(msupport::compare(e3, mse_f_df_rm(LST_41_1)));
    EXPECT_FALSE(msupport::compare(e3, a4));
    EXPECT_FALSE(msupport::compare(e3, s4));
}


TEST(MSE_Matrix_3C, MoveCtor)
{
    //- Default construct and verify initial state.
    //
    mse_f_df_rm    e1;

    e1.reshape_rows(4, 0);
    EXPECT_EQ(e1.rows(), 4);
    EXPECT_EQ(e1.columns(), 1);
    EXPECT_EQ(e1.row_capacity(), 4);
    EXPECT_EQ(e1.column_capacity(), 1);
    EXPECT_TRUE(msupport::compare(e1, LST_41_0));

    //- Assign new element values and verify them.
    //
    e1(0, 0) = 11.0f;
    e1(1, 0) = 12.0f;
    e1(2, 0) = 13.0f;
    e1(3, 0) = 14.0f;
    EXPECT_TRUE(msupport::compare(e1, fl_41_2));

    //- Construct a new engine via move ctor and verify that its initial state is as expected.
    //
    mse_f_df_rm    e2(std::move(e1));

    EXPECT_EQ(e1.rows(), 4);
    EXPECT_EQ(e1.columns(), 1);
    EXPECT_EQ(e2.row_capacity(), 4);
    EXPECT_EQ(e2.column_capacity(), 1);
    EXPECT_TRUE(msupport::compare(e2, fl_41_2));
}


TEST(MSE_Matrix_3C, CopyCtor)
{
    //- Default construct and verify initial state.
    //
    mse_f_df_rm    e1;

    e1.reshape_rows(4, 0);
    EXPECT_EQ(e1.rows(), 4);
    EXPECT_EQ(e1.columns(), 1);
    EXPECT_EQ(e1.row_capacity(), 4);
    EXPECT_EQ(e1.column_capacity(), 1);
    EXPECT_TRUE(msupport::compare(e1, LST_41_0));

    //- Assign new element values and verify them.
    //
    e1(0, 0) = 11.0f;
    e1(1, 0) = 12.0f;
    e1(2, 0) = 13.0f;
    e1(3, 0) = 14.0f;
    EXPECT_TRUE(msupport::compare(e1, fl_41_2));

    //- Construct new engines via copy ctors and verify their initial states.
    //
    mse_f_df_rm    e2(e1);

    EXPECT_EQ(e2.rows(), 4);
    EXPECT_EQ(e2.columns(), 1);
    EXPECT_EQ(e2.row_capacity(), 4);
    EXPECT_EQ(e2.column_capacity(), 1);
    EXPECT_TRUE(msupport::compare(e2, e1));

    mse_f_df_rm    e3 = e1;

    EXPECT_EQ(e3.rows(), 4);
    EXPECT_EQ(e3.columns(), 1);
    EXPECT_EQ(e3.row_capacity(), 4);
    EXPECT_EQ(e3.column_capacity(), 1);
    EXPECT_TRUE(msupport::compare(e3, e1));

    mse_f_df_rm    e4{e1};

    EXPECT_EQ(e4.rows(), 4);
    EXPECT_EQ(e4.columns(), 1);
    EXPECT_EQ(e4.row_capacity(), 4);
    EXPECT_EQ(e4.column_capacity(), 1);
    EXPECT_TRUE(msupport::compare(e4, e1));
}


TEST(MSE_Matrix_3C, ListCtor)
{
    //- Construct new engines via list ctor and verify that their initial states.
    //
    mse_f_df_rm    e1 = LST_41_1;
    mse_f_df_rm    e2 LST_41_1 ;
    mse_f_df_rm    e3(LST_41_1);

    EXPECT_EQ(e1.rows(), 4);
    EXPECT_EQ(e1.columns(), 1);
    EXPECT_EQ(e1.row_capacity(), 4);
    EXPECT_EQ(e1.column_capacity(), 1);
    EXPECT_TRUE(msupport::compare(e1, LST_41_1));

    EXPECT_EQ(e2.rows(), 4);
    EXPECT_EQ(e2.columns(), 1);
    EXPECT_EQ(e2.row_capacity(), 4);
    EXPECT_EQ(e2.column_capacity(), 1);
    EXPECT_TRUE(msupport::compare(e2, e1));

    EXPECT_EQ(e3.rows(), 4);
    EXPECT_EQ(e3.columns(), 1);
    EXPECT_EQ(e3.row_capacity(), 4);
    EXPECT_EQ(e3.column_capacity(), 1);
    EXPECT_TRUE(msupport::compare(e3, e1));

    mse_f_df_rm    e4 = LST_4_1;
    mse_f_df_rm    e5 LST_4_1 ;
    mse_f_df_rm    e6(LST_4_1);

    EXPECT_EQ(e4.rows(), 4);
    EXPECT_EQ(e4.columns(), 1);
    EXPECT_EQ(e4.row_capacity(), 4);
    EXPECT_EQ(e4.column_capacity(), 1);
    EXPECT_TRUE(msupport::compare(e4, LST_41_1));

    EXPECT_EQ(e5.rows(), 4);
    EXPECT_EQ(e5.columns(), 1);
    EXPECT_EQ(e5.row_capacity(), 4);
    EXPECT_EQ(e5.column_capacity(), 1);
    EXPECT_TRUE(msupport::compare(e2, e1));

    EXPECT_EQ(e6.rows(), 4);
    EXPECT_EQ(e6.columns(), 1);
    EXPECT_EQ(e6.row_capacity(), 4);
    EXPECT_EQ(e6.column_capacity(), 1);
    EXPECT_TRUE(msupport::compare(e3, e1));
}


TEST(MSE_Matrix_3C, EngineCtor)
{
    //- Construct new engines via direct engine ctor and verify their initial states.
    //
    mse_i_df_cm    e1 = LST_4_2;
    mse_f_df_rm    e2(e1);
    mse_d_df_rm    e3(e2);
    mse_f_df_cm    e4 = LST_4_2;
    mse_f_df_rm    e5{e4};

    EXPECT_EQ(e1.rows(), 4);
    EXPECT_EQ(e1.columns(), 1);
    EXPECT_EQ(e1.row_capacity(), 4);
    EXPECT_EQ(e1.column_capacity(), 1);
    EXPECT_TRUE(msupport::compare(e1, il_41_2));

    EXPECT_EQ(e2.rows(), 4);
    EXPECT_EQ(e2.columns(), 1);
    EXPECT_EQ(e2.row_capacity(), 4);
    EXPECT_EQ(e2.column_capacity(), 1);
    EXPECT_TRUE(msupport::compare(e2, e1));

    EXPECT_EQ(e3.rows(), 4);
    EXPECT_EQ(e3.columns(), 1);
    EXPECT_EQ(e3.row_capacity(), 4);
    EXPECT_EQ(e3.column_capacity(), 1);
    EXPECT_TRUE(msupport::compare(e3, e1));

    EXPECT_EQ(e4.rows(), 4);
    EXPECT_EQ(e4.columns(), 1);
    EXPECT_EQ(e4.row_capacity(), 4);
    EXPECT_EQ(e4.column_capacity(), 1);
    EXPECT_TRUE(msupport::compare(e4, e1));

    EXPECT_EQ(e5.rows(), 4);
    EXPECT_EQ(e5.columns(), 1);
    EXPECT_EQ(e5.row_capacity(), 4);
    EXPECT_EQ(e5.column_capacity(), 1);
    EXPECT_TRUE(msupport::compare(e5, e1));
}


TEST(MSE_Matrix_3C, OtherCtor)
{
    //- Construct new engines via other ctors and verify their initial state.
    //
    std::array<float, 4>        o1 = LST_4_1;
    std::vector<float>          o2 = fl_4_2;
    std::deque<float>           o3 = fl_4_3;
    mdspan<float, extents<4>>   o4(o1.data());

    mse_f_df_rm     e1(o1);
    mse_f_df_cm     e2(o2);
    mse_f_df_rm     e3(o3);
    mse_f_df_cm     e4(o4);

    EXPECT_TRUE(msupport::compare(e1, o1));
    EXPECT_TRUE(msupport::compare(e2, o2));
    EXPECT_TRUE(msupport::compare(e3, o3));
    EXPECT_TRUE(msupport::compare(e4, o4));

    EXPECT_TRUE(msupport::compare(e1, fl_4_1));
    EXPECT_TRUE(msupport::compare(e2, fl_4_2));
    EXPECT_TRUE(msupport::compare(e3, fl_4_3));
    EXPECT_TRUE(msupport::compare(e4, fl_4_1));
}


TEST(MSE_Matrix_3C, MoveAssign)
{
    //- Default construct and verify initial state.
    //
    mse_f_df_rm    e1;

    e1.reshape_rows(4, 0);
    EXPECT_EQ(e1.rows(), 4);
    EXPECT_EQ(e1.columns(), 1);
    EXPECT_EQ(e1.row_capacity(), 4);
    EXPECT_EQ(e1.column_capacity(), 1);
    EXPECT_TRUE(msupport::compare(e1, il_41_0));

    //- List construct and verify initial state.
    //
    mse_f_df_rm    e2(il_4_1);

    EXPECT_EQ(e2.rows(), 4);
    EXPECT_EQ(e2.columns(), 1);
    EXPECT_EQ(e2.row_capacity(), 4);
    EXPECT_EQ(e2.column_capacity(), 1);
    EXPECT_TRUE(msupport::compare(e2, fl_41_1));

    //- Move-assign and verify.
    //
    e1 = std::move(e2);
    EXPECT_TRUE(msupport::compare(e1, fl_41_1));
}


TEST(MSE_Matrix_3C, CopyAssign)
{
    //- Default construct and verify initial state.
    //
    mse_f_df_rm    e1;

    e1.reshape_rows(4, 0);
    EXPECT_EQ(e1.rows(), 4);
    EXPECT_EQ(e1.columns(), 1);
    EXPECT_EQ(e1.row_capacity(), 4);
    EXPECT_EQ(e1.column_capacity(), 1);
    EXPECT_TRUE(msupport::compare(e1, il_41_0));

    //- List construct and verify initial state.
    //
    mse_f_df_rm    e2(il_41_1);

    EXPECT_EQ(e2.rows(), 4);
    EXPECT_EQ(e2.columns(), 1);
    EXPECT_EQ(e2.row_capacity(), 4);
    EXPECT_EQ(e2.column_capacity(), 1);
    EXPECT_TRUE(msupport::compare(e2, fl_41_1));

    //- Copy-assign and verify.
    //
    e1 = e2;
    EXPECT_TRUE(msupport::compare(e1, fl_41_1));
}


TEST(MSE_Matrix_3C, EngineAssign)
{
    //- Default construct and verify initial state.
    //
    mse_f_df_rm    e1, e2;

    e1.reshape_rows(4, 0);
    EXPECT_EQ(e1.rows(), 4);
    EXPECT_EQ(e1.columns(), 1);
    EXPECT_EQ(e1.row_capacity(), 4);
    EXPECT_EQ(e1.column_capacity(), 1);
    EXPECT_TRUE(msupport::compare(e1, LST_41_0));

    e2.reshape_rows(4, 0);
    EXPECT_EQ(e2.rows(), 4);
    EXPECT_EQ(e2.columns(), 1);
    EXPECT_EQ(e2.row_capacity(), 4);
    EXPECT_EQ(e2.column_capacity(), 1);
    EXPECT_TRUE(msupport::compare(e2, fl_41_0));

    //- List construct and verify initial state.
    //
    mse_i_df_cm    e3 LST_41_1 ;

    EXPECT_EQ(e3.rows(), 4);
    EXPECT_EQ(e3.columns(), 1);
    EXPECT_EQ(e3.row_capacity(), 4);
    EXPECT_EQ(e3.column_capacity(), 1);
    EXPECT_TRUE(msupport::compare(e3, il_41_1));

    mse_i_df_cm    e4 LST_4_2 ;

    EXPECT_EQ(e4.rows(), 4);
    EXPECT_EQ(e4.columns(), 1);
    EXPECT_EQ(e4.row_capacity(), 4);
    EXPECT_EQ(e4.column_capacity(), 1);
    EXPECT_TRUE(msupport::compare(e4, il_41_2));

    EXPECT_FALSE(msupport::compare(e3, e4));

    //- Assign and verify.
    //
    e1 = e3;
    EXPECT_TRUE(msupport::compare(e1, fl_41_1));
    EXPECT_TRUE(msupport::compare(e1, e3));

    e2 = e4;
    EXPECT_TRUE(msupport::compare(e2, fl_41_2));
    EXPECT_TRUE(msupport::compare(e2, e4));
}


TEST(MSE_Matrix_3C, ListAssign)
{
    //- Default construct and verify initial state.
    //
    mse_f_df_rm     e1;
    mse_f_df_cm     e2(il_4_1);

    e1.reshape_rows(4, 0);
    EXPECT_EQ(e1.rows(), 4);
    EXPECT_EQ(e1.columns(), 1);
    EXPECT_EQ(e1.row_capacity(), 4);
    EXPECT_EQ(e1.column_capacity(), 1);
    EXPECT_TRUE(msupport::compare(e1, LST_41_0));

    EXPECT_EQ(e2.rows(), 4);
    EXPECT_EQ(e2.columns(), 1);
    EXPECT_EQ(e2.row_capacity(), 4);
    EXPECT_EQ(e2.column_capacity(), 1);
    EXPECT_TRUE(msupport::compare(e2, LST_41_1));

    //- Assign and verify.
    //
    e1 = LST_41_1;
    EXPECT_TRUE(msupport::compare(e1, LST_41_1));
    EXPECT_TRUE(msupport::compare(e1, il_41_1));
    EXPECT_TRUE(msupport::compare(e1, fl_41_1));
    EXPECT_TRUE(msupport::compare(e1, e2));

    e1 = fl_41_0;
    EXPECT_TRUE(msupport::compare(e1, LST_41_0));
    EXPECT_TRUE(msupport::compare(e1, il_41_0));
    EXPECT_TRUE(msupport::compare(e1, fl_41_0));

    e1 = LST_4_2;
    EXPECT_TRUE(msupport::compare(e1, LST_41_2));
    EXPECT_TRUE(msupport::compare(e1, il_41_2));
    EXPECT_TRUE(msupport::compare(e1, fl_41_2));
    EXPECT_FALSE(msupport::compare(e1, e2));

    e1 = fl_4_0;
    EXPECT_TRUE(msupport::compare(e1, LST_41_0));
    EXPECT_TRUE(msupport::compare(e1, il_41_0));
    EXPECT_TRUE(msupport::compare(e1, fl_41_0));
}


TEST(MSE_Matrix_3C, OtherAssign)
{
    //- Default construct and verify initial state.
    //
    mse_f_df_rm      e1;
    mse_f_df_cm      e2 = LST_5_2;

    e1.reshape_rows(4, 0);
    EXPECT_EQ(e1.rows(), 4);
    EXPECT_EQ(e1.columns(), 1);
    EXPECT_EQ(e1.row_capacity(), 4);
    EXPECT_EQ(e1.column_capacity(), 1);
    EXPECT_TRUE(msupport::compare(e1, LST_41_0));

    EXPECT_EQ(e2.rows(), 5);
    EXPECT_EQ(e2.columns(), 1);
    EXPECT_EQ(e2.row_capacity(), 5);
    EXPECT_EQ(e2.column_capacity(), 1);
    EXPECT_TRUE(msupport::compare(e2, fl_51_2));

    //- Assign and verify.
    //
    std::array<float, 4>        o1 = LST_4_1;
    std::vector<float>          o2 = fl_4_2;
    std::deque<float>           o3 = fl_4_3;
    mdspan<float, extents<4>>   o4(o1.data());

    e1 = o1;
    EXPECT_TRUE(msupport::compare(e1, o1));
    EXPECT_TRUE(msupport::compare(e1, fl_4_1));

    e2 = o2;
    EXPECT_TRUE(msupport::compare(e2, o2));
    EXPECT_TRUE(msupport::compare(e2, fl_4_2));

    e1 = o3;
    EXPECT_TRUE(msupport::compare(e1, o3));
    EXPECT_TRUE(msupport::compare(e1, fl_4_3));

    e2 = o4;
    EXPECT_TRUE(msupport::compare(e2, o4));
    EXPECT_TRUE(msupport::compare(e2, fl_4_1));
}


TEST(MSE_Matrix_3C, Swap)
{
    //- Default construct and verify initial state.
    //
    mse_f_df_rm    e1;

    e1.reshape_rows(4, 0);
    EXPECT_EQ(e1.rows(), 4);
    EXPECT_EQ(e1.columns(), 1);
    EXPECT_TRUE(msupport::compare(e1, fl_41_0));

    //- List construct and verify initial state.
    //
    mse_f_df_rm    e2(fl_4_2);

    EXPECT_EQ(e2.rows(), 4);
    EXPECT_EQ(e2.columns(), 1);
    EXPECT_TRUE(msupport::compare(e2, fl_41_2));

    //- Swap contents and verify.
    //
    e1.swap(e2);
    EXPECT_TRUE(msupport::compare(e1, fl_41_2));
    EXPECT_TRUE(msupport::compare(e2, fl_41_0));

    e2.swap(e1);
    EXPECT_TRUE(msupport::compare(e1, il_41_0));
    EXPECT_TRUE(msupport::compare(e2, il_41_2));
}


TEST(MSE_Matrix_3C, Span)
{
    mse_f_df_rm        e1(il_41_2), e2, e3{e1};
    mse_f_df_rm const& ce1 = e1;

    auto    sp1  = e1.span();
    auto    csp1 = ce1.span();

    e2.reshape_rows(4, 0);

    //- Const and mutable spans should have different types.
    //
    EXPECT_FALSE((std::is_same_v<decltype(sp1), decltype(csp1)>));

    //- The elements of the const and mutable spans should have the expected values.
    //
    EXPECT_EQ(sp1(0, 0), 11.0f);
    EXPECT_EQ(sp1(1, 0), 12.0f);
    EXPECT_EQ(sp1(2, 0), 13.0f);
    EXPECT_EQ(sp1(3, 0), 14.0f);

    EXPECT_EQ(csp1(0, 0), 11.0f);
    EXPECT_EQ(csp1(1, 0), 12.0f);
    EXPECT_EQ(csp1(2, 0), 13.0f);
    EXPECT_EQ(csp1(3, 0), 14.0f);

    //- Whole-object comparison between engines and spans should work as expected.
    //
    EXPECT_TRUE(msupport::compare(e1, sp1));
    EXPECT_TRUE(msupport::compare(e1, csp1));
    EXPECT_TRUE(msupport::compare(e3, sp1));
    EXPECT_TRUE(msupport::compare(e3, csp1));
    EXPECT_FALSE(msupport::compare(e2, sp1));
    EXPECT_FALSE(msupport::compare(e2, csp1));

    //- Setting values of individual span elements should be reflected in the owning engine.
    //
    sp1(0, 0) = 111.0f;
    sp1(1, 0) = 222.0f;
    sp1(2, 0) = 333.0f;
    sp1(3, 0) = 444.0f;
    EXPECT_EQ(e1(0), 111.0f);
    EXPECT_EQ(e1(1), 222.0f);
    EXPECT_EQ(e1(2), 333.0f);
    EXPECT_EQ(e1(3), 444.0f);
}

