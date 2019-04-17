#ifndef LINEAR_ALGEBRA_NEGATION_TRAITS_HPP_DEFINED
#define LINEAR_ALGEBRA_NEGATION_TRAITS_HPP_DEFINED

namespace STD_LA {
//==================================================================================================
//                        **** ELEMENT NEGATION TRAITS AND DETECTORS ****
//==================================================================================================
//
//- This traits type provides the default mechanism for determining the result of adding two
//  elements of possibly different types.
//
template<class T1>
struct matrix_negation_element_traits
{
    using element_type = decltype(-declval<T1>());
};


namespace detail {
//--------------------------------------------------------------------------------------------------
//- Form 1 type detection of nested element negation traits.
//  First, define two helper aliases.
//
template<typename OT>
using element_neg_traits_f1_t = typename OT::element_negation_traits;

template<typename OT>
using element_neg_type_f1_t = typename element_neg_traits_f1_t<OT>::element_type;


//- Define the form 1 detectors.
//
template<typename OT, typename = void>
struct detect_element_neg_traits_f1
:   public false_type
{
    using traits_type = void;
};

template<typename OT>
struct detect_element_neg_traits_f1<OT, void_t<element_neg_type_f1_t<OT>>>
:   public true_type
{
    using traits_type = element_neg_traits_f1_t<OT>;
};


//----------------------------------------------------------
//- Form 2 type detection of nested element negation traits.
//  First, define two helper aliases.
//
template<typename OT, typename T1>
using element_neg_traits_f2_t = typename OT::template element_negation_traits<T1>;

template<typename OT, typename T1>
using element_neg_type_f2_t = typename element_neg_traits_f2_t<OT, T1>::element_type;


//- Define the form 2 detectors.
//
template<typename OT, typename T1, typename = void>
struct detect_element_neg_traits_f2
:   public false_type
{
    using traits_type = void;
};

template<typename OT, typename T1>
struct detect_element_neg_traits_f2<OT, T1, void_t<element_neg_type_f2_t<OT, T1>>>
:   public true_type
{
    using traits_type = element_neg_traits_f2_t<OT, T1>;
};


//---------------------------------------------
//- Element negation traits type determination.
//
template<typename OT, typename T1>
struct element_neg_traits_chooser
{
    using CT1 = typename detect_element_neg_traits_f1<OT>::traits_type;
    using CT2 = typename detect_element_neg_traits_f2<OT, T1>::traits_type;
    using DEF = matrix_negation_element_traits<T1>;

    using traits_type = typename non_void_traits_chooser<CT1, CT2, DEF>::traits_type;
};

template<typename OT, typename T1>
using element_neg_traits_t = typename element_neg_traits_chooser<OT, T1>::traits_type;

template<class OT, class T1>
constexpr bool  has_element_neg_traits_v = detect_element_neg_traits_f2<OT, T1>::value ||
                                           detect_element_neg_traits_f1<OT>::value;


//---------------------------------------------
//- Element negation result type determination.
//
template<typename OT, typename T1>
struct element_neg_type
{
    using traits_type  = typename element_neg_traits_chooser<OT, T1>::traits_type;
    using element_type = typename traits_type::element_type;
};

template<typename OT, typename T1>
using element_neg_type_t = typename element_neg_type<OT, T1>::element_type;


}       //- detail namespace

//---------------------------
//- Alias interface to trait.
//
template<class OT, class T1>
using matrix_negation_element_t = detail::element_neg_type_t<OT, T1>;


//==================================================================================================
//                         **** ENGINE NEGATION TRAITS AND DETECTORS ****
//==================================================================================================
//
//- This traits type performs engine promotion type computations for binary negation.
//
template<class OT, class ET1>
struct matrix_negation_engine_traits
{
    using element_type_1 = typename ET1::element_type;
    using element_type   = matrix_negation_element_t<OT, element_type_1>;
    using engine_type    = conditional_t<ET1::is_matrix::value,
                                         dr_matrix_engine<element_type, allocator<element_type>>,
                                         dr_vector_engine<element_type, allocator<element_type>>>;
};


//- Note that all cases where allocators are rebound assume standard-conformant allocator types.
//
template<class OT, class T1, class A1>
struct matrix_negation_engine_traits<OT, dr_matrix_engine<T1, A1>>
{
    using element_type = matrix_negation_element_t<OT, T1>;
    using engine_type  = dr_matrix_engine<T1, A1>;
};

template<class OT, class T1, int32_t R1, int32_t C1>
struct matrix_negation_engine_traits<OT, fs_matrix_engine<T1, R1, C1>>
{
    using element_type = matrix_negation_element_t<OT, T1>;
    using engine_type  = fs_matrix_engine<T1, R1, C1>;
};

//- Transpose cases for matrices.
//
template<class OT, class ET1>
struct matrix_negation_engine_traits<OT, tr_matrix_engine<ET1>>
{
    using engine_type = typename matrix_negation_engine_traits<OT, ET1>::engine_type;
};

/*
namespace detail {
//--------------------------------------------------------------------------------------------------
//- Form 1 type detection of nested engine negation traits.
//  First, define two helper aliases.
//
template<typename OT>
using engine_neg_traits_f1_t = typename OT::engine_negation_traits;

template<typename OT>
using engine_neg_type_f1_t = typename engine_neg_traits_f1_t<OT>::engine_type;


//- Define the form 1 detectors.
//
template<typename OT, typename = void>
struct detect_engine_neg_traits_f1
:   public false_type
{
    using traits_type = void;
};

template<typename OT>
struct detect_engine_neg_traits_f1<OT, void_t<engine_neg_type_f1_t<OT>>>
:   public true_type
{
    using traits_type = engine_neg_traits_f1_t<OT>;
};


//---------------------------------------------------------
//- Form 2 type detection of nested engine negation traits.
//  First, define two helper aliases.
//
template<typename OT, typename T1, typename T2>
using engine_neg_traits_f2_t = typename OT::template engine_negation_traits<OT, T1, T2>;

template<typename OT, typename T1, typename T2>
using engine_neg_type_f2_t = typename engine_neg_traits_f2_t<OT, T1, T2>::engine_type;


//- Define the form 2 detectors.
//
template<typename OT, typename ET1, typename ET2, typename = void>
struct detect_engine_neg_traits_f2
:   public false_type
{
    using traits_type = void;
};

template<typename OT, typename ET1, typename ET2>
struct detect_engine_neg_traits_f2<OT, ET1, ET2, void_t<engine_neg_type_f2_t<OT, ET1, ET2>>>
:   public true_type
{
    using traits_type = engine_neg_traits_f2_t<OT, ET1, ET2>;
};


//--------------------------------------------
//- Engine negation traits type determination.
//
template<typename OT, typename ET1, typename ET2>
struct engine_neg_traits_chooser
{
    using CT1 = typename detect_engine_neg_traits_f1<OT>::traits_type;
    using CT2 = typename detect_engine_neg_traits_f2<OT, ET1, ET2>::traits_type;
    using DEF = matrix_negation_engine_traits<OT, ET1, ET2>;

    using traits_type = typename non_void_traits_chooser<CT1, CT2, DEF>::traits_type;
};

template<typename OT, typename ET1, typename ET2>
using engine_neg_traits_t = typename engine_neg_traits_chooser<OT, ET1, ET2>::traits_type;

template<class OT, class ET1, class ET2>
constexpr bool  has_engine_neg_traits_v = detect_engine_neg_traits_f2<OT, ET1, ET2>::value ||
                                          detect_engine_neg_traits_f1<OT>::value;


//--------------------------------------------
//- Engine negation result type determination.
//
template<typename OT, typename ET1, typename ET2>
struct engine_neg_type
{
    using traits_type = typename engine_neg_traits_chooser<OT, ET1, ET2>::traits_type;
    using engine_type = typename traits_type::engine_type;
};

template<typename OT, typename ET1, typename ET2>
using engine_neg_type_t = typename engine_neg_type<OT, ET1, ET2>::engine_type;


}       //- detail namespace

//---------------------------
//- Alias interface to trait.
//
template<class OT, class ET1, class ET2>
using matrix_negation_engine_t = detail::engine_neg_type_t<OT, ET1, ET2>;


//==================================================================================================
//                      **** NEGATION ARITHMETIC TRAITS AND DETECTORS ****
//==================================================================================================
//
//- This traits type actually performs binary negation.
//
template<class OTR, class ET1, class OT1, class ET2, class OT2>
struct matrix_negation_traits<OTR, vector<ET1, OT1>, vector<ET2, OT2>>
{
    using engine_type = matrix_negation_engine_t<OTR, ET1, ET2>;
    using op_traits   = OTR;
    using result_type = vector<engine_type, op_traits>;

    static result_type  negate(vector<ET1, OT1> const& v1, vector<ET2, OT2> const& v2);
};

template<class OTR, class ET1, class OT1, class ET2, class OT2>
auto
matrix_negation_traits<OTR, vector<ET1, OT1>, vector<ET2, OT2>>::add
(vector<ET1, OT1> const& v1, vector<ET2, OT2> const& v2) -> result_type
{
    PrintOperandTypes<result_type>("negation_traits", v1, v2);
    return result_type();
}


//------
//
template<class OTR, class ET1, class OT1, class ET2, class OT2>
struct matrix_negation_traits<OTR, matrix<ET1, OT1>, matrix<ET2, OT2>>
{
    using engine_type = matrix_negation_engine_t<OTR, ET1, ET2>;
    using op_traits   = OTR;
    using result_type = matrix<engine_type, op_traits>;

    static result_type  negate(matrix<ET1, OT1> const& m1, matrix<ET2, OT2> const& m2);
};

template<class OTR, class ET1, class OT1, class ET2, class OT2>
auto
matrix_negation_traits<OTR, matrix<ET1, OT1>, matrix<ET2, OT2>>::add
(matrix<ET1, OT1> const& m1, matrix<ET2, OT2> const& m2) -> result_type
{
    PrintOperandTypes<result_type>("negation_traits", m1, m2);
    return result_type();
}


namespace detail {
//--------------------------------------------------------------------------------------------------
//- Form 1 type detection of nested negation arithmetic traits.
//  First, define two helper aliases.
//
template<typename OT>
using add_traits_f1_t = typename OT::negation_traits;

template<typename OT>
using add_type_f1_t = typename add_traits_f1_t<OT>::result_type;


//- Define the form 1 detectors.
//
template<typename OT, typename = void>
struct detect_neg_traits_f1
:   public false_type
{
    using traits_type = void;
};

template<typename OT>
struct detect_neg_traits_f1<OT, void_t<add_type_f1_t<OT>>>
:   public true_type
{
    using traits_type = add_traits_f1_t<OT>;
};


//-------------------------------------------------------------
//- Form 2 type detection of nested negation arithmetic traits.
//  First, define two helper aliases.
//
template<typename OT, typename T1, typename T2>
using add_traits_f2_t = typename OT::template negation_traits<OT, T1, T2>;

template<typename OT, typename T1, typename T2>
using add_type_f2_t = typename add_traits_f2_t<OT, T1, T2>::result_type;


//- Define the form 2 detectors.
//
template<typename OT, typename OP1, typename OP2, typename = void>
struct detect_neg_traits_f2
:   public false_type
{
    using traits_type = void;
};

template<typename OT, typename OP1, typename OP2>
struct detect_neg_traits_f2<OT, OP1, OP2, void_t<add_type_f2_t<OT, OP1, OP2>>>
:   public true_type
{
    using traits_type = typename OT::template negation_traits<OT, OP1, OP2>;
};


//------------------------------------------------
//- Addition arithmetic traits type determination.
//
template<typename OT, typename OP1, typename OP2>
struct add_traits_chooser
{
    using CT1 = typename detect_neg_traits_f1<OT>::traits_type;
    using CT2 = typename detect_neg_traits_f2<OT, OP1, OP2>::traits_type;
    using DEF = matrix_negation_traits<OT, OP1, OP2>;

    using traits_type = typename non_void_traits_chooser<CT1, CT2, DEF>::traits_type;
};

template<typename OT, typename OP1, typename OP2>
using negation_traits_t = typename add_traits_chooser<OT, OP1, OP2>::traits_type;

template<class OT, class OP1, class OP2>
constexpr bool  has_neg_traits_v = detect_neg_traits_f2<OT, OP1, OP2>::value ||
                                   detect_neg_traits_f1<OT>::value;

}       //- detail namespace

//---------------------------
//- Alias interface to trait.
//
template<class OT, class OP1, class OP2>
using matrix_negation_traits_t = detail::negation_traits_t<OT, OP1, OP2>;

*/
}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_NEGATION_TRAITS_HPP_DEFINED
