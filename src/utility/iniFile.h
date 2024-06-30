# pragma once
#include "Value.h"
#include <map>
#include <string>

using std::string;

namespace breeze::utility
{
    class IniFile
    {
        typedef std::map<string, Value> Section;
    public:
        IniFile() = default;
        explicit IniFile(const string& filename_);
        ~IniFile() = default;


        void show() const;
        bool load(const string& filename_);
        Value& get(const string& section, const string& key);
        void set(const string& section, const string& key, const Value& val);

        Section& operator [](const string& section)
        {
            return sections[section];
        }
        bool has(const string& section);
        bool has(const string& section, const string& key);

        void remove(const string& section);
        void remove(const string& section, const string& key);

        void clear();

        void save(const string& fileName);
    private:
        string str() const;
        static string trim(const string& str);

    private:
        string m_fileName;
        std::map<string, Section> sections;
    };
}