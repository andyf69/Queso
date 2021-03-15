#include "CppUnitTest.h"
#include "../Decimal.h"
#include <sstream>
#include <string>
#include <vector>
#include <QtCore/QString>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

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
                s.oActual = d.asString();
            }

            // Assert
            for (auto& s : oStringToStrings)
                Assert::IsTrue(s.oExpected == s.oActual, QString("Input: %1 Expected: %2 Actual: %3").arg(s.oIn.c_str()).arg(s.oExpected.c_str()).arg(s.oActual.c_str()).toStdWString().c_str());
        }

        TEST_METHOD(Test_Construct_Decimal2_From_Double_And_Output_As_String)
        {
            // Arrange
            struct DoubleToString { double dIn; std::string oExpected, oActual; };
            std::vector<DoubleToString> oDoubleToStrings =
            {
                {1234.56, "1234.56"},
                {123.9, "123.90"},
                {0.1, "0.10"},
                {1.30, "1.30"},
                {123.0, "123.00"},
                {101.101, "101.10"},
                {-1.499, "-1.50"},
                {-1.500, "-1.50"},
                {-1.501, "-1.50"},
                {1.499, "1.50"},
                {1.500, "1.50"},
                {1.501, "1.50"},
                {-1.4949, "-1.49"},
                {-1.495, "-1.50"},
                {1.4949, "1.49"},
                {1.495, "1.50"}
            };

            // Act
            for (auto& s : oDoubleToStrings)
            {
                dec::decimal2 d(s.dIn);
                s.oActual = d.asString();
            }

            // Assert
            for (auto& s : oDoubleToStrings)
                Assert::IsTrue(s.oExpected == s.oActual, QString("Input: %1 Expected: %2 Actual: %3").arg(s.dIn).arg(s.oExpected.c_str()).arg(s.oActual.c_str()).toStdWString().c_str());
        }

        TEST_METHOD(Test_Construct_Decimal2_From_String_And_Output_As_Double)
        {
            // Arrange
            struct StringToDouble { std::string oIn; double dExpected, dActual; };
            std::vector<StringToDouble> oStringToDoubles =
            {
                {"1234.56", 1234.56},
                {"123.9", 123.9},
                {"0.1", 0.1},
                {"1.30", 1.3},
                {"123", 123.0},
                {"101.101", 101.1},
                {"-1.499", -1.5},
                {"-1.500", -1.5},
                {"-1.501", -1.5},
                {"1.499", 1.5},
                {"1.500", 1.5},
                {"1.501", 1.5},
                {"-1.4949", -1.49},
                {"-1.495", -1.5},
                {"1.4949", 1.49},
                {"1.495", 1.5}
            };

            // Act
            for (auto& s : oStringToDoubles)
            {
                dec::decimal2 d(s.oIn);
                s.dActual = d.asDouble();
            }

            // Assert
            for (auto& s : oStringToDoubles)
                Assert::IsTrue(s.dExpected == s.dActual, QString("Input: %1 Expected: %2 Actual: %3").arg(s.oIn.c_str()).arg(s.dExpected).arg(s.dActual).toStdWString().c_str());
        }

        TEST_METHOD(Test_Decimal2_Plus_Operator)
        {
            // Arrange
            struct Data { double dA, dB, dExpected, dActual; };
            std::vector<Data> oData =
            {
                {2.0, 2.0, 4.0},
                {2.0, 2.11, 4.11},
                {0.2, -0.1, 0.1},
                {-0.1, 0.3, 0.2},
                {0.50, 0.555, 1.06},
                {1.234, 3.456, 4.69},
                {1.4949, 1.5149, 3.0},
                {-2.22, -3.33, -5.55}
            };

            // Act
            for (auto& s : oData)
            {
                dec::decimal2 a(s.dA);
                dec::decimal2 b(s.dB);
                dec::decimal2 c = a + b;
                s.dActual = c.asDouble();
            }

            // Assert
            for (auto& s : oData)
                Assert::IsTrue(s.dExpected == s.dActual, QString("Input: %1, %2 Expected: %3 Actual: %4").arg(s.dA).arg(s.dB).arg(s.dExpected).arg(s.dActual).toStdWString().c_str());
        }

        TEST_METHOD(Test_Decimal2_Minus_Operator)
        {
            // Arrange
            struct Data { double dA, dB, dExpected, dActual; };
            std::vector<Data> oData =
            {
                {2.0, 2.0, 0.0},
                {2.0, 2.11, -0.11},
                {0.2, -0.1, 0.3},
                {-0.1, 0.3, -0.4},
                {0.50, 0.555, -0.06},
                {1.234, 3.456, -2.23},
                {1.4949, 1.5149, -0.02},
                {-2.22, -3.33, 1.11}
            };

            // Act
            for (auto& s : oData)
            {
                dec::decimal2 a(s.dA);
                dec::decimal2 b(s.dB);
                dec::decimal2 c = a - b;
                s.dActual = c.asDouble();
            }

            // Assert
            for (auto& s : oData)
                Assert::IsTrue(s.dExpected == s.dActual, QString("Input: %1, %2 Expected: %3 Actual: %4").arg(s.dA).arg(s.dB).arg(s.dExpected).arg(s.dActual).toStdWString().c_str());
        }

        TEST_METHOD(Test_Decimal2_Multiply_Operator)
        {
            // Arrange
            struct Data { double dA, dB, dExpected, dActual; };
            std::vector<Data> oData =
            {
                {2.0, 2.0, 4.0},
                {2.0, 2.11, 4.22},
                {0.2, -0.1, -0.02},
                {-0.1, 0.3, -0.03},
                {0.50, 0.555, 0.28},
                {1.234, 3.456, 4.26},
                {1.4949, 1.5149, 2.25},
                {-2.22, -3.33, 7.39}
            };

            // Act
            for (auto& s : oData)
            {
                dec::decimal2 a(s.dA);
                dec::decimal2 b(s.dB);
                dec::decimal2 c = a * b;
                s.dActual = c.asDouble();
            }

            // Assert
            for (auto& s : oData)
                Assert::IsTrue(s.dExpected == s.dActual, QString("Input: %1, %2 Expected: %3 Actual: %4").arg(s.dA).arg(s.dB).arg(s.dExpected).arg(s.dActual).toStdWString().c_str());
        }

        TEST_METHOD(Test_Decimal2_Divide_Operator)
        {
            // Arrange
            struct Data { double dA, dB, dExpected, dActual; };
            std::vector<Data> oData =
            {
                {2.0, 2.0, 1.0},
                {2.0, 2.11, 0.95},
                {0.2, -0.1, -2.0},
                {-0.1, 0.3, -0.33},
                {0.50, 0.555, 0.89},
                {1.234, 1.0, 1.23},
                {1.49, 100000.0, 0.0},
                {2.0, 0.01, 200.0}
            };

            // Act
            for (auto& s : oData)
            {
                dec::decimal2 a(s.dA);
                dec::decimal2 b(s.dB);
                dec::decimal2 c = a / b;
                s.dActual = c.asDouble();
            }

            // Assert
            for (auto& s : oData)
                Assert::IsTrue(s.dExpected == s.dActual, QString("Input: %1, %2 Expected: %3 Actual: %4").arg(s.dA).arg(s.dB).arg(s.dExpected).arg(s.dActual).toStdWString().c_str());
        }

        TEST_METHOD(Test_Decimal2_Plus_Equals_Operator)
        {
            // Arrange
            struct Data { double dA, dB, dExpected, dActual; };
            std::vector<Data> oData =
            {
                {2.0, 2.0, 4.0},
                {2.0, 2.11, 4.11},
                {0.2, -0.1, 0.1},
                {-0.1, 0.3, 0.2},
                {0.50, 0.555, 1.06},
                {1.234, 3.456, 4.69},
                {1.4949, 1.5149, 3.0},
                {-2.22, -3.33, -5.55}
            };

            // Act
            for (auto& s : oData)
            {
                dec::decimal2 a(s.dA);
                dec::decimal2 b(s.dB);
                a += b;
                s.dActual = a.asDouble();
            }

            // Assert
            for (auto& s : oData)
                Assert::IsTrue(s.dExpected == s.dActual, QString("Input: %1, %2 Expected: %3 Actual: %4").arg(s.dA).arg(s.dB).arg(s.dExpected).arg(s.dActual).toStdWString().c_str());
        }

        TEST_METHOD(Test_Decimal2_Minus_Equals_Operator)
        {
            // Arrange
            struct Data { double dA, dB, dExpected, dActual; };
            std::vector<Data> oData =
            {
                {2.0, 2.0, 0.0},
                {2.0, 2.11, -0.11},
                {0.2, -0.1, 0.3},
                {-0.1, 0.3, -0.4},
                {0.50, 0.555, -0.06},
                {1.234, 3.456, -2.23},
                {1.4949, 1.5149, -0.02},
                {-2.22, -3.33, 1.11}
            };

            // Act
            for (auto& s : oData)
            {
                dec::decimal2 a(s.dA);
                dec::decimal2 b(s.dB);
                a -= b;
                s.dActual = a.asDouble();
            }

            // Assert
            for (auto& s : oData)
                Assert::IsTrue(s.dExpected == s.dActual, QString("Input: %1, %2 Expected: %3 Actual: %4").arg(s.dA).arg(s.dB).arg(s.dExpected).arg(s.dActual).toStdWString().c_str());
        }

        TEST_METHOD(Test_Decimal2_Multiply_Equals_Operator)
        {
            // Arrange
            struct Data { double dA, dB, dExpected, dActual; };
            std::vector<Data> oData =
            {
                {2.0, 2.0, 4.0},
                {2.0, 2.11, 4.22},
                {0.2, -0.1, -0.02},
                {-0.1, 0.3, -0.03},
                {0.50, 0.555, 0.28},
                {1.234, 3.456, 4.26},
                {1.4949, 1.5149, 2.25},
                {-2.22, -3.33, 7.39}
            };

            // Act
            for (auto& s : oData)
            {
                dec::decimal2 a(s.dA);
                dec::decimal2 b(s.dB);
                a *= b;
                s.dActual = a.asDouble();
            }

            // Assert
            for (auto& s : oData)
                Assert::IsTrue(s.dExpected == s.dActual, QString("Input: %1, %2 Expected: %3 Actual: %4").arg(s.dA).arg(s.dB).arg(s.dExpected).arg(s.dActual).toStdWString().c_str());
        }

        TEST_METHOD(Test_Decimal2_Divide_Equals_Operator)
        {
            // Arrange
            struct Data { double dA, dB, dExpected, dActual; };
            std::vector<Data> oData =
            {
                {2.0, 2.0, 1.0},
                {2.0, 2.11, 0.95},
                {0.2, -0.1, -2.0},
                {-0.1, 0.3, -0.33},
                {0.50, 0.555, 0.89},
                {1.234, 1.0, 1.23},
                {1.49, 100000.0, 0.0},
                {2.0, 0.01, 200.0}
            };

            // Act
            for (auto& s : oData)
            {
                dec::decimal2 a(s.dA);
                dec::decimal2 b(s.dB);
                a /= b;
                s.dActual = a.asDouble();
            }

            // Assert
            for (auto& s : oData)
                Assert::IsTrue(s.dExpected == s.dActual, QString("Input: %1, %2 Expected: %3 Actual: %4").arg(s.dA).arg(s.dB).arg(s.dExpected).arg(s.dActual).toStdWString().c_str());
        }
    };
}
