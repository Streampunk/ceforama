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

    c->status = napi_create_external(env, c->client, nullptr, nullptr, &param);
    REJECT_STATUS;
    c->status = napi_set_named_property(env, result, "_external", param);
    REJECT_STATUS;

    napi_status status;
    status = napi_resolve_deferred(env, c->_deferred, result);
    FLOATING_STATUS;

    tidyCarrier(env, c);
}

napi_value client(napi_env env, napi_callback_info info) {
    return nullptr;
}

napi_value framePromise(napi_env env, napi_callback_info info) {
    return nullptr;
}

void frameResolver(napi_env env, napi_value jsCb, void* context, void* data) {

}

void paintoramaTsFnFinalize(napi_env env, void* data, void* hint) {

}