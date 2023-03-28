#include <iostream>
#include "search_marker.h"

int main()
{
    try
    {
        std::string s{"skal skolen udskammes for al skam i verden?"};
        
        // Chaining:
        search_marker::SearchMarker()
                       .AddToken("sk")
                       .AddToken("ka")
                       .AddToken("skolen")
                       .Process(s)
                       .Print();
        std::cout << s << std::endl;

        // Using SearchMarker::Get to extract the processed matches:
        auto    searcher = search_marker::SearchMarker();
        searcher.AddTokens({"sk", "ka", "skolen"});
        
        searcher.Process(s);

        for (auto [from, to] : searcher.Get())
        {
            std::cout << from 
                      << '\t' << to 
                      << '\t' << s.substr(from, to - from) 
                      << std::endl;
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << "Fatal error: " << e.what() << '\n';
    }

    return 0;
}
