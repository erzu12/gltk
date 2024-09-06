#include "window.h"
#include <iostream>
#include <thread>

namespace gltk {

void Window::framebuffer_size_callback(GLFWwindow* glfwWindow, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
    Window *window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
    window->width = width;
    window->height = height;
    window->rootLayout.setSize(MessureVec2(width, height));
    window->rootLayout.resolveTransform();
    window->rootLayout.registerForRenderRecursive();
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    Window* w = static_cast<Window*>(glfwGetWindowUserPointer(window));
    KeyModifierFlags key_mods(mods);
    if (action == GLFW_PRESS) {
        for(auto &callback : w->key_down_callbacks) {
            callback(static_cast<Key>(key), key_mods);
        }
    } else if (action == GLFW_RELEASE) {
        for(auto &callback : w->key_up_callbacks) {
            callback(static_cast<Key>(key), key_mods);
        }
    }
}

void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    float xscale, yscale;
    glfwGetWindowContentScale(window, &xscale, &yscale);
    Window* w = static_cast<Window*>(glfwGetWindowUserPointer(window));
    KeyModifierFlags key_mods(mods);
    if (action == GLFW_PRESS) {
        w->rootLayout.mouseKeyDownEventRecursive(Vec2(xpos * xscale, ypos * yscale), static_cast<MouseButton>(button), key_mods);
        for(auto &callback : w->mouse_down_callbacks) {
            callback(static_cast<MouseButton>(button), key_mods);
        }
    }
    else if (action == GLFW_RELEASE) {
        w->rootLayout.mouseKeyUpEventRecursive(Vec2(xpos * xscale, ypos * yscale), static_cast<MouseButton>(button), key_mods);
        for(auto &callback : w->mouse_up_callbacks) {
            callback(static_cast<MouseButton>(button), key_mods);
        }
    }
}

void Window::scrole_callback(GLFWwindow* window, double xoffset, double yoffset) {
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    float xscale, yscale;
    glfwGetWindowContentScale(window, &xscale, &yscale);
    Window* w = static_cast<Window*>(glfwGetWindowUserPointer(window));
    w->rootLayout.scrollEventRecursive(Vec2(xpos * xscale, ypos * yscale), Vec2(xoffset, yoffset));
}

void Window::cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    float xscale, yscale;
    glfwGetWindowContentScale(window, &xscale, &yscale);
    Window* w = static_cast<Window*>(glfwGetWindowUserPointer(window));
    Vec2 mousePos(xpos * xscale, ypos * yscale);
    w->lastMousePos = mousePos;
    for(auto &callback : w->mouse_move_callbacks) {
        callback(mousePos);
    }
}

Window::Window() {
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    window.reset(glfwCreateWindow(600, 800, "LearnOpenGL", NULL, NULL));
    if (window == nullptr)
    {
        glfwTerminate();
        throw window_exception("Failed to create GLFW window");
    }
    glfwMakeContextCurrent(window.get());
    glfwSwapInterval(0);
    glfwSetFramebufferSizeCallback(window.get(), framebuffer_size_callback);
    glfwSetWindowUserPointer(window.get(), this);
    glfwSetKeyCallback(window.get(), key_callback);
    glfwSetMouseButtonCallback(window.get(), mouse_button_callback);
    glfwSetScrollCallback(window.get(), scrole_callback);
    glfwSetCursorPosCallback(window.get(), cursor_position_callback);

    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (glfwRawMouseMotionSupported()) {
        glfwSetInputMode(window.get(), GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    }

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        throw window_exception("Failed to initialize GLAD");
    }

    glEnable( GL_DEBUG_OUTPUT );
    glDebugMessageCallback(debug_message_callback, 0 );

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void Window::run(std::function<void(Vec2)> render_callback) {
    glClearColor(.1f, .1f, .1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window.get());
    glfwPollEvents();
    rootLayout.resolveTransform();
    while (!glfwWindowShouldClose(window.get()))
    {
        if (renderer.willRender()) {
            glClearColor(.1f, .1f, .1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
        }

        Vec2 size = rootLayout.getSize();
        bool redraw = renderer.render(size);
        render_callback(Vec2(width, height));

        if (redraw) {
            glfwSwapBuffers(window.get());
        }
        glfwPollEvents();
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
}

Vec2 Window::get_mouse_pos() {
    return lastMousePos;
}

void Window::add_key_up_callback(std::function<void(Key key, KeyModifierFlags mods)> callback) {
    key_up_callbacks.push_back(callback);
}

void Window::add_key_down_callback(std::function<void(Key key, KeyModifierFlags mods)> callback) {
    key_down_callbacks.push_back(callback);
}

void Window::add_mouse_move_callback(std::function<void(Vec2)> callback) {
    mouse_move_callbacks.push_back(callback);
}

void Window::add_mouse_down_callback(std::function<void(MouseButton button, KeyModifierFlags mods)> callback) {
    mouse_down_callbacks.push_back(callback);
}

void Window::add_mouse_up_callback(std::function<void(MouseButton button, KeyModifierFlags mods)> callback) {
    mouse_up_callbacks.push_back(callback);
}

void Window::debug_message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam) {
    std::string full_message = "OpenGL message: ";
    switch (type) {
    case GL_DEBUG_TYPE_ERROR:
        full_message += "ERROR";
        break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        full_message += "DEPRECATED_BEHAVIOR";
        break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        full_message += "UNDEFINED_BEHAVIOR";
        break;
    case GL_DEBUG_TYPE_PORTABILITY:
        full_message += "PORTABILITY";
        break;
    case GL_DEBUG_TYPE_PERFORMANCE:
        full_message += "PERFORMANCE";
        break;
    case GL_DEBUG_TYPE_OTHER:
        full_message += "OTHER";
        break;
    }

    full_message += " " + std::to_string(id) + " ";

    switch (severity){
    case GL_DEBUG_SEVERITY_NOTIFICATION:
        full_message += "NOTIFICATION";
        break;
    case GL_DEBUG_SEVERITY_LOW:
        full_message += "LOW";
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        full_message += "MEDIUM";
        break;
    case GL_DEBUG_SEVERITY_HIGH:
        full_message += "HIGH";
        break;
    }

    full_message += " " + std::string(message);
    std::cout << full_message << std::endl;
}

Window::~Window() {
    glfwTerminate();
}


} // namespace gltk
