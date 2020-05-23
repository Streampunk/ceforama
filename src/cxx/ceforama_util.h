#ifndef CEFORAMA_UTIL
#define CEFORAMA_UTIL

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <cwchar>
#include <string>
#include <cstring>
#include <codecvt>
#include <locale>
#include <exception>
#include <inttypes.h>
#include "node_api.h"

#define DECLARE_NAPI_METHOD(name, func) { name, 0, func, 0, 0, 0, napi_default, 0 }
#define CHECK_STATUS if (checkStatus(env, status, __FILE__, __LINE__ - 1) != napi_ok) { return nullptr; }
#define PASS_STATUS if (status != napi_ok) return status

napi_status checkStatus(napi_env env, napi_status status,
  const char* file, uint32_t line);

// Async error handling
#define CEFORAMA_ERROR_START 6000
#define CEFORAMA_SUCCESS 0

#define NAPI_THROW_ERROR(msg) { \
  char errorMsg[256]; \
  sprintf(errorMsg, "%s", msg); \
  napi_throw_error(env, nullptr, errorMsg); \
  return nullptr; \
}

// #define NAPI_THROW_CORBA_EXCEPTION(ex) { \
//   char errorMsg[256]; \
//   snprintf(errorMsg, 256, "Exception thrown from CORBA subsystem: %s.", ex._name()); \
//   napi_throw_error(env, nullptr, errorMsg); \
//   return nullptr; \
// }

// #define NAPI_REJECT_CORBA_EXCEPTION(ex) { \
// 	char errorMsg[256]; \
// 	snprintf(errorMsg, 256, "Exception thrown from CORBA subsystem: %s.", ex._name()); \
// 	c->errorMsg = std::string(errorMsg); \
// 	c->status = QGW_CORBA_EXCEPTION; \
// 	return; \
// }

// #define NAPI_THROW_FATAL_EXCEPTION(fe) { \
//   char errorMsg[512]; \
//   snprintf(errorMsg, 512, "Omni ORB fatal exception thrown at line %i of %s: %s.", fe.line(), fe.file(), fe.errmsg()); \
//   napi_throw_error(env, nullptr, errorMsg); \
//   return nullptr; \
// }

// #define NAPI_REJECT_FATAL_EXCEPTION(fe) { \
// 	char errorMsg[512]; \
// 	snprintf(errorMsg, 512, "Omni ORB fatal exception thrown at line %i of %s: %s.", fe.line(), fe.file(), fe.errmsg()); \
// 	c->errorMsg = std::string(errorMsg); \
// 	c->status = QGW_FATAL_EXCEPTION; \
// 	return; \
// }

struct carrier {
  napi_ref passthru = nullptr;
  int32_t status = CEFORAMA_SUCCESS;
  std::string errorMsg;
  long long totalTime;
  napi_deferred _deferred;
  napi_async_work _request = nullptr;
	virtual ~carrier() {
	}
};

void tidyCarrier(napi_env env, carrier* c);
int32_t rejectStatus(napi_env env, carrier* c, char* file, int32_t line);

#define REJECT_STATUS if (rejectStatus(env, c, (char*) __FILE__, __LINE__) != CEFORAMA_SUCCESS) return;
#define REJECT_BAIL if (rejectStatus(env, c, (char*) __FILE__, __LINE__) != CEFORAMA_SUCCESS) goto bail;
#define REJECT_RETURN if (rejectStatus(env, c, (char*) __FILE__, __LINE__) != CEFORAMA_SUCCESS) return promise;
#define FLOATING_STATUS if (status != napi_ok) { \
  printf("Unexpected N-API status not OK in file %s at line %d value %i.\n", \
    __FILE__, __LINE__ - 1, status); \
}

#define REJECT_ERROR(msg, status) { \
  c->errorMsg = msg; \
  c->status = status; \
  REJECT_STATUS; \
}

#define REJECT_ERROR_RETURN(msg, stat) { \
  c->errorMsg = msg; \
  c->status = stat; \
  REJECT_RETURN; \
}

#endif // CEFORMAMA
