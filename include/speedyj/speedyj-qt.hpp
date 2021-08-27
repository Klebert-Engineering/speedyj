#pragma once

#include "speedyj.hpp"

class QString;
class QVariant;
class QJsonValue;
class QJsonArray;
class QJsonObject;

#include <QVector>
#include <QMap>
#include <QList>

namespace speedyj
{

SPEEDYJ_EXPORT Stream& operator<<(Stream&, const QVariant&);
SPEEDYJ_EXPORT Stream& operator<<(Stream&, const QString&);
SPEEDYJ_EXPORT Stream& operator<<(Stream&, const QStringList&);
SPEEDYJ_EXPORT Stream& operator<<(Stream&, const QJsonValue&);
SPEEDYJ_EXPORT Stream& operator<<(Stream&, const QJsonArray&);
SPEEDYJ_EXPORT Stream& operator<<(Stream&, const QJsonObject&);

template <class _T>
Stream& operator<<(Stream& s, const QVector<_T>& v)
{
    ScopedArray _(s);
    for (const auto& vv : v)
        s << vv;
    return s;
}

template <class _T>
Stream& operator<<(Stream& s, const QList<_T>& v)
{
    ScopedArray _(s);
    for (const auto& vv : v)
        s << vv;
    return s;
}

template <class _K, class _V>
Stream& operator<<(Stream& s, const QMap<_K, _V>& v)
{
    ScopedArray _(s);
    for (auto i = v.keyValueBegin(); i != v.keyValueEnd(); ++i)
        s << i->first << i->second;
    return s;
}

}
