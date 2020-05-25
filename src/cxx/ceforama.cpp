#include <include/cef_version.h>
#include <queue>
#include "ceforama_util.h"
#include "client.h"

napi_value version(napi_env env, napi_callback_info info) {
    napi_status status;
    napi_value value;

    status = napi_create_string_utf8(env, CEF_VERSION, NAPI_AUTO_LENGTH, &value);
    CHECK_STATUS;

    return value;
}

napi_value Init(napi_env env, napi_value exports) {
    napi_status status;

    napi_property_descriptor desc[] = {
        DECLARE_NAPI_METHOD("version", version),
        DECLARE_NAPI_METHOD("client", client)
    };
    status = napi_define_properties(env, exports, 1, desc);
    CHECK_STATUS;

    return exports;
}

NAPI_MODULE(ceforama, Init)
