#include "ceforama_util.h"
#include <include/cef_app.h>
#include <include/cef_client.h>
#include <include/cef_render_handler.h>

class ceforama
    : public CefClient
{ 
    public:
        ceforama() {
            printf("Hello from ceforama\n");
        }

    bool OnProcessMessageReceived(CefRefPtr<CefBrowser>        browser,
                                  CefProcessId                 source_process,
                                  CefRefPtr<CefProcessMessage> message) override
    {
        return false;
    }

    IMPLEMENT_REFCOUNTING(ceforama);

};

napi_value printSomething(napi_env env, napi_callback_info info) {
    printf("Here comes a lovely string.\n");
    new ceforama();
    return nullptr;
}

napi_value Init(napi_env env, napi_value exports) {
    napi_status status;

    napi_property_descriptor desc[] = {
        DECLARE_NAPI_METHOD("printme", printSomething)
    };
    status = napi_define_properties(env, exports, 1, desc);
    CHECK_STATUS;

    return exports;
}

NAPI_MODULE(ceforama, Init)
