//
//	InputManager.cpp
//	OpenGL Graphics
//
//	Created by Diego Revilla 14/01/23
//	Copyright © 2021 . All Rights reserved
//

#include "InputManager.h"
constexpr int c_sensibility = 5000;

namespace Engine {
	// ------------------------------------------------------------------------
	/*! Instance
	*
	*   Returns an instance of the Input Manager
	*/ // ---------------------------------------------------------------------
	InputManager& InputManager::Instance() noexcept {
		static InputManager inptmgr;

		return inptmgr;
	}

	// ------------------------------------------------------------------------
	/*! Is Key Pressed
	*
	*   Name says everything
	*/ // ---------------------------------------------------------------------
	bool InputManager::IsKeyPressed(const KeyCode key) noexcept {
		const bool previousState = mKeyboardState[key];

		return ((mKeyboardState[key] = IsKeyDown(key)) && !previousState);
	}

	// ------------------------------------------------------------------------
	/*! Is Key Released
	*
	*   Name says everything
	*/ // ---------------------------------------------------------------------
	bool InputManager::IsKeyReleased(const KeyCode key) noexcept {
		const bool previousState = mKeyboardState[key];

		return (!(mKeyboardState[key] = IsKeyDown(key)) && previousState);
	}

	// ------------------------------------------------------------------------
	/*! Get Mouse Movement
	*   Name says everything
	*/ // ---------------------------------------------------------------------
	glm::vec2 InputManager::GetMouseMovement() const noexcept {
		glm::vec2 pos = GetWindowCoordinatesMousePosition();
		pos -= mMousePositon;
		return pos;
	}

	// ------------------------------------------------------------------------
	/*! Set Mouse Position
	*   Name says everything
	*/ // ---------------------------------------------------------------------
	void InputManager::SetMousePosition(const unsigned w, const unsigned h) const noexcept {
		POINT p{ static_cast<LONG>(w), static_cast<LONG>(h) };
		ClientToScreen(GetActiveWindow(), &p);
		SetCursorPos(p.x, p.y);
	}

	// ------------------------------------------------------------------------
	/*! Process Input
	*
	*   Updates the Input Information for the application
	*/ // ---------------------------------------------------------------------
	void InputManager::ProcessInput() noexcept {
		for (KeyCode i = 0; i < 101; i++) mKeyboardState[i] = IsKeyDown(i);
		mMousePositon = GetWindowCoordinatesMousePosition();
	}

	// ------------------------------------------------------------------------
	/*! Get Window-Coordinates Mouse Position
	*
	*   Win32 Wrapper
	*/ // ---------------------------------------------------------------------
	glm::vec2 InputManager::GetWindowCoordinatesMousePosition() const noexcept {
		POINT p;
		GetCursorPos(&p);
		ScreenToClient(GetActiveWindow(), &p);

		return  { static_cast<float>(p.x), static_cast<float>(p.y) };
	}
}