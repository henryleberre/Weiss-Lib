#pragma once

#include "../misc/WSPch.h"
#include "../math/WSVector.h"

namespace WS {

	class Window;

	class Mouse {
	friend WS::Window;
	
	private:
		RawVectorComponents<uint16_t, 2u> m_position { 0u, 0u };
		RawVectorComponents<int16_t,  2u> m_deltaPosition { 0u, 0u };

		int16_t m_wheelDelta = 0;

		bool m_bIsLeftButtonDown  = false;
		bool m_bIsRightButtonDown = false;

	private:
		inline void PrepareForUpdate() noexcept
		{
			this->m_deltaPosition = { 0, 0 };
			this->m_wheelDelta    = 0;
		}

	public:
#ifdef __WEISS__OS_WINDOWS
		Mouse() WS_NOEXCEPT;
#endif

		[[nodiscard]] inline uint16_t GetX() const noexcept { return this->m_position.x; }
		[[nodiscard]] inline uint16_t GetY() const noexcept { return this->m_position.y; }

		[[nodiscard]] inline RawVectorComponents<uint16_t, 2u> GetPosition() const noexcept { return this->m_position; }

		[[nodiscard]] inline int16_t GetWheelDelta() const noexcept { return this->m_wheelDelta; }

		[[nodiscard]] inline bool IsLeftButtonDown()  const noexcept { return this->m_bIsLeftButtonDown;  }
		[[nodiscard]] inline bool IsRightButtonDown() const noexcept { return this->m_bIsRightButtonDown; }
	};

	struct Keyboard {
	friend WS::Window;

	private:
		bool m_downKeys[0xFE] = { false };
	
	public:
		[[nodiscard]] inline bool IsKeyDown(const uint8_t keyCode) const noexcept { return this->m_downKeys[keyCode];  }
		[[nodiscard]] inline bool IsKeyUp  (const uint8_t keyCode) const noexcept { return !this->m_downKeys[keyCode]; }
	};

}; // WS