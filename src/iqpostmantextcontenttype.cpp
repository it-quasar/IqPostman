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

#include "iqpostmantextcontenttype.h"
#include <QRegExp>

IqPostmanTextContentType::IqPostmanTextContentType(QObject *parent):
    IqPostmanAbstractContentType(parent),
    m_subType(UnknownSubType)
{
}

IqPostmanMime::ContentType IqPostmanTextContentType::type() const
{
    return IqPostmanMime::TypeText;
}

QString IqPostmanTextContentType::toString() const
{
    QString subTypeString;
    switch (subType()) {
    case Plain:
        subTypeString = "plain";
        break;
    case Richtext:
        subTypeString = "richtext";
        break;
    case Enriched:
        subTypeString = "enriched";
        break;
    case TabSeparatedValues:
        subTypeString = "tab-separated-values";
        break;
    case Html:
        subTypeString = "html";
        break;
    case UnknownSubType:
        return "";
        break;
    }

    if (!charset().isEmpty())
        return QString("Content-Type: text/%0; charset=%1")
                .arg(subTypeString)
                .arg(charset());

    return QString("Content-Type: text/%0")
            .arg(subTypeString);
}

bool IqPostmanTextContentType::fromString(const QString &string)
{
    QRegExp contetnTypeRx("^Content-Type: text\\/(\\w+)(;\\s*charset=\"?([^\"]+)\"?)?$", Qt::CaseInsensitive);
    if (contetnTypeRx.indexIn(string.trimmed()) == -1)
        return false;

    if (contetnTypeRx.cap(1).compare("plain", Qt::CaseInsensitive) == 0)
        setSubType(Plain);
    else if (contetnTypeRx.cap(1).compare("richtext", Qt::CaseInsensitive) == 0)
        setSubType(Richtext);
    else if (contetnTypeRx.cap(1).compare("enriched", Qt::CaseInsensitive) == 0)
        setSubType(Enriched);
    else if (contetnTypeRx.cap(1).compare("tab-separated-values", Qt::CaseInsensitive) == 0)
        setSubType(TabSeparatedValues);
    else if (contetnTypeRx.cap(1).compare("html", Qt::CaseInsensitive) == 0)
        setSubType(Html);
    else
        return false;

    setCharset(contetnTypeRx.cap(3));

    return true;
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

