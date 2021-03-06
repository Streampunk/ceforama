#include <include/cef_version.h>
#include <queue>
#include "ceforama_util.h"
#include "client.h"
#include "loop.h"

napi_value version(napi_env env, napi_callback_info info) {
    napi_status status;
    napi_value value;

    status = napi_create_string_utf8(env, CEF_VERSION, NAPI_AUTO_LENGTH, &value);
    CHECK_STATUS;

    return value;
}

class remove_handler : public CefV8Handler
{
	CefRefPtr<CefBrowser> browser_;
public:
	remove_handler(CefRefPtr<CefBrowser> browser)
		: browser_(browser)
	{
	}

	bool Execute(
			const CefString& name,
			CefRefPtr<CefV8Value> object,
			const CefV8ValueList& arguments,
			CefRefPtr<CefV8Value>& retval,
			CefString& exception) override
	{
		if (!CefCurrentlyOn(TID_RENDERER))
			return false;

		browser_->GetMainFrame()->SendProcessMessage(
				PID_BROWSER,
				CefProcessMessage::Create(REMOVE_MESSAGE_NAME));

		return true;
	}

	IMPLEMENT_REFCOUNTING(remove_handler);
};

class renderer_application : public CefApp, CefRenderProcessHandler
{
	std::vector<CefRefPtr<CefV8Context>> contexts_;
	const bool enable_gpu_;
public:

	explicit renderer_application(const bool enable_gpu) : enable_gpu_(enable_gpu)
	{
	}

	CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() override
	{
		return this;
	}

	void OnContextCreated(
			CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			CefRefPtr<CefV8Context> context) override
	{
		/* caspar_log(browser, boost::log::trivial::trace,
				"context for frame "
				+ boost::lexical_cast<std::string>(frame->GetIdentifier())
				+ " created"); */
		contexts_.push_back(context);

		auto window = context->GetGlobal();

		window->SetValue(
				"remove",
				CefV8Value::CreateFunction(
						"remove",
						new remove_handler(browser)),
				V8_PROPERTY_ATTRIBUTE_NONE);

		CefRefPtr<CefV8Value> ret;
		CefRefPtr<CefV8Exception> exception;
		bool injected = context->Eval(R"(
			var requestedAnimationFrames	= {};
			var currentAnimationFrameId		= 0;

			window.requestAnimationFrame = function(callback) {
				requestedAnimationFrames[++currentAnimationFrameId] = callback;
				return currentAnimationFrameId;
			}

			window.cancelAnimationFrame = function(animationFrameId) {
				delete requestedAnimationFrames[animationFrameId];
			}

			function tickAnimations() {
				var requestedFrames = requestedAnimationFrames;
				var timestamp = performance.now();
				requestedAnimationFrames = {};

				for (var animationFrameId in requestedFrames)
					if (requestedFrames.hasOwnProperty(animationFrameId))
						requestedFrames[animationFrameId](timestamp);
			}
		)", CefString(), 1, ret, exception);

		if (!injected) {
			printf("Could not inject javascript animation code.\n");
		}
	}

	void OnContextReleased(
			CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			CefRefPtr<CefV8Context> context)
	{
        // FIXME make this work!
		/* auto removed = boost::remove_if(
				contexts_, [&](const CefRefPtr<CefV8Context>& c)
				{
					return c->IsSame(context);
				});

		if (removed != contexts_.end())
			caspar_log(browser, boost::log::trivial::trace,
					"context for frame "
					+ boost::lexical_cast<std::string>(frame->GetIdentifier())
					+ " released");
		else
			caspar_log(browser, boost::log::trivial::warning,
					"context for frame "
					+ boost::lexical_cast<std::string>(frame->GetIdentifier())
					+ " released, but not found"); */
	}

	void OnBrowserDestroyed(CefRefPtr<CefBrowser> browser) override
	{
		contexts_.clear();
	}
	
	void OnBeforeCommandLineProcessing(
		const CefString& process_type,
		CefRefPtr<CefCommandLine> command_line) override
	{
		printf("In OnBeforeCommandLineProcessing\n");
		//command_line->AppendSwitch("ignore-gpu-blacklist");
		if (enable_gpu_)
			command_line->AppendSwitch("enable-webgl");

		command_line->AppendSwitch("enable-begin-frame-scheduling");
		command_line->AppendSwitch("enable-media-stream");
		command_line->AppendSwitch("single-process");
		// command_line->AppendSwitchWithValue("multi_threaded_message_loop", "1");

		if (process_type.empty() && !enable_gpu_)
		{
			printf("Disabling GPU\n");
			// This gives more performance, but disabled gpu effects. Without it a single 1080p producer cannot be run smoothly
			command_line->AppendSwitch("disable-gpu");
			command_line->AppendSwitch("disable-gpu-compositing");
			command_line->AppendSwitchWithValue("disable-gpu-vsync", "gpu");
		}
	}

	bool OnProcessMessageReceived(
			CefRefPtr<CefBrowser> browser,
            CefRefPtr<CefFrame> frame,
			CefProcessId source_process,
			CefRefPtr<CefProcessMessage> message) override
	{
		// printf("OnProcessMessageReceived in browser\n");
		// printf("\n");
		if (message->GetName().ToString() == TICK_MESSAGE_NAME)
		{
			for (auto& context : contexts_)
			{
				CefRefPtr<CefV8Value> ret;
				CefRefPtr<CefV8Exception> exception;

				bool tar = context->Eval("tickAnimations()", CefString(), 1, ret, exception);
				// printf("About to eval tickAnimations() %i\n", tar);
			}

			return true;
		}
		else
		{
			return false;
		}
	}

	IMPLEMENT_REFCOUNTING(renderer_application);
};

napi_value Init(napi_env env, napi_value exports) {
    napi_status status;

    napi_property_descriptor desc[] = {
        DECLARE_NAPI_METHOD("version", version),
        DECLARE_NAPI_METHOD("client", client),
		DECLARE_NAPI_METHOD("startLoop", startLoop),
		DECLARE_NAPI_METHOD("stopLoop", stopLoop),
		DECLARE_NAPI_METHOD("doWork", doWork)
    };
    status = napi_define_properties(env, exports, 5, desc);
    CHECK_STATUS;

    const bool enable_gpu = false;
    CefMainArgs main_args;
	CefSettings settings;
	settings.command_line_args_disabled = false;
	settings.no_sandbox = true;
	settings.remote_debugging_port = 4242;
	settings.windowless_rendering_enabled = true;
	// settings.multi_threaded_message_loop = true;
	// settings.external_message_pump = 1;
   
	// CefString(&settings.resources_dir_path) = ".";
	// CefString(&settings.locales_dir_path) = "./locales";

	cef_string_ascii_to_utf16("cef.log", 7, &settings.log_file);
    CefInitialize(main_args, settings, CefRefPtr<CefApp>(new renderer_application(enable_gpu)), nullptr);
    // CefInitialize(main_args, settings, nullptr, nullptr);

    return exports;
}

NAPI_MODULE(ceforama, Init)
