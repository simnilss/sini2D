// Basic math utilities for and helper functions exclusively for vectors
#pragma once
#ifndef SINI_VECTOR_UTIL_H
#define SINI_VECTOR_UTIL_H

#include "sini/math/Vector.hpp"
#include "sini/math/MathUtilitiesBase.hpp"

namespace sini {

using std::int32_t;
using std::uint32_t;

// Approximate equality, for floating point types
// -----------------------------------------------------------------------------
// float
SINI_CUDA_COMPAT bool approxEqual(vec2 a, vec2 b, float tol = DEFAULT_TOLERANCE) noexcept;
SINI_CUDA_COMPAT bool approxEqual(vec3 a, vec3 b, float tol = DEFAULT_TOLERANCE) noexcept;
SINI_CUDA_COMPAT bool approxEqual(vec4 a, vec4 b, float tol = DEFAULT_TOLERANCE) noexcept;
template<uint32_t N>
SINI_CUDA_COMPAT bool approxEqual(const Vector<float,N>& a,
                                  const Vector<float,N>& b,
                                  float tol = DEFAULT_TOLERANCE) noexcept;
// double
SINI_CUDA_COMPAT bool approxEqual(vec2d a, vec2d b, double tol = DEFAULT_TOLERANCE_DOUBLE) noexcept;
SINI_CUDA_COMPAT bool approxEqual(vec3d a, vec3d b, double tol = DEFAULT_TOLERANCE_DOUBLE) noexcept;
SINI_CUDA_COMPAT bool approxEqual(vec4d a, vec4d b, double tol = DEFAULT_TOLERANCE_DOUBLE) noexcept;
template<uint32_t N>
SINI_CUDA_COMPAT bool approxEqual(const Vector<double,N>& a,
                                  const Vector<double,N>& b,
                                  double tol = DEFAULT_TOLERANCE_DOUBLE) noexcept;

} // namespace sini

#include "sini/math/VectorUtilities.inl"
#endif // SINI_VECTOR_UTIL_H
