//
// MATLAB Compiler: 6.2 (R2016a)
// Date: Tue Mar 07 16:42:34 2017
// Arguments: "-B" "macro_default" "-W" "cpplib:NonlinerUKF8" "-T" "link:lib"
// "-d" "C:\Users\MVision\Desktop\ukf(1)\ukf\NonlinerUKF8\for_testing" "-v"
// "C:\Users\MVision\Desktop\ukf(1)\ukf\NonlinerUKF8.m" 
//

#ifndef __NonlinerUKF8_h
#define __NonlinerUKF8_h 1

#if defined(__cplusplus) && !defined(mclmcrrt_h) && defined(__linux__)
#  pragma implementation "mclmcrrt.h"
#endif
#include "mclmcrrt.h"
#include "mclcppclass.h"
#ifdef __cplusplus
extern "C" {
#endif

#if defined(__SUNPRO_CC)
/* Solaris shared libraries use __global, rather than mapfiles
 * to define the API exported from a shared library. __global is
 * only necessary when building the library -- files including
 * this header file to use the library do not need the __global
 * declaration; hence the EXPORTING_<library> logic.
 */

#ifdef EXPORTING_NonlinerUKF8
#define PUBLIC_NonlinerUKF8_C_API __global
#else
#define PUBLIC_NonlinerUKF8_C_API /* No import statement needed. */
#endif

#define LIB_NonlinerUKF8_C_API PUBLIC_NonlinerUKF8_C_API

#elif defined(_HPUX_SOURCE)

#ifdef EXPORTING_NonlinerUKF8
#define PUBLIC_NonlinerUKF8_C_API __declspec(dllexport)
#else
#define PUBLIC_NonlinerUKF8_C_API __declspec(dllimport)
#endif

#define LIB_NonlinerUKF8_C_API PUBLIC_NonlinerUKF8_C_API


#else

#define LIB_NonlinerUKF8_C_API

#endif

/* This symbol is defined in shared libraries. Define it here
 * (to nothing) in case this isn't a shared library. 
 */
#ifndef LIB_NonlinerUKF8_C_API 
#define LIB_NonlinerUKF8_C_API /* No special import/export declaration */
#endif

extern LIB_NonlinerUKF8_C_API 
bool MW_CALL_CONV NonlinerUKF8InitializeWithHandlers(
       mclOutputHandlerFcn error_handler, 
       mclOutputHandlerFcn print_handler);

extern LIB_NonlinerUKF8_C_API 
bool MW_CALL_CONV NonlinerUKF8Initialize(void);

extern LIB_NonlinerUKF8_C_API 
void MW_CALL_CONV NonlinerUKF8Terminate(void);



extern LIB_NonlinerUKF8_C_API 
void MW_CALL_CONV NonlinerUKF8PrintStackTrace(void);

extern LIB_NonlinerUKF8_C_API 
bool MW_CALL_CONV mlxNonlinerUKF8(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[]);


#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

/* On Windows, use __declspec to control the exported API */
#if defined(_MSC_VER) || defined(__BORLANDC__)

#ifdef EXPORTING_NonlinerUKF8
#define PUBLIC_NonlinerUKF8_CPP_API __declspec(dllexport)
#else
#define PUBLIC_NonlinerUKF8_CPP_API __declspec(dllimport)
#endif

#define LIB_NonlinerUKF8_CPP_API PUBLIC_NonlinerUKF8_CPP_API

#else

#if !defined(LIB_NonlinerUKF8_CPP_API)
#if defined(LIB_NonlinerUKF8_C_API)
#define LIB_NonlinerUKF8_CPP_API LIB_NonlinerUKF8_C_API
#else
#define LIB_NonlinerUKF8_CPP_API /* empty! */ 
#endif
#endif

#endif

extern LIB_NonlinerUKF8_CPP_API void MW_CALL_CONV NonlinerUKF8(int nargout, mwArray& x, mwArray& P, const mwArray& z, const mwArray& x_in1, const mwArray& P_in1, const mwArray& focalIndex, const mwArray& t, const mwArray& RelatObjCoor, const mwArray& euler2, const mwArray& T, const mwArray& flag);

#endif
#endif
