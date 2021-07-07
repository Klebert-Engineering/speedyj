#pragma once

#include <optional>
#include <algorithm>

#include <vector>
#include <deque>
#include <set>
#include <list>
#include <map>

namespace speedyj::impl
{

template <class _Iter>
void PushArray(Stream& s, _Iter begin, _Iter end)
{
    try {
        s << speedyj::Array;
        std::for_each(begin, end, [&](const auto& vv) {
            s << vv;
        });
        s << speedyj::End;
    } catch (...) {
        s << speedyj::End;
        throw;
    }
}

template <class _Iter>
void PushObject(Stream& s, _Iter begin, _Iter end)
{
    try {
        s << speedyj::Object;
        std::for_each(begin, end, [&](const auto& vv) {
            s << vv.first << vv.second;
        });
        s << speedyj::End;
    } catch (...) {
        s << speedyj::End;
        throw;
    }
}

template <class _T>
struct TypeHelper
{
    static void push(Stream& s, const _T& v)
    {
        s.push(v);
    }
};

template <class _T>
struct TypeHelper<std::optional<_T>>
{
    static void push(Stream& s, const _T& v)
    {
        if (v)
            s.push(*v);
        else
            s << speedyj::Null;
    }
};

template <class _T, class _Alloc>
struct TypeHelper<std::vector<_T, _Alloc>>
{
    static void push(Stream& s, const std::vector<_T, _Alloc>& v)
    {
        PushArray(s, v.begin(), v.end());
    }
};

template <class _T, class _Alloc>
struct TypeHelper<std::deque<_T, _Alloc>>
{
    static void push(Stream& s, const std::deque<_T, _Alloc>& v)
    {
        PushArray(s, v.begin(), v.end());
    }
};

template <class _T, class _Comp, class _Alloc>
struct TypeHelper<std::set<_T, _Comp, _Alloc>>
{
    static void push(Stream& s, const std::set<_T, _Comp, _Alloc>& v)
    {
        PushArray(s, v.begin(), v.end());
    }
};

template <class _T, class _Alloc>
struct TypeHelper<std::list<_T, _Alloc>>
{
    static void push(Stream& s, const std::list<_T, _Alloc>& v)
    {
        PushArray(s, v.begin(), v.end());
    }
};

template <class _K, class _V, class _Comp, class _Alloc>
struct TypeHelper<std::map<_K, _V, _Comp, _Alloc>>
{
    static void push(Stream& s, const std::map<_K, _V, _Comp, _Alloc>& v)
    {
        PushObject(s, v.begin(), v.end());
    }
};

template <>
struct TypeHelper<bool>
{
    static void push(Stream& s, const bool& v)
    {
        if (v)
            s << speedyj::True;
        else
            s << speedyj::False;
    }
};

template <>
struct TypeHelper<std::nullptr_t>
{
    static void push(Stream& s, std::nullptr_t)
    {
        s << speedyj::Null;
    }
};

}
