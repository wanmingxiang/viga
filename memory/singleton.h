#ifndef APP_SEARCH_MOVIE_USER_SE_COMMON_SINGLETON_H
#define APP_SEARCH_MOVIE_USER_SE_COMMON_SINGLETON_H

#include <boost/noncopyable.hpp>
#include <pthread.h>

namespace movie {

/// @brief Thread-safe, no-manual destroy Singleton template
template<typename T>
class Singleton : public boost::noncopyable {
public:
    /// @brief Get the singleton instance
    static T* get() {
        pthread_once(&_p_once, &Singleton::_new);
        return _instance;
    }

private:
    Singleton();
    ~Singleton();

    /// @brief Construct the singleton instance
    static void _new() {
        _instance = new T();
    }

    /// @brief  Destruct the singleton instance
    /// @note Only work with gcc
    __attribute__((destructor)) static void _delete() {
        typedef char T_must_be_complete[sizeof(T) == 0 ? -1 : 1];
        (void) sizeof(T_must_be_complete);
        delete _instance;
    }

    static pthread_once_t _p_once;      ///< Initialization once control
    static T*             _instance;    ///< The singleton instance
};

template<typename T>
pthread_once_t Singleton<T>::_p_once = PTHREAD_ONCE_INIT;

template<typename T>
T* Singleton<T>::_instance = NULL;

}

template<class T>
T* singleton()
{
    return movie::Singleton<T>::get();
}


#endif
