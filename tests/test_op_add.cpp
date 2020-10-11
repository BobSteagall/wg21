#define ENABLE_TEST_PRINTING
#include "test_common.hpp"

using namespace STD_LA;
//using namespace MDSPAN_NS;

template<class OTR, class ET1, class ET2>
struct addition_engine_traits2
{
  private:
    static_assert(STD_LA::detail::similar_engines<ET1, ET2>);

    using element_type_1 = typename ET1::element_type;
    using element_type_2 = typename ET2::element_type;
    using element_traits = get_addition_element_traits_t<OTR, element_type_1, element_type_2>;

    template<bool is_matrix> struct mat_t;

    template<>
    struct mat_t<true>
    {
        //- Get the extents for each engine
        //
        static constexpr ptrdiff_t  R1 = STD_LA::detail::extent_helper<ET1>::rows();
        static constexpr ptrdiff_t  C1 = STD_LA::detail::extent_helper<ET1>::columns();
        static constexpr ptrdiff_t  R2 = STD_LA::detail::extent_helper<ET2>::rows();
        static constexpr ptrdiff_t  C3 = STD_LA::detail::extent_helper<ET2>::columns();

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
    };

    template<>
    struct mat_t<false>
    {
    };

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
get_extent_helper(Lambda)
{
    return Lambda{}();
}

inline constexpr
ptrdiff_t
get_extent_helper(...)
{
    return dynamic_extent;
}

template<class ET> inline constexpr
ptrdiff_t
get_column_extent()
{
    return get_extent_helper([]{ return static_cast<ptrdiff_t>(ET().columns()); });
}

template<class ET> inline constexpr
ptrdiff_t
get_row_extent()
{
    return get_extent_helper([]{ return static_cast<ptrdiff_t>(ET().rows()); });
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

    constexpr auto  xc5 = extent_helper<fmf_33::engine_type>::columns();
    constexpr auto  xc6 = extent_helper<gmf_44::engine_type>::columns();
    constexpr auto  xc7 = extent_helper<decltype(mr.t())::engine_type>::columns();
    constexpr auto  xc8 = extent_helper<dmf::engine_type>::columns();

    //constexpr gmf_33    m10 = LST_33_1;
}
