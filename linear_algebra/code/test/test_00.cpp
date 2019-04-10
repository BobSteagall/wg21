#include "linear_algebra.hpp"

using cx_float  = std::complex<float>;
using cx_double = std::complex<double>;
using namespace STD_LA;

//--------------------------------------------------------------------------------------------------
//- The following are several traits types used to exercise the element, engine, and operation
//  type detection meta-functions.
//
//- This operation traits type is analogous to default_matrix_operations, but with a different name.
//
struct test_add_op_traits_empty {};


//- This operation traits type has its element/engine/operation nested traits type as ordinary
//  type aliases.
//
struct test_element_add_traits_ord {};
struct test_engine_add_traits_ord {};
struct test_add_traits_ord {};

struct test_add_op_traits_ord                   //- Suffix "_ord" means "ordinary"
{
    using element_addition_traits = test_element_add_traits_ord;
    using engine_addition_traits  = test_engine_add_traits_ord;
    using addition_traits         = test_add_traits_ord;
};


//- This operation traits type is analogous to default_matrix_operation_traits, where its nested
//  traits types for element/engine/operation are template type aliases.
//
template<class T1, class T2>
struct test_element_add_traits_nta {};

template<class OT, class T1, class T2>
struct test_engine_add_traits_nta {};

template<class OT, class T1, class T2>
struct test_add_traits_nta {};

struct test_add_op_traits_nta                   //- Suffix "_nta" means "nested type alias"
{
    template<class T1, class T2>
    using element_addition_traits = test_element_add_traits_nta<T1, T2>;

    template<class OT, class T1, class T2>
    using engine_addition_traits = test_engine_add_traits_nta<OT, T1, T2>;

    template<class OT, class T1, class T2>
    using addition_traits = test_add_traits_nta<OT, T1, T2>;
};

//- This operation traits type is has the element/engine/operation traits as nested class templates.
//
struct test_add_op_traits_nct                   //- Suffix "_nct" means "nested class type"
{
    template<class T1, class T2>
    struct element_addition_traits {};

    template<class OT, class T1, class T2>
    struct engine_addition_traits {};

    template<class OT, class T1, class T2>
    struct addition_traits {};
};



#define PRINT_TYPE(T)   cout << #T << ": " << get_type_name<T>() << endl

//- The purpose of this test function is to ensure that the type detection meta-functions are
//  working properly.
//
void t200()
{
    //- Detect element traits.
    //
    using elem_t = double;

    static_assert(!detail::has_element_add_traits_v<test_add_op_traits_empty, elem_t, elem_t>);
    static_assert(!detail::has_element_add_traits_v<default_matrix_operations, elem_t, elem_t>);
    static_assert(!detail::has_element_add_traits_v<void, elem_t, elem_t>);

    static_assert(detail::has_element_add_traits_v<default_matrix_operation_traits, elem_t, elem_t>);
    static_assert(detail::has_element_add_traits_v<test_add_op_traits_ord, elem_t, elem_t>);
    static_assert(detail::has_element_add_traits_v<test_add_op_traits_nta, elem_t, elem_t>);
    static_assert(detail::has_element_add_traits_v<test_add_op_traits_nct, elem_t, elem_t>);

    //- Detect engine traits.
    //
    using eng_t = dr_matrix_engine<elem_t>;

    static_assert(!detail::has_engine_add_traits_v<test_add_op_traits_empty, eng_t, eng_t>);
    static_assert(!detail::has_engine_add_traits_v<default_matrix_operations, eng_t, eng_t>);
    static_assert(!detail::has_engine_add_traits_v<void, eng_t, eng_t>);

    static_assert(detail::has_engine_add_traits_v<default_matrix_operation_traits, eng_t, eng_t>);
    static_assert(detail::has_engine_add_traits_v<test_add_op_traits_ord, eng_t, eng_t>);
    static_assert(detail::has_engine_add_traits_v<test_add_op_traits_nta, eng_t, eng_t>);
    static_assert(detail::has_engine_add_traits_v<test_add_op_traits_nct, eng_t, eng_t>);

    //- Detect operation traits.
    //
    using opnd_t = dyn_matrix<elem_t>;

    static_assert(!detail::has_add_traits_v<test_add_op_traits_empty, opnd_t, opnd_t>);
    static_assert(!detail::has_add_traits_v<default_matrix_operations, opnd_t, opnd_t>);
    static_assert(!detail::has_add_traits_v<void, opnd_t, opnd_t>);

    static_assert(detail::has_add_traits_v<default_matrix_operation_traits, opnd_t, opnd_t>);
    static_assert(detail::has_add_traits_v<test_add_op_traits_ord, opnd_t, opnd_t>);
    static_assert(detail::has_add_traits_v<test_add_op_traits_nta, opnd_t, opnd_t>);
    static_assert(detail::has_add_traits_v<test_add_op_traits_nct, opnd_t, opnd_t>);


/*
    using LT = dyn_matrix<float>;
    using RT = dyn_matrix<float>;

    static_assert(has_add_traits_v<default_matrix_operation_traits, LT, RT>);
    static_assert(has_add_traits_v<test_matrix_operation_traits<LT, RT>, LT, RT>);
    static_assert(!has_add_traits_v<foo, LT, RT>);
    static_assert(!has_add_traits_v<double, LT, RT>);

    cout << get_type_name< detail::element_add_traits_t<default_matrix_operation_traits, float, double> >() << std::endl;
    cout << get_type_name< detail::element_add_traits_t<default_matrix_operations, float, double>       >() << std::endl;
    cout << get_type_name< detail::element_add_traits_t<test_matrix_operation_traits<float, double>, float, double> >() << std::endl;
    cout << get_type_name< detail::element_add_traits_t<test_op_traits, float, double>                  >() << std::endl;
    cout << endl;

    using t00 = detail::engine_add_traits_t<default_matrix_operations,
                                            fs_matrix_engine<double, 3, 3>,
                                            fs_matrix_engine<double, 3, 3>>;
    PRINT_TYPE(t00);

    using t01 = detail::engine_add_traits_t<default_matrix_operation_traits,
                                            fs_matrix_engine<double, 3, 3>,
                                            fs_matrix_engine<double, 3, 3>>;
    PRINT_TYPE(t01);

    using t02 = detail::engine_add_traits_t<test_op_traits,
                                            fs_matrix_engine<double, 3, 3>,
                                            fs_matrix_engine<double, 3, 3>>;
    PRINT_TYPE(t02);
    cout << endl;

    using t10 = detail::addition_traits_t<default_matrix_operations,
                                          matrix<fs_matrix_engine<double, 3, 3>, default_matrix_operations>,
                                          matrix<fs_matrix_engine<double, 3, 3>, default_matrix_operations>>;
    PRINT_TYPE(t10);

    using t11 = detail::addition_traits_t<default_matrix_operation_traits,
                                          matrix<fs_matrix_engine<double, 3, 3>, default_matrix_operation_traits>,
                                          matrix<fs_matrix_engine<double, 3, 3>, default_matrix_operation_traits>>;
    PRINT_TYPE(t11);

    using t12 = detail::addition_traits_t<test_op_traits,
                                          matrix<fs_matrix_engine<double, 3, 3>, test_op_traits>,
                                          matrix<fs_matrix_engine<double, 3, 3>, test_op_traits>>;
    PRINT_TYPE(t12);
    cout << endl;
*/
}


void t201()
{
    fs_matrix<double, 3, 3> fm1;
    dyn_matrix<double>      dmd1(3, 3);

    cout << get_type_name<dyn_matrix<double>>() << endl;
    cout << get_type_name<decltype(fm1 + dmd1)>() << endl;
    cout << get_type_name<decltype(fm1.t() + dmd1.t())>() << endl;
    fm1 + dmd1;
}

void t202()
{
    fs_vector<double, 3>    fv1;
    dyn_vector<double>      dvd1(3);

    cout << get_type_name<decltype(fv1 + dvd1)>() << endl;
    cout << get_type_name<decltype(fv1 + dvd1)>() << endl;
    fv1 + dvd1;
}

void
TestGroup00()
{
    t200();
    t201();
    t202();
}
