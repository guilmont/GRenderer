#pragma once

#include "core.h"

// local
#include "quad.h"
#include "shader.h"
#include "framebuffer.h"
#include "texture.h"
#include "events.h"
#include "camera.h"
#include "camera2D.h"
#include "fonts.h"
#include "dialog.h"
#include "mailbox.h"


namespace GRender {

    class Application
    {
    public:
        Application(const std::string& name, uint32_t width, uint32_t height, const std::filesystem::path& layout);
        virtual ~Application(void);

        // Flow control
        void closeApp(void) { glfwSetWindowShouldClose(window, 1); }
        void run(void);

        // To be implemented by user
        virtual void onUserUpdate(float deltaT) = 0;
        virtual void ImGuiLayer(void) {}
        virtual void ImGuiMenuLayer(void) {}

        // Basic utilities
        Fonts fonts;
        Mouse mouse;
        Keyboard keyboard;
        Dialog dialog;
        Mailbox mailbox;

        void scaleSizes(float scale); // Used to rescales sizes for HIDPI screens

    private:
        GLFWwindow* window = nullptr;
        float deltaTime = 0.1f;   // This value is going to be uploaded by main loop

        std::string layoutINI;

        // Callback functions to control flow
        friend void winResize_callback(GLFWwindow*, int, int);
        friend void winDrop_callback(GLFWwindow*, int, const char**);
    };

    // Used to manage entry point in Windows, Linux and Mac
    Application* createApplication();
}