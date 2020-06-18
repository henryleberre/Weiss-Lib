#pragma once

#include "WSVector.h"

namespace WS {

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