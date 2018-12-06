/* 安全函数 */
#include "securec.h"
#include "securectype.h"

/********************************************************************
 * 功能描述 : 内存拷贝函数，从源地址的起始位置开始拷贝count个字节到目标地址的起始位置中
 *            1、函数返回值与危险函数不同，操作成功返回EOK(0)，出错返回非0
 *            2、新增参数destMax，表示dest中最多容纳的字符数量，其余参数与危险函数用法一致
 *            3、函数出错时，将dest指向的内存全部清零（清零内存的长度为destMax），例如：dest和src内存重叠或count > desMax时按出错处理
 * 输入参数 : dest    目标地址
 *            destMax 目标地址最大允许容纳的字符个数
 *            src     源地址
 *            count   需要拷贝的源字符个数
 * 输出参数 : dest    目标地址
 * 返回值   : 操作成功返回EOK(0)，出错返回非0
 ********************************************************************/
errno_t memcpy_s(void* dest, size_t destMax, const void* src, size_t count);

/********************************************************************
 * 功能描述 : 内存拷贝函数，从源地址的起始位置开始拷贝count个字节到目标地址的起始位置中
 *            1、函数返回值与危险函数不同，操作成功返回EOK(0)，出错返回非0
 *            2、新增参数destMax，表示dest中最多容纳的字符数量，其余参数与危险函数用法一致
 *            3、函数出错时，将dest指向的内存全部清零（清零内存的长度为destMax），例如：当count > desMax时按出错处理
 *            4、若目标地址和源地址有重叠，memmove_s能够保证源串在被覆盖之前将重叠区域的字节拷贝到目标区域中，但复制后源地址内容会被更改
 *            5、若目标地址和源地址没有重叠，则和memcpy_s函数功能相同，但memcpy_s的速度更快
 * 输入参数 : dest    目标地址
 *            destMax 目标地址最大允许容纳的字符个数
 *            src     源地址
 *            count   需要拷贝的源字符个数
 * 输出参数 : dest    目标地址
 * 返回值   : 操作成功返回EOK(0)，出错返回非0
 ********************************************************************/
errno_t memmove_s(void* dest, size_t destMax, const void* src, size_t count);

/********************************************************************
 * 功能描述 : 字符串拷贝函数，把源地址指向的字符串中前count个字节复制到目标地址指向的地址中
 *            1、函数返回值与危险函数不同，操作成功返回EOK(0)，出错返回非0
 *            2、新增参数destMax表示strDest中最多容纳字符数量（包括\0），其余参数与strcnpy函数用法一致
 *            3、函数出错时，则将strDest指向的内存首字符置0，例如：当复制的字符串长度大于destMax-1时按出错处理
 *            4、strncpy函数不保证strDest有\0结尾，而strncpy_s函数保证strDest有\0结尾
 *            5、当 n > strlen(src) 时，strncpy函数复制完字符串后在dest中填充count - strlen(src)个'\0'字符；strncpy_s会复制完字符串后仅保证有\0结尾
 *            6、若待拷贝的字符串中包含'\0'，则只能使用memcpy_s内存拷贝函数，strncpy_s函数会在遇到'\0'时终止
 * 输入参数 : strDest 目标地址
 *            destMax 目标地址最大允许容纳的字符个数
 *            strSrc  源地址
 *            count   需要拷贝的源字符个数
 * 输出参数 : strDest 目标地址
 * 返回值   : 操作成功返回EOK(0)，出错返回非0
 ********************************************************************/
errno_t strncpy_s(char* strDest, size_t destMax, const char* strSrc, size_t count);

/********************************************************************
 * 功能描述 : 字符串串接函数，把源地址指向的字符串中前count个字节串接到目标地址结尾处
 *            1、函数返回值与危险函数不同，操作成功返回EOK(0)，出错返回非0
 *            2、新增参数destMax表示strDest中最多容纳字符数量（包括\0），其余参数与危险函数用法一致
 *            3、函数出错时，如果strDest非空且destMax大于0，则将strDest指向的内存首字符置0，例如：当拼接后的字符串长度大于destMax-1时按出错处理
 *            4、源地址和目的地址所指内存区域不可以重叠
 *            5、目的地址必须有足够的空间来容纳源地址的字符串
 * 输入参数 : strDest 目标地址
 *            destMax 目标地址最大允许容纳的字符个数
 *            strSrc  源地址
 *            count   需要串接的源字符个数
 * 输出参数 : strDest 目标地址
 * 返回值   : 操作成功返回EOK(0)，出错返回非0
 ********************************************************************/
errno_t strncat_s(char* strDest, size_t destMax, const char* strSrc, size_t count);

/********************************************************************
 * 功能描述 : 格式化输出函数，将可变个参数(...)按照format格式化成字符串，然后将其复制到strDest中
 *            1、成功时函数返回格式化后的字符串长度（不包括\0），出错或发生截断时返回-1
 *            2、返回值与危险函数存在差异，发生截断时安全函数返回-1，危险函数返回格式化后的字符串长度
 *            3、新增参数destMax表示strDest中最多容纳字符数量（包括\0）
 *            4、参数count是与危险函数中的size相对应的参数，但是存在差异点：count是指允许输出到strDest中的格式化后字符串的字符数（不包括\0)
 *            5、函数出错时，将strDest指向的内存首字符置0，例如：当format中的格式不标准或格式化后的字符串长度大于destMax -1时按出错处理
 *            6、为了保证函数有截断功能，要求destMax > count,通常count的值可取destMax - 1
 * 输入参数 : strDest 目标地址
 *            destMax 目标地址最大允许容纳的字符个数
 *            count   允许输出到strDest中的格式化后字符串的字符数（不包括\0)
 *            format  窗体控件字符串
 *            ...    可选自变量
 * 输出参数 : strDest 目标地址
 * 返回值   : 成功时函数返回格式化后的字符串长度（不包括\0），出错或发生截断时返回-1
 ********************************************************************/
int snprintf_s (char* strDest, size_t destMax, size_t count, const char* format, ...);

/********************************************************************
 * 功能描述 : 格式化输入函数，读取格式化的字符串中的数据
 *            1、成功时函数返回正确处理并输入成功的格式个数，出错时返回-1
 *            2、当buffer为""时返回-1，并将第一个%s、%S、%c、%C或括号表达式对应的输入参数首字符置0
 *            3、当读到buffer结尾时没有输入成功任何数据，则函数返回-1；当fromat中格式不合法时按出错处理，返回-1
 *            4、如果格式化控制字符串（format）中包含了%s、%S、%c、%C或括号表达式（例如%[a-d]）时，函数中应该传入输入缓冲区长度参数，该参数表示最多能够输入到相应参数中的字符数量（字符串包括\0)，对于宽字符传入的缓冲区长度为宽字符数量。如果实际输入的数据超过缓冲区长度，则将缓冲区首字符置0，结束输入处理并返回已成功输入的格式个数
 * 输入参数 : buffer 存储的数据
 *            format 窗体控件字符串
 *            ...    可选自变量
 * 输出参数 : ...    可选自变量
 * 返回值   : 成功时函数返回正确处理并输入成功的格式个数，出错时返回-1
 ********************************************************************/
int sscanf_s (const char* buffer, const char* format, ...);

/********************************************************************
 * 功能描述 : 标准输入流输入函数
 *            1、新增参数destMax表示buffer最多容纳字符数量，包括'\0'字符
 *            2、如果读取到的数据是以\r或\n字符结尾的，则在buffer中\r或\n结尾字符替换为\0
 *            3、当输入的字符数超出destMax - 1时函数按出错处理，返回NULL
 *            4、为了兼容windows安全函数特性，在windows编译环境下允许destMax的值为(-1)，表示不校验buffer缓冲区大小
 * 输入参数 : buffer  存储的数据
 *            destMax 目标地址最大允许容纳的字符个数
 * 输出参数 : buffer  存储的数据
 * 返回值   : 成功时返回与参数buffer相同的指针，出错时返回NULL
 ********************************************************************/
char* gets_s(char* buffer, size_t destMax);

/********************************************************************
 * 功能描述 : 内存初始化函数，将目标地址的前count个字符的内容全部初始化为c指定的ASCII值，此函数通常用于初始化新申请的内存，destMax表示dest中最多容纳字节数量
 *            1、函数返回值与危险函数不同，操作成功返回EOK(0)，出错返回非0
 *            2、新增参数destMax表示dest中最多容纳字节数量，其余参数与memset函数用法一致
 *            3、当count > destMax时仅设置dest内存中的destMax个字节内容为c
 *            4.参数c中的低8位值有效(与memset特性一致)
 * 输入参数 : dest    目标地址
 *            destMax 目标地址最大允许容纳的字符个数
 *            c       初始化的ASCII值
 *            count   需要初始化的字符个数
 * 输出参数 : dest    目标地址
 * 返回值   : 成功返回EOK(0)，出错返回非0
 ********************************************************************/
errno_t memset_s(void* dest, size_t destMax, int c, size_t count);