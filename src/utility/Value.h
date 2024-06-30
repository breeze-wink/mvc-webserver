# pragma once
#include <string>
using std::string;

namespace breeze::utility {

    class Value
    {
    public:
        enum Type
        {
            V_NULL = 0,
            V_BOOL,
            V_INT,
            V_FLOAT,
            V_DOUBLE,
            V_STRING
        };
        //构造函数和赋值运算符重载都是为了 other -> Value

        Value();
        Value(bool value_);
        Value(int value_);
        Value(unsigned int value_);
        Value(float value_);
        Value(double value_);
        Value(const char* value_);
        Value(const string& value_);

        Value& operator = (bool value_);
        Value& operator = (int value_);
        Value& operator = (unsigned int value_);
        Value& operator = (float value_);
        Value& operator = (double value_);
        Value& operator = (const char* value_);
        Value& operator = (const string& value_);

        ~Value() = default;

        Type getType () const;

        //获取判断
        bool isNull();
        bool isBool();
        bool isInt();
        bool isFloat();
        bool isDouble();
        bool isString();

        bool operator == (const Value& other)const;
        bool operator != (const Value& other)const;

        //Value -> other， 考虑const 和 非const 两个版本
        operator bool();
        operator bool() const;
        operator int();
        operator int() const;
        operator unsigned int();
        operator unsigned int() const;
        operator float();
        operator float() const;
        operator double();
        operator double() const;
        operator string();
        operator string() const;

    private:
        string value;
        Type type;
    };

}



