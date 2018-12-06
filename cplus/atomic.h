/*******************************************************************************
 * Copyright    :   2016-2017, Huawei Tech.Co.,Ltd
 *
 * Author       :   luoenliang
 * Date         :   2016-02-01
 * Version      :   DSF V100R001
 *
 * Description  :
 ******************************************************************************/
//lint -e766

#ifndef __DSF_ATOMIC_H__
#define __DSF_ATOMIC_H__

#define atomic_read(x) \
    /*lint -save -e762 -e718 -e746 -e516 -e571*/  \
    (__sync_add_and_fetch((x),0))\
    /*lint -restore*/
#define atomic_inc(ptr) \
    /*lint -save -e762 -e718 -e746 -e516*/  \
    ((void)__sync_add_and_fetch(ptr, 1)) \
    /*lint -restore*/

#define atomic_dec(ptr) \
    /*lint -save -e762 -e718 -e746 -e516*/  \
    ((void)__sync_sub_and_fetch(ptr, 1)) \
    /*lint -restore*/
#define add_atomic(x, v) (/*lint -save -e1055*/ (__typeof(*(x))) __sync_add_and_fetch((x), (v)) /*lint -restore*/)
#define sub_atomic(x, v) (/*lint -save -e1055*/ (__typeof(*(x))) __sync_sub_and_fetch((x), (v)) /*lint -restore*/)
#define atomic_add(x, v) (/*lint -save -e1055*/ (__typeof(*(x))) __sync_fetch_and_add((x), (v)) /*lint -restore*/)
#define atomic_sub(x, v) (/*lint -save -e1055*/ (__typeof(*(x))) __sync_fetch_and_sub((x), (v)) /*lint -restore*/)
#define atomic_set_volatile(x, val) (*(__typeof(*(x)) volatile *)(x) = (val))
#define atomic_read_volatile(x) (*(__typeof(*(x)) volatile *)(x))
#define atomic_set_zero(x)\
/*lint -save -e762 -e718 -e746 -e516*/  \
((void)__sync_lock_release((x))) \
/*lint -restore*/

#define atomic_set_return_old(x, y)  \
/*lint -save -e762 -e718 -e746 -e516*/  \
((__typeof(*(x)))__sync_lock_test_and_set((x), (y))) \
/*lint -restore*/



bool cas32(volatile unsigned *mem, unsigned cmp, unsigned with);
bool cas64(volatile unsigned long long *mem, unsigned long long cmp, unsigned long long with);

unsigned getAndSet32(volatile unsigned *mem, const unsigned val);
unsigned incAndGet32(volatile unsigned *mem, const unsigned val);
unsigned decAndGet32(volatile unsigned *mem, const unsigned val);

unsigned long long getAndSet64(volatile unsigned long long *mem, const unsigned long long val);
unsigned long long incAndGet64(volatile unsigned long long *mem, const unsigned long long val);

template<typename T> class __Atomic;
template<typename T> class __Atomic64;


typedef  __Atomic<unsigned>  AtomicUInt32;


typedef  __Atomic64<unsigned long long>  AtomicUInt64;

// Atomic Type 
// l00150444 import Atomic Operation from Doug Lea's JUC
// [×¢Òâ]:²»ÒªÖ±½ÓÊ¹ÓÃ¸ÃÄ£°åÀà¡£
//        ×ÜÊÇÊ¹ÓÃÉÏÃæÌá¹©µÄÌØ»¯Àà¡£
template<typename T>
class __Atomic
{
private:
    volatile  T m_value;
public:
    __Atomic(T initial = 0) :m_value(initial) { }

    ~__Atomic() { m_value = 0; }

    T get() const { return m_value; }

    void set(T newValue) { m_value = newValue; }

    T getAndSet(T newValue)
    {
        #ifdef _AIX 
        for(;;) {
            T current = get();
            if (cas32(&m_value, current, newValue))
                return current;
        }
        #else
        return atomic_set_return_old(&m_value, newValue);
        #endif
    }

    T getAndIncrement()
    {
        #ifdef _AIX 
        for(;;) {
            T current = get();
            T next = current + 1;
            if (cas32(&m_value,current,next))
                return current;
        }
        #else
        return atomic_add(&m_value, 1);
        #endif
    }

    T getAndDecrement()
    {
        #ifdef _AIX 
        for(;;) {
            T current = get();
            T next = current - 1;
            if (cas32(&m_value,current, next))
                return current;
        }
        #else
        return atomic_sub(&m_value, 1);
        #endif
    }

    T getAndAdd(T delta)
    {
        #ifdef _AIX 
        for(;;) {
            T current = get();
            T next = current + delta;
            if (cas32(&m_value, current, next))
                return current;
        }
        #else
        return atomic_add(&m_value, delta);
        #endif
    }

    T incrementAndGet()
    {
        #ifdef _AIX 
        for(;;) {
            T current = get();
            T next = current + 1;
            if (cas32(&m_value, current, next))
                return next;
        }
        #else
        return add_atomic(&m_value, 1);
        #endif
    }

    T decrementAndGet()
    {
        #ifdef _AIX 
        for(;;) {
            T current = get();
            T next = current - 1;
            if (cas32(&m_value,current,next))
                return next;
        }
        #else
        return sub_atomic(&m_value, 1);
        #endif
    }

    T addAndGet(T delta)
    {
        #ifdef _AIX 
        for(;;) {
            T current = get();
            T next = current + delta;
            if (cas32(&m_value,current, next))
                return next;
        }
        #else
        return add_atomic(&m_value, delta);
        #endif
    }


    T operator ++ () // prefix
    {
        /// Increments the counter and returns the result.
        #ifdef _AIX 
        return addAndGet(1);
        #else
        return add_atomic(&m_value, 1);
        #endif
    }

    T operator ++ (int)// postfix
    {
        /// Increments the counter and returns the previous value.
        #ifdef _AIX 
        T val = getAndIncrement();
        return val;
        #else
        return atomic_add(&m_value, 1);
        #endif
    }

    T operator -- ()// prefix
    {
        /// Decrements the counter and returns the result.
        #ifdef _AIX 
        return addAndGet(-1);
        #else
        return sub_atomic(&m_value, 1);
        #endif
        
    }

    T operator -- (int) // postfix
    {
        /// Decrements the counter and returns the previous value.
        #ifdef _AIX 
        T val = getAndDecrement();
        return val;
        #else
        return atomic_sub(&m_value, 1);
        #endif
    }

};





// Atomic Type 
// l00150444 import Atomic Operation from Doug Lea's JUC
// [×¢Òâ]:²»ÒªÖ±½ÓÊ¹ÓÃ¸ÃÄ£°åÀà¡£
//        ×ÜÊÇÊ¹ÓÃÉÏÃæÌá¹©µÄÌØ»¯Àà¡£
template<typename T>
class  __Atomic64
{
private:
    volatile  T m_value;
public:
    __Atomic64(T initial = 0) :m_value(initial) { }

    ~__Atomic64() { m_value = 0; }
    
    T get() const { return m_value; }

    void set(T newValue) { m_value = newValue; }

    T getAndSet(T newValue)
    {
        #ifdef _AIX 
        for(;;) {
            T current = get();
            if (cas64(&m_value, current, newValue))
                return current;
        }
        #else
        return atomic_set_return_old(&m_value, newValue);
        #endif
    }

    T getAndIncrement()
    {
        #ifdef _AIX 
        for(;;) {
            T current = get();
            T next = current + 1;
            if (cas64(&m_value,current,next))
                return current;
        }
        #else
        return atomic_add(&m_value, 1);
        #endif
    }

    T getAndDecrement()
    {
        #ifdef _AIX 
        for(;;) {
            T current = get();
            T next = current - 1;
            if (cas64(&m_value,current, next))
                return current;
        }
        #else
        return atomic_sub(&m_value, 1);
        #endif
    }

    T getAndAdd(T delta)
    {
        #ifdef _AIX 
        for(;;) {
            T current = get();
            T next = current + delta;
            if (cas64(&m_value, current, next))
                return current;
        }
        #else
        return atomic_add(&m_value, delta);
        #endif
    }

    T incrementAndGet()
    {
        #ifdef _AIX 
        for(;;) {
            T current = get();
            T next = current + 1;
            if (cas64(&m_value, current, next))
                return next;
        }
        #else
        return add_atomic(&m_value, 1);
        #endif
    }

    T decrementAndGet()
    {
        #ifdef _AIX 
        for(;;) {
            T current = get();
            T next = current - 1;
            if (cas64(&m_value,current,next))
                return next;
        }
        #else
        return sub_atomic(&m_value, 1);
        #endif
    }

    T addAndGet(T delta)
    {
        #ifdef _AIX 
        for(;;) {
            T current = get();
            T next = current + delta;
            if (cas64(&m_value,current, next))
                return next;
        }
        #else
        return add_atomic(&m_value, delta);
        #endif
    }

    T operator ++ (int)// postfix
    {
        /// Increments the counter and returns the previous value.
        #ifdef _AIX 
        T val = getAndIncrement();
        return val;
        #else
        return atomic_add(&m_value, 1);
        #endif
    }


    T operator ++ () // prefix
    {
        /// Increments the counter and returns the result.
        #ifdef _AIX 
                return addAndGet(1);
        #else
                return add_atomic(&m_value, 1);
        #endif
    }

    T operator -- (int) // postfix
    {
        /// Decrements the counter and returns the previous value.
        #ifdef _AIX 
                T val = getAndDecrement();
                return val;
        #else
                return atomic_sub(&m_value, 1);
        #endif
    }

    T operator -- ()// prefix
    {
        /// Decrements the counter and returns the result.
        #ifdef _AIX 
        return addAndGet(-1);
        #else
        return sub_atomic(&m_value, 1);
        #endif
    }

};



#endif 
