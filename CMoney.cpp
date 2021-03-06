#include "CMoney.h"

#include <cassert>
#include <iomanip>
#include <sstream>

CMoney::CMoney(const QString& in)
{
    int pos = in.indexOf('.');
    if (pos == -1)
        m_iMicros = in.toLongLong() * 1000000;
    else
    {
        QString d = in.left(pos);
        QString m = in.mid(pos + 1);
        int ml = m.length();
        if (ml < 6)
            m += QString(6 - ml, '0');
        else if (ml > 6)
            m.truncate(6);
        if (in.startsWith('-'))
            m_iMicros = d.toLongLong() * 1000000 - m.toLongLong();
        else
            m_iMicros = d.toLongLong() * 1000000 + m.toLongLong();
    }
}

QString CMoney::asString(int iDecimals) const
{
    return QString::number(m_iMicros / 1000000.0, 'f', iDecimals);
    QString d(QString::number(static_cast<int>(m_iMicros / 1000000)));
    if (iDecimals <= 0)
        return d;

    int im = 1000000 + static_cast<int>(std::abs(m_iMicros % 1000000));
    int h = static_cast<int>(5 * std::pow(10, 5 - iDecimals));
    QString m(QString::number(im+h) + "000000");
    m = m.mid(1, iDecimals);
    return QString("%1.%2").arg(d).arg(m);

    //return QString("%1.%2").arg();
    //ss << (m_iMicros / 1000000) << '.';
    //ss << std::setfill('0') << std::setw(2);
    //ss << (m_iCents % 100);

    ////std::cout << m_iCents << ": " << ss << std::endl;
    //return ss.str();
}

void CMoney::unit_test()
{
    CMoney c1("1234.56");
    CMoney c2("123.9");
    CMoney c3("0.1");
    CMoney c4("1.30");
    CMoney c5("123");
    CMoney c6("333.");
    CMoney c7("101.101");
    CMoney c8("-1.499");
    CMoney c9("-1.500");
    CMoney c10("-1.501");
    CMoney c11("1.499");
    CMoney c12("1.500");
    CMoney c13("1.501");
    CMoney c14("-1.4949");
    CMoney c15("-1.495");
    CMoney c16("1.4949");
    CMoney c17("1.495");

    assert(c1.asString() == "1234.56");
    assert(c2.asString() == "123.90");
    assert(c3.asString() == "0.10");
    assert(c4.asString() == "1.30");
    assert(c5.asString() == "123.00");
    assert(c6.asString() == "333.00");
    assert(c7.asString() == "101.10");
    assert(c8.asString() == "-1.50");
    assert(c9.asString() == "-1.50");
    assert(c10.asString() == "-1.50");
    assert(c11.asString() == "1.50");
    assert(c12.asString() == "1.50");
    assert(c13.asString() == "1.50");
    assert(c14.asString() == "-1.49");
    assert(c15.asString() == "-1.50");
    assert(c16.asString() == "1.49");
    assert(c17.asString() == "1.50");
}
