#pragma once
// clang-format off
#include <glad/glad.h>
#include "GLFW/glfw3.h"
// clang-format on
#include "events.h"
#include "layout/relative_scene.h"
#include "layout/resolved_scene.h"
#include "vec_math.h"
#include <functional>
#include <memory>
#include <stdexcept>

namespace gltk {

class window_exception : public std::runtime_error {
  public:
    window_exception(const std::string &msg) : std::runtime_error(msg) {}
};

class Window {
  public:
    Window();
    ~Window();
    void add_key_down_callback(std::function<void(KeyEvent)> callback);
    void add_key_up_callback(std::function<void(KeyEvent)> callback);
    void add_text_input_callback(std::function<void(TextInputEvent)> callback);
    void add_mouse_move_callback(std::function<void(MouseMoveEvent)> callback);
    void add_mouse_down_callback(std::function<void(MouseButtonEvent)> callback);
    void add_mouse_up_callback(std::function<void(MouseButtonEvent)> callback);
    void run(std::function<void(Vec2)> render_callback);
    Vec2 get_mouse_pos();
    RelativeScene *getScene() { return relativeScene.get(); }

  private:
    static void framebuffer_size_callback(GLFWwindow *window, int width, int height);
    static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
    static void char_callback(GLFWwindow *window, unsigned int codepoint);
    static void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
    static void scrole_callback(GLFWwindow *window, double xoffset, double yoffset);
    static void debug_message_callback(
        GLenum source,
        GLenum type,
        GLuint id,
        GLenum severity,
        GLsizei length,
        const GLchar *message,
        const void *userParam
    );
    static void cursor_position_callback(GLFWwindow *window, double xpos, double ypos);
    std::vector<KeyModifiers> get_modifiers(int mods);

    std::unique_ptr<RelativeScene> relativeScene;
    std::unique_ptr<ResolvedScene> resolvedScene;

    std::unique_ptr<GLFWwindow, decltype(&glfwDestroyWindow)> window = {nullptr, glfwDestroyWindow};
    std::vector<std::function<void(KeyEvent)>> key_down_callbacks;
    std::vector<std::function<void(KeyEvent)>> key_up_callbacks;
    std::vector<std::function<void(TextInputEvent)>> text_input_callbacks;
    std::vector<std::function<void(MouseButtonEvent)>> mouse_down_callbacks;
    std::vector<std::function<void(MouseButtonEvent)>> mouse_up_callbacks;
    std::vector<std::function<void(MouseMoveEvent)>> mouse_move_callbacks;

    int width = 800;
    int height = 600;
    Vec2 lastMousePos;
    std::chrono::time_point<std::chrono::steady_clock> lastMouseButtonTime =
        std::chrono::steady_clock::time_point(std::chrono::milliseconds(0));
    MouseButton lastMouseButton = MouseButton::MOUSE_BUTTON_LEFT;
    int mouseButtonRepeat = 0;
};

} // namespace gltk
