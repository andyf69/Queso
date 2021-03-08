#include "Decimal.h"

using namespace dec;

static bool isMultOverflow(const int64_t value1, const int64_t value2)
{
	if (value1 == 0 || value2 == 0)
		return false;

	if ((value1 < 0) != (value2 < 0))
	{ // different sign
		if (value1 == std::numeric_limits<int64_t>::min())
			return value2 > 1;
		if (value2 == std::numeric_limits<int64_t>::min())
			return value1 > 1;
		if (value1 < 0)
			return isMultOverflow(-value1, value2);
		if (value2 < 0)
			return isMultOverflow(value1, -value2);
	}
	else if (value1 < 0 && value2 < 0)
	{
		if (value1 == std::numeric_limits<int64_t>::min())
			return value2 < -1;
		if (value2 == std::numeric_limits<int64_t>::min())
			return value1 < -1;
		return isMultOverflow(-value1, -value2);
	}

	return (value1 > std::numeric_limits<int64_t>::max() / value2);
}

// calculate greatest common divisor
static int64_t gcd(int64_t a, int64_t b)
{
	int64_t c;
	while (a != 0)
	{
		c = a;
		a = b % a;
		b = c;
	}
	return b;
}

// calculate output = round(a / b), where output, a, b are int64_t
bool dec::div_rounded(int64_t& output, int64_t a, int64_t b)
{
	int64_t divisorCorr = std::abs(b) / 2;
	if (a >= 0)
	{
		if (std::numeric_limits<int64_t>::max() - a >= divisorCorr)
		{
			output = (a + divisorCorr) / b;
			return true;
		}
		const int64_t i = a / b;
		const int64_t r = a - i * b;
		if (r < divisorCorr)
		{
			output = i;
			return true;
		}
	}
	else
	{
		if (-(std::numeric_limits<int64_t>::min() - a) >= divisorCorr)
		{
			output = (a - divisorCorr) / b;
			return true;
		}

		const int64_t i = a / b;
		const int64_t r = a - i * b;
		if (r < divisorCorr)
		{
			output = i;
			return true;
		}
	}

	output = 0;
	return false;
}

// result = (value1 * value2) / divisor
inline int64_t dec_utils::multDiv(const int64_t value1, const int64_t value2, int64_t divisor)
{
	// we don't check for division by zero, the caller should - the next line will throw.
	const int64_t value1int = value1 / divisor;
	int64_t value1dec = value1 % divisor;
	const int64_t value2int = value2 / divisor;
	int64_t value2dec = value2 % divisor;

	int64_t result = value1 * value2int + value1int * value2dec;

	if (value1dec == 0 || value2dec == 0)
		return result;

	if (!isMultOverflow(value1dec, value2dec))
	{ // no overflow
		int64_t resDecPart = value1dec * value2dec;
		if (!dec::div_rounded(resDecPart, resDecPart, divisor))
			resDecPart = 0;
		result += resDecPart;
		return result;
	}

	// minimize value1 & divisor
	{
		int64_t c = gcd(value1dec, divisor);
		if (c != 1) {
			value1dec /= c;
			divisor /= c;
		}

		// minimize value2 & divisor
		c = gcd(value2dec, divisor);
		if (c != 1) {
			value2dec /= c;
			divisor /= c;
		}
	}

	if (!isMultOverflow(value1dec, value2dec))
	{ // no overflow
		int64_t resDecPart = value1dec * value2dec;
		if (dec::div_rounded(resDecPart, resDecPart, divisor))
		{
			result += resDecPart;
			return result;
		}
	}

	// overflow can occur - use less precise version
	result += dec::round(static_cast<long double>(value1dec) * static_cast<long double>(value2dec) / static_cast<long double>(divisor));
	return result;
}

