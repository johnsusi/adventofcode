#pragma once
#include <algorithm>
#include <iterator>
#include <set>

template <typename T1, typename T2> void set_intersect(T1& source, const T2& other)
{
    T1 temp;
    std::set_intersection(source.begin(),
                          source.end(),
                          other.begin(),
                          other.end(),
                          std::inserter(temp, temp.begin()));
    source = temp;
}

template <typename T1, typename T2> void set_union(T1& source, const T2& other)
{
    T1 temp;
    std::set_union(source.begin(),
                   source.end(),
                   other.begin(),
                   other.end(),
                   std::inserter(temp, temp.begin()));
    source = temp;
}
