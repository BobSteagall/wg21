#include "test_common.hpp"

using namespace STD_LA;
using namespace STD_LA::detail;
using namespace MDSPAN_NS;

//==================================================================================================
//  Unit tests for fixed-size and resizable vector engines.
//==================================================================================================
//
using fve_type_v = matrix_storage_engine<float, extents<4>, void, void>;
using fve_type_a = matrix_storage_engine<float, extents<4>, std::allocator<float>, void>;
using dve_type   = matrix_storage_engine<float, extents<std::dynamic_extent>, std::allocator<float>, void>;

using vsupport = vector_engine_support;

TEST(MSE_Vector, DefaultCtor)
{
    //- Verify size, capacity, initial values of default-constructed vector engines.
    //
    fve_type_v  e0;

    EXPECT_EQ(e0.size(), 4);
    EXPECT_GE(e0.capacity(), 4);
    EXPECT_EQ(e0(0), 0.0f);
    EXPECT_EQ(e0(1), 0.0f);
    EXPECT_EQ(e0(2), 0.0f);
    EXPECT_EQ(e0(3), 0.0f);

    fve_type_a  e1;

    EXPECT_EQ(e1.size(), 4);
    EXPECT_GE(e1.capacity(), 4);
    EXPECT_EQ(e1(0), 0.0f);
    EXPECT_EQ(e1(1), 0.0f);
    EXPECT_EQ(e1(2), 0.0f);
    EXPECT_EQ(e1(3), 0.0f);

    dve_type    e2;

    EXPECT_EQ(e2.size(), 0);
    EXPECT_GE(e2.capacity(), 0);
    e2.reshape(4, 0);
    EXPECT_EQ(e2.size(), 4);
    EXPECT_GE(e2.capacity(), 4);
    EXPECT_EQ(e2(0), 0.0f);
    EXPECT_EQ(e2(1), 0.0f);
    EXPECT_EQ(e2(2), 0.0f);
    EXPECT_EQ(e2(3), 0.0f);
}


TEST(MSE_Vector, ListCtor)
{
    //- Verify size, capacity, initial values of list-constructed vector engines.
    //
    fve_type_v  e0 = il_4_1;

    EXPECT_EQ(e0.size(), 4);
    EXPECT_GE(e0.capacity(), 4);
    EXPECT_EQ(e0(0), 1.0f);
    EXPECT_EQ(e0(1), 2.0f);
    EXPECT_EQ(e0(2), 3.0f);
    EXPECT_EQ(e0(3), 4.0f);

    fve_type_a  e1 = fl_4_2;

    EXPECT_EQ(e1.size(), 4);
    EXPECT_GE(e1.capacity(), 4);
    EXPECT_EQ(e1(0), 11.0f);
    EXPECT_EQ(e1(1), 12.0f);
    EXPECT_EQ(e1(2), 13.0f);
    EXPECT_EQ(e1(3), 14.0f);

    dve_type    e2 = il_4_3;

    EXPECT_EQ(e2.size(), 4);
    EXPECT_GE(e2.capacity(), 4);
    EXPECT_EQ(e2(0), 101.0f);
    EXPECT_EQ(e2(1), 202.0f);
    EXPECT_EQ(e2(2), 303.0f);
    EXPECT_EQ(e2(3), 404.0f);
}


TEST(MSE_Vector, CmpEq)
{
    std::initializer_list<float>    l1 = {0, 0, 0, 0};
    std::initializer_list<float>    l2 = {13, 17, 19, 23};
    std::array<float, 4>            a1 = {0, 0, 0, 0};
    std::array<float, 4>            a2 = {13, 17, 19, 23};
    mdspan<float, extents<4>>       s1(a1.data());
    mdspan<float, extents<4>>       s2(a2.data());

    fve_type_v  e1;
    fve_type_a  e2;
    dve_type    e3(e1.size());

    //- Verify size, capacity, initial values.
    //
    EXPECT_EQ(e1.size(), 4);
    EXPECT_EQ(e1.capacity(), 4);
    EXPECT_EQ(e1(0), 0.0f);
    EXPECT_EQ(e1(1), 0.0f);
    EXPECT_EQ(e1(2), 0.0f);
    EXPECT_EQ(e1(3), 0.0f);

    //- Verify expected equality.
    //
    EXPECT_TRUE(vsupport::compare(e1, e1));
    EXPECT_TRUE(vsupport::compare(e1, e2));
    EXPECT_TRUE(vsupport::compare(e1, e3));
    EXPECT_TRUE(vsupport::compare(e2, e2));
    EXPECT_TRUE(vsupport::compare(e2, e3));
    EXPECT_TRUE(vsupport::compare(e3, e3));

    //- Verify equality against an init-list of equivalent values.
    //
    EXPECT_TRUE(vsupport::compare(e1, {0, 0, 0, 0}));
    EXPECT_TRUE(vsupport::compare(e1, l1));
    EXPECT_TRUE(vsupport::compare(e1, a1));
    EXPECT_TRUE(vsupport::compare(e1, s1));

    EXPECT_TRUE(vsupport::compare(e2, {0, 0, 0, 0}));
    EXPECT_TRUE(vsupport::compare(e2, l1));
    EXPECT_TRUE(vsupport::compare(e2, a1));
    EXPECT_TRUE(vsupport::compare(e2, s1));

    EXPECT_TRUE(vsupport::compare(e3, {0, 0, 0, 0}));
    EXPECT_TRUE(vsupport::compare(e3, l1));
    EXPECT_TRUE(vsupport::compare(e3, a1));
    EXPECT_TRUE(vsupport::compare(e3, s1));

    //- Verify inequality against an init-list of different values.
    //
    EXPECT_FALSE(vsupport::compare(e1, l2));
    EXPECT_FALSE(vsupport::compare(e1, {13, 17, 19, 23}));
    EXPECT_FALSE(vsupport::compare(e2, l2));
    EXPECT_FALSE(vsupport::compare(e2, {13, 17, 19, 23}));
    EXPECT_FALSE(vsupport::compare(e3, l2));
    EXPECT_FALSE(vsupport::compare(e3, {13, 17, 19, 23}));

    //- Assign element values via mutable indexing and verify them.
    //
    e2(0) = 13.0f;
    e2(1) = 17.0f;
    e2(2) = 19.0f;
    e2(3) = 23.0f;
    EXPECT_EQ(e2(0), 13.0f);
    EXPECT_EQ(e2(1), 17.0f);
    EXPECT_EQ(e2(2), 19.0f);
    EXPECT_EQ(e2(3), 23.0f);
    EXPECT_FALSE(vsupport::compare(e1, e2));

    e3(0) = 13.0f;
    e3(1) = 17.0f;
    e3(2) = 19.0f;
    e3(3) = 23.0f;
    EXPECT_EQ(e3(0), 13.0f);
    EXPECT_EQ(e3(1), 17.0f);
    EXPECT_EQ(e3(2), 19.0f);
    EXPECT_EQ(e3(3), 23.0f);
    EXPECT_FALSE(vsupport::compare(e1, e3));

    //- Verify expected equality of various argument combinations.
    //
    EXPECT_TRUE(vsupport::compare(e2, e3));

    EXPECT_TRUE(vsupport::compare(e2, {13, 17, 19, 23}));
    EXPECT_TRUE(vsupport::compare(e2, l2));
    EXPECT_TRUE(vsupport::compare(e2, a2));
    EXPECT_TRUE(vsupport::compare(e2, s2));

    EXPECT_TRUE(vsupport::compare(e3, {13, 17, 19, 23}));
    EXPECT_TRUE(vsupport::compare(e3, l2));
    EXPECT_TRUE(vsupport::compare(e3, a2));
    EXPECT_TRUE(vsupport::compare(e3, s2));

    //- Verify expected inequality when compared to init-lists of incorrect size.
    //
    EXPECT_FALSE(vsupport::compare(e3, {13, 17, 19}));
    EXPECT_FALSE(vsupport::compare(e3, {13, 17, 19, 23, 29}));

    //- Verify expected inequality against init-lists and engines having different contents.
    //
    EXPECT_FALSE(vsupport::compare(e3, {11, 17, 19, 23}));
    EXPECT_FALSE(vsupport::compare(e3, fve_type_v({11, 17, 19, 23})));

    EXPECT_FALSE(vsupport::compare(e3, {13, 11, 19, 23}));
    EXPECT_FALSE(vsupport::compare(e3, fve_type_a({13, 11, 19, 23})));

    EXPECT_FALSE(vsupport::compare(e3, {13, 17, 11, 23}));
    EXPECT_FALSE(vsupport::compare(e3, dve_type({13, 17, 11, 23})));

    EXPECT_FALSE(vsupport::compare(e3, {13, 17, 19, 11}));
    EXPECT_FALSE(vsupport::compare(e3, dve_type({13, 17, 19, 11})));
}


TEST(MSE_Vector, MoveCtor)
{
    std::initializer_list<float>    l1 = {13, 17, 19, 23};

    //- List construct and verify initial state.
    //
    fve_type_v      e1(l1);
    fve_type_a      e2(l1);
    dve_type        e3(l1);

    EXPECT_TRUE(vsupport::compare(e1, l1));
    EXPECT_TRUE(vsupport::compare(e2, l1));
    EXPECT_TRUE(vsupport::compare(e3, l1));

    //- Construct new engines via move ctor and verify their initial state.
    //
    fve_type_v      e1b(std::move(e1));
    fve_type_a      e2b(std::move(e2));
    dve_type        e3b(std::move(e3));

    EXPECT_TRUE(vsupport::compare(e1b, l1));
    EXPECT_TRUE(vsupport::compare(e2b, l1));
    EXPECT_TRUE(vsupport::compare(e3b, l1));
}


TEST(MSE_Vector, CopyCtor)
{
    std::initializer_list<float>    l1 = {13, 17, 19, 23};

    //- List construct and verify initial state.
    //
    fve_type_v      e1(l1);
    fve_type_a      e2(l1);
    dve_type        e3(l1);

    EXPECT_TRUE(vsupport::compare(e1, l1));
    EXPECT_TRUE(vsupport::compare(e2, l1));
    EXPECT_TRUE(vsupport::compare(e3, l1));

    //- Construct new engines via copy ctor and verify their initial state.
    //
    fve_type_v      e1b(e1);
    fve_type_a      e2b(e2);
    dve_type        e3b(e3);

    EXPECT_TRUE(vsupport::compare(e1b, l1));
    EXPECT_TRUE(vsupport::compare(e2b, l1));
    EXPECT_TRUE(vsupport::compare(e3b, l1));
}


TEST(MSE_Vector, EngineCtor)
{
    //- List construct and verify initial state.
    //
    fve_type_v      e1(il_4_1);
    fve_type_a      e2(il_4_2);
    dve_type        e3(il_4_3);

    EXPECT_TRUE(vsupport::compare(e1, il_4_1));
    EXPECT_TRUE(vsupport::compare(e2, il_4_2));
    EXPECT_TRUE(vsupport::compare(e3, il_4_3));

    //- Construct new engines via engine ctor and verify their initial state.
    //
    fve_type_v      e1b(e2);
    fve_type_a      e2b(e3);
    dve_type        e3b(e1);

    EXPECT_TRUE(vsupport::compare(e1b, il_4_2));
    EXPECT_TRUE(vsupport::compare(e1b, e2));
    EXPECT_FALSE(vsupport::compare(e1b, e1));

    EXPECT_TRUE(vsupport::compare(e2b, il_4_3));
    EXPECT_TRUE(vsupport::compare(e2b, e3));
    EXPECT_FALSE(vsupport::compare(e2b, e2));

    EXPECT_TRUE(vsupport::compare(e3b, il_4_1));
    EXPECT_TRUE(vsupport::compare(e3b, e1));
    EXPECT_FALSE(vsupport::compare(e3b, e3));
}


TEST(MSE_Vector, OtherCtor)
{
    //- List construct and verify initial state.
    //
    fve_type_v      e1(il_4_1);
    fve_type_a      e2(il_4_2);
    dve_type        e3(il_4_3);

    EXPECT_TRUE(vsupport::compare(e1, il_4_1));
    EXPECT_TRUE(vsupport::compare(e2, il_4_2));
    EXPECT_TRUE(vsupport::compare(e3, il_4_3));

    //- Construct new engines via other ctors and verify their initial state.
    //
    std::array<float, 4>        o1 = LST_4_1;
    std::vector<float>          o2 = fl_4_2;
    std::deque<float>           o3 = fl_4_3;
    mdspan<float, extents<4>>   o4(o1.data());

    fve_type_v      e1b(o2);
    fve_type_a      e2b(o3);
    dve_type        e3b(o1);
    dve_type        e4b(o4);

    EXPECT_TRUE(vsupport::compare(e1b, il_4_2));
    EXPECT_TRUE(vsupport::compare(e1b, e2));
    EXPECT_FALSE(vsupport::compare(e1b, e1));

    EXPECT_TRUE(vsupport::compare(e2b, il_4_3));
    EXPECT_TRUE(vsupport::compare(e2b, e3));
    EXPECT_FALSE(vsupport::compare(e2b, e2));

    EXPECT_TRUE(vsupport::compare(e3b, il_4_1));
    EXPECT_TRUE(vsupport::compare(e3b, e1));
    EXPECT_FALSE(vsupport::compare(e3b, e3));

    EXPECT_TRUE(vsupport::compare(e4b, il_4_1));
    EXPECT_TRUE(vsupport::compare(e4b, e1));
    EXPECT_FALSE(vsupport::compare(e4b, e3));
}


TEST(MSE_Vector, ListAssign)
{
    std::initializer_list<float>    l0 = {13, 17, 19, 23};

    fve_type_v      e1a, e1b(il_4_1);
    fve_type_a      e2a, e2b(il_4_2);
    dve_type        e3a, e3b(il_4_3);

    EXPECT_TRUE(vsupport::compare(e1a, fl_4_0));
    EXPECT_TRUE(vsupport::compare(e1b, fl_4_1));

    EXPECT_TRUE(vsupport::compare(e2a, fl_4_0));
    EXPECT_TRUE(vsupport::compare(e2b, fl_4_2));

    EXPECT_EQ(e3a.size(), 0);
    EXPECT_TRUE(vsupport::compare(e3b, fl_4_3));

    //- List assign and verify.
    //
    e1a = l0;
    e1b = l0;
    EXPECT_TRUE(vsupport::compare(e1a, l0));
    EXPECT_TRUE(vsupport::compare(e1b, l0));
    EXPECT_TRUE(vsupport::compare(e1a, e1b));

    e2a = l0;
    e2b = l0;
    EXPECT_TRUE(vsupport::compare(e2a, l0));
    EXPECT_TRUE(vsupport::compare(e2b, l0));
    EXPECT_TRUE(vsupport::compare(e2a, e2b));

    e3a = l0;
    e3b = l0;
    EXPECT_TRUE(vsupport::compare(e3a, l0));
    EXPECT_TRUE(vsupport::compare(e3b, l0));
    EXPECT_TRUE(vsupport::compare(e3a, e3b));
}


TEST(MSE_Vector, MoveAssign)
{
    fve_type_v      e1a, e1b(il_4_1), e1c(il_4_2), e1d(il_4_3);
    fve_type_a      e2a, e2b(il_4_1), e2c(il_4_2), e2d(il_4_3);
    dve_type        e3a, e3b(il_4_1), e3c(il_4_2), e3d(il_4_3);

    EXPECT_TRUE(vsupport::compare(e1a, fl_4_0));
    EXPECT_TRUE(vsupport::compare(e1b, fl_4_1));
    EXPECT_TRUE(vsupport::compare(e1c, fl_4_2));
    EXPECT_TRUE(vsupport::compare(e1d, fl_4_3));

    EXPECT_TRUE(vsupport::compare(e2a, fl_4_0));
    EXPECT_TRUE(vsupport::compare(e2b, fl_4_1));
    EXPECT_TRUE(vsupport::compare(e2c, fl_4_2));
    EXPECT_TRUE(vsupport::compare(e2d, fl_4_3));

    EXPECT_EQ(e3a.size(), 0);
    EXPECT_TRUE(vsupport::compare(e3b, fl_4_1));
    EXPECT_TRUE(vsupport::compare(e3c, fl_4_2));
    EXPECT_TRUE(vsupport::compare(e3d, fl_4_3));

    //- Move assign and verify.
    //
    e1a = std::move(e1c);
    e1b = std::move(e1d);
    EXPECT_TRUE(vsupport::compare(e1a, fl_4_2));
    EXPECT_TRUE(vsupport::compare(e1b, fl_4_3));

    e2a = std::move(e2c);
    e2b = std::move(e2d);
    EXPECT_TRUE(vsupport::compare(e2a, fl_4_2));
    EXPECT_TRUE(vsupport::compare(e2b, fl_4_3));

    e3a = std::move(e3c);
    e3b = std::move(e3d);
    EXPECT_TRUE(vsupport::compare(e3a, fl_4_2));
    EXPECT_TRUE(vsupport::compare(e3b, fl_4_3));
}


TEST(MSE_Vector, CopyAssign)
{
    fve_type_v      e1a, e1b(il_4_1), e1c(il_4_2), e1d(il_4_3);
    fve_type_a      e2a, e2b(il_4_1), e2c(il_4_2), e2d(il_4_3);
    dve_type        e3a, e3b(il_4_1), e3c(il_4_2), e3d(il_4_3);

    EXPECT_TRUE(vsupport::compare(e1a, fl_4_0));
    EXPECT_TRUE(vsupport::compare(e1b, fl_4_1));
    EXPECT_TRUE(vsupport::compare(e1c, fl_4_2));
    EXPECT_TRUE(vsupport::compare(e1d, fl_4_3));

    EXPECT_TRUE(vsupport::compare(e2a, fl_4_0));
    EXPECT_TRUE(vsupport::compare(e2b, fl_4_1));
    EXPECT_TRUE(vsupport::compare(e2c, fl_4_2));
    EXPECT_TRUE(vsupport::compare(e2d, fl_4_3));

    EXPECT_EQ(e3a.size(), 0);
    EXPECT_TRUE(vsupport::compare(e3b, fl_4_1));
    EXPECT_TRUE(vsupport::compare(e3c, fl_4_2));
    EXPECT_TRUE(vsupport::compare(e3d, fl_4_3));

    //- Copy assign and verify.
    //
    e1a = e1c;
    e1b = e1d;
    EXPECT_TRUE(vsupport::compare(e1a, fl_4_2));
    EXPECT_TRUE(vsupport::compare(e1b, fl_4_3));
    EXPECT_TRUE(vsupport::compare(e1a, e1c));
    EXPECT_TRUE(vsupport::compare(e1b, e1d));

    e2a = e2c;
    e2b = e2d;
    EXPECT_TRUE(vsupport::compare(e2a, fl_4_2));
    EXPECT_TRUE(vsupport::compare(e2b, fl_4_3));
    EXPECT_TRUE(vsupport::compare(e2a, e2c));
    EXPECT_TRUE(vsupport::compare(e2b, e2d));

    e3a = e3c;
    e3b = e3d;
    EXPECT_TRUE(vsupport::compare(e3a, fl_4_2));
    EXPECT_TRUE(vsupport::compare(e3b, fl_4_3));
    EXPECT_TRUE(vsupport::compare(e3a, e3c));
    EXPECT_TRUE(vsupport::compare(e3b, e3d));
}


TEST(MSE_Vector, EngineAssign)
{
    fve_type_v      e1a, e1b(il_4_1), e1c(il_4_2), e1d(il_4_3);
    fve_type_a      e2a, e2b(il_4_1), e2c(il_4_2), e2d(il_4_3);
    dve_type        e3a, e3b(il_4_1), e3c(il_4_2), e3d(il_4_3);

    EXPECT_TRUE(vsupport::compare(e1a, fl_4_0));
    EXPECT_TRUE(vsupport::compare(e1b, fl_4_1));
    EXPECT_TRUE(vsupport::compare(e1c, fl_4_2));
    EXPECT_TRUE(vsupport::compare(e1d, fl_4_3));

    EXPECT_TRUE(vsupport::compare(e2a, fl_4_0));
    EXPECT_TRUE(vsupport::compare(e2b, fl_4_1));
    EXPECT_TRUE(vsupport::compare(e2c, fl_4_2));
    EXPECT_TRUE(vsupport::compare(e2d, fl_4_3));

    EXPECT_EQ(e3a.size(), 0);
    EXPECT_TRUE(vsupport::compare(e3b, fl_4_1));
    EXPECT_TRUE(vsupport::compare(e3c, fl_4_2));
    EXPECT_TRUE(vsupport::compare(e3d, fl_4_3));

    //- Engine assign and verify.
    //
    e1a = e2c;
    e1b = e2d;
    EXPECT_TRUE(vsupport::compare(e1a, fl_4_2));
    EXPECT_TRUE(vsupport::compare(e1b, fl_4_3));
    EXPECT_TRUE(vsupport::compare(e1a, e2c));
    EXPECT_TRUE(vsupport::compare(e1b, e2d));

    e2a = e3c;
    e2b = e3d;
    EXPECT_TRUE(vsupport::compare(e2a, fl_4_2));
    EXPECT_TRUE(vsupport::compare(e2b, fl_4_3));
    EXPECT_TRUE(vsupport::compare(e2a, e3c));
    EXPECT_TRUE(vsupport::compare(e2b, e3d));

    e3a = e1c;
    e3b = e1d;
    EXPECT_TRUE(vsupport::compare(e3a, fl_4_2));
    EXPECT_TRUE(vsupport::compare(e3b, fl_4_3));
    EXPECT_TRUE(vsupport::compare(e3a, e1c));
    EXPECT_TRUE(vsupport::compare(e3b, e1d));
}


TEST(MSE_Vector, OtherAssign)
{
    fve_type_v      e1a, e1b(il_4_1), e1c(il_4_2), e1d(il_4_3);
    fve_type_a      e2a, e2b(il_4_1), e2c(il_4_2), e2d(il_4_3);
    dve_type        e3a, e3b(il_4_1), e3c(il_4_2), e3d(il_4_3);

    EXPECT_TRUE(vsupport::compare(e1a, fl_4_0));
    EXPECT_TRUE(vsupport::compare(e1b, fl_4_1));
    EXPECT_TRUE(vsupport::compare(e1c, fl_4_2));
    EXPECT_TRUE(vsupport::compare(e1d, fl_4_3));

    EXPECT_TRUE(vsupport::compare(e2a, fl_4_0));
    EXPECT_TRUE(vsupport::compare(e2b, fl_4_1));
    EXPECT_TRUE(vsupport::compare(e2c, fl_4_2));
    EXPECT_TRUE(vsupport::compare(e2d, fl_4_3));

    EXPECT_EQ(e3a.size(), 0);
    EXPECT_TRUE(vsupport::compare(e3b, fl_4_1));
    EXPECT_TRUE(vsupport::compare(e3c, fl_4_2));
    EXPECT_TRUE(vsupport::compare(e3d, fl_4_3));

    //- Assign to engines via other assignment operators and verify.
    //
    std::array<float, 4>        o1 = LST_4_1;
    std::vector<float>          o2 = fl_4_2;
    std::deque<float>           o3 = fl_4_3;
    mdspan<float, extents<4>>   o4(o1.data());

    e1a = o1;
    e2a = o1;
    e3a = o1;
    EXPECT_TRUE(vsupport::compare(e1a, o1));
    EXPECT_TRUE(vsupport::compare(e2a, o1));
    EXPECT_TRUE(vsupport::compare(e3a, o1));
    EXPECT_TRUE(vsupport::compare(e1a, e2a));
    EXPECT_TRUE(vsupport::compare(e1a, e3a));
    EXPECT_TRUE(vsupport::compare(e2a, e3a));

    e1b = o2;
    e2b = o2;
    e3b = o2;
    EXPECT_TRUE(vsupport::compare(e1b, o2));
    EXPECT_TRUE(vsupport::compare(e2b, o2));
    EXPECT_TRUE(vsupport::compare(e3b, o2));
    EXPECT_TRUE(vsupport::compare(e1b, e2b));
    EXPECT_TRUE(vsupport::compare(e1b, e3b));
    EXPECT_TRUE(vsupport::compare(e2b, e3b));

    e1c = o3;
    e2c = o3;
    e3c = o3;
    EXPECT_TRUE(vsupport::compare(e1c, o3));
    EXPECT_TRUE(vsupport::compare(e2c, o3));
    EXPECT_TRUE(vsupport::compare(e3c, o3));
    EXPECT_TRUE(vsupport::compare(e1c, e2c));
    EXPECT_TRUE(vsupport::compare(e1c, e3c));
    EXPECT_TRUE(vsupport::compare(e2c, e3c));

    e1d = o4;
    e2d = o4;
    e3d = o4;
    EXPECT_TRUE(vsupport::compare(e1d, o4));
    EXPECT_TRUE(vsupport::compare(e2d, o4));
    EXPECT_TRUE(vsupport::compare(e3d, o4));
    EXPECT_TRUE(vsupport::compare(e1d, e2d));
    EXPECT_TRUE(vsupport::compare(e1d, e3d));
    EXPECT_TRUE(vsupport::compare(e2d, e3d));
}


TEST(MSE_Vector, Swap)
{
    fve_type_v      e1a, e1b(il_4_1), e1c(il_4_2), e1d(il_4_3);
    fve_type_a      e2a, e2b(il_4_1), e2c(il_4_2), e2d(il_4_3);
    dve_type        e3a, e3b(il_4_1), e3c(il_4_2), e3d(il_4_3);

    EXPECT_TRUE(vsupport::compare(e1a, fl_4_0));
    EXPECT_TRUE(vsupport::compare(e1b, fl_4_1));
    EXPECT_TRUE(vsupport::compare(e1c, fl_4_2));
    EXPECT_TRUE(vsupport::compare(e1d, fl_4_3));

    EXPECT_TRUE(vsupport::compare(e2a, fl_4_0));
    EXPECT_TRUE(vsupport::compare(e2b, fl_4_1));
    EXPECT_TRUE(vsupport::compare(e2c, fl_4_2));
    EXPECT_TRUE(vsupport::compare(e2d, fl_4_3));

    EXPECT_EQ(e3a.size(), 0);
    EXPECT_TRUE(vsupport::compare(e3b, fl_4_1));
    EXPECT_TRUE(vsupport::compare(e3c, fl_4_2));
    EXPECT_TRUE(vsupport::compare(e3d, fl_4_3));

    //- Swap and verify.
    //
    e1a.swap(e1c);
    e1b.swap(e1d);
    EXPECT_TRUE(vsupport::compare(e1a, fl_4_2));
    EXPECT_TRUE(vsupport::compare(e1b, fl_4_3));
    EXPECT_TRUE(vsupport::compare(e1c, fl_4_0));
    EXPECT_TRUE(vsupport::compare(e1d, fl_4_1));

    e2a.swap(e2c);
    e2b.swap(e2d);
    EXPECT_TRUE(vsupport::compare(e2a, fl_4_2));
    EXPECT_TRUE(vsupport::compare(e2b, fl_4_3));
    EXPECT_TRUE(vsupport::compare(e2c, fl_4_0));
    EXPECT_TRUE(vsupport::compare(e2d, fl_4_1));

    e3a.swap(e3c);
    e3b.swap(e3d);
    EXPECT_TRUE(vsupport::compare(e3a, fl_4_2));
    EXPECT_TRUE(vsupport::compare(e3b, fl_4_3));
    EXPECT_EQ(e3c.size(), 0);
    EXPECT_TRUE(vsupport::compare(e3d, fl_4_1));
}


TEST(MSE_Vector, Span)
{
    fve_type_v          e1(il_4_1);
    fve_type_v const&   ce1 = e1;
    fve_type_a          e2(il_4_2);
    fve_type_a const&   ce2 = e2;
    dve_type            e3(il_4_3);
    dve_type const&     ce3 = e3;

    EXPECT_TRUE(vsupport::compare(e1, fl_4_1));
    EXPECT_TRUE(vsupport::compare(e2, fl_4_2));
    EXPECT_TRUE(vsupport::compare(e3, fl_4_3));

    auto    sp1  = e1.span();
    auto    csp1 = ce1.span();
    auto    sp2  = e2.span();
    auto    csp2 = ce2.span();
    auto    sp3  = e3.span();
    auto    csp3 = ce3.span();

    //- Const and mutable spans should have different types.
    //
    EXPECT_FALSE((std::is_same_v<decltype(sp1), decltype(csp1)>));
    EXPECT_FALSE((std::is_same_v<decltype(sp2), decltype(csp2)>));
    EXPECT_FALSE((std::is_same_v<decltype(sp3), decltype(csp3)>));

    //- The elements of the const and mutable spans should have the expected values.
    //
    EXPECT_EQ(sp1(0), e1(0));
    EXPECT_EQ(sp1(1), e1(1));
    EXPECT_EQ(sp1(2), e1(2));
    EXPECT_EQ(sp1(3), e1(3));
    EXPECT_EQ(csp1(0), e1(0));
    EXPECT_EQ(csp1(1), e1(1));
    EXPECT_EQ(csp1(2), e1(2));
    EXPECT_EQ(csp1(3), e1(3));

    //- Whole-object comparison between engines and spans should work as expected.
    //
    EXPECT_TRUE(vsupport::compare(e1, sp1));
    EXPECT_TRUE(vsupport::compare(e1, csp1));
    EXPECT_TRUE(vsupport::compare(e2, sp2));
    EXPECT_TRUE(vsupport::compare(e2, csp2));
    EXPECT_TRUE(vsupport::compare(e3, sp3));
    EXPECT_TRUE(vsupport::compare(e3, csp3));

    EXPECT_FALSE(vsupport::compare(e1, sp3));
    EXPECT_FALSE(vsupport::compare(e1, csp3));
    EXPECT_FALSE(vsupport::compare(e2, sp1));
    EXPECT_FALSE(vsupport::compare(e2, csp1));
    EXPECT_FALSE(vsupport::compare(e3, sp2));
    EXPECT_FALSE(vsupport::compare(e3, csp2));

    //- Setting values of individual span elements should be reflected in the owning engine.
    //
    sp1(0) = 11.0f;
    sp1(1) = 12.0f;
    sp1(2) = 13.0f;
    sp1(3) = 14.0f;
    EXPECT_TRUE(vsupport::compare(e1, {11, 12, 13, 14}));

    sp2(0) = 11.0f;
    sp2(1) = 12.0f;
    sp2(2) = 13.0f;
    sp2(3) = 14.0f;
    EXPECT_TRUE(vsupport::compare(e2, {11, 12, 13, 14}));

    sp3(0) = 11.0f;
    sp3(1) = 12.0f;
    sp3(2) = 13.0f;
    sp3(3) = 14.0f;
    EXPECT_TRUE(vsupport::compare(e3, {11, 12, 13, 14}));
}
