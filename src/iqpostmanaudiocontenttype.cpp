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

#include "iqpostmanaudiocontenttype.h"
#include <QRegExp>
#include <QStringList>

QHash<IqPostmanAudioContentType::SubType, QString> IqPostmanAudioContentType::m_subTypeNames;
QHash<int, QString> IqPostmanAudioContentType::m_subTypeIntNames;

IqPostmanAudioContentType::IqPostmanAudioContentType(QObject *parent):
    IqPostmanAbstractContentType(parent),
    m_subType(UnknownSubType)
{
    if (m_subTypeNames.isEmpty()) {
        m_subTypeNames[BasicSubType] = "basic";
        m_subTypeNames[Mp3SubType] = "mp3";

        QHashIterator<SubType, QString> subTypeI(m_subTypeNames);
        while (subTypeI.hasNext()) {
            subTypeI.next();
            m_subTypeIntNames[subTypeI.key()] = subTypeI.value();
        }
    }
}

QString IqPostmanAudioContentType::imageFormat() const
{
    return m_subTypeNames[subType()];
}

IqPostmanAbstractContentType::ContentType IqPostmanAudioContentType::type() const
{
    return TypeAudio;
}

QString IqPostmanAudioContentType::typeName() const
{
    return staticTypeName();
}

QString IqPostmanAudioContentType::staticTypeName()
{
    return QLatin1String("audio");
}

IqPostmanAudioContentType::SubType IqPostmanAudioContentType::subType() const
{
    return m_subType;
}

void IqPostmanAudioContentType::setSubType(const SubType &format)
{
    if (m_subType != format) {
        m_subType = format;
        emit subTypeChanged();
    }
}

int IqPostmanAudioContentType::subTypeNumber() const
{
    return m_subType;
}

void IqPostmanAudioContentType::setSubTypeFromNumber(int subTypeNumber)
{
    m_subType = static_cast<SubType>(subTypeNumber);
}

QHash<int, QString> IqPostmanAudioContentType::subTypeNames() const
{
    return m_subTypeIntNames;
}

bool IqPostmanAudioContentType::setData(const QHash<QString, QString> &data)
{
    Q_UNUSED(data);
    return true;
}

QHash<QString, QString> IqPostmanAudioContentType::data() const
{
    QHash<QString, QString> result;

    return result;
}
