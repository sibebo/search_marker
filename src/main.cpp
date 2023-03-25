#include <iostream>
#include "search_marker.h"

int main()
{
    try
    {
        std::string s{"skal skolen udskammes for al skam i verden?"};
        search_marker::SearchMarker()
                       .AddToken("sk")
                       .AddToken("ka")
                       .AddToken("skolen")
                       .Process(s)
                       .Print();
        std::cout << s << std::endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << "Fatal error: " << e.what() << '\n';
    }

    return 0;
}
