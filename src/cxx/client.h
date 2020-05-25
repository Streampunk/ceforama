#ifndef CLIENT_H
#define CLIENT_H

#include <queue>
#include <string>
#include <include/cef_app.h>
#include <include/cef_client.h>
#include <include/cef_render_handler.h>
#include "ceforama_util.h"
#include "node_api.h"

napi_value client(napi_env env, napi_callback_info info);

class ceforama
    : public CefClient,
      public CefRenderHandler,
      public CefLifeSpanHandler,
      public CefLoadHandler,
      public CefDisplayHandler
{ 
    CefRefPtr<CefBrowser> browser;
    int width;
    int height;
    double fps;
    std::string url;
    napi_threadsafe_function tsFn;

public:
    ceforama();
    ~ceforama();

    CefRefPtr<CefBrowserHost> get_browser_host() const;

private:
    void OnAfterCreated(CefRefPtr<CefBrowser> browser) override;

    void OnBeforeClose(CefRefPtr<CefBrowser> browser) override;

    bool DoClose(CefRefPtr<CefBrowser> browser) override; 
    
    void GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect) override;
    
    void OnPaint(CefRefPtr<CefBrowser> browser,
                 PaintElementType      type,
                 const RectList&       dirtyRects,
                 const void*           buffer,
                 int                   width,
                 int                   height) override;
 
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
                       bool*                   no_javascript_access) override;

    bool OnProcessMessageReceived(CefRefPtr<CefBrowser>        browser,
                                  CefRefPtr<CefFrame>          frame,
                                  CefProcessId                 source_process,
                                  CefRefPtr<CefProcessMessage> message) override; 

    bool OnConsoleMessage(CefRefPtr<CefBrowser> browser,
                          cef_log_severity_t    level,
                          const CefString&      message,
                          const CefString&      source,
                          int                   line) override;

    CefRefPtr<CefRenderHandler> GetRenderHandler() override;

    CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override;

    CefRefPtr<CefLoadHandler> GetLoadHandler() override;

    CefRefPtr<CefDisplayHandler> GetDisplayHandler() override;

    IMPLEMENT_REFCOUNTING(ceforama);
};

#endif