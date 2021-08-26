#include "speedyj/speedyj-qt.hpp"

#include <QString>
#include <QStringList>
#include <QVariant>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonObject>

namespace speedyj
{

Stream& operator<<(Stream& s, const QVariant& v)
{
    if (!v.isValid() || v.isNull())
        return s << speedyj::Null;

    switch ((QMetaType::Type)v.type()) {
    case QMetaType::Bool:
        return s << v.value<bool>();
    case QMetaType::Int:
        return s << v.value<int>();
    case QMetaType::UInt:
        return s << v.value<unsigned int>();
    case QMetaType::Double:
        return s << v.value<double>();
    case QMetaType::QString:
        return s << v.value<QString>();
    case QMetaType::Long:
        return s << v.value<long>();
    case QMetaType::LongLong:
        return s << v.value<long long>();
    case QMetaType::Short:
        return s << v.value<short>();
    case QMetaType::Char:
        return s << v.value<char>();
    case QMetaType::ULong:
        return s << v.value<unsigned long>();
    case QMetaType::ULongLong:
        return s << v.value<unsigned long long>();
    case QMetaType::UShort:
        return s << v.value<unsigned short>();
    case QMetaType::SChar:
        return s << v.value<signed char>();
    case QMetaType::UChar:
        return s << v.value<unsigned char>();
    case QMetaType::Float:
        return s << v.value<float>();
    case QMetaType::QVariant:
        return s << v.value<QVariant>();
    case QMetaType::QStringList:
        return s << v.value<QStringList>();
    case QMetaType::QVariantList:
        return s << v.value<QVariantList>();
    case QMetaType::QVariantMap:
        return s << v.value<QVariantMap>();
    case QMetaType::QJsonValue:
        return s << v.value<QJsonValue>();
    case QMetaType::QJsonArray:
        return s << v.value<QJsonArray>();
    case QMetaType::QJsonObject:
        return s << v.value<QJsonObject>();
    default:
        throw Error("Unsupported QVariant meta-type");
    }
}

Stream& operator<<(Stream& s, const QString& v)
{
    return s << v.toStdString();
}

Stream& operator<<(Stream& s, const QStringList& v)
{
    ScopedArray _(s);
    for (const auto& vv : v)
        s << vv;
    return s;
}

Stream& operator<<(Stream& s, const QJsonValue& v)
{
    return s << v.toVariant();
}

Stream& operator<<(Stream& s, const QJsonArray& v)
{
    return s << v.toVariantList();
}

Stream& operator<<(Stream& s, const QJsonObject& v)
{
    return s << v.toVariantMap();
}

}
