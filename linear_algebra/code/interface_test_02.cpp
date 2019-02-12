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
struct has_resize<T, std::void_t<decltype(std::declval<T>().resize(0,0))>>
:   std::true_type {};

template<typename T>
inline constexpr bool   has_resize_v = has_resize<T>::value;

void    t01()
{
    constexpr bool  b0 = is_complex_v<std::string>;
    constexpr bool  b1 = is_complex_v<double>;
    constexpr bool  b2 = is_complex_v<std::complex<int>>;

    constexpr bool  b4 = is_matrix_element_v<double>;
    constexpr bool  b5 = is_matrix_element_v<std::complex<double>>;
    constexpr bool  b6 = is_matrix_element_v<std::complex<int32_t>>;

    constexpr bool  b001 = is_matrix_element_v<std::string>;
    constexpr bool  b002 = is_matrix_element_v<std::complex<std::string>>;

    //- use detection idiom stuff from above.
    //
    constexpr bool  b003 = has_resize_v<fs_matrix<double, 3, 3>>;
    constexpr bool  b004 = has_resize_v<dyn_matrix<double>>;
}

void t02()
{
    fs_matrix_engine<double, 2, 2>      e22;
    fs_matrix_engine<cx_double, 3, 3>   e33;

    dr_matrix_engine<double>       de2;
    dr_matrix_engine<cx_double>    de3;

    matrix_transpose_engine<fs_matrix_engine<cx_double, 3, 3>>  te2(e33);
    matrix_transpose_engine<dr_matrix_engine<cx_double>>       te3(de3);

#ifndef ENFORCE_COMPLEX_OPERAND_HOMOGENEITY
    matrix_element_promotion_t<int32_t, cx_double>     v1 = 0;
    matrix_element_promotion_t<cx_float, double>       v2 = 0;
    matrix_element_promotion_t<double, cx_float>       v3 = 0;
    matrix_element_promotion_t<cx_float, cx_double>    v4 = 0;
#endif
}

void t03()
{
    fs_col_vector<double, 3>    fcv1;
    fs_row_vector<double, 3>    frv1;
    fs_matrix<double, 3, 3>     fm1;

    dyn_col_vector<double>      dcv1(16);
    dyn_row_vector<double>      drv1(16);
    dyn_matrix<double>          dmd1(16, 16);
}

void t04()
{
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
}

void t07()
{
    float       f = 1.0f;
    double      d = 1.0;
    cx_double   c = {1.0, 0.0};

    dyn_col_vector<float>       dcvf(3);
    dyn_col_vector<double>      dcvd(3);
    dyn_col_vector<cx_double>   dcvc(3);

    fs_col_vector<float, 3>     fcvf;
    fs_col_vector<double, 3>    fcvd;
    fs_col_vector<cx_double, 3> fcvc;

    auto    r01 = dcvf * f;
    auto    r02 = dcvd * d;
    auto    r03 = dcvc * c;
    auto    r04 = dcvf * d;
    auto    r05 = dcvd * f;

    auto    r11 = f * dcvf;
    auto    r12 = d * dcvd;
    auto    r13 = c * dcvc;
    auto    r14 = d * dcvf;
    auto    r15 = f * dcvd;

    auto    r21 = fcvf * f;
    auto    r22 = fcvd * d;
    auto    r23 = fcvc * c;
    auto    r24 = fcvf * d;
    auto    r25 = fcvd * f;

    auto    r31 = f * fcvf;
    auto    r32 = d * fcvd;
    auto    r33 = c * fcvc;
    auto    r34 = d * fcvf;
    auto    r35 = f * fcvd;
}

void t08()
{
    float       f = 1.0f;
    double      d = 1.0;
    cx_double   c = {1.0, 0.0};

    dyn_row_vector<float>       drvf(3);
    dyn_row_vector<double>      drvd(3);
    dyn_row_vector<cx_double>   drvc(3);

    fs_row_vector<float, 3>     frvf;
    fs_row_vector<double, 3>    frvd;
    fs_row_vector<cx_double, 3> frvc;

    auto    r01 = drvf * f;
    auto    r02 = drvd * d;
    auto    r03 = drvc * c;
    auto    r04 = drvf * d;
    auto    r05 = drvd * f;

    auto    r11 = f * drvf;
    auto    r12 = d * drvd;
    auto    r13 = c * drvc;
    auto    r14 = d * drvf;
    auto    r15 = f * drvd;

    auto    r21 = frvf * f;
    auto    r22 = frvd * d;
    auto    r23 = frvc * c;
    auto    r24 = frvf * d;
    auto    r25 = frvd * f;

    auto    r31 = f * frvf;
    auto    r32 = d * frvd;
    auto    r33 = c * frvc;
    auto    r34 = d * frvf;
    auto    r35 = f * frvd;
}

void t09()
{
    dyn_col_vector<float>       dcvf(3);
    dyn_col_vector<double>      dcvd(3);

    fs_col_vector<float, 3>     fcvf;
    fs_col_vector<double, 3>    fcvd;

    dyn_row_vector<float>       drvf(3);
    dyn_row_vector<double>      drvd(3);

    fs_row_vector<float, 3>     frvf;
    fs_row_vector<double, 3>    frvd;

    auto    r01 = drvf * dcvf;
    auto    r02 = frvf * dcvf;
    auto    r03 = drvf * fcvf;
    auto    r04 = frvf * fcvf;

    auto    r11 = dcvf * drvf;
    auto    r12 = fcvf * drvf;
    auto    r13 = dcvf * frvf;
    auto    r14 = fcvf * frvf;

    auto    r21 = drvf * dcvd;
    auto    r22 = frvf * dcvd;
    auto    r23 = drvf * fcvd;
    auto    r24 = frvf * fcvd;

    auto    r31 = dcvf * drvd;
    auto    r32 = fcvf * drvd;
    auto    r33 = dcvf * frvd;
    auto    r34 = fcvf * frvd;
}

void t10()
{
    dyn_col_vector<float>       dcvf(3);
    dyn_col_vector<double>      dcvd(3);

    fs_col_vector<float, 3>     fcvf;
    fs_col_vector<double, 3>    fcvd;

    dyn_row_vector<float>       drvf(3);
    dyn_row_vector<double>      drvd(3);

    fs_row_vector<float, 3>     frvf;
    fs_row_vector<double, 3>    frvd;

    dyn_matrix<double>          dmd(3, 3);
    dyn_matrix<float>           dmf(3, 3);
    dyn_matrix<float>           dmf_cv(3, 1);
    dyn_matrix<float>           dmf_rv(1, 3);

    fs_matrix<double, 3, 3>     fmd;
    fs_matrix<float, 3, 3>      fmf;
    fs_matrix<float, 3, 1>      fmf_cv;
    fs_matrix<float, 1, 3>      fmf_rv;

    auto    r01 = dmf * dcvf;       //- dyn_col_vector<float>
    auto    r02 = dmf_cv * drvf;    //- dyn_matrix<float>
    auto    r03 = drvf * dmf;       //- dyn_row_vector<float>
    auto    r04 = dcvf * fmf_rv;    //- dyn_matrix<float>

    auto    r11 = dmf * dcvd;       //- dyn_col_vector<double>
    auto    r12 = dmf_cv * drvd;    //- dyn_matrix<double>
    auto    r13 = drvf * dmd;       //- dyn_row_vector<double>
    auto    r14 = dcvd * dmf_rv;    //- dyn_matrix<double>

    auto    r21 = fmf * fcvf;       //- fs_col_vector<float, 3>
    auto    r22 = fmf_cv * frvf;    //- fs_matrix<float, 3, 3>
    auto    r23 = frvf * fmf;       //- fs_row_vector<float, 3>
    auto    r24 = fcvf * fmf_rv;    //- fs_matrix<float, 3, 3>

    auto    r31 = fmf * fcvd;       //- fs_col_vector<double, 3>
    auto    r32 = fmf_cv * frvd;    //- fs_matrix<double, 3, 3>
    auto    r33 = frvf * fmd;       //- fs_row_vector<3>
    auto    r34 = fcvd * fmf_rv;    //- fs_matrix<double, 3, 3>

    auto    r41 = drvf * dcvf;      //- float
    auto    r42 = frvf * dcvd;      //- double
    auto    r43 = frvd * fcvd;      //- double
}

void t20()
{
    dyn_col_vector<float>       dcvf(3, 3);
    dyn_col_vector<double>      dcvd(3, 3);

    fs_col_vector<float, 3>     fcvf;
    fs_col_vector<double, 3>    fcvd;

    dyn_row_vector<float>       drvf(3, 3);
    dyn_row_vector<double>      drvd(3, 3);

    fs_row_vector<float, 3>     frvf;
    fs_row_vector<double, 3>    frvd;

    dyn_matrix<float>           dmf(3, 3);
    dyn_matrix<double>          dmd(3, 3);

    fs_matrix<float, 3, 3>      fmf;
    fs_matrix<double, 3, 3>     fmd;

    auto    r01 = dcvf + dcvf;
    auto    r02 = dcvf + dcvd;
    auto    r03 = dcvd + dcvf;

    auto    r04 = fcvf + fcvf;
    auto    r05 = fcvf + fcvd;
    auto    r06 = fcvd + fcvf;

    auto    r07 = dcvf + fcvf;
    auto    r08 = dcvf + fcvd;
    auto    r09 = dcvd + fcvd;

    auto    r10 = fcvf + dcvf;
    auto    r11 = fcvf + dcvd;
    auto    r12 = fcvd + dcvd;

    auto    r21 = drvf + drvf;
    auto    r22 = drvf + drvd;
    auto    r23 = drvd + drvf;

    auto    r24 = frvf + frvf;
    auto    r25 = frvf + frvd;
    auto    r26 = frvd + frvf;

    auto    r27 = drvf + frvf;
    auto    r28 = drvf + frvd;
    auto    r29 = drvd + frvd;

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
    dyn_col_vector<float>       dcvf(3, 3);
    dyn_col_vector<double>      dcvd(3, 3);

    fs_col_vector<float, 3>     fcvf;
    fs_col_vector<double, 3>    fcvd;

    dyn_row_vector<float>       drvf(3, 3);
    dyn_row_vector<double>      drvd(3, 3);

    fs_row_vector<float, 3>     frvf;
    fs_row_vector<double, 3>    frvd;

    dyn_matrix<float>           dmf(3, 3);
    dyn_matrix<double>          dmd(3, 3);

    fs_matrix<float, 3, 3>      fmf;
    fs_matrix<double, 3, 3>     fmd;

    auto    r01 = dcvf - dcvf;
    auto    r02 = dcvf - dcvd;
    auto    r03 = dcvd - dcvf;

    auto    r04 = fcvf - fcvf;
    auto    r05 = fcvf - fcvd;
    auto    r06 = fcvd - fcvf;

    auto    r07 = dcvf - fcvf;
    auto    r08 = dcvf - fcvd;
    auto    r09 = dcvd - fcvd;

    auto    r10 = fcvf - dcvf;
    auto    r11 = fcvf - dcvd;
    auto    r12 = fcvd - dcvd;

    auto    r21 = drvf - drvf;
    auto    r22 = drvf - drvd;
    auto    r23 = drvd - drvf;

    auto    r24 = frvf - frvf;
    auto    r25 = frvf - frvd;
    auto    r26 = frvd - frvf;

    auto    r27 = drvf - frvf;
    auto    r28 = drvf - frvd;
    auto    r29 = drvd - frvd;

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
    dyn_col_vector<float>       dcvf(3, 3);
    fs_col_vector<float, 3>     fcvf;
    dyn_row_vector<float>       drvf(3, 3);
    fs_row_vector<float, 3>     frvf;
    dyn_matrix<float>           dmf(3, 3);
    fs_matrix<float, 3, 3>      fmf;

    auto    r01 = -dcvf;
    auto    r02 = -fcvf;
    auto    r03 = -drvf;
    auto    r04 = -frvf;
    auto    r05 = -dmf;
    auto    r06 = -fmf;
}

void t30()
{
    float       f = 1.0f;
    double      d = 1.0;
    cx_double   c = {1.0, 0.0};

    dyn_row_vector<float>       drvf(3);
    dyn_row_vector<double>      drvd(3);
    dyn_row_vector<cx_double>   drvc(3);

    fs_row_vector<float, 3>     frvf;
    fs_row_vector<double, 3>    frvd;
    fs_row_vector<cx_double, 3> frvc;

    dyn_col_vector<float>       dcvf(3, 3);
    dyn_col_vector<double>      dcvd(3, 3);
    dyn_col_vector<cx_double>   dcvc(3, 3);

    fs_col_vector<float, 3>     fcvf;
    fs_col_vector<double, 3>    fcvd;
    fs_col_vector<cx_double, 3> fcvc;

    dyn_matrix<float>           dmf(3, 3);
    dyn_matrix<double>          dmd(3, 3);
    dyn_matrix<cx_double>       dmc(3, 3);

    fs_matrix<float, 3, 3>      fmf;
    fs_matrix<double, 3, 3>     fmd;
    fs_matrix<cx_double, 3, 3>  fmc;

    auto    r01 = -(dmf*(fmd*fcvd) + dcvf - fcvf)*c;
}

void t40()
{
    dyn_col_vector<float>       dcvf(3, 3);
    dyn_col_vector<double>      dcvd(3, 3);

    fs_col_vector<float, 3>     fcvf;
    fs_col_vector<double, 3>    fcvd;

    dyn_row_vector<float>       drvf(3, 3);
    dyn_row_vector<double>      drvd(3, 3);

    fs_row_vector<float, 3>     frvf;
    fs_row_vector<double, 3>    frvd;

    dyn_matrix<float>           dmf(3, 3);
    dyn_matrix<double>          dmd(3, 3);

    fs_matrix<float, 3, 3>      fmf;
    fs_matrix<double, 3, 3>     fmd;

    auto    r01 = dcvf.t() + drvf;
    auto    r02 = drvf + dcvd.t();
    auto    r03 = dcvd.t() + dcvf.t();

    auto    r04 = fcvf.t() + frvf;
    auto    r05 = frvf + fcvd.t();
    auto    r06 = fcvd.t() + fcvf.t();

    auto    r07 = dcvf.t() + frvf;
    auto    r08 = drvf + fcvd.t();
    auto    r09 = dcvd.t() + fcvd.t();

    auto    r10 = fcvf.t() + drvf;
    auto    r11 = frvf + dcvd.t();
    auto    r12 = fcvd.t() + dcvd.t();

    auto    r21 = drvf.t() + dcvf;
    auto    r22 = drvf + dcvd.t();
    auto    r23 = drvd.t() + drvf.t();

    auto    r24 = frvf.t() + fcvf;
    auto    r25 = frvf + fcvd.t();
    auto    r26 = frvd.t() + frvf.t();

    auto    r27 = drvf.t() + fcvf;
    auto    r28 = drvf + fcvd.t();
    auto    r29 = drvd.t() + frvd.t();

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

void t41()
{
    dyn_col_vector<float>       dcvf(3, 3);
    dyn_col_vector<double>      dcvd(3, 3);

    fs_col_vector<float, 3>     fcvf;
    fs_col_vector<double, 3>    fcvd;

    dyn_row_vector<float>       drvf(3, 3);
    dyn_row_vector<double>      drvd(3, 3);

    fs_row_vector<float, 3>     frvf;
    fs_row_vector<double, 3>    frvd;

    dyn_matrix<float>           dmf(3, 3);
    dyn_matrix<double>          dmd(3, 3);

    fs_matrix<float, 3, 3>      fmf;
    fs_matrix<double, 3, 3>     fmd;

    auto    r01 = dcvf.t() - drvf;
    auto    r02 = drvf - dcvd.t();
    auto    r03 = dcvd.t() - dcvf.t();

    auto    r04 = fcvf.t() - frvf;
    auto    r05 = frvf - fcvd.t();
    auto    r06 = fcvd.t() - fcvf.t();

    auto    r07 = dcvf.t() - frvf;
    auto    r08 = drvf - fcvd.t();
    auto    r09 = dcvd.t() - fcvd.t();

    auto    r10 = fcvf.t() - drvf;
    auto    r11 = frvf - dcvd.t();
    auto    r12 = fcvd.t() - dcvd.t();

    auto    r21 = drvf.t() - dcvf;
    auto    r22 = drvf - dcvd.t();
    auto    r23 = drvd.t() - drvf.t();

    auto    r24 = frvf.t() - fcvf;
    auto    r25 = frvf - fcvd.t();
    auto    r26 = frvd.t() - frvf.t();

    auto    r27 = drvf.t() - fcvf;
    auto    r28 = drvf - fcvd.t();
    auto    r29 = drvd.t() - frvd.t();

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

void t42()
{
    dyn_col_vector<float>       dcvf(3, 3);
    fs_col_vector<float, 3>     fcvf;
    dyn_row_vector<float>       drvf(3, 3);
    fs_row_vector<float, 3>     frvf;
    dyn_matrix<float>           dmf(3, 3);
    fs_matrix<float, 3, 3>      fmf;

    auto    r01 = -dcvf.t();
    auto    r02 = -fcvf.t();
    auto    r03 = -drvf.t();
    auto    r04 = -frvf.t();
    auto    r05 = -dmf.t();
    auto    r06 = -fmf.t();
}
