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

#include "iqpostmanattachmentcontent.h"
#include "iqpostmanabstractclient.h"

IqPostmanAttachmentContent::IqPostmanAttachmentContent(QObject *parent) :
    IqPostmanAbstractContent(parent)
{
}

QString IqPostmanAttachmentContent::format() const
{
    return "";
}

bool IqPostmanAttachmentContent::fromContentData(const IqPostmanContentData &data)
{
    QByteArray encodedData = decode(data.content, transferEncoding());

    return setData(encodedData);
}

const IqPostmanContentData IqPostmanAttachmentContent::toContentData() const
{
    IqPostmanContentData result;

    result.content = encode(data(), transferEncoding());

    return result;
}

QByteArray IqPostmanAttachmentContent::data() const
{
    return m_data;
}

bool IqPostmanAttachmentContent::setData(const QByteArray &data)
{
    if (m_data != data) {
        m_data = data;
        emit dataChanged();
    }

    return true;
}


