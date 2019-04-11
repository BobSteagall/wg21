#ifndef LINEAR_ALGEBRA_ADDITION_TRAITS_HPP_DEFINED
#define LINEAR_ALGEBRA_ADDITION_TRAITS_HPP_DEFINED

//- These helper macros are used only in the addition-related detectors defined below.  Each one
//  corresponds to a form of detection, where "form" means the syntactic pattern to be detected.
//
#define VOID_T_ADD_F1(TT, TN)           void_t<decltype(std::declval<typename TT::TN>())>
#define VOID_T_ADD_F2(TT, X1, X2, TN)   void_t<decltype(std::declval<typename TT::template TN<X1, X2>>())>
#define VOID_T_ADD_F3(TT, X1, X2, TN)   void_t<decltype(std::declval<typename TT::template TN<TT, X1, X2>>())>

namespace STD_LA {
//==================================================================================================
//                        **** ELEMENT ADDITION TRAITS AND DETECTORS ****
//==================================================================================================
//
//- This traits type provides the default mechanism for determining the result of adding two
//  elements of possibly different types.
//
template<class T1, class T2>
struct matrix_element_addition_traits
{
    using traits_category = matrix_element_addition_traits_tag;
    using element_type    = decltype(declval<T1>() + declval<T2>());
};

//- Alias interface to trait.
//
template<class T1, class T2>
using matrix_element_addition_t = typename matrix_element_addition_traits<T1, T2>::element_type;


namespace detail {
//--------------------------------------------------------------------------------------------------
//- Form 1 type detection of nested element addition traits.
//
template<typename OT, typename = void>
struct detect_element_add_traits_f1
:   public false_type
{
    using type = void;
};

template<typename OT>
struct detect_element_add_traits_f1<OT, VOID_T_ADD_F1(OT, element_addition_traits)>
:   public true_type
{
    using type = typename OT::element_addition_traits;
};


//----------------------------------------------------------
//- Form 2 type detection of nested element addition traits.
//
template<typename OT, typename T1, typename T2, typename = void>
struct detect_element_add_traits_f2
:   public false_type
{
    using type = void;
};

template<typename OT, typename T1, typename T2>
struct detect_element_add_traits_f2<OT, T1, T2, VOID_T_ADD_F2(OT, T1, T2, element_addition_traits)>
:   public true_type
{
    using type = typename OT::template element_addition_traits<T1, T2>;
};


//---------------------------------------------
//- Element addition traits type determination.
//
template<typename OT, typename T1, typename T2>
struct element_add_traits_chooser
{
    using CT1 = typename detect_element_add_traits_f1<OT>::type;
    using CT2 = typename detect_element_add_traits_f2<OT, T1, T2>::type;
    using DEF = matrix_element_addition_traits<T1, T2>;

    using type = typename non_void_traits_chooser<CT1, CT2, DEF>::type;
};

template<typename OT, typename T1, typename T2>
using element_add_traits_t = typename element_add_traits_chooser<OT, T1, T2>::type;

template<class OT, class T1, class T2>
constexpr bool  has_element_add_traits_v = detect_element_add_traits_f2<OT, T1, T2>::value ||
                                           detect_element_add_traits_f1<OT>::value;


//---------------------------------------------
//- Element addition result type determination.
//
template<typename OT, typename T1, typename T2>
struct element_add_type
{
    using traits_type  = typename element_add_traits_chooser<OT, T1, T2>::type;
    using element_type = typename traits_type::element_type;
};

template<typename OT, typename T1, typename T2>
using element_add_type_t = typename element_add_type<OT, T1, T2>::element_type;


}       //- detail namespace
//==================================================================================================
//                         **** ENGINE ADDITION TRAITS AND DETECTORS ****
//==================================================================================================
//
//- This traits type performs engine promotion type computations for binary addition.
//
template<class OT, class ET1, class ET2>
struct matrix_engine_addition_traits
{
    static_assert(ET1::is_matrix::value == ET2::is_matrix::value  &&
                  ET1::is_vector::value == ET2::is_vector::value  &&
                  ET1::is_matrix::value != ET2::is_vector::value);

    using traits_category = matrix_engine_addition_traits_tag;
    using element_type_1  = typename ET1::element_type;
    using element_type_2  = typename ET2::element_type;
    using element_type    = detail::element_add_type_t<OT, element_type_1, element_type_2>;
    using engine_type     = conditional_t<ET1::is_matrix::value,
                                          dr_matrix_engine<element_type, allocator<element_type>>,
                                          dr_vector_engine<element_type, allocator<element_type>>>;
};


//- Note that all cases where allocators are rebound assume standard-conformant allocator types.
//
template<class OT, class T1, class A1, class T2, class A2>
struct matrix_engine_addition_traits<OT, dr_vector_engine<T1,A1>, dr_vector_engine<T2,A2>>
{
    using traits_category = matrix_engine_addition_traits_tag;
    using element_type    = detail::element_add_type_t<OT, T1, T2>;
    using alloc_type      = detail::rebind_alloc_t<A1, element_type>;
    using engine_type     = dr_vector_engine<element_type, alloc_type>;
};

template<class OT, class T1, class A1, class T2, int32_t N2>
struct matrix_engine_addition_traits<OT, dr_vector_engine<T1, A1>, fs_vector_engine<T2, N2>>
{
    using traits_category = matrix_engine_addition_traits_tag;
    using element_type    = detail::element_add_type_t<OT, T1, T2>;
    using alloc_type      = detail::rebind_alloc_t<A1, element_type>;
    using engine_type     = dr_vector_engine<element_type, alloc_type>;
};

template<class OT, class T1, int32_t N1, class T2, class A2>
struct matrix_engine_addition_traits<OT, fs_vector_engine<T1, N1>, dr_vector_engine<T2, A2>>
{
    using traits_category = matrix_engine_addition_traits_tag;
    using element_type    = detail::element_add_type_t<OT, T1, T2>;
    using alloc_type      = detail::rebind_alloc_t<A2, element_type>;
    using engine_type     = dr_vector_engine<element_type, alloc_type>;
};

template<class OT, class T1, int32_t N1, class T2, int32_t N2>
struct matrix_engine_addition_traits<OT, fs_vector_engine<T1, N1>, fs_vector_engine<T2, N2>>
{
    static_assert(N1 == N2);
    using traits_category = matrix_engine_addition_traits_tag;
    using element_type    = detail::element_add_type_t<OT, T1, T2>;
    using engine_type     = fs_vector_engine<element_type, N1>;
};

//------
//
template<class OT, class T1, class A1, class T2, class A2>
struct matrix_engine_addition_traits<OT, dr_matrix_engine<T1,A1>, dr_matrix_engine<T2,A2>>
{
    using traits_category = matrix_engine_addition_traits_tag;
    using element_type    = detail::element_add_type_t<OT, T1, T2>;
    using alloc_type      = detail::rebind_alloc_t<A1, element_type>;
    using engine_type     = dr_matrix_engine<element_type, alloc_type>;
};

template<class OT, class T1, class A1, class T2, int32_t R2, int32_t C2>
struct matrix_engine_addition_traits<OT, dr_matrix_engine<T1, A1>, fs_matrix_engine<T2, R2, C2>>
{
    using traits_category = matrix_engine_addition_traits_tag;
    using element_type    = detail::element_add_type_t<OT, T1, T2>;
    using alloc_type      = detail::rebind_alloc_t<A1, element_type>;
    using engine_type     = dr_matrix_engine<element_type, alloc_type>;
};

template<class OT, class T1, int32_t R1, int32_t C1, class T2, class A2>
struct matrix_engine_addition_traits<OT, fs_matrix_engine<T1, R1, C1>, dr_matrix_engine<T2, A2>>
{
    using traits_category = matrix_engine_addition_traits_tag;
    using element_type    = detail::element_add_type_t<OT, T1, T2>;
    using alloc_type      = detail::rebind_alloc_t<A2, element_type>;
    using engine_type     = dr_matrix_engine<element_type, alloc_type>;
};

template<class OT, class T1, int32_t R1, int32_t C1, class T2, int32_t R2, int32_t C2>
struct matrix_engine_addition_traits<OT, fs_matrix_engine<T1, R1, C1>, fs_matrix_engine<T2, R2, C2>>
{
    static_assert(R1 == R2);
    static_assert(C1 == C2);
    using traits_category = matrix_engine_addition_traits_tag;
    using element_type    = detail::element_add_type_t<OT, T1, T2>;
    using engine_type     = fs_matrix_engine<element_type, R1, C1>;
};


//- Transpose cases for matrices.
//
template<class OT, class ET1, class ET2>
struct matrix_engine_addition_traits<OT, tr_matrix_engine<ET1>, ET2>
{
    using traits_category = matrix_engine_addition_traits_tag;
    using engine_type     = typename matrix_engine_addition_traits<OT, ET1, ET2>::engine_type;
};

template<class OT, class ET1, class ET2>
struct matrix_engine_addition_traits<OT, ET1, tr_matrix_engine<ET2>>
{
    using traits_category = matrix_engine_addition_traits_tag;
    using engine_type     = typename matrix_engine_addition_traits<OT, ET1, ET2>::engine_type;
};

template<class OT, class ET1, class ET2>
struct matrix_engine_addition_traits<OT, tr_matrix_engine<ET1>, tr_matrix_engine<ET2>>
{
    using traits_category = matrix_engine_addition_traits_tag;
    using engine_type     = typename matrix_engine_addition_traits<OT, ET1, ET2>::engine_type;
};

template<class OT, class T1, int32_t R1, int32_t C1, class T2, int32_t R2, int32_t C2>
struct matrix_engine_addition_traits<OT,
                                     tr_matrix_engine<fs_matrix_engine<T1, R1, C1>>,
                                     fs_matrix_engine<T2, R2, C2>>
{
    static_assert(R1 == C2);
    static_assert(C1 == R2);

    using traits_category = matrix_engine_addition_traits_tag;
    using element_type    = detail::element_add_type_t<OT, T1, T2>;
    using engine_type     = fs_matrix_engine<element_type, R2, C2>;
};

template<class OT, class T1, int32_t R1, int32_t C1, class T2, int32_t R2, int32_t C2>
struct matrix_engine_addition_traits<OT,
                                     fs_matrix_engine<T1, R1, C1>,
                                     tr_matrix_engine<fs_matrix_engine<T2, R2, C2>>>
{
    static_assert(R1 == C2);
    static_assert(C1 == R2);
    using traits_category = matrix_engine_addition_traits_tag;
    using element_type    = detail::element_add_type_t<OT, T1, T2>;
    using engine_type     = fs_matrix_engine<element_type, R1, C1>;
};

template<class OT, class T1, int32_t R1, int32_t C1, class T2, int32_t R2, int32_t C2>
struct matrix_engine_addition_traits<OT,
                                     tr_matrix_engine<fs_matrix_engine<T1, R1, C1>>,
                                     tr_matrix_engine<fs_matrix_engine<T2, R2, C2>>>
{
    static_assert(R1 == R2);
    static_assert(C1 == C2);
    using traits_category = matrix_engine_addition_traits_tag;
    using element_type    = detail::element_add_type_t<OT, T1, T2>;
    using engine_type     = fs_matrix_engine<element_type, C1, R1>;
};

//- Alias interface to trait.
//
template<class OT, class ET1, class ET2>
using matrix_engine_addition_t = typename matrix_engine_addition_traits<OT, ET1, ET2>::engine_type;


namespace detail {
//--------------------------------------------------------------------------------------------------
//- Form 1 type detection of nested engine addition traits.
//
template<typename OT, typename = void>
struct detect_engine_add_traits_f1
:   public false_type
{
    using type = void;
};

template<typename OT>
struct detect_engine_add_traits_f1<OT, VOID_T_ADD_F1(OT, engine_addition_traits)>
:   public true_type
{
    using type = typename OT::engine_addition_traits;
};


//---------------------------------------------------------
//- Form 3 type detection of nested engine addition traits.
//
template<typename OT, typename ET1, typename ET2, typename = void>
struct detect_engine_add_traits_f3
:   public false_type
{
    using type = void;
};

template<typename OT, typename ET1, typename ET2>
struct detect_engine_add_traits_f3<OT, ET1, ET2, VOID_T_ADD_F3(OT, ET1, ET2, engine_addition_traits)>
:   public true_type
{
    using type = typename OT::template engine_addition_traits<OT, ET1, ET2>;
};


//--------------------------------------------
//- Engine addition traits type determination.
//
template<typename OT, typename ET1, typename ET2>
struct engine_add_traits_chooser
{
    using CT1 = typename detect_engine_add_traits_f1<OT>::type;
    using CT2 = typename detect_engine_add_traits_f3<OT, ET1, ET2>::type;
    using DEF = matrix_engine_addition_traits<OT, ET1, ET2>;

    using type = typename non_void_traits_chooser<CT1, CT2, DEF>::type;
};

template<typename OT, typename ET1, typename ET2>
using engine_add_traits_t = typename engine_add_traits_chooser<OT, ET1, ET2>::type;

template<class OT, class ET1, class ET2>
constexpr bool  has_engine_add_traits_v = detect_engine_add_traits_f3<OT, ET1, ET2>::value ||
                                          detect_engine_add_traits_f1<OT>::value;


//--------------------------------------------
//- Engine addition result type determination.
//
template<typename OT, typename ET1, typename ET2>
struct engine_add_type
{
    using traits_type = typename engine_add_traits_chooser<OT, ET1, ET2>::type;
    using engine_type = typename traits_type::engine_type;
};

template<typename OT, typename ET1, typename ET2>
using engine_add_type_t = typename engine_add_type<OT, ET1, ET2>::engine_type;


}       //- detail namespace
//==================================================================================================
//                      **** ADDITION ARITHMETIC TRAITS AND DETECTORS ****
//==================================================================================================
//
//- This traits type actually performs binary addition.
//
template<class OTR, class ET1, class OT1, class ET2, class OT2>
struct matrix_addition_traits<OTR, vector<ET1, OT1>, vector<ET2, OT2>>
{
    using category    = matrix_addition_traits_tag;
    using engine_type = detail::engine_add_type_t<OTR, ET1, ET2>;
    using op_traits   = OTR;
    using result_type = vector<engine_type, op_traits>;

    static result_type  add(vector<ET1, OT1> const& v1, vector<ET2, OT2> const& v2);
};

template<class OTR, class ET1, class OT1, class ET2, class OT2>
auto
matrix_addition_traits<OTR, vector<ET1, OT1>, vector<ET2, OT2>>::add
(vector<ET1, OT1> const& v1, vector<ET2, OT2> const& v2) -> result_type
{
    PrintOperandTypes<result_type>("addition_traits", v1, v2);
    return result_type();
}


//------
//
template<class OTR, class ET1, class OT1, class ET2, class OT2>
struct matrix_addition_traits<OTR, matrix<ET1, OT1>, matrix<ET2, OT2>>
{
    using category    = matrix_addition_traits_tag;
    using engine_type = detail::engine_add_type_t<OTR, ET1, ET2>;
    using op_traits   = OTR;
    using result_type = matrix<engine_type, op_traits>;

    static result_type  add(matrix<ET1, OT1> const& m1, matrix<ET2, OT2> const& m2);
};

template<class OTR, class ET1, class OT1, class ET2, class OT2>
auto
matrix_addition_traits<OTR, matrix<ET1, OT1>, matrix<ET2, OT2>>::add
(matrix<ET1, OT1> const& m1, matrix<ET2, OT2> const& m2) -> result_type
{
    PrintOperandTypes<result_type>("addition_traits", m1, m2);
    return result_type();
}


namespace detail {
//--------------------------------------------------------------------------------------------------
//- Form 1 type detection of nested addition arithmetic traits.
//
template<typename OT, typename = void>
struct detect_add_traits_f1
:   public false_type
{
    using type = void;
};

template<typename OT>
struct detect_add_traits_f1<OT, VOID_T_ADD_F1(OT, addition_traits)>
:   public true_type
{
    using type = typename OT::addition_traits;
};


//-------------------------------------------------------------
//- Form 3 type detection of nested addition arithmetic traits.
//
template<typename OT, typename OP1, typename OP2, typename = void>
struct detect_add_traits_f3
:   public false_type
{
    using type = void;
};

template<typename OT, typename OP1, typename OP2>
struct detect_add_traits_f3<OT, OP1, OP2, VOID_T_ADD_F3(OT, OP1, OP1, addition_traits)>
:   public true_type
{
    using type = typename OT::template addition_traits<OT, OP1, OP2>;
};


//------------------------------------------------
//- Addition arithmetic traits type determination.
//
template<typename OT, typename OP1, typename OP2>
struct add_traits_chooser
{
    using CT1 = typename detect_add_traits_f1<OT>::type;
    using CT2 = typename detect_add_traits_f3<OT, OP1, OP2>::type;
    using DEF = matrix_addition_traits<OT, OP1, OP2>;

    using type = typename non_void_traits_chooser<CT1, CT2, DEF>::type;
};

template<typename OT, typename OP1, typename OP2>
using addition_traits_t = typename add_traits_chooser<OT, OP1, OP2>::type;

template<class OT, class OP1, class OP2>
constexpr bool  has_add_traits_v = detect_add_traits_f3<OT, OP1, OP2>::value ||
                                   detect_add_traits_f1<OT>::value;

}       //- detail namespace
}       //- STD_LA namespace

#undef  VOID_T_ADD_F3
#undef  VOID_T_ADD_F2
#undef  VOID_T_ADD_F1

#endif  //- LINEAR_ALGEBRA_ADDITION_TRAITS_HPP_DEFINED
