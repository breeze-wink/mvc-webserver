#pragma once
#include <string>
using std::string;

namespace breeze::web
{

    class Response
    {
    public:
        enum class Type 
        {
            HTML,
            JSON,
            JS,
            CSS,
            JPG,
            PNG,
            GIF,
            ICO
        };

        Response();
        ~Response();

        void code(int code);
        void data(Type type, const string& data);
        string data() const;

        void html(const string& data);
        void json(const string& data);

        static string page_not_found();

    private:
        int m_code;
        Type m_type;
        string m_data;
    };
}