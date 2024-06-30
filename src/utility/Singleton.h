#pragma once

namespace breeze
{
    namespace utility
    {
        //单例模式模板类
        template <typename T>
        class Singleton
        {
        public:
            //线程安全，类似于饿汉式
            static T* Instance()
            {
                static T instance;
                return &instance;
            }
        private:
            Singleton() = default;
            Singleton(const Singleton<T>&) = delete;
            Singleton<T>& operator = (const Singleton<T> &) = delete;
            ~Singleton() = default;
        };
        #define SINGLETON(classname) \
        friend class Singleton<classname>; \
        private: \
        classname() = default; \
        classname(const classname &) = delete; \
        classname & operator = (const classname &) = delete; \
        ~classname() = default;
    }
}