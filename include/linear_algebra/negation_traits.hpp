//==================================================================================================
//  File:       negation_traits.hpp
//
//  Summary:    This header defines several private and public traits types that are used to
//              implement the unary negation operator.  This is a somewhat long file, divided
//              into two main sections, each with three sub-sections:
//                  1. Private traits definitions that:
//                      A. determine the element promotion traits to be used for negation
//                      B. determine the engine promotion traits to be used for negation
//                      C. determine the arithmetic traits to be used to perform negation
//                  2. Public traits definitions that:
//                      A. perform element promotion for negation
//                      B. perform engine promotion for negation
//                      C. perform the actual act of negation
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_NEGATION_TRAITS_HPP_DEFINED
#define LINEAR_ALGEBRA_NEGATION_TRAITS_HPP_DEFINED

namespace STD_LA {
namespace detail {
//==================================================================================================
//                          **** ELEMENT NEGATION TRAITS DETECTORS ****
//==================================================================================================
//
//- Form 0 type detection of nested element negation traits.
//
template<typename OT>
using element_neg_traits_f0_t = typename OT::negation_element_traits;

template<typename OT>
using element_neg_result_f0_t = typename element_neg_traits_f0_t<OT>::element_type;

//- Define the form 0 detectors.
//
template<typename OT, typename = void>
struct detect_element_neg_traits_f0
:   public false_type
{
    using traits_type = void;
};

template<typename OT>
struct detect_element_neg_traits_f0<OT, void_t<element_neg_result_f0_t<OT>>>
:   public true_type
{
    using traits_type = element_neg_traits_f0_t<OT>;
};

//----------------------------------------------------------
//- Form 2 type detection of nested element negation traits.
//  First, define two helper aliases.
//
template<typename OT, typename T1>
using element_neg_traits_f2_t = typename OT::template negation_element_traits<T1>;

template<typename OT, typename T1>
using element_neg_result_f2_t = typename element_neg_traits_f2_t<OT, T1>::element_type;


//- Define the form 2 detectors.
//
template<typename OT, typename T1, typename = void>
struct detect_element_neg_traits_f2
:   public false_type
{
    using traits_type = void;
};

template<typename OT, typename T1>
struct detect_element_neg_traits_f2<OT, T1, void_t<element_neg_result_f2_t<OT, T1>>>
:   public true_type
{
    using traits_type = element_neg_traits_f2_t<OT, T1>;
};

//- Used only for testing.
//
template<class OT, class T1>
constexpr bool  has_element_neg_traits_v = detect_element_neg_traits_f2<OT, T1>::value ||
                                           detect_element_neg_traits_f0<OT>::value;

//---------------------------------------------
//- Element negation traits type determination.
//
template<typename OT, typename T1>
struct element_neg_traits_chooser
{
    using CT1 = typename detect_element_neg_traits_f0<OT>::traits_type;
    using CT2 = typename detect_element_neg_traits_f2<OT, T1>::traits_type;
    using DEF = matrix_negation_element_traits<T1>;

    using traits_type = typename non_void_traits_chooser<CT1, CT2, DEF>::traits_type;
    using element_type = typename traits_type::element_type;
};

template<typename OT, typename T1>
using element_neg_result_t = typename element_neg_traits_chooser<OT, T1>::element_type;


//==================================================================================================
//                           **** ENGINE NEGATION TRAITS DETECTORS ****
//==================================================================================================
//
//- Form 0 type detection of nested engine negation traits.
//
template<typename OT>
using engine_neg_traits_f0_t = typename OT::negation_engine_traits;

template<typename OT>
using engine_neg_result_f0_t = typename engine_neg_traits_f0_t<OT>::engine_type;

//- Define the form 0 detectors.
//
template<typename OT, typename = void>
struct detect_engine_neg_traits_f0
:   public false_type
{
    using traits_type = void;
};

template<typename OT>
struct detect_engine_neg_traits_f0<OT, void_t<engine_neg_result_f0_t<OT>>>
:   public true_type
{
    using traits_type = engine_neg_traits_f0_t<OT>;
};

//---------------------------------------------------------
//- Form 2 type detection of nested engine negation traits.
//
template<typename OT, typename T1>
using engine_neg_traits_f2_t = typename OT::template negation_engine_traits<OT, T1>;

template<typename OT, typename T1>
using engine_neg_result_f2_t = typename engine_neg_traits_f2_t<OT, T1>::engine_type;

//- Define the form 2 detectors.
//
template<typename OT, typename ET1, typename = void>
struct detect_engine_neg_traits_f2
:   public false_type
{
    using traits_type = void;
};

template<typename OT, typename ET1>
struct detect_engine_neg_traits_f2<OT, ET1, void_t<engine_neg_result_f2_t<OT, ET1>>>
:   public true_type
{
    using traits_type = engine_neg_traits_f2_t<OT, ET1>;
};

//- Used only for testing.
//
template<class OT, class ET1>
constexpr bool  has_engine_neg_traits_v = detect_engine_neg_traits_f2<OT, ET1>::value ||
                                          detect_engine_neg_traits_f0<OT>::value;

//--------------------------------------------
//- Engine negation traits type determination.
//
template<typename OT, typename ET1>
struct engine_neg_traits_chooser
{
    using CT1 = typename detect_engine_neg_traits_f0<OT>::traits_type;
    using CT2 = typename detect_engine_neg_traits_f2<OT, ET1>::traits_type;
    using DEF = matrix_negation_engine_traits<OT, ET1>;

    using traits_type = typename non_void_traits_chooser<CT1, CT2, DEF>::traits_type;
    using engine_type = typename traits_type::engine_type;
};

template<typename OT, typename ET1>
using engine_neg_result_t = typename engine_neg_traits_chooser<OT, ET1>::engine_type;


//==================================================================================================
//                      **** NEGATION ARITHMETIC TRAITS AND DETECTORS ****
//==================================================================================================
//
//- Form 0 type detection of nested negation arithmetic traits.
//
template<typename OT>
using neg_traits_f0_t = typename OT::negation_arithmetic_traits;

template<typename OT>
using neg_result_f0_t = typename neg_traits_f0_t<OT>::result_type;


//- Define the form 0 detectors.
//
template<typename OT, typename = void>
struct detect_neg_traits_f0
:   public false_type
{
    using traits_type = void;
};

template<typename OT>
struct detect_neg_traits_f0<OT, void_t<neg_result_f0_t<OT>>>
:   public true_type
{
    using traits_type = neg_traits_f0_t<OT>;
};

//-------------------------------------------------------------
//- Form 2 type detection of nested negation arithmetic traits.
//
template<typename OT, typename T1>
using neg_traits_f2_t = typename OT::template negation_arithmetic_traits<OT, T1>;

template<typename OT, typename T1>
using neg_result_f2_t = typename neg_traits_f2_t<OT, T1>::result_type;

//- Define the form 2 detectors.
//
template<typename OT, typename OP1, typename = void>
struct detect_neg_traits_f2
:   public false_type
{
    using traits_type = void;
};

template<typename OT, typename OP1>
struct detect_neg_traits_f2<OT, OP1, void_t<neg_result_f2_t<OT, OP1>>>
:   public true_type
{
    using traits_type = typename OT::template negation_arithmetic_traits<OT, OP1>;
};

//- Used only for testing.
//
template<class OT, class OP1>
constexpr bool  has_neg_traits_v = detect_neg_traits_f2<OT, OP1>::value ||
                                   detect_neg_traits_f0<OT>::value;

//------------------------------------------------
//- Addition arithmetic traits type determination.
//
template<typename OT, typename OP1>
struct neg_traits_chooser
{
    using CT1 = typename detect_neg_traits_f0<OT>::traits_type;
    using CT2 = typename detect_neg_traits_f2<OT, OP1>::traits_type;
    using DEF = matrix_negation_arithmetic_traits<OT, OP1>;

    using traits_type = typename non_void_traits_chooser<CT1, CT2, DEF>::traits_type;
};

template<typename OT, typename OP1>
using neg_traits_result_t = typename neg_traits_chooser<OT, OP1>::traits_type;


}       //- detail namespace
//==================================================================================================
//                                 **** ELEMENT NEGATION TRAITS ****
//==================================================================================================
//
//- Alias interface to detection meta-function that extracts the element negation traits type.
//
template<class OT, class T1>
using select_matrix_negation_element_t = detail::element_neg_result_t<OT, T1>;


//- The standard element negation traits type provides the default mechanism for determining the
//  result of negating a vector or matrix element.
//
template<class T1>
struct matrix_negation_element_traits
{
    using element_type = decltype(-declval<T1>());
};


//==================================================================================================
//                                   **** ENGINE NEGATION TRAITS ****
//==================================================================================================
//
//- Alias interface to detection meta-function that extracts the engine negation traits type.
//- Alias interface to trait.
//
template<class OT, class ET1>
using select_matrix_negation_engine_t = detail::engine_neg_result_t<OT, ET1>;


//- The standard engine negation traits type provides the default mechanism for determining the
//  correct engine type for a matrix or vector negation.
//
template<class OT, class ET1>
struct matrix_negation_engine_traits
{
    using element_type_1 = typename ET1::element_type;
    using element_type   = select_matrix_negation_element_t<OT, element_type_1>;
    using engine_type    = conditional_t<is_matrix_engine_v<ET1>,
                                         dr_matrix_engine<element_type, allocator<element_type>>,
                                         dr_vector_engine<element_type, allocator<element_type>>>;
};

//- General transpose cases for matrices.
//
template<class OT, class ET1, class MCT1>
struct matrix_negation_engine_traits<OT, transpose_engine<ET1, MCT1>>
{
    using element_type = select_matrix_negation_element_t<OT, typename ET1::element_type>;
    using engine_type  = typename matrix_negation_engine_traits<OT, ET1>::engine_type;
};

//--------------------------------------------------------------------------------------------------
//  -ENGINE case for vector.  Note that all partial specializations below in which allocators
//  are rebound assume standard-conformant allocator types.
//
//- dr_vector_engine.
//
template<class OT, class T1, class A1>
struct matrix_negation_engine_traits<OT, dr_vector_engine<T1, A1>>
{
    using element_type = select_matrix_negation_element_t<OT, T1>;
    using engine_type  = dr_vector_engine<T1, A1>;
};

//-------------------
//- fs_vector_engine.
//
template<class OT, class T1, ptrdiff_t N1>
struct matrix_negation_engine_traits<OT, fs_vector_engine<T1, N1>>
{
    using element_type = select_matrix_negation_element_t<OT, T1>;
    using engine_type  = fs_vector_engine<T1, N1>;
};

//--------------------------------------------------------------------------------------------------
//  -ENGINE case for matrix.  Note that all partial specializations below in which allocators
//  are rebound assume standard-conformant allocator types.
//
//- dr_matrix_engine.
//
template<class OT, class T1, ptrdiff_t R1, ptrdiff_t C1>
struct matrix_negation_engine_traits<OT, fs_matrix_engine<T1, R1, C1>>
{
    using element_type = select_matrix_negation_element_t<OT, T1>;
    using engine_type  = fs_matrix_engine<T1, R1, C1>;
};

template<class OT, class T1, ptrdiff_t R1, ptrdiff_t C1, class MCT1>
struct matrix_negation_engine_traits<OT, transpose_engine<fs_matrix_engine<T1, R1, C1>, MCT1>>
{
    using element_type = select_matrix_negation_element_t<OT, T1>;
    using engine_type  = fs_matrix_engine<element_type, C1, R1>;
};

//-------------------
//- fs_matrix_engine.
//
template<class OT, class T1, class A1>
struct matrix_negation_engine_traits<OT, dr_matrix_engine<T1, A1>>
{
    using element_type = select_matrix_negation_element_t<OT, T1>;
    using engine_type  = dr_matrix_engine<T1, A1>;
};

template<class OT, class T1, class A1, class MCT1>
struct matrix_negation_engine_traits<OT, transpose_engine<dr_matrix_engine<T1, A1>, MCT1>>
{
    using element_type = select_matrix_negation_element_t<OT, T1>;
    using engine_type  = dr_matrix_engine<T1, A1>;
};


//==================================================================================================
//                               **** NEGATION ARITHMETIC TRAITS ****
//==================================================================================================
//
//- Alias interface to detection meta-function that extracts the negation traits type.
//
template<class OT, class OP1>
using select_matrix_negation_arithmetic_t = detail::neg_traits_result_t<OT, OP1>;


//- The standard addition traits type provides the default mechanism for computing the result
//  of a matrix or vector negation.
//
template<class OT, class ET1, class OT1>
struct matrix_negation_arithmetic_traits<OT, vector<ET1, OT1>>
{
    using engine_type = select_matrix_negation_engine_t<OT, ET1>;
    using result_type = vector<engine_type, OT>;

    static constexpr result_type    negate(vector<ET1, OT1> const& v1);
};

template<class OT, class ET1, class OT1> inline constexpr
auto
matrix_negation_arithmetic_traits<OT, vector<ET1, OT1>>::negate(vector<ET1, OT1> const& v1) -> result_type
{
    using index_type = typename vector<ET1, OT1>::index_type;

    index_type      elems = v1.size();
    result_type     vr;

    if constexpr (is_resizable_engine_v<engine_type>)
    {
        vr.resize(elems);
    }

    index_type  ir = 0;
    index_type  i1 = 0;

    for (;  ir < elems;  ++ir, ++i1)
    {
        vr(ir) = -v1(i1);
    }

    return vr;
}

//------
//
template<class OT, class ET1, class OT1>
struct matrix_negation_arithmetic_traits<OT, matrix<ET1, OT1>>
{
    using engine_type = select_matrix_negation_engine_t<OT, ET1>;
    using result_type = matrix<engine_type, OT>;

    static constexpr result_type    negate(matrix<ET1, OT1> const& m1);
};

template<class OT, class ET1, class OT1> inline constexpr
auto
matrix_negation_arithmetic_traits<OT, matrix<ET1, OT1>>::negate(matrix<ET1, OT1> const& m1) -> result_type
{
    using index_type = typename matrix<ET1, OT1>::index_type;

    index_type      rows = m1.rows();
    index_type      cols = m1.columns();
    result_type		mr;

    if constexpr (is_resizable_engine_v<engine_type>)
    {
        mr.resize(rows, cols);
    }

    index_type  ir = 0;
    index_type  i1 = 0;

    for (;  ir < rows;  ++ir, ++i1)
    {
        index_type  jr = 0;
        index_type  j1 = 0;

        for (;  jr < cols;  ++jr, ++j1)
        {
            mr(ir, jr) = -m1(i1, j1);
        }
    }

    return mr;
}

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_NEGATION_TRAITS_HPP_DEFINED
