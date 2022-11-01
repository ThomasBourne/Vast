#ifndef PACKET_H
#define PACKET_H

#include <Ultralight/Ultralight.h>
#include <AppCore/AppCore.h>
#include <JavaScriptCore/JavaScript.h>

#define WEB_PAGE "http://photography.thomasbourne.uk/NEW/"
namespace packet {
    void CreateWebWindow() {
        ultralight::Config config;

        config.resource_path = "./resources/";

        config.use_gpu_renderer = true;

        config.device_scale = 1.0;

        ultralight::Platform::instance().set_config(config);

        ultralight::Ref<ultralight::App> app = ultralight::App::Create();
        ultralight::Ref<ultralight::Window> window = ultralight::Window::Create(app->main_monitor(), 1920, 1080, false, ultralight::kWindowFlags_Titled);
        window->SetTitle("Ultralight Sample 2 - Basic App");
        app->set_window(window);
        ultralight::Ref<ultralight::Overlay> overlay = ultralight::Overlay::Create(window, window->width(), window->height(), 0, 0);
        overlay->view()->LoadURL(WEB_PAGE);
        app->Run();
    }
}

#endif