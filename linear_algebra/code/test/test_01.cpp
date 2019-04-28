
#include "linear_algebra.hpp"

using cx_float  = std::complex<float>;
using cx_double = std::complex<double>;
using namespace STD_LA;

//- Some detection idiom stuff to make sure SFINAE is working for fixed-size
//  -vs- dynamic interfaces.
//
template<typename T, typename = void>
struct has_resize : std::false_type {};

template<typename T>
struct has_resize<T, std::void_t<decltype(std::declval<T>().resize(0, 0))>>
:   std::true_type {};

template<typename T, typename = void>
struct has_resize_1 : std::false_type {};

template<typename T>
struct has_resize_1<T, std::void_t<decltype(std::declval<T>().resize(0))>>
:   std::true_type {};

template<typename T>
inline constexpr bool   has_resize_1_v = has_resize_1<T>::value;


template<typename T, typename = void>
struct has_resize_2 : std::false_type {};

template<typename T>
struct has_resize_2<T, std::void_t<decltype(std::declval<T>().resize(0, 0))>>
:   std::true_type {};

template<typename T>
inline constexpr bool   has_resize_2_v = has_resize_2<T>::value;

template<typename T>
inline constexpr bool   has_resize_v = has_resize_1_v<T> || has_resize_2_v<T>;


template<typename T, typename = void>
struct has_element_addition_traits
:   std::false_type {};

template<typename T>
struct has_element_addition_traits<T, void_t<typename T::element_addition_traits>>
:   std::true_type {};

template<typename T, typename M1, typename M2, typename = void>
struct has_element_addition_traits_mt
:   std::false_type {};

template<typename T, typename M1, typename M2>
struct has_element_addition_traits_mt<T, M1, M2, void_t<typename T::template element_addition_traits<M1, M2>>>
:   std::true_type {};

template<typename T, typename M1, typename M2>
constexpr bool  has_element_addition_traits_v = has_element_addition_traits<T>::type::value ||
                                                has_element_addition_traits_mt<T, M1, M2>::type::value;


template<class T1, class T2, class T3, class DEF>
struct nv_traits_chooser3;

template<class T1, class DEF>
struct nv_traits_chooser3<T1, void, void, DEF>
{
    using type = T1;
};

template<class T2, class DEF>
struct nv_traits_chooser3<void, T2, void, DEF>
{
    using type = T2;
};

template<class T3, class DEF>
struct nv_traits_chooser3<void, void, T3, DEF>
{
    using type = T3;
};

template<class DEF>
struct nv_traits_chooser3<void, void, void, DEF>
{
    using type = DEF;
};

template<class T1, class T2, class DEF>
struct nv_traits_chooser2;

template<class T1, class DEF>
struct nv_traits_chooser2<T1, void, DEF>
{
    using type = T1;
};

template<class T2, class DEF>
struct nv_traits_chooser2<void, T2, DEF>
{
    using type = T2;
};

template<class DEF>
struct nv_traits_chooser2<void, void, DEF>
{
    using type = DEF;
};

//------
//
template<typename OT, typename = void>
struct extract_element_addition_traits_stf : public std::false_type
{
    using type = void;
};

template<typename OT>
struct extract_element_addition_traits_stf<OT, void_t<decltype(std::declval<typename OT::element_addition_traits>())>>  : public std::true_type
{
    using type = typename OT::element_addition_traits;
};


template<typename OT, typename T1, typename T2, typename = void>
struct extract_element_addition_traits_mtf : public std::false_type
{
    using type = void;
};

template<typename OT, typename T1, typename T2>
struct extract_element_addition_traits_mtf<OT, T1, T2, void_t<decltype(std::declval<typename OT::template element_addition_traits<T1, T2>>())>> : public std::true_type
{
    using type = typename OT::template element_addition_traits<T1, T2>;
};


template<template<typename, typename> typename OT, typename T1, typename T2, typename = void>
struct extract_element_addition_traits_ttf : public std::false_type
{
    using type = void;
};

template<template<typename, typename> typename OT, typename T1, typename T2>
struct extract_element_addition_traits_ttf<OT, T1, T2, void_t<decltype(std::declval<typename OT<T1, T2>::element_addition_traits>())>> : public std::true_type
{
    using type = typename OT<T1, T2>::element_addition_traits;
};


template<typename OT, typename T1, typename T2>
struct extract_element_addition_traits
{
    using CT1 = typename extract_element_addition_traits_stf<OT>::type;
    using CT2 = typename extract_element_addition_traits_mtf<OT, T1, T2>::type;
    using DEF = STD_LA::matrix_element_addition_traits<T1, T2>;

    using type = typename nv_traits_chooser2<CT1, CT2, DEF>::type;
};

template<typename OT, typename T1, typename T2>
using extract_element_addition_traits_t = typename extract_element_addition_traits<OT, T1, T2>::type;


//------
//
template<typename OT, typename = void>
struct extract_engine_addition_traits_stf
{
    using type = void;
};

template<typename OT>
struct extract_engine_addition_traits_stf<OT, void_t<decltype(std::declval<typename OT::engine_addition_traits>())>>
{
    using type = typename OT::engine_addition_traits;
};


template<typename OT, typename ET1, typename ET2, typename = void>
struct extract_engine_addition_traits_mtf
{
    using type = void;
};

template<typename OT, typename ET1, typename ET2>
struct extract_engine_addition_traits_mtf<OT, ET1, ET2, void_t<decltype(std::declval<typename OT::template engine_addition_traits<ET1, ET2>>())>>
{
    using type = typename OT::template engine_addition_traits<ET1, ET2>;
};


template<typename OT, typename ET1, typename ET2>
struct extract_engine_addition_traits
{
    using CET1 = typename extract_engine_addition_traits_stf<OT>::type;
    using CET2 = typename extract_engine_addition_traits_mtf<OT, ET1, ET2>::type;
    using DEF  = STD_LA::matrix_engine_addition_traits<OT, ET1, ET2>;

    using type = typename nv_traits_chooser2<CET1, CET2, DEF>::type;
};

template<typename OT, typename ET1, typename ET2>
using extract_engine_addition_traits_t = typename extract_engine_addition_traits<OT, ET1, ET2>::type;


//------
//
template<typename OT, typename = void>
struct extract_addition_traits_stf
{
    using type = void;
};

template<typename OT>
struct extract_addition_traits_stf<OT, void_t<decltype(std::declval<typename OT::addition_traits>())>>
{
    using type = typename OT::addition_traits;
};


template<typename OT, typename OP1, typename OP2, typename = void>
struct extract_addition_traits_mtf
{
    using type = void;
};

template<typename OT, typename OP1, typename OP2>
struct extract_addition_traits_mtf<OT, OP1, OP2, void_t<decltype(std::declval<typename OT::template addition_traits<OP1, OP2>>())>>
{
    using type = typename OT::template addition_traits<OP1, OP2>;
};


template<typename OT, typename OP1, typename OP2>
struct extract_addition_traits
{
    using COP1 = typename extract_addition_traits_stf<OT>::type;
    using COP2 = typename extract_addition_traits_mtf<OT, OP1, OP2>::type;
    using DEF  = STD_LA::matrix_addition_traits<OT, OP1, OP2>;

    using type = typename nv_traits_chooser2<COP1, COP2, DEF>::type;
};

template<typename OT, typename OP1, typename OP2>
using extract_addition_traits_t = typename extract_addition_traits<OT, OP1, OP2>::type;


template<class T, class L, class R>
constexpr bool  has_add_traits_v = extract_element_addition_traits_mtf<T, L, R>::value ||
                                   extract_element_addition_traits_stf<T>::value
                                    ;

template<class OT1, class OT2>
struct extract_element_addition
{
};

struct foo {};

template<class T1, class T2>
struct test_element_addition_traits {};

template<class T1, class T2>
struct test_matrix_operation_traits
{
    using element_addition_traits = test_element_addition_traits<T1, T2>;
};

struct test_element_add_traits
{
    using type = double;
};

struct test_engine_add_traits
{
    using type = dr_matrix_engine<double>;
};

struct test_add_traits
{
    using type = matrix<dr_matrix_engine<double>>;
};

struct test_op_traits
{
    using element_addition_traits = test_element_add_traits;
    using engine_addition_traits  = test_engine_add_traits;
    using addition_traits         = test_add_traits;
};


#define PRINT_TYPE(T)   cout << get_type_name<T>() << endl

void t200()
{
    using LT = dyn_matrix<float>;
    using RT = dyn_matrix<float>;

    static_assert(has_element_addition_traits_v<default_matrix_operation_traits, LT, RT>);
    static_assert(!has_element_addition_traits_v<void, LT, RT>);

    static_assert(has_add_traits_v<default_matrix_operation_traits, LT, RT>);
    static_assert(has_add_traits_v<test_matrix_operation_traits<LT, RT>, LT, RT>);
    static_assert(!has_add_traits_v<foo, LT, RT>);
    static_assert(!has_add_traits_v<double, LT, RT>);

    cout << get_type_name< extract_element_addition_traits_t<default_matrix_operation_traits, float, double> >() << std::endl;
    cout << get_type_name< extract_element_addition_traits_t<default_matrix_operations, float, double>       >() << std::endl;
    cout << get_type_name< extract_element_addition_traits_t<test_matrix_operation_traits<float, double>, float, double>                  >() << std::endl;
    cout << get_type_name< extract_element_addition_traits_t<test_op_traits, float, double>                  >() << std::endl;

    using t00 = extract_engine_addition_traits_t<default_matrix_operations,
                                                 fs_matrix_engine<double, 3, 3>,
                                                 fs_matrix_engine<double, 3, 3>>;
    PRINT_TYPE(t00);

    using t01 = extract_engine_addition_traits_t<default_matrix_operation_traits,
                                                 fs_matrix_engine<double, 3, 3>,
                                                 fs_matrix_engine<double, 3, 3>>;
    PRINT_TYPE(t01);

    using t02 = extract_engine_addition_traits_t<test_op_traits,
                                                 fs_matrix_engine<double, 3, 3>,
                                                 fs_matrix_engine<double, 3, 3>>;
    PRINT_TYPE(t02);

    using t10 = extract_addition_traits_t<default_matrix_operations,
                                          matrix<fs_matrix_engine<double, 3, 3>, default_matrix_operations>,
                                          matrix<fs_matrix_engine<double, 3, 3>, default_matrix_operations>>;
    PRINT_TYPE(t10);

    using t11 = extract_addition_traits_t<default_matrix_operation_traits,
                                          matrix<fs_matrix_engine<double, 3, 3>, default_matrix_operation_traits>,
                                          matrix<fs_matrix_engine<double, 3, 3>, default_matrix_operation_traits>>;
    PRINT_TYPE(t11);

    using t12 = extract_addition_traits_t<test_op_traits,
                                          matrix<fs_matrix_engine<double, 3, 3>, test_op_traits>,
                                          matrix<fs_matrix_engine<double, 3, 3>, test_op_traits>>;
    PRINT_TYPE(t12);
}


void t01()
{
    PRINT_FN_NAME(t01);

    static_assert(is_field_v<double>);
    static_assert(!is_field_v<std::string>);
    static_assert(is_ring_v<double>);
    static_assert(!is_ring_v<std::string>);
    static_assert(is_semi_ring_v<double>);
    static_assert(!is_semi_ring_v<std::string>);

    static_assert(!is_complex_v<std::string>);
    static_assert(!is_complex_v<double>);
    static_assert(is_complex_v<std::complex<int>>);

    static_assert(is_matrix_element_v<double>);
    static_assert(is_matrix_element_v<std::complex<double>>);
    static_assert(is_matrix_element_v<std::complex<int32_t>>);

    static_assert(!is_matrix_element_v<std::string>);
    static_assert(!is_matrix_element_v<std::complex<std::string>>);

    //- use detection idiom stuff from above.
    //
    static_assert(!has_resize_v<fs_matrix<double, 3, 3>>);
    static_assert(has_resize_v<dyn_matrix<double>>);
}

void t02()
{
    PRINT_FN_NAME(t02);

    fs_vector_engine<double, 2>         e12;
    fs_vector_engine<cx_double, 3>      e13;
    fs_matrix_engine<double, 2, 2>      e22;
    fs_matrix_engine<cx_double, 3, 3>   e23;

    dr_vector_engine<double>       de12;
    dr_vector_engine<cx_double>    de13;
    dr_matrix_engine<double>       de22;
    dr_matrix_engine<cx_double>    de23;

    matrix_transpose_view<fs_matrix_engine<cx_double, 3, 3>>  te2(e23);
    matrix_transpose_view<dr_matrix_engine<cx_double>>        te3(de23);

#ifndef ENFORCE_COMPLEX_OPERAND_HOMOGENEITY
    matrix_element_promotion_t<int32_t, cx_double>     v1 = 0;
    matrix_element_promotion_t<cx_float, double>       v2 = 0;
    matrix_element_promotion_t<double, cx_float>       v3 = 0;
    matrix_element_promotion_t<cx_float, cx_double>    v4 = 0;
#endif
}

void t03()
{
    PRINT_FN_NAME(t03);

    fs_vector<double, 3>    fv1;
    fs_matrix<double, 3, 3> fm1;

    dyn_vector<double>      dv1(16);
    dyn_matrix<double>      dmd1(16, 16);
}

void t04()
{
    PRINT_FN_NAME(t04);

    float       f = 1.0f;
    double      d = 1.0;
    cx_double   c = {1.0, 0.0};

    dyn_matrix<float>       mf(3, 3);
    dyn_matrix<double>      md(3, 3);
    dyn_matrix<cx_double>   mc(3, 3);

    auto    m01 = mf * f;
    auto    m02 = md * d;
    auto    m03 = mc * c;
    auto    m04 = mf * d;
    auto    m05 = md * f;

    auto    m11 = f * mf;
    auto    m12 = d * md;
    auto    m13 = c * mc;
    auto    m14 = d * mf;
    auto    m15 = f * md;

    auto    m21 = mf * mf;
    auto    m22 = md * md;
    auto    m23 = mc * mc;
    auto    m24 = md * mf;
    auto    m25 = mf * md;
}

void t05()
{
    PRINT_FN_NAME(t05);

    float       f = 1.0f;
    double      d = 1.0;
    cx_double   c = {1.0, 0.0};

    fs_matrix<float, 3, 3>      mf;
    fs_matrix<double, 3, 3>     md;
    fs_matrix<cx_double, 3, 3>  mc;

    auto    m01 = mf * f;
    auto    m02 = md * d;
    auto    m03 = mc * c;
    auto    m04 = mf * d;
    auto    m05 = md * f;

    auto    m11 = f * mf;
    auto    m12 = d * md;
    auto    m13 = c * mc;
    auto    m14 = d * mf;
    auto    m15 = f * md;

    auto    m21 = mf * mf;
    auto    m22 = md * md;
    auto    m23 = mc * mc;
    auto    m24 = md * mf;
    auto    m25 = mf * md;

    fs_matrix<double, 3, 7>     md2;
    fs_matrix<float, 7, 5>      md3;

    auto    m31 = md2 * md3;
}

void t06()
{
    PRINT_FN_NAME(t06);

    float       f = 1.0f;
    double      d = 1.0;
    cx_double   c = {1.0, 0.0};

    dyn_matrix<float>       dmf(3, 3);
    dyn_matrix<double>      dmd(3, 3);
    dyn_matrix<cx_double>   dmc(3, 3);

    fs_matrix<float, 3, 3>      fmf;
    fs_matrix<double, 3, 3>     fmd;
    fs_matrix<cx_double, 3, 3>  fmc;

    auto    m01 = dmf*fmf;
    auto    m02 = dmd*fmd;
    auto    m03 = dmc*fmc;
    auto    m04 = fmf*dmf;
    auto    m05 = fmd*dmd;
    auto    m06 = fmc*dmc;
    auto    m07 = fmf*fmd;
    auto    m08 = fmf*fmf;

    dmf.resize(1, 1, 3, 3);
    dmf.swap_rows(1, 2);

    //fmf.resize(1, 1, 3, 3);
    //fmf.swap_rows(1, 2);
}

#if 0
void t08()
{
    PRINT_FN_NAME(t08);

    float       f = 1.0f;
    double      d = 1.0;
    cx_double   c = {1.0, 0.0};

    dyn_vector<float>       dvf(3);
    dyn_vector<double>      dvd(3);
    dyn_vector<cx_double>   dvc(3);

    fs_vector<float, 3>     fvf;
    fs_vector<double, 3>    fvd;
    fs_vector<cx_double, 3> fvc;

    auto    r01 = dvf * f;
    auto    r02 = dvd * d;
    auto    r03 = dvc * c;
    auto    r04 = dvf * d;
    auto    r05 = dvd * f;

    auto    r11 = f * dvf;
    auto    r12 = d * dvd;
    auto    r13 = c * dvc;
    auto    r14 = d * dvf;
    auto    r15 = f * dvd;

    auto    r21 = fvf * f;
    auto    r22 = fvd * d;
    auto    r23 = fvc * c;
    auto    r24 = fvf * d;
    auto    r25 = fvd * f;

    auto    r31 = f * fvf;
    auto    r32 = d * fvd;
    auto    r33 = c * fvc;
    auto    r34 = d * fvf;
    auto    r35 = f * fvd;
}

void t09()
{
    PRINT_FN_NAME(t09);

    dyn_vector<float>       dvf(3);
    dyn_vector<double>      dvd(3);

    fs_vector<float, 3>     fvf;
    fs_vector<double, 3>    fvd;

    auto    r11 = dvf * dvf;
    auto    r12 = fvf * dvf;
    auto    r13 = dvf * fvf;
    auto    r14 = fvf * fvf;

    auto    r21 = dvf * dvd;
    auto    r22 = fvf * dvd;
    auto    r23 = dvf * fvd;
    auto    r24 = fvf * fvd;

    auto    r31 = dvf * dvd;
    auto    r32 = fvf * dvd;
    auto    r33 = dvf * fvd;
    auto    r34 = fvf * fvd;
}

void t10()
{
    PRINT_FN_NAME(t10);

    dyn_vector<float>       dvf(3);
    dyn_vector<double>      dvd(3);

    fs_vector<float, 3>     fvf;
    fs_vector<double, 3>    fvd;

    dyn_matrix<double>      dmd(3, 3);
    dyn_matrix<float>       dmf(3, 3);
    dyn_matrix<float>       dmf_cv(3, 1);
    dyn_matrix<float>       dmf_rv(1, 3);

    fs_matrix<double, 3, 3> fmd;
    fs_matrix<float, 3, 3>  fmf;
    fs_matrix<float, 3, 1>  fmf_cv;
    fs_matrix<float, 1, 3>  fmf_rv;

    auto    r02 = dmf_cv * dvf;     //- dyn_matrix<float>
    auto    r03 = dvf * dmf;        //- dyn_vector<float>

    auto    r12 = dmf_cv * dvd;     //- dyn_matrix<double>
    auto    r13 = dvf * dmd;        //- dyn_vector<double>

    auto    r22 = fmf_cv * fvf;     //- fs_matrix<float, 3, 3>
    auto    r23 = fvf * fmf;        //- fs_vector<float, 3>

    auto    r32 = fmf_cv * fvd;     //- fs_matrix<double, 3, 3>
    auto    r33 = fvf * fmd;        //- fs_vector<3>
}

void t20()
{
    PRINT_FN_NAME(t20);

    dyn_vector<float>       dvf(3, 3);
    dyn_vector<double>      dvd(3, 3);

    fs_vector<float, 3>     fvf;
    fs_vector<double, 3>    fvd;

    dyn_matrix<float>       dmf(3, 3);
    dyn_matrix<double>      dmd(3, 3);

    fs_matrix<float, 3, 3>  fmf;
    fs_matrix<double, 3, 3> fmd;

    auto    r21 = dvf + dvf;
    auto    r22 = dvf + dvd;
    auto    r23 = dvd + dvf;

    auto    r24 = fvf + fvf;
    auto    r25 = fvf + fvd;
    auto    r26 = fvd + fvf;

    auto    r27 = dvf + fvf;
    auto    r28 = dvf + fvd;
    auto    r29 = dvd + fvd;

    auto    r30 = fmf + dmf;
    auto    r31 = fmf + dmd;
    auto    r32 = fmd + dmd;

    auto    r41 = dmf + dmf;
    auto    r42 = dmf + dmd;
    auto    r43 = dmd + dmf;

    auto    r44 = fmf + fmf;
    auto    r45 = fmf + fmd;
    auto    r46 = fmd + fmf;

    auto    r47 = dmf + fmf;
    auto    r48 = dmf + fmd;
    auto    r49 = dmd + fmd;

    auto    r50 = fmf + dmf;
    auto    r51 = fmf + dmd;
    auto    r52 = fmd + dmd;
}

void t21()
{
    PRINT_FN_NAME(t21);

    dyn_vector<float>       dvf(3, 3);
    dyn_vector<double>      dvd(3, 3);

    fs_vector<float, 3>     fvf;
    fs_vector<double, 3>    fvd;

    dyn_matrix<float>       dmf(3, 3);
    dyn_matrix<double>      dmd(3, 3);

    fs_matrix<float, 3, 3>  fmf;
    fs_matrix<double, 3, 3> fmd;

    auto    r21 = dvf - dvf;
    auto    r22 = dvf - dvd;
    auto    r23 = dvd - dvf;

    auto    r24 = fvf - fvf;
    auto    r25 = fvf - fvd;
    auto    r26 = fvd - fvf;

    auto    r27 = dvf - fvf;
    auto    r28 = dvf - fvd;
    auto    r29 = dvd - fvd;

    auto    r30 = fmf - dmf;
    auto    r31 = fmf - dmd;
    auto    r32 = fmd - dmd;

    auto    r41 = dmf - dmf;
    auto    r42 = dmf - dmd;
    auto    r43 = dmd - dmf;

    auto    r44 = fmf - fmf;
    auto    r45 = fmf - fmd;
    auto    r46 = fmd - fmf;

    auto    r47 = dmf - fmf;
    auto    r48 = dmf - fmd;
    auto    r49 = dmd - fmd;

    auto    r50 = fmf - dmf;
    auto    r51 = fmf - dmd;
    auto    r52 = fmd - dmd;
}


void t22()
{
    PRINT_FN_NAME(t22);

    dyn_vector<float>       dvf(3, 3);
    fs_vector<float, 3>     fvf;
    dyn_matrix<float>       dmf(3, 3);
    fs_matrix<float, 3, 3>  fmf;

    auto    r03 = -dvf;
    auto    r04 = -fvf;
    auto    r05 = -dmf;
    auto    r06 = -fmf;
}

void t23()
{
    PRINT_FN_NAME(t23);

    dyn_vector<float>       dvf(3, 3);
    dyn_vector<double>      dvd(3, 3);

    fs_vector<float, 3>     fvf;
    fs_vector<double, 3>    fvd;

    dyn_matrix<float>           dmf(3, 3);
    dyn_matrix<double>          dmd(3, 3);

    fs_matrix<float, 3, 3>      fmf;
    fs_matrix<double, 3, 3>     fmd;

    auto    r29 = dvd.t() + fvd.t();

    auto    r30 = fmf.t() + dmf;
    auto    r31 = fmf + dmd.t();
    auto    r32 = fmd.t() + dmd.t();

    auto    r41 = dmf.t() + dmf;
    auto    r42 = dmf + dmd.t();
    auto    r43 = dmd.t() + dmf.t();

    auto    r44 = fmf.t() + fmf;
    auto    r45 = fmf + fmd.t();
    auto    r46 = fmd.t() + fmf.t();

    auto    r47 = dmf.t() + fmf;
    auto    r48 = dmf + fmd.t();
    auto    r49 = dmd.t() + fmd.t();

    auto    r50 = fmf.t() + dmf;
    auto    r51 = fmf + dmd.t();
    auto    r52 = fmd.t() + dmd.t();
}

void t24()
{
    PRINT_FN_NAME(t24);

    dyn_vector<float>       dvf(3, 3);
    dyn_vector<double>      dvd(3, 3);

    fs_vector<float, 3>     fvf;
    fs_vector<double, 3>    fvd;

    dyn_matrix<float>       dmf(3, 3);
    dyn_matrix<double>      dmd(3, 3);

    fs_matrix<float, 3, 3>  fmf;
    fs_matrix<double, 3, 3> fmd;

    auto    r29 = dvd.t() - fvd.t();

    auto    r30 = fmf.t() - dmf;
    auto    r31 = fmf - dmd.t();
    auto    r32 = fmd.t() - dmd.t();

    auto    r41 = dmf.t() - dmf;
    auto    r42 = dmf - dmd.t();
    auto    r43 = dmd.t() - dmf.t();

    auto    r44 = fmf.t() - fmf;
    auto    r45 = fmf - fmd.t();
    auto    r46 = fmd.t() - fmf.t();

    auto    r47 = dmf.t() - fmf;
    auto    r48 = dmf - fmd.t();
    auto    r49 = dmd.t() - fmd.t();

    auto    r50 = fmf.t() - dmf;
    auto    r51 = fmf - dmd.t();
    auto    r52 = fmd.t() - dmd.t();
}

void t25()
{
    PRINT_FN_NAME(t25);

    dyn_vector<float>       dvf(3, 3);
    fs_vector<float, 3>     fvf;
    dyn_matrix<float>       dmf(3, 3);
    fs_matrix<float, 3, 3>  fmf;

    auto    r01 = -dvf.t();
    auto    r02 = -fvf.t();
    auto    r03 = -dmf.t();
    auto    r04 = -fmf.t();
}

void t30()
{
    PRINT_FN_NAME(t30);

    float       f = 1.0f;
    double      d = 1.0;
    cx_double   c = {1.0, 0.0};

    dyn_vector<float>       dvf(3);
    dyn_vector<double>      dvd(3);
    dyn_vector<cx_double>   dvc(3);

    fs_vector<float, 3>     fvf;
    fs_vector<double, 3>    fvd;
    fs_vector<cx_double, 3> fvc;

    dyn_matrix<float>           dmf(3, 3);
    dyn_matrix<double>          dmd(3, 3);
    dyn_matrix<cx_double>       dmc(3, 3);

    fs_matrix<float, 3, 3>      fmf;
    fs_matrix<double, 3, 3>     fmd;
    fs_matrix<cx_double, 3, 3>  fmc;

    auto    r01 = -(dmf*(fmd*fvd) + dvf - fvf)*c;
}


void    t100();

#endif

int main()
{
#if 0
    t01();
    t02();
    t03();
    t04();
    t05();
    t06();

    t08();
    t09();
    t10();

    t20();
    t21();
    t22();
    t23();
    t24();
    t25();
    t30();

    t100();
#else
    t200();
#endif

    return 0;
}
