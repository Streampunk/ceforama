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
#include <iostream>
#include "node_api.h"

#define DECLARE_NAPI_METHOD(name, func) { name, 0, func, 0, 0, 0, napi_default, 0 }
#define CHECK_STATUS if (checkStatus(env, status, __FILE__, __LINE__ - 1) != napi_ok) { return nullptr; }
#define PASS_STATUS if (status != napi_ok) return status

napi_status checkStatus(napi_env env, napi_status status,
  const char* file, uint32_t line);

// Async error handling
#define CEFORAMA_ERROR_START 9000
#define CEFORAMA_INVALID_ARGS 9001
#define CEFORAMA_LOOP_STATE 9002
#define CEFORAMA_SUCCESS 0

#define NAPI_THROW_ERROR(msg) { \
  char errorMsg[256]; \
  sprintf(errorMsg, "%s", msg); \
  napi_throw_error(env, nullptr, errorMsg); \
  return nullptr; \
}

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

napi_value nop(napi_env env, napi_callback_info info);

const std::string TICK_MESSAGE_NAME = "CeforamaTick";
const std::string REMOVE_MESSAGE_NAME = "CeforamaRemove";

#endif // CEFORAMA
