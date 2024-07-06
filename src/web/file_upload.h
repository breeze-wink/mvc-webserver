#pragma once
#include <map>
#include <string>

namespace breeze::web
{
    class FileUpload
    {
    public:
        FileUpload();
        ~FileUpload();

        std::string name() const;
        std::string filename() const;
        std::string type() const;
        const char* data() const;
        int size() const;
        std::string extension() const;

        void parse(const char* buf, int len);
        bool save(const std::string& filename);
        

    private:
        std::string m_name;
        std::string m_type;
        std::string m_filename;
        std::string m_extension;
        std::string m_boundary;
        std::map<std::string, std::string> m_headers;
        const char* m_buf;
        int m_len;
        int m_start;
        int m_end;
    };
}