#ifndef INPUT_HPP
#define INPUT_HPP
#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <vector>

#include "eventpp/callbacklist.h"
#include "glm/glm.hpp"
#include "math/vector2.hpp"

class Window;

namespace Input {

enum class Key {
	/* The unknown key */
	Unknown = -1,

	/* Printable keys */
	Space = 32,
	Apostrophe = 39, /* ' */
	Comma = 44,		 /* , */
	Minus = 45,		 /* - */
	Period = 46,	 /* . */
	Slash = 47,		 /* / */
	Zero = 48,
	One = 49,
	Two = 50,
	Three = 51,
	Four = 52,
	Five = 53,
	Six = 54,
	Seven = 55,
	Eight = 56,
	Nine = 57,
	Semicolon = 59, /* ; */
	Equal = 61,		/* = */
	A = 65,
	B = 66,
	C = 67,
	D = 68,
	E = 69,
	F = 70,
	G = 71,
	H = 72,
	I = 73,
	J = 74,
	K = 75,
	L = 76,
	M = 77,
	N = 78,
	O = 79,
	P = 80,
	Q = 81,
	R = 82,
	S = 83,
	T = 84,
	U = 85,
	V = 86,
	W = 87,
	X = 88,
	Y = 89,
	Z = 90,
	LeftBracket = 91,  /* [ */
	Backslash = 92,	   /* \ */
	RightBracket = 93, /* ] */
	GraveAccent = 96,  /* ` */
	World1 = 161,	   /* non-US #1 */
	World2 = 162,	   /* non-US #2 */

	/* Function keys */
	Escape = 256,
	Enter = 257,
	Tab = 258,
	Backspace = 259,
	Insert = 260,
	Delete = 261,
	Right = 262,
	Left = 263,
	Down = 264,
	Up = 265,
	PageUp = 266,
	PageDown = 267,
	Home = 268,
	End = 269,
	CapsLock = 280,
	ScrollLock = 281,
	NumLock = 282,
	PrintScreen = 283,
	Pause = 284,
	F1 = 290,
	F2 = 291,
	F3 = 292,
	F4 = 293,
	F5 = 294,
	F6 = 295,
	F7 = 296,
	F8 = 297,
	F9 = 298,
	F10 = 299,
	F11 = 300,
	F12 = 301,
	F13 = 302,
	F14 = 303,
	F15 = 304,
	F16 = 305,
	F17 = 306,
	F18 = 307,
	F19 = 308,
	F20 = 309,
	F21 = 310,
	F22 = 311,
	F23 = 312,
	F24 = 313,
	F25 = 314,
	Keypad0 = 320,
	Keypad1 = 321,
	Keypad2 = 322,
	Keypad3 = 323,
	Keypad4 = 324,
	Keypad5 = 325,
	Keypad6 = 326,
	Keypad7 = 327,
	Keypad8 = 328,
	Keypad9 = 329,
	KeypadDecimal = 330,
	KeypadDivide = 331,
	KeypadMultiply = 332,
	KeypadSubtract = 333,
	KeypadAdd = 334,
	KeypadEnter = 335,
	KeypadEqual = 336,
	LeftShift = 340,
	LeftControl = 341,
	LeftAlt = 342,
	LeftSuper = 343,
	RightShit = 344,
	RightControl = 345,
	RightAlt = 346,
	RightSuper = 347,
	Menu = 348,

	Mouse1 = 10001,
	Mouse2 = 10002,
	Mouse3 = 10003,
	Mouse4 = 10004,
	Mouse5 = 10005,
	Mouse6 = 10006,
	Mouse7 = 10007,
	Mouse8 = 10008,
	MouseLeft = Mouse1,
	MouseRight = Mouse2,
	MouseMiddle = Mouse3,
};

enum class EventType {
	Key = 0,
	Button,
	MouseMove,
	Scroll,
};
struct Event {
	EventType type;

	struct {
		Key key;
		int action;
	} key;

	struct {
		Key key;
		int action;
	} button;

	struct {
		double deltaX;
		double deltaY;
	} mouseMove;

	struct {
		double xOffset;
		double yOffset;
	} scroll;
};

void InitState(Window *window);

void Poll();
bool IsKeyDown(Key key);
bool IsKeyJustPressed(Key key);
bool IsKeyJustReleased(Key key);
void SetActionKeys(const char *actionName, const std::vector<Key> &keys);
const std::vector<Key> &GetActionKeys(const char *actionName);
bool IsActionPressed(const char *actionName);
bool IsActionJustPressed(const char *actionName);
bool IsActionJustReleased(const char *actionName);
float GetActionWeight(const char *negativeAction, const char *positiveAction);
Vector2 GetActionWeight2(const char *negativeActionX, const char *positiveActionX, const char *negativeActionY, const char *positiveActionY);

glm::vec2 GetMousePosition();
float GetMouseScrollY();
float GetMouseScrollX();

eventpp::CallbackList<void(Event e)> &InputEvent();

namespace Callback {
void Key(GLFWwindow *window, int key, int scancode, int action, int mods);
void MouseButton(GLFWwindow *window, int button, int action, int mods);
void MouseMove(GLFWwindow *window, double x, double y);
void Scroll(GLFWwindow *window, double xOffset, double yOffset);
} // namespace Callback
}; // namespace Input

using Input::Key;

#endif // INPUT_HPP