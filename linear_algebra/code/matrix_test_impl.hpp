#ifndef MATRIX_TEST_IMPL_HPP_DEFINED
#define MATRIX_TEST_IMPL_HPP_DEFINED

//- This file contains strawman implementations of various pieces-parts in order to:
//    1. Test the interfaces and make sure everything compiles
//    2. Build something that runs to verify the expected behavior at runtime.
//
//  The implementation code below is separated from its corresponding interface declarations
//  for the purposes of convenience while developing those interfaces.
//
#include "matrix_operators.hpp"

namespace STD_LA {
//=================================================================================================
//
//=================================================================================================
//
template<class T, size_t R, size_t C>
inline T
fs_matrix_engine<T,R,C>::operator ()(size_t i) const
{
    return ma_elems[i];
}

template<class T, size_t R, size_t C>
inline T
fs_matrix_engine<T,R,C>::operator ()(size_t i, size_t j) const
{
    return ma_elems[i*C + j];
}

template<class T, size_t R, size_t C>
inline T const*
fs_matrix_engine<T,R,C>::data() const noexcept
{
    return &ma_elems[0];
}

template<class T, size_t R, size_t C>
inline size_t
fs_matrix_engine<T,R,C>::columns() const noexcept
{
    return C;
}

template<class T, size_t R, size_t C>
inline size_t
fs_matrix_engine<T,R,C>::rows() const noexcept
{
    return R;
}

template<class T, size_t R, size_t C>
inline typename fs_matrix_engine<T,R,C>::size_tuple
fs_matrix_engine<T,R,C>::size() const noexcept
{
    return size_tuple(R, C);
}

template<class T, size_t R, size_t C>
inline size_t
fs_matrix_engine<T,R,C>::column_capacity() const noexcept
{
    return C;
}

template<class T, size_t R, size_t C>
inline size_t
fs_matrix_engine<T,R,C>::row_capacity() const noexcept
{
    return R;
}

template<class T, size_t R, size_t C>
inline typename fs_matrix_engine<T,R,C>::size_tuple
fs_matrix_engine<T,R,C>::capacity() const noexcept
{
    return size_tuple(R, C);
}

template<class T, size_t R, size_t C>
inline T&
fs_matrix_engine<T,R,C>::operator ()(size_t i)
{
    return ma_elems[i];
}

template<class T, size_t R, size_t C>
inline T&
fs_matrix_engine<T,R,C>::operator ()(size_t i, size_t j)
{
    return ma_elems[i*C + j];
}

template<class T, size_t R, size_t C>
inline T*
fs_matrix_engine<T,R,C>::data() noexcept
{
    return &ma_elems[0];
}

template<class T, size_t R, size_t C>
inline void
fs_matrix_engine<T,R,C>::swap_columns(size_t, size_t)
{}

template<class T, size_t R, size_t C>
inline void
fs_matrix_engine<T,R,C>::swap_rows(size_t, size_t)
{}


//=================================================================================================
//
//=================================================================================================
//
template<class T, class AT> inline
dr_matrix_engine<T,AT>::dr_matrix_engine()
{}

template<class T, class AT> inline
dr_matrix_engine<T,AT>::dr_matrix_engine(dr_matrix_engine&&)
{}

template<class T, class AT> inline
dr_matrix_engine<T,AT>::dr_matrix_engine(dr_matrix_engine const&)
{}

template<class T, class AT> inline
dr_matrix_engine<T,AT>::dr_matrix_engine(size_tuple)
{}

template<class T, class AT> inline
dr_matrix_engine<T,AT>::dr_matrix_engine(size_t, size_t)
{}

template<class T, class AT> inline
dr_matrix_engine<T,AT>::dr_matrix_engine(size_tuple, size_tuple)
{}

template<class T, class AT> inline
dr_matrix_engine<T,AT>::dr_matrix_engine(size_t, size_t, size_t, size_t)
{}

template<class T, class AT> inline
dr_matrix_engine<T,AT>&
dr_matrix_engine<T,AT>::operator =(dr_matrix_engine&&)
{
    return *this;
}

template<class T, class AT> inline
dr_matrix_engine<T,AT>&
dr_matrix_engine<T,AT>::operator =(dr_matrix_engine const&)
{
    return *this;
}

template<class T, class AT>
inline T
dr_matrix_engine<T,AT>::operator ()(size_t i) const
{
    return mp_bias[i];
}

template<class T, class AT>
inline T
dr_matrix_engine<T,AT>::operator ()(size_t i, size_t j) const
{
    return mp_bias[i*m_colcap + j];
}

template<class T, class AT>
inline T const*
dr_matrix_engine<T,AT>::data() const noexcept
{
    return &mp_elems[0];
}

template<class T, class AT>
inline size_t
dr_matrix_engine<T,AT>::columns() const noexcept
{
    return m_cols;
}

template<class T, class AT>
inline size_t
dr_matrix_engine<T,AT>::rows() const noexcept
{
    return m_rows;
}

template<class T, class AT>
inline typename dr_matrix_engine<T,AT>::size_tuple
dr_matrix_engine<T,AT>::size() const noexcept
{
    return size_tuple(m_rows, m_cols);
}

template<class T, class AT>
inline size_t
dr_matrix_engine<T,AT>::column_capacity() const noexcept
{
    return m_colcap;
}

template<class T, class AT>
inline size_t
dr_matrix_engine<T,AT>::row_capacity() const noexcept
{
    return m_rowcap;
}

template<class T, class AT>
inline typename dr_matrix_engine<T,AT>::size_tuple
dr_matrix_engine<T,AT>::capacity() const noexcept
{
    return size_tuple(m_rowcap, m_colcap);
}

template<class T, class AT>
inline T&
dr_matrix_engine<T,AT>::operator ()(size_t i)
{
    return mp_bias[i];
}

template<class T, class AT>
inline T&
dr_matrix_engine<T,AT>::operator ()(size_t i, size_t j)
{
    return mp_bias[i*m_colcap + j];
}

template<class T, class AT>
inline T*
dr_matrix_engine<T,AT>::data() noexcept
{
    return &mp_elems[0];
}

template<class T, class AT>
inline void
dr_matrix_engine<T,AT>::reserve(size_tuple)
{}

template<class T, class AT>
inline void
dr_matrix_engine<T,AT>::reserve(size_t, size_t)
{}

template<class T, class AT>
inline void
dr_matrix_engine<T,AT>::resize(size_tuple)
{}

template<class T, class AT>
inline void
dr_matrix_engine<T,AT>::resize(size_t, size_t)
{}

template<class T, class AT>
inline void
dr_matrix_engine<T,AT>::resize(size_tuple, size_tuple)
{}

template<class T, class AT>
inline void
dr_matrix_engine<T,AT>::resize(size_t, size_t, size_t, size_t)
{}

template<class T, class AT>
inline void
dr_matrix_engine<T,AT>::swap_columns(size_t, size_t)
{}

template<class T, class AT>
inline void
dr_matrix_engine<T,AT>::swap_rows(size_t, size_t)
{}


//=================================================================================================
//
//=================================================================================================
//
template<class ET> inline
matrix_transpose_engine<ET>::matrix_transpose_engine()
{}

template<class ET> inline
matrix_transpose_engine<ET>::matrix_transpose_engine(engine_type const&)
{}

template<class ET> inline
matrix_transpose_engine<ET>::matrix_transpose_engine(matrix_transpose_engine&&)
{}

template<class ET> inline
matrix_transpose_engine<ET>::matrix_transpose_engine(matrix_transpose_engine const&)
{}

template<class ET> inline
matrix_transpose_engine<ET>&
matrix_transpose_engine<ET>::operator =(matrix_transpose_engine&&)
{
    return *this;
}

template<class ET> inline
matrix_transpose_engine<ET>&
matrix_transpose_engine<ET>::operator =(matrix_transpose_engine const&)
{
    return *this;
}

template<class ET>
inline typename matrix_transpose_engine<ET>::element_type
matrix_transpose_engine<ET>::operator ()(size_t i) const
{
    return (*mp_other)[i];
}

template<class ET>
inline typename matrix_transpose_engine<ET>::element_type
matrix_transpose_engine<ET>::operator ()(size_t i, size_t j) const
{
    return (*mp_other)(j, i);
}

template<class ET>
inline typename matrix_transpose_engine<ET>::element_type const*
matrix_transpose_engine<ET>::data() const noexcept
{
    return mp_other->data();
}

template<class ET>
inline size_t
matrix_transpose_engine<ET>::columns() const noexcept
{
    return mp_other->rows();
}

template<class ET>
inline size_t
matrix_transpose_engine<ET>::rows() const noexcept
{
    return mp_other->columns();
}

template<class ET>
inline typename matrix_transpose_engine<ET>::size_tuple
matrix_transpose_engine<ET>::size() const noexcept
{
    return size_tuple(columns(), rows());
}

template<class ET>
inline size_t
matrix_transpose_engine<ET>::column_capacity() const noexcept
{
    return mp_other->row_capacity();
}

template<class ET>
inline size_t
matrix_transpose_engine<ET>::row_capacity() const noexcept
{
    return mp_other->column_capacity();
}

template<class ET>
inline typename matrix_transpose_engine<ET>::size_tuple
matrix_transpose_engine<ET>::capacity() const noexcept
{
    return size_tuple(column_capacity(), row_capacity());
}


//=================================================================================================
//
//=================================================================================================
//
template<class ET, class OT> inline
column_vector<ET,OT>::column_vector()
{}

template<class ET, class OT>
template<class ET2, class OT2> inline
column_vector<ET,OT>::column_vector(column_vector<ET2, OT2> const&)
{}

template<class ET, class OT> inline
column_vector<ET,OT>::column_vector(engine_type const&)
{}

template<class ET, class OT>
template<class ET2, enable_if_resizable_t<ET, ET2>> inline
column_vector<ET,OT>::column_vector(size_t)
{}

template<class ET, class OT>
template<class ET2, enable_if_resizable_t<ET, ET2>>
column_vector<ET,OT>::column_vector(size_t, size_t)
{}

template<class ET, class OT>
template<class ET2, class OT2> inline
column_vector<ET,OT>&
column_vector<ET,OT>::operator =(column_vector<ET2, OT2> const&)
{
    return *this;
}

template<class ET, class OT> inline
typename column_vector<ET,OT>::element_type
column_vector<ET,OT>::operator ()(size_t i) const
{
    return m_engine(i);
}

template<class ET, class OT> inline
typename column_vector<ET,OT>::element_type const*
column_vector<ET,OT>::data() const noexcept
{
    return m_engine.data();
}

template<class ET, class OT> inline
size_t
column_vector<ET,OT>::columns() const noexcept
{
    return m_engine.columns();
}

template<class ET, class OT> inline
size_t
column_vector<ET,OT>::rows() const noexcept
{
    return m_engine.rows();
}

template<class ET, class OT> inline
typename column_vector<ET,OT>::size_tuple
column_vector<ET,OT>::size() const noexcept
{
    return m_engine.size();
}

template<class ET, class OT> inline
size_t
column_vector<ET,OT>::column_capacity() const noexcept
{
    return m_engine.column_capacity();
}

template<class ET, class OT> inline
size_t
column_vector<ET,OT>::row_capacity() const noexcept
{
    return m_engine.row_capacity();
}

template<class ET, class OT> inline
typename column_vector<ET,OT>::size_tuple
column_vector<ET,OT>::capacity() const noexcept
{
    return m_engine.capacity();
}

template<class ET, class OT> inline
typename column_vector<ET,OT>::transpose_type
column_vector<ET,OT>::t() const
{
    return transpose_type(m_engine);
}

template<class ET, class OT> inline
typename column_vector<ET,OT>::hermitian_type
column_vector<ET,OT>::h() const
{
    if constexpr (is_complex_v<element_type>)
    {
        return hermitian_type();
    }
    else
    {
        return hermitian_type(m_engine);
    }
}

template<class ET, class OT> inline
typename column_vector<ET,OT>::element_type&
column_vector<ET,OT>::operator ()(size_t i)
{
    return m_engine(i);
}

template<class ET, class OT> inline
typename column_vector<ET,OT>::element_type*
column_vector<ET,OT>::data() noexcept
{
    return m_engine.data();
}

template<class ET, class OT>
template<class ET2, enable_if_resizable_t<ET, ET2>> inline
void
column_vector<ET,OT>::reserve(size_t)
{}

template<class ET, class OT>
template<class ET2, enable_if_resizable_t<ET, ET2>> inline
void
column_vector<ET,OT>::resize(size_t)
{}

template<class ET, class OT>
template<class ET2, enable_if_resizable_t<ET, ET2>> inline
void
column_vector<ET,OT>::resize(size_t, size_t)
{}

template<class ET, class OT> inline
void
column_vector<ET,OT>::swap_rows(size_t, size_t)
{}


//=================================================================================================
//
//=================================================================================================
//
template<class ET, class OT> inline
row_vector<ET,OT>::row_vector()
{}

template<class ET, class OT>
template<class ET2, class OT2> inline
row_vector<ET,OT>::row_vector(row_vector<ET2, OT2> const&)
{}

template<class ET, class OT>
template<class ET2, enable_if_resizable_t<ET, ET2>> inline
row_vector<ET,OT>::row_vector(size_t)
{}

template<class ET, class OT>
template<class ET2, enable_if_resizable_t<ET, ET2>>
row_vector<ET,OT>::row_vector(size_t, size_t)
{}

template<class ET, class OT> inline
row_vector<ET,OT>::row_vector(engine_type const&)
{}

template<class ET, class OT>
template<class ET2, class OT2> inline
row_vector<ET,OT>&
row_vector<ET,OT>::operator =(row_vector<ET2, OT2> const&)
{
    return *this;
}

template<class ET, class OT> inline
typename row_vector<ET,OT>::element_type
row_vector<ET,OT>::operator ()(size_t i) const
{
    return m_engine(i);
}

template<class ET, class OT> inline
typename row_vector<ET,OT>::element_type const*
row_vector<ET,OT>::data() const noexcept
{
    return m_engine.data();
}

template<class ET, class OT> inline
size_t
row_vector<ET,OT>::columns() const noexcept
{
    return m_engine.columns();
}

template<class ET, class OT> inline
size_t
row_vector<ET,OT>::rows() const noexcept
{
    return m_engine.rows();
}

template<class ET, class OT> inline
typename row_vector<ET,OT>::size_tuple
row_vector<ET,OT>::size() const noexcept
{
    return m_engine.size();
}

template<class ET, class OT> inline
size_t
row_vector<ET,OT>::column_capacity() const noexcept
{
    return m_engine.column_capacity();
}

template<class ET, class OT> inline
size_t
row_vector<ET,OT>::row_capacity() const noexcept
{
    return m_engine.row_capacity();
}

template<class ET, class OT> inline
typename row_vector<ET,OT>::size_tuple
row_vector<ET,OT>::capacity() const noexcept
{
    return m_engine.capacity();
}

template<class ET, class OT> inline
typename row_vector<ET,OT>::transpose_type
row_vector<ET,OT>::t() const
{
    return transpose_type(m_engine);
}

template<class ET, class OT> inline
typename row_vector<ET,OT>::hermitian_type
row_vector<ET,OT>::h() const
{
    if constexpr (is_complex_v<element_type>)
    {
        return hermitian_type();
    }
    else
    {
        return hermitian_type(m_engine);
    }
}

template<class ET, class OT> inline
typename row_vector<ET,OT>::element_type&
row_vector<ET,OT>::operator ()(size_t i)
{
    return m_engine(i);
}

template<class ET, class OT> inline
typename row_vector<ET,OT>::element_type*
row_vector<ET,OT>::data() noexcept
{
    return m_engine.data();
}

template<class ET, class OT>
template<class ET2, enable_if_resizable_t<ET, ET2>> inline
void
row_vector<ET,OT>::reserve(size_t)
{}

template<class ET, class OT>
template<class ET2, enable_if_resizable_t<ET, ET2>> inline
void
row_vector<ET,OT>::resize(size_t)
{}

template<class ET, class OT>
template<class ET2, enable_if_resizable_t<ET, ET2>> inline
void
row_vector<ET,OT>::resize(size_t, size_t)
{}

template<class ET, class OT> inline
void
row_vector<ET,OT>::swap_columns(size_t, size_t)
{}


//=================================================================================================
//
//=================================================================================================
//
template<class ET, class OT> inline
matrix<ET,OT>::matrix()
{}

template<class ET, class OT>
template<class ET2, class OT2> inline
matrix<ET,OT>::matrix(matrix<ET2, OT2> const&)
{}

template<class ET, class OT>
template<class ET2, enable_if_resizable_t<ET, ET2>> inline
matrix<ET,OT>::matrix(size_tuple)
{}

template<class ET, class OT>
template<class ET2, enable_if_resizable_t<ET, ET2>>
matrix<ET,OT>::matrix(size_t, size_t)
{}

template<class ET, class OT>
template<class ET2, enable_if_resizable_t<ET, ET2>> inline
matrix<ET,OT>::matrix(size_tuple, size_tuple)
{}

template<class ET, class OT>
template<class ET2, enable_if_resizable_t<ET, ET2>>
matrix<ET,OT>::matrix(size_t, size_t, size_t, size_t)
{}

template<class ET, class OT> inline
matrix<ET,OT>::matrix(engine_type const&)
{}

template<class ET, class OT>
template<class ET2, class OT2> inline
matrix<ET,OT>&
matrix<ET,OT>::operator =(matrix<ET2, OT2> const&)
{
    return *this;
}

template<class ET, class OT> inline
typename matrix<ET,OT>::element_type
matrix<ET,OT>::operator ()(size_t i, size_t j) const
{
    return m_engine(i, j);
}

template<class ET, class OT> inline
typename matrix<ET,OT>::element_type const*
matrix<ET,OT>::data() const noexcept
{
    return m_engine.data();
}

template<class ET, class OT> inline
size_t
matrix<ET,OT>::columns() const noexcept
{
    return m_engine.columns();
}

template<class ET, class OT> inline
size_t
matrix<ET,OT>::rows() const noexcept
{
    return m_engine.rows();
}

template<class ET, class OT> inline
typename matrix<ET,OT>::size_tuple
matrix<ET,OT>::size() const noexcept
{
    return m_engine.size();
}

template<class ET, class OT> inline
size_t
matrix<ET,OT>::column_capacity() const noexcept
{
    return m_engine.column_capacity();
}

template<class ET, class OT> inline
size_t
matrix<ET,OT>::row_capacity() const noexcept
{
    return m_engine.row_capacity();
}

template<class ET, class OT> inline
typename matrix<ET,OT>::size_tuple
matrix<ET,OT>::capacity() const noexcept
{
    return m_engine.capacity();
}

template<class ET, class OT> inline
typename matrix<ET,OT>::transpose_type
matrix<ET,OT>::t() const
{
    return transpose_type(m_engine);
}

template<class ET, class OT> inline
typename matrix<ET,OT>::hermitian_type
matrix<ET,OT>::h() const
{
    if constexpr (is_complex_v<element_type>)
    {
        return hermitian_type();
    }
    else
    {
        return hermitian_type(m_engine);
    }
}

template<class ET, class OT> inline
typename matrix<ET,OT>::element_type&
matrix<ET,OT>::operator ()(size_t i, size_t j)
{
    return m_engine(i, j);
}

template<class ET, class OT> inline
typename matrix<ET,OT>::element_type*
matrix<ET,OT>::data() noexcept
{
    return m_engine.data();
}

template<class ET, class OT>
template<class ET2, enable_if_resizable_t<ET, ET2>> inline
void
matrix<ET,OT>::reserve(size_tuple)
{}

template<class ET, class OT>
template<class ET2, enable_if_resizable_t<ET, ET2>> inline
void
matrix<ET,OT>::reserve(size_t, size_t)
{}

template<class ET, class OT>
template<class ET2, enable_if_resizable_t<ET, ET2>> inline
void
matrix<ET,OT>::resize(size_tuple)
{}

template<class ET, class OT>
template<class ET2, enable_if_resizable_t<ET, ET2>> inline
void
matrix<ET,OT>::resize(size_t, size_t)
{}

template<class ET, class OT>
template<class ET2, enable_if_resizable_t<ET, ET2>> inline
void
matrix<ET,OT>::resize(size_tuple, size_tuple)
{}

template<class ET, class OT>
template<class ET2, enable_if_resizable_t<ET, ET2>> inline
void
matrix<ET,OT>::resize(size_t, size_t, size_t, size_t)
{}

template<class ET, class OT> inline
void
matrix<ET,OT>::swap_columns(size_t, size_t)
{}

template<class ET, class OT> inline
void
matrix<ET,OT>::swap_rows(size_t, size_t)
{}


template<class RT, class O1>
void
PrintOperandTypes(std::string const& loc, O1 const& o1)
{
    cout << "in " << loc << endl
         << "  op1: " << get_type_name(o1) << endl
         << "  ret: " << get_type_name<RT>() << endl << endl;
}

template<class RT, class O1, class O2>
void
PrintOperandTypes(std::string const& loc, O1 const& o1, O2 const& o2)
{
    cout << "in " << loc << endl
         << "  op1: " << get_type_name(o1) << endl
         << "  op2: " << get_type_name(o2) << endl
         << "  ret: " << get_type_name<RT>() << endl << endl;
}

//=================================================================================================
//
//=================================================================================================
//
template<class ET1, class OT1, class OTR> inline
typename matrix_negation_traits<column_vector<ET1, OT1>, OTR>::result_type
matrix_negation_traits<column_vector<ET1, OT1>, OTR>::negate(column_vector<ET1,OT1> const& cv1)
{
    PrintOperandTypes<result_type>("negation_traits", cv1);
    return result_type();
}

template<class ET1, class OT1, class OTR> inline
typename matrix_negation_traits<row_vector<ET1, OT1>, OTR>::result_type
matrix_negation_traits<row_vector<ET1, OT1>, OTR>::negate(row_vector<ET1,OT1> const& rv1)
{
    PrintOperandTypes<result_type>("negation_traits", rv1);
    return result_type();
}

template<class ET1, class OT1, class OTR> inline
typename matrix_negation_traits<matrix<ET1, OT1>, OTR>::result_type
matrix_negation_traits<matrix<ET1, OT1>, OTR>::negate(matrix<ET1,OT1> const& m1)
{
    PrintOperandTypes<result_type>("negation_traits", m1);
    return result_type();
}


//=================================================================================================
//
//=================================================================================================
//
template<class ET1, class OT1, class ET2, class OT2, class OTR>
typename matrix_addition_traits<column_vector<ET1, OT1>, column_vector<ET2, OT2>, OTR>::result_type
matrix_addition_traits<column_vector<ET1, OT1>, column_vector<ET2, OT2>, OTR>::add
(column_vector<ET1, OT1> const& cv1, column_vector<ET2, OT2> const& cv2)
{
    PrintOperandTypes<result_type>("addition_traits", cv1, cv2);
    return result_type();
}

template<class ET1, class OT1, class ET2, class OT2, class OTR>
typename matrix_addition_traits<row_vector<ET1, OT1>, row_vector<ET2, OT2>, OTR>::result_type
matrix_addition_traits<row_vector<ET1, OT1>, row_vector<ET2, OT2>, OTR>::add
(row_vector<ET1, OT1> const& rv1, row_vector<ET2, OT2> const& rv2)
{
    PrintOperandTypes<result_type>("addition_traits", rv1, rv2);
    return result_type();
}

template<class ET1, class OT1, class ET2, class OT2, class OTR>
typename matrix_addition_traits<matrix<ET1, OT1>, matrix<ET2, OT2>, OTR>::result_type
matrix_addition_traits<matrix<ET1, OT1>, matrix<ET2, OT2>, OTR>::add
(matrix<ET1, OT1> const& m1, matrix<ET2, OT2> const& m2)
{
    PrintOperandTypes<result_type>("addition_traits", m1, m2);
    return result_type();
}


//=================================================================================================
//
//=================================================================================================
//
template<class ET1, class OT1, class ET2, class OT2, class OTR>
typename matrix_subtraction_traits<column_vector<ET1, OT1>, column_vector<ET2, OT2>, OTR>::result_type
matrix_subtraction_traits<column_vector<ET1, OT1>, column_vector<ET2, OT2>, OTR>::subtract
(column_vector<ET1, OT1> const& cv1, column_vector<ET2, OT2> const& cv2)
{
    PrintOperandTypes<result_type>("subtraction_traits", cv1, cv2);
    return result_type();
}

template<class ET1, class OT1, class ET2, class OT2, class OTR>
typename matrix_subtraction_traits<row_vector<ET1, OT1>, row_vector<ET2, OT2>, OTR>::result_type
matrix_subtraction_traits<row_vector<ET1, OT1>, row_vector<ET2, OT2>, OTR>::subtract
(row_vector<ET1, OT1> const& rv1, row_vector<ET2, OT2> const& rv2)
{
    PrintOperandTypes<result_type>("subtraction_traits", rv1, rv2);
    return result_type();
}

template<class ET1, class OT1, class ET2, class OT2, class OTR>
typename matrix_subtraction_traits<matrix<ET1, OT1>, matrix<ET2, OT2>, OTR>::result_type
matrix_subtraction_traits<matrix<ET1, OT1>, matrix<ET2, OT2>, OTR>::subtract
(matrix<ET1, OT1> const& m1, matrix<ET2, OT2> const& m2)
{
    PrintOperandTypes<result_type>("subtraction_traits", m1, m2);
    return result_type();
}


//=================================================================================================
//
//=================================================================================================
//
template<class ET1, class OT1, class T2, class OTR> inline
typename matrix_multiplication_traits<column_vector<ET1, OT1>, T2, OTR>::result_type
matrix_multiplication_traits<column_vector<ET1, OT1>, T2, OTR>::multiply
(column_vector<ET1, OT1> const& cv1, T2 s2)
{
    PrintOperandTypes<result_type>("multiplication_traits", cv1, s2);
    return result_type();
}

template<class T1, class ET2, class OT2, class OTR> inline
typename matrix_multiplication_traits<T1, column_vector<ET2, OT2>, OTR>::result_type
matrix_multiplication_traits<T1, column_vector<ET2, OT2>, OTR>::multiply
(T1 s1, column_vector<ET2, OT2> const& cv2)
{
    PrintOperandTypes<result_type>("multiplication_traits", s1, cv2);
    return result_type();
}

//------
//
template<class ET1, class OT1, class T2, class OTR> inline
typename matrix_multiplication_traits<row_vector<ET1, OT1>, T2, OTR>::result_type
matrix_multiplication_traits<row_vector<ET1, OT1>, T2, OTR>::multiply
(row_vector<ET1, OT1> const& rv1, T2 s2)
{
    PrintOperandTypes<result_type>("multiplication_traits", rv1, s2);
    return result_type();
}

template<class T1, class ET2, class OT2, class OTR> inline
typename matrix_multiplication_traits<T1, row_vector<ET2, OT2>, OTR>::result_type
matrix_multiplication_traits<T1, row_vector<ET2, OT2>, OTR>::multiply
(T1 s1, row_vector<ET2, OT2> const& rv2)
{
    PrintOperandTypes<result_type>("multiplication_traits", s1, rv2);
    return result_type();
}

//------
//
template<class ET1, class OT1, class T2, class OTR> inline
typename matrix_multiplication_traits<matrix<ET1, OT1>, T2, OTR>::result_type
matrix_multiplication_traits<matrix<ET1, OT1>, T2, OTR>::multiply(matrix<ET1, OT1> const& m1, T2 s2)
{
    PrintOperandTypes<result_type>("multiplication_traits", m1, s2);
    return result_type();
}

//------
//
template<class T1, class ET2, class OT2, class OTR> inline
typename matrix_multiplication_traits<T1, matrix<ET2, OT2>, OTR>::result_type
matrix_multiplication_traits<T1, matrix<ET2, OT2>, OTR>::multiply(T1 s1, matrix<ET2, OT2> const& m2)
{
    PrintOperandTypes<result_type>("multiplication_traits", s1, m2);
    return result_type();
}

//------
//
template<class ET1, class OT1, class ET2, class OT2, class OTR> inline
typename matrix_multiplication_traits<column_vector<ET1, OT1>, row_vector<ET2, OT2>, OTR>::result_type
matrix_multiplication_traits<column_vector<ET1, OT1>, row_vector<ET2, OT2>, OTR>::multiply
(column_vector<ET1, OT1> const& cv1, row_vector<ET2, OT2> const& rv2)
{
    PrintOperandTypes<result_type>("multiplication_traits", cv1, rv2);
    return result_type();
}

template<class ET1, class OT1, class ET2, class OT2, class OTR> inline
typename matrix_multiplication_traits<row_vector<ET1, OT1>, column_vector<ET2, OT2>, OTR>::result_type
matrix_multiplication_traits<row_vector<ET1, OT1>, column_vector<ET2, OT2>, OTR>::multiply
(row_vector<ET1, OT1> const& rv1, column_vector<ET2, OT2> const& cv2)
{
    PrintOperandTypes<result_type>("multiplication_traits", rv1, cv2);
    return result_type();
}

//------
//
template<class ET1, class OT1, class ET2, class OT2, class OTR> inline
typename matrix_multiplication_traits<matrix<ET1, OT1>, column_vector<ET2, OT2>, OTR>::result_type
matrix_multiplication_traits<matrix<ET1, OT1>, column_vector<ET2, OT2>, OTR>::multiply
(matrix<ET1, OT1> const& m1, column_vector<ET2, OT2> const& cv2)
{
    PrintOperandTypes<result_type>("multiplication_traits", m1, cv2);
    return result_type();
}

template<class ET1, class OT1, class ET2, class OT2, class OTR> inline
typename matrix_multiplication_traits<matrix<ET1, OT1>, row_vector<ET2, OT2>, OTR>::result_type
matrix_multiplication_traits<matrix<ET1, OT1>, row_vector<ET2, OT2>, OTR>::multiply
(matrix<ET1, OT1> const& m1, row_vector<ET2, OT2> const& rv2)
{
    PrintOperandTypes<result_type>("multiplication_traits", m1, rv2);
    return result_type();
}

//------
//
template<class ET1, class OT1, class ET2, class OT2, class OTR> inline
typename matrix_multiplication_traits<column_vector<ET1, OT1>, matrix<ET2, OT2>, OTR>::result_type
matrix_multiplication_traits<column_vector<ET1, OT1>, matrix<ET2, OT2>, OTR>::multiply
(column_vector<ET1, OT1> const& cv1, matrix<ET2, OT2> const& m2)
{
    PrintOperandTypes<result_type>("multiplication_traits", cv1, m2);
    return result_type();
}

template<class ET1, class OT1, class ET2, class OT2, class OTR> inline
typename matrix_multiplication_traits<row_vector<ET1, OT1>, matrix<ET2, OT2>, OTR>::result_type
matrix_multiplication_traits<row_vector<ET1, OT1>, matrix<ET2, OT2>, OTR>::multiply
(row_vector<ET1, OT1> const& rv1, matrix<ET2, OT2> const& m2)
{
    PrintOperandTypes<result_type>("multiplication_traits", rv1, m2);
    return result_type();
}

//------
//
template<class ET1, class OT1, class ET2, class OT2, class OTR> inline
typename matrix_multiplication_traits<matrix<ET1, OT1>, matrix<ET2, OT2>, OTR>::result_type
matrix_multiplication_traits<matrix<ET1, OT1>, matrix<ET2, OT2>, OTR>::multiply
(matrix<ET1, OT1> const& m1, matrix<ET2, OT2> const& m2)
{
    PrintOperandTypes<result_type>("multiplication_traits", m1, m2);
    return result_type();
}


//=================================================================================================
//
//=================================================================================================
//


//=================================================================================================
//
//=================================================================================================
//


//=================================================================================================
//
//=================================================================================================
//


//=================================================================================================
//
//=================================================================================================
//


//=================================================================================================
//
//=================================================================================================
//


//=================================================================================================
//
//=================================================================================================
//


//=================================================================================================
//
//=================================================================================================
//


//=================================================================================================
//
//=================================================================================================
//


//=================================================================================================
//
//=================================================================================================
//


//=================================================================================================
//
//=================================================================================================
//



}       //- STD_LA namespace
#endif  //- MATRIX_OPERATORS_HPP_DEFINED
