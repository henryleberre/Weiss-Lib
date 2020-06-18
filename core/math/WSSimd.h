#pragma once

#include "../misc/WSPch.h"

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

}; // WS