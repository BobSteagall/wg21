//==================================================================================================
//  File:       matrix_storage_engine_support.hpp
//
//  Summary:    This header defines an owning, dense, rectangular engine that owns and manages
//              the elements of a matrix.
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_ENGINE_SUPPORT_HPP_DEFINED
#define LINEAR_ALGEBRA_ENGINE_SUPPORT_HPP_DEFINED

namespace STD_LA {
namespace detail {
//--------------------------------------------------------------------------------------------------
//  Concepts:   same_types<T, U>,  returns<T, U>,  convertible_element<T, U>
//
//  These are some simple utility concepts, used by the other concepts below, and perhaps
//  to be re-evaluated later.
//--------------------------------------------------------------------------------------------------
//
template<typename T, typename U>
concept same_types = is_same_v<T, U>;

template<typename T, typename U>
concept returns = is_same_v<T, U>;

template<typename T, typename U>
concept convertible_element = is_convertible_v<T, U>;


template<class ET>
concept has_engine_aliases =
    requires
    {
        typename ET::engine_category;
        typename ET::engine_interface;
        typename ET::element_layout;
        typename ET::value_type;
        typename ET::element_type;
        typename ET::index_type;
        typename ET::index_tuple_type;
        typename ET::reference;
        typename ET::const_reference;
    };

template<class ET>
concept readable_matrix_engine =
    has_engine_aliases<ET> and
    requires (ET const& eng, typename ET::index_type i)
    {
        { eng.columns() } -> returns<typename ET::index_type>;
        { eng.rows()    } -> returns<typename ET::index_type>;
        { eng(i, i)     } -> returns<typename ET::const_reference>;
    };


inline void
check_sizes(ptrdiff_t rows, ptrdiff_t cols)
{
    if (rows < 1  || cols < 1)
    {
        throw runtime_error("invalid size");
    }
}

inline void
check_capacities(ptrdiff_t rowcap, ptrdiff_t colcap)
{
    if (rowcap < 0  || colcap < 0)
    {
        throw runtime_error("invalid capacity");
    }
}

inline void
check_size(ptrdiff_t size)
{
    if (size < 1)
    {
        throw runtime_error("invalid size");
    }
}

inline void
check_capacity(ptrdiff_t capy)
{
    if (capy < 0)
    {
        throw runtime_error("invalid capacity");
    }
}


}       //- detail namespace
}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_ENGINE_SUPPORT_HPP_DEFINED
