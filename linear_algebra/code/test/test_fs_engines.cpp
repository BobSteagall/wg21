#include "test_common.hpp"

using dsve_4 = fs_vector_engine<double, 4>;
using fsve_4 = fs_vector_engine<float, 4>;
using isve_4 = fs_vector_engine<int, 4>;

TEST(FsVectorEngine, DefaultCtor)
{
    fs_vector_engine<float, 4>      e1;

    EXPECT_EQ(e1.elements(), 4);
    EXPECT_EQ(e1.capacity(), 4);

    EXPECT_EQ(e1(0), 0.0f);
    EXPECT_EQ(e1(1), 0.0f);
    EXPECT_EQ(e1(2), 0.0f);
    EXPECT_EQ(e1(3), 0.0f);
}

TEST(FsVectorEngine, CmpEq)
{
    std::initializer_list<float>    l1 = {0, 0, 0, 0};
    std::initializer_list<float>    l2 = {13, 17, 19, 23};
    fs_vector_engine<float, 4>      e1, e2, e3;

    EXPECT_EQ(e1.elements(), 4);
    EXPECT_EQ(e1.capacity(), 4);

    EXPECT_EQ(e1(0), 0.0f);
    EXPECT_EQ(e1(1), 0.0f);
    EXPECT_EQ(e1(2), 0.0f);
    EXPECT_EQ(e1(3), 0.0f);

    EXPECT_TRUE(v_cmp_eq(e1, e1));
    EXPECT_TRUE(v_cmp_eq(e1, e2));
    EXPECT_TRUE(v_cmp_eq(e1, e3));

    EXPECT_TRUE(v_cmp_eq(e1, l1));
    EXPECT_TRUE(v_cmp_eq(e1, {0, 0, 0, 0}));
    EXPECT_FALSE(v_cmp_eq(e1, l2));
    EXPECT_FALSE(v_cmp_eq(e1, {13, 17, 19, 23}));

    e2(0) = 13.0f;
    e2(1) = 17.0f;
    e2(2) = 19.0f;
    e2(3) = 23.0f;
    EXPECT_EQ(e2(0), 13.0f);
    EXPECT_EQ(e2(1), 17.0f);
    EXPECT_EQ(e2(2), 19.0f);
    EXPECT_EQ(e2(3), 23.0f);
    EXPECT_FALSE(v_cmp_eq(e1, e2));

    e3(0) = 13.0f;
    e3(1) = 17.0f;
    e3(2) = 19.0f;
    e3(3) = 23.0f;
    EXPECT_EQ(e3(0), 13.0f);
    EXPECT_EQ(e3(1), 17.0f);
    EXPECT_EQ(e3(2), 19.0f);
    EXPECT_EQ(e3(3), 23.0f);
    EXPECT_FALSE(v_cmp_eq(e1, e3));

    EXPECT_TRUE(v_cmp_eq(e2, e3));
    EXPECT_TRUE(v_cmp_eq(e2, l2));
    EXPECT_TRUE(v_cmp_eq(e2, {13, 17, 19, 23}));
    EXPECT_TRUE(v_cmp_eq(e3, l2));
    EXPECT_TRUE(v_cmp_eq(e3, {13, 17, 19, 23}));

    EXPECT_FALSE(v_cmp_eq(e3, {13, 17, 19}));
    EXPECT_FALSE(v_cmp_eq(e3, {13, 17, 19, 23, 29}));

    EXPECT_FALSE(v_cmp_eq(e3, {11, 17, 19, 23}));
    EXPECT_FALSE(v_cmp_eq(e3, fs_vector_engine<float, 4>({11, 17, 19, 23})));

    EXPECT_FALSE(v_cmp_eq(e3, {13, 11, 19, 23}));
    EXPECT_FALSE(v_cmp_eq(e3, fs_vector_engine<float, 4>({13, 11, 19, 23})));

    EXPECT_FALSE(v_cmp_eq(e3, {13, 17, 11, 23}));
    EXPECT_FALSE(v_cmp_eq(e3, fs_vector_engine<float, 4>({13, 17, 11, 23})));

    EXPECT_FALSE(v_cmp_eq(e3, {13, 17, 19, 11}));
    EXPECT_FALSE(v_cmp_eq(e3, fs_vector_engine<float, 4>({13, 17, 19, 11})));
}

TEST(FsVectorEngine, MoveCtor)
{
    fs_vector_engine<float, 4>      e1;

    EXPECT_EQ(e1.elements(), 4);
    EXPECT_EQ(e1.capacity(), 4);

    EXPECT_EQ(e1(0), 0.0f);
    EXPECT_EQ(e1(1), 0.0f);
    EXPECT_EQ(e1(2), 0.0f);
    EXPECT_EQ(e1(3), 0.0f);

    e1(0) = 13.0f;
    e1(1) = 17.0f;
    e1(2) = 19.0f;
    e1(3) = 23.0f;

    EXPECT_EQ(e1(0), 13.0f);
    EXPECT_EQ(e1(1), 17.0f);
    EXPECT_EQ(e1(2), 19.0f);
    EXPECT_EQ(e1(3), 23.0f);

    fs_vector_engine<float, 4>      e2(std::move(e1));

    EXPECT_EQ(e2(0), 13.0f);
    EXPECT_EQ(e2(1), 17.0f);
    EXPECT_EQ(e2(2), 19.0f);
    EXPECT_EQ(e2(3), 23.0f);
}

TEST(FsVectorEngine, CopyCtor)
{
    fs_vector_engine<float, 4>      e1;

    EXPECT_EQ(e1.elements(), 4);
    EXPECT_EQ(e1.capacity(), 4);

    EXPECT_EQ(e1(0), 0.0f);
    EXPECT_EQ(e1(1), 0.0f);
    EXPECT_EQ(e1(2), 0.0f);
    EXPECT_EQ(e1(3), 0.0f);

    e1(0) = 13.0f;
    e1(1) = 17.0f;
    e1(2) = 19.0f;
    e1(3) = 23.0f;

    EXPECT_EQ(e1(0), 13.0f);
    EXPECT_EQ(e1(1), 17.0f);
    EXPECT_EQ(e1(2), 19.0f);
    EXPECT_EQ(e1(3), 23.0f);

    fs_vector_engine<float, 4>      e2(e1);

    EXPECT_EQ(e2(0), 13.0f);
    EXPECT_EQ(e2(1), 17.0f);
    EXPECT_EQ(e2(2), 19.0f);
    EXPECT_EQ(e2(3), 23.0f);
}

TEST(FsVectorEngine, ListCtor)
{
    fs_vector_engine<float, 4>      e1 = {13, 17, 19, 23};
    fs_vector_engine<float, 4>      e2{13, 17, 19, 23};
    fs_vector_engine<float, 4>      e3({13, 17, 19, 23});

    EXPECT_EQ(e1.elements(), 4);
    EXPECT_EQ(e1.capacity(), 4);

    EXPECT_EQ(e1(0), 13.0f);
    EXPECT_EQ(e1(1), 17.0f);
    EXPECT_EQ(e1(2), 19.0f);
    EXPECT_EQ(e1(3), 23.0f);

    EXPECT_EQ(e2.elements(), 4);
    EXPECT_EQ(e2.capacity(), 4);

    EXPECT_EQ(e2(0), 13.0f);
    EXPECT_EQ(e2(1), 17.0f);
    EXPECT_EQ(e2(2), 19.0f);
    EXPECT_EQ(e2(3), 23.0f);

    EXPECT_EQ(e3.elements(), 4);
    EXPECT_EQ(e3.capacity(), 4);

    EXPECT_EQ(e3(0), 13.0f);
    EXPECT_EQ(e3(1), 17.0f);
    EXPECT_EQ(e3(2), 19.0f);
    EXPECT_EQ(e3(3), 23.0f);
}

TEST(FsVectorEngine, EngineCtor)
{
    fs_vector_engine<int, 4>        e1{13, 17, 19, 23};
    fs_vector_engine<float, 4>      e2(e1);
    fs_vector_engine<double, 4>     e3(e2);
    dr_vector_engine<double>        e4{13, 17, 19, 23};
    fs_vector_engine<float, 4>      e5{e4};

    EXPECT_EQ(e1.elements(), 4);
    EXPECT_EQ(e1.capacity(), 4);

    EXPECT_EQ(e1(0), 13);
    EXPECT_EQ(e1(1), 17);
    EXPECT_EQ(e1(2), 19);
    EXPECT_EQ(e1(3), 23);

    EXPECT_EQ(e2.elements(), 4);
    EXPECT_EQ(e2.capacity(), 4);

    EXPECT_EQ(e2(0), 13.0f);
    EXPECT_EQ(e2(1), 17.0f);
    EXPECT_EQ(e2(2), 19.0f);
    EXPECT_EQ(e2(3), 23.0f);

    EXPECT_EQ(e3.elements(), 4);
    EXPECT_EQ(e3.capacity(), 4);

    EXPECT_EQ(e3(0), 13.0);
    EXPECT_EQ(e3(1), 17.0);
    EXPECT_EQ(e3(2), 19.0);
    EXPECT_EQ(e3(3), 23.0);

    EXPECT_EQ(e4.elements(), 4);
    EXPECT_GE(e4.capacity(), 4);

    EXPECT_EQ(e4(0), 13.0);
    EXPECT_EQ(e4(1), 17.0);
    EXPECT_EQ(e4(2), 19.0);
    EXPECT_EQ(e4(3), 23.0);

    EXPECT_EQ(e5.elements(), 4);
    EXPECT_EQ(e5.capacity(), 4);

    EXPECT_EQ(e5(0), 13.0f);
    EXPECT_EQ(e5(1), 17.0f);
    EXPECT_EQ(e5(2), 19.0f);
    EXPECT_EQ(e5(3), 23.0f);
}
