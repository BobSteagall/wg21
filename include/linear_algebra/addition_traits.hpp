//==================================================================================================
//  File:       addition_traits.hpp
//
//  Summary:    This header defines several private and public traits types that are used to
//              implement the binary addition operator.  This is a somewhat long file, divided
//              into two main sections, each with three sub-sections:
//                  1. Private traits definitions that:
//                      A. determine the element promotion traits to be used for addition
//                      B. determine the engine promotion traits to be used for addition
//                      C. determine the arithmetic traits to be used to perform addition
//                  2. Public traits definitions that:
//                      A. perform element promotion for addition
//                      B. perform engine promotion for addition
//                      C. perform the actual act of addition
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_ADDITION_TRAITS_HPP_DEFINED
#define LINEAR_ALGEBRA_ADDITION_TRAITS_HPP_DEFINED

namespace STD_LA {
namespace detail {
//==================================================================================================
//                         **** ELEMENT ADDITION TRAITS DETECTORS ****
//==================================================================================================
//
//- Form 0 type detection of nested element addition traits.
//
template<typename OT>
using element_add_traits_f0_t = typename OT::addition_element_traits;

template<typename OT>
using element_add_result_f0_t = typename element_add_traits_f0_t<OT>::element_type;

//- Define the form 0 detectors.
//
template<typename OT, typename = void>
struct detect_element_add_traits_f0
:   public false_type
{
    using traits_type = void;
};

template<typename OT>
struct detect_element_add_traits_f0<OT, void_t<element_add_result_f0_t<OT>>>
:   public true_type
{
    using traits_type = element_add_traits_f0_t<OT>;
};

//----------------------------------------------------------
//- Form 2 type detection of nested element addition traits.
//
template<typename OT, typename T1, typename T2>
using element_add_traits_f2_t = typename OT::template addition_element_traits<T1, T2>;

template<typename OT, typename T1, typename T2>
using element_add_result_f2_t = typename element_add_traits_f2_t<OT, T1, T2>::element_type;

//- Define the form 2 detectors.
//
template<typename OT, typename T1, typename T2, typename = void>
struct detect_element_add_traits_f2
:   public false_type
{
    using traits_type = void;
};

template<typename OT, typename T1, typename T2>
struct detect_element_add_traits_f2<OT, T1, T2, void_t<element_add_result_f2_t<OT, T1, T2>>>
:   public true_type
{
    using traits_type = element_add_traits_f2_t<OT, T1, T2>;
};

//- Used only for testing.
//
template<class OT, class T1, class T2>
constexpr bool  has_element_add_traits_v = detect_element_add_traits_f2<OT, T1, T2>::value ||
                                           detect_element_add_traits_f0<OT>::value;

//---------------------------------------------
//- Element addition traits type determination.
//
template<typename OT, typename T1, typename T2>
struct element_add_traits_chooser
{
    using CT1 = typename detect_element_add_traits_f0<OT>::traits_type;
    using CT2 = typename detect_element_add_traits_f2<OT, T1, T2>::traits_type;
    using DEF = matrix_addition_element_traits<T1, T2>;

    using traits_type  = typename non_void_traits_chooser<CT1, CT2, DEF>::traits_type;
    using element_type = typename traits_type::element_type;
};

template<typename OT, typename T1, typename T2>
using element_add_result_t = typename element_add_traits_chooser<OT, T1, T2>::element_type;


//==================================================================================================
//                          **** ENGINE ADDITION TRAITS DETECTORS ****
//==================================================================================================
//
//- Form 0 type detection of nested engine addition traits.
//
template<typename OT>
using engine_add_traits_f0_t = typename OT::addition_engine_traits;

template<typename OT>
using engine_add_result_f0_t = typename engine_add_traits_f0_t<OT>::engine_type;

//- Define the form 0 detectors.
//
template<typename OT, typename = void>
struct detect_engine_add_traits_f0
:   public false_type
{
    using traits_type = void;
};

template<typename OT>
struct detect_engine_add_traits_f0<OT, void_t<engine_add_result_f0_t<OT>>>
:   public true_type
{
    using traits_type = engine_add_traits_f0_t<OT>;
};

//---------------------------------------------------------
//- Form 2 type detection of nested engine addition traits.
//
template<typename OT, typename T1, typename T2>
using engine_add_traits_f2_t = typename OT::template addition_engine_traits<OT, T1, T2>;

template<typename OT, typename T1, typename T2>
using engine_add_result_f2_t = typename engine_add_traits_f2_t<OT, T1, T2>::engine_type;

//- Define the form 2 detectors.
//
template<typename OT, typename ET1, typename ET2, typename = void>
struct detect_engine_add_traits_f2
:   public false_type
{
    using traits_type = void;
};

template<typename OT, typename ET1, typename ET2>
struct detect_engine_add_traits_f2<OT, ET1, ET2, void_t<engine_add_result_f2_t<OT, ET1, ET2>>>
:   public true_type
{
    using traits_type = engine_add_traits_f2_t<OT, ET1, ET2>;
};

//- Used only for testing.
//
template<class OT, class ET1, class ET2>
constexpr bool  has_engine_add_traits_v = detect_engine_add_traits_f2<OT, ET1, ET2>::value ||
                                          detect_engine_add_traits_f0<OT>::value;

//--------------------------------------------
//- Engine addition traits type determination.
//
template<typename OT, typename ET1, typename ET2>
struct engine_add_traits_chooser
{
    using CT1 = typename detect_engine_add_traits_f0<OT>::traits_type;
    using CT2 = typename detect_engine_add_traits_f2<OT, ET1, ET2>::traits_type;
    using DEF = matrix_addition_engine_traits<OT, ET1, ET2>;

    using traits_type = typename non_void_traits_chooser<CT1, CT2, DEF>::traits_type;
    using engine_type = typename traits_type::engine_type;
};

template<typename OT, typename ET1, typename ET2>
using engine_add_result_t = typename engine_add_traits_chooser<OT, ET1, ET2>::engine_type;


//==================================================================================================
//                         **** ADDITION ARITHMETIC TRAITS DETECTORS ****
//==================================================================================================
//
//- Form 0 type detection of nested addition arithmetic traits.
//
template<typename OT>
using add_traits_f0_t = typename OT::addition_arithmetic_traits;

template<typename OT>
using add_result_f0_t = typename add_traits_f0_t<OT>::result_type;

//- Define the form 0 detectors.
//
template<typename OT, typename = void>
struct detect_add_traits_f0
:   public false_type
{
    using traits_type = void;
};

template<typename OT>
struct detect_add_traits_f0<OT, void_t<add_result_f0_t<OT>>>
:   public true_type
{
    using traits_type = add_traits_f0_t<OT>;
};

//-------------------------------------------------------------
//- Form 2 type detection of nested addition arithmetic traits.
//
template<typename OT, typename T1, typename T2>
using add_traits_f2_t = typename OT::template addition_arithmetic_traits<OT, T1, T2>;

template<typename OT, typename T1, typename T2>
using add_result_f2_t = typename add_traits_f2_t<OT, T1, T2>::result_type;

//- Define the form 2 detectors.
//
template<typename OT, typename OP1, typename OP2, typename = void>
struct detect_add_traits_f2
:   public false_type
{
    using traits_type = void;
};

template<typename OT, typename OP1, typename OP2>
struct detect_add_traits_f2<OT, OP1, OP2, void_t<add_result_f2_t<OT, OP1, OP2>>>
:   public true_type
{
    using traits_type = typename OT::template addition_arithmetic_traits<OT, OP1, OP2>;
};

//- Used only for testing.
//
template<class OT, class OP1, class OP2>
constexpr bool  has_add_traits_v = detect_add_traits_f2<OT, OP1, OP2>::value ||
                                   detect_add_traits_f0<OT>::value;

//------------------------------------------------
//- Addition arithmetic traits type determination.
//
template<typename OT, typename OP1, typename OP2>
struct add_traits_chooser
{
    using CT1 = typename detect_add_traits_f0<OT>::traits_type;
    using CT2 = typename detect_add_traits_f2<OT, OP1, OP2>::traits_type;
    using DEF = matrix_addition_arithmetic_traits<OT, OP1, OP2>;

    using traits_type = typename non_void_traits_chooser<CT1, CT2, DEF>::traits_type;
};

template<typename OT, typename OP1, typename OP2>
using add_traits_result_t = typename add_traits_chooser<OT, OP1, OP2>::traits_type;


}       //- detail namespace
//==================================================================================================
//                              **** ELEMENT ADDITION TRAITS ****
//==================================================================================================
//
//- Alias interface to detection meta-function that extracts the element addition traits type.
//
template<class OT, class T1, class T2>
using select_matrix_addition_element_t = detail::element_add_result_t<OT, T1, T2>;


//- The standard element addition traits type provides the default mechanism for determining the
//  result of adding two elements of (possibly) different types.
//
template<class T1, class T2>
struct matrix_addition_element_traits
{
    using element_type = decltype(declval<T1>() + declval<T2>());
};


//==================================================================================================
//                               **** ENGINE ADDITION TRAITS ****
//==================================================================================================
//
//- Alias interface to detection meta-function that extracts the engine addition traits type.
//
template<class OT, class ET1, class ET2>
using select_matrix_addition_engine_t = detail::engine_add_result_t<OT, ET1, ET2>;


//- The standard engine addition traits type provides the default mechanism for determining the
//  correct engine type for a matrix/matrix or vector/vector addition.
//
template<class OT, class ET1, class ET2>
struct matrix_addition_engine_traits
{
    static_assert(engines_are_similar_v<ET1, ET2>);

    using element_type_1 = typename ET1::element_type;
    using element_type_2 = typename ET2::element_type;
    using element_type   = select_matrix_addition_element_t<OT, element_type_1, element_type_2>;
    using engine_type    = conditional_t<is_matrix_engine_v<ET1>,
                                         dr_matrix_engine<element_type, allocator<element_type>>,
                                         dr_vector_engine<element_type, allocator<element_type>>>;
};

//- General transpose cases for matrices.
//
template<class OT, class ET1, class MCT1, class ET2>
struct matrix_addition_engine_traits<OT, matrix_transpose_engine<ET1, MCT1>, ET2>
{
    using engine_type = typename matrix_addition_engine_traits<OT, ET1, ET2>::engine_type;
};

template<class OT, class ET1, class ET2, class MCT2>
struct matrix_addition_engine_traits<OT, ET1, matrix_transpose_engine<ET2, MCT2>>
{
    using engine_type = typename matrix_addition_engine_traits<OT, ET1, ET2>::engine_type;
};

template<class OT, class ET1, class MCT1, class ET2, class MCT2>
struct matrix_addition_engine_traits<OT,
                                     matrix_transpose_engine<ET1, MCT1>,
                                     matrix_transpose_engine<ET2, MCT2>>
{
    using engine_type = typename matrix_addition_engine_traits<OT, ET1, ET2>::engine_type;
};


//--------------------------------------------------------------------------------------------------
//- ENGINE + ENGINE cases for vector/vector.  Note that all partial specializations below in which
//  allocators are rebound assume standard-conformant allocator types.
//
//- dr_vector_engine + dr_vector_engine.
//
template<class OT, class T1, class A1, class T2, class A2>
struct matrix_addition_engine_traits<OT, dr_vector_engine<T1, A1>, dr_vector_engine<T2, A2>>
{
    using element_type = select_matrix_addition_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<A1, element_type>;
    using engine_type  = dr_vector_engine<element_type, alloc_type>;
};

//--------------------------------------
//- dr_vector_engine + fs_vector_engine.
//
template<class OT, class T1, class A1, class T2, ptrdiff_t N2>
struct matrix_addition_engine_traits<OT, dr_vector_engine<T1, A1>, fs_vector_engine<T2, N2>>
{
    using element_type = select_matrix_addition_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<A1, element_type>;
    using engine_type  = dr_vector_engine<element_type, alloc_type>;
};

//--------------------------------------
//- fs_vector_engine + dr_vector_engine.
//
template<class OT, class T1, ptrdiff_t N1, class T2, class A2>
struct matrix_addition_engine_traits<OT, fs_vector_engine<T1, N1>, dr_vector_engine<T2, A2>>
{
    using element_type = select_matrix_addition_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<A2, element_type>;
    using engine_type  = dr_vector_engine<element_type, alloc_type>;
};

//--------------------------------------
//- fs_vector_engine + fs_vector_engine.
//
template<class OT, class T1, ptrdiff_t N1, class T2, ptrdiff_t N2>
struct matrix_addition_engine_traits<OT, fs_vector_engine<T1, N1>, fs_vector_engine<T2, N2>>
{
    static_assert(N1 == N2);
    using element_type = select_matrix_addition_element_t<OT, T1, T2>;
    using engine_type  = fs_vector_engine<element_type, N1>;
};

//--------------------------------------------------------------------------------------------------
//- ENGINE + ENGINE cases for matrix/matrix.  Note that all partial specializations below in which
//  allocators are rebound assume standard-conformant allocator types.
//
//- dr_matrix_engine + dr_matrix_engine.
//
template<class OT, class T1, class A1, class T2, class A2>
struct matrix_addition_engine_traits<OT,
                                     dr_matrix_engine<T1, A1>,
                                     dr_matrix_engine<T2, A2>>
{
    using element_type = select_matrix_addition_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<A1, element_type>;
    using engine_type  = dr_matrix_engine<element_type, alloc_type>;
};

template<class OT, class T1, class A1, class T2, class A2, class MCT2>
struct matrix_addition_engine_traits<OT,
                                     dr_matrix_engine<T1, A1>,
                                     matrix_transpose_engine<dr_matrix_engine<T2, A2>, MCT2>>
{
    using element_type = select_matrix_addition_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<A1, element_type>;
    using engine_type  = dr_matrix_engine<element_type, alloc_type>;
};

template<class OT, class T1, class A1, class MCT1, class T2, class A2>
struct matrix_addition_engine_traits<OT,
                                     matrix_transpose_engine<dr_matrix_engine<T1, A1>, MCT1>,
                                     dr_matrix_engine<T2, A2>>
{
    using element_type = select_matrix_addition_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<A1, element_type>;
    using engine_type  = dr_matrix_engine<element_type, alloc_type>;
};

template<class OT, class T1, class A1, class MCT1, class T2, class A2, class MCT2>
struct matrix_addition_engine_traits<OT,
                                     matrix_transpose_engine<dr_matrix_engine<T1, A1>, MCT1>,
                                     matrix_transpose_engine<dr_matrix_engine<T2, A2>, MCT2>>
{
    using element_type = select_matrix_addition_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<A1, element_type>;
    using engine_type  = dr_matrix_engine<element_type, alloc_type>;
};

//--------------------------------------
//- dr_matrix_engine + fs_matrix_engine.
//
template<class OT, class T1, class A1, class T2, ptrdiff_t R2, ptrdiff_t C2>
struct matrix_addition_engine_traits<OT,
                                     dr_matrix_engine<T1, A1>,
                                     fs_matrix_engine<T2, R2, C2>>
{
    using element_type = select_matrix_addition_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<A1, element_type>;
    using engine_type  = dr_matrix_engine<element_type, alloc_type>;
};

template<class OT, class T1, class A1, class T2, ptrdiff_t R2, ptrdiff_t C2, class MCT2>
struct matrix_addition_engine_traits<OT,
                                     dr_matrix_engine<T1, A1>,
                                     matrix_transpose_engine<fs_matrix_engine<T2, R2, C2>, MCT2>>
{
    using element_type = select_matrix_addition_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<A1, element_type>;
    using engine_type  = dr_matrix_engine<element_type, alloc_type>;
};

template<class OT, class T1, class A1, class MCT1, class T2, ptrdiff_t R2, ptrdiff_t C2>
struct matrix_addition_engine_traits<OT,
                                     matrix_transpose_engine<dr_matrix_engine<T1, A1>, MCT1>,
                                     fs_matrix_engine<T2, R2, C2>>
{
    using element_type = select_matrix_addition_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<A1, element_type>;
    using engine_type  = dr_matrix_engine<element_type, alloc_type>;
};

template<class OT, class T1, class A1, class MCT1, class T2, ptrdiff_t R2, ptrdiff_t C2, class MCT2>
struct matrix_addition_engine_traits<OT,
                                     matrix_transpose_engine<dr_matrix_engine<T1, A1>, MCT1>,
                                     matrix_transpose_engine<fs_matrix_engine<T2, R2, C2>, MCT2>>
{
    using element_type = select_matrix_addition_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<A1, element_type>;
    using engine_type  = dr_matrix_engine<element_type, alloc_type>;
};

//--------------------------------------
//- fs_matrix_engine + dr_matrix_engine.
//
template<class OT, class T1, ptrdiff_t R1, ptrdiff_t C1, class T2, class A2>
struct matrix_addition_engine_traits<OT,
                                     fs_matrix_engine<T1, R1, C1>,
                                     dr_matrix_engine<T2, A2>>
{
    using element_type = select_matrix_addition_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<A2, element_type>;
    using engine_type  = dr_matrix_engine<element_type, alloc_type>;
};

template<class OT, class T1, ptrdiff_t R1, ptrdiff_t C1, class T2, class A2, class MCT2>
struct matrix_addition_engine_traits<OT,
                                     fs_matrix_engine<T1, R1, C1>,
                                     matrix_transpose_engine<dr_matrix_engine<T2, A2>, MCT2>>
{
    using element_type = select_matrix_addition_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<A2, element_type>;
    using engine_type  = dr_matrix_engine<element_type, alloc_type>;
};

template<class OT, class T1, ptrdiff_t R1, ptrdiff_t C1, class MCT1, class T2, class A2>
struct matrix_addition_engine_traits<OT,
                                     matrix_transpose_engine<fs_matrix_engine<T1, R1, C1>, MCT1>,
                                     dr_matrix_engine<T2, A2>>
{
    using element_type = select_matrix_addition_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<A2, element_type>;
    using engine_type  = dr_matrix_engine<element_type, alloc_type>;
};

template<class OT, class T1, ptrdiff_t R1, ptrdiff_t C1, class MCT1, class T2, class A2, class MCT2>
struct matrix_addition_engine_traits<OT,
                                     matrix_transpose_engine<fs_matrix_engine<T1, R1, C1>, MCT1>,
                                     matrix_transpose_engine<dr_matrix_engine<T2, A2>, MCT2>>
{
    using element_type = select_matrix_addition_element_t<OT, T1, T2>;
    using alloc_type   = detail::rebind_alloc_t<A2, element_type>;
    using engine_type  = dr_matrix_engine<element_type, alloc_type>;
};

//--------------------------------------
//- fs_matrix_engine + fs_matrix_engine.
//
template<class OT, class T1, ptrdiff_t R1, ptrdiff_t C1, class T2, ptrdiff_t R2, ptrdiff_t C2>
struct matrix_addition_engine_traits<OT,
                                     fs_matrix_engine<T1, R1, C1>,
                                     fs_matrix_engine<T2, R2, C2>>
{
    static_assert(R1 == R2);
    static_assert(C1 == C2);
    using element_type = select_matrix_addition_element_t<OT, T1, T2>;
    using engine_type  = fs_matrix_engine<element_type, R1, C1>;
};

template<class OT, class T1, ptrdiff_t R1, ptrdiff_t C1, class MCT1, class T2, ptrdiff_t R2, ptrdiff_t C2>
struct matrix_addition_engine_traits<OT,
                                     matrix_transpose_engine<fs_matrix_engine<T1, R1, C1>, MCT1>,
                                     fs_matrix_engine<T2, R2, C2>>
{
    static_assert(R1 == C2);
    static_assert(C1 == R2);
    using element_type = select_matrix_addition_element_t<OT, T1, T2>;
    using engine_type  = fs_matrix_engine<element_type, R2, C2>;
};

template<class OT, class T1, ptrdiff_t R1, ptrdiff_t C1, class T2, ptrdiff_t R2, ptrdiff_t C2, class MCT2>
struct matrix_addition_engine_traits<OT,
                                     fs_matrix_engine<T1, R1, C1>,
                                     matrix_transpose_engine<fs_matrix_engine<T2, R2, C2>, MCT2>>
{
    static_assert(R1 == C2);
    static_assert(C1 == R2);
    using element_type = select_matrix_addition_element_t<OT, T1, T2>;
    using engine_type  = fs_matrix_engine<element_type, R1, C1>;
};

template<class OT, class T1, ptrdiff_t R1, ptrdiff_t C1, class MCT1, class T2, ptrdiff_t R2, ptrdiff_t C2, class MCT2>
struct matrix_addition_engine_traits<OT,
                                     matrix_transpose_engine<fs_matrix_engine<T1, R1, C1>, MCT1>,
                                     matrix_transpose_engine<fs_matrix_engine<T2, R2, C2>, MCT2>>
{
    static_assert(R1 == R2);
    static_assert(C1 == C2);
    using element_type = select_matrix_addition_element_t<OT, T1, T2>;
    using engine_type  = fs_matrix_engine<element_type, C1, R1>;
};


//==================================================================================================
//                                 **** ADDITION TRAITS ****
//==================================================================================================
//
//- Alias interface to detection meta-function that extracts the addition traits type.
//
template<class OT, class OP1, class OP2>
using select_matrix_addition_arithmetic_t = detail::add_traits_result_t<OT, OP1, OP2>;


//- The standard addition traits type provides the default mechanism for computing the result
//  of a matrix/matrix or vector/vector addition.
//
//- (vector + vector)
//
template<class OT, class ET1, class OT1, class ET2, class OT2>
struct matrix_addition_arithmetic_traits<OT, vector<ET1, OT1>, vector<ET2, OT2>>
{
    using engine_type = select_matrix_addition_engine_t<OT, ET1, ET2>;
    using result_type = vector<engine_type, OT>;

    static constexpr result_type    add(vector<ET1, OT1> const& v1, vector<ET2, OT2> const& v2);
};

template<class OT, class ET1, class OT1, class ET2, class OT2> inline constexpr
auto
matrix_addition_arithmetic_traits<OT, vector<ET1, OT1>, vector<ET2, OT2>>::add
(vector<ET1, OT1> const& v1, vector<ET2, OT2> const& v2) -> result_type
{
    using index_type_1 = typename vector<ET1, OT1>::index_type;
    using index_type_2 = typename vector<ET2, OT2>::index_type;
    using index_type_r = typename result_type::index_type;

    index_type_r    elems = static_cast<index_type_r>(v1.size());
    result_type     vr;

    if constexpr (is_resizable_engine_v<engine_type>)
    {
        vr.resize(elems);
    }

    index_type_r    ir = 0;
    index_type_1    i1 = 0;
    index_type_2    i2 = 0;

    for (;  ir < elems;  ++ir, ++i1, ++i2)
    {
        vr(ir) = v1(i1) + v2(i2);
    }

    return vr;
}

//-------------------
//- (matrix + matrix)
//
template<class OT, class ET1, class OT1, class ET2, class OT2>
struct matrix_addition_arithmetic_traits<OT, matrix<ET1, OT1>, matrix<ET2, OT2>>
{
    using engine_type = select_matrix_addition_engine_t<OT, ET1, ET2>;
    using result_type = matrix<engine_type, OT>;

    static constexpr result_type    add(matrix<ET1, OT1> const& m1, matrix<ET2, OT2> const& m2);
};

template<class OT, class ET1, class OT1, class ET2, class OT2> inline constexpr
auto
matrix_addition_arithmetic_traits<OT, matrix<ET1, OT1>, matrix<ET2, OT2>>::add
(matrix<ET1, OT1> const& m1, matrix<ET2, OT2> const& m2) -> result_type
{
    using index_type_1 = typename matrix<ET1, OT1>::index_type;
    using index_type_2 = typename matrix<ET2, OT2>::index_type;
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
    index_type_2    i2 = 0;

	for (;  ir < rows;  ++ir, ++i1, ++i2)
    {
        index_type_r    jr = 0;
        index_type_1    j1 = 0;
        index_type_2    j2 = 0;

        for (;  jr < cols;  ++jr, ++j1, ++j2)
        {
			mr(ir, jr) = m1(i1, j1) + m2(i2, j2);
        }
    }

	return mr;
}

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_ADDITION_TRAITS_HPP_DEFINED
