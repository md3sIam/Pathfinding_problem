#include "CSVUtil.h"

std::vector<std::string> CSVUtil::parse_to_strings(const std::string &s) {
    unsigned long start = 0, end = s.length();
    std::vector<std::string> result;
    do {
        end = s.find(',', start);

        if (end == std::string::npos){
            result.push_back(s.substr(start));
        }
        else {
            result.push_back(s.substr(start, end - start));
            start = end + 1;
        }

    } while(end != std::string::npos);
    return result;
}

std::string CSVUtil::vector_to_string(const std::vector<std::string> &v) {
    std::string result;
    for (const auto &s : v){
        result += s + " ";
    }
    return result;
}