#include "test_common.hpp"

TEST(DynVectorEngine, DefaultCtor)
{
    dr_vector_engine<float>     e1;

    EXPECT_EQ(e1.size(), 0);
    EXPECT_GE(e1.capacity(), 0);
}


TEST(DynVectorEngine, DirectCtor)
{
    dr_vector_engine<float>     e1(4);

    EXPECT_EQ(e1.size(), 4);
    EXPECT_GE(e1.capacity(), 4);

    //- Verify elements are value initialized.
    //
    EXPECT_EQ(e1(0), 0.0f);
    EXPECT_EQ(e1(1), 0.0f);
    EXPECT_EQ(e1(2), 0.0f);
    EXPECT_EQ(e1(3), 0.0f);
}


TEST(DynVectorEngine, CmpEq)
{
    std::initializer_list<float>    l1 = {0, 0, 0, 0};
    std::initializer_list<float>    l2 = {13, 17, 19, 23};
    dr_vector_engine<float>         e1(4), e2(4), e3(4);

    //- Verify size, capacity, initial values.
    //
    EXPECT_EQ(e1.size(), 4);
    EXPECT_GE(e1.capacity(), 4);
    EXPECT_EQ(e1(0), 0.0f);
    EXPECT_EQ(e1(1), 0.0f);
    EXPECT_EQ(e1(2), 0.0f);
    EXPECT_EQ(e1(3), 0.0f);

    //- Verify expected equality of direct-constructed engines.
    //
    EXPECT_TRUE(v_cmp_eq(e1, e1));
    EXPECT_TRUE(v_cmp_eq(e1, e2));
    EXPECT_TRUE(v_cmp_eq(e1, e3));

    //- Verify equality against an init-list of equivalent values.
    //
    EXPECT_TRUE(v_cmp_eq(e1, l1));
    EXPECT_TRUE(v_cmp_eq(e1, {0, 0, 0, 0}));

    //- Verify inequality against an init-list of different values.
    //
    EXPECT_FALSE(v_cmp_eq(e1, l2));
    EXPECT_FALSE(v_cmp_eq(e1, {13, 17, 19, 23}));

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

    //- Verify expected equality of various argument combinations.
    //
    EXPECT_TRUE(v_cmp_eq(e2, e3));
    EXPECT_TRUE(v_cmp_eq(e2, l2));
    EXPECT_TRUE(v_cmp_eq(e2, {13, 17, 19, 23}));
    EXPECT_TRUE(v_cmp_eq(e3, l2));
    EXPECT_TRUE(v_cmp_eq(e3, {13, 17, 19, 23}));

    //- Verify expected inequality when compared to init-lists of incorrect size.
    //
    EXPECT_FALSE(v_cmp_eq(e3, {13, 17, 19}));
    EXPECT_FALSE(v_cmp_eq(e3, {13, 17, 19, 23, 29}));

    //- Verify expected inequality against init-lists and engines having different contents.
    //
    EXPECT_FALSE(v_cmp_eq(e3, {11, 17, 19, 23}));
    EXPECT_FALSE(v_cmp_eq(e3, dr_vector_engine<float>({11, 17, 19, 23})));

    EXPECT_FALSE(v_cmp_eq(e3, {13, 11, 19, 23}));
    EXPECT_FALSE(v_cmp_eq(e3, dr_vector_engine<float>({13, 11, 19, 23})));

    EXPECT_FALSE(v_cmp_eq(e3, {13, 17, 11, 23}));
    EXPECT_FALSE(v_cmp_eq(e3, dr_vector_engine<float>({13, 17, 11, 23})));

    EXPECT_FALSE(v_cmp_eq(e3, {13, 17, 19, 11}));
    EXPECT_FALSE(v_cmp_eq(e3, dr_vector_engine<float>({13, 17, 19, 11})));
}


TEST(DynVectorEngine, MoveCtor)
{
    //- Direct construct and verify initial state.
    //
    dr_vector_engine<float>     e1(4);
    EXPECT_EQ(e1.size(), 4);
    EXPECT_GE(e1.capacity(), 4);
    EXPECT_TRUE(v_cmp_eq(e1, {0, 0, 0, 0}));

    //- Change the state to new element values and verify them.
    //
    e1(0) = 13.0f;
    e1(1) = 17.0f;
    e1(2) = 19.0f;
    e1(3) = 23.0f;
    EXPECT_TRUE(v_cmp_eq(e1, {13.0f, 17.0f, 19.0f, 23.0f}));

    //- Construct a new engine via move ctor and verify that its initial state is as expected.
    //
    dr_vector_engine<float>     e2(std::move(e1));
    EXPECT_EQ(e2.size(), 4);
    EXPECT_GE(e2.capacity(), 4);
    EXPECT_FALSE(v_cmp_eq(e2, e1));
}


TEST(DynVectorEngine, CopyCtor)
{
    //- Direct construct and verify initial state.
    //
    dr_vector_engine<float>     e1(4);
    EXPECT_EQ(e1.size(), 4);
    EXPECT_EQ(e1.capacity(), 4);
    EXPECT_TRUE(v_cmp_eq(e1, {0, 0, 0, 0}));

    //- Change the state to new element values and verify them.
    //
    e1(0) = 13.0f;
    e1(1) = 17.0f;
    e1(2) = 19.0f;
    e1(3) = 23.0f;
    EXPECT_TRUE(v_cmp_eq(e1, {13.0f, 17.0f, 19.0f, 23.0f}));

    //- Construct new engines via copy ctors and verify that their initial states are as expected.
    //
    dr_vector_engine<float>     e2(e1);
    EXPECT_EQ(e2.size(), 4);
    EXPECT_GE(e2.capacity(), 4);
    EXPECT_TRUE(v_cmp_eq(e2, e1));

    dr_vector_engine<float>     e3{e1};
    EXPECT_EQ(e3.size(), 4);
    EXPECT_GE(e3.capacity(), 4);
    EXPECT_TRUE(v_cmp_eq(e3, e1));

    dr_vector_engine<float>     e4 = e1;
    EXPECT_EQ(e4.size(), 4);
    EXPECT_GE(e4.capacity(), 4);
    EXPECT_TRUE(v_cmp_eq(e4, e1));
}


TEST(DynVectorEngine, ListCtor)
{
    //- Construct new engines via list ctor and verify that their initial states are as expected.
    //
    dr_vector_engine<float>     e1 = {13, 17, 19, 23};
    dr_vector_engine<float>     e2{13, 17, 19, 23};
    dr_vector_engine<float>     e3({13, 17, 19, 23});

    EXPECT_EQ(e1.size(), 4);
    EXPECT_GE(e1.capacity(), 4);
    EXPECT_TRUE(v_cmp_eq(e1, {13, 17, 19, 23}));

    EXPECT_EQ(e2.size(), 4);
    EXPECT_GE(e2.capacity(), 4);
    EXPECT_TRUE(v_cmp_eq(e2, e1));

    EXPECT_EQ(e3.size(), 4);
    EXPECT_GE(e3.capacity(), 4);
    EXPECT_TRUE(v_cmp_eq(e3, e1));
}


TEST(DynVectorEngine, EngineCtor)
{
    //- Construct new engines via direct engine ctor and verify that their initial states are
    //  as expected.
    //
    dr_vector_engine<int>       e1{13, 17, 19, 23};
    dr_vector_engine<float>     e2(e1);
    dr_vector_engine<double>    e3(e2);
    fs_vector_engine<double, 4> e4{13, 17, 19, 23};
    dr_vector_engine<float>     e5{e4};

    EXPECT_EQ(e1.size(), 4);
    EXPECT_GE(e1.capacity(), 4);
    EXPECT_TRUE(v_cmp_eq(e1, {13, 17, 19, 23}));

    EXPECT_EQ(e2.size(), 4);
    EXPECT_GE(e2.capacity(), 4);
    EXPECT_TRUE(v_cmp_eq(e2, e1));

    EXPECT_EQ(e3.size(), 4);
    EXPECT_GE(e3.capacity(), 4);
    EXPECT_TRUE(v_cmp_eq(e3, e1));

    EXPECT_EQ(e4.size(), 4);
    EXPECT_EQ(e4.capacity(), 4);
    EXPECT_TRUE(v_cmp_eq(e4, e1));

    EXPECT_EQ(e5.size(), 4);
    EXPECT_GE(e5.capacity(), 4);
    EXPECT_TRUE(v_cmp_eq(e5, e1));
}


TEST(DynVectorEngine, MoveAssign)
{
    //- Direct construct and verify initial state.
    //
    dr_vector_engine<float>     e1(4);
    EXPECT_EQ(e1.size(), 4);
    EXPECT_GE(e1.capacity(), 4);
    EXPECT_TRUE(v_cmp_eq(e1, {0, 0, 0, 0}));

    //- List construct and verify initial state.
    //
    dr_vector_engine<float>     e2{13, 17, 19, 23};
    EXPECT_EQ(e2.size(), 4);
    EXPECT_GE(e2.capacity(), 4);
    EXPECT_TRUE(v_cmp_eq(e2, {13, 17, 19, 23}));

    //- Assign and verify.
    //
    e1 = std::move(e2);
    EXPECT_TRUE(v_cmp_eq(e1, {13, 17, 19, 23}));
}


TEST(DynVectorEngine, CopyAssign)
{
    //- Direct construct and verify initial state.
    //
    dr_vector_engine<float>     e1(4);
    EXPECT_EQ(e1.size(), 4);
    EXPECT_GE(e1.capacity(), 4);
    EXPECT_TRUE(v_cmp_eq(e1, {0, 0, 0, 0}));

    //- List construct and verify initial state.
    //
    dr_vector_engine<float>     e2{13, 17, 19, 23};
    EXPECT_EQ(e2.size(), 4);
    EXPECT_EQ(e2.capacity(), 4);
    EXPECT_TRUE(v_cmp_eq(e2, {13, 17, 19, 23}));

    //- Assign and verify.
    //
    e1 = e2;
    EXPECT_TRUE(v_cmp_eq(e1, {13, 17, 19, 23}));
}


TEST(DynVectorEngine, EngineAssign)
{
    //- Direct construct and verify initial state.
    //
    dr_vector_engine<float>     e1(3), e2(3);
    EXPECT_EQ(e1.size(), 3);
    EXPECT_GE(e1.capacity(), 3);
    EXPECT_TRUE(v_cmp_eq(e1, {0, 0, 0}));
    EXPECT_EQ(e2.size(), 3);
    EXPECT_GE(e2.capacity(), 3);
    EXPECT_TRUE(v_cmp_eq(e2, {0, 0, 0}));

    //- List construct and verify initial state.
    //
    fs_vector_engine<int, 4>    e3{13, 17, 19, 23};
    EXPECT_EQ(e3.size(), 4);
    EXPECT_GE(e3.capacity(), 4);
    EXPECT_TRUE(v_cmp_eq(e3, {13, 17, 19, 23}));

    dr_vector_engine<int>       e4{130, 170, 190, 230};
    EXPECT_EQ(e4.size(), 4);
    EXPECT_GE(e4.capacity(), 4);
    EXPECT_TRUE(v_cmp_eq(e4, {130, 170, 190, 230}));

    //- Assign and verify.
    //
    e1 = e3;
    EXPECT_TRUE(v_cmp_eq(e1, {13, 17, 19, 23}));
    EXPECT_EQ(e1.size(), 4);
    EXPECT_GE(e1.capacity(), 4);
    EXPECT_TRUE(v_cmp_eq(e1, e3));

    e2 = e4;
    EXPECT_TRUE(v_cmp_eq(e2, {130, 170, 190, 230}));
    EXPECT_EQ(e2.size(), 4);
    EXPECT_GE(e2.capacity(), 4);
    EXPECT_TRUE(v_cmp_eq(e2, e4));
}


TEST(DynVectorEngine, ListAssign)
{
    //- Direct construct and verify initial state.
    //
    std::initializer_list<float>    l1 = {13, 17, 19, 23};
    dr_vector_engine<float>         e1(4);
    dr_vector_engine<float>         e2{l1};

    EXPECT_EQ(e1.size(), 4);
    EXPECT_EQ(e1.capacity(), 4);
    EXPECT_TRUE(v_cmp_eq(e1, {0, 0, 0, 0}));
    EXPECT_EQ(e2.size(), 4);
    EXPECT_EQ(e2.capacity(), 4);
    EXPECT_TRUE(v_cmp_eq(e2, l1));

    //- Assign and verify.
    //
    e1 = l1;
    EXPECT_TRUE(v_cmp_eq(e1, {13, 17, 19, 23}));
    EXPECT_TRUE(v_cmp_eq(e1, l1));
    EXPECT_TRUE(v_cmp_eq(e1, e2));
}


TEST(DynVectorEngine, Reserve)
{
    //- List construct and verify initial state.
    //
    dr_vector_engine<float>      e1{13, 17, 19, 23};
    EXPECT_EQ(e1.size(), 4);
    EXPECT_GE(e1.capacity(), 4);
    EXPECT_LT(e1.capacity(), 256);
    EXPECT_TRUE(v_cmp_eq(e1, {13, 17, 19, 23}));

    //- Reserve extra space and verify.
    //
    e1.reserve(256);
    EXPECT_EQ(e1.size(), 4);
    EXPECT_GE(e1.capacity(), 256);
    EXPECT_TRUE(v_cmp_eq(e1, {13, 17, 19, 23}));
}


TEST(DynVectorEngine, Resize)
{
    //- List construct and verify initial state.
    //
    dr_vector_engine<float>      e1{13, 17, 19, 23};
    EXPECT_EQ(e1.size(), 4);
    EXPECT_GE(e1.capacity(), 4);
    EXPECT_TRUE(v_cmp_eq(e1, {13, 17, 19, 23}));

    //- Resize upward and verify.
    //
    e1.resize(8);
    EXPECT_EQ(e1.size(), 8);
    EXPECT_GE(e1.capacity(), 8);
    EXPECT_TRUE(v_cmp_eq(e1, {13, 17, 19, 23, 0, 0, 0, 0}));

    //- Resize downward and verify.
    //
    e1.resize(3);
    EXPECT_EQ(e1.size(), 3);
    EXPECT_GE(e1.capacity(), 8);
    EXPECT_TRUE(v_cmp_eq(e1, {13, 17, 19}));
}


TEST(DynVectorEngine, Swap)
{
    //- Direct construct and verify initial state.
    //
    dr_vector_engine<float>      e1(4);
    EXPECT_EQ(e1.size(), 4);
    EXPECT_GE(e1.capacity(), 4);
    EXPECT_TRUE(v_cmp_eq(e1, {0, 0, 0, 0}));

    //- List construct and verify initial state.
    //
    dr_vector_engine<float>      e2{13, 17, 19, 23};
    EXPECT_EQ(e2.size(), 4);
    EXPECT_GE(e2.capacity(), 4);
    EXPECT_TRUE(v_cmp_eq(e2, {13, 17, 19, 23}));

    //- Swap contents and verify.
    //
    e1.swap(e2);
    EXPECT_TRUE(v_cmp_eq(e1, {13, 17, 19, 23}));
    EXPECT_TRUE(v_cmp_eq(e2, {0, 0, 0, 0}));
    e2.swap(e1);
    EXPECT_TRUE(v_cmp_eq(e1, {0, 0, 0, 0}));
    EXPECT_TRUE(v_cmp_eq(e2, {13, 17, 19, 23}));

    //- Swap some elements and verify.
    //
    e2.swap_elements(0, 3);
    e2.swap_elements(1, 2);
    EXPECT_TRUE(v_cmp_eq(e2, {23, 19, 17, 13}));
}


#ifdef LA_USE_MDSPAN

TEST(DynVectorEngine, Span)
{
    dr_vector_engine<float>         e1{13, 17, 19, 23}, e2(4), e3{e1};
    dr_vector_engine<float> const&  ce1 = e1;

    auto    sp1  = e1.span();
    auto    csp1 = ce1.span();

    //- Const and mutable spans should have different types.
    //
    EXPECT_FALSE((std::is_same_v<decltype(sp1), decltype(csp1)>));

    //- The elements of the const and mutable spans should have the expected values.
    //
    EXPECT_EQ(sp1(0), 13.0f);
    EXPECT_EQ(sp1(1), 17.0f);
    EXPECT_EQ(sp1(2), 19.0f);
    EXPECT_EQ(sp1(3), 23.0f);
    EXPECT_EQ(csp1(0), 13.0f);
    EXPECT_EQ(csp1(1), 17.0f);
    EXPECT_EQ(csp1(2), 19.0f);
    EXPECT_EQ(csp1(3), 23.0f);

    //- Whole-object comparison between engines and spans should work as expected.
    //
    EXPECT_TRUE(v_cmp_eq(e1, sp1));
    EXPECT_TRUE(v_cmp_eq(e1, csp1));
    EXPECT_TRUE(v_cmp_eq(e3, sp1));
    EXPECT_TRUE(v_cmp_eq(e3, csp1));
    EXPECT_FALSE(v_cmp_eq(e2, sp1));
    EXPECT_FALSE(v_cmp_eq(e2, csp1));

    //- Setting values of individual span elements should be reflected in the owning engine.
    //
    sp1(0) = 11.0f;
    sp1(1) = 12.0f;
    sp1(2) = 13.0f;
    sp1(3) = 14.0f;
    EXPECT_TRUE(v_cmp_eq(e1, {11, 12, 13, 14}));
}

#endif
