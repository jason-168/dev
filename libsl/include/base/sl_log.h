/*
 * sl_log.h
 *
 *  Created on: Nov 27, 2015
 *      Author: root
 */

#ifndef SL_LOG_H_
#define SL_LOG_H_

#ifdef __cplusplus
extern "C" {
#endif

#if defined(ANDROID) && !defined(SL_USING_LOGFILE)
#include <android/log.h>

#else
#define USE_LOGGER 1

/*
 * Android log priority values, in ascending priority order.
 */
typedef enum android_LogPriority {
    ANDROID_LOG_UNKNOWN = 0,
    ANDROID_LOG_DEFAULT,    /* only for SetMinPriority() */
    ANDROID_LOG_VERBOSE,
    ANDROID_LOG_DEBUG,
    ANDROID_LOG_INFO,
    ANDROID_LOG_WARN,
    ANDROID_LOG_ERROR,
    ANDROID_LOG_FATAL,
    ANDROID_LOG_SILENT,     /* only for SetMinPriority(); must be last */
} android_LogPriority;

#include <base/sl_log_helper.h>
#endif //ANDROID end


int __sl_log_check_isNULL();
// ---------------------------------------------------------------------

/*
 * Normally we strip ALOGV (VERBOSE messages) from release builds.
 * You can modify this (for example with "#define LOG_NDEBUG 0"
 * at the top of your source file) to change that behavior.
 */
#ifndef LOG_NDEBUG
#ifdef NDEBUG
#define LOG_NDEBUG 1
#else
#define LOG_NDEBUG 0
#endif
#endif

/*
 * This is the local tag used for the following simplified
 * logging macros.  You can change this preprocessor definition
 * before using the other macros to change the tag.
 */
#ifndef LOG_TAG
#define LOG_TAG NULL
#endif

// ---------------------------------------------------------------------

#ifndef __predict_false
#define __predict_false(exp) __builtin_expect((exp) != 0, 0)
#endif

/*
 * Simplified macro to send a verbose log message using the current LOG_TAG.
 */
#ifndef ALOGV
#define ALOGV(...) ((void)ALOG(LOG_VERBOSE, LOG_TAG, __VA_ARGS__))
#endif

#ifndef ALOGV_IF
#define ALOGV_IF(cond, ...) \
    ( (__predict_false(cond)) \
    ? ((void)ALOG(LOG_VERBOSE, LOG_TAG, __VA_ARGS__)) \
    : (void)0 )
#endif

/*
 * Simplified macro to send a debug log message using the current LOG_TAG.
 */
#ifndef ALOGD
#ifdef DEBUG
#define ALOGD(...) ((void)ALOG(LOG_DEBUG, LOG_TAG, __VA_ARGS__))
#else
#define ALOGD(...) ((void)0)
#endif
#endif

#ifndef ALOGD_IF
#ifdef DEBUG
#define ALOGD_IF(cond, ...) ((void)ALOG(LOG_DEBUG, LOG_TAG, __VA_ARGS__))
#else
#define ALOGD_IF(cond, ...) \
    ( (__predict_false(cond)) \
    ? ((void)ALOG(LOG_DEBUG, LOG_TAG, __VA_ARGS__)) \
    : (void)0 )
#endif
#endif

/*
 * Simplified macro to send an info log message using the current LOG_TAG.
 */
#ifndef ALOGI
#define ALOGI(...) ((void)ALOG(LOG_INFO, LOG_TAG, __VA_ARGS__))
#endif

#ifndef ALOGI_IF
#define ALOGI_IF(cond, ...) \
    ( (__predict_false(cond)) \
    ? ((void)ALOG(LOG_INFO, LOG_TAG, __VA_ARGS__)) \
    : (void)0 )
#endif

/*
 * Simplified macro to send a warning log message using the current LOG_TAG.
 */
#ifndef ALOGW
#define ALOGW(...) ((void)ALOG(LOG_WARN, LOG_TAG, __VA_ARGS__))
#endif

#ifndef ALOGW_IF
#define ALOGW_IF(cond, ...) \
    ( (__predict_false(cond)) \
    ? ((void)ALOG(LOG_WARN, LOG_TAG, __VA_ARGS__)) \
    : (void)0 )
#endif

/*
 * Simplified macro to send an error log message using the current LOG_TAG.
 */
#ifndef ALOGE
#define ALOGE(...) ((void)ALOG(LOG_ERROR, LOG_TAG, __VA_ARGS__))
#endif

#ifndef ALOGE_IF
#define ALOGE_IF(cond, ...) \
    ( (__predict_false(cond)) \
    ? ((void)ALOG(LOG_ERROR, LOG_TAG, __VA_ARGS__)) \
    : (void)0 )
#endif

// ---------------------------------------------------------------------

void __sl_log_assert(const char* file, const char* func, int line, const char* strexpr);

void __sl_log_fatal(const char* file, const char* func, int line,
		const char* strexpr, long int v1, long int v2);


// Helper macro for binary operators.
// Don't use this macro directly in your code, use SL_CHECK_EQ et al below.
//
#define SL_CHECK_OP(name, op, val1, val2) \
__Check##name##Impl((val1), (val2), #val1 " " #op " " #val2, __FILE__, __func__, __LINE__)


#define DEFINE_SL_CHECK_OP_IMPL(name, op)                                   \
  static inline void __Check##name##Impl(long int v1, long int v2, const char* __strexpr, \
	const char* file, const char* func, int line) { 						\
    if (__predict_false(!(v1 op v2)))                                       \
    	__sl_log_fatal(file, func, line, __strexpr, v1, v2);         		\
  }

DEFINE_SL_CHECK_OP_IMPL(EQ, ==)
DEFINE_SL_CHECK_OP_IMPL(NE, !=)
DEFINE_SL_CHECK_OP_IMPL(LE, <=)
DEFINE_SL_CHECK_OP_IMPL(LT, < )
DEFINE_SL_CHECK_OP_IMPL(GE, >=)
DEFINE_SL_CHECK_OP_IMPL(GT, > )

#define SL_CHECK_EQ(val1, val2) SL_CHECK_OP(EQ, ==, val1, val2)
#define SL_CHECK_NE(val1, val2) SL_CHECK_OP(NE, !=, val1, val2)
#define SL_CHECK_LE(val1, val2) SL_CHECK_OP(LE, <=, val1, val2)
#define SL_CHECK_LT(val1, val2) SL_CHECK_OP(LT, < , val1, val2)
#define SL_CHECK_GE(val1, val2) SL_CHECK_OP(GE, >=, val1, val2)
#define SL_CHECK_GT(val1, val2) SL_CHECK_OP(GT, > , val1, val2)

// ---------------------------------------------------------------------

//assert true
#ifndef ALOG_ASSERT
#define ALOG_ASSERT(cond) \
    ( (__predict_false(cond)) \
    ? ((void)__sl_log_assert(__FILE__, __func__, __LINE__, #cond)) \
    : (void)0 )
#endif

// ---------------------------------------------------------------------

/*
 * Basic log message macro.
 *
 * Example:
 *  ALOG(LOG_WARN, NULL, "Failed with error %d", errno);
 *
 * The second argument may be NULL or "" to indicate the "global" tag.
 */
#ifndef ALOG
#define ALOG(priority, tag, ...) \
    LOG_PRI(ANDROID_##priority, tag, __VA_ARGS__)
#endif

/*
 * Log macro that allows you to specify a number for the priority.
 */
#ifndef LOG_PRI
#define LOG_PRI(priority, tag, ...) \
    android_printLog(priority, tag, __VA_ARGS__)
#endif

/*
 * ===========================================================================
 *
 * The stuff in the rest of this file should not be used directly.
 */
#ifndef android_printLog
#if defined(ANDROID) && !defined(SL_USING_LOGFILE)
#define android_printLog(prio, tag, fmt...) __android_log_print(prio, tag, fmt)
#else
#define android_printLog(prio, tag, fmt...) __sl_log_print(prio, tag, fmt)
#endif
#endif

///*
// * Send a simple string to the log.
// */
//int __android_log_buf_write(int bufID, int prio, const char *tag, const char *text);
//int __android_log_buf_print(int bufID, int prio, const char *tag, const char *fmt, ...)
//#if defined(__GNUC__)
//    __attribute__((__format__(printf, 4, 5)))
//#endif
//    ;

//#endif

#ifdef __cplusplus
}
#endif

#endif /* SL_LOG_H_ */
