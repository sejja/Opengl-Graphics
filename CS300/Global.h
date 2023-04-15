#ifndef _GLOBAL__H_
#define _GLOBAL__H_

template <typename T>
class Singleton {
public:
    static T& Instance() {
        static T instance;
        return instance;
    }
protected:
    Singleton() {}
    ~Singleton() {}
   
public:
    Singleton(Singleton const&) = delete;
    Singleton& operator=(Singleton const&) = delete;
};

#endif