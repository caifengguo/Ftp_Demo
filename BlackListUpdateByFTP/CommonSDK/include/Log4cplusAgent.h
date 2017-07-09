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
 * \brief ��ʼ����־ģ�飬ͨ���ýӿ�Ψһ��ȷ�������ļ�
 * \param[in] logConfigFileName �����ļ���
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
     * \brief ��¼һ�����ȼ�ΪTRACE_LEVEL����־
     * \param[in] stringFormat ��ʽ����־�ַ���
     * \param[in] ... stringFormat�Ĳ���
     */
    void trace(const tchar* stringFormat, ...) throw();

    /** 
     * \brief ��¼һ�����ȼ�ΪTRACE_LEVEL����־
     * \param[in] message Ҫд����־�ļ�����Ϣ
     */
    void trace(const tstring& message) throw();

    /** 
     * \brief ��¼һ�����ȼ�ΪDEBUG_LEVEL����־
     * \param[in] stringFormat ��ʽ����־�ַ���
     * \param[in] ... stringFormat�Ĳ���
     */
    void debug(const tchar* stringFormat, ...) throw();

    /** 
     * \brief ��¼һ�����ȼ�ΪDEBUG_LEVEL����־
     * \param[in] message Ҫд����־�ļ�����Ϣ
     **/
    void debug(const tstring& message) throw();

    /** 
     * \brief ��¼һ�����ȼ�ΪINFO_LEVEL����־
     * \param[in] stringFormat ��ʽ����־�ַ���
     * \param[in] ... stringFormat�Ĳ���
     */
    void info(const tchar* stringFormat, ...) throw();

    /** 
     * \brief ��¼һ�����ȼ�ΪINFO_LEVEL����־
     * \param[in] message Ҫд����־�ļ�����Ϣ
     */
    void info(const tstring& message) throw();

    /** 
     * \brief ��¼һ�����ȼ�ΪWARN_LEVEL����־
     * \param[in] stringFormat ��ʽ����־�ַ���
     * \param[in] ... stringFormat�Ĳ���
     */
    void warn(const tchar* stringFormat, ...) throw();

    /** 
     * \brief ��¼һ�����ȼ�ΪWARN_LEVEL����־
     * \param[in] message Ҫд����־�ļ�����Ϣ
     */
    void warn(const tstring& message) throw();

    /** 
     * \brief ��¼һ�����ȼ�ΪERROR_LEVEL����־
     * \param[in] stringFormat ��ʽ����־�ַ���
     * \param[in] ... stringFormat�Ĳ���
     */
    void error(const tchar* stringFormat, ...) throw();

    /** 
     * \brief ��¼һ�����ȼ�ΪERROR_LEVEL����־
     * \param[in] message Ҫд����־�ļ�����Ϣ
     */
    void error(const tstring& message) throw();

    /** 
     * \brief ��¼һ�����ȼ�ΪFATAL_LEVEL����־
     * \param[in] stringFormat ��ʽ����־�ַ���
     * \param[in] ... stringFormat�Ĳ���
     */
    void fatal(const tchar* stringFormat, ...) throw();

    /** 
     * \brief ��¼һ�����ȼ�ΪFATAL_LEVEL����־
     * \param[in] message Ҫд����־�ļ�����Ϣ
     */
    void fatal(const tstring& message) throw();

    /**
     * \brief ��¼һ����־��Ϣ
     * \param[in] level ��־�ȼ�
     * \param[in] stringFormat ��ʽ����־�ַ���
     * \param[in] ... stringFormat�Ĳ���
     * \return NONE
     */
    void WriteLog(LogLevel level, const tchar* stringFormat, ...) throw();

    /**
     * \brief ��¼һ����־��Ϣ
     * \param[in] level ��־�ȼ�
     * \param[in] message Ҫд����־�ļ�����Ϣ
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
     * \brief ͨ���ⲿ�ļ�������־�����ʽ
     * \param[in] lpszCfgPathName �����ļ���ȫ·�������磺D:/Mylog.cfg
     * \return ���óɹ�����TRUE
     */
    void SetConfigFileName(tstring lpszCfgPathName); ///< ���������ļ���������

    /**
     * \brief �Կ���̨��ʽ���ָ��ģ�����־
     * \param[in] strModelName ģ������
     * \param[in] eMinLevel ��־�������͵ȼ�
     * \param[in] strPatton �����־��ʽ
     * \return NONE
     */
    void AddConsoleLogger(
        const tstring strModelName,
        LogLevel eMinLevel = NOT_SET_LEVEL,
        const tstring strPatton = _T("%D{%Y-%m-%d %H:%M:%S}  - %m%n"));

    /**
     * \brief ���ļ���ʽ���ָ��ģ�����־
     * \param[in] strModelName ģ������
     * \param[in] strLogFileName ��־�ļ���
     * \param[in] openMode ��־�ļ��򿪷�ʽ��֧�ֵķ�ʽ���£�
     - app
     - ate
     - binary
     - in 
     - out
     - trunc
     * \param[in] eMinLevel ��־�������͵ȼ�
     * \param[in] strPatton �����־��ʽ
     * \return NONE
     */
    void AddFileLogger(
        const tstring strModelName,
        const tstring strLogFileName,
        std::ios::openmode openMode = std::ios::app ,
        LogLevel eMinLevel = NOT_SET_LEVEL,
        tstring strPatton = _T("%D{%Y-%m-%d %H:%M:%S}  - %m%n"));

    /**
     * \brief ���ļ���ʽ���ָ��ģ�����־��ѭ����¼������Ԥ���趨�Ĵ�С
     (maxFileSize)�������Ƿ�ת�����������ô�С������log��Ϣ����浽���ļ��У�
     ���洢���ļ���Ŀ����maxBackupIndex+1ʱ����ɾ���������ɵ��ļ���
     ��֤�����ļ���Ŀ����maxBackupIndex+1 
     * \param[in] strModelName ģ������
     * \param[in] strLogFileName ��־�ļ���
     * \param[in] maxFileSize ������־�ļ�����ֽ�����Ĭ��10M
     * \param[in] maxBackupIndex ����ļ�����
     * \param[in] eMinLevel ��־�������͵ȼ�
     * \param[in] strPatton �����־��ʽ
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
     * \brief ���ļ���ʽ���ָ��ģ�����־������Ԥ���趨��Ƶ���������Ƿ�ת����
     ��������Ƶ�ȣ�����log��Ϣ����浽���ļ��У������Ƶ�Ȱ�����
     MONTHLY,WEEKLY,DAILY,TWICE_DAILY,HOURLY,MINUTELY
     * \param[in] strModelName ģ������
     * \param[in] strLogFileName ��־�ļ���
     * \param[in] eSchedule ��־��¼Ƶ��
     * \param[in] maxBackupIndex ����ļ���������Ƶ�Ƚϴ�ʱ���ã�
     * \param[in] eMinLevel ��־�������͵ȼ�
     * \param[in] strPatton �����־��ʽ
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