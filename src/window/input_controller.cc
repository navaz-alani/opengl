#include "input_controller.h"

// initially we have no bound input controller
InputController *InputController::s_activeController = nullptr;

bool InputController::isControllerBound() {
  return s_activeController != nullptr;
}

InputController::InputController()
  : m_handleKeyEvents{ false }, m_handleCharEvents{ false }
{ }

void InputController::keyCallback(
  GLFWwindow* window,
  int key, int scancode, int action, int mods
) {
  if (!s_activeController || !s_activeController->m_handleKeyEvents) return;
  for (KeyEventHandler handler : s_activeController->m_keyEventHanlders)
    handler(key, scancode, action, mods);
}

void InputController::charCallback(
  GLFWwindow *window,
  unsigned int codepoint
) {
  if (!s_activeController || !s_activeController->m_handleCharEvents) return;
  for (CharEventHandler handler : s_activeController->m_charEventHandlers)
    handler(codepoint);
}

void InputController::Bind() { s_activeController = this; }
void InputController::Unbind() { s_activeController = nullptr; }

void InputController::setHandleKeyEvents(const bool handleKeyEvents) {
  m_handleKeyEvents = handleKeyEvents;
}

void InputController::setHandleCharEvents(const bool handleCharEvents) {
  m_handleCharEvents = handleCharEvents;
}

void InputController::addKeyEventHandler(const KeyEventHandler handler) {
  m_keyEventHanlders.push_front(handler);
}

void InputController::removeKeyEventHandler(const KeyEventHandler handler) {
  m_keyEventHanlders.remove(handler);
}

void InputController::addCharEventHandler(const CharEventHandler handler) {
  m_charEventHandlers.push_front(handler);
}

void InputController::removeCharEventHandler(const CharEventHandler handler) {
  m_charEventHandlers.remove(handler);
}
