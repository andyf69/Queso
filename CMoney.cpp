#include "CMoney.h"
#include "Decimal.h"

#include <cassert>
//#include <iomanip>
//#include <sstream>
//
//CMoney::CMoney(const QString& in)
//{
//    int pos = in.indexOf('.');
//    if (pos == -1)
//        m_iMicros = in.toLongLong() * 1000000;
//    else
//    {
//        QString d = in.left(pos);
//        QString m = in.mid(pos + 1);
//        int ml = m.length();
//        if (ml < 6)
//            m += QString(6 - ml, '0');
//        else if (ml > 6)
//            m.truncate(6);
//        if (in.startsWith('-'))
//            m_iMicros = d.toLongLong() * 1000000 - m.toLongLong();
//        else
//            m_iMicros = d.toLongLong() * 1000000 + m.toLongLong();
//    }
//}
//
//QString CMoney::asString(int iDecimals) const
//{
//    return QString::number(m_iMicros / 1000000.0, 'f', iDecimals);
//    QString d(QString::number(static_cast<int>(m_iMicros / 1000000)));
//    if (iDecimals <= 0)
//        return d;
//
//    int im = 1000000 + static_cast<int>(std::abs(m_iMicros % 1000000));
//    int h = static_cast<int>(5 * std::pow(10, 5 - iDecimals));
//    QString m(QString::number(im+h) + "000000");
//    m = m.mid(1, iDecimals);
//    return QString("%1.%2").arg(d).arg(m);
//
//    //return QString("%1.%2").arg();
//    //ss << (m_iMicros / 1000000) << '.';
//    //ss << std::setfill('0') << std::setw(2);
//    //ss << (m_iCents % 100);
//
//    ////std::cout << m_iCents << ": " << ss << std::endl;
//    //return ss.str();
//}

void CMoney::unit_test()
{
    dec::decimal2 c1("1234.56");
    dec::decimal2 c2("123.9");
    dec::decimal2 c3("0.1");
    dec::decimal2 c4("1.30");
    dec::decimal2 c5("123");
    dec::decimal2 c6("333.");
    dec::decimal2 c7("101.101");
    dec::decimal2 c8("-1.499");
    dec::decimal2 c9("-1.500");
    dec::decimal2 c10("-1.501");
    dec::decimal2 c11("1.499");
    dec::decimal2 c12("1.500");
    dec::decimal2 c13("1.501");
    dec::decimal2 c14("-1.4949");
    dec::decimal2 c15("-1.495");
    dec::decimal2 c16("1.4949");
    dec::decimal2 c17("1.495");

    assert(toString(c1) == "1234.56");
    assert(toString(c2) == "123.90");
    assert(toString(c3) == "0.10");
    assert(toString(c4) == "1.30");
    assert(toString(c5) == "123.00");
    assert(toString(c6) == "333.00");
    assert(toString(c7) == "101.10");
    assert(toString(c8) == "-1.50");
    assert(toString(c9) == "-1.50");
    assert(toString(c10) == "-1.50");
    assert(toString(c11) == "1.50");
    assert(toString(c12) == "1.50");
    assert(toString(c13) == "1.50");
    assert(toString(c14) == "-1.49");
    assert(toString(c15) == "-1.50");
    assert(toString(c16) == "1.49");
    assert(toString(c17) == "1.50");
}
