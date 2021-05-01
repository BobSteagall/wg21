#define ENABLE_TEST_PRINTING
#include "test_common.hpp"

using namespace STD_LA;
//using namespace MDSPAN_NS;

template<class ET1, class ET2, bool IsMatrix> struct engine_addition_extents_helper;

template<class ET1, class ET2>
struct engine_addition_extents_helper<ET1, ET2, true>
{
    //- Get the extents for each engine
    //
    static constexpr ptrdiff_t  R1 = STD_LA::detail::engine_extents_helper<ET1>::rows();
    static constexpr ptrdiff_t  C1 = STD_LA::detail::engine_extents_helper<ET1>::columns();
    static constexpr ptrdiff_t  R2 = STD_LA::detail::engine_extents_helper<ET2>::rows();
    static constexpr ptrdiff_t  C2 = STD_LA::detail::engine_extents_helper<ET2>::columns();

    //- Determine there are any dynamic row or column extents.
    //
    static constexpr bool   dyn_rows = ((R1 == dynamic_extent) || (R2 == dynamic_extent));
    static constexpr bool   dyn_cols = ((C1 == dynamic_extent) || (C2 == dynamic_extent));

    //- Validate the extents.
    //
    static_assert((R1 == R2 || dyn_rows), "mis-matched/invalid number of rows for addition");
    static_assert((C1 == C2 || dyn_cols), "mis-matched/invalid number of columns for addition");

    //- Decide on the new extents.
    //
    static constexpr ptrdiff_t  RR = (dyn_rows) ? dynamic_extent : R1;
    static constexpr ptrdiff_t  CR = (dyn_cols) ? dynamic_extent : C1;

    //- Specify the new extents type.
    //
    using extents_type = extents<RR, CR>;
};

template<class ET1, class ET2>
struct engine_addition_extents_helper<ET1, ET2, false>
{
    //- Get the extent for each engine.
    //
    static constexpr ptrdiff_t  S1 = STD_LA::detail::engine_extents_helper<ET1>::size();
    static constexpr ptrdiff_t  S2 = STD_LA::detail::engine_extents_helper<ET2>::size();

    //- Determine if the size extent is dynamic.
    //
    static constexpr bool   dyn_size = ((S1 == dynamic_extent) || (S2 == dynamic_extent));

    //- Validate the extents.
    //
    static_assert((S1 == S2 || dyn_size), "mis-matched/invalid size for addition");

    //- Decide on the new extent.
    //
    static constexpr ptrdiff_t  SR = (dyn_size) ? dynamic_extent : S1;

    //- Specify the new extents type.
    //
    using extents_type = extents<SR>;
};


template<class OTR, class ET1, class ET2>
struct addition_engine_traits2
{
  private:
    using element_type_1 = typename ET1::element_type;
    using element_type_2 = typename ET2::element_type;
    using element_traits = get_addition_element_traits_t<OTR, element_type_1, element_type_2>;

//    using extents_type = typename size_helper<STD_LA::detail::readable_matrix_engine<ET1>>::extents_type;


  public:
    using element_type = typename element_traits::element_type;
//    using engine_type  = conditional_t<readable_matrix_engine<ET1>,
//                                       dynamic_matrix_engine<element_type>,
//                                       dynamic_vector_engine<element_type>>;
};


using namespace STD_LA::detail;

using fmf_33 = fixed_size_matrix<float, 3, 3>;
using fmf_44 = fixed_size_matrix<float, 4, 4>;

using gmf_33 = general_matrix<float, 3, 3>;
using gmf_44 = general_matrix<float, 4, 4>;

using dmf    = dynamic_matrix<float>;

template<class Lambda, int = (Lambda{}(), 0)> inline constexpr
ptrdiff_t
get_engine_extents_helper(Lambda)
{
    return Lambda{}();
}

inline constexpr
ptrdiff_t
get_engine_extents_helper(...)
{
    return dynamic_extent;
}

template<class ET> inline constexpr
ptrdiff_t
get_column_extent()
{
    return get_engine_extents_helper([]{ return static_cast<ptrdiff_t>(ET().columns()); });
}

template<class ET> inline constexpr
ptrdiff_t
get_row_extent()
{
    return get_engine_extents_helper([]{ return static_cast<ptrdiff_t>(ET().rows()); });
}

TEST(Add, Constexpr)
{
    constexpr fmf_33    m1 = LST_33_1;
    constexpr fmf_33    m2 = LST_33_2;
    constexpr fmf_33    mr = m1 + m2;

    constexpr fmf_33    ms = mr + mr.t();

    PRINT(mr);
    PRINT(ms);

    static_assert(m1 == m1);
    static_assert(m1 != m2);

    constexpr auto  xc1 = get_column_extent<fmf_33::engine_type>();
    constexpr auto  xc2 = get_column_extent<gmf_44::engine_type>();
    constexpr auto  xc3 = get_column_extent<decltype(mr.t())::engine_type>();
    constexpr auto  xc4 = get_column_extent<dmf::engine_type>();

    constexpr auto  xc5 = engine_extents_helper<fmf_33::engine_type>::columns();
    constexpr auto  xc6 = engine_extents_helper<gmf_44::engine_type>::columns();
    constexpr auto  xc7 = engine_extents_helper<decltype(mr.t())::engine_type>::columns();
    constexpr auto  xc8 = engine_extents_helper<dmf::engine_type>::columns();

    //constexpr gmf_33    m10 = LST_33_1;
}
