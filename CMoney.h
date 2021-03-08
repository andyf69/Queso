#pragma once

#include <QtCore/QString>

class CMoney
{
public:
    //CMoney() = delete;
    //CMoney(const CMoney&) = default;
    //explicit CMoney(const QString& oDollars);
    //explicit CMoney(int64_t iMicros) { m_iMicros = iMicros; }

    //const CMoney operator+(const CMoney& that) const { return CMoney(m_iMicros + that.m_iMicros); }
    //const CMoney operator-(const CMoney& that) const { return CMoney(m_iMicros - that.m_iMicros); }
    //CMoney& operator+=(const CMoney& that) { m_iMicros += that.m_iMicros; return *this; }
    //CMoney& operator-=(const CMoney& that) { m_iMicros -= that.m_iMicros; return *this; }

    //QString asString(int iDecimalPlaces = 2) const;

    static void unit_test();

private:
    //int64_t m_iMicros; // 1,000,000 micros == 1 Dollar
};

