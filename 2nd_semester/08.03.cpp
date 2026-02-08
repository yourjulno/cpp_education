// /chapter_number_processing/floating_point_numbers.cpp

/////////////////////////////////////////////////////////////////////////////////////////
//
// chapter : Number Processing
// section : Floating-Point Numbers
//
/////////////////////////////////////////////////////////////////////////////////////////

#include <cassert>
#include <cmath>
#include <cstdint>
#include <format>
#include <limits>
#include <string>

/////////////////////////////////////////////////////////////////////////////////////////

static_assert(sizeof(int) == 4, "This task assumes 4-byte int.");
static_assert(sizeof(unsigned int) == 4, "This task assumes 4-byte unsigned int.");
static_assert(sizeof(float) == 4, "This task assumes 4-byte float.");

namespace ilog2_detail
{
	[[nodiscard]] constexpr int ilog2_u32(unsigned int u) noexcept
	{
		int r = -1;
		while (u != 0u)
		{
			u >>= 1u;
			++r;
		}
		return r;
	}

	[[nodiscard]] constexpr int ilog2_int(int x) noexcept
	{
		assert(x > 0);
		return ilog2_u32(static_cast<unsigned int>(x));
	}

	[[nodiscard]] inline int ilog2_float(float x) noexcept
	{
		assert(x > 0.0f);
		assert(std::isfinite(x));

		union FloatBits
		{
			float f;
			unsigned int u;
		};

		FloatBits fb{.f = x};

		const unsigned int bits = fb.u;
		const unsigned int exp = (bits >> 23u) & 0xFFu;
		const unsigned int frac = bits & 0x7FFFFFu;

		if (exp == 0u)
		{
			assert(frac != 0u);
			return ilog2_u32(frac) - 149;
		}

		if (exp == 0xFFu)
		{
			assert(false && "ilog2_float: inf/nan not supported for integer log2.");
			return 0;
		}

		return static_cast<int>(exp) - 127;
	}
} // namespace ilog2_detail

/////////////////////////////////////////////////////////////////////////////////////////

int main()
{
	for (auto i = 1uz; i <= std::numeric_limits<double>::digits10; ++i)
	{
		assert(std::format("{:.{}f}", 1.0, i) == "1." + std::string(i, '0'));
	}

	for (auto i = 0uz; i <= std::numeric_limits<double>::max_exponent10; ++i)
	{
		auto s = (i < 10 ? "0" : "") + std::to_string(i);
		assert(std::format("{:.{}e}", std::pow(10.0, i), 0) == "1e+" + s);
	}

	if (std::numeric_limits<double>::has_infinity)
	{
		assert(std::format("{}", std::numeric_limits<double>::infinity()) == "inf");
	}

	if (std::numeric_limits<double>::has_quiet_NaN)
	{
		assert(std::format("{}", std::numeric_limits<double>::quiet_NaN()) == "nan");
	}

	using ilog2_detail::ilog2_int;
	assert(ilog2_int(1) == 0);
	assert(ilog2_int(2) == 1);
	assert(ilog2_int(3) == 1);
	assert(ilog2_int(4) == 2);
	assert(ilog2_int(7) == 2);
	assert(ilog2_int(8) == 3);
	assert(ilog2_int(0x7FFFFFFF) == 30);

	using ilog2_detail::ilog2_float;

	assert(ilog2_float(1.0f) == 0);
	assert(ilog2_float(2.0f) == 1);
	assert(ilog2_float(3.0f) == 1);
	assert(ilog2_float(4.0f) == 2);
	assert(ilog2_float(0.5f) == -1);
	assert(ilog2_float(0.25f) == -2);

	const float den_min = std::numeric_limits<float>::denorm_min();
	assert(den_min > 0.0f);
	assert(ilog2_float(den_min) == -149);

	union FB { float f; unsigned int u; };
	FB den_max{};
	den_max.u = 0x007FFFFFu; // exp=0
	assert(den_max.f > 0.0f);
	assert(ilog2_float(den_max.f) == -127);

	const float norm_min = std::numeric_limits<float>::min();
	assert(norm_min > 0.0f);
	assert(ilog2_float(norm_min) == -126);

	const float fmax = std::numeric_limits<float>::max();
	assert(std::isfinite(fmax));
	assert(ilog2_float(fmax) == 127);

	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////
