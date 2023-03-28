# search_marker

A colorizer of search matches.

## Example of usage

``` c++
{
    auto    searcher = search_marker::SearchMarker();
    searcher.AddToken("id")
            .AddToken("se")
            .AddToken("con");
            
    std::string s("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.");
    searcher.Process(s).Print();
}
```

## Dependencies
This class is depending on agauniyal/rang for cross platform colorizing the text in colsole.
Please see: [agauniyal/rang](https://github.com/agauniyal/rang)
