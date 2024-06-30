
#include "iniFile.h"
#include <iostream>
#include <fstream>
#include <limits>
#include <sstream>
using namespace breeze::utility;

IniFile::IniFile(const string& filename_)
{
    load(filename_);
}

bool IniFile::load(const std::string &filename_)
{
    sections.clear();
    m_fileName = filename_;

    std::ifstream ifs(filename_);
    if(ifs.bad())
    {
        throw std::runtime_error("fileIn is corrupted" + filename_);
    }
    if(ifs.fail())
    {
        ifs.clear();
        ifs.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        throw std::runtime_error("loading ini file error" + filename_);
    }

    string line;
    string name;
    while(std::getline(ifs, line))
    {
        line = trim(line);
        if(line.empty())
        {
            continue;
        }
        if(line[0] == '#' || line[0] == ';')
        {
            continue;
        }
        if(line[0] == '[')
        {
            auto pos = line.find_first_of(']');
            if(pos == std::string::npos)
            {
                throw std::runtime_error("invalid section: " + line);
            }
            name = trim(line.substr(1, pos - 1));
            sections[name] = Section();
        }
        else
        {
            auto pos = line.find_first_of('=');
            if(pos == std::string::npos)
            {
                throw std::runtime_error("invalid option: " + line);
            }
            string key = line.substr(0, pos);
            key = trim(key);
            string val = line.substr(pos + 1);
            val = trim(val);
            sections[name][key] = val;
        }
    }


    return true;
}

string IniFile::trim(const string &str) {
    if(str.empty())
    {
        return str;
    }
    string s = str;
    s.erase(0, s.find_first_not_of(" \r\n"));
    s.erase(s.find_last_not_of(" \r\n") + 1);
    return s;
}

string IniFile::str() const{
    std::stringstream ss;
    for(const auto& section:sections)
    {
        ss << "[" << section.first << "]" << std::endl;
        for(const auto& option : section.second)
        {
            ss << option.first << " = " << (string)(option.second) << std::endl;
        }
        ss << std::endl;

    }
    return ss.str(); // 关注这个函数
}
void IniFile::show()const
{
    std::cout << str();
}

Value& IniFile::get(const string &section, const string &key)
{
    return sections[section][key];
}

void IniFile::set(const string &section, const string &key, const Value &val)
{
    sections[section][key] = val;
}

bool IniFile::has(const string &section)
{
    return sections.find(section) != sections.end();
}

bool IniFile::has(const string &section, const string &key)
{
    auto it = sections.find(section);
    if(it == sections.end()) return false;

    return it -> second.find(key) != it -> second.end();
}

void IniFile::remove(const string& section)
{
    sections.erase(section);
}
void IniFile::remove(const string& section, const string& key)
{
    auto it = sections.find(section);
    if(it == sections.end())
    {
        return;
    }
    it -> second.erase(key);
}

void IniFile::clear()
{
    sections.clear();
}

void IniFile::save(const string &fileName)
{
    std::ofstream ofs(fileName, std::ios::out);
    if(ofs.fail())
    {
        throw std::runtime_error("saving ini file failed: " + fileName);
    }
    ofs << str();
    ofs.close();
}
