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

#include "iqpostmanmultipartcontent.h"
#include "iqpostmanabstractclient.h"
#include <QStringList>

IqPostmanMultipartContent::IqPostmanMultipartContent(QObject *parent) :
    IqPostmanAbstractContent(parent),
    m_contentType(new IqPostmanMultipartContentType(this))
{
}

IqPostmanMultipartContent::~IqPostmanMultipartContent()
{
}

IqPostmanMultipartContentType *IqPostmanMultipartContent::contentType() const
{
    return m_contentType;
}

bool IqPostmanMultipartContent::fromString(const QString &string)
{
    m_parts.clear();

    QString contentTypeString;
    QString contentTransferEncodingString;
    QString contentString;
    splitContent(string, &contentTypeString, &contentTransferEncodingString, &contentString);
    if (!m_contentType->fromString(contentTypeString))
        return false;
    setTransferEncoding(IqPostmanMime::contentTransferEncodingFromString(contentTransferEncodingString));

    QStringList parts = contentString.split("--" + m_contentType->boundary());
    foreach (const QString &part, parts) {
        IqPostmanAbstractContent *content = IqPostmanAbstractContent::createFromString(part);
        if (content) {
            content->setParent(this);
            m_parts.append(content);
        }
    }

    return true;
}

QString IqPostmanMultipartContent::toString() const
{
    Q_ASSERT(!m_contentType->boundary().isEmpty());

    QString result;
    result.append(m_contentType->toString());
    result.append(IqPostmanAbstractClient::crlf());
    result.append(IqPostmanMime::contentTransferEncodingToString(transferEncoding()));
    result.append(IqPostmanAbstractClient::crlf());

    foreach (const IqPostmanAbstractContent *part, m_parts) {
        result.append("--" + m_contentType->boundary() + IqPostmanAbstractClient::crlf());
        result.append(part->toString());
    }
    result.append("--" + m_contentType->boundary() + "--" + IqPostmanAbstractClient::crlf());

    return result;
}

QList<IqPostmanAbstractContent *> IqPostmanMultipartContent::parts() const
{
    return m_parts;
}

void IqPostmanMultipartContent::setParts(const QList<IqPostmanAbstractContent *> &parts)
{
    if (m_parts != parts) {
        foreach (IqPostmanAbstractContent *part, parts) {
            Q_CHECK_PTR(part);
            part->setParent(this);
            m_parts.removeOne(part);
        }
        qDeleteAll(m_parts);

        m_parts = parts;
        emit partsChanged();
    }
}
