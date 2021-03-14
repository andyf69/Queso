#include "CppUnitTest.h"
#include "../Decimal.h"
#include <sstream>
#include <string>
#include <vector>
#include <QtCore/QString>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#define MSG(msg) [&]{ std::stringstream _s; _s << msg; return _s.str(); }()

namespace UnitTests
{
	TEST_CLASS(DecimalTests)
	{
	public:
		
		TEST_METHOD(Test_Construct_Decimal2_From_String_And_Output_As_String)
		{
			// Arrange
            struct StringToString { std::string oIn, oExpected, oActual; };
            std::vector<StringToString> oStringToStrings =
            {
                {"1234.56", "1234.56"},
                {"123.9", "123.90"},
                {"0.1", "0.10"},
                {"1.30", "1.30"},
                {"123", "123.00"},
                {"333.", "333.00"},
                {"101.101", "101.10"},
                {"-1.499", "-1.50"},
                {"-1.500", "-1.50"},
                {"-1.501", "-1.50"},
                {"1.499", "1.50"},
                {"1.500", "1.50"},
                {"1.501", "1.50"},
                {"-1.4949", "-1.49"},
                {"-1.495", "-1.50"},
                {"1.4949", "1.49"},
                {"1.495", "1.50"}
            };

            // Act
            for (auto& s : oStringToStrings)
            {
                dec::decimal2 d(s.oIn);
                s.oActual = toString(d);
            }

            // Assert
            for (auto& s : oStringToStrings)
                Assert::IsTrue(s.oExpected == s.oActual, QString("Input: %1 Expected: %2 Actual: %3").arg(s.oIn.c_str()).arg(s.oExpected.c_str()).arg(s.oActual.c_str()).toStdWString().c_str());
		}
	};
}
