#pragma once

#ifdef DLL_MODULE
#define DLL_EXPT __declspec(dllexport)
#else
#define DLL_EXPT
#endif

#include <string>
#include <tchar.h>
#include <sstream>

#ifdef UNICODE
typedef wchar_t tchar;
typedef std::wstring tstring;
#else
typedef char tchar;
typedef std::string tstring;
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief 初始化日志模块，通过该接口唯一的确定配置文件
 * \param[in] logConfigFileName 配置文件名
 * \return NONE
 */
void DLL_EXPT InitLogAgentLibrary(const tstring& logConfigFileName = _T(""));

enum LogLevel
{
    OFF_LEVEL = 60000,
    FATAL_LEVEL = 50000,
    ERROR_LEVEL = 40000,
    WARN_LEVEL = 30000,
    INFO_LEVEL = 20000,
    DEBUG_LEVEL = 10000,
    TRACE_LEVEL = 0,
    ALL_LEVEL = TRACE_LEVEL,
    NOT_SET_LEVEL = -1
};

class DLL_EXPT CLog4cplusAgent
{
private:
    CLog4cplusAgent(void);

public:
	CLog4cplusAgent(const tstring &strInstanceName);
	~CLog4cplusAgent(void);

public:
    /** 
     * \brief 记录一条优先级为TRACE_LEVEL的日志
     * \param[in] stringFormat 格式化日志字符串
     * \param[in] ... stringFormat的参数
     */
    void trace(const tchar* stringFormat, ...) throw();

    /** 
     * \brief 记录一条优先级为TRACE_LEVEL的日志
     * \param[in] message 要写入日志文件的信息
     */
    void trace(const tstring& message) throw();

    /** 
     * \brief 记录一条优先级为DEBUG_LEVEL的日志
     * \param[in] stringFormat 格式化日志字符串
     * \param[in] ... stringFormat的参数
     */
    void debug(const tchar* stringFormat, ...) throw();

    /** 
     * \brief 记录一条优先级为DEBUG_LEVEL的日志
     * \param[in] message 要写入日志文件的信息
     **/
    void debug(const tstring& message) throw();

    /** 
     * \brief 记录一条优先级为INFO_LEVEL的日志
     * \param[in] stringFormat 格式化日志字符串
     * \param[in] ... stringFormat的参数
     */
    void info(const tchar* stringFormat, ...) throw();

    /** 
     * \brief 记录一条优先级为INFO_LEVEL的日志
     * \param[in] message 要写入日志文件的信息
     */
    void info(const tstring& message) throw();

    /** 
     * \brief 记录一条优先级为WARN_LEVEL的日志
     * \param[in] stringFormat 格式化日志字符串
     * \param[in] ... stringFormat的参数
     */
    void warn(const tchar* stringFormat, ...) throw();

    /** 
     * \brief 记录一条优先级为WARN_LEVEL的日志
     * \param[in] message 要写入日志文件的信息
     */
    void warn(const tstring& message) throw();

    /** 
     * \brief 记录一条优先级为ERROR_LEVEL的日志
     * \param[in] stringFormat 格式化日志字符串
     * \param[in] ... stringFormat的参数
     */
    void error(const tchar* stringFormat, ...) throw();

    /** 
     * \brief 记录一条优先级为ERROR_LEVEL的日志
     * \param[in] message 要写入日志文件的信息
     */
    void error(const tstring& message) throw();

    /** 
     * \brief 记录一条优先级为FATAL_LEVEL的日志
     * \param[in] stringFormat 格式化日志字符串
     * \param[in] ... stringFormat的参数
     */
    void fatal(const tchar* stringFormat, ...) throw();

    /** 
     * \brief 记录一条优先级为FATAL_LEVEL的日志
     * \param[in] message 要写入日志文件的信息
     */
    void fatal(const tstring& message) throw();

    /**
     * \brief 记录一条日志信息
     * \param[in] level 日志等级
     * \param[in] stringFormat 格式化日志字符串
     * \param[in] ... stringFormat的参数
     * \return NONE
     */
    void WriteLog(LogLevel level, const tchar* stringFormat, ...) throw();

    /**
     * \brief 记录一条日志信息
     * \param[in] level 日志等级
     * \param[in] message 要写入日志文件的信息
     * \return NONE
     */
    void WriteLog(LogLevel level, const tstring& message) throw();

private:

    tstring m_strInstanceName;
};

enum DailyRollingSchedule { MONTHLY, WEEKLY, DAILY, TWICE_DAILY, HOURLY, MINUTELY};

class DLL_EXPT CLogConfig
{
public:
    CLogConfig();

public:
    /**
     * \brief 通过外部文件配置日志输出格式
     * \param[in] lpszCfgPathName 配置文件的全路径名，如：D:/Mylog.cfg
     * \return 设置成功返回TRUE
     */
    void SetConfigFileName(tstring lpszCfgPathName); ///< 加载配置文件进行配置

    /**
     * \brief 以控制台形式输出指定模块的日志
     * \param[in] strModelName 模块名称
     * \param[in] eMinLevel 日志输出的最低等级
     * \param[in] strPatton 输出日志格式
     * \return NONE
     */
    void AddConsoleLogger(
        const tstring strModelName,
        LogLevel eMinLevel = NOT_SET_LEVEL,
        const tstring strPatton = _T("%D{%Y-%m-%d %H:%M:%S}  - %m%n"));

    /**
     * \brief 以文件形式输出指定模块的日志
     * \param[in] strModelName 模块名称
     * \param[in] strLogFileName 日志文件名
     * \param[in] openMode 日志文件打开方式，支持的方式如下：
     - app
     - ate
     - binary
     - in 
     - out
     - trunc
     * \param[in] eMinLevel 日志输出的最低等级
     * \param[in] strPatton 输出日志格式
     * \return NONE
     */
    void AddFileLogger(
        const tstring strModelName,
        const tstring strLogFileName,
        std::ios::openmode openMode = std::ios::app ,
        LogLevel eMinLevel = NOT_SET_LEVEL,
        tstring strPatton = _T("%D{%Y-%m-%d %H:%M:%S}  - %m%n"));

    /**
     * \brief 以文件形式输出指定模块的日志，循环记录，根据预先设定的大小
     (maxFileSize)来决定是否转储，当超过该大小，后续log信息会另存到新文件中，
     当存储的文件数目超过maxBackupIndex+1时，会删除最早生成的文件，
     保证整个文件数目等于maxBackupIndex+1 
     * \param[in] strModelName 模块名称
     * \param[in] strLogFileName 日志文件名
     * \param[in] maxFileSize 单个日志文件最大字节数，默认10M
     * \param[in] maxBackupIndex 最大文件个数
     * \param[in] eMinLevel 日志输出的最低等级
     * \param[in] strPatton 输出日志格式
     * \return NONE
     */
    void AddRollingFileLogger(
        const tstring strModelName,
        const tstring strLogFileName, 
        long maxFileSize = 10 * 1024 * 1024,
        int maxBackupIndex = 5, 
        LogLevel eMinLevel = NOT_SET_LEVEL,
        tstring strPatton = _T("%D{%Y-%m-%d %H:%M:%S}  - %m%n"));



    /**
     * \brief 以文件形式输出指定模块的日志，根据预先设定的频度来决定是否转储，
     当超过该频度，后续log信息会另存到新文件中，这里的频度包括：
     MONTHLY,WEEKLY,DAILY,TWICE_DAILY,HOURLY,MINUTELY
     * \param[in] strModelName 模块名称
     * \param[in] strLogFileName 日志文件名
     * \param[in] eSchedule 日志记录频度
     * \param[in] maxBackupIndex 最大文件个数（当频度较大时有用）
     * \param[in] eMinLevel 日志输出的最低等级
     * \param[in] strPatton 输出日志格式
     * \return NONE
     */
    void AddDailyRollingFileLogger(
        const tstring strModelName,
        const tstring strLogFileName, 
        DailyRollingSchedule eSchedule = DAILY,
        int maxBackupIndex = 20, 
        LogLevel eMinLevel = NOT_SET_LEVEL,
        tstring strPatton = _T("%D{%Y-%m-%d %H:%M:%S}  - %m%n"));
};

#ifdef __cplusplus
}
#endif