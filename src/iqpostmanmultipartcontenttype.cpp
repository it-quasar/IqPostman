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

#define BOUNDARY "boundary"

#include "iqpostmanmultipartcontenttype.h"
#include <QRegExp>
#include <QStringList>

QHash<IqPostmanMultipartContentType::SubType, QString> IqPostmanMultipartContentType::m_subTypeNames;
QHash<int, QString> IqPostmanMultipartContentType::m_subTypeIntNames;

IqPostmanMultipartContentType::IqPostmanMultipartContentType(QObject *parent):
    IqPostmanAbstractContentType(parent),
    m_subType(UnknownSubType)
{
    if (m_subTypeNames.isEmpty()) {
        m_subTypeNames[MixedSubType] = QLatin1String("mixed");
        m_subTypeNames[AlternativeSubType] = QLatin1String("alternative");
        m_subTypeNames[ParallelSubType] = QLatin1String("parallel");
        m_subTypeNames[DigestSubType] = QLatin1String("digest");
        m_subTypeNames[RelatedSubType] = QLatin1String("related");

        QHashIterator<SubType, QString> subTypeI(m_subTypeNames);
        while (subTypeI.hasNext()) {
            subTypeI.next();
            m_subTypeIntNames[subTypeI.key()] = subTypeI.value();
        }
    }
}

IqPostmanAbstractContentType::ContentType IqPostmanMultipartContentType::type() const
{
    return TypeMultipart;
}

QString IqPostmanMultipartContentType::typeName() const
{
    return staticTypeName();
}

QString IqPostmanMultipartContentType::staticTypeName()
{
    return QLatin1String("multipart");
}

IqPostmanMultipartContentType::SubType IqPostmanMultipartContentType::subType() const
{
    return m_subType;
}

void IqPostmanMultipartContentType::setSubType(const SubType &subType)
{
    if (m_subType != subType) {
        m_subType = subType;
        emit subTypeChanged();
    }
}

QString IqPostmanMultipartContentType::boundary() const
{
    return m_boundary;
}

void IqPostmanMultipartContentType::setBoundary(const QString &boundary)
{
    if (m_boundary != boundary) {
        m_boundary = boundary;
        emit boundaryChanged();
    }
}

int IqPostmanMultipartContentType::subTypeNumber() const
{
    return m_subType;
}

void IqPostmanMultipartContentType::setSubTypeFromNumber(int subTypeNumber)
{
    m_subType = static_cast<SubType>(subTypeNumber);
}

QHash<int, QString> IqPostmanMultipartContentType::subTypeNames() const
{
    return m_subTypeIntNames;
}

bool IqPostmanMultipartContentType::setData(const QHash<QString, QString> &data)
{
    setBoundary(data[BOUNDARY]);
    return true;
}

QHash<QString, QString> IqPostmanMultipartContentType::data() const
{
    QHash<QString, QString> result;

    if (!boundary().isEmpty())
        result[BOUNDARY] = boundary();

    return result;
}
