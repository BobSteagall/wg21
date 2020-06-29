#include "test_common.hpp"

using mat_engine_34  = fs_matrix_engine<float, 3, 4>;
using mat_engine_43  = fs_matrix_engine<float, 4, 3>;

using r_cnj_engine_34 = matrix_hermitian_engine<mat_engine_34>;

using r_sub_engine_34 = matrix_subset_engine<mat_engine_34, readable_matrix_engine_tag>;
using w_sub_engine_34 = matrix_subset_engine<mat_engine_34, writable_matrix_engine_tag>;

using r_tr_engine_34     = matrix_transpose_engine<mat_engine_34>;
using r_neg_tr_engine_34 = matrix_negation_engine<r_tr_engine_34>;

using r_neg_engine_34    = matrix_negation_engine<mat_engine_34>;
using r_tr_neg_engine_34 = matrix_transpose_engine<r_neg_engine_34>;

//==================================================================================================
//  Unit tests for matrix negation engine.
//==================================================================================================
//
TEST(MatrixNegationEngine, DefaultCtor)
{
    r_neg_engine_34 rt1;

    EXPECT_FALSE(rt1.is_valid());
}

TEST(MatrixEngine, DirectCtor)
{
    mat_engine_34   e1 = fl_34_1;
    r_neg_engine_34 rt1(e1);

    //- Verify size and elements of the owning engine.
    //
    EXPECT_EQ(e1.size(), st_34);
    EXPECT_TRUE(m_cmp_eq(e1, fl_34_1));

    //- Verify the validity, size, capacity, and contents of the readable view.
    //
    EXPECT_TRUE(rt1.is_valid());
    EXPECT_EQ(e1.rows(), rt1.rows());
    EXPECT_EQ(e1.columns(), rt1.columns());
    EXPECT_EQ(rt1.size(), st_34);
    EXPECT_EQ(rt1.size(), rt1.capacity());

    EXPECT_EQ(rt1(0,0), -11.0f);

    EXPECT_TRUE(m_cmp_eq(rt1, fl_34_1_n));

    //- Verify the contents of the span produced by the view.
    //
    auto    rt1s = rt1.span();
    EXPECT_TRUE(m_cmp_eq(rt1, rt1s));
}

//==================================================================================================
//  Unit tests for matrix transpose engine.
//==================================================================================================
//
TEST(MatrixTransposeEngine, DefaultCtor)
{
    r_tr_engine_34  rt1;

    EXPECT_FALSE(rt1.is_valid());
}

TEST(MatrixTransposeEngine, DirectCtor)
{
    mat_engine_34   e1 = fl_34_1;
    r_tr_engine_34  rt1(e1);

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

    //- Verify the contents of the span produced by the owning engine.
    //
    EXPECT_TRUE(m_cmp_eq(e1, e1.span()));

    //- Verify the contents of the spans produced by the views.
    //
    EXPECT_TRUE(m_cmp_eq(rt1, rt1.span()));
}

TEST(MatrixTransposeEngine, AssignAndSwap)
{
    mat_engine_34   e1 = fl_34_1;
    r_tr_engine_34  rt1(e1);

    mat_engine_34   e2 = fl_34_2;
    r_tr_engine_34  rt2(e2);

    //- Verify sizes and contents.
    //
    EXPECT_EQ(e1.size(), st_34);
    EXPECT_TRUE(m_cmp_eq(e1, fl_34_1));
    EXPECT_TRUE(m_cmp_eq(rt1, fl_34_1_t));

    EXPECT_EQ(e2.size(), st_34);
    EXPECT_TRUE(m_cmp_eq(e2, fl_34_2));
    EXPECT_TRUE(m_cmp_eq(rt2, fl_34_2_t));

    //- Swap and then verify that the views point to the correct contents.
    //
    rt1.swap(rt2);
    EXPECT_TRUE(m_cmp_eq(rt1, fl_34_2_t));
    EXPECT_TRUE(m_cmp_eq(rt2, fl_34_1_t));
}

