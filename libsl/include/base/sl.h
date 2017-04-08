/*
 * sl.h
 *
 *  Created on: Apr 18, 2016
 *      Author: root
 */

#ifndef SL_H_
#define SL_H_

/**
 * Type for specifying an error or status code.
 */
typedef int sl_status_t;

#define SL_TRUE		1
#define SL_FALSE	0

typedef int sl_bool;


/**
 * SL public API wrap for C++ compilers.
 */
#ifdef __cplusplus
#define SL_BEGIN_DECLS     extern "C" {
#define SL_END_DECLS       }
#else
#define SL_BEGIN_DECLS
#define SL_END_DECLS
#endif

#if defined(DOXYGEN) || !defined(WIN32)

#define SL_DECLARE(type)            type
#define SL_DECLARE_NONSTD(type)     type
#define SL_DECLARE_DATA

#elif defined(SL_DECLARE_STATIC)
#define SL_DECLARE(type)            type __stdcall
#define SL_DECLARE_NONSTD(type)     type __cdecl
#define SL_DECLARE_DATA
#elif defined(SL_DECLARE_EXPORT)
#define SL_DECLARE(type)            __declspec(dllexport) type __stdcall
#define SL_DECLARE_NONSTD(type)     __declspec(dllexport) type __cdecl
#define SL_DECLARE_DATA             __declspec(dllexport)
#else
#define SL_DECLARE(type)            __declspec(dllimport) type __stdcall
#define SL_DECLARE_NONSTD(type)     __declspec(dllimport) type __cdecl
#define SL_DECLARE_DATA             __declspec(dllimport)
#endif

#endif /* SL_H_ */
