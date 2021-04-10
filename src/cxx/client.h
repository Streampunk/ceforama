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
napi_value framePromise(napi_env env, napi_callback_info info);
void frameResolver(napi_env env, napi_value jsCb, void* context, void* data);
void clientFinalize(napi_env env, void* data, void* hint);
void paintoramaTsFnFinalize(napi_env env, void* data, void* hint);
void frameFinalize(napi_env env, void* data, void* hint);
const std::string TICK_MESSAGE_NAME = "CasparCGTick";
const std::string REMOVE_MESSAGE_NAME = "CasparCGRemove";

struct frameCarrier : carrier {
    ~frameCarrier() { }
};

class clientorama
    : public CefClient,
      public CefRenderHandler,
      public CefLifeSpanHandler,
      public CefLoadHandler,
      public CefDisplayHandler
{ 
    CefRefPtr<CefBrowser> browser_ = nullptr;

public:
    int width;
    int height;
    double fps;
    std::string url;
    napi_threadsafe_function tsFn;
    uint32_t frameCount = 0;

    std::queue<frameCarrier*> framePromises;

    clientorama();
    ~clientorama();

    CefRefPtr<CefBrowserHost> get_browser_host() const;

    void update();
    void close();

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

    IMPLEMENT_REFCOUNTING(clientorama);
};

struct clientCarrier : carrier {
    CefRefPtr<clientorama> client = nullptr;
    uint32_t width = 1920;
    uint32_t height = 1080;
    double fps = 25.0;
    std::string url;
};

struct frameData {
    void* frame;
    size_t size;
};

#endif