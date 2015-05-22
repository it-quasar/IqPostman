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

#define CHARSER "charset"

#include "iqpostmantextcontenttype.h"
#include <QRegExp>
#include <QStringList>

QHash<IqPostmanTextContentType::SubType, QString> IqPostmanTextContentType::m_subTypeNames;
QHash<int, QString> IqPostmanTextContentType::m_subTypeIntNames;

IqPostmanTextContentType::IqPostmanTextContentType(QObject *parent):
    IqPostmanAbstractContentType(parent),
    m_subType(UnknownSubType)
{
    if (m_subTypeNames.isEmpty()) {
        m_subTypeNames[PlainSubType] = QLatin1String("plain");
        m_subTypeNames[RichtextSubType] = QLatin1String("richtext");
        m_subTypeNames[EnrichedSubType] = QLatin1String("enriched");
        m_subTypeNames[TabSeparatedValuesSubType] = QLatin1String("tab-separated-values");
        m_subTypeNames[HtmlSubType] = QLatin1String("html");

        QHashIterator<SubType, QString> subTypeI(m_subTypeNames);
        while (subTypeI.hasNext()) {
            subTypeI.next();
            m_subTypeIntNames[subTypeI.key()] = subTypeI.value();
        }
    }
}

IqPostmanAbstractContentType::ContentType IqPostmanTextContentType::type() const
{
    return TypeText;
}

QString IqPostmanTextContentType::typeName() const
{
    return staticTypeName();
}

QString IqPostmanTextContentType::staticTypeName()
{
    return QLatin1String("text");
}

IqPostmanTextContentType::SubType IqPostmanTextContentType::subType() const
{
    return m_subType;
}

void IqPostmanTextContentType::setSubType(const SubType &subType)
{
    if (m_subType != subType) {
        m_subType = subType;
        emit subTypeChanged();
    }
}

QString IqPostmanTextContentType::charset() const
{
    return m_charset;
}

void IqPostmanTextContentType::setCharset(const QString &charset)
{
    if (m_charset != charset) {
        m_charset = charset;
        emit charsetChanged();
    }
}

int IqPostmanTextContentType::subTypeNumber() const
{
    return m_subType;
}

void IqPostmanTextContentType::setSubTypeFromNumber(int subTypeNumber)
{
    m_subType = static_cast<SubType>(subTypeNumber);
}

QHash<int, QString> IqPostmanTextContentType::subTypeNames() const
{
    return m_subTypeIntNames;
}

bool IqPostmanTextContentType::setData(const QHash<QString, QString> &data)
{
    setCharset(data[CHARSER]);
    return true;
}

QHash<QString, QString> IqPostmanTextContentType::data() const
{
    QHash<QString, QString> result;

    if (!charset().isEmpty())
        result[CHARSER] = charset();

    return result;
}
