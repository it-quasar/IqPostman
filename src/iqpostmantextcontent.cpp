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

bool IqPostmanTextContent::fromContentData(const IqPostmanContentData &data)
{
    QByteArray encodedData = decode(data.content, transferEncoding());

    QTextCodec *codec = QTextCodec::codecForName(contentType()->charset().toLocal8Bit().constData());
    if (codec)
        setText(codec->toUnicode(encodedData));
    else
        setText(encodedData);

    return true;
}

const IqPostmanContentData IqPostmanTextContent::toContentData() const
{
    IqPostmanContentData result;

    result.content = encode(text().toLocal8Bit(), transferEncoding());
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

