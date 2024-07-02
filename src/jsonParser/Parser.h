#pragma once
#include <string>
#include "Json.h"


using std::string;
namespace breeze::json
{
    class Parser
    {
    public:
        Parser() = default;
        ~Parser() = default;

        void load(const string& fileName);
        void load(const char* buf, int len);
        Json parse();
    private:
        void skip_white_space();
        char get_next_token();
        bool in_range(int x, int lower, int upper) const;
        Json parse_null();
        Json parse_bool();
        Json parse_number();
        Json parse_string();
        Json parse_array(); //
        Json parse_object();//用到了递归，值得细品
    private:
        string m_str;
        size_t m_idx = 0;
    };
}


