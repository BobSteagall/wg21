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


void
TestGroup00()
{
    t200();
}
