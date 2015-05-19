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

#include "iqpostmanapplicationcontenttype.h"
#include <QRegExp>
#include <QStringList>

QHash<IqPostmanApplicationContentType::SubType, QString> IqPostmanApplicationContentType::m_subTypeNames;
QHash<int, QString> IqPostmanApplicationContentType::m_subTypeIntNames;

IqPostmanApplicationContentType::IqPostmanApplicationContentType(QObject *parent):
    IqPostmanAbstractContentType(parent),
    m_subType(UnknownSubType)
{
    if (m_subTypeNames.isEmpty()) {
        m_subTypeNames[OctetStreamSubType] = "octet-stream";
        m_subTypeNames[PostScriptSubType] = "postscript";
        m_subTypeNames[RtfSubType] = "rtf";
        m_subTypeNames[PdfSubType] = "pdf";
        m_subTypeNames[MswordSubType] = "msword";

        QHashIterator<SubType, QString> subTypeI(m_subTypeNames);
        while (subTypeI.hasNext()) {
            subTypeI.next();
            m_subTypeIntNames[subTypeI.key()] = subTypeI.value();
        }
    }
}

QString IqPostmanApplicationContentType::imageFormat() const
{
    return m_subTypeNames[subType()];
}

IqPostmanMime::ContentType IqPostmanApplicationContentType::type() const
{
    return IqPostmanMime::TypeImage;
}

QString IqPostmanApplicationContentType::typeName() const
{
    return staticTypeName();
}

QString IqPostmanApplicationContentType::staticTypeName()
{
    return QLatin1String("image");
}

IqPostmanApplicationContentType::SubType IqPostmanApplicationContentType::subType() const
{
    return m_subType;
}

void IqPostmanApplicationContentType::setSubType(const SubType &format)
{
    if (m_subType != format) {
        m_subType = format;
        emit subTypeChanged();
    }
}

int IqPostmanApplicationContentType::subTypeNumber() const
{
    return m_subType;
}

void IqPostmanApplicationContentType::setSubTypeFromNumber(int subTypeNumber)
{
    m_subType = static_cast<SubType>(subTypeNumber);
}

QHash<int, QString> IqPostmanApplicationContentType::subTypeNames() const
{
    return m_subTypeIntNames;
}

bool IqPostmanApplicationContentType::setData(const QHash<QString, QString> &data)
{
    Q_UNUSED(data);
    return true;
}

QHash<QString, QString> IqPostmanApplicationContentType::data() const
{
    QHash<QString, QString> result;

    return result;
}
