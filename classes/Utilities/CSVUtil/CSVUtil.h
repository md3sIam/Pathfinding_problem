#pragma once

#include <vector>
#include <string>


class CSVUtil {

public:
    static std::vector<std::string> parse_to_strings(const std::string &s);
    static std::string vector_to_string(const std::vector<std::string> &v);
    static void csv_to_cpp_fp(std::string &s);
};
