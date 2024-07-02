#ifndef STRING_STRING_H
#define STRING_STRING_H
#include <string>
#include <cstdarg>
#include <vector>
using std::string;

namespace breeze::utility {


    class String
    {
    public:
        String() = default;
        ~String() = default;

        static string to_lower(const string& input);
        static string to_upper(const string& input);

        static string ltrim(const string& input, const string& trims);
        static string rtrim(const string& input, const string& trims);
        static string trim(const string& input, const string& trims);

        static std::vector<string> split(const string& input, const string& separator);
        static std::vector<string> split(const string& input, char separator);

        static string join(const std::vector<string>& input, const string& joiner);
        static string join(const std::vector<string>& input, char joiner);

        static bool hasPrefix(const string& input, const string& prefix);
        static bool hasSuffix(const string& input, const string& suffix);

        static string capitalize(const string& input);
        static string format(const char* input, ...);
    };


}

#endif //STRING_STRING_H
