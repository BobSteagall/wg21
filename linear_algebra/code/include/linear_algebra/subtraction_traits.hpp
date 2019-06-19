//==================================================================================================
//  File:       subtraction_traits.hpp
//
//  Summary:    This header defines several private and public traits types that are used to
//              implement the binary subtraction operator.  This is a somewhat long file, divided
//              into two main sections, each with three sub-sections:
//                  1. Private traits definitions that:
//                      A. determine the element promotion traits to be used for subtraction
//                      B. determine the engine promotion traits to be used for subtraction
//                      C. determine the arithmetic traits to be used to perform subtraction
//                  2. Public traits definitions that:
//                      A. perform element promotion for subtraction
//                      B. perform engine promotion for subtraction
//                      C. perform the actual act of subtraction
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_SUBTRACTION_TRAITS_HPP_DEFINED
#define LINEAR_ALGEBRA_SUBTRACTION_TRAITS_HPP_DEFINED

namespace STD_LA {
namespace detail {
//==================================================================================================
//                          **** ELEMENT SUBTRACTION TRAITS DETECTORS ****
//==================================================================================================
//
//- Form 1 type detection of nested element subtraction traits.
//
template<typename OT>
using element_sub_traits_f1_t = typename OT::element_subtraction_traits;

template<typename OT>
using element_sub_type_f1_t = typename element_sub_traits_f1_t<OT>::element_type;

//- Define the form 1 detectors.
//
template<typename OT, typename = void>
struct detect_element_sub_traits_f1
:   public false_type
{
    using traits_type = void;
};

template<typename OT>
struct detect_element_sub_traits_f1<OT, void_t<element_sub_type_f1_t<OT>>>
:   public true_type
{
    using traits_type = element_sub_traits_f1_t<OT>;
};

//-------------------------------------------------------------
//- Form 2 type detection of nested element subtraction traits.
//
template<typename OT, typename T1, typename T2>
using element_sub_traits_f2_t = typename OT::template element_subtraction_traits<T1, T2>;

template<typename OT, typename T1, typename T2>
using element_sub_type_f2_t = typename element_sub_traits_f2_t<OT, T1, T2>::element_type;

//- Define the form 2 detectors.
//
template<typename OT, typename T1, typename T2, typename = void>
struct detect_element_sub_traits_f2
:   public false_type
{
    using traits_type = void;
};

template<typename OT, typename T1, typename T2>
struct detect_element_sub_traits_f2<OT, T1, T2, void_t<element_sub_type_f2_t<OT, T1, T2>>>
:   public true_type
{
    using traits_type = element_sub_traits_f2_t<OT, T1, T2>;
};

//------------------------------------------------
//- Element subtraction traits type determination.
//
template<typename OT, typename T1, typename T2>
struct element_sub_traits_chooser
{
    using CT1 = typename detect_element_sub_traits_f1<OT>::traits_type;
    using CT2 = typename detect_element_sub_traits_f2<OT, T1, T2>::traits_type;
    using DEF = matrix_subtraction_element_traits<T1, T2>;

    using traits_type = typename non_void_traits_chooser<CT1, CT2, DEF>::traits_type;
};

template<typename OT, typename T1, typename T2>
using element_sub_traits_t = typename element_sub_traits_chooser<OT, T1, T2>::traits_type;

template<class OT, class T1, class T2>
constexpr bool  has_element_sub_traits_v = detect_element_sub_traits_f2<OT, T1, T2>::value ||
                                           detect_element_sub_traits_f1<OT>::value;

//------------------------------------------------
//- Element subtraction result type determination.
//
template<typename OT, typename T1, typename T2>
struct element_sub_type
{
    using traits_type  = typename element_sub_traits_chooser<OT, T1, T2>::traits_type;
    using element_type = typename traits_type::element_type;
};

template<typename OT, typename T1, typename T2>
using element_sub_type_t = typename element_sub_type<OT, T1, T2>::element_type;


//==================================================================================================
//                           **** ENGINE SUBTRACTION TRAITS DETECTORS ****
//==================================================================================================
//
//- Form 1 type detection of nested engine subtraction traits.
//
template<typename OT>
using engine_sub_traits_f1_t = typename OT::engine_subtraction_traits;

template<typename OT>
using engine_sub_type_f1_t = typename engine_sub_traits_f1_t<OT>::engine_type;

//- Define the form 1 detectors.
//
template<typename OT, typename = void>
struct detect_engine_sub_traits_f1
:   public false_type
{
    using traits_type = void;
};

template<typename OT>
struct detect_engine_sub_traits_f1<OT, void_t<engine_sub_type_f1_t<OT>>>
:   public true_type
{
    using traits_type = engine_sub_traits_f1_t<OT>;
};

//------------------------------------------------------------
//- Form 2 type detection of nested engine subtraction traits.
//
template<typename OT, typename T1, typename T2>
using engine_sub_traits_f2_t = typename OT::template engine_subtraction_traits<OT, T1, T2>;

template<typename OT, typename T1, typename T2>
using engine_sub_type_f2_t = typename engine_sub_traits_f2_t<OT, T1, T2>::engine_type;

//- Define the form 2 detectors.
//
template<typename OT, typename ET1, typename ET2, typename = void>
struct detect_engine_sub_traits_f2
:   public false_type
{
    using traits_type = void;
};

template<typename OT, typename ET1, typename ET2>
struct detect_engine_sub_traits_f2<OT, ET1, ET2, void_t<engine_sub_type_f2_t<OT, ET1, ET2>>>
:   public true_type
{
    using traits_type = engine_sub_traits_f2_t<OT, ET1, ET2>;
};

//-----------------------------------------------
//- Engine subtraction traits type determination.
//
template<typename OT, typename ET1, typename ET2>
struct engine_sub_traits_chooser
{
    using CT1 = typename detect_engine_sub_traits_f1<OT>::traits_type;
    using CT2 = typename detect_engine_sub_traits_f2<OT, ET1, ET2>::traits_type;
    using DEF = matrix_subtraction_engine_traits<OT, ET1, ET2>;

    using traits_type = typename non_void_traits_chooser<CT1, CT2, DEF>::traits_type;
};

template<typename OT, typename ET1, typename ET2>
using engine_sub_traits_t = typename engine_sub_traits_chooser<OT, ET1, ET2>::traits_type;

template<class OT, class ET1, class ET2>
constexpr bool  has_engine_sub_traits_v = detect_engine_sub_traits_f2<OT, ET1, ET2>::value ||
                                          detect_engine_sub_traits_f1<OT>::value;

//-----------------------------------------------
//- Engine subtraction result type determination.
//
template<typename OT, typename ET1, typename ET2>
struct engine_sub_type
{
    using traits_type = typename engine_sub_traits_chooser<OT, ET1, ET2>::traits_type;
    using engine_type = typename traits_type::engine_type;
};

template<typename OT, typename ET1, typename ET2>
using engine_sub_type_t = typename engine_sub_type<OT, ET1, ET2>::engine_type;


//==================================================================================================
//                        **** SUBTRACTION ARITHMETIC TRAITS DETECTORS ****
//==================================================================================================
//
//- Form 1 type detection of nested subtraction arithmetic traits.
//
template<typename OT>
using sub_traits_f1_t = typename OT::subtraction_traits;

template<typename OT>
using sub_type_f1_t = typename sub_traits_f1_t<OT>::result_type;

//- Define the form 1 detectors.
//
template<typename OT, typename = void>
struct detect_sub_traits_f1
:   public false_type
{
    using traits_type = void;
};

template<typename OT>
struct detect_sub_traits_f1<OT, void_t<sub_type_f1_t<OT>>>
:   public true_type
{
    using traits_type = sub_traits_f1_t<OT>;
};

//----------------------------------------------------------------
//- Form 2 type detection of nested subtraction arithmetic traits.
//
template<typename OT, typename T1, typename T2>
using sub_traits_f2_t = typename OT::template subtraction_traits<OT, T1, T2>;

template<typename OT, typename T1, typename T2>
using sub_type_f2_t = typename sub_traits_f2_t<OT, T1, T2>::result_type;

//- Define the form 2 detectors.
//
template<typename OT, typename OP1, typename OP2, typename = void>
struct detect_sub_traits_f2
:   public false_type
{
    using traits_type = void;
};

template<typename OT, typename OP1, typename OP2>
struct detect_sub_traits_f2<OT, OP1, OP2, void_t<sub_type_f2_t<OT, OP1, OP2>>>
:   public true_type
{
    using traits_type = typename OT::template subtraction_traits<OT, OP1, OP2>;
};

//---------------------------------------------------
//- Subtraction arithmetic traits type determination.
//
template<typename OT, typename OP1, typename OP2>
struct sub_traits_chooser
{
    using CT1 = typename detect_sub_traits_f1<OT>::traits_type;
    using CT2 = typename detect_sub_traits_f2<OT, OP1, OP2>::traits_type;
    using DEF = matrix_subtraction_traits<OT, OP1, OP2>;

    using traits_type = typename non_void_traits_chooser<CT1, CT2, DEF>::traits_type;
};

template<typename OT, typename OP1, typename OP2>
using subtraction_traits_t = typename sub_traits_chooser<OT, OP1, OP2>::traits_type;

template<class OT, class OP1, class OP2>
constexpr bool  has_sub_traits_v = detect_sub_traits_f2<OT, OP1, OP2>::value ||
                                   detect_sub_traits_f1<OT>::value;


}       //- detail namespace
//==================================================================================================
//                              **** ELEMENT SUBTRACTION TRAITS ****
//==================================================================================================
//
//- Alias interface to detection meta-function that extracts the element subtraction traits type.
//
template<class OT, class T1, class T2>
using matrix_subtraction_element_t = detail::element_sub_type_t<OT, T1, T2>;


//- The standard element subtraction traits type provides the default mechanism for determining the
//  result of subtracting two elements of (possibly) different types.
//
template<class T1, class T2>
struct matrix_subtraction_element_traits
{
    using element_type = decltype(declval<T1>() - declval<T2>());
};


//==================================================================================================
//                                **** ENGINE SUBTRACTION TRAITS ****
//==================================================================================================
//
//- Alias interface to detection meta-function that extracts the engine subtraction traits type.
//
template<class OT, class ET1, class ET2>
using matrix_subtraction_engine_t = detail::engine_sub_type_t<OT, ET1, ET2>;


//- The standard engine subtraction traits type provides the default mechanism for determining the
//  correct engine type for a matrix/matrix or vector/vector subtraction.
//
template<class OT, class ET1, class ET2>
struct matrix_subtraction_engine_traits
{
    static_assert(detail::engines_match_v<ET1, ET2>);

    using element_type_1 = typename ET1::element_type;
    using element_type_2 = typename ET2::element_type;
    using element_type   = matrix_subtraction_element_t<OT, element_type_1, element_type_2>;
    using engine_type    = conditional_t<detail::is_matrix_engine_v<ET1>,
                                         dr_matrix_engine<element_type, allocator<element_type>>,
                                         dr_vector_engine<element_type, allocator<element_type>>>;
};

//- General transpose cases for matrices.
//
template<class OT, class ET1, class ET2>
struct matrix_subtraction_engine_traits<OT, matrix_transpose_view<ET1>, ET2>
{
    using engine_type = typename matrix_subtraction_engine_traits<OT, ET1, ET2>::engine_type;
};

template<class OT, class ET1, class ET2>
struct matrix_subtraction_engine_traits<OT, ET1, matrix_transpose_view<ET2>>
{
    using engine_type = typename matrix_subtraction_engine_traits<OT, ET1, ET2>::engine_type;
};

template<class OT, class ET1, class ET2>
struct matrix_subtraction_engine_traits<OT, matrix_transpose_view<ET1>, matrix_transpose_view<ET2>>
{
    using engine_type = typename matrix_subtraction_engine_traits<OT, ET1, ET2>::engine_type;
};

//--------------------------------------------------------------------------------------------------
//- ENGINE - ENGINE cases for vector/vector.  Note that all partial specializations below in which
//  allocators are rebound assume standard-conformant allocator types.
//
//- dr_vector_engine - dr_vector_engine.
//
template<class OT, class T1, class A1, class T2, class A2>
struct matrix_subtraction_engine_traits<OT, dr_vector_engine<T1, A1>, dr_vector_engine<T2, A2>>
{
    using element_type = matrix_subtraction_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<A1, element_type>;
    using engine_type  = dr_vector_engine<element_type, alloc_type>;
};

//--------------------------------------
//- dr_vector_engine - fs_vector_engine.
//
template<class OT, class T1, class A1, class T2, int32_t N2>
struct matrix_subtraction_engine_traits<OT, dr_vector_engine<T1, A1>, fs_vector_engine<T2, N2>>
{
    using element_type = matrix_subtraction_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<A1, element_type>;
    using engine_type  = dr_vector_engine<element_type, alloc_type>;
};

//--------------------------------------
//- fs_vector_engine - dr_vector_engine.
//
template<class OT, class T1, int32_t N1, class T2, class A2>
struct matrix_subtraction_engine_traits<OT, fs_vector_engine<T1, N1>, dr_vector_engine<T2, A2>>
{
    using element_type = matrix_subtraction_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<A2, element_type>;
    using engine_type  = dr_vector_engine<element_type, alloc_type>;
};

//--------------------------------------
//- fs_vector_engine - fs_vector_engine.
//
template<class OT, class T1, int32_t N1, class T2, int32_t N2>
struct matrix_subtraction_engine_traits<OT, fs_vector_engine<T1, N1>, fs_vector_engine<T2, N2>>
{
    static_assert(N1 == N2);
    using element_type = matrix_subtraction_element_t<OT, T1, T2>;
    using engine_type  = fs_vector_engine<element_type, N1>;
};

//--------------------------------------------------------------------------------------------------
//- ENGINE - ENGINE cases for matrix/matrix.  Note that all partial specializations below in which
//  allocators are rebound assume standard-conformant allocator types.
//
//- dr_matrix_engine - dr_matrix_engine.
//
template<class OT, class T1, class A1, class T2, class A2>
struct matrix_subtraction_engine_traits<OT,
                                        dr_matrix_engine<T1, A1>,
                                        dr_matrix_engine<T2, A2>>
{
    using element_type = matrix_subtraction_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<A1, element_type>;
    using engine_type  = dr_matrix_engine<element_type, alloc_type>;
};

template<class OT, class T1, class A1, class T2, class A2>
struct matrix_subtraction_engine_traits<OT,
                                        dr_matrix_engine<T1, A1>,
                                        matrix_transpose_view<dr_matrix_engine<T2, A2>>>
{
    using element_type = matrix_subtraction_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<A1, element_type>;
    using engine_type  = dr_matrix_engine<element_type, alloc_type>;
};

template<class OT, class T1, class A1, class T2, class A2>
struct matrix_subtraction_engine_traits<OT,
                                        matrix_transpose_view<dr_matrix_engine<T1, A1>>,
                                        dr_matrix_engine<T2, A2>>
{
    using element_type = matrix_subtraction_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<A1, element_type>;
    using engine_type  = dr_matrix_engine<element_type, alloc_type>;
};

template<class OT, class T1, class A1, class T2, class A2>
struct matrix_subtraction_engine_traits<OT,
                                        matrix_transpose_view<dr_matrix_engine<T1, A1>>,
                                        matrix_transpose_view<dr_matrix_engine<T2, A2>>>
{
    using element_type = matrix_subtraction_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<A1, element_type>;
    using engine_type  = dr_matrix_engine<element_type, alloc_type>;
};

//--------------------------------------
//- dr_matrix_engine - fs_matrix_engine.
//
template<class OT, class T1, class A1, class T2, int32_t R2, int32_t C2>
struct matrix_subtraction_engine_traits<OT,
                                        dr_matrix_engine<T1, A1>,
                                        fs_matrix_engine<T2, R2, C2>>
{
    using element_type = matrix_subtraction_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<A1, element_type>;
    using engine_type  = dr_matrix_engine<element_type, alloc_type>;
};

template<class OT, class T1, class A1, class T2, int32_t R2, int32_t C2>
struct matrix_subtraction_engine_traits<OT,
                                        dr_matrix_engine<T1, A1>,
                                        matrix_transpose_view<fs_matrix_engine<T2, R2, C2>>>
{
    using element_type = matrix_subtraction_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<A1, element_type>;
    using engine_type  = dr_matrix_engine<element_type, alloc_type>;
};

template<class OT, class T1, class A1, class T2, int32_t R2, int32_t C2>
struct matrix_subtraction_engine_traits<OT,
                                        matrix_transpose_view<dr_matrix_engine<T1, A1>>,
                                        fs_matrix_engine<T2, R2, C2>>
{
    using element_type = matrix_subtraction_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<A1, element_type>;
    using engine_type  = dr_matrix_engine<element_type, alloc_type>;
};

template<class OT, class T1, class A1, class T2, int32_t R2, int32_t C2>
struct matrix_subtraction_engine_traits<OT,
                                        matrix_transpose_view<dr_matrix_engine<T1, A1>>,
                                        matrix_transpose_view<fs_matrix_engine<T2, R2, C2>>>
{
    using element_type = matrix_subtraction_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<A1, element_type>;
    using engine_type  = dr_matrix_engine<element_type, alloc_type>;
};

//--------------------------------------
//- fs_matrix_engine - dr_matrix_engine.
//
template<class OT, class T1, int32_t R1, int32_t C1, class T2, class A2>
struct matrix_subtraction_engine_traits<OT,
                                        fs_matrix_engine<T1, R1, C1>,
                                        dr_matrix_engine<T2, A2>>
{
    using element_type = matrix_subtraction_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<A2, element_type>;
    using engine_type  = dr_matrix_engine<element_type, alloc_type>;
};

template<class OT, class T1, int32_t R1, int32_t C1, class T2, class A2>
struct matrix_subtraction_engine_traits<OT,
                                        fs_matrix_engine<T1, R1, C1>,
                                        matrix_transpose_view<dr_matrix_engine<T2, A2>>>
{
    using element_type = matrix_subtraction_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<A2, element_type>;
    using engine_type  = dr_matrix_engine<element_type, alloc_type>;
};

template<class OT, class T1, int32_t R1, int32_t C1, class T2, class A2>
struct matrix_subtraction_engine_traits<OT,
                                        matrix_transpose_view<fs_matrix_engine<T1, R1, C1>>,
                                        dr_matrix_engine<T2, A2>>
{
    using element_type = matrix_subtraction_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<A2, element_type>;
    using engine_type  = dr_matrix_engine<element_type, alloc_type>;
};

template<class OT, class T1, int32_t R1, int32_t C1, class T2, class A2>
struct matrix_subtraction_engine_traits<OT,
                                        matrix_transpose_view<fs_matrix_engine<T1, R1, C1>>,
                                        matrix_transpose_view<dr_matrix_engine<T2, A2>>>
{
    using element_type = matrix_subtraction_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<A2, element_type>;
    using engine_type  = dr_matrix_engine<element_type, alloc_type>;
};

//--------------------------------------
//- fs_matrix_engine - fs_matrix_engine.
//
template<class OT, class T1, int32_t R1, int32_t C1, class T2, int32_t R2, int32_t C2>
struct matrix_subtraction_engine_traits<OT,
                                        fs_matrix_engine<T1, R1, C1>,
                                        fs_matrix_engine<T2, R2, C2>>
{
    static_assert(R1 == R2);
    static_assert(C1 == C2);
    using element_type = matrix_subtraction_element_t<OT, T1, T2>;
    using engine_type  = fs_matrix_engine<element_type, R1, C1>;
};

template<class OT, class T1, int32_t R1, int32_t C1, class T2, int32_t R2, int32_t C2>
struct matrix_subtraction_engine_traits<OT,
                                        matrix_transpose_view<fs_matrix_engine<T1, R1, C1>>,
                                        fs_matrix_engine<T2, R2, C2>>
{
    static_assert(R1 == C2);
    static_assert(C1 == R2);
    using element_type = matrix_subtraction_element_t<OT, T1, T2>;
    using engine_type  = fs_matrix_engine<element_type, R2, C2>;
};

template<class OT, class T1, int32_t R1, int32_t C1, class T2, int32_t R2, int32_t C2>
struct matrix_subtraction_engine_traits<OT,
                                        fs_matrix_engine<T1, R1, C1>,
                                        matrix_transpose_view<fs_matrix_engine<T2, R2, C2>>>
{
    static_assert(R1 == C2);
    static_assert(C1 == R2);
    using element_type = matrix_subtraction_element_t<OT, T1, T2>;
    using engine_type  = fs_matrix_engine<element_type, R1, C1>;
};

template<class OT, class T1, int32_t R1, int32_t C1, class T2, int32_t R2, int32_t C2>
struct matrix_subtraction_engine_traits<OT,
                                        matrix_transpose_view<fs_matrix_engine<T1, R1, C1>>,
                                        matrix_transpose_view<fs_matrix_engine<T2, R2, C2>>>
{
    static_assert(R1 == R2);
    static_assert(C1 == C2);
    using element_type = matrix_subtraction_element_t<OT, T1, T2>;
    using engine_type  = fs_matrix_engine<element_type, C1, R1>;
};


//==================================================================================================
//                                   **** SUBTRACTION TRAITS ****
//==================================================================================================
//
//- Alias interface to detection meta-function that extracts the subtraction traits type.
//
template<class OT, class OP1, class OP2>
using matrix_subtraction_traits_t = detail::subtraction_traits_t<OT, OP1, OP2>;


//- The standard subtraction traits type provides the default mechanism for computing the result
//  of a matrix/matrix or vector/vector subtraction.
//
template<class OT, class ET1, class OT1, class ET2, class OT2>
struct matrix_subtraction_traits<OT, vector<ET1, OT1>, vector<ET2, OT2>>
{
    using engine_type = matrix_subtraction_engine_t<OT, ET1, ET2>;
    using op_traits   = OT;
    using result_type = vector<engine_type, op_traits>;

    static result_type  subtract(vector<ET1, OT1> const& v1, vector<ET2, OT2> const& v2);
};

//------
//
template<class OT, class ET1, class OT1, class ET2, class OT2>
struct matrix_subtraction_traits<OT, matrix<ET1, OT1>, matrix<ET2, OT2>>
{
    using engine_type = matrix_subtraction_engine_t<OT, ET1, ET2>;
    using op_traits   = OT;
    using result_type = matrix<engine_type, op_traits>;

    static result_type  subtract(matrix<ET1, OT1> const& m1, matrix<ET2, OT2> const& m2);
};

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_SUBTRACTION_TRAITS_HPP_DEFINED
