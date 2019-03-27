#ifndef LINEAR_ALGEBRA_DEBUG_HELPERS_HPP_DEFINED
#define LINEAR_ALGEBRA_DEBUG_HELPERS_HPP_DEFINED

//-------------------------------------------------------------------------------------------------
//  Helper functions and utilities for testing the interface; not part of the final proposal!
//-------------------------------------------------------------------------------------------------
//
#include <iostream>

namespace STD_LA {

using std::cout;    //- Yes, we're cheating, but only for convenience, we promise....
using std::endl;

template <class T>
constexpr std::string_view
type_name()                //- Thanks to Howard Hinnant for this!
{
    using namespace std;
#ifdef __clang__
    string_view p = __PRETTY_FUNCTION__;
    return string_view(p.data() + 34, p.size() - 34 - 1);
#elif defined(__GNUC__)
    string_view p = __PRETTY_FUNCTION__;
    # if __cplusplus < 201402
        return string_view(p.data() + 36, p.size() - 36 - 1);
    #else
        return string_view(p.data() + 49, p.find(';', 49) - 49);
    #endif
#elif defined(_MSC_VER)
    string_view p = __FUNCSIG__;
    #if (_MSC_VER >= 1920)          //- VS 2019
        return string_view(p.data() + 88, p.size() - 88 - 7);
    #elif (_MSC_VER >= 1910)        //- VS 2017
        return string_view(p.data() + 88, p.size() - 88 - 7);
    #else
        #error this compiler not supported
    #endif
#endif
}

#define STRINGIFY_H(X)  #X
#define STRINGIFY(X)    STRINGIFY_H(X)
#ifdef _MSC_VER

template<class C, class T, class A>
std::basic_string<C,T,A>
clean_type_name(std::basic_string<C,T,A> tname)
{
    static std::basic_string<C,T,A> const   cl = "class ";
    static std::basic_string<C,T,A> const   st = "struct ";
    static std::basic_string<C,T,A> const   ns = STRINGIFY(STD_LA) "::";
    static std::basic_string<C,T,A> const   aa = "> >";

    for (auto pos = std::string::npos;  (pos = tname.rfind(cl, pos)) != std::string::npos; )
    {
        tname.erase(pos, cl.size());
    }

    for (auto pos = std::string::npos;  (pos = tname.rfind(st, pos)) != std::string::npos; )
    {
        tname.erase(pos, st.size());
    }

    for (auto pos = std::string::npos;  (pos = tname.rfind(ns, pos)) != std::string::npos; )
    {
        tname.erase(pos, ns.size());
    }

    for (auto pos = std::string::npos;  (pos = tname.rfind(aa, pos)) != std::string::npos; )
    {
        tname.replace(pos, 3u, ">>");
    }

    return tname;
}

#else

template<class C, class T, class A>
std::basic_string<C,T,A>
clean_type_name(std::basic_string<C,T,A> tname)
{
    static std::basic_string<C,T,A> const   ns = STRINGIFY(STD_LA) "::";

    for (auto pos = std::string::npos;  (pos = tname.rfind(ns, pos)) != std::string::npos; )
    {
        tname.erase(pos, ns.size());
    }

    return tname;
}

#endif
#undef STRINGIFY
#undef STRINGIFY_H

template<class T>
std::string
get_type_name()
{
    auto    view = type_name<T>();
    return clean_type_name(std::string(view.data(), view.size()));
}

template<class T>
std::string
get_type_name(T const&)
{
    auto    view = type_name<T>();
    return clean_type_name(std::string(view.data(), view.size()));
}

#define PRINT_FN_NAME(name) \
    std::cout \
        << "********************************************************************************\n" \
        << "********************************************************************************\n" \
        << "entering test function " << #name << "()\n" << std::endl

}   //- STD_LA namespace

#endif
