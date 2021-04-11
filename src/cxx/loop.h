#ifndef LOOP_H
#define LOOP_H

#include <include/cef_app.h>
#include <include/cef_client.h>
#include "ceforama_util.h"
#include "node_api.h"

napi_value startLoop(napi_env env, napi_callback_info info);
napi_value stopLoop(napi_env env, napi_callback_info info);
napi_value doWork(napi_env env, napi_callback_info info);

struct loopCarrier : carrier { 
    ~loopCarrier() { }
};

#endif