#include "input_controller.h"

// initially we have no bound input controller
InputController *InputController::s_activeController = nullptr;

bool InputController::isControllerBound() {
  return s_activeController != nullptr;
}

InputController::InputController()
  : m_handleKeyEvents{ false }, m_handleCharEvents{ false }
{ }

InputController::~InputController() {
  // unbind the controller if it is currently bound
  if (s_activeController == this) s_activeController = nullptr;
}

void InputController::keyCallback(
  GLFWwindow* window,
  int key, int scancode, int action, int mods
) {
  if (!s_activeController || !s_activeController->m_handleKeyEvents) return;
  for (Callback<KeyEventHandler> &cb : s_activeController->m_keyEventHanlders)
    cb.cb_handler(cb.cb_context, key, scancode, action, mods);
}

void InputController::charCallback(
  GLFWwindow *window,
  unsigned int codepoint
) {
  if (!s_activeController || !s_activeController->m_handleCharEvents) return;
  for (Callback<CharEventHandler> &cb : s_activeController->m_charEventHandlers)
    cb.cb_handler(cb.cb_context, codepoint);
}

void InputController::Bind() { s_activeController = this; }
void InputController::Unbind() { s_activeController = nullptr; }

void InputController::setHandleKeyEvents(const bool handleKeyEvents) {
  m_handleKeyEvents = handleKeyEvents;
}

void InputController::setHandleCharEvents(const bool handleCharEvents) {
  m_handleCharEvents = handleCharEvents;
}

void InputController::addKeyEventHandler(const KeyEventHandler handler, void *ctx) {
  m_keyEventHanlders.push_front({ handler, ctx });
}

void InputController::removeKeyEventHandler(const KeyEventHandler handler) {
  m_keyEventHanlders.remove_if([&](Callback<KeyEventHandler> &cb) {
    return cb.cb_handler == handler;
  });
}

void InputController::addCharEventHandler(const CharEventHandler handler, void *ctx) {
  m_charEventHandlers.push_front({ handler, ctx });
}

void InputController::removeCharEventHandler(const CharEventHandler handler) {
  m_charEventHandlers.remove_if([&](Callback<CharEventHandler> &cb) {
    return cb.cb_handler == handler;
  });
}
