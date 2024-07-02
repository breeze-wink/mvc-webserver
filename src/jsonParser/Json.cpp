#include "Parser.h"
#include "Json.h"
#include <sstream>

using namespace breeze::json;

Json::Json() : m_type(JSON_NULL) {}

Json::Json(Json::Type type) : m_type(type) {

    switch (m_type) {
        case JSON_NULL:
            break;
        case JSON_BOOL:
            m_value.m_bool = false;
            break;
        case JSON_INT:
            m_value.m_int = 0;
            break;
        case JSON_DOUBLE:
            m_value.m_double = 0.0;
            break;
        case JSON_STRING:
            m_value.m_string = new string("");
            break;
        case JSON_ARRAY:
            m_value.m_array = new std::vector<Json>();
            break;
        case JSON_OBJECT:
            m_value.m_object = new std::map<string, Json>();
            break;
        default:
            break;
    }
}

Json::Json(bool value) : m_type(JSON_BOOL) {
    m_value.m_bool = value;
}

Json::Json(int value) : m_type(JSON_INT) {
    m_value.m_int = value;
}

Json::Json(double value) : m_type(JSON_DOUBLE) {
    m_value.m_double = value;
}

Json::Json(const char *value) : m_type(JSON_STRING) {
    m_value.m_string = new string(value);
}

Json::Json(const string &value) : m_type(JSON_STRING) {
    m_value.m_string = new string(value);
}

Json::~Json() {
    clear();
}

string Json::str() const {
    std::ostringstream oss;
    switch (m_type) {
        case JSON_NULL:
            oss << "null";
            break;
        case JSON_BOOL: {
            if (m_value.m_bool) {
                oss << "true";
            } else {
                oss << "false";
            }
            break;
        }
        case JSON_INT:
            oss << m_value.m_int;
            break;
        case JSON_DOUBLE:
            oss << m_value.m_double;
            break;
        case JSON_STRING:
            oss << "\"" + *m_value.m_string + "\"";
            break;
        case JSON_ARRAY:
            oss << "[";
            for (auto it = (m_value.m_array)->begin(); it != m_value.m_array->end(); ++it) {
                if (it != m_value.m_array->begin()) {
                    oss << ",";
                }
                oss << (*it).str();
            }
            oss << "]";
            break;
        case JSON_OBJECT:
            oss << "{";
            for (auto it = m_value.m_object->begin(); it != m_value.m_object->end(); ++it) {
                if (it != m_value.m_object->begin()) {
                    oss << ",";
                }
                oss << "\"" << it->first << "\"" << ":" << it->second.str();
            }
            oss << "}";
            break;
        default:
            break;
    }
    return oss.str();
}


Json::Type Json::type() const {
    return m_type;
}

bool Json::is_null() const {
    return m_type == JSON_NULL;
}

bool Json::is_bool() const {
    return m_type == JSON_BOOL;
}

bool Json::is_int() const {
    return m_type == JSON_INT;
}

bool Json::is_double() const {
    return m_type == JSON_DOUBLE;
}

bool Json::is_string() const {
    return m_type == JSON_STRING;
}

bool Json::is_array() const {
    return m_type == JSON_ARRAY;
}

bool Json::is_object() const {
    return m_type == JSON_OBJECT;
}

bool Json::as_bool() const {
    if (m_type != JSON_BOOL) {
        throw std::logic_error("type error: not bool type");
    }
    return m_value.m_bool;
}

int Json::as_int() const {
    if (m_type != JSON_INT) {
        throw std::logic_error("type error: not int type");
    }
    return m_value.m_int;
}

double Json::as_double() const {
    if (m_type != JSON_DOUBLE) {
        throw std::logic_error("type error: not double type");
    }
    return m_value.m_double;
}

string Json::as_string() const {
    if (m_type != JSON_STRING) {
        throw std::logic_error("type error: not string type");
    }

    return *m_value.m_string;
}

Json::operator bool() {
    return as_bool();
}

Json::operator bool() const {
    return as_bool();
}

Json::operator int() {
    return as_int();
}

Json::operator int() const {
    return as_int();
}

Json::operator double() {
    return as_double();
}

Json::operator double() const {
    return as_double();
}

Json::operator string() {
    return as_string();
}

Json::operator string() const {
    return as_string();
}

Json &Json::operator=(bool value) {
    clear();//删去旧的内存
    m_type = JSON_BOOL;
    m_value.m_bool = value;
    return *this;
}

Json &Json::operator=(int value) {
    clear();
    m_type = JSON_INT;
    m_value.m_int = value;
    return *this;
}

Json &Json::operator=(double value) {
    clear();
    m_type = JSON_DOUBLE;
    m_value.m_double = value;
    return *this;
}

Json &Json::operator=(const char *value) {
    clear();
    m_type = JSON_STRING;
    m_value.m_string = new string(value);
    return *this;
}

Json &Json::operator=(const string &value) {
    clear();
    m_type = JSON_STRING;
    m_value.m_string = new string(value);
    return *this;
}

void Json::clear() {
    switch (m_type) {
        case JSON_NULL:
        case JSON_INT:
        case JSON_BOOL:
        case JSON_DOUBLE:
            break;
        case JSON_STRING:
            if (m_value.m_string != nullptr) {
                delete m_value.m_string;
                m_value.m_string = nullptr;
            }
            break;
        case JSON_ARRAY:
            if (m_value.m_array != nullptr) {
                auto &vec = m_value.m_array;
                for (auto &it: *vec) {
                    it.clear();
                }
                delete vec;
                vec = nullptr;
            }
        case JSON_OBJECT:
            if (m_value.m_object != nullptr) {
                auto &map = m_value.m_object;
                for (auto &it: *map) {
                    it.second.clear();
                }
                delete map;
                map = nullptr;
            }
        default:
            break;
    }
    m_type = JSON_NULL;
}

void Json::append(const Json &value) {
    if (m_type != JSON_ARRAY) {
        clear();
        m_type = JSON_ARRAY;
        m_value.m_array = new std::vector<Json>();
    }
    m_value.m_array->push_back(value);
}

bool Json::has(int index) const{
    if (m_type != JSON_ARRAY) {
        return false;
    }
    int size = static_cast<int>(m_value.m_array->size());
    return index >= 0 && index < size;
}

Json Json::get(int index) const{
    if (!has(index)) {
        throw std::logic_error("out_of_range");
    }
    return m_value.m_array->at(index);
}

void Json::remove(int index) {
    if (!has(index)) {
        throw std::logic_error("out_of_range");
    }
    auto &vec = m_value.m_array;
    vec->at(index).clear();
    vec->erase(vec->begin() + index);
}

Json &Json::operator[](int index) {
    if (m_type != JSON_ARRAY) {
        throw std::logic_error("type error: not array");
    }
    if (!has(index)) {
        throw std::logic_error("array out of range");
    }
    return m_value.m_array->at(index);
}

Json Json::get(const char *key) const{
    if (!has(key)) {
        return Json();
    }
    return (*m_value.m_object)[key];
}

Json Json::get(const string &key) const {
    return get(key.c_str());
}

void Json::remove(const char *key) {
    if (m_type != JSON_OBJECT) {
        return;
    }
    auto it = m_value.m_object->find(key);
    if (it == m_value.m_object->end()) {
        return;
    }
    it->second.clear();
    m_value.m_object->erase(it);
}

void Json::remove(const string &key) {
    remove(key.c_str());
}

Json &Json::operator[](const char *key) {
    if (m_type != JSON_OBJECT) {
        clear();
        m_type = JSON_OBJECT;
        m_value.m_object = new std::map<string, Json>();
    }

    return (*m_value.m_object)[key];
}

Json &Json::operator[](const string &key) {
    return (*this)[key.c_str()];
}

bool Json::has(const char *key) const {
    if (m_type != JSON_OBJECT) {
        return false;
    }
    return m_value.m_object->find(key) != m_value.m_object->end();
}

bool Json::has(const string &key) const {
    return has(key.c_str());
}

int Json::size() const {
    switch (m_type) {
        case JSON_ARRAY:
            return (int) (m_value.m_array)->size();
        case JSON_OBJECT:
            return (int) (m_value.m_object)->size();
        default:
            break;
    }
    return -1;
}

bool Json::empty() const {
    switch (m_type) {
        case JSON_NULL:
            return true;
        case JSON_ARRAY:
            return m_value.m_array->empty();
        case JSON_OBJECT:
            return m_value.m_object->empty();
        default:
            break;
    }
    return false;
}

Json::Json(const Json &other) {
    copy(other);
}

Json &Json::operator=(const Json &other) {
    copy(other);
    return *this;
}

void Json::copy(const Json &other) //深拷贝
{
    clear();
    m_type = other.type();
    switch (m_type) {
        case JSON_NULL:
            break;
        case JSON_BOOL:
        case JSON_INT:
        case JSON_DOUBLE:
            m_value = other.m_value;
            break;
        case JSON_STRING:
            if (other.m_value.m_string != nullptr) {
                m_value.m_string = new string(*other.m_value.m_string);
            }
            break;
        case JSON_ARRAY:
            if (other.m_value.m_array != nullptr) {
                m_value.m_array = new std::vector<Json>();
                for (auto &json: *other.m_value.m_array) {
                    m_value.m_array->push_back(json);
                }
            }
            break;
        case JSON_OBJECT:
            if (other.m_value.m_object != nullptr) {
                m_value.m_object = new std::map<string, Json>();
                for (auto &pair: *other.m_value.m_object) {
                    m_value.m_object->insert(std::make_pair(pair.first, pair.second));
                }
            }
            break;
        default:
            break;
    }
}

void Json::parse(const string &file) {
    clear();
    Parser parser;
    parser.load(file);
    *this = parser.parse();
}

void Json::parse(const char *buf, int len) {
    clear();
    Parser parser;
    parser.load(buf, len);
    *this = parser.parse();
}

Json::Json(Json &&other) noexcept {
    swap(other);
}

Json &Json::operator=(Json &&other) noexcept {
    swap(other);
    return *this;
}

void Json::swap(Json &other) {
    Type type = m_type;
    Value value = m_value;
    m_type = other.m_type;
    m_value = other.m_value;
    other.m_type = type;
    other.m_value = value;
}

void Json::append(Json &&value) {
    if (m_type != JSON_ARRAY) {
        clear();
        m_type = JSON_ARRAY;
        m_value.m_array = new std::vector<Json>();
    }
    m_value.m_array->push_back(std::move(value));
}
