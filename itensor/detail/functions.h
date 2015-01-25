//
// Distributed under the ITensor Library License, Version 1.2
//    (See accompanying LICENSE file.)
//
#ifndef __ITENSOR_FUNCTIONS_H
#define __ITENSOR_FUNCTIONS_H

namespace itensor {
namespace detail {

template <typename Set1,
          typename Set2,
          typename Perm>
void
calc_permutation(const Set1& s1,
                 const Set2& s2,
                 Perm& p)
    {
    for(size_t i2 = 0; i2 < s2.size(); ++i2)
        {
        const auto& v2 = s2[i2];
        bool found = false;
        for(size_t i1 = 0; i1 < s1.size(); ++i1)
            {
            if(v2 == s1[i1])
                {
                p[i1] = i2;
                found = true;
                break;
                }
            }

        if(!found)
            {
            throw ITError("sets are not permutations of each other");
            }
        }
    }


template <typename Set1,
          typename Set2Iter,
          typename RType,
          typename Map>
void
permute_map(const Set1& s1,
            const Set2Iter& s2begin,
            const Set2Iter& s2end,
            RType& r,
            Map&& m)
    {
    for(auto it = s2begin; it != s2end; ++it)
        {
        const auto& v2 = *it;
        bool found = false;
        for(size_t i1 = 0; i1 < s1.size(); ++i1)
            {
            if(v2 == s1[i1])
                {
                r[i1] = m(v2);
                found = true;
                break;
                }
            }

        if(!found)
            {
            throw ITError("sets are not permutations of each other");
            }
        }
    }

template <typename Set1,
          typename Set2,
          typename RType,
          typename Map>
void
permute_map(const Set1& s1,
            const Set2& s2,
            RType& r,
            Map&& m)
    {
    permute_map(s1,std::begin(s2),std::end(s2),r,std::forward<Map>(m));
    }

template <typename Container, typename Item>
bool
contains(const Container& C,
         const Item& I)
    {
    for(const auto& c : C) 
        {
        if(I == c) return true;
        }
    return false;
    }

//Simple linear congruential random number generator
Real inline
quickran()
    {
    static int seed = (std::time(NULL) + getpid());
    int im = 134456;
    int ia = 8121;
    int ic = 28411;
    Real scale = 1.0 / im;
    seed = (seed*ia+ic)%im;
    return Real(seed) * scale;
    }


template <typename Ret, class T, typename V>
auto 
call_impl(T&& obj, V&& v, int) -> decltype(obj(v))
    {
    return obj(v);
    }
template <typename Ret, class T, typename V>
Ret
call_impl(T&& obj, V&& v, long) 
    {
    throw std::runtime_error("Object does not support operator(v)->Ret for specified type.");
    return Ret();
    }
//
// The call(obj,v) function uses substitution-failure-is-not-an-error (sfinae)
// to either "plug" v into obj's operator() method if it has one defined
// or else to throw a runtime error if not.
// Use call(obj,v) to convert the absence of a specific operator() method
// to be a run-time error instead of a compile-time error.
//
template <typename Ret, class T, typename V>
Ret
call(T&& obj, V&& v)
    {
    return call_impl<Ret,T,V>(std::forward<decltype(obj)>(obj),v,0);
    }

/////////////////////////

template <class T, typename V>
void 
call_impl(T&& obj, V&& v, int)
    {
    obj(v);
    }
template <class T, typename V>
void
call_impl(T&& obj, V&& v, long) 
    {
    throw std::runtime_error("Object does not support operator(v) for specified type.");
    }
//
// The call(obj,v) function uses substitution-failure-is-not-an-error (sfinae)
// to either "plug" v into obj's operator() method if it has one defined
// or else to throw a runtime error if not.
// Use call(obj,v) to convert the absence of a specific operator() method
// to be a run-time error instead of a compile-time error.
//
template <class T, typename V>
void
call(T&& obj, V&& v)
    {
    call_impl<T,V>(std::forward<decltype(obj)>(obj),v,0);
    }

/////////////////////////

template <typename Ret, class FType, typename T1, typename T2>
auto 
call_impl(FType& func, T1& a1, T2& a2, int) -> decltype(func(a1,a2))
    {
    return func(a1,a2);
    }
template <typename Ret, class FType, typename T1, typename T2>
Ret
call_impl(FType& func, T1& a1, T2& a2, long) 
    {
    throw std::runtime_error("Object does not support operator(v1,v2)->Ret for specified type.");
    return Ret();
    }
//
// The call(obj,v1,v2) function uses substitution-failure-is-not-an-error (sfinae)
// to either "plug" v1,v2 into obj's operator() method if it has one defined
// or else to throw a runtime error if not.
// Use call(obj,v1,v2) to convert the absence of a specific operator() method
// to be a run-time error instead of a compile-time error.
//
template <typename Ret, class FType, typename T1, typename T2>
Ret
call(FType&& func, T1&& a1, T2&& a2)
    {
    return call_impl<Ret,FType,T1,T2>(std::forward<FType>(func),
                                      std::forward<T1>(a1),
                                      std::forward<T2>(a2),0);
    }

/////////////////////////

template <class T, typename V1, typename V2>
void 
call_impl(T&& obj, V1&& v1, V2&& v2, int)
    {
    obj(v1,v2);
    }
template <class T, typename V1, typename V2>
void
call_impl(T&& obj, V1&& v1, V2&& v2, long) 
    {
    throw std::runtime_error("Object does not support operator(v1,v2) for specified type.");
    }
//
// The call(obj,v1,v2) function uses substitution-failure-is-not-an-error (sfinae)
// to either "plug" v1,v2 into obj's operator() method if it has one defined
// or else to throw a runtime error if not.
// Use call(obj,v1,v2) to convert the absence of a specific operator() method
// to be a run-time error instead of a compile-time error.
//
template <class T, typename V1, typename V2>
void
call(T&& obj, V1&& v1, V2&& v2)
    {
    call_impl<T,V1,V2>(std::forward<T>(obj),
                       std::forward<V1>(v1),
                       std::forward<V2>(v2),0);
    }


}; //namespace detail
}; //namespace itensor

#endif

