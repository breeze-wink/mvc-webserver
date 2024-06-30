#pragma once
#include "xml.h"

namespace breeze::xml
{
    class Parser
    {
    public:
        Parser() = default;
        ~Parser() = default;

        bool load(const string& filename);
        bool load(const char* buf, size_t len);
        Xml parse();

    private:
        void skip_white_space();
        bool parse_declaration();
        bool parse_comment();
        Xml  parse_element();
        string parse_element_name();
        string parse_element_text();
        string parse_element_attr_key();
        string parse_element_atrr_val();

    private:
        string m_str;
        size_t m_idx = 0;
    };
}