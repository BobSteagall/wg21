#include "test_common.hpp"

using namespace STD_LA;
using namespace STD_LA::detail;
using namespace MDSPAN_NS;

//==================================================================================================
//  Unit tests for fixed-size, statically allocated matrix engine, Case 2A (row matrix)
//==================================================================================================
//
using mse_f_ffv_cm = matrix_storage_engine<float, extents<1, 4>, void, column_major>;
using mse_f_ffv_rm = matrix_storage_engine<float, extents<1, 4>, void, row_major>;
using mse_i_ffv_cm = matrix_storage_engine<int, extents<1, 4>, void, column_major>;
using mse_d_ffv_rm = matrix_storage_engine<double, extents<1, 4>, void, column_major>;

using msupport = matrix_engine_support;

TEST(MSE_Matrix_2A, DefaultCtor)
{
    mse_f_ffv_rm    e1;

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


TEST(MSE_Matrix_2A, CmpEq)
{
    mse_f_ffv_rm    e1, e2, e3;

    std::array<float, 4>    a1 = LST_4_0;
    mdspan<float, 4>        s1(a1.data());

    std::array<float, 4>    a2 = LST_4_2;
    mdspan<float, 4>        s2(a2.data());

    std::array<float, 5>    a3 = LST_5_2;
    mdspan<float, 5>        s3(a2.data());

    std::array<float, 4>    a4 = LST_4_3;
    mdspan<float, 4>        s4(a4.data());

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
    EXPECT_TRUE(msupport::compare(e1, e1));
    EXPECT_TRUE(msupport::compare(e1, e2));
    EXPECT_TRUE(msupport::compare(e1, e3));

    //- Verify equality against an init-list of equivalent values.
    //
    EXPECT_TRUE(msupport::compare(e1, LST_14_0));
    EXPECT_TRUE(msupport::compare(e1, il_14_0));
    EXPECT_TRUE(msupport::compare(e1, fl_14_0));
    EXPECT_TRUE(msupport::compare(e1, a1));
    EXPECT_TRUE(msupport::compare(e1, s1));

    //- Verify inequality against an init-list of different values.
    //
    EXPECT_FALSE(msupport::compare(e1, LST_14_1));
    EXPECT_FALSE(msupport::compare(e1, il_14_1));
    EXPECT_FALSE(msupport::compare(e1, fl_14_1));
    EXPECT_FALSE(msupport::compare(e1, a2));
    EXPECT_FALSE(msupport::compare(e1, s2));

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
    EXPECT_FALSE(msupport::compare(e1, e2));

    e3(0) = 11.0f;
    e3(1) = 12.0f;
    e3(2) = 13.0f;
    e3(3) = 14.0f;
    EXPECT_EQ(e3(0, 0), 11.0f);
    EXPECT_EQ(e3(0, 1), 12.0f);
    EXPECT_EQ(e3(0, 2), 13.0f);
    EXPECT_EQ(e3(0, 3), 14.0f);
    EXPECT_FALSE(msupport::compare(e1, e3));

    //- Verify expected equality of various argument combinations.
    //
    EXPECT_TRUE(msupport::compare(e2, e3));

    EXPECT_TRUE(msupport::compare(e2, LST_14_2));
    EXPECT_TRUE(msupport::compare(e2, il_14_2));
    EXPECT_TRUE(msupport::compare(e2, fl_14_2));
    EXPECT_TRUE(msupport::compare(e2, a2));
    EXPECT_TRUE(msupport::compare(e2, s2));

    EXPECT_TRUE(msupport::compare(e3, LST_14_2));
    EXPECT_TRUE(msupport::compare(e3, il_14_2));
    EXPECT_TRUE(msupport::compare(e3, fl_14_2));
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
    EXPECT_FALSE(msupport::compare(e3, fl_14_0));
    EXPECT_FALSE(msupport::compare(e3, il_14_1));
    EXPECT_FALSE(msupport::compare(e3, mse_f_ffv_rm(LST_14_1)));
    EXPECT_FALSE(msupport::compare(e3, a4));
    EXPECT_FALSE(msupport::compare(e3, s4));
}


TEST(MSE_Matrix_2A, MoveCtor)
{
    //- Default construct and verify initial state.
    //
    mse_f_ffv_rm    e1;

    EXPECT_EQ(e1.rows(), 1);
    EXPECT_EQ(e1.columns(), 4);
    EXPECT_EQ(e1.row_capacity(), 1);
    EXPECT_EQ(e1.column_capacity(), 4);
    EXPECT_TRUE(msupport::compare(e1, LST_14_0));

    //- Assign new element values and verify them.
    //
    e1(0, 0) = 11.0f;
    e1(0, 1) = 12.0f;
    e1(0, 2) = 13.0f;
    e1(0, 3) = 14.0f;
    EXPECT_TRUE(msupport::compare(e1, fl_14_2));

    //- Construct a new engine via move ctor and verify that its initial state is as expected.
    //
    mse_f_ffv_rm    e2(std::move(e1));

    EXPECT_EQ(e1.rows(), 1);
    EXPECT_EQ(e1.columns(), 4);
    EXPECT_EQ(e2.row_capacity(), 1);
    EXPECT_EQ(e2.column_capacity(), 4);
    EXPECT_TRUE(msupport::compare(e2, fl_14_2));
}


TEST(MSE_Matrix_2A, CopyCtor)
{
    //- Default construct and verify initial state.
    //
    mse_f_ffv_rm    e1;

    EXPECT_EQ(e1.rows(), 1);
    EXPECT_EQ(e1.columns(), 4);
    EXPECT_EQ(e1.row_capacity(), 1);
    EXPECT_EQ(e1.column_capacity(), 4);
    EXPECT_TRUE(msupport::compare(e1, LST_14_0));

    //- Assign new element values and verify them.
    //
    e1(0, 0) = 11.0f;
    e1(0, 1) = 12.0f;
    e1(0, 2) = 13.0f;
    e1(0, 3) = 14.0f;
    EXPECT_TRUE(msupport::compare(e1, fl_14_2));

    //- Construct new engines via copy ctors and verify their initial states.
    //
    mse_f_ffv_rm    e2(e1);

    EXPECT_EQ(e1.rows(), 1);
    EXPECT_EQ(e1.columns(), 4);
    EXPECT_EQ(e2.row_capacity(), 1);
    EXPECT_EQ(e2.column_capacity(), 4);
    EXPECT_TRUE(msupport::compare(e2, e1));

    mse_f_ffv_rm    e3 = e1;

    EXPECT_EQ(e1.rows(), 1);
    EXPECT_EQ(e1.columns(), 4);
    EXPECT_EQ(e3.row_capacity(), 1);
    EXPECT_EQ(e3.column_capacity(), 4);
    EXPECT_TRUE(msupport::compare(e3, e1));

    mse_f_ffv_rm    e4{e1};

    EXPECT_EQ(e1.rows(), 1);
    EXPECT_EQ(e1.columns(), 4);
    EXPECT_EQ(e4.row_capacity(), 1);
    EXPECT_EQ(e4.column_capacity(), 4);
    EXPECT_TRUE(msupport::compare(e4, e1));
}


TEST(MSE_Matrix_2A, ListCtor)
{
    //- Construct new engines via list ctor and verify that their initial states.
    //
    mse_f_ffv_rm    e1 = LST_14_1;
    mse_f_ffv_rm    e2 LST_14_1 ;
    mse_f_ffv_rm    e3(LST_14_1);

    EXPECT_EQ(e1.rows(), 1);
    EXPECT_EQ(e1.columns(), 4);
    EXPECT_EQ(e1.row_capacity(), 1);
    EXPECT_EQ(e1.column_capacity(), 4);
    EXPECT_TRUE(msupport::compare(e1, LST_14_1));

    EXPECT_EQ(e2.rows(), 1);
    EXPECT_EQ(e2.columns(), 4);
    EXPECT_EQ(e2.row_capacity(), 1);
    EXPECT_EQ(e2.column_capacity(), 4);
    EXPECT_TRUE(msupport::compare(e2, e1));

    EXPECT_EQ(e3.rows(), 1);
    EXPECT_EQ(e3.columns(), 4);
    EXPECT_EQ(e3.row_capacity(), 1);
    EXPECT_EQ(e3.column_capacity(), 4);
    EXPECT_TRUE(msupport::compare(e3, e1));

    mse_f_ffv_rm    e4 = LST_4_1;
    mse_f_ffv_rm    e5 LST_4_1 ;
    mse_f_ffv_rm    e6(LST_4_1);

    EXPECT_EQ(e4.rows(), 1);
    EXPECT_EQ(e4.columns(), 4);
    EXPECT_EQ(e4.row_capacity(), 1);
    EXPECT_EQ(e4.column_capacity(), 4);
    EXPECT_TRUE(msupport::compare(e4, LST_14_1));

    EXPECT_EQ(e5.rows(), 1);
    EXPECT_EQ(e5.columns(), 4);
    EXPECT_EQ(e5.row_capacity(), 1);
    EXPECT_EQ(e5.column_capacity(), 4);
    EXPECT_TRUE(msupport::compare(e2, e1));

    EXPECT_EQ(e6.rows(), 1);
    EXPECT_EQ(e6.columns(), 4);
    EXPECT_EQ(e6.row_capacity(), 1);
    EXPECT_EQ(e6.column_capacity(), 4);
    EXPECT_TRUE(msupport::compare(e3, e1));
}


TEST(MSE_Matrix_2A, EngineCtor)
{
    //- Construct new engines via direct engine ctor and verify their initial states.
    //
    mse_i_ffv_cm    e1 = LST_4_2;
    mse_f_ffv_rm    e2(e1);
    mse_d_ffv_rm    e3(e2);
    mse_f_ffv_cm    e4 = LST_4_2;
    mse_f_ffv_rm    e5{e4};

    EXPECT_EQ(e1.rows(), 1);
    EXPECT_EQ(e1.columns(), 4);
    EXPECT_EQ(e1.row_capacity(), 1);
    EXPECT_EQ(e1.column_capacity(), 4);
    EXPECT_TRUE(msupport::compare(e1, il_14_2));

    EXPECT_EQ(e2.rows(), 1);
    EXPECT_EQ(e2.columns(), 4);
    EXPECT_EQ(e2.row_capacity(), 1);
    EXPECT_EQ(e2.column_capacity(), 4);
    EXPECT_TRUE(msupport::compare(e2, e1));

    EXPECT_EQ(e3.rows(), 1);
    EXPECT_EQ(e3.columns(), 4);
    EXPECT_EQ(e3.row_capacity(), 1);
    EXPECT_EQ(e3.column_capacity(), 4);
    EXPECT_TRUE(msupport::compare(e3, e1));

    EXPECT_EQ(e4.rows(), 1);
    EXPECT_EQ(e4.columns(), 4);
    EXPECT_EQ(e4.row_capacity(), 1);
    EXPECT_EQ(e4.column_capacity(), 4);
    EXPECT_TRUE(msupport::compare(e4, e1));

    EXPECT_EQ(e5.rows(), 1);
    EXPECT_EQ(e5.columns(), 4);
    EXPECT_EQ(e5.row_capacity(), 1);
    EXPECT_EQ(e5.column_capacity(), 4);
    EXPECT_TRUE(msupport::compare(e5, e1));
}


TEST(MSE_Matrix_2A, OtherCtor)
{
    //- Construct new engines via other ctors and verify their initial state.
    //
    std::array<float, 4>    o1 = LST_4_1;
    std::vector<float>      o2 = fl_4_2;
    std::deque<float>       o3 = fl_4_3;
    mdspan<float, 4>        o4(o1.data());

    mse_f_ffv_rm    e1(o1);
    mse_f_ffv_cm    e2(o2);
    mse_f_ffv_rm    e3(o3);
    mse_f_ffv_cm    e4(o4);

    EXPECT_TRUE(msupport::compare(e1, o1));
    EXPECT_TRUE(msupport::compare(e2, o2));
    EXPECT_TRUE(msupport::compare(e3, o3));
    EXPECT_TRUE(msupport::compare(e4, o4));

    EXPECT_TRUE(msupport::compare(e1, fl_4_1));
    EXPECT_TRUE(msupport::compare(e2, fl_4_2));
    EXPECT_TRUE(msupport::compare(e3, fl_4_3));
    EXPECT_TRUE(msupport::compare(e4, fl_4_1));
}


TEST(MSE_Matrix_2A, MoveAssign)
{
    //- Default construct and verify initial state.
    //
    mse_f_ffv_rm    e1;

    EXPECT_EQ(e1.rows(), 1);
    EXPECT_EQ(e1.columns(), 4);
    EXPECT_EQ(e1.row_capacity(), 1);
    EXPECT_EQ(e1.column_capacity(), 4);
    EXPECT_TRUE(msupport::compare(e1, il_14_0));

    //- List construct and verify initial state.
    //
    mse_f_ffv_rm    e2(il_4_1);

    EXPECT_EQ(e2.rows(), 1);
    EXPECT_EQ(e2.columns(), 4);
    EXPECT_EQ(e2.row_capacity(), 1);
    EXPECT_EQ(e2.column_capacity(), 4);
    EXPECT_TRUE(msupport::compare(e2, fl_14_1));

    //- Move-assign and verify.
    //
    e1 = std::move(e2);
    EXPECT_TRUE(msupport::compare(e1, fl_14_1));
}


TEST(MSE_Matrix_2A, CopyAssign)
{
    //- Default construct and verify initial state.
    //
    mse_f_ffv_rm    e1;

    EXPECT_EQ(e1.rows(), 1);
    EXPECT_EQ(e1.columns(), 4);
    EXPECT_EQ(e1.row_capacity(), 1);
    EXPECT_EQ(e1.column_capacity(), 4);
    EXPECT_TRUE(msupport::compare(e1, il_14_0));

    //- List construct and verify initial state.
    //
    mse_f_ffv_rm    e2(il_14_1);

    EXPECT_EQ(e2.rows(), 1);
    EXPECT_EQ(e2.columns(), 4);
    EXPECT_EQ(e2.row_capacity(), 1);
    EXPECT_EQ(e2.column_capacity(), 4);
    EXPECT_TRUE(msupport::compare(e2, fl_14_1));

    //- Copy-assign and verify.
    //
    e1 = e2;
    EXPECT_TRUE(msupport::compare(e1, fl_14_1));
}


TEST(MSE_Matrix_2A, EngineAssign)
{
    //- Default construct and verify initial state.
    //
    mse_f_ffv_rm    e1, e2;

    EXPECT_EQ(e1.rows(), 1);
    EXPECT_EQ(e1.columns(), 4);
    EXPECT_EQ(e1.row_capacity(), 1);
    EXPECT_EQ(e1.column_capacity(), 4);
    EXPECT_TRUE(msupport::compare(e1, LST_14_0));

    EXPECT_EQ(e2.rows(), 1);
    EXPECT_EQ(e2.columns(), 4);
    EXPECT_EQ(e2.row_capacity(), 1);
    EXPECT_EQ(e2.column_capacity(), 4);
    EXPECT_TRUE(msupport::compare(e2, fl_14_0));

    //- List construct and verify initial state.
    //
    mse_i_ffv_cm    e3 LST_14_1 ;

    EXPECT_EQ(e3.rows(), 1);
    EXPECT_EQ(e3.columns(), 4);
    EXPECT_EQ(e3.row_capacity(), 1);
    EXPECT_EQ(e3.column_capacity(), 4);
    EXPECT_TRUE(msupport::compare(e3, il_14_1));

    mse_i_ffv_cm    e4 LST_4_2 ;

    EXPECT_EQ(e4.rows(), 1);
    EXPECT_EQ(e4.columns(), 4);
    EXPECT_EQ(e4.row_capacity(), 1);
    EXPECT_EQ(e4.column_capacity(), 4);
    EXPECT_TRUE(msupport::compare(e4, il_14_2));

    EXPECT_FALSE(msupport::compare(e3, e4));

    //- Assign and verify.
    //
    e1 = e3;
    EXPECT_TRUE(msupport::compare(e1, fl_14_1));
    EXPECT_TRUE(msupport::compare(e1, e3));

    e2 = e4;
    EXPECT_TRUE(msupport::compare(e2, fl_14_2));
    EXPECT_TRUE(msupport::compare(e2, e4));
}


TEST(MSE_Matrix_2A, ListAssign)
{
    //- Default construct and verify initial state.
    //
    mse_f_ffv_rm     e1;
    mse_f_ffv_cm     e2{il_4_1};

    EXPECT_EQ(e1.rows(), 1);
    EXPECT_EQ(e1.columns(), 4);
    EXPECT_EQ(e1.row_capacity(), 1);
    EXPECT_EQ(e1.column_capacity(), 4);
    EXPECT_TRUE(msupport::compare(e1, LST_14_0));

    EXPECT_EQ(e2.rows(), 1);
    EXPECT_EQ(e2.columns(), 4);
    EXPECT_EQ(e2.row_capacity(), 1);
    EXPECT_EQ(e2.column_capacity(), 4);
    EXPECT_TRUE(msupport::compare(e2, LST_14_1));

    //- Assign and verify.
    //
    e1 = LST_14_1;
    EXPECT_TRUE(msupport::compare(e1, LST_14_1));
    EXPECT_TRUE(msupport::compare(e1, il_14_1));
    EXPECT_TRUE(msupport::compare(e1, fl_14_1));
    EXPECT_TRUE(msupport::compare(e1, e2));

    e1 = fl_14_0;
    EXPECT_TRUE(msupport::compare(e1, LST_14_0));
    EXPECT_TRUE(msupport::compare(e1, il_14_0));
    EXPECT_TRUE(msupport::compare(e1, fl_14_0));
    EXPECT_TRUE(msupport::compare(e1, mse_f_ffv_rm()));

    e1 = LST_4_2;
    EXPECT_TRUE(msupport::compare(e1, LST_14_2));
    EXPECT_TRUE(msupport::compare(e1, il_14_2));
    EXPECT_TRUE(msupport::compare(e1, fl_14_2));
    EXPECT_FALSE(msupport::compare(e1, e2));

    e1 = fl_4_0;
    EXPECT_TRUE(msupport::compare(e1, LST_14_0));
    EXPECT_TRUE(msupport::compare(e1, il_14_0));
    EXPECT_TRUE(msupport::compare(e1, fl_14_0));
    EXPECT_TRUE(msupport::compare(e1, mse_f_ffv_rm()));
}


TEST(MSE_Matrix_2A, OtherAssign)
{
    //- Default construct and verify initial state.
    //
    mse_f_ffv_rm     e1;
    mse_f_ffv_cm     e2(il_4_1);

    EXPECT_EQ(e1.rows(), 1);
    EXPECT_EQ(e1.columns(), 4);
    EXPECT_EQ(e1.row_capacity(), 1);
    EXPECT_EQ(e1.column_capacity(), 4);
    EXPECT_TRUE(msupport::compare(e1, LST_14_0));

    EXPECT_EQ(e2.rows(), 1);
    EXPECT_EQ(e2.columns(), 4);
    EXPECT_EQ(e2.row_capacity(), 1);
    EXPECT_EQ(e2.column_capacity(), 4);
    EXPECT_TRUE(msupport::compare(e2, LST_14_1));

    //- Assign and verify.
    //
    std::array<float, 4>    o1 = LST_4_1;
    std::vector<float>      o2 = fl_4_2;
    std::deque<float>       o3 = fl_4_3;
    mdspan<float, 4>        o4(o1.data());

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


TEST(MSE_Matrix_2A, Swap)
{
    //- Default construct and verify initial state.
    //
    mse_f_ffv_rm    e1;

    EXPECT_EQ(e1.rows(), 1);
    EXPECT_EQ(e1.columns(), 4);
    EXPECT_TRUE(msupport::compare(e1, fl_14_0));

    //- List construct and verify initial state.
    //
    mse_f_ffv_rm    e2(fl_4_2);

    EXPECT_EQ(e2.rows(), 1);
    EXPECT_EQ(e2.columns(), 4);
    EXPECT_TRUE(msupport::compare(e2, fl_14_2));

    //- Swap contents and verify.
    //
    e1.swap(e2);
    EXPECT_TRUE(msupport::compare(e1, fl_14_2));
    EXPECT_TRUE(msupport::compare(e2, fl_14_0));

    e2.swap(e1);
    EXPECT_TRUE(msupport::compare(e1, il_14_0));
    EXPECT_TRUE(msupport::compare(e2, il_14_2));
}


TEST(MSE_Matrix_2A, Span)
{
    mse_f_ffv_rm        e1(il_14_2), e2, e3{e1};
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

    EXPECT_EQ(csp1(0, 0), 11.0f);
    EXPECT_EQ(csp1(0, 1), 12.0f);
    EXPECT_EQ(csp1(0, 2), 13.0f);
    EXPECT_EQ(csp1(0, 3), 14.0f);

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
    sp1(0, 1) = 222.0f;
    sp1(0, 2) = 333.0f;
    sp1(0, 3) = 444.0f;
    EXPECT_EQ(e1(0), 111.0f);
    EXPECT_EQ(e1(1), 222.0f);
    EXPECT_EQ(e1(2), 333.0f);
    EXPECT_EQ(e1(3), 444.0f);
}
