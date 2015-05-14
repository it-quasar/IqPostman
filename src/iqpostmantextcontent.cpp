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

#include "iqpostmantextcontent.h"
#include "iqpostmanabstractclient.h"
#include <QTextCodec>

IqPostmanTextContent::IqPostmanTextContent(QObject *parent) :
    IqPostmanAbstractContent(parent),
    m_contentType(new IqPostmanTextContentType(this))
{
}

IqPostmanTextContent::~IqPostmanTextContent()
{
}

IqPostmanTextContentType *IqPostmanTextContent::contentType() const
{
    return m_contentType;
}

bool IqPostmanTextContent::fromString(const QString &string)
{
    QString contentTypeString;
    QString contentTransferEncodingString;
    QString contentString;
    splitContent(string, &contentTypeString, &contentTransferEncodingString, &contentString);
    if (!m_contentType->fromString(contentTypeString))
        return false;
    setTransferEncoding(IqPostmanMime::contentTransferEncodingFromString(contentTransferEncodingString));

    QTextCodec *codec = QTextCodec::codecForName(m_contentType->charset().toLocal8Bit().constData());
    if (codec)
        setText(codec->toUnicode(decode(contentString, transferEncoding())));
    else
        setText(contentString);

    return true;
}

QString IqPostmanTextContent::toString() const
{
    QString result;
    result.append(m_contentType->toString());
    result.append(IqPostmanAbstractClient::crlf());
    result.append(IqPostmanMime::contentTransferEncodingToString(transferEncoding()));
    result.append(IqPostmanAbstractClient::crlf());

    result.append(encode(text().toLocal8Bit(), transferEncoding()));
    return result;
}

QString IqPostmanTextContent::text() const
{
    return m_text;
}

void IqPostmanTextContent::setText(const QString &text)
{
    if (m_text != text) {
        m_text = text;
        emit textChanged();
    }
}

