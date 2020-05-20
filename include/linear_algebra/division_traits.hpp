//==================================================================================================
//  File:       division_traits.hpp
//
//  Summary:    This header defines several private and public traits types that are used to
//              implement the binary scalar division operator.  This is a somewhat long file,
//              divided into two main sections, each with three sub-sections:
//                  1. Private traits definitions that:
//                      A. determine the element promotion traits to be used for scalar division
//                      B. determine the engine promotion traits to be used for scalar division
//                      C. determine the arithmetic traits to be used to perform scalar division
//                  2. Public traits definitions that:
//                      A. perform element promotion for scalar division
//                      B. perform engine promotion for scalar division
//                      C. perform the actual act of scalar division
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_DIVISION_TRAITS_HPP_DEFINED
#define LINEAR_ALGEBRA_DIVISION_TRAITS_HPP_DEFINED

namespace STD_LA {
namespace detail {
//==================================================================================================
//                         **** ELEMENT DIVISION TRAITS DETECTORS ****
//==================================================================================================
//
//- Form 0 type detection of nested element division traits.
//
template<typename OT>
using element_div_traits_f0_t = typename OT::element_division_traits;

template<typename OT>
using element_div_result_f0_t = typename element_div_traits_f0_t<OT>::element_type;

//- Define the form 0 detectors.
//
template<typename OT, typename = void>
struct detect_element_div_traits_f0
:   public false_type
{
    using traits_type = void;
};

template<typename OT>
struct detect_element_div_traits_f0<OT, void_t<element_div_result_f0_t<OT>>>
:   public true_type
{
    using traits_type = element_div_traits_f0_t<OT>;
};

//----------------------------------------------------------------
//- Form 2 type detection of nested element division traits.
//
template<typename OT, typename T1, typename T2>
using element_div_traits_f2_t = typename OT::template element_division_traits<T1, T2>;

template<typename OT, typename T1, typename T2>
using element_div_result_f2_t = typename element_div_traits_f2_t<OT, T1, T2>::element_type;

//- Define the form 2 detectors.
//
template<typename OT, typename T1, typename T2, typename = void>
struct detect_element_div_traits_f2
:   public false_type
{
    using traits_type = void;
};

template<typename OT, typename T1, typename T2>
struct detect_element_div_traits_f2<OT, T1, T2, void_t<element_div_result_f2_t<OT, T1, T2>>>
:   public true_type
{
    using traits_type = element_div_traits_f2_t<OT, T1, T2>;
};

//- Used only for testing.
//
template<class OT, class T1, class T2>
constexpr bool  has_element_div_traits_v = detect_element_div_traits_f2<OT, T1, T2>::value ||
                                           detect_element_div_traits_f0<OT>::value;

//---------------------------------------------------
//- Element division traits type determination.
//
template<typename OT, typename T1, typename T2>
struct element_div_traits_chooser
{
    using CT1 = typename detect_element_div_traits_f0<OT>::traits_type;
    using CT2 = typename detect_element_div_traits_f2<OT, T1, T2>::traits_type;
    using DEF = matrix_division_element_traits<T1, T2>;

    using traits_type = typename non_void_traits_chooser<CT1, CT2, DEF>::traits_type;
    using element_type = typename traits_type::element_type;
};

template<typename OT, typename T1, typename T2>
using element_div_result_t = typename element_div_traits_chooser<OT, T1, T2>::element_type;


//==================================================================================================
//                         **** ENGINE DIVISION TRAITS DETECTORS ****
//==================================================================================================
//
//- Form 0 type detection of nested engine division traits.
//
template<typename OT>
using engine_div_traits_f0_t = typename OT::engine_division_traits;

template<typename OT>
using engine_div_result_f0_t = typename engine_div_traits_f0_t<OT>::engine_type;

//- Define the form 0 detectors.
//
template<typename OT, typename = void>
struct detect_engine_div_traits_f0
:   public false_type
{
    using traits_type = void;
};

template<typename OT>
struct detect_engine_div_traits_f0<OT, std::void_t<engine_div_result_f0_t<OT>>>
:   public true_type
{
    using traits_type = engine_div_traits_f0_t<OT>;
};

//---------------------------------------------------------------
//- Form 2 type detection of nested engine division traits.
//
template<typename OT, typename T1, typename T2>
using engine_div_traits_f2_t = typename OT::template engine_division_traits<OT, T1, T2>;

template<typename OT, typename T1, typename T2>
using engine_div_result_f2_t = typename engine_div_traits_f2_t<OT, T1, T2>::engine_type;

//- Define the form 2 detectors.
//
template<typename OT, typename ET1, typename ET2, typename = void>
struct detect_engine_div_traits_f2
:   public false_type
{
    using traits_type = void;
};

template<typename OT, typename ET1, typename ET2>
struct detect_engine_div_traits_f2<OT, ET1, ET2, std::void_t<engine_div_result_f2_t<OT, ET1, ET2>>>
:   public true_type
{
    using traits_type = engine_div_traits_f2_t<OT, ET1, ET2>;
};

//- Used only for testing.
//
template<class OT, class ET1, class ET2>
constexpr bool  has_engine_div_traits_v = detect_engine_div_traits_f2<OT, ET1, ET2>::value ||
                                          detect_engine_div_traits_f0<OT>::value;

//--------------------------------------------------
//- Engine division traits type determination.
//
template<typename OT, typename ET1, typename ET2>
struct engine_div_traits_chooser
{
    using CT1 = typename detect_engine_div_traits_f0<OT>::traits_type;
    using CT2 = typename detect_engine_div_traits_f2<OT, ET1, ET2>::traits_type;
    using DEF = matrix_division_engine_traits<OT, ET1, ET2>;

    using traits_type = typename non_void_traits_chooser<CT1, CT2, DEF>::traits_type;
    using engine_type = typename traits_type::engine_type;
};

template<typename OT, typename ET1, typename ET2>
using engine_div_result_t = typename engine_div_traits_chooser<OT, ET1, ET2>::engine_type;


//==================================================================================================
//                        **** DIVISION ARITHMETIC TRAITS DETECTORS ****
//==================================================================================================
//
//- Form 0 type detection of nested division arithmetic traits.
//
template<typename OT>
using div_traits_f0_t = typename OT::division_traits;

template<typename OT>
using div_result_f0_t = typename div_traits_f0_t<OT>::result_type;

//- Define the form 0 detectors.
//
template<typename OT, typename = void>
struct detect_div_traits_f0
:   public false_type
{
    using traits_type = void;
};

template<typename OT>
struct detect_div_traits_f0<OT, void_t<div_result_f0_t<OT>>>
:   public true_type
{
    using traits_type = div_traits_f0_t<OT>;
};

//-------------------------------------------------------------------
//- Form 2 type detection of nested division arithmetic traits.
//
template<typename OT, typename T1, typename T2>
using div_traits_f2_t = typename OT::template division_traits<OT, T1, T2>;

template<typename OT, typename T1, typename T2>
using div_result_f2_t = typename div_traits_f2_t<OT, T1, T2>::result_type;


//- Define the form 2 detectors.
//
template<typename OT, typename OP1, typename OP2, typename = void>
struct detect_div_traits_f2
:   public false_type
{
    using traits_type = void;
};

template<typename OT, typename OP1, typename OP2>
struct detect_div_traits_f2<OT, OP1, OP2, void_t<div_result_f2_t<OT, OP1, OP2>>>
:   public true_type
{
    using traits_type = typename OT::template division_traits<OT, OP1, OP2>;
};

//- Used only for testing.
//
template<class OT, class OP1, class OP2>
constexpr bool  has_div_traits_v = detect_div_traits_f2<OT, OP1, OP2>::value ||
                                   detect_div_traits_f0<OT>::value;

//------------------------------------------------------
//- Multiplication arithmetic traits type determination.
//
template<typename OT, typename OP1, typename OP2>
struct div_traits_chooser
{
    using CT1 = typename detect_div_traits_f0<OT>::traits_type;
    using CT2 = typename detect_div_traits_f2<OT, OP1, OP2>::traits_type;
    using DEF = matrix_division_traits<OT, OP1, OP2>;

    using traits_type = typename non_void_traits_chooser<CT1, CT2, DEF>::traits_type;
};

template<typename OT, typename OP1, typename OP2>
using div_traits_result_t = typename div_traits_chooser<OT, OP1, OP2>::traits_type;


}       //- detail namespace
//==================================================================================================
//                           **** ELEMENT DIVISION TRAITS ****
//==================================================================================================
//
//- Alias interface to detection meta-function that extracts the element division traits type.
//
template<class OT, class T1, class T2>
using select_matrix_division_element_t = detail::element_div_result_t<OT, T1, T2>;


//- The standard element division traits type provides the default mechanism for determining
//  the result of multiplying two elements of (possibly) different types.
//
template<class T1, class T2>
struct matrix_division_element_traits
{
    using element_type = decltype(declval<T1>() / declval<T2>());
};


//==================================================================================================
//                            **** ENGINE DIVISION TRAITS ****
//==================================================================================================
//
//- Alias interface to detection meta-function that extracts the engine division traits type.
//
template<class OT, class ET1, class ET2>
using select_matrix_division_engine_t = detail::engine_div_result_t<OT, ET1, ET2>;

//- The standard engine division traits type provides the default mechanism for determining
//  the correct engine type for a matrix/matrix, matrix/vector, matrix/scalar, vector/vector, or
//  vector/scalar division.
//
template<class OT, class ET1, class ET2>
struct matrix_division_engine_traits
{
    static_assert((is_matrix_engine_v<ET1> || is_vector_engine_v<ET1>) && is_scalar_engine_v<ET2>);

    static constexpr bool
    use_matrix_engine = (is_matrix_engine_v<ET1> && is_scalar_engine_v<ET2>);

    using element_type_1 = typename ET1::element_type;
    using element_type_2 = typename ET2::element_type;
    using element_type   = select_matrix_division_element_t<OT, element_type_1, element_type_2>;
    using engine_type    = conditional_t<use_matrix_engine,
                                         dr_matrix_engine<element_type, allocator<element_type>>,
                                         dr_vector_engine<element_type, allocator<element_type>>>;
};

//- General transpose cases for matrices.
//
template<class OT, class ET1, class MCT1, class ET2>
struct matrix_division_engine_traits<OT, transpose_engine<ET1, MCT1>, ET2>
{
    using engine_type = typename matrix_division_engine_traits<OT, ET1, ET2>::engine_type;
};

//--------------------------------------------------------------------------------------------------
//- ENGINE / SCALAR cases for vectors.
//
template<class OT, class T1, class A1, class T2>
struct matrix_division_engine_traits<OT, dr_vector_engine<T1, A1>, scalar_engine<T2>>
{
    using element_type = select_matrix_division_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<A1, element_type>;
    using engine_type  = dr_vector_engine<element_type, alloc_type>;
};

template<class OT, class T1, ptrdiff_t N1, class T2>
struct matrix_division_engine_traits<OT, fs_vector_engine<T1, N1>, scalar_engine<T2>>
{
    using element_type = select_matrix_division_element_t<OT, T1, T2>;
    using engine_type  = fs_vector_engine<element_type, N1>;
};

//--------------------------------------------------------------------------------------------------
//- ENGINE / SCALAR cases for matrices.
//
//- dr_matrix_engine / scalar.
//
template<class OT, class T1, class A1, class T2>
struct matrix_division_engine_traits<OT,
                                     dr_matrix_engine<T1, A1>,
                                     scalar_engine<T2>>
{
    using element_type = select_matrix_division_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<A1, element_type>;
    using engine_type  = dr_matrix_engine<element_type, alloc_type>;
};

template<class OT, class T1, class A1, class MCT1, class T2>
struct matrix_division_engine_traits<OT,
                                     transpose_engine<dr_matrix_engine<T1, A1>, MCT1>,
                                     scalar_engine<T2>>
{
    using element_type = select_matrix_division_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<A1, element_type>;
    using engine_type  = dr_matrix_engine<element_type, alloc_type>;
};

//----------------------------
//- fs_matrix_engine / scalar.
//
template<class OT, class T1, ptrdiff_t R1, ptrdiff_t C1, class T2>
struct matrix_division_engine_traits<OT,
                                     fs_matrix_engine<T1, R1, C1>,
                                     scalar_engine<T2>>
{
    using element_type = select_matrix_division_element_t<OT, T1, T2>;
    using engine_type  = fs_matrix_engine<element_type, R1, C1>;
};

template<class OT, class T1, ptrdiff_t R1, ptrdiff_t C1, class MCT1, class T2>
struct matrix_division_engine_traits<OT,
                                     transpose_engine<fs_matrix_engine<T1, R1, C1>, MCT1>,
                                     scalar_engine<T2>>
{
    using element_type = select_matrix_division_element_t<OT, T1, T2>;
    using engine_type  = fs_matrix_engine<element_type, C1, R1>;
};


//==================================================================================================
//                                  **** DIVISION TRAITS ****
//==================================================================================================
//
//- Alias interface to detection meta-function that extracts the division traits type.
//
template<class OT, class OP1, class OP2>
using select_matrix_division_traits_t = detail::div_traits_result_t<OT, OP1, OP2>;


//- The standard division traits type provides the default mechanism for computing the
//  correct result type of a division.
//
//-------------------
//- (vector / scalar)
//
template<class OT, class ET1, class OT1, class T2>
struct matrix_division_traits<OT, vector<ET1, OT1>, T2>
{
    using engine_type = select_matrix_division_engine_t<OT, ET1, scalar_engine<T2>>;
    using result_type = vector<engine_type, OT>;

    static constexpr result_type    divide(vector<ET1, OT1> const& v1, T2 const& s2);
};

template<class OTR, class ET1, class OT1, class T2> inline constexpr
auto
matrix_division_traits<OTR, vector<ET1, OT1>, T2>::divide
(vector<ET1, OT1> const& v1, T2 const& s2) -> result_type
{
    using index_type_1 = typename vector<ET1, OT1>::index_type;
    using index_type_r = typename result_type::index_type;

    index_type_r    elems = static_cast<index_type_r>(v1.size());
    result_type     vr;

    if constexpr (is_resizable_engine_v<engine_type>)
    {
        vr.resize(elems);
    }

    index_type_r    ir = 0;
    index_type_1    i1 = 0;

    for (;  ir < elems;  ++ir, ++i1)
    {
        vr(ir) = v1(i1) / s2;
    }

    return vr;
}


//-------------------
//- (matrix / scalar)
//
template<class OT, class ET1, class OT1, class T2>
struct matrix_division_traits<OT, matrix<ET1, OT1>, T2>
{
    using engine_type = select_matrix_division_engine_t<OT, ET1, scalar_engine<T2>>;
    using result_type = matrix<engine_type, OT>;

    static constexpr result_type    divide(matrix<ET1, OT1> const& m1, T2 const& s2);
};

template<class OTR, class ET1, class OT1, class T2> inline constexpr
auto
matrix_division_traits<OTR, matrix<ET1, OT1>, T2>::divide
(matrix<ET1, OT1> const& m1, T2 const& s2) -> result_type
{
    using index_type_1 = typename matrix<ET1, OT1>::index_type;
    using index_type_r = typename result_type::index_type;

    index_type_r    rows = static_cast<index_type_r>(m1.rows());
    index_type_r    cols = static_cast<index_type_r>(m1.columns());
    result_type		mr;

    if constexpr (is_resizable_engine_v<engine_type>)
    {
        mr.resize(rows, cols);
    }

    index_type_r    ir = 0;
    index_type_1    i1 = 0;

    for (;  ir < rows;  ++ir, ++i1)
    {
        index_type_r    jr = 0;
        index_type_1    j1 = 0;

        for (;  jr < cols;  ++jr, ++j1)
        {
            mr(ir, jr) = m1(i1, j1) / s2;
        }
    }

    return mr;
}

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_DIVISION_TRAITS_HPP_DEFINED
