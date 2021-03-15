#pragma once
// Decimal value type. Use for capital calculations.
// Note: Maximum handled value is +9,223,372,036,854,775,807 (divided by prec)
#include <cstdint>
#include <iomanip>
#include <iosfwd>
#include <limits>
#include <sstream>

namespace dec
{
	enum { max_decimal_points = 18 };

	template<int Prec> struct DecimalFactor { static const int64_t value = 10 * DecimalFactor<Prec - 1>::value; };
	template<> struct DecimalFactor<0> { static const int64_t value = 1; };
	template<> struct DecimalFactor<1> { static const int64_t value = 10; };
	template<int Prec, bool positive> struct DecimalFactorDiff_impl { static const int64_t value = DecimalFactor<Prec>::value; };
	template<int Prec> struct DecimalFactorDiff_impl<Prec, false> {	static const int64_t value = std::numeric_limits<int64_t>::min(); };
	template<int Prec> struct DecimalFactorDiff { static const int64_t value = DecimalFactorDiff_impl<Prec, Prec >= 0>::value; };

	// round floating point value and convert to int64_t
	template<class T>
	inline int64_t round(T value)
	{
		T val1;

		if (value < 0.0)
			val1 = value - 0.5;
		else
			val1 = value + 0.5;

		return static_cast<int64_t>(val1);
	}

	// calculate output = round(a / b), where output, a, b are int64_t
	bool div_rounded(int64_t& output, int64_t a, int64_t b);

	class dec_utils
	{
	public:
		// result = (value1 * value2) / divisor
		static int64_t multDiv(const int64_t value1, const int64_t value2, int64_t divisor);
	};

	template<int Prec>
	class decimal
	{
	public:
		enum { decimal_points = Prec };

		decimal() noexcept : m_value(0) {}
		decimal(const decimal& src) = default;
		explicit decimal(unsigned int value) { m_value = DecimalFactor<Prec>::value * value; }
		explicit decimal(int value) { m_value = DecimalFactor<Prec>::value * value; }
		explicit decimal(int64_t value) { m_value = DecimalFactor<Prec>::value * value; }
		explicit decimal(long double value) { m_value = fpToStorage(value); }
		explicit decimal(double value) { m_value = fpToStorage(value); }
		explicit decimal(float value) { m_value = fpToStorage(static_cast<double>(value)); }

		explicit decimal(int64_t value, int64_t precFactor)
		{
			int64_t ownFactor = DecimalFactor<Prec>::value;

			if (ownFactor == precFactor) // no conversion required
				m_value = value;
			else if (ownFactor > precFactor)
				m_value = value * (ownFactor / precFactor);
			else // conversion
				dec::div_rounded(m_value, value, precFactor / ownFactor);
		}

		explicit decimal(const std::string& value) { fromString(value, *this); }

		~decimal() = default;

		static int64_t getPrecFactor() { return DecimalFactor<Prec>::value; }
		static int getDecimalPoints() { return Prec; }

		decimal& operator=(const decimal& rhs) = default;

		template<int Prec2>
		decimal& operator=(const decimal<Prec2>& rhs)
		{
			if (Prec2 > Prec)
				dec::div_rounded(m_value, rhs.getUnbiased(), DecimalFactorDiff<Prec2 - Prec>::value);
			else
				m_value = rhs.getUnbiased() * DecimalFactorDiff<Prec - Prec2>::value;
			return *this;
		}

		decimal& operator=(int64_t rhs)
		{
			m_value = DecimalFactor<Prec>::value * rhs;
			return *this;
		}

		decimal& operator=(int rhs)
		{
			m_value = DecimalFactor<Prec>::value * rhs;
			return *this;
		}

		decimal& operator=(double rhs)
		{
			m_value = fpToStorage(rhs);
			return *this;
		}

		decimal& operator=(long double rhs)
		{
			m_value = fpToStorage(rhs);
			return *this;
		}

		template <typename T> bool operator==(const T& rhs) const { return (*this == static_cast<decimal>(rhs)); }
		template <typename T> bool operator<(const T& rhs) const { return (*this < static_cast<decimal>(rhs)); }
		template <typename T> bool operator<=(const T& rhs) const { return (*this <= static_cast<decimal>(rhs)); }
		template <typename T> bool operator>(const T& rhs) const { return (*this > static_cast<decimal>(rhs)); }
		template <typename T> bool operator>=(const T& rhs) const { return (*this >= static_cast<decimal>(rhs)); }
		template <typename T> bool operator!=(const T& rhs) const { return !(*this == rhs); }

		bool operator==(const decimal& rhs) const { return (m_value == rhs.m_value); }
		bool operator<(const decimal& rhs) const { return (m_value < rhs.m_value); }
		bool operator<=(const decimal& rhs) const { return (m_value <= rhs.m_value); }
		bool operator>(const decimal& rhs) const { return (m_value > rhs.m_value); }
		bool operator>=(const decimal& rhs) const { return (m_value >= rhs.m_value); }
		bool operator!=(const decimal& rhs) const { return !(*this == rhs); }

		template <typename T> const decimal operator+(const T& rhs) const { return *this + static_cast<decimal>(rhs); }
		template <typename T> const decimal operator-(const T& rhs) const { return *this - static_cast<decimal>(rhs); }
		template <typename T> const decimal operator*(const T& rhs) const { return *this * static_cast<decimal>(rhs); }
		template <typename T> const decimal operator/(const T& rhs) const { return *this / static_cast<decimal>(rhs); }

		const decimal operator+(const decimal& rhs) const
		{
			decimal result = *this;
			result.m_value += rhs.m_value;
			return result;
		}
		const decimal operator-(const decimal& rhs) const
		{
			decimal result = *this;
			result.m_value -= rhs.m_value;
			return result;
		}
		const decimal operator*(const decimal& rhs) const
		{
			decimal result = *this;
			result.m_value = dec_utils::multDiv(result.m_value, rhs.m_value, DecimalFactor<Prec>::value);
			return result;
		}
		const decimal operator/(const decimal& rhs) const
		{
			decimal result = *this;
			result.m_value = dec_utils::multDiv(result.m_value, DecimalFactor<Prec>::value, rhs.m_value);
			return result;
		}

		template<int Prec2> const decimal operator+(const decimal<Prec2>& rhs) const
		{
			decimal result = *this;
			if (Prec2 > Prec)
			{
				int64_t val;
				dec::div_rounded(val, rhs.getUnbiased(), DecimalFactorDiff<Prec2 - Prec>::value);
				result.m_value += val;
			}
			else
				result.m_value += rhs.getUnbiased() * DecimalFactorDiff<Prec - Prec2>::value;

			return result;
		}
		template<int Prec2> const decimal operator-(const decimal<Prec2>& rhs) const
		{
			decimal result = *this;
			if (Prec2 > Prec)
			{
				int64_t val;
				dec::div_rounded(val, rhs.getUnbiased(), DecimalFactorDiff<Prec2 - Prec>::value);
				result.m_value -= val;
			}
			else
				result.m_value -= rhs.getUnbiased() * DecimalFactorDiff<Prec - Prec2>::value;

			return result;
		}
		template<int Prec2> const decimal operator*(const decimal<Prec2>& rhs) const
		{
			decimal result = *this;
			result.m_value = dec_utils::multDiv(result.m_value, rhs.getUnbiased(), DecimalFactor<Prec2>::value);
			return result;
		}
		template<int Prec2> const decimal operator/(const decimal<Prec2>& rhs) const
		{
			decimal result = *this;
			result.m_value = dec_utils::multDiv(result.m_value, DecimalFactor<Prec2>::value, rhs.getUnbiased());
			return result;
		}

		template <typename T> decimal& operator+=(const T& rhs)
		{
			*this += static_cast<decimal>(rhs);
			return *this;
		}
		template <typename T> decimal& operator-=(const T& rhs)
		{
			*this -= static_cast<decimal>(rhs);
			return *this;
		}
		template <typename T> decimal& operator*=(const T& rhs)
		{
			*this *= static_cast<decimal>(rhs);
			return *this;
		}
		template <typename T> decimal& operator/=(const T& rhs)
		{
			*this /= static_cast<decimal>(rhs);
			return *this;
		}

		decimal& operator+=(const decimal& rhs)
		{
			m_value += rhs.m_value;
			return *this;
		}
		decimal& operator-=(const decimal& rhs)
		{
			m_value -= rhs.m_value;
			return *this;
		}
		decimal& operator*=(const decimal& rhs)
		{
			m_value = dec_utils::multDiv(m_value, rhs.m_value, DecimalFactor<Prec>::value);
			return *this;
		}
		decimal& operator/=(const decimal& rhs)
		{
			m_value = dec_utils::multDiv(m_value, DecimalFactor<Prec>::value, rhs.m_value);
			return *this;
		}

		template<int Prec2> decimal& operator+=(const decimal<Prec2>& rhs)
		{
			if (Prec2 > Prec)
			{
				int64_t val;
				dec::div_rounded(val, rhs.getUnbiased(), DecimalFactorDiff<Prec2 - Prec>::value);
				m_value += val;
			}
			else
				m_value += rhs.getUnbiased() * DecimalFactorDiff<Prec - Prec2>::value;

			return *this;
		}
		template<int Prec2> decimal& operator-=(const decimal<Prec2>& rhs)
		{
			if (Prec2 > Prec)
			{
				int64_t val;
				dec::div_rounded(val, rhs.getUnbiased(), DecimalFactorDiff<Prec2 - Prec>::value);
				m_value -= val;
			}
			else
				m_value -= rhs.getUnbiased() * DecimalFactorDiff<Prec - Prec2>::value;

			return *this;
		}
		template<int Prec2> decimal& operator*=(const decimal<Prec2>& rhs)
		{
			m_value = dec_utils::multDiv(m_value, rhs.getUnbiased(), DecimalFactor<Prec2>::value);
			return *this;
		}
		template<int Prec2> decimal& operator/=(const decimal<Prec2>& rhs)
		{
			m_value = dec_utils::multDiv(m_value, DecimalFactor<Prec2>::value, rhs.getUnbiased());
			return *this;
		}

		const decimal operator+() const { return *this; }
		const decimal operator-() const
		{
			decimal result = *this;
			result.m_value = -result.m_value;
			return result;
		}

		int sign() const { return (m_value > 0) ? 1 : ((m_value < 0) ? -1 : 0); }

		double asDouble() const { return static_cast<double>(m_value) / getPrecFactorDouble(); }
		void setAsDouble(double value) { m_value = fpToStorage(value); }

		std::string asString() const
		{
			int64_t before, after;
			unpack(before, after);
			int sign = 1;

			if (before < 0)
			{
				sign = -1;
				before = -before;
			}

			if (after < 0)
			{
				sign = -1;
				after = -after;
			}

			std::stringstream output;
			if (sign < 0)
				output << "-";

			output << before;

			if (Prec > 0)
				output << '.' << std::setw(Prec) << std::setfill('0') << std::right << after;
			return output.str();
		}

		// returns integer value = real_value * (10 ^ precision)
		// use to load/store decimal value in external memory
		int64_t getUnbiased() const { return m_value; }
		void setUnbiased(int64_t value) { m_value = value; }

		decimal<Prec> abs() const
		{
			if (m_value >= 0)
				return *this;
			return (decimal<Prec>(0) - *this);
		}

		// Returns two parts: before and after decimal point
		// For negative values both numbers are negative or zero.
		void unpack(int64_t& beforeValue, int64_t& afterValue) const
		{
			afterValue = m_value % DecimalFactor<Prec>::value;
			beforeValue = (m_value - afterValue) / DecimalFactor<Prec>::value;
		}

		// Combines two parts (before and after decimal point) into decimal value.
		// Both input values have to have the same sign for correct results.
		// Does not perform any rounding or input validation - afterValue must be less than 10^prec.
		// beforeValue value before decimal point
		// afterValue value after decimal point multiplied by 10^prec
		// Returns *this
		decimal& pack(int64_t beforeValue, int64_t afterValue)
		{
			if (Prec > 0)
			{
				m_value = beforeValue * DecimalFactor<Prec>::value;
				m_value += (afterValue % DecimalFactor<Prec>::value);
			}
			else
				m_value = beforeValue * DecimalFactor<Prec>::value;
			return *this;
		}

		// Version of pack() with rounding, sourcePrec specifies precision of source values.
		template<int sourcePrec>
		decimal& pack_rounded(int64_t beforeValue, int64_t afterValue)
		{
			decimal<sourcePrec> temp;
			temp.pack(beforeValue, afterValue);
			decimal<Prec> result(temp.getUnbiased(), temp.getPrecFactor());

			*this = result;
			return *this;
		}

	protected:
		inline long double getPrecFactorXDouble() const { return static_cast<long double>(DecimalFactor<Prec>::value); }
		inline double getPrecFactorDouble() const { return static_cast<double>(DecimalFactor<Prec>::value); }

		template<typename T>
		static int64_t fpToStorage(T value)
		{
			int64_t intPart = static_cast<int64_t>(value);
			T fracPart = value - intPart;
			return dec::round(static_cast<T>(DecimalFactor<Prec>::value) * fracPart) + DecimalFactor<Prec>::value * intPart;
		}

		template<typename T>
		static T abs(T value)
		{
			if (value < 0)
				return -value;
			return value;
		}
	protected:
		int64_t m_value;
	};

	// Pre-defined types
	typedef decimal<2> decimal2;
	typedef decimal<4> decimal4;
	typedef decimal<6> decimal6;

	// global functions
	template<int Prec, class T>
	decimal<Prec> decimal_cast(const T& arg) { return decimal<Prec>(arg.getUnbiased(), arg.getPrecFactor()); }

	// Example of use:
	//   c = dec::decimal_cast<6>(a * b);
	template<int Prec> decimal<Prec> decimal_cast(unsigned int arg) { return decimal<Prec>(arg); }
	template<int Prec> decimal<Prec> decimal_cast(int arg) { return decimal<Prec>(arg); }
	template<int Prec> decimal<Prec> decimal_cast(int64_t arg) { return decimal<Prec>(arg); }
	template<int Prec> decimal<Prec> decimal_cast(double arg) { return decimal<Prec>(arg); }
	template<int Prec> decimal<Prec> decimal_cast(const std::string& arg) { return decimal<Prec>(arg); }
	template<int Prec, int N> decimal<Prec> decimal_cast(const char(&arg)[N]) { return decimal<Prec>(arg); }

	/// Exports decimal to stream
	/// Used format: {-}bbbb.aaaa where
	/// {-} is optional '-' sign character
	/// '.' is decimal point character
	/// bbbb is stream of digits before decimal point
	/// aaaa is stream of digits after decimal point
	template<class decimal_type, typename StreamType>
	void toStream(const decimal_type& arg, StreamType& output)
	{
		int64_t before, after;
		arg.unpack(before, after);
		int sign = 1;

		if (before < 0)
		{
			sign = -1;
			before = -before;
		}

		if (after < 0)
		{
			sign = -1;
			after = -after;
		}

		if (sign < 0)
			output << "-";

		output << before;

		if (arg.getDecimalPoints() > 0)
			output << '.' << std::setw(arg.getDecimalPoints()) << std::setfill('0') << std::right << after;
	}

	namespace details
	{
		template<typename StreamType>
		bool parse_unpacked(StreamType& input, int& sign, int64_t& before, int64_t& after, int& decimalDigits)
		{
			enum StateEnum
			{
				IN_SIGN, IN_BEFORE_FIRST_DIG, IN_BEFORE_DEC, IN_AFTER_DEC, IN_END
			} state = IN_SIGN;
			enum ErrorCodes
			{
				ERR_WRONG_CHAR = -1,
				ERR_NO_DIGITS = -2,
				ERR_WRONG_STATE = -3,
				ERR_STREAM_GET_ERROR = -4
			};

			before = after = 0;
			sign = 1;

			int error = 0;
			int digitsCount = 0;
			int afterDigitCount = 0;
			char c;

			while ((input) && (state != IN_END)) // loop while extraction from file is possible
			{
				c = static_cast<char>(input.get());

				switch (state)
				{
				case IN_SIGN:
					if (c == '-')
					{
						sign = -1;
						state = IN_BEFORE_FIRST_DIG;
					}
					else if (c == '+')
						state = IN_BEFORE_FIRST_DIG;
					else if ((c >= '0') && (c <= '9'))
					{
						state = IN_BEFORE_DEC;
						before = static_cast<int>(c - '0');
						digitsCount++;
					}
					else if (c == '.')
						state = IN_AFTER_DEC;
					else if ((c != ' ') && (c != '\t'))
					{
						state = IN_END;
						error = ERR_WRONG_CHAR;
					}
					// else ignore char
					break;
				case IN_BEFORE_FIRST_DIG:
					if ((c >= '0') && (c <= '9'))
					{
						before = 10 * before + static_cast<int>(c - '0');
						state = IN_BEFORE_DEC;
						digitsCount++;
					}
					else if (c == '.')
						state = IN_AFTER_DEC;
					else
					{
						state = IN_END;
						error = ERR_WRONG_CHAR;
					}
					break;
				case IN_BEFORE_DEC:
					if ((c >= '0') && (c <= '9'))
					{
						before = 10 * before + static_cast<int>(c - '0');
						digitsCount++;
					}
					else if (c == '.')
						state = IN_AFTER_DEC;
					else 
						state = IN_END;
					break;
				case IN_AFTER_DEC:
					if ((c >= '0') && (c <= '9'))
					{
						after = 10 * after + static_cast<int>(c - '0');
						afterDigitCount++;
						if (afterDigitCount >= dec::max_decimal_points)
							state = IN_END;
					}
					else
					{
						state = IN_END;
						if (digitsCount == 0)
							error = ERR_NO_DIGITS;
					}
					break;
				default:
					error = ERR_WRONG_STATE;
					state = IN_END;
					break;
				} // switch state
			} // while stream good & not end

			decimalDigits = afterDigitCount;

			if (error >= 0)
			{
				if (sign < 0)
				{
					before = -before;
					after = -after;
				}
			}
			else
				before = after = 0;

			return (error >= 0);
		} // function
	}

	// Converts stream of chars to decimal
	// Spaces and tabs on the front are ignored.
	// Performs rounding when provided value has higher precision than in output type.
	// input input stream
	// output decimal value, 0 on error
	// Returns true if conversion succeeded
	template<typename decimal_type, typename StreamType>
	bool fromStream(StreamType& input, decimal_type& output) 
	{
		int sign, afterDigits;
		int64_t before, after;
		bool result = details::parse_unpacked(input, sign, before, after, afterDigits);
		if (result)
		{
			if (afterDigits <= decimal_type::decimal_points)
			{
				// direct mode
				int corrCnt = decimal_type::decimal_points - afterDigits;
				while (corrCnt > 0)
				{
					after *= 10;
					--corrCnt;
				}
				output.pack(before, after);
			}
			else
			{
				// rounding mode
				int corrCnt = afterDigits;
				int64_t decimalFactor = 1;
				while (corrCnt > 0)
				{
					before *= 10;
					decimalFactor *= 10;
					--corrCnt;
				}
				decimal_type temp(before + after, decimalFactor);
				output = temp;
			}
		}
		else
			output = decimal_type(0);
		return result;
	}

	template<int prec>
	std::string& toString(const decimal<prec>& arg, std::string& output)
	{
		std::ostringstream out;
		toStream(arg, out);
		output = out.str();
		return output;
	}

	/// Exports decimal to string
	/// Used format: {-}bbbb.aaaa where
	/// {-} is optional '-' sign character
	/// '.' is locale-dependent decimal point character
	/// bbbb is stream of digits before decimal point
	/// aaaa is stream of digits after decimal point
	template<int prec> std::string toString(const decimal<prec>& arg)
	{
		std::string res;
		toString(arg, res);
		return res;
	}

	// input
	template<class charT, class traits, int prec>
	std::basic_istream<charT, traits>& operator>>(std::basic_istream<charT, traits>& is, decimal<prec>& d)
	{
		if (!fromStream(is, d))
			d.setUnbiased(0);
		return is;
	}

	// output
	template<class charT, class traits, int prec>
	std::basic_ostream<charT, traits>& operator<<(std::basic_ostream<charT, traits>& os, const decimal<prec>& d)
	{
		toStream(d, os);
		return os;
	}

	/// Imports decimal from string
	/// Used format: {-}bbbb.aaaa where
	/// {-} is optional '-' sign character
	/// '.' is locale-dependent decimal point character
	/// bbbb is stream of digits before decimal point
	/// aaaa is stream of digits after decimal point
	template<typename T> T fromString(const std::string& str)
	{
		std::istringstream is(str);
		T t;

		if (!fromStream(is, t))
			t.setUnbiased(0);
		return t;
	}

	template<typename T> void fromString(const std::string& str, T& out) { out = fromString<T>(str); }
}
