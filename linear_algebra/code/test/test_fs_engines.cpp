#include "test_common.hpp"

TEST(FsVectorEngine, DefaultCtor)
{
    fs_vector_engine<float, 4>      e1;

    EXPECT_EQ(e1.elements(), 4);
    EXPECT_EQ(e1.capacity(), 4);

    //- Verify elements are value initialized.
    //
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

    //- Verify size, capacity, initial values.
    //
    EXPECT_EQ(e1.elements(), 4);
    EXPECT_EQ(e1.capacity(), 4);
    EXPECT_EQ(e1(0), 0.0f);
    EXPECT_EQ(e1(1), 0.0f);
    EXPECT_EQ(e1(2), 0.0f);
    EXPECT_EQ(e1(3), 0.0f);

    //- Verify expected equality of default-constructed engines.
    //
    EXPECT_TRUE(v_cmp_eq(e1, e1));
    EXPECT_TRUE(v_cmp_eq(e1, e2));
    EXPECT_TRUE(v_cmp_eq(e1, e3));

    //- Verify equality against an init-list of equivalent values.
    //
    EXPECT_TRUE(v_cmp_eq(e1, l1));
    EXPECT_TRUE(v_cmp_eq(e1, {0, 0, 0, 0}));

    //- Verify inequality against an init-list of different values.
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
    //- Default construct and verify initial state.
    //
    fs_vector_engine<float, 4>      e1;
    EXPECT_EQ(e1.elements(), 4);
    EXPECT_EQ(e1.capacity(), 4);
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
    fs_vector_engine<float, 4>      e2(std::move(e1));
    EXPECT_EQ(e2.elements(), 4);
    EXPECT_EQ(e2.capacity(), 4);
    EXPECT_TRUE(v_cmp_eq(e2, e1));
}


TEST(FsVectorEngine, CopyCtor)
{
    //- Default construct and verify initial state.
    //
    fs_vector_engine<float, 4>      e1;
    EXPECT_EQ(e1.elements(), 4);
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
    fs_vector_engine<float, 4>      e2(e1);
    EXPECT_EQ(e2.elements(), 4);
    EXPECT_EQ(e2.capacity(), 4);
    EXPECT_TRUE(v_cmp_eq(e2, e1));

    fs_vector_engine<float, 4>      e3{e1};
    EXPECT_EQ(e3.elements(), 4);
    EXPECT_EQ(e3.capacity(), 4);
    EXPECT_TRUE(v_cmp_eq(e3, e1));

    fs_vector_engine<float, 4>      e4 = e1;
    EXPECT_EQ(e4.elements(), 4);
    EXPECT_EQ(e4.capacity(), 4);
    EXPECT_TRUE(v_cmp_eq(e4, e1));
}


TEST(FsVectorEngine, ListCtor)
{
    //- Construct new engines via list ctor and verify that their initial states are as expected.
    //
    fs_vector_engine<float, 4>      e1 = {13, 17, 19, 23};
    fs_vector_engine<float, 4>      e2{13, 17, 19, 23};
    fs_vector_engine<float, 4>      e3({13, 17, 19, 23});

    EXPECT_EQ(e1.elements(), 4);
    EXPECT_EQ(e1.capacity(), 4);
    EXPECT_TRUE(v_cmp_eq(e1, {13, 17, 19, 23}));

    EXPECT_EQ(e2.elements(), 4);
    EXPECT_EQ(e2.capacity(), 4);
    EXPECT_TRUE(v_cmp_eq(e2, e1));

    EXPECT_EQ(e3.elements(), 4);
    EXPECT_EQ(e3.capacity(), 4);
    EXPECT_TRUE(v_cmp_eq(e3, e1));
}


TEST(FsVectorEngine, EngineCtor)
{
    //- Construct new engines via direct engine ctor and verify that their initial states are
    //  as expected.
    //
    fs_vector_engine<int, 4>        e1{13, 17, 19, 23};
    fs_vector_engine<float, 4>      e2(e1);
    fs_vector_engine<double, 4>     e3(e2);
    dr_vector_engine<double>        e4{13, 17, 19, 23};
    fs_vector_engine<float, 4>      e5{e4};

    EXPECT_EQ(e1.elements(), 4);
    EXPECT_EQ(e1.capacity(), 4);
    EXPECT_TRUE(v_cmp_eq(e1, {13, 17, 19, 23}));

    EXPECT_EQ(e2.elements(), 4);
    EXPECT_EQ(e2.capacity(), 4);
    EXPECT_TRUE(v_cmp_eq(e2, e1));

    EXPECT_EQ(e3.elements(), 4);
    EXPECT_EQ(e3.capacity(), 4);
    EXPECT_TRUE(v_cmp_eq(e3, e1));

    EXPECT_EQ(e4.elements(), 4);
    EXPECT_GE(e4.capacity(), 4);
    EXPECT_TRUE(v_cmp_eq(e4, e1));

    EXPECT_EQ(e5.elements(), 4);
    EXPECT_EQ(e5.capacity(), 4);
    EXPECT_TRUE(v_cmp_eq(e5, e1));
}


TEST(FsVectorEngine, MoveAssign)
{
    //- Default construct and verify initial state.
    //
    fs_vector_engine<float, 4>      e1;
    EXPECT_EQ(e1.elements(), 4);
    EXPECT_EQ(e1.capacity(), 4);
    EXPECT_TRUE(v_cmp_eq(e1, {0, 0, 0, 0}));

    //- List construct and verify initial state.
    //
    fs_vector_engine<float, 4>      e2{13, 17, 19, 23};
    EXPECT_EQ(e2.elements(), 4);
    EXPECT_EQ(e2.capacity(), 4);
    EXPECT_TRUE(v_cmp_eq(e2, {13, 17, 19, 23}));

    //- Assign and verify.
    //
    e1 = std::move(e2);
    EXPECT_TRUE(v_cmp_eq(e1, {13, 17, 19, 23}));
}


TEST(FsVectorEngine, CopyAssign)
{
    //- Default construct and verify initial state.
    //
    fs_vector_engine<float, 4>      e1;
    EXPECT_EQ(e1.elements(), 4);
    EXPECT_EQ(e1.capacity(), 4);
    EXPECT_TRUE(v_cmp_eq(e1, {0, 0, 0, 0}));

    //- List construct and verify initial state.
    //
    fs_vector_engine<float, 4>      e2{13, 17, 19, 23};
    EXPECT_EQ(e2.elements(), 4);
    EXPECT_EQ(e2.capacity(), 4);
    EXPECT_TRUE(v_cmp_eq(e2, {13, 17, 19, 23}));

    //- Assign and verify.
    //
    e1 = e2;
    EXPECT_TRUE(v_cmp_eq(e1, {13, 17, 19, 23}));
}


TEST(FsVectorEngine, EngineAssign)
{
    //- Default construct and verify initial state.
    //
    fs_vector_engine<float, 4>      e1, e2;
    EXPECT_EQ(e1.elements(), 4);
    EXPECT_EQ(e1.capacity(), 4);
    EXPECT_TRUE(v_cmp_eq(e1, {0, 0, 0, 0}));
    EXPECT_EQ(e2.elements(), 4);
    EXPECT_EQ(e2.capacity(), 4);
    EXPECT_TRUE(v_cmp_eq(e2, {0, 0, 0, 0}));

    //- List construct and verify initial state.
    //
    fs_vector_engine<int, 4>        e3{13, 17, 19, 23};
    EXPECT_EQ(e3.elements(), 4);
    EXPECT_EQ(e3.capacity(), 4);
    EXPECT_TRUE(v_cmp_eq(e3, {13, 17, 19, 23}));

    dr_vector_engine<int>           e4{130, 170, 190, 230};
    EXPECT_EQ(e4.elements(), 4);
    EXPECT_GE(e4.capacity(), 4);
    EXPECT_TRUE(v_cmp_eq(e4, {130, 170, 190, 230}));

    //- Assign and verify.
    //
    e1 = e3;
    EXPECT_TRUE(v_cmp_eq(e1, {13, 17, 19, 23}));
    EXPECT_TRUE(v_cmp_eq(e1, e3));

    e2 = e4;
    EXPECT_TRUE(v_cmp_eq(e2, {130, 170, 190, 230}));
    EXPECT_TRUE(v_cmp_eq(e2, e4));
}


TEST(FsVectorEngine, ListAssign)
{
    //- Default construct and verify initial state.
    //
    std::initializer_list<float>    l1 = {13, 17, 19, 23};
    fs_vector_engine<float, 4>      e1;
    fs_vector_engine<float, 4>      e2{l1};

    EXPECT_EQ(e1.elements(), 4);
    EXPECT_EQ(e1.capacity(), 4);
    EXPECT_TRUE(v_cmp_eq(e1, {0, 0, 0, 0}));
    EXPECT_EQ(e2.elements(), 4);
    EXPECT_EQ(e2.capacity(), 4);
    EXPECT_TRUE(v_cmp_eq(e2, l1));

    //- Assign and verify.
    //
    e1 = l1;
    EXPECT_TRUE(v_cmp_eq(e1, {13, 17, 19, 23}));
    EXPECT_TRUE(v_cmp_eq(e1, l1));
    EXPECT_TRUE(v_cmp_eq(e1, e2));
}


TEST(FsVectorEngine, Swap)
{
    //- Default construct and verify initial state.
    //
    fs_vector_engine<float, 4>      e1;
    EXPECT_EQ(e1.elements(), 4);
    EXPECT_EQ(e1.capacity(), 4);
    EXPECT_TRUE(v_cmp_eq(e1, {0, 0, 0, 0}));

    //- List construct and verify initial state.
    //
    fs_vector_engine<float, 4>      e2{13, 17, 19, 23};
    EXPECT_EQ(e2.elements(), 4);
    EXPECT_EQ(e2.capacity(), 4);
    EXPECT_TRUE(v_cmp_eq(e2, {13, 17, 19, 23}));

    //- Swap and verify.
    //
    e1.swap(e2);
    EXPECT_TRUE(v_cmp_eq(e1, {13, 17, 19, 23}));
    EXPECT_TRUE(v_cmp_eq(e2, {0, 0, 0, 0}));
    e2.swap(e1);
    EXPECT_TRUE(v_cmp_eq(e1, {0, 0, 0, 0}));
    EXPECT_TRUE(v_cmp_eq(e2, {13, 17, 19, 23}));
}


#ifdef LA_USE_MDSPAN

TEST(FsVectorEngine, Span)
{
    fs_vector_engine<float, 4>          e1{13, 17, 19, 23}, e2, e3{e1};
    fs_vector_engine<float, 4> const&   ce1 = e1;

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
