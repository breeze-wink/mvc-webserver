#include "file_upload.h"
#include <cstring>
#include "Logger.h"
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

using namespace breeze::web;
using namespace breeze::utility;
using std::string;

FileUpload::FileUpload() : m_buf(nullptr), m_len(0), m_start(0), m_end(0)
{

}

FileUpload::~FileUpload()
{

}

string FileUpload::name() const
{
    return m_name;
}
string FileUpload::filename() const
{
    return m_filename;
}
string FileUpload::type() const
{
    return m_type;
}
const char* FileUpload::data() const
{
    return m_buf;
}
int FileUpload::size() const
{
    return m_end - m_start;
}
string FileUpload::extension() const
{
    return m_extension;
}

void FileUpload::parse(const char* buf, int len)
{
    m_buf = buf;
    m_len = len;

    const char* s = buf;
    const char* e = buf + len - 1;
    const char* i = s;

    auto skip_chars = [&](const std::string& str){
        while (i < e && str.find(*i) != std::string::npos) i ++;
    };

    auto go_to_next_char = [&](const std::string str){
        while (i < e && str.find(*i) == std::string::npos) i ++;
    };

    // 解析 multipart/form-data boundary
    go_to_next_char("\r\n");
    m_boundary = string(s, 2, i - s);

    skip_chars("\r\n");
    s = i;

    //开始解析form data headers
    while (i < e)
    {
        //parse form data head
        go_to_next_char(": ");
        string name = string(s, 0, i - s);

        skip_chars(": ");
        s = i;

        //parse data head value
        go_to_next_char("\r\n");
        string value = string(s, 0, i - s);

        m_headers[name] = value;

        if (name == "Content-Disposition")
        {
            //form name
            size_t pos = value.find("name=");
            size_t start = pos + 6;
            size_t end = value.find("\"", start);
            m_name = value.substr(start, end - start);

            log_debug("name = %s", m_name.c_str());

            //filename
            pos = value.find("filename=");
            start = pos + 10;
            end = value.find("\"", start);
            m_filename = value.substr(start, end - start);

            log_debug("filename=", m_filename.c_str());

            size_t ext = m_filename.find(".");
            m_extension = m_filename.substr(ext);
        }

        else if (name == "Content-Type")
        {
            m_type = value;
        }

        if (strncmp(i, "\r\n\r\n", 4) == 0)
        {
            i += 4;
            break;
        }
        skip_chars("\r\n");

        s = i;
    }

    //metadata parse over

    m_start = i - buf;
    string str = "--" + m_boundary + "--";
    const char* t = e - str.size();
    m_end = t - m_buf;
    log_debug("upload content: %s", s);
    
}

bool FileUpload::save(const std::string& filename)
{
    fs::path filepath = filename;
    if (!fs::exists(filepath.parent_path()))
    {
        try
        {
            fs::create_directories(filepath.parent_path());
        }
        catch (const std::exception& e)
        {
            log_error("Error creating directories: %s", e.what());
            return false;
        }
    }

    std::ofstream ofs(filepath);

    if (ofs.fail())
    {
        log_error("open file failed: %s", filename.c_str());
        return false;
    }

    ofs.write(m_buf + m_start, m_end - m_start);
    ofs.flush();
    return true;
}