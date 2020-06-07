#pragma once

#include "WSPch.h"

#ifdef __WEISS__DISABLE_SIMD

	#pragma message("[WEISS] Vector Math Will Be Slower Since __WEISS__DISABLE__SIMD is defined")

#endif

namespace WS {

	class _WS_TYPE_DOESNT_EXIST
	{
	};

#ifndef __WEISS__DISABLE_SIMD

	template <typename _T>
	using GET_SIMD_TYPE = typename std::conditional<
		std::is_same<_T, float>::value, __m128,
		typename std::conditional<
		std::is_same<_T, int32_t>::value, __m128i,
		typename std::conditional<
		std::is_same<_T, int16_t>::value, __m128i,
		typename std::conditional<
		std::is_same<_T, double>::value, __m256d, _WS_TYPE_DOESNT_EXIST>::type>::type>::type>::type;

#define WS_CAN_PERFORM_SIMD(_T) \
	(!std::is_same<GET_SIMD_TYPE<_T>, _WS_TYPE_DOESNT_EXIST>::value)

#define WS_CAN_SIMD_PERFORM_ARITHMETIC(_T, _T_2)            \
	(WS_CAN_PERFORM_SIMD(_T) && WS_CAN_PERFORM_SIMD(_T_2) && \
	std::is_same<_T, _T_2>::value)

#else

	template <typename _T>
	using GET_SIMD_TYPE = _WS_TYPE_DOESNT_EXIST;

#define WS_CAN_PERFORM_SIMD(_T) false
#define WS_CAN_SIMD_PERFORM_ARITHMETIC(_T, _T_2) false

#endif

#ifndef __WEISS__DISABLE_SIMD

	// ///////////////-\\\\\\\\\\\\\\\ \\
	// [/‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾\| \\
	// |-------SIMD Operations-------| \\
	// |\___________________________/| \\
	// \\\\\\\\\\\\\\\-/////////////// \\

	template <typename _T>
	static inline GET_SIMD_TYPE<_T> SIMDAdd(const GET_SIMD_TYPE<_T>& vecA, const GET_SIMD_TYPE<_T>& vecB) noexcept
	{
		if constexpr (std::is_same<_T, float>::value)
			return _mm_add_ps(vecA, vecB);
		else if constexpr (std::is_same<_T, int32_t>::value)
			return _mm_add_epi32(vecA, vecB);
		else if constexpr (std::is_same<_T, int16_t>::value)
			return _mm_add_epi16(vecA, vecB);
		else if constexpr (std::is_same<_T, double>::value)
			return _mm256_add_pd(vecA, vecB);
	}

	template <typename _T>
	static inline GET_SIMD_TYPE<_T> SIMDSub(const GET_SIMD_TYPE<_T>& vecA, const GET_SIMD_TYPE<_T>& vecB) noexcept
	{
		if constexpr (std::is_same<_T, float>::value)
			return _mm_sub_ps(vecA, vecB);
		else if constexpr (std::is_same<_T, int32_t>::value)
			return _mm_sub_epi32(vecA, vecB);
		else if constexpr (std::is_same<_T, int16_t>::value)
			return _mm_sub_epi16(vecA, vecB);
		else if constexpr (std::is_same<_T, double>::value)
			return _mm256_sub_pd(vecA, vecB);
	}

	template <typename _T>
	static inline GET_SIMD_TYPE<_T> SIMDMul(const GET_SIMD_TYPE<_T>& vecA, const GET_SIMD_TYPE<_T>& vecB) noexcept
	{
		if constexpr (std::is_same<_T, float>::value)
			return _mm_mul_ps(vecA, vecB);
		else if constexpr (std::is_same<_T, int32_t>::value)
			return _mm_mul_epi32(vecA, vecB);
		else if constexpr (std::is_same<_T, int16_t>::value)
			return _mm_mul_epi16(vecA, vecB);
		else if constexpr (std::is_same<_T, double>::value)
			return _mm256_mul_pd(vecA, vecB);
	}

	template <typename _T>
	static inline GET_SIMD_TYPE<_T> SIMDDiv(const GET_SIMD_TYPE<_T>& vecA, const GET_SIMD_TYPE<_T>& vecB) noexcept
	{
		if constexpr (std::is_same<_T, float>::value)
			return _mm_div_ps(vecA, vecB);
		else if constexpr (std::is_same<_T, int32_t>::value)
			return _mm_div_epi32(vecA, vecB);
		else if constexpr (std::is_same<_T, int16_t>::value)
			return _mm_div_epi16(vecA, vecB);
		else if constexpr (std::is_same<_T, double>::value)
			return _mm256_div_pd(vecA, vecB);
	}

	template <typename _T>
	static inline GET_SIMD_TYPE<_T> SIMDSet(const _T& x, const _T& y, const _T& z,
		const _T& w) noexcept
	{
		if constexpr (std::is_same<_T, float>::value)
			return _mm_set_ps(w, z, y, x);
		else if constexpr (std::is_same<_T, int32_t>::value)
			return _mm_set_epi32(w, z, y, x);
		else if constexpr (std::is_same<_T, int16_t>::value)
			return _mm_set_epi16(0, 0, 0, 0, w, z, y, x);
		else if constexpr (std::is_same<_T, double>::value)
			return _mm256_set_pd(w, z, y, x);
	}

	template <typename _T>
	static inline void SIMDStore(_T* src, const GET_SIMD_TYPE<_T>& sseVec) noexcept
	{
		if constexpr (std::is_same<_T, float>::value)
			_mm_store_ps(src, sseVec);
		else if constexpr (std::is_same<_T, int32_t>::value || std::is_same<_T, int16_t>::value)
			_mm_store_si128((__m128i*)src, sseVec);
		else if constexpr (std::is_same<_T, double>::value)
			_mm256_store_pd(src, sseVec);
	}

	template <typename _T>
	static inline GET_SIMD_TYPE<_T> SIMDLoad(_T* src) noexcept
	{
		if constexpr (std::is_same<_T, float>::value)
			return _mm_load_ps(src);
		else if constexpr (std::is_same<_T, int32_t>::value || std::is_same<_T, int16_t>::value)
			return _mm_load_si128((__m128i*)src);
		else if constexpr (std::is_same<_T, double>::value)
			return _mm256_load_pd(src);
	}

	template <typename _T, int INDEX>
	static inline _T SIMDExtractElement(const GET_SIMD_TYPE<_T>& sseVector) noexcept
	{
		if constexpr (std::is_same<_T, float>::value) {
			const int l = _mm_extract_ps(sseVector, INDEX);
			return *reinterpret_cast<const _T*>(&l);
		} else if constexpr (std::is_same<_T, int32_t>::value) {
			const int l = _mm_extract_epi16(sseVector, INDEX);
			return *reinterpret_cast<const _T*>(&l);
		} else if constexpr (std::is_same<_T, int16_t>::value) {
			const int l = _mm_extract_epi16(sseVector, INDEX);
			return *reinterpret_cast<const _T*>(&l);
		} else if constexpr (std::is_same<_T, double>::value) {
			if constexpr (INDEX <= 1) {
				return _mm256_cvtsd_f64(_mm256_shuffle_pd(sseVector, sseVector,
					_MM_SHUFFLE(INDEX, INDEX, INDEX, INDEX)));
			} else {
				return _mm_cvtsd_f64(_mm_shuffle_pd(_mm256_extractf128_pd(sseVector, 1), // I am
					_mm256_extractf128_pd(sseVector, 1), // proud of
					_MM_SHUFFLE(INDEX, INDEX, INDEX, INDEX))); // this magic
			}
		}
	}

	template <typename _T>
	static inline _T SIMDDotProduct(const GET_SIMD_TYPE<_T>& a, const GET_SIMD_TYPE<_T>& b) noexcept
	{
		if constexpr (std::is_same<_T, float>::value)
			return SIMDExtractElement<_T, 0>(_mm_dp_ps(a, b, 0xFF));
		else if constexpr (std::is_same<_T, int32_t>::value)
			return 0;
		else if constexpr (std::is_same<_T, int16_t>::value)
			return 0;
		else if constexpr (std::is_same<_T, double>::value)
			return SIMDExtractElement<_T, 0>(_mm256_dp_ps(a, b, 0xFF));
	}

#endif // #ifndef __WEISS__DISABLE_SIMD

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

	// ///////////////--\\\\\\\\\\\\\\\ \\
	// [/‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾\| \\
	// |----------Matrix<_T>----------| \\
	// |\____________________________/| \\
	// \\\\\\\\\\\\\\\--/////////////// \\

	/*
	 * Represents a row-major 4x4 matrix of "_T" values as shown below
	 * |-------------------|-------------------|-------------------|-------------------|
	 * | _T m00 (index 0)  | _T m01 (index 1)  | _T m02 (index 2)  | _T m03 (index 3)  |
	 * | _T m10 (index 4)  | _T m11 (index 5)  | _T m12 (index 6)  | _T m13 (index 7)  |
	 * | _T m20 (index 8)  | _T m21 (index 9)  | _T m22 (index 10) | _T m23 (index 11) |
	 * | _T m30 (index 12) | _T m31 (index 13) | _T m32 (index 14) | _T m33 (index 15) |
	 * |-------------------|-------------------|-------------------|-------------------|
	 */

	template <typename _T>
	class Matrix
	{
	private:
		_T m[16] = { 0 };

	public:
		Matrix() = default;

		inline Matrix(const _T* buff)                 noexcept { std::memcpy(this->m, buff, sizeof(_T) * 16u); }

		inline Matrix(const std::array<_T, 16u>& arr) noexcept { std::memcpy(this->m, arr.data(), sizeof(_T) * 16u); }

		inline Matrix(const Matrix<_T>& other) noexcept { std::memcpy(this->m, other.m, sizeof(_T) * 16u); }

		inline _T& operator[](const size_t i) noexcept { return this->m[i]; }

		inline const _T& operator[](const size_t i) const noexcept { return this->m[i]; }

		inline _T& operator()(const size_t r, const size_t c) noexcept { return this->m[r * 4u + c]; }

		inline const _T& operator()(const size_t r, const size_t c) const noexcept { return this->m[r * 4u + c]; }

		// --- Static Functions --- //



		static inline Matrix<_T> MakeZeros()    noexcept { return Matrix<_T>(); }

		static inline Matrix<_T> MakeIdentity() noexcept {
			return Matrix<_T>(std::array<_T, 16u>{
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1
			});
		}

		static inline Matrix<_T> MakeRotationX(const float radX = 0) noexcept
		{
			const _T sinX = std::sin(radX);
			const _T cosX = std::cos(radX);

			return Matrix<_T>(std::array<_T, 16u>{
				1, 0,    0,     0,
				0, cosX, -sinX, 0,
				0, sinX, cosX,  0,
				0, 0,    0,     1
			});
		}

		static inline Matrix<_T> MakeRotationY(const float radY = 0) noexcept
		{
			const _T sinY = std::sin(radY);
			const _T cosY = std::cos(radY);

			return Matrix<_T>(std::array<_T, 16u>{
				cosY,  0, sinY, 0,
				0,     1, 0,    0,
				-sinY, 0, cosY, 0,
				0,     0, 0,    1
			});
		}

		static inline Matrix<_T> MakeRotationZ(const float radZ = 0) noexcept
		{
			const _T sinZ = std::sin(radZ);
			const _T cosZ = std::cos(radZ);

			return Matrix<_T>(std::array<_T, 16u>{
				cosZ, -sinZ, 0, 0,
				sinZ, cosZ,  0, 0,
				0,    0,     1, 0,
				0,    0,     0, 1
			});
		}

		static inline Matrix<_T> MakeRotation(const float radX = 0, const float radY = 0, const float radZ = 0) noexcept
		{
			return MakeRotationX(radX) * MakeRotationY(radY) * MakeRotationZ(radZ);
		}

		static inline Matrix<_T> MakeRotation(const Vector<_T>& radians) noexcept
		{
			return Matrix<_T>::MakeRotation(radians.x, radians.y, radians.z);
		}

		static inline Matrix<_T> MakeTranslation(const float x, const float y, const float z) noexcept
		{
			return Matrix<_T>(std::array<_T, 16u>{
				1,  0,  0,  0,
				0,  1,  0,  0,
				0,  0,  1,  0,
				-x, -y, -z, 1,
			});
		}

		static inline Matrix<_T> MakeTranslation(const Vector<_T>& translation) noexcept
		{
			return Matrix<_T>::MakeTranslation(translation.x, translation.y, translation.z);
		}

		static inline Matrix<_T> MakePerspective(const float zNear, const float zFar, const float fovRad, const float aspectRatio)
		{
			return Matrix<_T>(std::array<_T, 16u>{
				aspectRatio * fovRad, 0,      0,                                0,
				0,                    fovRad, 0,                                0,
				0,                    0,      zFar / (zFar - zNear),            1,
				0,                    0,      (-zFar * zNear) / (zFar - zNear), 1,
			});
		}

		static inline Matrix<_T> MakeScaling(const float scaleX = 1.0f, const float scaleY = 1.0f, const float scaleZ = 1.0f, const float scaleW = 1.0f) noexcept
		{
			return Matrix<_T>(std::array<_T, 16u>{
				scaleX, 0, 0, 0,
				0, scaleY, 0, 0,
				0, 0, scaleZ, 0,
				0, 0, 0, scaleW
			});
		}

		static inline Matrix<_T> MakeScaling(const Vecf32& scale) noexcept
		{
			return Matrix<_T>::MakeScaling(scale.x, scale.y, scale.z, scale.w);
		}

		static inline Matrix<_T> MakeTransposed(const Matrix<_T>& mat) noexcept
		{
			return Matrix<_T>(std::array<_T, 16u>{
				mat(0, 0), mat(1, 0), mat(2, 0), mat(3, 0),
				mat(0, 1), mat(1, 1), mat(2, 1), mat(3, 1),
				mat(0, 2), mat(1, 2), mat(2, 2), mat(3, 2),
				mat(0, 3), mat(1, 3), mat(2, 3), mat(3, 3)
			});
		}

		// Thanks to: https://stackoverflow.com/questions/349050/calculating-a-lookat-matrix
		static inline Matrix<_T> MakeLookAt(const Vector<_T>& cameraPosition, const Vector<_T>& focalPoint, const Vector<_T>& upDirection) noexcept
		{
			const Vector<_T> zaxis = Vector<_T>::Normalized(focalPoint - cameraPosition);
			const Vector<_T> xaxis = Vector<_T>::Normalized(Vector<_T>::CrossProduct3D(upDirection, zaxis));
			const Vector<_T> yaxis = Vector<_T>::CrossProduct3D(zaxis, xaxis);

			const _T m30 = -Vector<_T>::DotProduct(xaxis, cameraPosition);
			const _T m31 = -Vector<_T>::DotProduct(yaxis, cameraPosition);
			const _T m32 = -Vector<_T>::DotProduct(zaxis, cameraPosition);

			return Matrix<_T>(std::array<_T, 16u>{
				xaxis.x, yaxis.x, zaxis.x, 0,
				xaxis.y, yaxis.y, zaxis.y, 0,
				xaxis.z, yaxis.z, zaxis.z, 0,
				m30,     m31,     m32,     1
			});
		}
	};

	template <typename _T, typename _T_2>
	inline auto operator*(const Matrix<_T>& matA, const Matrix<_T_2>& matB) noexcept
		-> Matrix<decltype(matA[0] + matB[0])>
	{
		Matrix<decltype(matA[0] + matB[0])> matResult;

		for (size_t r = 0u; r < 4u; r++) {
			for (size_t c = 0u; c < 4u; c++) {
				const Vector<_T>   vecA = Vector<_T>(matA(r, 0), matA(r, 1), matA(r, 2), matA(r, 3));
				const Vector<_T_2> vecB = Vector<_T_2>(matB(0, c), matB(1, c), matB(2, c), matB(3, c));

				matResult(r, c) = Vector<>::DotProduct(vecA, vecB);
			}
		}

		return matResult;
	}

	template <typename _T>
	inline std::ostream& operator<<(std::ostream& stream, const Matrix<_T>& mat) noexcept
	{
		constexpr const size_t MAX_DIGITS = 5u;

		stream << std::setprecision(MAX_DIGITS) << '|';

		for (size_t i = 0u; i < 4u * ((MAX_DIGITS + 1u) + 1u) + 1u; i++)
			stream << '-';

		stream << "|\n";
		for (size_t r = 0u; r < 4u; r++) {
			stream << "| ";

			for (size_t c = 0u; c < 4u; c++)
				stream << std::setw(MAX_DIGITS + 1u) << mat(r, c) << ' ';

			stream << "|\n";
		}

		stream << '|';

		for (size_t i = 0u; i < 4u * ((MAX_DIGITS + 1u) + 1u) + 1u; i++)
			stream << '-';

		stream << "|\n";

		return stream;
	}

	/*
	 * Multiplies a 1x4 vector with a 4x4 matrix like shown below :
								    |----------------------------------------|
								    | _T_2 m00; _T_2 m01; _T_2 m02; _T_2 m03 |
								    | _T_2 m10; _T_2 m11; _T_2 m12; _T_2 m13 |
	   |------------------------|   | _T_2 m20; _T_2 m21; _T_2 m22; _T_2 m23 |   |----------------------------|
	   | _T x; _T y; _T z; _T w | * | _T_2 m30; _T_2 m31; _T_2 m32; _T_2 m33 | = | decltype(x+m00) x, y, z, w |
	   |------------------------|   |----------------------------------------|   |----------------------------|
	 */

	template <typename _T, typename _T_2>
	inline auto operator*(const Vector<_T>& vec, const Matrix<_T_2>& mat) noexcept
		-> Vector<decltype(vec.x + mat[0])>
	{
		return Vector<decltype(vec.x + mat[0])>(
			Vector<>::DotProduct(vec, Vector<_T_2>(mat(0, 0), mat(1, 0), mat(2, 0), mat(3, 0))),
			Vector<>::DotProduct(vec, Vector<_T_2>(mat(0, 1), mat(1, 1), mat(2, 1), mat(3, 1))),
			Vector<>::DotProduct(vec, Vector<_T_2>(mat(0, 2), mat(1, 2), mat(2, 2), mat(3, 2))),
			Vector<>::DotProduct(vec, Vector<_T_2>(mat(0, 3), mat(1, 3), mat(2, 3), mat(3, 3)))
		);
	}

	typedef Matrix<std::enable_if_t<std::numeric_limits<float>::is_iec559, float>> Matf32;

}; // WS