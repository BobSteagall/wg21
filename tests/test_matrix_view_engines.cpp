#include "test_common.hpp"

using mat_engine_34  = fs_matrix_engine<float, 3, 4>;
using r_tr_engine_34 = matrix_transpose_engine<mat_engine_34, readable_matrix_engine_tag>;
using w_tr_engine_34 = matrix_transpose_engine<mat_engine_34, writable_matrix_engine_tag>;

//==================================================================================================
//  Unit tests for dynamically-resizeable vector engine.
//==================================================================================================
//
TEST(MatrixTransposeEngine, DefaultCtor)
{
    r_tr_engine_34  rt1;
    w_tr_engine_34  wt1;

    EXPECT_FALSE(rt1.is_valid());
    EXPECT_FALSE(wt1.is_valid());
}

TEST(MatrixTransposeEngine, DirectCtor)
{
    mat_engine_34   e1 = fl_34_1;
    r_tr_engine_34  rt1(e1);
    w_tr_engine_34  wt1(e1);

    //- Verify size and elements of the owning engine.
    //
    EXPECT_EQ(e1.size(), st_34);
    EXPECT_TRUE(m_cmp_eq(e1, fl_34_1));

    //- Verify the validity, size, capacity, and contents of the readable view.
    //
    EXPECT_TRUE(rt1.is_valid());
    EXPECT_EQ(e1.rows(), rt1.columns());
    EXPECT_EQ(e1.columns(), rt1.rows());
    EXPECT_EQ(rt1.size(), st_43);
    EXPECT_EQ(rt1.size(), rt1.capacity());
    EXPECT_TRUE(m_cmp_eq(rt1, fl_34_1_t));

    //- Verify the validity, size, capacity, and contents of the writable view.
    //
    EXPECT_TRUE(wt1.is_valid());
    EXPECT_EQ(e1.rows(), wt1.columns());
    EXPECT_EQ(e1.columns(), wt1.rows());
    EXPECT_EQ(wt1.size(), st_43);
    EXPECT_EQ(wt1.size(), wt1.capacity());
    EXPECT_TRUE(m_cmp_eq(wt1, fl_34_1_t));

    //- Verify the contents of the span produced by the owning engine.
    //
    EXPECT_TRUE(m_cmp_eq(e1, e1.span()));

    //- Verify the contents of the spans produced by the views.
    //
    EXPECT_TRUE(m_cmp_eq(rt1, rt1.span()));
    EXPECT_TRUE(m_cmp_eq(wt1, wt1.span()));

    //- Verify element update with writable view.
    //
    wt1 = fl_34_2_t;
    EXPECT_TRUE(m_cmp_eq(wt1, fl_34_2_t));
    EXPECT_TRUE(m_cmp_eq(e1, fl_34_2));
}
