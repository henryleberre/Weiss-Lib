#pragma once

#include "WSPch.h"
#include "WSMath.h"

namespace WS {

	class Window;

	class Mouse {
		friend Window;
	private:
		RawVectorComponents<uint16_t, 2u> m_position { 0u, 0u };
		RawVectorComponents<int16_t,  2u> m_deltaPosition { 0u, 0u };

		int16_t m_wheelDelta = 0;

		bool m_bIsCursorInWindow  = false;
		bool m_bIsLeftButtonDown  = false;
		bool m_bIsRightButtonDown = false;

	public:
#ifdef __WEISS__OS_WINDOWS

		Mouse() WS_NOEXCEPT;

		~Mouse() WS_NOEXCEPT;

#elif defined(__WEISS__OS_LINUX)

		Mouse() = default;

		~Mouse() = default
;
#endif

		[[nodiscard]] inline uint16_t GetX() const noexcept { return this->m_position.x; }
		[[nodiscard]] inline uint16_t GetY() const noexcept { return this->m_position.y; }

		[[nodiscard]] inline RawVectorComponents<uint16_t, 2u> GetPosition() const noexcept { return this->m_position; }

		[[nodiscard]] inline int16_t GetWheelDelta() const noexcept { return this->m_wheelDelta; }

		[[nodiscard]] inline bool IsLeftButtonDown()  const noexcept { return this->m_bIsLeftButtonDown;  }
		[[nodiscard]] inline bool IsRightButtonDown() const noexcept { return this->m_bIsRightButtonDown; }
		[[nodiscard]] inline bool IsCursorInWindow()  const noexcept { return this->m_bIsCursorInWindow;  }

	private:
		void BeginUpdate() WS_NOEXCEPT;

#ifdef __WEISS__OS_WINDOWS
		std::optional<LRESULT> HandleEvent(const UINT msg, const WPARAM wParam, const LPARAM lParam) WS_NOEXCEPT;
#elif defined(__WEISS__OS_LINUX)
		bool HandleEvent(const XEvent& xEvent) WS_NOEXCEPT;
#endif
	};

}; // WS