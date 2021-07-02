#ifndef __INPUT_CONTROLLER_H__
#define __INPUT_CONTROLLER_H__

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <forward_list>

#define KeyUnknown        GLFW_KEY_UNKNOWN
#define KeySpace          GLFW_KEY_SPACE
#define KeyApostrophe     GLFW_KEY_APOSTROPHE
#define KeyComma          GLFW_KEY_COMMA
#define KeyMinus          GLFW_KEY_MINUS
#define KeyPeriod         GLFW_KEY_PERIOD
#define KeySlash          GLFW_KEY_SLASH
#define Key0              GLFW_KEY_0
#define Key1              GLFW_KEY_1
#define Key2              GLFW_KEY_2
#define Key3              GLFW_KEY_3
#define Key4              GLFW_KEY_4
#define Key5              GLFW_KEY_5
#define Key6              GLFW_KEY_6
#define Key7              GLFW_KEY_7
#define Key8              GLFW_KEY_8
#define Key9              GLFW_KEY_9
#define KeySemicolon      GLFW_KEY_SEMICOLON
#define KeyEqual          GLFW_KEY_EQUAL
#define KeyA              GLFW_KEY_A
#define KeyB              GLFW_KEY_B
#define KeyC              GLFW_KEY_C
#define KeyD              GLFW_KEY_D
#define KeyE              GLFW_KEY_E
#define KeyF              GLFW_KEY_F
#define KeyG              GLFW_KEY_G
#define KeyH              GLFW_KEY_H
#define KeyI              GLFW_KEY_I
#define KeyJ              GLFW_KEY_J
#define KeyK              GLFW_KEY_K
#define KeyL              GLFW_KEY_L
#define KeyM              GLFW_KEY_M
#define KeyN              GLFW_KEY_N
#define KeyO              GLFW_KEY_O
#define KeyP              GLFW_KEY_P
#define KeyQ              GLFW_KEY_Q
#define KeyR              GLFW_KEY_R
#define KeyS              GLFW_KEY_S
#define KeyT              GLFW_KEY_T
#define KeyU              GLFW_KEY_U
#define KeyV              GLFW_KEY_V
#define KeyW              GLFW_KEY_W
#define KeyX              GLFW_KEY_X
#define KeyY              GLFW_KEY_Y
#define KeyZ              GLFW_KEY_Z
#define KeyLeftBracket    GLFW_KEY_LEFT_BRACKET
#define KeyBackslash      GLFW_KEY_BACKSLASH
#define KeyRightBracket   GLFW_KEY_RIGHT_BRACKET
#define KeyGraveAccent    GLFW_KEY_GRAVE_ACCENT
#define KeyWorld1         GLFW_KEY_WORLD_1
#define KeyWorld2         GLFW_KEY_WORLD_2
#define KeyEscape         GLFW_KEY_ESCAPE
#define KeyEnter          GLFW_KEY_ENTER
#define KeyTab            GLFW_KEY_TAB
#define KeyBackspace      GLFW_KEY_BACKSPACE
#define KeyInsert         GLFW_KEY_INSERT
#define KeyDelete         GLFW_KEY_DELETE
#define KeyRight          GLFW_KEY_RIGHT
#define KeyLeft           GLFW_KEY_LEFT
#define KeyDown           GLFW_KEY_DOWN
#define KeyUp             GLFW_KEY_UP
#define KeyPageUp         GLFW_KEY_PAGE_UP
#define KeyPageDown       GLFW_KEY_PAGE_DOWN
#define KeyHome           GLFW_KEY_HOME
#define KeyEnd            GLFW_KEY_END
#define KeyCapsLock       GLFW_KEY_CAPS_LOCK
#define KeyScrollLock     GLFW_KEY_SCROLL_LOCK
#define KeyNumLock        GLFW_KEY_NUM_LOCK
#define KeyPrintScreen    GLFW_KEY_PRINT_SCREEN
#define KeyPause          GLFW_KEY_PAUSE
#define KeyF1             GLFW_KEY_F1
#define KeyF2             GLFW_KEY_F2
#define KeyF3             GLFW_KEY_F3
#define KeyF4             GLFW_KEY_F4
#define KeyF5             GLFW_KEY_F5
#define KeyF6             GLFW_KEY_F6
#define KeyF7             GLFW_KEY_F7
#define KeyF8             GLFW_KEY_F8
#define KeyF9             GLFW_KEY_F9
#define KeyF10            GLFW_KEY_F10
#define KeyF11            GLFW_KEY_F11
#define KeyF12            GLFW_KEY_F12
#define KeyF13            GLFW_KEY_F13
#define KeyF14            GLFW_KEY_F14
#define KeyF15            GLFW_KEY_F15
#define KeyF16            GLFW_KEY_F16
#define KeyF17            GLFW_KEY_F17
#define KeyF18            GLFW_KEY_F18
#define KeyF19            GLFW_KEY_F19
#define KeyF20            GLFW_KEY_F20
#define KeyF21            GLFW_KEY_F21
#define KeyF22            GLFW_KEY_F22
#define KeyF23            GLFW_KEY_F23
#define KeyF24            GLFW_KEY_F24
#define KeyF25            GLFW_KEY_F25
#define KeyKp0            GLFW_KEY_KP_0
#define KeyKp1            GLFW_KEY_KP_1
#define KeyKp2            GLFW_KEY_KP_2
#define KeyKp3            GLFW_KEY_KP_3
#define KeyKp4            GLFW_KEY_KP_4
#define KeyKp5            GLFW_KEY_KP_5
#define KeyKp6            GLFW_KEY_KP_6
#define KeyKp7            GLFW_KEY_KP_7
#define KeyKp8            GLFW_KEY_KP_8
#define KeyKp9            GLFW_KEY_KP_9
#define KeyKpDecimal      GLFW_KEY_KP_DECIMAL
#define KeyKpDivide       GLFW_KEY_KP_DIVIDE
#define KeyKpMultiply     GLFW_KEY_KP_MULTIPLY
#define KeyKpSubtract     GLFW_KEY_KP_SUBTRACT
#define KeyKpAdd          GLFW_KEY_KP_ADD
#define KeyKpEnter        GLFW_KEY_KP_ENTER
#define KeyKpEqual        GLFW_KEY_KP_EQUAL
#define KeyLeftShift      GLFW_KEY_LEFT_SHIFT
#define KeyLeftControl    GLFW_KEY_LEFT_CONTROL
#define KeyLeftAlt        GLFW_KEY_LEFT_ALT
#define KeyLeftSuper      GLFW_KEY_LEFT_SUPER
#define KeyRightShift     GLFW_KEY_RIGHT_SHIFT
#define KeyRightControl   GLFW_KEY_RIGHT_CONTROL
#define KeyRightAlt       GLFW_KEY_RIGHT_ALT
#define KeyRightSuper     GLFW_KEY_RIGHT_SUPER
#define KeyMenu           GLFW_KEY_MENU
#define KeyLast           GLFW_KEY_MENU
// modifier keys bits
#define ModShift          GLFW_MOD_SHIFT
#define ModCtrl           GLFW_MOD_CONTROL
#define ModNumLock        GLFW_MOD_NUM_LOCK
#define ModCapsLock       GLFW_MOD_CAPS_LOCK
#define ModSuper          GLFW_MOD_SUPER
#define ModAlt            GLFW_MOD_ALT
// actions
#define ActionPress       GLFW_PRESS
#define ActionRepeat      GLFW_REPEAT
#define ActionRelease     GLFW_RELEASE

/*
 * INPUT CONTROLLER
 */

// forward declare to prevent include cycle
class Window;

// Callback is a collection of the data needed to execute a callback by the
// InputController. In order to avoid hacks to get lambda functions to work
// (since with captures, a lambda function cannot be converted to a function
// pointer), the user may use lambda functions without any captures by bundling
// the state that they need to act on into a "callback context", which is
// supplied to the callback hander as the first argument.
template<typename T>
struct Callback {
  T     cb_handler;
  void *cb_context;
};

// handle keyboard input as key press input
typedef void(*KeyEventHandler) (void *ctx, int key, int scancode, int action, int mods);
// handle keyboard input as character input
typedef void(*CharEventHandler) (void *ctx, unsigned int codepoint);

// InputController is an object which manages input from a single window
// context.
// User callbacks are executed last registered to first registered.
//
// By design, exactly one controller can be active at any time.
// This is a side effect of encapsulating the input controller as a class - we
// may not take a pointer to a non-static member function (which is needed when
// registering event callbacks with the window) because the implicit `this`
// argument cannot be injected by the window.
// Therefore, to avoid having any mutable global state, we have a static
// variable for the InputController class which points to the currently active
// controller.
// With this change, the callback functions can be made static and they
// internally refer to the currently active input controller static variable.
class InputController {
  // pointer to active controller
  static InputController             *s_activeController;
  // flags indicating whether key and character events are handled
  bool m_handleKeyEvents;
  bool m_handleCharEvents;
  // currently registered callbacks
  std::forward_list<Callback<KeyEventHandler>>  m_keyEventHanlders;
  std::forward_list<Callback<CharEventHandler>> m_charEventHandlers;
  // callback functions which are passed to the window
  static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
  static void charCallback(GLFWwindow* window, unsigned int codepoint);

  friend Window;
public:
  InputController();
  ~InputController();

  // activate / deactivate an input controller instance

  void Bind();
  void Unbind();

  // set which events are handled

  void setHandleKeyEvents(const bool);  // whether or not to handle key events
  void setHandleCharEvents(const bool); // whether or not to handle char events

  // register and de-register callbacks

  void addKeyEventHandler(const KeyEventHandler handler, void *ctx);
  void removeKeyEventHandler(const KeyEventHandler handler);
  void addCharEventHandler(const CharEventHandler handler, void *ctx);
  void removeCharEventHandler(const CharEventHandler handler);

  static bool isControllerBound();
  inline static int getScancode(const int key) { return glfwGetKeyScancode(key); }
};

#endif
