//==================================================================================================
//  File:       debug_helpers.hpp
//
//  Summary:    This header contains miscellaneous functions and macros used in the development
//              of the library; nothing in this header is intended for standardization.
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_DEBUG_HELPERS_HPP_DEFINED
#define LINEAR_ALGEBRA_DEBUG_HELPERS_HPP_DEFINED

#include <iostream>
#include <iomanip>
#include <string_view>

namespace STD_LA {
USING_STD

//#define EXEC_OP_TEST_OUTPUT

template <class T>
constexpr string_view
type_name()                //- From StackOverflow...
{
    using namespace std;
#ifdef __clang__
    string_view p = __PRETTY_FUNCTION__;
    return string_view(p.data() + 59, p.size() - 59 - 1);
#elif defined(__GNUC__)
    string_view p = __PRETTY_FUNCTION__;
    # if __cplusplus < 201402
        return string_view(p.data() + 36, p.size() - 36 - 1);
    #else
        auto    n1 = p.find('[', 0) + 10;
        auto    n2 = p.find(';', n1);
        return string_view(p.data() + n1, n2 - n1);
    #endif
#elif defined(_MSC_VER)
    string_view p = __FUNCSIG__;
    #if (_MSC_VER >= 1920)          //- VS 2019
        return string_view(p.data() + 74, p.size() - 74 - 7);
    #elif (_MSC_VER >= 1910)        //- VS 2017
        return string_view(p.data() + 88, p.size() - 88 - 7);
    #else
        #error this compiler not supported
    #endif
#endif
}

#define MATRIX_STRINGIFY_H(X)   #X
#define MATRIX_STRINGIFY(X)     MATRIX_STRINGIFY_H(X)
#ifdef _MSC_VER

template<class C, class T, class A>
basic_string<C,T,A>
clean_type_name(basic_string<C,T,A> tname)
{
    static basic_string<C,T,A> const   cl = "class ";
    static basic_string<C,T,A> const   st = "struct ";
    static basic_string<C,T,A> const   ns = MATRIX_STRINGIFY(STD_LA) "::";
    static basic_string<C,T,A> const   sl = "std::";
    static basic_string<C,T,A> const   aa = "> >";
    static basic_string<C,T,A> const   tn = "type_name<";

    for (auto pos = string::npos;  (pos = tname.rfind(cl, pos)) != string::npos; )
    {
        tname.erase(pos, cl.size());
    }

    for (auto pos = string::npos;  (pos = tname.rfind(st, pos)) != string::npos; )
    {
        tname.erase(pos, st.size());
    }

    for (auto pos = string::npos;  (pos = tname.rfind(ns, pos)) != string::npos; )
    {
        tname.erase(pos, ns.size());
    }

    for (auto pos = string::npos;  (pos = tname.rfind(sl, pos)) != string::npos; )
    {
        tname.erase(pos, sl.size());
    }

    for (auto pos = string::npos;  (pos = tname.rfind(aa, pos)) != string::npos; )
    {
        tname.replace(pos, 3u, ">>");
    }

    for (auto pos = string::npos;  (pos = tname.rfind(tn, pos)) != string::npos; )
    {
        tname.erase(pos, tn.size());
    }
    return tname;
}

#else

template<class C, class T, class A>
basic_string<C,T,A>
clean_type_name(basic_string<C,T,A> tname)
{
    static basic_string<C,T,A> const   ns = MATRIX_STRINGIFY(STD_LA) "::";
    static basic_string<C,T,A> const   sl = "std::";
    static basic_string<C,T,A> const   ex = "experimental::";
    static basic_string<C,T,A> const   aa = "> >";

    for (auto pos = string::npos;  (pos = tname.rfind(ns, pos)) != string::npos; )
    {
        tname.erase(pos, ns.size());
    }

    for (auto pos = string::npos;  (pos = tname.rfind(sl, pos)) != string::npos; )
    {
        tname.erase(pos, sl.size());
    }

    for (auto pos = string::npos;  (pos = tname.rfind(ex, pos)) != string::npos; )
    {
        tname.erase(pos, ex.size());
    }

    for (auto pos = string::npos;  (pos = tname.rfind(aa, pos)) != string::npos; )
    {
        tname.replace(pos, 3u, ">>");
    }

    return tname;
}

#endif
#undef MATRIX_STRINGIFY
#undef MATRIX_STRINGIFY_H

inline void
PrintFuncName(char const* name)
{
    cout << "--------------------------------------------------------------------------------\n";
    cout << "--------------------------------------------------------------------------------\n";
    cout << "    In test function: " << name << endl;
}

#define PRINT_FNAME()   STD_LA::PrintFuncName(__func__)

template<class T>
string
get_type_name()
{
    auto    view = type_name<T>();
    return clean_type_name(string(view.data(), view.size()));
//    return string(view.data(), view.size());
}

template<class T>
string
get_type_name(T const&)
{
    auto    view = type_name<T>();
    return clean_type_name(string(view.data(), view.size()));
}

template<class RT, class O1>
void
PrintOperandTypes(string const& loc, O1 const& o1)
{
    cout << "in " << loc << endl
         << "  op1: " << get_type_name(o1) << endl
         << "  ret: " << get_type_name<RT>() << endl << endl;
}

template<class RT, class O1, class O2>
void
PrintOperandTypes(string const& loc, O1 const& o1, O2 const& o2)
{
    cout << "in " << loc << endl
         << "  op1: " << get_type_name(o1) << endl
         << "  op2: " << get_type_name(o2) << endl
         << "  ret: " << get_type_name<RT>() << endl << endl;
}

#define PRINT_TYPE(T)       std::cout << #T << ": " << STD_LA::get_type_name<T>() << std::endl


template<class ET, class OT>
void
Print(matrix<ET, OT> const& m, char const* pname = nullptr)
{
    using size_type = typename matrix<ET, OT>::size_type;

    cout << endl << "matrix: " << ((pname) ? pname : "<anon>") << endl;
    cout << "  size: " << m.rows() << "x" << m.columns() << endl;
    cout << "  capy: " << m.row_capacity() << "x" << m.column_capacity() << endl;
    cout << "  -----" << endl;

    for (size_type i = 0;  i < m.rows();  ++i)
    {
        cout << right << setw(4) << setprecision(3) << (double) m(i, 0);

        for (size_type j = 1;  j < m.columns();  ++j)
        {
             cout << right << setw(6) << setprecision(3) << (double) m(i, j);
        }

        cout << endl;
    }
}

template<class ET, class OT>
void
Print(vector<ET, OT> const& v, char const* pname = nullptr)
{
    using size_type = typename vector<ET, OT>::size_type;

    cout << endl << "vector: " << ((pname) ? pname : "<anon>") << endl;
    cout << "  size: " << v.size() << endl;
    cout << "  capy: " << v.capacity() << endl;
    cout << "  -----" << endl;

    cout << "(idx) " << right << setw(4) << setprecision(3) << (double) v(0);

    for (size_type i = 1;  i < v.size();  ++i)
    {
            cout << right << setw(6) << setprecision(3) << (double) v(i);
    }
    cout << endl;

    auto    iter = cbegin(v);
    auto    last = cend(v);

    cout << "(itr) " << right << setw(4) << setprecision(3) << (double) *iter;

    for (++iter;  iter != last;  ++iter)
    {
            cout << right << setw(6) << setprecision(3) << (double) *iter;
    }
    cout << endl;
}

#ifdef LA_USE_MDSPAN

template<class T, ptrdiff_t X0, ptrdiff_t X1, class L, class A>
void
Print(basic_mdspan<T, extents<X0, X1>, L, A> const& s, char const* pname = nullptr)
{
    using index_type = ptrdiff_t;

    cout << endl << "mdspan: " << ((pname) ? pname : "<anon>") << endl;
    cout << "  size: " << s.extent(0) << "x" << s.extent(1) << endl;
    cout << "  -----" << endl;

    for (index_type i = 0;  i < s.extent(0);  ++i)
    {
        std::cout << std::right << std::setw(4) << std::setprecision(3) << (double) s(i, 0);

        for (index_type j = 1;  j < s.extent(1);  ++j)
        {
             std::cout << std::right << std::setw(6) << std::setprecision(3) << (double) s(i, j);
        }
        cout << endl;
    }
}

template<class T, ptrdiff_t X0, class L, class A>
void
Print(basic_mdspan<T, extents<X0>, L, A> const& s, char const* pname = nullptr)
{
    using index_type = ptrdiff_t;

    cout << endl << "mdspan: " << ((pname) ? pname : "<anon>") << endl;
    cout << "  size: " << s.extent(0) << endl;
    cout << "  -----" << endl;

    cout << "(idx) " << right << setw(4) << setprecision(3) << (double) s(0);

    for (index_type i = 1;  i < s.extent(0);  ++i)
    {
         std::cout << std::right << std::setw(6) << std::setprecision(3) << (double) s(i);
    }
    cout << endl;
}

#endif  //- LA_USE_MDSPAN

inline void
Print(bool b, char const* pname = nullptr)
{
    cout << endl << "bool: " << ((pname) ? pname : "<anon>") << b << endl;
}

#define PRINT(X)    Print(X, #X)

template<class ET, class OT>
void
Fill(vector<ET, OT>& v)
{
    using size_type    = typename vector<ET, OT>::size_type;
    using element_type = typename vector<ET, OT>::element_type;

    element_type    x = 1;

    for (size_type i = 0;  i < v.size();  ++i)
    {
         v(i) = x;  x = x + 1;
    }
}

template<class ET, class OT>
void
Fill(matrix<ET, OT>& m)
{
    using size_type    = typename STD_LA::matrix<ET, OT>::size_type;
    using element_type = typename STD_LA::matrix<ET, OT>::element_type;

    element_type    x = 1;

    for (size_type i = 0;  i < m.rows();  ++i)
    {
        for (size_type j = 0;  j < m.columns();  ++j)
        {
            m(i, j) = x;  x = x + 1;
        }
    }
}

}   //- STD_LA namespace

#endif
