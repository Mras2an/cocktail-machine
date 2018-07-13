#ifndef _DEBUG_H_
#define _DEBUG_H_

#include "BspConfig.h"
#include "Bsp.h"

typedef enum eBarError {
    BAR_NO_ERROR          = 0,    // No errors.
    BAR_UNKNOWN_ERROR     = -100,
    BAR_INVALID_PARAMETER,
    BAR_JSON_PARSER_ERROR,
} eBarError_t;

eBarError_t Debug_printf(eBarError_t error, const char * format, ...);
void Debug_checkFailed(eBarError_t error, const char *function, int line, const char *functionCall);

#define BAR_DEBUG

#ifdef BAR_DEBUG
    #define BarDebug_info(msg, ...) OsPrintf("[INFO: %s, L%d] "msg, __FUNCTION__, __LINE__, ##__VA_ARGS__)
    #define BarDebug_err  OsDebug_err
#else
    #define BarDebug_info(msg, ...)
    #define BarDebug_err  OsDebug_err
#endif

#define BAR_ERROR(x)  Debug_printf(x, "[ERR : %s, L%d] 0x%08X \n",x ,__FUNCTION__ ,__LINE__ , x)

#ifdef NDEBUG
#define BAR_ERROR_CHECK(x) do {                                         \
        eBarError_t rc = (x);                                           \
        (void) sizeof(rc);                                              \
    } while(0);
#else
#define BAR_ERROR_CHECK(x) do {                                         \
        eBarError_t rc = (x);                                           \
        if (rc != BAR_NO_ERROR) {                                        \
            Debug_checkFailed(rc, __FUNCTION__, __LINE__, #x);          \
        }                                                               \
    } while(0);
#endif

#endif /* _DEBUG_H_ */
