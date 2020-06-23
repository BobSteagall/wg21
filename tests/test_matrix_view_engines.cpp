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

TEST(FFF, YYY)
{
    static_assert(STD_LA::detail::is_owning_engine_v<mat_engine_34>);

    using t1 = STD_LA::detail::determine_owning_engine_type_t<mat_engine_34>;

    static_assert(STD_LA::detail::is_non_owning_engine_v<r_neg_engine_34>);
    static_assert(STD_LA::detail::is_non_owning_engine_v<r_tr_engine_34>);
    static_assert(STD_LA::detail::is_non_owning_engine_v<r_tr_neg_engine_34>);
}

#if 1
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

    //constexpr ptrdiff_t r = r_tr_engine_34().rows();
}
#endif

#if 1
template<class T, typename = void>
struct auto_size_test : public std::false_type
{};

template<class T>
//struct auto_size_test<T, void_t<decltype(T::rows())>>
struct auto_size_test<T, void_t<decltype(declval<T>().rows())>>
: public std::true_type
{};

template<class T, int = 0>
struct auto_size_test2 : public std::false_type
{};

template<class T>
struct auto_size_test2<T, 5>
: public std::true_type
{};

template<class T> constexpr void test_expr(T&&) {}
#define IS_CONSTEXPR(...) noexcept(test_expr(__VA_ARGS__))

template<class Lambda, int=(Lambda{}(), 0)>
inline constexpr bool is_constexpr(Lambda) { return true; }
inline constexpr bool is_constexpr(...) { return false; }

template<class T>   constexpr T   make_t() { return {};}

template<class T> inline
constexpr bool has_constexpr_size()
{
    return is_constexpr([]{ T().rows(); })  &&  is_constexpr([]{ T().columns(); });
}

template<class T> inline constexpr
bool    has_cx_size = is_constexpr([]{ T().rows(); })  &&  is_constexpr([]{ T().columns(); });

template<class ET>
class tvob
{
  public:
    using engine_type = ET;

    constexpr tvob(int = 0) : m_engine() {}
    constexpr engine_type const&  engine() const { return m_engine; }

  private:
    engine_type     m_engine;
};

template<class ET, class MCT, class VFT>
class tvob<matrix_view_engine<ET, MCT, VFT>> : public tvob<ET>
{
  public:
    using engine_type = matrix_view_engine<ET, MCT, VFT>;
    using base_type   = tvob<ET>;

    constexpr tvob(int = 0) : base_type(0), m_engine(base_type::engine()) {}
    constexpr engine_type const&  engine() const { return m_engine; }

  private:
    engine_type     m_engine;
};

template<class ET, class MFT, class VFT>
class test_engine;

template<class ET>
class erep
{
    static_assert(STD_LA::detail::is_owning_engine_v<ET>);

  public:
    using engine_type        = ET;
    using owning_engine_type = ET;

    constexpr erep() = default;
    constexpr erep(engine_type const& eng) : mp_owning_engine(&eng) {}

    constexpr owning_engine_type const& owning_engine() const { return *mp_owning_engine; }

  private:
    engine_type const*  mp_owning_engine;
};

template<class ET, class MCT, class VFT>
class erep<test_engine<ET, MCT, VFT>>
{
  public:
    using engine_type        = test_engine<ET, MCT, VFT>;
    using owning_engine_type = typename engine_type::owning_engine_type;

    constexpr erep() = default;
    constexpr erep(engine_type const& eng)        : m_non_owning_engine(eng) {}
    constexpr erep(owning_engine_type const& eng) : m_non_owning_engine(eng) {}

    constexpr owning_engine_type const& owning_engine() const { return m_non_owning_engine.owning_engine(); }

  private:
    engine_type     m_non_owning_engine;
};

template<class ET, class MFT = readable_matrix_engine_tag, class VFT = negation_view_tag>
class test_engine
{
  public:
    using engine_type        = ET;
    using owning_engine_type = ET;

    constexpr test_engine() = default;
    constexpr test_engine(engine_type const& eng) : mp_engine(&eng) {}

    constexpr ptrdiff_t     rows() const { return mp_engine->rows(); }

    constexpr owning_engine_type const& owning_engine() const   { return *mp_engine; }

  private:
    engine_type const*  mp_engine;
};

template<class ET2, class MFT2, class VFT2, class MFT, class VFT>
class test_engine<test_engine<ET2, MFT2, VFT2>, MFT, VFT>
{
  public:
    using engine_type        = test_engine<ET2, MFT2, VFT2>;
    using owning_engine_type = typename test_engine<ET2, MFT2, VFT2>::owning_engine_type;

    constexpr test_engine() = default;
    constexpr test_engine(engine_type const& eng)        : m_engine(eng) {}
    constexpr test_engine(owning_engine_type const& eng) : m_engine(eng) {}

    constexpr ptrdiff_t     rows() const { return m_engine.rows(); }

    constexpr owning_engine_type const& owning_engine() const { return m_engine.owning_engine(); }

  private:
    engine_type     m_engine;
};


constexpr auto tf()
{
//    r_tr_engine_34      tv2(e1);
//    r_neg_tr_engine_34  tv3(tv2);

//    tvob<mat_engine_34>       v1(0);
//    tvob<r_tr_engine_34>      v2(0);

    mat_engine_34                           e1;
    test_engine<mat_engine_34>              te1(e1);

    test_engine<test_engine<mat_engine_34>> te2a(te1);
    test_engine<test_engine<mat_engine_34>> te2b(e1);

    test_engine<test_engine<test_engine<mat_engine_34>>> te3a(te2a);
    test_engine<test_engine<test_engine<mat_engine_34>>> te3b(e1);

//    static_assert(e1.rows() == 3);
//    static_assert(te1.rows() == 3);

    constexpr bool b = STD_LA::detail::is_constexpr([]{ return mat_engine_34().rows();});

    matrix_negation_engine<mat_engine_34>       ne1(e1);
    matrix_negation_engine<matrix_negation_engine<mat_engine_34>>   ne2a(e1);
    matrix_negation_engine<matrix_negation_engine<mat_engine_34>>   ne2b(ne1);
    matrix_negation_engine<matrix_negation_engine<matrix_negation_engine<mat_engine_34>>>   ne3a(e1);
    matrix_negation_engine<matrix_negation_engine<matrix_negation_engine<mat_engine_34>>>   ne3b(ne2b);

    bool b1 = ne1.is_valid();
    bool b2 = ne3b.is_valid();

    //dr_matrix_engine<float> fe;

    return ne3b.rows();
}

TEST(FFF, XXX)
{
    constexpr auto i = tf();

    std::integral_constant<ptrdiff_t, tf()>     x;

    static_assert(x.value == i);

    static_assert(STD_LA::detail::is_constexpr([]{fs_matrix_engine<int,1,1>().rows();}));
    static_assert(!STD_LA::detail::is_constexpr([]{dr_matrix_engine<int>().rows();}));

/*
    static_assert(auto_size_test<mat_engine_34>::value == true);
    //static_assert(auto_size_test<dr_matrix_engine<float>>::value == true);

    static_assert(noexcept(declval<mat_engine_34>().rows()));
    static_assert(noexcept(declval<dyn_matrix<float>>().rows()));
    static_assert(!noexcept(declval<dr_matrix_engine<float>>().resize(0, 0)));

    static_assert(is_constexpr([]{ mat_engine_34().rows(); }));
//    static_assert(is_constexpr([]{ fs_matrix<float, 3, 4>().rows(); }));
    static_assert(!is_constexpr([]{ dr_matrix_engine<float>().rows(); }));
    static_assert(!is_constexpr([]{ make_t<dyn_matrix<float>>().rows(); }));

    static_assert(is_constexpr([]{ return r_tr_engine_34().rows(); }));
    static_assert(is_constexpr([]{ return "hello"; }));

//    static_assert(has_constexpr_size<fs_matrix<float,4,4>>());
//    static_assert(has_cx_size<fs_matrix<float,4,4>>);
    static_assert(has_cx_size<r_tr_engine_34>);

    static_assert(!has_constexpr_size<dyn_matrix<float>>());
    static_assert(!has_cx_size<dyn_matrix<float>>);
    static_assert(!has_cx_size<matrix_transpose_engine<dr_matrix_engine<float>>>);


    using tr_eng_1 = matrix_transpose_engine<matrix_negation_engine<matrix_transpose_engine<mat_engine_34>>>;

    static_assert(is_same_v<typename tr_eng_1::owning_engine_type, mat_engine_34>);
    static_assert(STD_LA::detail::is_owning_engine_v<tr_eng_1> == false);

    static_assert(STD_LA::detail::engine_rows_v<mat_engine_34> == 3);
    static_assert(STD_LA::detail::engine_rows_v<r_neg_engine_34> == 3);
    static_assert(STD_LA::detail::engine_rows_v<r_tr_engine_34> == 4);

    using r_tr_neg_engine_34 = matrix_transpose_engine<r_neg_engine_34>;
//    static_assert(STD_LA::detail::engine_columns_v<r_tr_neg_engine_34> == 3);
//    static_assert(STD_LA::detail::engine_rows_v<tr_eng_1> == 3);

    constexpr tvob<mat_engine_34>       v1;
    static_assert(v1.engine().rows() == 3);

    //constexpr tvob<r_tr_engine_34>      v2;
    //static_assert(v2.engine().rows() == 4);
    */
}


template<class T, int R, int C>
using fs_test_engine = STD_LA::matrix_storage_engine<T, extents<R,C>, void, STD_LA::row_major>;

TEST(GGG, HHH)
{
    STD_LA::matrix_storage_engine<float, extents<-1, -1>, std::allocator<float>, STD_LA::row_major>       m1(2, 2, 4, 4);
    STD_LA::matrix_storage_engine<float, extents<-1, -1>, std::allocator<float>, STD_LA::column_major>    m2;
//    STD_LA::matrix_storage_engine<float, extents<-1, -1>, std::allocator<float>, STD_LA::general_layout>  m3;
//    STD_LA::matrix_storage_engine<float, extents<-1, -1>, std::allocator<double>, STD_LA::column_major>   m4;

    STD_LA::matrix_storage_engine<float, extents<3, 3>, void, STD_LA::row_major>      m21;
    STD_LA::matrix_storage_engine<float, extents<4, 4>, void, STD_LA::column_major>   m22;

//    STD_LA::matrix_storage_engine<float, extents<3, 3>, std::allocator<float>, STD_LA::row_major>      m31(3, 4);
    STD_LA::matrix_storage_engine<float, extents<4, 4>, std::allocator<float>, STD_LA::column_major>   m32;


    static_assert(STD_LA::detail::is_constexpr([]{fs_test_engine<float,2,2>().rows();}));

    static_assert(STD_LA::detail::mse_traits<float, extents<-1, -1>, std::allocator<float>, STD_LA::row_major>::is_resizable);

    m2.reshape(4, 4, 20, 20);
    m1.reserve(25, 25);

    m2.span();
    m22.span();

    auto const&     m2cr = m2;
    auto const&     m22cr = m22;

    m2cr.span();
    m22cr.span();


    m32 = LST_44_1;
    m22 = LST_44_1;
    m2  = LST_34_2;

    dr_matrix_engine<float>         dr1 = LST_44_2;
    fs_matrix_engine<float, 4, 4>   fs1 = LST_44_2;

    m32 = dr1;
    m32 = fs1;

    m22 = dr1;
    m22 = fs1;

//    m32.reserve(10, 10);

    static_assert(STD_LA::detail::valid_mse_extents<extents<10>>);
    static_assert(STD_LA::detail::valid_mse_extents<extents<2,2>>);
    static_assert(!STD_LA::detail::valid_mse_extents<extents<>>);
    static_assert(!STD_LA::detail::valid_mse_extents<extents<2,2,2>>);
    static_assert(!STD_LA::detail::valid_mse_extents<extents<-5>>);
    static_assert(!STD_LA::detail::valid_mse_extents<extents<-5,0>>);
    static_assert(!STD_LA::detail::valid_mse_extents<int>);
}
#endif
