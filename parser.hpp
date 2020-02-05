#pragma once

bool foundDelimiter (size_t n) { return n!=std::string::npos; }

std::vector<std::string> parse (const std::string& str, const std::string& delimiters)
{
    size_t pos, currentPos(0);
    std::vector <std::string> tmp;
    
    while (foundDelimiter (pos = str.find_first_of (delimiters, currentPos)))
    {
        tmp.emplace_back (str.substr (currentPos, pos-currentPos));
        currentPos = pos+1;
    }
    
    tmp.emplace_back (str.substr (currentPos));
    return tmp;
}
