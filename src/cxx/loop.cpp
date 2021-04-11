#include "loop.h"

bool isRunning = false;

void startLoopExecute(napi_env env, void* data) {
    printf("Starting CEF message loop\n");
    CefRunMessageLoop();
    printf("I don't get here\n");
}

void startLoopComplete(napi_env env, napi_status asyncStatus, void* data) {
    printf("Start loop complete\n");
}

napi_value startLoop(napi_env env, napi_callback_info info) {
	napi_value options, resourceName, promise;
	loopCarrier* c = new loopCarrier;
	
    c->status = napi_get_undefined(env, &promise);
    REJECT_RETURN;

    if (isRunning) {
        REJECT_ERROR_RETURN("Loop is already running.", CEFORAMA_LOOP_STATE);
    }

	c->status = napi_create_string_utf8(env, "StartLoopThread", NAPI_AUTO_LENGTH, &resourceName);
    REJECT_RETURN;
    c->status = napi_create_async_work(env, nullptr, resourceName, startLoopExecute, startLoopComplete, c, &c->_request);
    REJECT_RETURN;
    c->status = napi_queue_async_work(env, c->_request);
    REJECT_RETURN;
    isRunning = true;

    return promise;
}

napi_value stopLoop(napi_env env, napi_callback_info info) {
    napi_status status;
    napi_value result;
    
    status = napi_get_undefined(env, &result);
    CHECK_STATUS;

    printf("State of isRunning %s\n", isRunning ? "true" : "false");

    if (!isRunning) {
        NAPI_THROW_ERROR("Loop is not running.");
    }

    printf("About to stop message loop");
    CefQuitMessageLoop();
    return result;
}

napi_value doWork(napi_env env, napi_callback_info info) {
    napi_status status;
    napi_value result;
    
    status = napi_get_undefined(env, &result);
    CHECK_STATUS;

    CefDoMessageLoopWork();

    return result;
}