/********************************************************************
 * 功能描述 : 常用返回值
 ********************************************************************/
#ifndef SUCCESS
#define SUCCESS (0)
#endif

#ifndef FAILURE
#define FAILURE (1)
#endif

#ifndef RET_FALSE
#define RET_FALSE (0)
#endif

#ifndef RET_TRUE
#define RET_TRUE (1)
#endif

#define RET_ERR (1)
#define RET_OK (0)

/********************************************************************
 * 功能描述 : 公共数据类型
 ********************************************************************/
#ifndef CHAR
#define CHAR char
#endif

#ifndef CHAR_MIN
#define CHAR_MIN (-128)
#endif

#ifndef CHAR_MAX
#define CHAR_MAX (127)
#endif

#ifndef UCHAR
#define UCHAR unsigned char
#endif

#ifndef UCHAR_MAX
#define UCHAR_MAX (255)
#endif

#ifndef SHORT
#define SHORT short
#endif

#ifndef SHORT_MIN
#define SHORT_MIN (-32768)
#endif

#ifndef SHORT_MAX
#define SHORT_MAX (32767)
#endif

#ifndef USHORT
#define USHORT unsigned short
#endif

#ifndef USHORT_MIN
#define USHORT_MIN (0)
#endif

#ifndef USHORT_MAX
#define USHORT_MAX (65535)
#endif

#ifndef LONG
#define LONG long
#endif

#ifndef ULONG
#define ULONG unsigned long
#endif

#ifndef LLONG
#define LLONG long long
#endif

#ifndef ULLONG
#define ULLONG unsigned long long
#endif

#ifndef FLOAT
#define FLOAT float
#endif

#ifndef DOUBLE
#define DOUBLE double
#endif

#ifndef INT8
#define INT8 char
#endif

#ifndef INT8_MIN
#define INT8_MIN (-128)
#endif

#ifndef INT8_MAX
#define INT8_MAX (127)
#endif

#ifndef UINT8
#define UINT8 unsigned char
#endif

#ifndef UINT8_MAX
#define UINT8_MAX (255)
#endif

#ifndef INT16
#define INT16 short
#endif

#ifndef INT16_MAX
#define INT16_MAX (32767)
#endif

#ifndef INT16_MIN
#define INT16_MIN (-INT16_MAX - 1)
#endif

#ifndef UINT16
#define UINT16 unsigned short
#endif

#ifndef UINT16_MAX
#define UINT16_MAX (65535)
#endif

#ifndef INT32
#define INT32 int
#endif

#ifndef INT32_MAX
#define INT32_MAX (2147483647)
#endif

#ifndef INT32_MIN
#define INT32_MIN (-INT32_MAX - 1)
#endif

#ifndef UINT32
#define UINT32 unsigned int
#endif

#ifndef UINT32_MAX
#define UINT32_MAX (4294967295U)
#endif

#ifndef INT64
#define INT64 long long int
#endif

#ifndef INT64_MIN
#define INT64_MIN (-INT64_MAX - 1LL)
#endif

#ifndef INT64_MAX
#define INT64_MAX (9223372036854775807LL)
#endif

#ifndef UINT64
#define UINT64 unsigned long long int
#endif

#ifndef UINT64_MAX
#define UINT64_MAX (18446744073709551615LLU)
#endif

#ifndef LONG_MAX
#if __WORDSIZE == 64
#define LONG_MAX 9223372036854775807L
#else
#define LONG_MAX 2147483647L
#endif
#endif

#ifndef LONG_MIN
#define LONG_MIN (-LONG_MAX - 1L)
#endif

#ifndef ULONG_MAX
#if __WORDSIZE == 64
#define ULONG_MAX 18446744073709551615UL
#else
#define ULONG_MAX 4294967295UL
#endif
#endif

#ifndef VOID
#define VOID void
#endif

#ifndef OFF_T
#define OFF_T off_t
#endif

#ifndef SIZE_T
#define SIZE_T size_t
#endif

#ifndef SSIZE_T
#define SSIZE_T ssize_t
#endif

#ifndef STRING
#define STRING std::string
#endif

#ifndef STRING_LIST
#define STRING_LIST std::list<std::string>
#endif

#ifndef PAIR
#define PAIR std::pair
#endif

#ifndef SET
#define SET std::set
#endif

#ifndef MULTISET
#define MULTISET std::multiset
#endif

#ifndef LIST
#define LIST std::list
#endif

#ifndef QUEUE
#define QUEUE std::queue
#endif

#ifndef DEQUE
#define DEQUE std::deque
#endif

#ifndef MAP
#define MAP std::map
#endif

#ifndef MULTIMAP
#define MULTIMAP std::multimap
#endif

#ifndef VECTOR
#define VECTOR std::vector
#endif

#ifndef SSTREAM
#define SSTREAM std::stringstream
#endif

#ifndef ISSTREAM
#define ISSTREAM std::istringstream
#endif

#ifndef OSSTREAM
#define OSSTREAM std::ostringstream
#endif

#ifndef PTHREAD_T
#define PTHREAD_T pthread_t
#endif

#ifndef NULL
#define NULL (0)
#endif

#ifndef BOOL
#define BOOL int
#endif

/* 系统退出命令 */
#define EXIT(status) _exit(status)
/* 最大域名长度 */
#define MAX_DOMAIN_NAME 256
/* 最大文件名长度 */
#define MAX_FILENAME_LEN 256
/* 最大文件全路径长度 */
#define MAX_PATH 1024
/* 最大URL长度 */
#define MAX_URL_LEN 1024
/* 秒的换算单位 */
#define UNIT_SECOND2MS 1000
/* s到us的换算 */
#define UINT_SECOND2USEC 1000000
/* 分钟的换算单位 */
#define UNIT_MIN2SEC 60
/* 小时的换算单位 */
#define UNIT_HOUR2SEC 3600
/* 字节换算单位 */
#define UNIT_KB 1024
/* 比特换算单位 */
#define UNIT_KBIT 1000
/* 字节到比特的转换单位 */
#define UNIT_BYTE2BIT 8
/* 十六进制基数 */
#define UNIT_HEX 16
/* 八进制基数 */
#define UNIT_OCTAL 8
/* 二进制基数 */
#define UNIT_BINARY 2
/* 计算百分比基数 */
#define UNIT_PERCENT 100
/* 非法文件句柄 */
#define INVALID_FD (-1)
/* 非法socket句柄 */
#define INVALID_SOCK (-1)
/* 非法CPU标识 */
#define INVALID_CPU_ID (-1)
/* 非法会话标识(UINT32) */
#define INVALID_SESSION_ID (0)

/********************************************************************
 * 功能描述 : 常用宏定义
 ********************************************************************/
/* 判断空格 */
#ifndef isspace
#define isspace(c)      (((c) == ' ') || ((c) == '\t') || ((c) == '\r') || ((c) == '\n'))
#endif
/* 判断ASCII码 */
#ifndef isascii
#define isascii(c)       (((unsigned char)(c))<=0x7f)
#endif
/* 判断大写字符 */
#ifndef isupper
#define isupper(c)       ((c) >= 'A' && (c) <= 'Z')
#endif
/* 判断小写字符 */
#ifndef islower
#define islower(c)       ((c) >= 'a' && (c) <= 'z')
#endif
/* 判断字符 */
#ifndef isalpha
#define isalpha(c)       (isupper(c) || (islower(c)))
#endif
/* 判断数字 */
#ifndef isdigit
#define isdigit(c)       ((c) >= '0' && (c) <= '9')
#endif
/* 判断字符&数字 */
#ifndef isxdigit
#define isxupper(c)      ((c) >= 'A' && (c) <= 'F')
#define isxlower(c)      ((c) >= 'a' && (c) <= 'f')
#define isxdigit(c)      (isdigit(c) || isxupper(c) ||isxlower(c))
#endif
/* 返回最小值 */
#undef RET_MIN
#define RET_MIN(x, y)    \
    {    \
        typeof(x) _x = (x);    \
        typeof(y) _y = (y);    \
        (void)(&_x == &_y);    \
        _x < _y ? _x : _y;    \
    }
/* 返回最大值 */
#undef RET_MAX
#define RET_MAX(x, y)    \
    {    \
        typeof(x) _x = (x);    \
        typeof(y) _y = (y);    \
        (void)(&_x == &_y);    \
        _x > _y ? _x : _y;    \
    }
/* likely & unlikely */
#undef likely
#undef unlikely
#if defined(__GNUC__)
#define likely(x)    __builtin_expect(!!(x),1)  //x很可能为真
#define unlikely(x)  __builtin_expect(!!(x),0)  //x很可能为假
#else
#define likely(x) (x)
#define unlikely(x) (x)
#endif
/* CHECK RET & CONTINUE & BREAK */
#define CHK_RET(x, ret) {if (unlikely(x)){return ret;};}
#define CHK_CONTINUE(x) {if (unlikely(x)){continue;}}
#define CHK_BREAK(x)    {if (unlikely(x)){break;}}
/* 不允许拷贝构造和赋值 */
#undef FROBID_COPY_AND_ASSIGN
#define FROBID_COPY_AND_ASSIGN(CLASSNAME) \
private: \
    CLASSNAME(const CLASSNAME&); \
    const CLASSNAME& operator=(const CLASSNAME&);
/* 不允许自身创建对象 */
#undef FROBID_CREATE_BY_SELF
#define FROBID_CREATE_BY_SELF(CLASSNAME) \
private: \
    CLASSNAME(); \
    ~CLASSNAME(); \
    CLASSNAME(const CLASSNAME&);

/********************************************************************
 * 功能描述 : 参数校验，无返回值
 * 输入参数 : ptr  指针
 *            fmt  窗体控件字符串
 *            args 可选自变量
 * 输出参数 : 无
 * 返回值   : 无
 ********************************************************************/
#define CHK_PTR_WITHOUT_RET_LOG(ptr, fmt, args...)    \
    do {    \
        if (unlikely(NULL == (ptr))) {     \
            LOG_PRINT(LOG_ERROR, fmt, ##args);    \
        }    \
    } while (0)

/********************************************************************
 * 功能描述 : 参数校验，无返回值
 * 输入参数 : ptr  指针
 *            fmt  窗体控件字符串
 *            args 可选自变量
 * 输出参数 : 无
 * 返回值   : 无
 ********************************************************************/
#define CHK_PTR_WITHOUT_RET_VAL_LOG(ptr, fmt, args...)    \
    do {    \
        if (unlikely(NULL == (ptr))) {     \
            LOG_PRINT(LOG_ERROR, fmt, ##args);    \
            return;    \
        }    \
    } while (0)

/********************************************************************
 * 功能描述 : 参数校验，有返回值
 * 输入参数 : ptr      指针
 *            retValue 返回值
 *            fmt      窗体控件字符串
 *            args     可选自变量
 * 输出参数 : 无
 * 返回值   : retValue
 ********************************************************************/
#define CHK_PTR_WITH_RET_VAL_LOG(ptr, retValue, fmt, args...)    \
    do {    \
        if (unlikely(NULL == (ptr))) {    \
            LOG_PRINT(LOG_ERROR, fmt, ##args);    \
            return retValue;    \
        }    \
    } while (0)

/********************************************************************
 * 功能描述 : 参数校验，无返回
 * 输入参数 : state 值
 *            fmt   窗体控件字符串
 *            args  可选自变量
 * 输出参数 : 无
 * 返回值   : 无
 ********************************************************************/
#define CHK_STATE_WITHOUT_RET_LOG(state, fmt, args...)    \
    do {    \
        if (unlikely(state)) {    \
            LOG_PRINT(LOG_ERROR, fmt, ##args);    \
        }    \
    } while (0)

/********************************************************************
 * 功能描述 : 参数校验，无返回值
 * 输入参数 : state 值
 *            fmt   窗体控件字符串
 *            args  可选自变量
 * 输出参数 : 无
 * 返回值   : 无
 ********************************************************************/
#define CHK_STATE_WITHOUT_RET_VAL_LOG(state, fmt, args...)    \
    do {    \
        if (unlikely(state)) {    \
            LOG_PRINT(LOG_ERROR, fmt, ##args);    \
            return;    \
        }    \
    } while (0)

/********************************************************************
 * 功能描述 : 参数校验，有返回值
 * 输入参数 : state    值
 *            retValue 返回值
 *            fmt      窗体控件字符串
 *            args     可选自变量
 * 输出参数 : 无
 * 返回值   : retValue
 ********************************************************************/
#define CHK_STATE_WITH_RET_VAL_LOG(state, retValue, fmt, args...)    \
    do {    \
        if (unlikely(state)) {    \
            LOG_PRINT(LOG_ERROR, fmt, ##args);    \
            return (retValue);    \
        }    \
    } while (0)

/********************************************************************
 * 功能描述 : 参数校验
 * 输入参数 : ptr 指针
 * 输出参数 : 无
 * 返回值   : 无
 ********************************************************************/
#define CHK_PTR_BREAK(ptr)    \
    do {    \
        if (unlikely(NULL == (ptr)))    \
        {    \
            break;    \
        }    \
    } while (0)

/********************************************************************
 * 功能描述 : 参数校验
 * 输入参数 : state 值
 * 输出参数 : 无
 * 返回值   : 无
 ********************************************************************/
#define CHK_STATE_CONTINUE(state)    \
    do {    \
        if (unlikely(state))    \
        {   \
            continue;    \
        }    \
    } while (0)

/********************************************************************
 * 功能描述 : 参数校验
 * 输入参数 : str (char *)
 * 输出参数 : 无
 * 返回值   : 无
 ********************************************************************/
#define CHECK_STR_NULL(str)    \
    do {    \
        if (unlikely(NULL==(str)) || unlikely('\0'==(str[0]))) {    \
            LOG_PRINT(LOG_ERROR, "Entrance parameter %s is error\n", #param);    \
       }    \
    }while(0)

/********************************************************************
 * 功能描述 : 内存释放
 * 输入参数 : ptr 指针
 * 输出参数 : 无
 * 返回值   : 无
 ********************************************************************/
#define SAFE_FREE(ptr)    \
    do {    \
        if (ptr) {    \
            free(ptr);    \
            ptr = NULL;    \
        }    \
    } while (0)

/********************************************************************
 * 功能描述 : 内存清除并释放
 * 输入参数 : ptr 指针
 * 输出参数 : 无
 * 返回值   : 无
 ********************************************************************/
#define CLEAR_AND_SAFE_FREE_STR(ptr)    \
    do {    \
            if(ptr)    \
            {    \
                size_t len = strlen(ptr);    \
                (VOID)memset_s((void*)ptr, len, 0, len);    \
                free(ptr);    \
                ptr = NULL;    \
            }    \
     } while (0)

/* 获取CPU核数 */
#define GET_CPU_NUM() sysconf(_SC_NPROCESSORS_ONLN)

// log config
extern int log_level;

#define LOG_ERROR(format, args...) \
    if(log_level >= ERROR_LEVEL) { \
		log_error("%s %s(%d): " format, "ERROR", __FILE__, __LINE__, ##args); \
    }

#define LOG_WARN(format, args...) \
    if(log_level >= WARN_LEVEL) { \
		log_warn("%s %s(%d): " format, "WARN", __FILE__, __LINE__, ##args); \
    }

#define LOG_INFO(format, args...) \
    if(log_level >= INFO_LEVEL) { \
		log_info("%s %s(%d): " format, "INFO", __FILE__, __LINE__, ##args); \
    }

#define LOG_DEBUG(format, args...) \
    if(log_level >= DEBUG_LEVEL) { \
		log_debug("%s : " format, "DEBUG", ##args); \
    }

/********************************************************************
 * 功能描述 : 文件夹拼接，可以避免连续多个‘/’
 * 输入参数 : first_path  待拼接的第一个路径
 *            second_path 待拼接的第二个路径
 *            dst_path    输出路径
 *            dst_size    输出路径长度
 * 输出参数 : dst_path    输出路径
 * 返回值   : 无
 ********************************************************************/
#define MAKE_FULL_PATH(first_path, second_path, dst_path, dst_size)     \
    do     \
    {     \
        if (first_path[strlen(first_path) - 1] != '/')     \
        {     \
            (void)snprintf_s(dst_path, dst_size, dst_size, "%s/%s",     \
                first_path, second_path);     \
        }     \
        else     \
        {     \
            (void)snprintf_s(dst_path, dst_size, dst_size, "%s%s",     \
                first_path, second_path);     \
        }     \
    } while (0)

/********************************************************************
 * 功能描述 : 安全函数，用于高性能场景
 ********************************************************************/
#if defined(WITH_PERFORMANCE_ADDONS) 
    /* for strncpy_s performance optimization */
#define STRNCPY_SM(dest, destMax, src, count) \
    ((NULL != (void*)dest && NULL != (void*)src && (size_t)destMax >0 && (((UINT64T)(destMax) & (UINT64T)(-2)) < SECUREC_STRING_MAX_LEN) && (TWO_MIN(count , strlen(src)) + 1) <= (size_t)destMax ) ?  ( (count < strlen(src))? (memcpy(dest, src, count), *((char*)dest + count) = '\0', EOK) :( memcpy(dest, src, strlen(src) + 1), EOK ) ) :(strncpy_error(dest, destMax, src, count))  )

#define STRCPY_SM(dest, destMax, src) \
    (( NULL != (void*)dest && NULL != (void*)src  && (size_t)destMax >0 && (((UINT64T)(destMax) & (UINT64T)(-2)) < SECUREC_STRING_MAX_LEN) && ( strlen(src) + 1) <= (size_t)destMax )? (memcpy(dest, src, strlen(src) + 1), EOK) :( strcpy_error(dest, destMax, src)))

   /* for strcat_s performance optimization */
#if defined(__GNUC__)
#define STRCAT_SM(dest, destMax, src) \
    ({ int catRet =EOK;\
    if ( NULL != (void*)dest && NULL != (void*)src && (size_t)(destMax) >0 && (((UINT64T)(destMax) & (UINT64T)(-2)) < SECUREC_STRING_MAX_LEN) ) {\
        char* pCatTmpDst = (dest);\
        size_t catRestSz = (destMax);\
        do{\
            while(catRestSz > 0 && *pCatTmpDst) {\
                ++pCatTmpDst;\
                --catRestSz;\
            }\
            if (catRestSz == 0) {\
                catRet = EINVAL;\
                break;\
            }\
            if ( ( strlen(src) + 1) <= catRestSz ) {\
                memcpy(pCatTmpDst, (src), strlen(src) + 1);\
                catRet = EOK;\
            }else{\
                catRet = ERANGE;\
            }\
        }while(0);\
        if ( EOK != catRet) catRet = strcat_s((dest), (destMax), (src));\
    }else{\
        catRet = strcat_s((dest), (destMax), (src));\
    }\
    catRet;})
#else
#define STRCAT_SM(dest, destMax, src) strcat_s(dest, destMax, src)
#endif

    /*for strncat_s performance optimization*/
#if defined(__GNUC__)
#define STRNCAT_SM(dest, destMax, src, count) \
    ({ int ncatRet = EOK;\
    if (NULL != (void*)dest && NULL != (void*)src && (size_t)destMax > 0 && (((UINT64T)(destMax) & (UINT64T)(-2)) < SECUREC_STRING_MAX_LEN)  && (((UINT64T)(count) & (UINT64T)(-2)) < SECUREC_STRING_MAX_LEN)) {\
        char* pCatTmpDest = (dest);\
        size_t ncatRestSz = (destMax);\
        do{\
            while(ncatRestSz > 0 && *pCatTmpDest) {\
                ++pCatTmpDest;\
                --ncatRestSz;\
            }\
            if (ncatRestSz == 0) {\
                ncatRet = EINVAL;\
                break;\
            }\
            if ( (TWO_MIN((count) , strlen(src)) + 1) <= ncatRestSz ) {\
                if ((count) < strlen(src)) {\
                    memcpy(pCatTmpDest, (src), (count));\
                    *(pCatTmpDest + (count)) = '\0';\
                }else {\
                    memcpy(pCatTmpDest, (src), strlen(src) + 1);\
                }\
            }else{\
                ncatRet = ERANGE;\
            }\
        }while(0);\
        if ( EOK != ncatRet) ncatRet = strncat_s((dest), (destMax), (src), (count));\
    }else{\
        ncatRet = strncat_s((dest), (destMax), (src), (count));\
    }\
    ncatRet;})
#else
#define STRNCAT_SM(dest, destMax, src, count) strncat_s(dest, destMax, src, count)
#endif

    /*
    MEMCPY_SM do NOT check buffer overlap by default, or you can add this check to improve security
    condCheck = condCheck || (dest == src) || (dest > src && dest < (void*)((UINT8T*)src + count));\
    condCheck = condCheck || (src > dest && src < (void*)((UINT8T*)dest + count)); \
    */

#define  MEMCPY_SM(dest, destMax, src, count)\
    (!(((size_t)destMax== 0 )||(((UINT64T)(destMax) & (UINT64T)(-2)) > SECUREC_MEM_MAX_LEN)||((size_t)count > (size_t)destMax) || (NULL == (void*)dest) || (NULL == (void*)src))? (memcpy(dest, src, count), EOK) : (memcpy_s(dest, destMax, src, count)))

#define  MEMSET_SM(dest, destMax, c, count)\
    (!(((size_t)destMax == 0 ) || (((UINT64T)(destMax) & (UINT64T)(-2)) > SECUREC_MEM_MAX_LEN) || (NULL == (void*)dest) || ((size_t)count > (size_t)destMax)) ? (memset(dest, c, count), EOK) : ( memset_s(dest, destMax, c, count)))

#endif /* WITH_PERFORMANCE_ADDONS */
