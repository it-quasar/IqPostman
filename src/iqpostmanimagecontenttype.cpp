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

#include "iqpostmanimagecontenttype.h"
#include <QRegExp>
#include <QStringList>

QHash<IqPostmanImageContentType::SubType, QString> IqPostmanImageContentType::m_subTypeNames;
QHash<int, QString> IqPostmanImageContentType::m_subTypeIntNames;

IqPostmanImageContentType::IqPostmanImageContentType(QObject *parent):
    IqPostmanAbstractContentType(parent),
    m_subType(UnknownSubType)
{
    if (m_subTypeNames.isEmpty()) {
        m_subTypeNames[BmpSubType] = "bmp";
        m_subTypeNames[GifSubType] = "gif";
        m_subTypeNames[JpgSubType] = "jpg";
        m_subTypeNames[JpegSubType] = "jpeg";
        m_subTypeNames[PngSubType] = "png";
        m_subTypeNames[PbmSubType] = "pbm";
        m_subTypeNames[PgmSubType] = "pgm";
        m_subTypeNames[PpmSubType] = "ppm";
        m_subTypeNames[XbmSubType] = "xbm";
        m_subTypeNames[XpmSubType] = "xpm";
        m_subTypeNames[SvgSubType] = "svg";

        QHashIterator<SubType, QString> subTypeI(m_subTypeNames);
        while (subTypeI.hasNext()) {
            subTypeI.next();
            m_subTypeIntNames[subTypeI.key()] = subTypeI.value();
        }
    }
}

QString IqPostmanImageContentType::imageFormat() const
{
    return m_subTypeNames[subType()];
}

IqPostmanMime::ContentType IqPostmanImageContentType::type() const
{
    return IqPostmanMime::TypeImage;
}

QString IqPostmanImageContentType::typeName() const
{
    return staticTypeName();
}

QString IqPostmanImageContentType::staticTypeName()
{
    return QLatin1String("image");
}

IqPostmanImageContentType::SubType IqPostmanImageContentType::subType() const
{
    return m_subType;
}

void IqPostmanImageContentType::setSubType(const SubType &format)
{
    if (m_subType != format) {
        m_subType = format;
        emit subTypeChanged();
    }
}

int IqPostmanImageContentType::subTypeNumber() const
{
    return m_subType;
}

void IqPostmanImageContentType::setSubTypeFromNumber(int subTypeNumber)
{
    m_subType = static_cast<SubType>(subTypeNumber);
}

QHash<int, QString> IqPostmanImageContentType::subTypeNames() const
{
    return m_subTypeIntNames;
}

bool IqPostmanImageContentType::setData(const QHash<QString, QString> &data)
{
    Q_UNUSED(data);
    return true;
}

QHash<QString, QString> IqPostmanImageContentType::data() const
{
    QHash<QString, QString> result;

    return result;
}
