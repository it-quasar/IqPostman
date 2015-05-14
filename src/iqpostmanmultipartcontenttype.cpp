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

#include "iqpostmanmultipartcontenttype.h"
#include <QRegExp>

IqPostmanMultipartContentType::IqPostmanMultipartContentType(QObject *parent):
    IqPostmanAbstractContentType(parent),
    m_subType(UnknownSubType)
{
}

IqPostmanMime::ContentType IqPostmanMultipartContentType::type() const
{
    return IqPostmanMime::TypeMultipart;
}

bool IqPostmanMultipartContentType::fromString(const QString &string)
{
    QRegExp contetnTypeRx("^Content-Type: multipart\\/(\\w+);\\s*boundary=\"?([^\"]+)\"?$", Qt::CaseInsensitive);
    if (contetnTypeRx.indexIn(string.trimmed()) == -1)
        return false;

    if (contetnTypeRx.cap(1).compare("mixed", Qt::CaseInsensitive) == 0)
        setSubType(Mixed);
    else if (contetnTypeRx.cap(1).compare("alternative", Qt::CaseInsensitive) == 0)
        setSubType(Alternative);
    else if (contetnTypeRx.cap(1).compare("parallel", Qt::CaseInsensitive) == 0)
        setSubType(Parallel);
    else if (contetnTypeRx.cap(1).compare("digest", Qt::CaseInsensitive) == 0)
        setSubType(Digest);
    else
        return false;

    setBoundary(contetnTypeRx.cap(2));

    return true;
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



QString IqPostmanMultipartContentType::toString() const
{
    QString subTypeString;
    switch (subType()) {
    case Mixed:
        subTypeString = "mixed";
        break;
    case Alternative:
        subTypeString = "alternative";
        break;
    case Parallel:
        subTypeString = "parallel";
        break;
    case Digest:
        subTypeString = "digest";
        break;
    case UnknownSubType:
        return "";
        break;
    }

    return QString("Content-Type: multipart/%0; boundary=\"%1\"")
            .arg(subTypeString)
            .arg(boundary());
}
