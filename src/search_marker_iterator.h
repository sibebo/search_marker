#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <utility>

#include "rang/rang.hpp"

namespace search_marker {

class SearchMarker
{
public:
    SearchMarker& AddToken(std::string t) 
    {
        tokens.push_back(t); 
        return *this;
    }

    SearchMarker& AddTokens(std::vector<std::string> ts) 
    {
        tokens.insert(tokens.begin(), ts.begin(), ts.end()); 
        return *this;
    }

    SearchMarker& Process(const std::string &s)
    {
        this->s = s;

        Search();
        Sort();
        Consolidate();

        return *this;
    }

    bool    AnyHits() const {return !v.empty();} 

    void    Print() 
    {
        Print(rang::bg::yellow, rang::fg::black);
    }

private:
    std::string                 s;
    std::vector<std::string>    tokens;

    std::vector<std::pair<std::string::iterator, std::string::iterator>>    v;

    std::pair<std::string::iterator, std::string::iterator>    Find(std::string::iterator b, std::string::iterator e, std::string token)
    {
        auto i = std::search(b, e, token.begin(), token.end());
        if (i != e)
        {
            return {i, i+token.size()};
        }
        else
        {
            return {e,e};
        }
    }

    std::vector<std::pair<std::string::iterator, std::string::iterator>>    Find(std::string token)
    {
        std::vector<std::pair<std::string::iterator, std::string::iterator>>    r;

        auto iter = s.begin();
        while (iter != s.end())
        {
            auto hit = Find(iter, s.end(), token);
            if ( hit.first != s.end())
            {
                r.push_back(hit);
            }
            iter = hit.second;
        }

        return r;
    }

    void    Search()
    {
        v.clear();
        for (auto t : tokens)
        {
            auto    hits = Find(t);
            v.insert(v.end(), hits.begin(), hits.end());
        }
    }

    void    Sort()
    {
        std::sort(v.begin(), v.end(), [](const auto &a, const auto &b)
        {
            if (a.first == b.first) return (a.second < b.second);
            else return (a.first < b.first);
        });
    }

    bool    Overlapping(const std::pair<std::string::iterator, std::string::iterator> &a,
                        const std::pair<std::string::iterator, std::string::iterator> &b)
    {
        return !(a.second < b.first || b.second < a.first);
    }

    bool    Adjacent(const std::pair<std::string::iterator, std::string::iterator> &a,
                     const std::pair<std::string::iterator, std::string::iterator> &b)
    {
        return (a.second == b.first && b.second == a.first);
    }

    std::pair<std::string::iterator, std::string::iterator>    Merge(const std::pair<std::string::iterator, std::string::iterator> &a,
                                                                     const std::pair<std::string::iterator, std::string::iterator> &b)
    {
        return {(a.first < b.first ? a.first : b.first), (a.second > b.second ? a.second : b.second)};
    }

    void    Consolidate()
    {
        auto    f = v.begin();
        auto    l = f + 1;
        while (f != v.end() && l != v.end())
        {
            if (Overlapping(*f, *l) || Adjacent(*f, *l))
            {
                *f = Merge(*f, *l);
                l = v.erase(l);
            }
            else
            {
                ++f;
                ++l;
            }
        }
    }

    std::string    ToString(const std::pair<std::string::iterator, std::string::iterator> &a)
    {
        return std::string(a.first, a.second);
    }

    template <typename Tbg, typename Tfg>
    void    Print(Tbg bg, Tfg fg)
    {
        auto    iter = s.begin();
        for (const auto &p : v)
        {
            std::cout << ToString({iter, p.first});
            std::cout << bg << fg << ToString(p) << Tfg::reset << Tbg::reset;
            iter = p.second;
        }

        std::cout << ToString({iter, s.end()}) << std::endl;
    }
};

} //namespace search_marker
