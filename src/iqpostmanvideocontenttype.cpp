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

#include "iqpostmanvideocontenttype.h"
#include <QRegExp>
#include <QStringList>

QHash<IqPostmanVideoContentType::SubType, QString> IqPostmanVideoContentType::m_subTypeNames;
QHash<int, QString> IqPostmanVideoContentType::m_subTypeIntNames;

IqPostmanVideoContentType::IqPostmanVideoContentType(QObject *parent):
    IqPostmanAbstractContentType(parent),
    m_subType(UnknownSubType)
{
    if (m_subTypeNames.isEmpty()) {
        m_subTypeNames[MpegSubType] = "mpeg";
        m_subTypeNames[QuickTimeSubType] = "quicktime";

        QHashIterator<SubType, QString> subTypeI(m_subTypeNames);
        while (subTypeI.hasNext()) {
            subTypeI.next();
            m_subTypeIntNames[subTypeI.key()] = subTypeI.value();
        }
    }
}

QString IqPostmanVideoContentType::imageFormat() const
{
    return m_subTypeNames[subType()];
}

IqPostmanMime::ContentType IqPostmanVideoContentType::type() const
{
    return IqPostmanMime::TypeImage;
}

QString IqPostmanVideoContentType::typeName() const
{
    return staticTypeName();
}

QString IqPostmanVideoContentType::staticTypeName()
{
    return QLatin1String("video");
}

IqPostmanVideoContentType::SubType IqPostmanVideoContentType::subType() const
{
    return m_subType;
}

void IqPostmanVideoContentType::setSubType(const SubType &format)
{
    if (m_subType != format) {
        m_subType = format;
        emit subTypeChanged();
    }
}

int IqPostmanVideoContentType::subTypeNumber() const
{
    return m_subType;
}

void IqPostmanVideoContentType::setSubTypeFromNumber(int subTypeNumber)
{
    m_subType = static_cast<SubType>(subTypeNumber);
}

QHash<int, QString> IqPostmanVideoContentType::subTypeNames() const
{
    return m_subTypeIntNames;
}

bool IqPostmanVideoContentType::setData(const QHash<QString, QString> &data)
{
    Q_UNUSED(data);
    return true;
}

QHash<QString, QString> IqPostmanVideoContentType::data() const
{
    QHash<QString, QString> result;

    return result;
}
