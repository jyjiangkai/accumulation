## 单例实现模板
#include <pthread.h>

template <typename T>

class Singleton
{
    public:
        static T* singleton();
    protected:
        static void InitSingleton()
	{
	    pInstance = new T();
	}
	static pthread_once_t _ponce;
	static T* pInstance;
};

template <typename T>
T* Singleton<T>::singleton()
{
    pthread_once(&_ponce, &Singleton<T>::InitSingleton);
    return pInstance;
}

使用方式：
DSFServiceRouter *p_route = Singleton<DSFServiceRouter>::singleton();

