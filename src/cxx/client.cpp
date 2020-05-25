#include "client.h"

ceforama::ceforama() {
    printf("Hello from ceforama\n");
}

ceforama::~ceforama() { }

CefRefPtr<CefBrowserHost> ceforama::get_browser_host() const
{
    if (browser != nullptr)
        return browser->GetHost();
    return nullptr;
}

void ceforama::OnAfterCreated(CefRefPtr<CefBrowser> browser)
{
    browser = std::move(browser);
}

void ceforama::OnBeforeClose(CefRefPtr<CefBrowser> browser)
{
    browser = nullptr;
}

bool ceforama::DoClose(CefRefPtr<CefBrowser> browser)
{
    return false;
}  
    
void ceforama::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect)
{
    // TODO confirm on the correct thread

    rect = CefRect(0, 0, 1920, 1080); // FIXME - make this dynamic
}
    
void ceforama::OnPaint(CefRefPtr<CefBrowser> browser,
                       PaintElementType      type,
                       const RectList&       dirtyRects,
                       const void*           buffer,
                       int                   width,
                       int                   height)
{

}

// This blocks popup windows from opening, as they dont make sense and hit an exception in get_browser_host upon
// closing
bool ceforama::OnBeforePopup(CefRefPtr<CefBrowser>   browser,
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

bool ceforama::OnProcessMessageReceived(CefRefPtr<CefBrowser>        browser,
                                        CefRefPtr<CefFrame>          frame,
                                        CefProcessId                 source_process,
                                        CefRefPtr<CefProcessMessage> message) 
{
    // FIXME - implement
    return false;
} 

bool ceforama::OnConsoleMessage(CefRefPtr<CefBrowser> browser,
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

CefRefPtr<CefRenderHandler> ceforama::GetRenderHandler() { return this; }

CefRefPtr<CefLifeSpanHandler> ceforama::GetLifeSpanHandler() { return this; }

CefRefPtr<CefLoadHandler> ceforama::GetLoadHandler() { return this; }

CefRefPtr<CefDisplayHandler> ceforama::GetDisplayHandler() { return this; }

napi_value client(napi_env env, napi_callback_info info) {
    return nullptr;
}