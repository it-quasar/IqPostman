/**********************************************************************************
 * Copyright © 2015 Pavel A. Puchkov                                              *
 *                                                                                *
 * This file is part of IqPostman.                                                *
 *                                                                                *
 * IqPostman is free software: you can redistribute it and/or modify              *
 * it under the terms of the GNU Lesser General Public License as published by    *
 * the Free Software Foundation, either version 3 of the License, or              *
 * (at your option) any later version.                                            *
 *                                                                                *
 * IqPostman is distributed in the hope that it will be useful,                   *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of                 *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                  *
 * GNU Lesser General Public License for more details.                            *
 *                                                                                *
 * You should have received a copy of the GNU Lesser General Public License       *
 * along with IqPostman.  If not, see <http://www.gnu.org/licenses/>.             *
 **********************************************************************************/

#include "iqpostmanabstractcontenttype.h"

#include "iqpostmantextcontenttype.h"
#include "iqpostmanmultipartcontenttype.h"

#include <QRegExp>
#include <QStringList>

IqPostmanAbstractContentType::IqPostmanAbstractContentType(QObject *parent) :
    QObject(parent)
{
}

IqPostmanAbstractContentType::~IqPostmanAbstractContentType()
{
}

bool IqPostmanAbstractContentType::fromString(const QString &string)
{
    QRegExp contentTypeRx(QString("^Content-Type: %0\\/(\\w+)")
                          .arg(typeName()), Qt::CaseInsensitive);
    if (contentTypeRx.indexIn(string.trimmed()) == -1)
        return false;

    QString contentSubType = contentTypeRx.cap(1);

    bool find = false;
    QHashIterator<int, QString> subTypeI(subTypeNames());
    while (subTypeI.hasNext()) {
        subTypeI.next();

        if (contentSubType.compare(subTypeI.value(), Qt::CaseInsensitive) == 0) {
            setSubTypeFromNumber(subTypeI.key());
            find = true;
            break;
        }
    }
    if (!find)
        return false;

    QHash<QString, QString> data = parseData(string);
    if (!setData(data))
        return false;

    return true;
}

QString IqPostmanAbstractContentType::toString() const
{
    if (subTypeNumber() == 0)
        return "";
    QString subTypeString = subTypeNames()[subTypeNumber()];

    QStringList resultList;

    resultList << QString("Content-Type: %0/%1")
                  .arg(typeName())
                  .arg(subTypeString);

    QHashIterator<QString, QString> dataI(data());
    while (dataI.hasNext()) {
        dataI.next();
        resultList << QString("%0=%1")
                      .arg(dataI.key())
                      .arg(dataI.value());
    }

    return resultList.join("; ");
}

IqPostmanAbstractContentType *IqPostmanAbstractContentType::createFromString(const QString &string)
{
    switch (IqPostmanMime::contentTypeFromString(string)) {
    case IqPostmanMime::TypeText: {
        IqPostmanTextContentType *result = new IqPostmanTextContentType();
        if (result->fromString(string))
            return result;
        delete result;
        break;
    }
    case IqPostmanMime::TypeMultipart: {
        IqPostmanMultipartContentType *result = new IqPostmanMultipartContentType();
        if (result->fromString(string))
            return result;
        delete result;
        break;
    }
    case IqPostmanMime::TypeUnknown:
        break;
    }

    return Q_NULLPTR;
}

QHash<QString, QString> IqPostmanAbstractContentType::parseData(const QString &string) const
{
    QRegExp contentDataRx(QString("^Content-Type: %0\\/\\w+; (.+)")
                          .arg(typeName()), Qt::CaseInsensitive);
    if (contentDataRx.indexIn(string.trimmed()) == -1)
        return QHash<QString, QString>();

    QStringList dataList = contentDataRx.cap(1).split(';');
    QHash<QString, QString> result;
    foreach (const QString &dataStr, dataList) {
        QStringList paramList = dataStr.trimmed().split("=");
        if (paramList.count() == 2)
            result[paramList.at(0)] = paramList.at(1);
    }

    return result;
}
