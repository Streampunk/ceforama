#include "ceforama_util.h"
#include <include/cef_app.h>
#include <include/cef_client.h>
#include <include/cef_render_handler.h>
#include <include/cef_version.h>
#include <queue>

class ceforama
    : public CefClient,
      public CefRenderHandler,
      public CefLifeSpanHandler,
      public CefLoadHandler,
      public CefDisplayHandler
{ 
    CefRefPtr<CefBrowser> browser;
    int widhth;
    int height;
    double fps;
    string url;
    napi_threadsafe_function tsFn;

public:
    ceforama() {
        printf("Hello from ceforama\n");
    }

   CefRefPtr<CefBrowserHost> get_browser_host() const
    {
        if (browser != nullptr)
            return browser->GetHost();
        return nullptr;
    }

private:
    void OnAfterCreated(CefRefPtr<CefBrowser> browser) override
    {
        browser = std::move(browser);
    }

    void OnBeforeClose(CefRefPtr<CefBrowser> browser) override
    {
        browser = nullptr;
    }

    bool DoClose(CefRefPtr<CefBrowser> browser) override
    {
        return false;
    }  
    
    void GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect) override
    {
        // TODO confirm on the correct thread

        rect = CefRect(0, 0, 1920, 1080); // FIXME - make this dynamic
    }
    
    void OnPaint(CefRefPtr<CefBrowser> browser,
                 PaintElementType      type,
                 const RectList&       dirtyRects,
                 const void*           buffer,
                 int                   width,
                 int                   height) override
    {
    }

    // This blocks popup windows from opening, as they dont make sense and hit an exception in get_browser_host upon
    // closing
    bool OnBeforePopup(CefRefPtr<CefBrowser>   browser,
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
                       bool*                   no_javascript_access) override
    {
        return true;
    }

    bool OnProcessMessageReceived(CefRefPtr<CefBrowser>        browser,
                                  CefRefPtr<CefFrame>          frame,
                                  CefProcessId                 source_process,
                                  CefRefPtr<CefProcessMessage> message) override
    {
        // FIXME - implement
        return false;
    } 

    bool OnConsoleMessage(CefRefPtr<CefBrowser> browser,
                          cef_log_severity_t    level,
                          const CefString&      message,
                          const CefString&      source,
                          int                   line) override
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

    CefRefPtr<CefRenderHandler> GetRenderHandler() override { return this; }

    CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override { return this; }

    CefRefPtr<CefLoadHandler> GetLoadHandler() override { return this; }

    CefRefPtr<CefDisplayHandler> GetDisplayHandler() override { return this; }

    IMPLEMENT_REFCOUNTING(ceforama);
};

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
        DECLARE_NAPI_METHOD("version", version)
    };
    status = napi_define_properties(env, exports, 1, desc);
    CHECK_STATUS;

    return exports;
}

NAPI_MODULE(ceforama, Init)
