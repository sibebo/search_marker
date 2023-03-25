#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <utility>

#include "rang.hpp"

namespace search_marker {

class SearchMarker
{
public:
    /// @brief AddToken appends a single search token to the internal token list.
    /// @param t    the token to add
    /// @return     a reference to this.
    SearchMarker& AddToken(std::string t) 
    {
        tokens.push_back(t); 
        return *this;
    }

    /// @brief AddTokens appends a sequence of search token to the internal token list.
    /// @param t    the sequence of tokens to add
    /// @return     a reference to this.
    SearchMarker& AddTokens(std::vector<std::string> ts) 
    {
        tokens.insert(tokens.begin(), ts.begin(), ts.end()); 
        return *this;
    }

    /// @brief Process processes the input string and stores the occurence of matches.
    /// @param s    the string to search for matches.
    /// @return     a reference to this.
    SearchMarker& Process(const std::string &s)
    {
        this->s = s;

        Search();
        Sort();
        Consolidate();

        return *this;
    }


    /// @brief AnyHits returns true if there where any hits found during processing 
    /// @return     true if any hits.
    /// @see Process.
    bool    AnyHits() const {return !v.empty();} 

    /// @brief Print prints the string with the matches marked with colours.
    void    Print() 
    {
        Print(rang::bg::yellow, rang::fg::black);
    }

    /// @brief Get returns a const reference to the internal vector of matches.
    /// @return a const reference to the internal vector of matches.
    const std::vector<std::pair<size_t, size_t>>& Get() const 
    {
        return v;
    }

private:
    std::string                 s;
    std::vector<std::string>    tokens;

    std::vector<std::pair<size_t, size_t>>    v;

    std::pair<size_t, size_t>    Find(size_t index, std::string token)
    {
        auto    pos = s.find(token, index);
        if (pos != std::string::npos)
        {
            return {pos, pos + token.size()};
        }
        else
        {
            return {std::string::npos, std::string::npos};
        }
    }

    std::vector<std::pair<size_t, size_t>>    Find(std::string token)
    {
        std::vector<std::pair<size_t, size_t>>    r;

        size_t  index = 0;
        while (index != std::string::npos)
        {
            auto hit = Find(index, token);
            if ( hit.first != std::string::npos)
            {
                r.push_back(hit);
            }
            index = hit.second;
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

    bool    Overlapping(const std::pair<size_t, size_t> &a,
                        const std::pair<size_t, size_t> &b)
    {
        return !(a.second < b.first || b.second < a.first);
    }

    bool    Adjacent(const std::pair<size_t, size_t> &a,
                     const std::pair<size_t, size_t> &b)
    {
        return (a.second == b.first && b.second == a.first);
    }

    std::pair<size_t, size_t>    Merge(const std::pair<size_t, size_t> &a,
                                       const std::pair<size_t, size_t> &b)
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

    std::string    ToString(const std::pair<size_t, size_t> &a)
    {
        return s.substr(a.first, a.second - a.first);
    }

    template <typename Tbg, typename Tfg>
    void    Print(Tbg bg, Tfg fg)
    {
        size_t  index = 0;
        for (const auto &p : v)
        {
            std::cout << ToString({index, p.first});
            std::cout << bg << fg << ToString(p) << Tfg::reset << Tbg::reset;
            index = p.second;
        }

        std::cout << ToString({index, std::string::npos}) << std::endl;
    }
};

} //namespace search_marker
