#pragma once
#include <string>
#include <vector>
#include <map>
#include <iostream>

using std::string;
namespace breeze::json
{
    class Json
    {
    public:
        enum Type
        {
            JSON_NULL = 0,
            JSON_BOOL,
            JSON_INT,
            JSON_DOUBLE,
            JSON_STRING,
            JSON_ARRAY,
            JSON_OBJECT
        };
        Json();
        Json(Type type);
        Json(bool value);
        Json(int value);
        Json(double value);
        Json(const char* value);
        Json(const string& value);
        Json(const Json& other);
        Json (Json && other) noexcept;
        ~Json();

        Type type() const;
        bool is_null() const;
        bool is_bool() const;
        bool is_int() const;
        bool is_double() const;
        bool is_string() const;
        bool is_array() const;
        bool is_object() const;

        bool as_bool() const;
        int as_int() const;
        double as_double() const;
        string as_string() const;

        operator bool();
        operator bool() const;
        operator int();
        operator int() const;
        operator double();
        operator double() const;
        operator string();
        operator string()const;

        Json& operator = (bool value);
        Json& operator = (int value);
        Json& operator = (double value);
        Json& operator = (const char* value);
        Json& operator = (const string& value);
        Json& operator = (const Json& other);
        Json& operator = (Json&& other) noexcept;

        void append(const Json& value);
        void append(Json&& value);

        Json get(int index) const;
        Json get(const char* key) const;
        Json get(const string& key) const;

        void remove(int index);
        void remove(const char* key);
        void remove(const string& key);

        Json& operator [](int index);
        Json& operator [](const char* key);
        Json& operator [](const string& key);

        typedef std::vector<Json>::iterator iterator;

        iterator begin()
        {
            return m_value.m_array -> begin();
        }
        iterator end()
        {
            return m_value.m_array -> end();
        }

        string str()const;
        int size() const;
        bool empty() const;
        void parse(const string& file);
        void parse(const char* buf, int len);

        friend std::ostream& operator << (std::ostream& cout, const Json& json)
        {
            cout << json.str();
            return cout;
        }

        void clear();

    private:
        
        void copy(const Json& other);
        void swap(Json& other);
        bool has(int index) const;
        bool has(const char* key) const;
        bool has(const string& key) const;

    private:
        union Value// 共同体，内存占用取决于需要的最大内存
        {
            bool m_bool;
            int m_int;
            double m_double;
            string* m_string;
            std::vector<Json>* m_array;
            std::map<string, Json>* m_object; //全部弄成指针，节省内存
        };
        Type m_type;
        Value m_value;
    };

}
