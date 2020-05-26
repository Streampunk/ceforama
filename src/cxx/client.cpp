#include "client.h"

clientorama::clientorama() {
    printf("Hello from clientorama\n");
}

clientorama::~clientorama() { }

CefRefPtr<CefBrowserHost> clientorama::get_browser_host() const
{
    if (browser != nullptr)
        return browser->GetHost();
    return nullptr;
}

void clientorama::close()
{
    if (browser != nullptr) {
        browser->GetHost()->CloseBrowser(true);
    }
}

void clientorama::OnAfterCreated(CefRefPtr<CefBrowser> browser)
{
    browser = std::move(browser);
}

void clientorama::OnBeforeClose(CefRefPtr<CefBrowser> browser)
{
    browser = nullptr;
}

bool clientorama::DoClose(CefRefPtr<CefBrowser> browser)
{
    return false;
}  
    
void clientorama::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect)
{
    // TODO confirm on the correct thread

    rect = CefRect(0, 0, this->width, this->height);
}
    
void clientorama::OnPaint(CefRefPtr<CefBrowser> browser,
                       PaintElementType      type,
                       const RectList&       dirtyRects,
                       const void*           buffer,
                       int                   width,
                       int                   height)
{

}

// This blocks popup windows from opening, as they dont make sense and hit an exception in get_browser_host upon
// closing
bool clientorama::OnBeforePopup(CefRefPtr<CefBrowser>   browser,
                             CefRefPtr<CefFrame>     frame,
                             const CefString&        target_url,
                             const CefString&        target_frame_name,
                             WindowOpenDisposition   target_disposition,
                             bool                    user_gesture,
                             const CefPopupFeatures& popupFeatures,
                             CefWindowInfo&          windowInfo,
                             CefRefPtr<CefClient>&   client,
                             CefBrowserSettings&     settings,
                             CefRefPtr<CefDictionaryValue>& extra_info,
                             bool*                   no_javascript_access)
{
    return true;
}

bool clientorama::OnProcessMessageReceived(CefRefPtr<CefBrowser>        browser,
                                        CefRefPtr<CefFrame>          frame,
                                        CefProcessId                 source_process,
                                        CefRefPtr<CefProcessMessage> message) 
{
    // FIXME - implement
    return false;
} 

bool clientorama::OnConsoleMessage(CefRefPtr<CefBrowser> browser,
                                cef_log_severity_t    level,
                                const CefString&      message,
                                const CefString&      source,
                                int                   line)
{
    if (level == cef_log_severity_t::LOGSEVERITY_DEBUG)
        std::wcout << L" Log: Debug: " << message.ToWString();
    else if (level == cef_log_severity_t::LOGSEVERITY_WARNING)
        std::wcout << L" Log: Warning: " << message.ToWString();
    else if (level == cef_log_severity_t::LOGSEVERITY_ERROR)
        std::wcerr << L" Log: Error: " << message.ToWString();
    else if (level == cef_log_severity_t::LOGSEVERITY_FATAL)
        std::wcerr << L" Log: Fatal: " << message.ToWString();
    else
        std::wcout << L" Log: " << message.ToWString();
    return true;
}

CefRefPtr<CefRenderHandler> clientorama::GetRenderHandler() { return this; }

CefRefPtr<CefLifeSpanHandler> clientorama::GetLifeSpanHandler() { return this; }

CefRefPtr<CefLoadHandler> clientorama::GetLoadHandler() { return this; }

CefRefPtr<CefDisplayHandler> clientorama::GetDisplayHandler() { return this; }

void clientExecute(napi_env env, void* data) {
    clientCarrier* c = (clientCarrier*) data;

    c->client = new clientorama;
    c->client->width = c->width;
    c->client->height = c->height;
    c->client->fps = c->fps;
    c->client->url = std::string(c->url);

    CefWindowInfo windowInfo;
    windowInfo.width = c->width;
    windowInfo.height = c->height;
    windowInfo.windowless_rendering_enabled = true;
    windowInfo.shared_texture_enabled = false;

    CefBrowserSettings browserSettings;
    browserSettings.web_security = cef_state_t::STATE_DISABLED;
    browserSettings.web_security = cef_state_t::STATE_DISABLED;
    browserSettings.windowless_frame_rate = int(ceil(c->fps));
    CefBrowserHost::CreateBrowser(windowInfo, c->client.get(), c->client->url, browserSettings, nullptr, nullptr);
}

void clientComplete(napi_env env, napi_status asyncStatus, void* data) {
    clientCarrier* c = (clientCarrier*) data;
    napi_value param, result, asyncName;
    
    if (asyncStatus != napi_ok) {
        c->status = asyncStatus;
        c->errorMsg = "Async capture creator failed to complete.";
    }
    REJECT_STATUS;

    c->status = napi_create_object(env, &result);
    REJECT_STATUS;
    c->status = napi_create_string_utf8(env, "clientorama", NAPI_AUTO_LENGTH, &param);
    REJECT_STATUS;
    c->status = napi_set_named_property(env, result, "type", param);
    REJECT_STATUS;

    c->status = napi_create_int32(env, c->width, &param);
    REJECT_STATUS;
    c->status = napi_set_named_property(env, result, "width", param);
    REJECT_STATUS;

    c->status = napi_create_int32(env, c->height, &param);
    REJECT_STATUS;
    c->status = napi_set_named_property(env, result, "height", param);
    REJECT_STATUS;

    c->status = napi_create_int32(env, int(ceil(c->fps)), &param);
    REJECT_STATUS;
    c->status = napi_set_named_property(env, result, "fps", param);
    REJECT_STATUS;

    c->status = napi_create_function(env, "frame", NAPI_AUTO_LENGTH, framePromise,
        nullptr, &param);
    REJECT_STATUS;
    c->status = napi_set_named_property(env, result, "frame", param);
    REJECT_STATUS;

    c->status = napi_create_string_utf8(env, "paintorama", NAPI_AUTO_LENGTH, &asyncName);
    REJECT_STATUS;
    c->status = napi_create_function(env, "nop", NAPI_AUTO_LENGTH, nop, nullptr, &param);
    REJECT_STATUS;
    c->status = napi_create_threadsafe_function(env, param, nullptr, asyncName,
        20, 1, nullptr, paintoramaTsFnFinalize, c->client, frameResolver, &c->client->tsFn);
    REJECT_STATUS;

    c->status = napi_create_external(env, c->client, clientFinalize, nullptr, &param);
    REJECT_STATUS;
    c->status = napi_set_named_property(env, result, "_external", param);
    REJECT_STATUS;

    napi_status status;
    status = napi_resolve_deferred(env, c->_deferred, result);
    FLOATING_STATUS;

    tidyCarrier(env, c);
}

napi_value client(napi_env env, napi_callback_info info) {
    napi_value options, param, promise, resourceName;
    napi_valuetype type;
    bool isArray;
    clientCarrier* c = new clientCarrier;

    c->status = napi_create_promise(env, &c->_deferred, &promise);
    REJECT_RETURN;

    size_t argc = 1;
    napi_value args[1];
    c->status = napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);
    REJECT_RETURN;

    if (argc >= 1) {
        c->status = napi_typeof(env, args[0], &type);
        REJECT_RETURN;
        c->status = napi_is_array(env, args[0], &isArray);
        REJECT_RETURN;
        if ((type != napi_object) || (isArray == true)) REJECT_ERROR_RETURN(
            "Options provided to capture create must be an object and not an array.",
            CEFORAMA_INVALID_ARGS);
        options = args[0];
    } 
    else {
        c->status = napi_create_object(env, &options);
        REJECT_RETURN;
    }



    return nullptr;
}

napi_value framePromise(napi_env env, napi_callback_info info) {
    return nullptr;
}

void frameResolver(napi_env env, napi_value jsCb, void* context, void* data) {

}

void clientFinalize(napi_env env, void* data, void* hint) {
    clientorama* client = (clientorama*) data;
    if (client != nullptr) {
        client->close();
    }
}

void paintoramaTsFnFinalize(napi_env env, void* data, void* hint) {

}
