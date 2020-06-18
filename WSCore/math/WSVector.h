#pragma once

#include "WSSimd.h"

namespace WS {

	// ////////////////////////-\\\\\\\\\\\\\\\\\\\\\\\\ \\
	// [/‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾\| \\
	// |----------RawVectorComponents<_T, _D>----------| \\
	// |\_____________________________________________/| \\
	// \\\\\\\\\\\\\\\\\\\\\\\\-//////////////////////// \\

	// Should be used when you don't wish to perform any math operation to the data (i.e just storing it)
	// If you wish to perform math operations, convert it to a Vector<_T> since it supports SIMD and then convert it back

	// _T : the type of every vector component
	// _D : the number of dimensions (i.e components) (max 4)
	template <typename _T, size_t _D>
	struct RawVectorComponents {  };

	// Template specialisation
	template <typename _T>
	struct RawVectorComponents<_T, 1u> { union { _T x; _T r; }; };

	template <typename _T>
	struct RawVectorComponents<_T, 2u> : RawVectorComponents<_T, 1u> { union { _T y; _T g; }; };

	template <typename _T>
	struct RawVectorComponents<_T, 3u> : RawVectorComponents<_T, 2u> { union { _T z; _T b; }; };

	template <typename _T>
	struct RawVectorComponents<_T, 4u> : RawVectorComponents<_T, 3u> { union { _T w; _T a; }; };

	template <typename _T, size_t _D>
	inline std::ostream& operator<<(std::ostream& stream, const RawVectorComponents<_T, _D>& raw) noexcept
	{
		static_assert(_D <= 4);

		stream << '(';

		if constexpr (_D == 1u)
			stream << raw.x;
		else if constexpr (_D == 2u)
			stream << raw.x << ", " << raw.y;
		else if constexpr (_D == 3u)
			stream << raw.x << ", " << raw.y << ", " << raw.z;
		else
			stream << raw.x << ", " << raw.y << ", " << raw.z << ", " << raw.w;

		stream << ')';

		return stream;
	}

	// ///////////////--\\\\\\\\\\\\\\\ \\
	// [/‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾\| \\
	// |----------Vector<_T>----------| \\
	// |\____________________________/| \\
	// \\\\\\\\\\\\\\\--/////////////// \\

	// Should be used for quick SIMD Enabled vector math
	// Can be converted into RawVectorComponents<_T, _D>
	// _T : the type of every vector component
	template <typename _T = int>
	struct Vector
	{
		union {
			GET_SIMD_TYPE<_T> m_sseVector; // Is a "_WS_TYPE_DOESNT_EXIST" if no intrinsic type can be created
			RawVectorComponents<_T, 4u> m_rawComponents; // Use to convert this 4 dimensional SIMD Enable Vector into any lower dimension vector.
			union {
				struct { _T x, y, z, w; };
				struct { _T r, g, b, a; };
			};
			_T m_arr[4u];
		};

		inline Vector(const _T& x = 0, const _T& y = 0, const _T& z = 0, const _T& w = 0) noexcept
		{
			this->Set(x, y, z, w);
		}

		template <typename _T_2 = _T>
		inline Vector(const Vector<_T_2>& other) noexcept
		{
			if constexpr (WS_CAN_PERFORM_SIMD(_T) && std::is_same<_T, _T_2>::value) {
#ifndef __WEISS__DISABLE_SIMD
				this->m_sseVector = other.m_sseVector;
#endif // __WEISS__DISABLE_SIMD
			} else if constexpr (std::is_same<_T, _T_2>::value) {
				std::memcpy(&this->m_arr, &other.m_arr, sizeof(_T) * 4u);
			} else {
				this->Set(other.x, other.y, other.z, other.w);
			}
		}

#ifndef __WEISS__DISABLE_SIMD

		template <typename _T_2 = _T>
		inline Vector(const GET_SIMD_TYPE<_T_2>& other) noexcept
		{
			static_assert(WS_CAN_PERFORM_SIMD(_T));

			if constexpr (std::is_same<_T, _T_2>::value) {
				this->m_sseVector = other;
			} else {
				this->m_sseVector = SIMDSet(static_cast<_T>(SIMDExtractElement<_T_2, 0u>(this->m_sseVector)),
					static_cast<_T>(SIMDExtractElement<_T_2, 1u>(this->m_sseVector)),
					static_cast<_T>(SIMDExtractElement<_T_2, 2u>(this->m_sseVector)),
					static_cast<_T>(SIMDExtractElement<_T_2, 3u>(this->m_sseVector)));
			}
		}

#endif

		template <typename _T_2, size_t _D_2>
		inline Vector(const RawVectorComponents<_T_2, _D_2>& raw) noexcept
		{
			static_assert(_D_2 <= 4);

			if constexpr (_D_2 == 1)
				this->Set(raw.x);
			else if constexpr (_D_2 == 2)
				this->Set(raw.x, raw.y);
			else if constexpr (_D_2 == 3)
				this->Set(raw.x, raw.y, raw.z);
			else
				this->Set(raw.x, raw.y, raw.z, raw.w);
		}

		inline void Set(const _T& x, const _T& y = 0, const _T& z = 0, const _T& w = 0) noexcept
		{
			if constexpr (WS_CAN_PERFORM_SIMD(_T)) {
#ifndef __WEISS__DISABLE_SIMD
				this->m_sseVector = SIMDSet<_T>(x, y, z, w);
#endif // #ifndef __WEISS__DISABLE_SIMD
			} else {
				this->x = x;
				this->y = y;
				this->z = z;
				this->w = w;
			}
		}

		[[nodiscard]] inline float GetLength() const noexcept
		{
			if constexpr (WS_CAN_PERFORM_SIMD(_T)) {
#ifndef __WEISS__DISABLE_SIMD
				return std::sqrt(SIMDDotProduct<_T>(this->m_sseVector, this->m_sseVector));
#endif // #ifndef __WEISS__DISABLE_SIMD
			} else {
				return std::sqrt(this->x * this->x + this->y * this->y + this->z * this->z + this->w * this->w);
			}
		}


		inline void Normalize() noexcept
		{
			this->operator/=(this->GetLength());
		}

		template <typename _T_2>
		inline void operator+=(const Vector<_T_2>& other) noexcept
		{
			if constexpr (WS_CAN_SIMD_PERFORM_ARITHMETIC(_T, _T_2)) {
#ifndef __WEISS__DISABLE_SIMD
				this->m_sseVector = SIMDAdd<_T>(this->m_sseVector, other.m_sseVector);
#endif // #ifndef __WEISS__DISABLE_SIMD
			} else {
				this->x += other.x;
				this->y += other.y;
				this->z += other.z;
				this->w += other.w;
			}
		}

		template <typename _T_2>
		inline void operator-=(const Vector<_T_2>& other) noexcept
		{
			if constexpr (WS_CAN_SIMD_PERFORM_ARITHMETIC(_T, _T_2)) {
#ifndef __WEISS__DISABLE_SIMD
				this->m_sseVector = SIMDSub<_T>(
					this->m_sseVector, other.m_sseVector);
#endif // #ifndef __WEISS__DISABLE_SIMD
			} else {
				this->x -= other.x;
				this->y -= other.y;
				this->z -= other.z;
				this->w -= other.w;
			}
		}

		template <typename _T_2>
		inline void operator*=(const Vector<_T_2>& other) noexcept
		{
			if constexpr (WS_CAN_SIMD_PERFORM_ARITHMETIC(_T, _T_2)) {
#ifndef __WEISS__DISABLE_SIMD
				this->m_sseVector = SIMDMul<_T>(this->m_sseVector, other.m_sseVector);
#endif // #ifndef __WEISS__DISABLE_SIMD
			} else {
				this->x *= other.x;
				this->y *= other.y;
				this->z *= other.z;
				this->w *= other.w;
			}
		}

		template <typename _T_2>
		inline void operator/=(const Vector<_T_2>& other) noexcept
		{
			if constexpr (WS_CAN_SIMD_PERFORM_ARITHMETIC(_T, _T_2)) {
#ifndef __WEISS__DISABLE_SIMD
				this->m_sseVector = SIMDDiv<_T>(this->m_sseVector, other.m_sseVector);
#endif // #ifndef __WEISS__DISABLE_SIMD
			} else {
				this->x /= other.x;
				this->y /= other.y;
				this->z /= other.z;
				this->w /= other.w;
			}
		}

		template <typename _T_2>
		inline bool operator==(const Vector<_T_2>& other) const noexcept
		{
			if constexpr (std::is_same<_T, _T_2>::value) {
				return std::memcmp(this->m_arr, other.m_arr, 4u * sizeof(_T)) == 0;
			} else {
				return this->x == other.x && this->y == other.y &&
					this->z == other.z && this->w == other.w;
			}
		}

		template <typename _T_2>
		inline bool operator!=(const Vector<_T_2>& other) const noexcept
		{
			if constexpr (std::is_same<_T, _T_2>::value) {
				return std::memcmp(this->m_arr, other.m_arr, 4u * sizeof(_T)) != 0;
			} else {
				return this->x != other.x || this->y != other.y ||
					this->z != other.z || this->w != other.w;
			}
		}

		template <typename _T_2, typename = std::enable_if_t<std::is_arithmetic_v<_T_2>, void>>
		inline void operator+=(const _T_2& n) noexcept
		{
			this->operator+=(Vector<_T_2>(n, n, n, n));
		}

		template <typename _T_2>
		inline void operator-=(const _T_2& n) noexcept
		{
			this->operator-=(Vector<_T_2>(n, n, n, n));
		}

		template <typename _T_2>
		inline void operator*=(const _T_2& n) noexcept
		{
			this->operator*=(Vector<_T_2>(n, n, n, n));
		}

		template <typename _T_2>
		inline void operator/=(const _T_2& n) noexcept
		{
			this->operator/=(Vector<_T_2>(n, n, n, n));
		}

		template <size_t _D>
		inline operator RawVectorComponents<_T, _D>() const noexcept { return this->m_rawComponents; }

		template <typename _T_2>
		[[nodiscard]] inline static Vector<_T_2> Normalized(const Vector<_T_2>& vec) noexcept
		{
			return vec / vec.GetLength();
		}

		template <typename _T_2, typename _T_3>
		[[nodiscard]] inline static auto CrossProduct3D(const Vector<_T_2>& vecA, const Vector<_T_3>& vecB) noexcept
			-> Vector<decltype(vecA.x + vecB.x)>
		{
			if constexpr (WS_CAN_SIMD_PERFORM_ARITHMETIC(_T_2, _T_3)) {
#ifndef __WEISS__DISABLE_SIMD
				const Vector<_T_2> tempA1(vecA.y, vecA.z, vecA.x, 0);
				const Vector<_T_2> tempB1(vecB.z, vecB.x, vecB.y, 0);
				const Vector<_T_2> tempA2(vecA.z, vecA.x, vecA.y, 0);
				const Vector<_T_2> tempB2(vecB.y, vecB.z, vecB.x, 0);

				return tempA1 * tempB1 - tempA2 * tempB2;
#endif // #ifndef __WEISS__DISABLE_SIMD
			} else {
				return Vector<decltype(vecA.x + vecB.x)>(
					vecA.y * vecB.z - vecA.z * vecB.y,
					vecA.z * vecB.x - vecA.x * vecB.z,
					vecA.x * vecB.y - vecA.y * vecB.x,
					0
				);
			}
		}

		template <typename _T_2, typename _T_3>
		[[nodiscard]] static inline auto DotProduct(const Vector<_T_2>& a, const Vector<_T_3>& b) noexcept
			-> decltype(a.x + b.x)
		{
			if constexpr (WS_CAN_SIMD_PERFORM_ARITHMETIC(_T_2, _T_3)) {
#ifndef __WEISS__DISABLE_SIMD
				return SIMDDotProduct<_T_2>(a.m_sseVector, b.m_sseVector);
#endif // #ifndef __WEISS__DISABLE_SIMD
			} else {
				return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
			}
		}

		template <typename _T_2, typename _T_3>
		[[nodiscard]] static inline auto GetReflected(const Vector<_T_2>& in, const Vector<_T_3>& normal) noexcept
			-> Vector<decltype(in.x + normal.x)>
		{
			return in - normal * 2 * Vector<>::DotProduct(in, normal);
		}
	};

	template <typename _T, typename _T_2>
	[[nodiscard]] inline auto operator+(const Vector<_T>& a,
		const Vector<_T_2>& b) noexcept
		-> Vector<decltype(a.x + b.x)>
	{
		if constexpr (WS_CAN_SIMD_PERFORM_ARITHMETIC(_T, _T_2)) {
#ifndef __WEISS__DISABLE_SIMD
			return Vector<_T>(SIMDAdd<_T>(a.m_sseVector, b.m_sseVector));
#endif // #ifndef __WEISS__DISABLE_SIMD
		} else {
			return Vector<decltype(a.x + b.x)>(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
		}
	}

	template <typename _T, typename _T_2>
	[[nodiscard]] inline auto operator-(const Vector<_T>& a, const Vector<_T_2>& b) noexcept
		-> Vector<decltype(a.x - b.x)>
	{
		if constexpr (WS_CAN_SIMD_PERFORM_ARITHMETIC(_T, _T_2)) {
#ifndef __WEISS__DISABLE_SIMD
			return Vector<_T>(SIMDSub<_T>(a.m_sseVector, b.m_sseVector));
#endif // #ifndef __WEISS__DISABLE_SIMD
		} else {
			return Vector<decltype(a.x - b.x)>(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
		}
	}

	template <typename _T, typename _T_2>
	[[nodiscard]] inline auto operator*(const Vector<_T>& a, const Vector<_T_2>& b) noexcept
		-> Vector<decltype(a.x* b.x)>
	{
		if constexpr (WS_CAN_SIMD_PERFORM_ARITHMETIC(_T, _T_2)) {
#ifndef __WEISS__DISABLE_SIMD
			return Vector<_T>(SIMDMul<_T>(a.m_sseVector, b.m_sseVector));
#endif // #ifndef __WEISS__DISABLE_SIMD
		} else {
			return Vector<decltype(a.x* b.x)>(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w);
		}
	}

	template <typename _T, typename _T_2>
	[[nodiscard]] inline auto operator/(const Vector<_T>& a, const Vector<_T_2>& b) noexcept
		-> Vector<decltype(a.x / b.x)>
	{
		if constexpr (WS_CAN_SIMD_PERFORM_ARITHMETIC(_T, _T_2)) {
#ifndef __WEISS__DISABLE_SIMD
			return Vector<_T>(SIMDDiv<_T>(a.m_sseVector, b.m_sseVector));
#endif // #ifndef __WEISS__DISABLE_SIMD
		} else {
			return Vector<decltype(a.x / b.x)>(a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w);
		}
	}

	template <typename _T, typename _T_2>
	inline auto operator+(const Vector<_T>& vec, const _T_2& n) noexcept
		-> Vector<decltype(vec.x + n)>
	{
		return vec + Vector<_T_2>(n, n, n, n);
	}

	template <typename _T, typename _T_2>
	inline auto operator-(const Vector<_T>& vec, const _T_2& n) noexcept
		-> Vector<decltype(vec.x - n)>
	{
		return vec - Vector<_T_2>(n, n, n, n);
	}

	template <typename _T, typename _T_2>
	inline auto operator*(const Vector<_T>& vec, const _T_2& n) noexcept
		-> Vector<decltype(vec.x* n)>
	{
		return vec * Vector<_T_2>(n, n, n, n);
	}

	template <typename _T, typename _T_2>
	inline auto operator/(const Vector<_T>& vec, const _T_2& n) noexcept
		-> Vector<decltype(vec.x / n)>
	{
		return vec / Vector<_T_2>(n, n, n, n);
	}

	template <typename _T>
	inline std::ostream& operator<<(std::ostream& stream, const Vector<_T>& vec) noexcept
	{
		stream << "(" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << ")";

		return stream;
	}

	typedef Vector<int8_t>  Veci8;
	typedef Vector<int16_t> Veci16;
	typedef Vector<int32_t> Veci32;

	typedef Vector<uint8_t>  Vecu8;
	typedef Vector<uint16_t> Vecu16;
	typedef Vector<uint32_t> Vecu32;

	typedef Vector<std::enable_if_t<std::numeric_limits<float>::is_iec559, float>>   Vecf32;
	typedef Vector<std::enable_if_t<std::numeric_limits<double>::is_iec559, double>> Vecd64;

	typedef Vecf32 Colorf32;
	typedef Vecu8  Coloru8;

}; // WS