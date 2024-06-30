#include "Value.h"
#include <iostream>
#include <sstream>

using namespace breeze::utility;

Value::Value():type(V_NULL)
{

}

Value::Value(bool value_):type(V_BOOL)
{
    value = value_ ? "true":"false";
}
Value::Value(int value_):type(V_INT)
{
    value = std::to_string(value_);
}
Value::Value(unsigned int value_):type(V_INT)
{
    value = std::to_string(value_);
}

Value::Value(float value_):type(V_FLOAT)
{
    value = std::to_string(value_);
}
Value::Value(double value_):type(V_DOUBLE)
{
    value = std::to_string(value_);
}
Value::Value(const char* value_):type(V_STRING)
{
    value = value_;
}
Value::Value(const string& value_):type(V_STRING)
{
    value = value_;
}

Value &Value::operator=(bool value_)
{
    type = V_BOOL;
    value = value_ ? "true":"false";
    return *this;
}

Value &Value::operator=(int value_)
{
    type = V_INT;
    value = std::to_string(value_);
    return *this;
}

Value &Value::operator=(unsigned int value_)
{
    type = V_INT;
    value = std::to_string(value_);
    return *this;
}

Value &Value::operator=(float value_)
{
    type = V_FLOAT;
    value = std::to_string(value_);
    return *this;
}

Value &Value::operator=(double value_)
{
    type = V_DOUBLE;
    value = std::to_string(value_);
    return *this;
}

Value& Value::operator=(const char* value_)
{
    type = V_STRING;
    value = value_;
    return *this;
}

Value& Value::operator=(const string &value_)
{
    type = V_STRING;
    value = value_;
    return *this;
}

Value::Type Value::getType() const
{
    return type;
}

bool Value::isNull()
{
    return type == V_NULL;
}
bool Value::isBool()
{
    return type == V_BOOL;
}
bool Value::isInt()
{
    return type == V_INT;
}
bool Value::isFloat()
{
    return type == V_FLOAT;
}
bool Value::isDouble()
{
    return type == V_DOUBLE;
}
bool Value::isString()
{
    return type == V_STRING;
}

bool Value::operator==(const Value &other) const
{
    return type == other.type && value == other.value;
}

bool Value::operator!=(const Value &other) const
{
    return type != other.type || value != other.value;
}

Value::operator bool()
{
    return value == "true";
}
Value::operator bool() const
{
    return value == "true";
}
Value::operator int()
{
    return std::stoi(value);
}
Value::operator int() const
{
    return std::stoi(value);
}
Value::operator unsigned int()
{
    return std::stoi(value);
}
Value::operator unsigned int() const
{
    return std::stoi(value);
}
Value::operator float()
{
    float val;
    std::stringstream ss;
    ss << value;
    ss >> val;
    return val;
}
Value::operator float() const
{
    float val;
    std::stringstream ss;
    ss << value;
    ss >> val;
    return val;
}
Value::operator double()
{
    double val;
    std::stringstream ss;
    ss << value;
    ss >> val;
    return val;
}
Value::operator double() const
{
    double val;
    std::stringstream ss;
    ss << value;
    ss >> val;
    return val;
}
Value::operator string()
{
    return value;
}
Value::operator string() const
{
    return value;
}